/**
 * @file "simple_crypto.h"
 * @author Ben Janis
 * @brief Simplified Crypto API Header 
 * @date 2025
 *
 * This source file is part of an example system for MITRE's 2025 Embedded System CTF (eCTF).
 * This code is being provided only for educational purposes for the 2025 MITRE eCTF competition,
 * and may not meet MITRE standards for quality. Use this code at your own risk!
 *
 * @copyright Copyright (c) 2025 The MITRE Corporation
 */

#ifndef ECTF_CRYPTO_H
#define ECTF_CRYPTO_H

//#include "wolfssl/wolfcrypt/settings.h"
#include "wolfssl/wolfcrypt/aes.h"
#include "wolfssl/wolfcrypt/random.h"
#include "wolfssl/wolfcrypt/ecc.h"
#include "wolfssl/wolfcrypt/hash.h"
#include "wolfssl/wolfcrypt/sha.h"
#include "trng.h"

/******************************** MACRO DEFINITIONS ********************************/
#define BLOCK_SIZE AES_BLOCK_SIZE
#define KEY_SIZE 32
#define HASH_SIZE SHA256_DIGEST_SIZE
#define PADDING_CHAR '\0'

/******************************** FUNCTION PROTOTYPES ********************************/
/** @brief Encrypts plaintext using a symmetric cipher
 *
 * @param plaintext A pointer to a buffer of length len containing the
 *          plaintext to encrypt
 * @param len The length of the plaintext to encrypt. Must be a multiple of
 *          BLOCK_SIZE (16 bytes)
 * @param key A pointer to a buffer of length KEY_SIZE (16 bytes) containing
 *          the key to use for encryption
 * @param ciphertext A pointer to a buffer of length len where the resulting
 *          ciphertext will be written to
 *
 * @return 0 on success, -1 on bad length, other non-zero for other error
 */
int encrypt_sym(uint8_t *plaintext, size_t len, uint8_t *key, uint8_t *ciphertext);

/** @brief Decrypts ciphertext using a symmetric cipher
 *
 * @param ciphertext A pointer to a buffer of length len containing the
 *           ciphertext to decrypt
 * @param len The length of the ciphertext to decrypt. Must be a multiple of
 *           BLOCK_SIZE (16 bytes)
 * @param key A pointer to a buffer of length KEY_SIZE (16 bytes) containing
 *           the key to use for decryption
 * @param plaintext A pointer to a buffer of length len where the resulting
 *           plaintext will be written to
 *
 * @return 0 on success, -1 on bad length, other non-zero for other error
 */
int decrypt_sym(uint8_t *ciphertext, size_t len, uint8_t *key, uint8_t *plaintext, size_t *plaintext_len);


/** @brief Generates HMAC on a given buffer and authenticates with a given HMAC tag
 *
 * @param key       A pointer to a buffer containing the key/secret for generating the HMAC
 * @param buffer    A pointer to a buffer containing the message to be autheneticated
 * @param tag       A pointer to a buffer containing the pre-calculated HMAC tag to compare to for authentication
 *
 * @return 0 on success, non-zero for other error
 */
int verify_hmac(uint8_t* key, uint8_t* buffer, uint8_t* tag);
/** @brief Hashes arbitrary-length data
 *
 * @param data A pointer to a buffer of length len containing the data
 *           to be hashed
 * @param len The length of the plaintext to hash
 * @param hash_out A pointer to a buffer of length HASH_SIZE (16 bytes) where the resulting
 *           hash output will be written to
 *
 * @return 0 on success, non-zero for other error
 */
int hash(void *data, size_t len, uint8_t *hash_out);

int KDF_Gen(const uint8_t* salt, size_t salt_len, uint8_t* sym_key, uint8_t* mac_key);
#endif // ECTF_CRYPTO_H
