/**
 * @file "simple_crypto.c"
 * @author Ben Janis
 * @brief Simplified Crypto API Implementation
 * @date 2025
 *
 * This source file is part of an example system for MITRE's 2025 Embedded System CTF (eCTF).
 * This code is being provided only for educational purposes for the 2025 MITRE eCTF competition,
 * and may not meet MITRE standards for quality. Use this code at your own risk!
 *
 * @copyright Copyright (c) 2025 The MITRE Corporation
 */

#include "wolfssl/wolfcrypt/settings.h"
#include "simple_crypto.h"
#include "host_messaging.h"
#include "wolfssl/wolfcrypt/hmac.h"
#include "global_secrets.h"
#include <stdint.h>
#include <string.h>

#define PADDING_CHAR '\0'
#define MAC_KEY_LEN 32
#define MAC_TAG_LEN 32
#define ROOT_KEY_LEN 32
#define MAC_BUF_LEN 64
/******************************** FUNCTION PROTOTYPES ********************************/
/** @brief Decrypts ciphertext using a symmetric cipher
 *
 * @param ciphertext A pointer to a buffer of length len containing the
 *          ciphertext to decrypt
 * @param len The length of the ciphertext to decrypt. Must be a multiple of
 *          BLOCK_SIZE (16 bytes)
 * @param key A pointer to a buffer of length KEY_SIZE (16 bytes) containing
 *          the key to use for decryption
 * @param plaintext A pointer to a buffer of length len where the resulting
 *          plaintext will be written to
 *
 * @return 0 on success, -1 on bad length, other non-zero for other error
 */
int decrypt_sym(uint8_t *ciphertext, size_t len, uint8_t *key, uint8_t *plaintext, size_t *plaintext_len) {
    Aes ctx; // Context for decryption
    int result; // Library result

    // Ensure valid length
    if (len <= 0 || len % BLOCK_SIZE)
        return -1;

    // NONCE -> NULL if fail
    // Set the key for decryption
    result = wc_AesSetKey(&ctx, key, 32, NONCE, AES_DECRYPTION);
    if (result != 0)
        return result; // Report error
    
    int decrypt_result = wc_AesCbcDecrypt(&ctx, plaintext, ciphertext, len);
    if(decrypt_result != 0){
        return decrypt_result;
    }

    size_t padding = 0;

    for(size_t i = len - 1; i >= 0; i--){
        if(plaintext[i] == PADDING_CHAR){
            padding++;
        }
        else{
            break;
        }
    }

    *plaintext_len = len - padding;
    
    return 0;
}

/** @brief Generates HMAC on a given buffer and authenticates with a given HMAC tag
 *
 * @param key       A pointer to a buffer containing the key/secret for generating the HMAC
 * @param buffer    A pointer to a buffer containing the message to be autheneticated
 * @param tag       A pointer to a buffer containing the pre-calculated HMAC tag to compare to for authentication
 *
 * @return 0 on success, non-zero for other error
 */
int verify_hmac(uint8_t* key, uint8_t* buffer, uint8_t* tag){
    Hmac hmac;
    uint8_t digest[SHA256_DIGEST_SIZE] = {0};
    
    int key_res = wc_HmacSetKey(&hmac, SHA256, key, MAC_KEY_LEN);
    if(key_res < 0){
        return -111;
    }
    
    int update_res = wc_HmacUpdate(&hmac, buffer, MAC_BUF_LEN);
    if(update_res < 0){
        return -222;
    }
    
    int final_res = wc_HmacFinal(&hmac, digest);
    if(final_res < 0){
        return -333;
    }

    int auth = memcmp(digest, tag, MAC_TAG_LEN);
    if(auth != 0){
        return -444;
    }

    return 0;
}

/** @brief Hashes arbitrary-length data
 *
 * @param data A pointer to a buffer of length len containing the data
 *          to be hashed
 * @param len The length of the plaintext to hash
 * @param hash_out A pointer to a buffer of length HASH_SIZE (16 bytes) where the resulting
 *          hash output will be written to
 *
 * @return 0 on success, non-zero for other error
 */
int hash(void *data, size_t len, uint8_t *hash_out) {
    // Pass values to hash
    return wc_Sha256Hash((uint8_t *)data, len, hash_out);
}


int KDF_Gen(const uint8_t* salt, size_t salt_len, uint8_t* sym_key, uint8_t* mac_key){
    uint8_t* master = (uint8_t*)malloc(64 * sizeof(uint8_t));
    int res = wc_HKDF(WC_SHA256, ROOT_KEY, 32, salt, salt_len, NULL, 0, master, 64);

    memmove(sym_key, master, 32);
    memmove(mac_key, master + 32, 32);

    if(res < 0){
        return -555;
    }

    free(master);

    return 0;
}



unsigned int custom_rand_generate_block(byte* data, word32 len){
    int ret = MXC_TRNG_Random(data, len);
    
    return ret == 0 ? 0 : ret;
}

unsigned int rand_gen(void){
    return MXC_TRNG_RandomInt();
}
