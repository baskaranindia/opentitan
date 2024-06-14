/**
 * acorn.h: dynamic library interface for acorn.
 * @file
 * Copyright 2015-2024 Entrust, Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

 * Generated by interface.js -- DO NOT EDIT */
#ifndef ACORN_H
#define ACORN_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
# define EXPORT __declspec(dllexport) __cdecl
#else
# define EXPORT
#endif

#include <stdio.h>

/**
 * Automatically called by dynamic library initialization. */
int EXPORT
acorn_init(void);

/**
 * Automatically called by dynamic library cleanup. */
void EXPORT
acorn_cleanup(void);

void EXPORT
acorn_errstream(FILE *out);

/** A byte array with length included */
struct acorn_buffer {
  /** Number of bytes referenced by ptr */
  unsigned len;
  /** Pointer to an array of bytes */
  unsigned char *ptr;
};

enum acorn_enum_generateFlags {
  acorn_enum_generateFlags_overwrite = 0,
  acorn_enum_generateFlags_exportPrivate = 1,
  acorn_enum_generateFlags__LIMIT = 2
};

/**
 * Describes a key pair */
struct acorn_object_keyEntry {
  /**
   * User defined name for key pair */
  char *alias;
    /**
   * Cryptographic algorithm with which this key pair can be used.
   * For example, this might be `dilitium2` */
  char *algorithm;
  };

/**
 * Input parameters for acorn_command_getVersion.
 * @see acorn_command_getVersion
 * @see acorn_response_getVersion */
struct acorn_request_getVersion {
  /** Some C compilers reject empty structs */
  int unused;
};

/**
 * Output parameters for acorn_command_getVersion.
 * @see acorn_command_getVersion
 * @see acorn_request_getVersion */
struct acorn_response_getVersion {
  /**
   * Semantic version number for host software. For example,
   * "0.1.2" All versions with the same initial number should be
   * compatible at runtime. */
  char *version;
};

typedef int (*acorn_commandfn_getVersion)
(const struct acorn_request_getVersion *request,
 struct acorn_response_getVersion *response,
 char **errstring);

/**
 * Returns information about the host software.
 *
 * @param request input data to process
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 *               and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_command_getVersion
(const struct acorn_request_getVersion *request,
 struct acorn_response_getVersion *response,
 char **errstring);

typedef int (*acorn_cmdfn_getVersion)
( struct acorn_response_getVersion *response,
 char **errstring);

/**
 * Returns information about the host software.
 *
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 * and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_cmd_getVersion
(struct acorn_response_getVersion *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_getVersion
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_getVersion
(struct acorn_request_getVersion *request,
 struct acorn_response_getVersion *response);

/**
 * Input parameters for acorn_command_list.
 * @see acorn_command_list
 * @see acorn_response_list */
struct acorn_request_list {
  /** Some C compilers reject empty structs */
  int unused;
};

/**
 * Output parameters for acorn_command_list.
 * @see acorn_command_list
 * @see acorn_request_list */
struct acorn_response_list {
  /** Number of entries in entries */
unsigned n_entries;
  /**
   * Description of each key saved in the system */
  struct acorn_object_keyEntry *entries;
};

typedef int (*acorn_commandfn_list)
(const struct acorn_request_list *request,
 struct acorn_response_list *response,
 char **errstring);

/**
 * Returns a list of available key aliases with algorithm names.
 *
 * @param request input data to process
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 *               and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_command_list
(const struct acorn_request_list *request,
 struct acorn_response_list *response,
 char **errstring);

typedef int (*acorn_cmdfn_list)
( struct acorn_response_list *response,
 char **errstring);

/**
 * Returns a list of available key aliases with algorithm names.
 *
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 * and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_cmd_list
(struct acorn_response_list *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_list
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_list
(struct acorn_request_list *request,
 struct acorn_response_list *response);

/**
 * Input parameters for acorn_command_getPublic.
 * @see acorn_command_getPublic
 * @see acorn_response_getPublic */
struct acorn_request_getPublic {
  /**
   * Specifies which saved key pair for which to fetch the public
   * component */
  char *alias;
  };

/**
 * Output parameters for acorn_command_getPublic.
 * @see acorn_command_getPublic
 * @see acorn_request_getPublic */
struct acorn_response_getPublic {
  /**
   * Public key material */
  struct acorn_buffer publicKey;
};

typedef int (*acorn_commandfn_getPublic)
(const struct acorn_request_getPublic *request,
 struct acorn_response_getPublic *response,
 char **errstring);

/**
 * Returns the public key associated with a given alias
 *
 * @param request input data to process
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 *               and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_command_getPublic
(const struct acorn_request_getPublic *request,
 struct acorn_response_getPublic *response,
 char **errstring);

typedef int (*acorn_cmdfn_getPublic)
(const char *alias,
  struct acorn_response_getPublic *response,
 char **errstring);

/**
 * Returns the public key associated with a given alias
 *
 * @param alias Specifies which saved key pair for which to fetch the
 * public component
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 * and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_cmd_getPublic
(const char *alias,
 struct acorn_response_getPublic *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_getPublic
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_getPublic
(struct acorn_request_getPublic *request,
 struct acorn_response_getPublic *response);

/**
 * Input parameters for acorn_command_getPublicHash.
 * @see acorn_command_getPublicHash
 * @see acorn_response_getPublicHash */
struct acorn_request_getPublicHash {
  /**
   * Public key material */
  struct acorn_buffer publicKey;
  };

/**
 * Output parameters for acorn_command_getPublicHash.
 * @see acorn_command_getPublicHash
 * @see acorn_request_getPublicHash */
struct acorn_response_getPublicHash {
  /**
   * Uniquely idenfies a public key */
  char *hash;
};

typedef int (*acorn_commandfn_getPublicHash)
(const struct acorn_request_getPublicHash *request,
 struct acorn_response_getPublicHash *response,
 char **errstring);

/**
 * Returns a hash value for a given public key.
 *
 * @param request input data to process
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 *               and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_command_getPublicHash
(const struct acorn_request_getPublicHash *request,
 struct acorn_response_getPublicHash *response,
 char **errstring);

typedef int (*acorn_cmdfn_getPublicHash)
(const struct acorn_buffer publicKey,
  struct acorn_response_getPublicHash *response,
 char **errstring);

/**
 * Returns a hash value for a given public key.
 *
 * @param publicKey Public key material
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 * and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_cmd_getPublicHash
(const struct acorn_buffer publicKey,
 struct acorn_response_getPublicHash *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_getPublicHash
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_getPublicHash
(struct acorn_request_getPublicHash *request,
 struct acorn_response_getPublicHash *response);

/**
 * Input parameters for acorn_command_getKeyInfo.
 * @see acorn_command_getKeyInfo
 * @see acorn_response_getKeyInfo */
struct acorn_request_getKeyInfo {
  /**
   * Specifies which key pair to describe */
  char *alias;
  };

/**
 * Output parameters for acorn_command_getKeyInfo.
 * @see acorn_command_getKeyInfo
 * @see acorn_request_getKeyInfo */
struct acorn_response_getKeyInfo {
  /**
   * Uniquely idenfies a public key */
  char *hash;
  /**
   * Cryptographic algorithm with which this key pair can be used */
  char *algorithm;
  /**
   * Public key material */
  struct acorn_buffer publicKey;
  /**
   * Opaque and encrypted private key */
  struct acorn_buffer privateBlob;
};

typedef int (*acorn_commandfn_getKeyInfo)
(const struct acorn_request_getKeyInfo *request,
 struct acorn_response_getKeyInfo *response,
 char **errstring);

/**
 * Returns the hash, algorithm and public key associated with a given
 * alias.
 *
 * @param request input data to process
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 *               and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_command_getKeyInfo
(const struct acorn_request_getKeyInfo *request,
 struct acorn_response_getKeyInfo *response,
 char **errstring);

typedef int (*acorn_cmdfn_getKeyInfo)
(const char *alias,
  struct acorn_response_getKeyInfo *response,
 char **errstring);

/**
 * Returns the hash, algorithm and public key associated with a given
 * alias.
 *
 * @param alias Specifies which key pair to describe
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 * and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_cmd_getKeyInfo
(const char *alias,
 struct acorn_response_getKeyInfo *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_getKeyInfo
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_getKeyInfo
(struct acorn_request_getKeyInfo *request,
 struct acorn_response_getKeyInfo *response);

/**
 * Input parameters for acorn_command_generate.
 * @see acorn_command_generate
 * @see acorn_response_generate */
struct acorn_request_generate {
  /**
   * Name for new key pair (optional) */
  char *alias;
    /**
   * Cryptographic algorithm to use */
  char *algorithm;
    /**
   * Specifies the operator card set or softoken to use. Use the
   * preload command to avoid console prompts */
  char *token;
    /**
   * Set bit one to overwrite existing alias key pair if one
   * exists.
 *
   * Set bit two to provide private key without encryption in the
   * response privateKey field (DANGEROUS). Setting this
   * undermines the ability of the HSM to protect the key
   * material, though the key still benefits from hardware random
   * number generation. */
  unsigned flags;
  };

/**
 * Output parameters for acorn_command_generate.
 * @see acorn_command_generate
 * @see acorn_request_generate */
struct acorn_response_generate {
  /**
   * Name of new key pair */
  char *alias;
  /**
   * Unique identifier for key pair */
  char *hash;
  /**
   * Opaque representation of encrypted private key material. */
  struct acorn_buffer privateBlob;
  /**
   * Exposed private key material (present only when exportPrivate
   * flag is set). */
  struct acorn_buffer privateKey;
};

typedef int (*acorn_commandfn_generate)
(const struct acorn_request_generate *request,
 struct acorn_response_generate *response,
 char **errstring);

/**
 * Generates a key pair and saves it under the specified alias. The key
 * pair can be used for subsequent commands and will be saved between
 * instances of the application.
 *
 * @param request input data to process
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 *               and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_command_generate
(const struct acorn_request_generate *request,
 struct acorn_response_generate *response,
 char **errstring);

typedef int (*acorn_cmdfn_generate)
(const char *alias,
 const char *algorithm,
 const char *token,
 unsigned flags,
  struct acorn_response_generate *response,
 char **errstring);

/**
 * Generates a key pair and saves it under the specified alias. The key
 * pair can be used for subsequent commands and will be saved between
 * instances of the application.
 *
 * @param alias Name for new key pair (optional)
 * @param algorithm Cryptographic algorithm to use
 * @param token Specifies the operator card set or softoken to use. Use
 * the preload command to avoid console prompts
 * @param flags Set bit one to overwrite existing alias key pair if one
 * exists.
 *
 * @param flags Set bit two to provide private key without encryption in
 * the response privateKey field (DANGEROUS). Setting this undermines
 * the ability of the HSM to protect the key material, though the key
 * still benefits from hardware random number generation.
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 * and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_cmd_generate
(const char *alias,
 const char *algorithm,
 const char *token,
 unsigned flags,
 struct acorn_response_generate *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_generate
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_generate
(struct acorn_request_generate *request,
 struct acorn_response_generate *response);

/**
 * Input parameters for acorn_command_importKeypair.
 * @see acorn_command_importKeypair
 * @see acorn_response_importKeypair */
struct acorn_request_importKeypair {
  /**
   * Name for new key pair (optional) */
  char *alias;
    /**
   * Cryptographic algorithm to use */
  char *algorithm;
    /**
   * Public portion of key pair */
  struct acorn_buffer publicKey;
    /**
   * Private portion of key pair */
  struct acorn_buffer privateKey;
    /**
   * Specifies the operator card set or softoken to use. Use the
   * preload command to avoid console prompts */
  char *token;
    /**
   * Delete existing alias key pair if one exists */
  unsigned overwrite;
  };

/**
 * Output parameters for acorn_command_importKeypair.
 * @see acorn_command_importKeypair
 * @see acorn_request_importKeypair */
struct acorn_response_importKeypair {
  /**
   * Name of new key pair */
  char *alias;
  /**
   * Unique identifier for key pair */
  char *hash;
  /**
   * Opaque representation of encrypted private key material. */
  struct acorn_buffer privateBlob;
};

typedef int (*acorn_commandfn_importKeypair)
(const struct acorn_request_importKeypair *request,
 struct acorn_response_importKeypair *response,
 char **errstring);

/**
 * Imports a signature key pair and saves it under the specified alias.
 * The key pair can be used for subsequent commands and will be saved
 * between instances of the application.
 *
 * @param request input data to process
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 *               and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_command_importKeypair
(const struct acorn_request_importKeypair *request,
 struct acorn_response_importKeypair *response,
 char **errstring);

typedef int (*acorn_cmdfn_importKeypair)
(const char *alias,
 const char *algorithm,
 const struct acorn_buffer publicKey,
 const struct acorn_buffer privateKey,
 const char *token,
 unsigned overwrite,
  struct acorn_response_importKeypair *response,
 char **errstring);

/**
 * Imports a signature key pair and saves it under the specified alias.
 * The key pair can be used for subsequent commands and will be saved
 * between instances of the application.
 *
 * @param alias Name for new key pair (optional)
 * @param algorithm Cryptographic algorithm to use
 * @param publicKey Public portion of key pair
 * @param privateKey Private portion of key pair
 * @param token Specifies the operator card set or softoken to use. Use
 * the preload command to avoid console prompts
 * @param overwrite Delete existing alias key pair if one exists
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 * and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_cmd_importKeypair
(const char *alias,
 const char *algorithm,
 const struct acorn_buffer publicKey,
 const struct acorn_buffer privateKey,
 const char *token,
 unsigned overwrite,
 struct acorn_response_importKeypair *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_importKeypair
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_importKeypair
(struct acorn_request_importKeypair *request,
 struct acorn_response_importKeypair *response);

/**
 * Input parameters for acorn_command_sign.
 * @see acorn_command_sign
 * @see acorn_response_sign */
struct acorn_request_sign {
  /**
   * Specifies key pair to sign with */
  char *alias;
    /**
   * Ignored unless alias is missing */
  char *keyhash;
    /**
   * Data to be signed */
  struct acorn_buffer message;
  };

/**
 * Output parameters for acorn_command_sign.
 * @see acorn_command_sign
 * @see acorn_request_sign */
struct acorn_response_sign {
  /**
   * Cryptographic signature of message */
  struct acorn_buffer signature;
};

typedef int (*acorn_commandfn_sign)
(const struct acorn_request_sign *request,
 struct acorn_response_sign *response,
 char **errstring);

/**
 * Creates a digital signature of the specified message. The keyhash
 * argument is ignored unless the alias is missing.
 *
 * @param request input data to process
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 *               and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_command_sign
(const struct acorn_request_sign *request,
 struct acorn_response_sign *response,
 char **errstring);

typedef int (*acorn_cmdfn_sign)
(const char *alias,
 const char *keyhash,
 const struct acorn_buffer message,
  struct acorn_response_sign *response,
 char **errstring);

/**
 * Creates a digital signature of the specified message. The keyhash
 * argument is ignored unless the alias is missing.
 *
 * @param alias Specifies key pair to sign with
 * @param keyhash Ignored unless alias is missing
 * @param message Data to be signed
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 * and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_cmd_sign
(const char *alias,
 const char *keyhash,
 const struct acorn_buffer message,
 struct acorn_response_sign *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_sign
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_sign
(struct acorn_request_sign *request,
 struct acorn_response_sign *response);

/**
 * Input parameters for acorn_command_verify.
 * @see acorn_command_verify
 * @see acorn_response_verify */
struct acorn_request_verify {
  /**
   * Specifies key pair used for signature */
  char *alias;
    /**
   * Ignored unless alias is missing */
  char *keyhash;
    /**
   * Data that has been signed */
  struct acorn_buffer message;
    /**
   * Cryptographic signature of message */
  struct acorn_buffer signature;
  };

/**
 * Output parameters for acorn_command_verify.
 * @see acorn_command_verify
 * @see acorn_request_verify */
struct acorn_response_verify {
  /**
   * True if and only if signature is cryptographically valid for
   * given message */
  unsigned verified;
};

typedef int (*acorn_commandfn_verify)
(const struct acorn_request_verify *request,
 struct acorn_response_verify *response,
 char **errstring);

/**
 * Determines whether a given signature is valid for a message and key
 * pair. The keyhash argument is ignored unless the alias is missing.
 *
 * @param request input data to process
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 *               and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_command_verify
(const struct acorn_request_verify *request,
 struct acorn_response_verify *response,
 char **errstring);

typedef int (*acorn_cmdfn_verify)
(const char *alias,
 const char *keyhash,
 const struct acorn_buffer message,
 const struct acorn_buffer signature,
  struct acorn_response_verify *response,
 char **errstring);

/**
 * Determines whether a given signature is valid for a message and key
 * pair. The keyhash argument is ignored unless the alias is missing.
 *
 * @param alias Specifies key pair used for signature
 * @param keyhash Ignored unless alias is missing
 * @param message Data that has been signed
 * @param signature Cryptographic signature of message
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 * and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_cmd_verify
(const char *alias,
 const char *keyhash,
 const struct acorn_buffer message,
 const struct acorn_buffer signature,
 struct acorn_response_verify *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_verify
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_verify
(struct acorn_request_verify *request,
 struct acorn_response_verify *response);

/**
 * Input parameters for acorn_command_signImmediate.
 * @see acorn_command_signImmediate
 * @see acorn_response_signImmediate */
struct acorn_request_signImmediate {
  /**
   * Opaque representation of encrypted private key material */
  struct acorn_buffer privateBlob;
    /**
   * Specifies the operator card set or softoken to use. Use the
   * preload command to avoid console prompts */
  char *token;
    /**
   * Data to be signed */
  struct acorn_buffer message;
  };

/**
 * Output parameters for acorn_command_signImmediate.
 * @see acorn_command_signImmediate
 * @see acorn_request_signImmediate */
struct acorn_response_signImmediate {
  /**
   * Cryptographic signature of message */
  struct acorn_buffer signature;
};

typedef int (*acorn_commandfn_signImmediate)
(const struct acorn_request_signImmediate *request,
 struct acorn_response_signImmediate *response,
 char **errstring);

/**
 * Creates a digital signature of the specified message. The keyhash
 * argument is ignored unless the alias is missing.
 *
 * @param request input data to process
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 *               and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_command_signImmediate
(const struct acorn_request_signImmediate *request,
 struct acorn_response_signImmediate *response,
 char **errstring);

typedef int (*acorn_cmdfn_signImmediate)
(const struct acorn_buffer privateBlob,
 const char *token,
 const struct acorn_buffer message,
  struct acorn_response_signImmediate *response,
 char **errstring);

/**
 * Creates a digital signature of the specified message. The keyhash
 * argument is ignored unless the alias is missing.
 *
 * @param privateBlob Opaque representation of encrypted private key
 * material
 * @param token Specifies the operator card set or softoken to use. Use
 * the preload command to avoid console prompts
 * @param message Data to be signed
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 * and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_cmd_signImmediate
(const struct acorn_buffer privateBlob,
 const char *token,
 const struct acorn_buffer message,
 struct acorn_response_signImmediate *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_signImmediate
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_signImmediate
(struct acorn_request_signImmediate *request,
 struct acorn_response_signImmediate *response);

/**
 * Input parameters for acorn_command_verifyImmediate.
 * @see acorn_command_verifyImmediate
 * @see acorn_response_verifyImmediate */
struct acorn_request_verifyImmediate {
  /**
   * Public key of pair used to sign */
  struct acorn_buffer publicKey;
    /**
   * Cryptographic algorithm used to sign */
  char *algorithm;
    /**
   * Data that has been signed */
  struct acorn_buffer message;
    /**
   * Cryptographic signature of message */
  struct acorn_buffer signature;
  };

/**
 * Output parameters for acorn_command_verifyImmediate.
 * @see acorn_command_verifyImmediate
 * @see acorn_request_verifyImmediate */
struct acorn_response_verifyImmediate {
  /**
   * True if and only if signature is cryptographically valid for
   * given message */
  unsigned verified;
};

typedef int (*acorn_commandfn_verifyImmediate)
(const struct acorn_request_verifyImmediate *request,
 struct acorn_response_verifyImmediate *response,
 char **errstring);

/**
 * Determines whether a given signature is valid for a message and
 * public key
 *
 * @param request input data to process
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 *               and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_command_verifyImmediate
(const struct acorn_request_verifyImmediate *request,
 struct acorn_response_verifyImmediate *response,
 char **errstring);

typedef int (*acorn_cmdfn_verifyImmediate)
(const struct acorn_buffer publicKey,
 const char *algorithm,
 const struct acorn_buffer message,
 const struct acorn_buffer signature,
  struct acorn_response_verifyImmediate *response,
 char **errstring);

/**
 * Determines whether a given signature is valid for a message and
 * public key
 *
 * @param publicKey Public key of pair used to sign
 * @param algorithm Cryptographic algorithm used to sign
 * @param message Data that has been signed
 * @param signature Cryptographic signature of message
 * @param response output data to populate
 * @param errstr (Optional) when not NULL, error message is allocated
 * and assogned to this instead of being printed.
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_cmd_verifyImmediate
(const struct acorn_buffer publicKey,
 const char *algorithm,
 const struct acorn_buffer message,
 const struct acorn_buffer signature,
 struct acorn_response_verifyImmediate *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_verifyImmediate
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_verifyImmediate
(struct acorn_request_verifyImmediate *request,
 struct acorn_response_verifyImmediate *response);

/**
 * Input parameters for acorn_command_see_getVersion.
 * @see acorn_command_see_getVersion
 * @see acorn_response_see_getVersion */
struct acorn_request_see_getVersion {
  /** Some C compilers reject empty structs */
  int unused;
};

/**
 * Output parameters for acorn_command_see_getVersion.
 * @see acorn_command_see_getVersion
 * @see acorn_request_see_getVersion */
struct acorn_response_see_getVersion {
  /**
   * Sementic version number of SEE machine */
  char *version;
};

typedef int (*acorn_commandfn_see_getVersion)
(const struct acorn_request_see_getVersion *request,
 struct acorn_response_see_getVersion *response,
 char **errstring);

int EXPORT
acorn_command_see_getVersion
(const struct acorn_request_see_getVersion *request,
 struct acorn_response_see_getVersion *response,
 char **errstring);

typedef int (*acorn_cmdfn_see_getVersion)
( struct acorn_response_see_getVersion *response,
 char **errstring);

int EXPORT
acorn_cmd_see_getVersion
(struct acorn_response_see_getVersion *response,
 char **errstring);

/**
 * Reclaim resources used by corresponding command
 *
 * @see acorn_command_see_getVersion
 * @param request (Optional) request data to free
 * @param response (Optional) response data to free */
void EXPORT
acorn_free_see_getVersion
(struct acorn_request_see_getVersion *request,
 struct acorn_response_see_getVersion *response);

struct acorn_commandlist {
  acorn_commandfn_getVersion getVersion;
  acorn_commandfn_list list;
  acorn_commandfn_getPublic getPublic;
  acorn_commandfn_getPublicHash getPublicHash;
  acorn_commandfn_getKeyInfo getKeyInfo;
  acorn_commandfn_generate generate;
  acorn_commandfn_importKeypair importKeypair;
  acorn_commandfn_sign sign;
  acorn_commandfn_verify verify;
  acorn_commandfn_signImmediate signImmediate;
  acorn_commandfn_verifyImmediate verifyImmediate;
  acorn_commandfn_see_getVersion see_getVersion;
};

struct acorn_cmdlist {
  acorn_cmdfn_getVersion getVersion;
  acorn_cmdfn_list list;
  acorn_cmdfn_getPublic getPublic;
  acorn_cmdfn_getPublicHash getPublicHash;
  acorn_cmdfn_getKeyInfo getKeyInfo;
  acorn_cmdfn_generate generate;
  acorn_cmdfn_importKeypair importKeypair;
  acorn_cmdfn_sign sign;
  acorn_cmdfn_verify verify;
  acorn_cmdfn_signImmediate signImmediate;
  acorn_cmdfn_verifyImmediate verifyImmediate;
  acorn_commandfn_see_getVersion see_getVersion;
};

/**
 * Populates a structure containing each command
 * in request structure form.
 *
 * @param list function pointer list to be populated
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_getcommandlist
(struct acorn_commandlist *list,
 char **errstring);

/**
 * Populates a structure containing each command
 * in request inline form.
 *
 * @param list function pointer list to be populated
 * @returns EXIT_SUCCESS (0) or a non-zero failure code */
int EXPORT
acorn_getcmdlist
(struct acorn_cmdlist *list,
 char **errstring);

#ifdef __cplusplus
}
#endif
#endif /* ACORN_H */
