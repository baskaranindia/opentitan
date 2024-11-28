// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#include "devicetables.h"
#include "sw/device/lib/arch/device.h"
#include "sw/device/lib/base/mmio.h"
#include "sw/device/lib/dif/dif_gpio.h"
#include "sw/device/lib/dif/dif_pinmux.h"
#include "sw/device/lib/dif/dif_rv_plic.h"
#include "sw/device/lib/runtime/hart.h"
#include "sw/device/lib/runtime/irq.h"
#include "sw/device/lib/runtime/log.h"
#include "sw/device/lib/testing/pinmux_testutils.h"
#include "sw/device/lib/testing/rv_plic_testutils.h"
#include "sw/device/lib/testing/test_framework/check.h"
#include "sw/device/lib/testing/test_framework/ottf_main.h"

enum {
  kPlicTarget = 0,
};

// Assume that the pins in dt_gpio_pin_t are numbered 0, 1, and so on.
static_assert(kDtGpioPinGpio0 == 0, "kDtGpioPinGpio0 is expected to be 0");
static_assert(kDtGpioPinGpio1 == 1, "kDtGpioPinGpio1 is expected to be 1");
// Assume that the IRQs in dt_gpio_irq_t are numbered 0, 1, and so on.
static_assert(kDtGpioIrqGpio0 == 0, "kDtGpioIrqGpio0 is expected to be 0");
static_assert(kDtGpioIrqGpio1 == 1, "kDtGpioIrqGpio1 is expected to be 1");
static_assert(kDtGpioPinCount == kDifGpioNumPins,
              "kDtGpioPinCount does not match kDifGpioNumPins");

static dif_gpio_t gpio;
static dif_pinmux_t pinmux;
static dif_rv_plic_t plic;
static const dt_gpio_t *kGpioDt = &kDtGpio[0];

// These indicate the GPIO pin irq expected to fire, declared volatile since
// they are used by the ISR.
static volatile uint32_t expected_gpio_pin_irq;
static volatile bool expected_irq_edge;

/**
 * GPIO test - verifies the GPIO pins in the input and output directions.
 *
 * In the output direction, SW writes the following pattern:
 * 1. Start with GPIOs = all zeros
 * 2. Walk a 1 through ALL GPIOs (presented by the IP), read `data_in` with each
 *    write to ensure correctness
 * 3. Set all GPIOs to 0s, followed by all 1s.
 * 4. Walk a 0 through ALL GPIOs (presented by the IP), read `data_in` with each
 *    write to ensure correctness
 * 5. Set all GPIOs to 1s, followed by all 0s.
 *
 * The correctness of the GPIO values on the chip pins is verified by the
 * external testbench. The correctness of `data_in` is limited to the number of
 * GPIOs exposed by the chip, so we mask the written value accordingly.
 *
 * In the input direction, the external testbench sends the following pattern:
 * 1. Walk a 1 in 'temperature' pattern (0001, 0011, 0111, 1111, 1110, 100, ..)
 *
 * Both, rising and falling edges are configured for generating an interrupt. As
 * each pin rises or falls, the SW checks the interrupt, status and `data_in`
 * for correctness.
 */

/**
 * Runs the GPIO output test.
 *
 * Walks a 1 over the GPIO pins, followed by walking a 0.
 * The external testbench checks the GPIO values for correctness.
 */
static void gpio_output_test(const dif_gpio_t *gpio, uint32_t mask) {
  LOG_INFO("Starting GPIO output test");

  // Set the GPIOs to be in output mode.
  CHECK_DIF_OK(dif_gpio_output_set_enabled_all(gpio, mask));

  // Walk 1s - 0001, 0010, 0100, 1000, etc.
  for (uint32_t i = 0; i < kDifGpioNumPins; ++i) {
    uint32_t gpio_val = 1 << i;
    CHECK_DIF_OK(dif_gpio_write_all(gpio, gpio_val));

    // Read GPIO_IN to confirm what we wrote.
    uint32_t read_val;
    CHECK_DIF_OK(dif_gpio_read_all(gpio, &read_val));

    // Check written and read val for correctness.
    CHECK(gpio_val == read_val, "GPIOs mismatched (written = %x, read = %x)",
          gpio_val, read_val);
  }

  // Write all 0s to the GPIOs.
  CHECK_DIF_OK(dif_gpio_write_all(gpio, ~mask));

  // Write all 1s to the GPIOs.
  CHECK_DIF_OK(dif_gpio_write_all(gpio, mask));

  // Now walk 0s - 1110, 1101, 1011, 0111, etc.
  for (uint32_t i = 0; i < kDifGpioNumPins; ++i) {
    uint32_t gpio_val = ~(1 << i);
    CHECK_DIF_OK(dif_gpio_write_all(gpio, gpio_val));

    // Read GPIO_IN to confirm what we wrote.
    uint32_t read_val;
    CHECK_DIF_OK(dif_gpio_read_all(gpio, &read_val));

    // Check written and read val for correctness.
    CHECK(gpio_val == read_val, "GPIOs mismatched (written = %x, read = %x)",
          gpio_val, read_val);
  }

  // Write all 1s to the GPIOs.
  CHECK_DIF_OK(dif_gpio_write_all(gpio, mask));

  // Write all 0s to the GPIOs.
  CHECK_DIF_OK(dif_gpio_write_all(gpio, ~mask));
}

/**
 * Runs the GPIO input test.
 *
 * We start off with all 0s. The function polls the GPIOs for a 'thermometer
 * code' pattern (0, 1, 11, 111 etc) which is driven by the testbench, through
 * interrupts. The rising edge of each subsequent pin causes an interrupt to
 * fire. The ISR checks for the right GPIO and polarity. The testbench then
 * reverses the thermometer pattern (1111, 1110, 1100, 1000, etc).to capture the
 * interrupt on the falling edge.
 */
static void gpio_input_test(const dif_gpio_t *gpio, uint32_t mask) {
  LOG_INFO("Starting GPIO input test");

  // Enable the noise filter on all GPIOs.
  CHECK_DIF_OK(
      dif_gpio_input_noise_filter_set_enabled(gpio, mask, kDifToggleEnabled));

  // Configure all GPIOs to be rising and falling edge interrupts.
  CHECK_DIF_OK(dif_gpio_irq_set_trigger(gpio, mask,
                                        kDifGpioIrqTriggerEdgeRisingFalling));

  // Enable interrupts on all GPIOs.
  CHECK_DIF_OK(dif_gpio_irq_restore_all(gpio, &mask));

  // Set the GPIOs to be in input mode.
  CHECK_DIF_OK(dif_gpio_output_set_enabled_all(gpio, 0u));

  // Wait for rising edge interrupt on each pin.
  expected_irq_edge = true;
  for (expected_gpio_pin_irq = 0; expected_gpio_pin_irq < kDifGpioNumPins;
       ++expected_gpio_pin_irq) {
    wait_for_interrupt();
  }
  uint32_t read_val;
  uint32_t gpio_exp_val;

  gpio_exp_val = mask;
  CHECK_DIF_OK(dif_gpio_read_all(gpio, &read_val));
  CHECK(gpio_exp_val == read_val,
        "GPIOs mismatched (expected = %x, actual = %x)", gpio_exp_val,
        read_val);

  // Wait for falling edge interrupt on each pin.
  expected_irq_edge = false;
  for (expected_gpio_pin_irq = 0; expected_gpio_pin_irq < kDifGpioNumPins;
       ++expected_gpio_pin_irq) {
    wait_for_interrupt();
  }

  gpio_exp_val = ~mask;
  CHECK_DIF_OK(dif_gpio_read_all(gpio, &read_val));
  CHECK(gpio_exp_val == read_val,
        "GPIOs mismatched (expected = %x, actual = %x)", gpio_exp_val,
        read_val);
}

/**
 * Provides external irq handling for this test.
 *
 * This function overrides the default OTTF external ISR.
 */
void ottf_external_isr(uint32_t *exc_info) {
  // Find which interrupt fired at PLIC by claiming it.
  dif_rv_plic_irq_id_t plic_irq_id;
  CHECK_DIF_OK(dif_rv_plic_irq_claim(&plic, kPlicTarget, &plic_irq_id));

  // Check if it is the right peripheral.
  dt_gpio_irq_t gpio_pin_irq = dt_gpio_irq_from_plic_id(kGpioDt, plic_irq_id);
  CHECK(gpio_pin_irq != kDtGpioIrqCount, "Interrupt %d does not come from GPIO",
        plic_irq_id);

  // Here we assume that the IRQs are numbered 0, 1, so that they correspond to
  // GPIO numbers.
  uint32_t gpio_pin_irq_fired = gpio_pin_irq;

  // Check if we did expect the right GPIO IRQ to fire.
  CHECK(gpio_pin_irq_fired == expected_gpio_pin_irq,
        "Incorrect GPIO interrupt (exp: %d, obs: %d)", expected_gpio_pin_irq,
        gpio_pin_irq_fired);

  // Check if the same interrupt fired at GPIO as well.
  uint32_t gpio_irqs_status;
  CHECK_DIF_OK(dif_gpio_irq_get_state(&gpio, &gpio_irqs_status));
  CHECK(gpio_irqs_status == (1 << expected_gpio_pin_irq),
        "Incorrect GPIO irqs status {exp: %x, obs: %x}",
        (1 << expected_gpio_pin_irq), gpio_irqs_status);

  // Read the gpio pin value to ensure the right value is being reflected.
  bool pin_val;
  CHECK_DIF_OK(dif_gpio_read(&gpio, expected_gpio_pin_irq, &pin_val));

  // Check if the pin value is set correctly.
  CHECK(pin_val == expected_irq_edge, "Incorrect GPIO %d pin value (exp: %b)",
        expected_gpio_pin_irq, expected_irq_edge);

  // Clear the interrupt at GPIO.
  CHECK_DIF_OK(dif_gpio_irq_acknowledge(&gpio, gpio_pin_irq_fired));

  // Complete the IRQ at PLIC.
  CHECK_DIF_OK(dif_rv_plic_irq_complete(&plic, kPlicTarget, plic_irq_id));
}

OTTF_DEFINE_TEST_CONFIG();

void configure_pinmux(void) {
  for (size_t i = 0; i < kDifGpioNumPins; ++i) {
    dt_pad_t pad = kDtPad[kPinmuxTestutilsGpioPads[i]];
    // Assume that the pins in dt_gpio_pin_t are numbered 0, 1, and so on.
    dt_gpio_pin_t gpio_pin = (dt_gpio_pin_t)i;
    dt_pin_t pin = dt_gpio_pin(kGpioDt, gpio_pin);
    CHECK_DIF_OK(dif_pinmux_mio_select_output(&pinmux, pad, pin));
    CHECK_DIF_OK(dif_pinmux_mio_select_input(&pinmux, pin, pad));
  }
}

bool test_main(void) {
  // Initialize the pinmux.
  CHECK_DIF_OK(dif_pinmux_init_from_dt(&kDtPinmux[0], &pinmux));
  pinmux_testutils_init(&pinmux);
  configure_pinmux();

  // Initialize the GPIO.
  CHECK_DIF_OK(dif_gpio_init_from_dt(kGpioDt, &gpio));

  // Initialize the PLIC.
  // Here we assume that the IRQs are numbered 0, 1, so that they correspond to
  // GPIO numbers.
  CHECK_DIF_OK(dif_rv_plic_init_from_dt(&kDtRvPlic[0], &plic));
  rv_plic_testutils_irq_range_enable(
      &plic, kPlicTarget, dt_gpio_irq_to_plic_id(kGpioDt, kDtGpioIrqGpio0),
      dt_gpio_irq_to_plic_id(kGpioDt, kDtGpioIrqGpio0 + kDifGpioNumPins - 1));

  // Enable the external IRQ at Ibex.
  irq_global_ctrl(true);
  irq_external_ctrl(true);

  // Run the tests.
  uint32_t gpio_mask = pinmux_testutils_get_testable_gpios_mask();
  gpio_output_test(&gpio, gpio_mask);
  gpio_input_test(&gpio, gpio_mask);

  return true;
}
