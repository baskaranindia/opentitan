#!/usr/bin/env python3
# Copyright lowRISC contributors (OpenTitan project).
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
"""make_new_dif.py is a script for instantiating templates needed to begin
development on a new DIF.

To instantiate the files for a new IP named ip_ctrl, run the command:

$ util/make_new_dif.py -i /path/to/ip.hjson --ip-name-snake "ip_ctrl" --ip-name-long "IP Controller"

where "IP Controller" is a documentation-friendly name for the IP.
For example, compare "pwrmgr" and "Power Manager".

It will instantiate:
- `util/make_new_dif/templates/dif_template.h.tpl` as the DIF Header boilerplate
  into `dif_<ip>.h`, which should be manually edited/enhanced.
- `doc/project_governance/checklist/sw_checklist.md.tpl` as the DIF Checklist
  into dif_<ip>.md,  which should be manually edited.

Each one of these templates has comments with more specific information.

You can also use the `--only=header`, `--only=checklist` to
instantiate a subset of the templates. This can be passed multiple times, and
including `--only=all` will instantiate every template.

Note: the files will still need some cleaning up before they can be used.
"""

import argparse
import logging
import subprocess
import sys
from pathlib import Path

from make_new_dif.ip import Ip
from mako.template import Template

# This file is $REPO_TOP/util/make_new_dif.py, so it takes two parent()
# calls to get back to the top.
REPO_TOP = Path(__file__).resolve().parent.parent

ALL_PARTS = ["header", "checklist"]


def main():
    dif_dir = REPO_TOP / "sw/device/lib/dif"
    autogen_dif_dir = dif_dir / "autogen"

    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)

    parser.add_argument("--ipcfg",
                        "-i",
                        type=Path,
                        required=True,
                        help="`<ip>.hjson` file.")
    parser.add_argument("--ip-name-snake",
                        "-s",
                        required=True,
                        help="the short name of the IP, in snake_case.")
    parser.add_argument("--ip-name-long",
                        "-l",
                        required=True,
                        help="the documentation-friendly name of the IP.")
    parser.add_argument("--only",
                        choices=ALL_PARTS,
                        default=[],
                        action="append",
                        help="only create some files; defaults to all.")
    args = parser.parse_args()

    ip = Ip(args.ip_name_snake, args.ip_name_long, args.ipcfg)

    # Default to generating all parts.
    if len(args.only) == 0:
        args.only += ALL_PARTS

    # Create output directories if needed.
    if len(args.only) > 0:
        dif_dir.mkdir(exist_ok=True)
        autogen_dif_dir.mkdir(exist_ok=True)

    # Render DIF templates.
    template_path = REPO_TOP / "util/make_new_dif/templates"

    if "header" in args.only:
        header_template_file = template_path / "dif_template.h.tpl"
        header_out_file = dif_dir / "dif_{}.h".format(ip.name_snake)
        if header_out_file.is_file():
            raise FileExistsError(
                "DIF header already exists for the IP. To overwrite, "
                "delete existing header and try again.")
        header_template = Template(header_template_file.read_text())
        header_out_file.write_text(header_template.render(ip=ip))
        logging.info("DIF header successfully written to {}.".format(
            str(header_out_file)))

        # Format autogenerated file with clang-format.
        try:
            subprocess.check_call(
                ["./bazelisk.sh", "run", "//quality:clang_format_fix", "--", header_out_file])
        except subprocess.CalledProcessError:
            logging.error(f"failed to format {header_out_file} with clang-format.")
            sys.exit(1)

    if "checklist" in args.only:
        checklist_template_file = (
            REPO_TOP /
            "doc/project_governance/checklist/sw_checklist.md.tpl")
        checklist_out_file = dif_dir / "dif_{}.md".format(ip.name_snake)
        if checklist_out_file.is_file():
            raise FileExistsError(
                "DIF checklist already exists for the IP. To "
                "overwrite, delete existing checklist and try again.")
        markdown_template = Template(checklist_template_file.read_text())
        checklist_out_file.write_text(markdown_template.render(ip=ip))
        logging.info("DIF Checklist successfully written to {}.".format(
            str(checklist_out_file)))


if __name__ == "__main__":
    main()
