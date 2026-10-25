/*
 * Copyright (c) 2026 Siddharth Chandrasekaran <sidcha@osdp.dev>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <psa/crypto.h>

#include <utils/utils.h>

#define AES_BLOCK_SIZE 16

/*
 * The crypto interface cannot report errors, and carrying on after one would
 * leave predictable nonces or garbage ciphertext behind. assert() compiles
 * out in release builds, so stop explicitly, as the OpenSSL backend does.
 */
static void __noreturn osdp_psa_fatal(void)
{
	abort();
}

static void psa_check(psa_status_t status)
{
	if (status != PSA_SUCCESS) {
		osdp_psa_fatal();
	}
}

/*
 * The other backends need no setup, so nothing stops a caller from reaching
 * the crypto entry points before osdp_crypt_setup(). psa_crypto_init() is
 * idempotent and cheap once done, so each entry point calls it.
 */
static void psa_ensure_init(void)
{
	psa_check(psa_crypto_init());
}

void osdp_crypt_setup()
{
	psa_ensure_init();
}

void osdp_fill_zeros(void *buf, int len)
{
	volatile uint8_t *p = (volatile uint8_t *)buf;

	while (len--) {
		*p++ = 0;
	}
}

/*
 * AES-128 over data in place: ECB when iv is NULL, CBC otherwise. Callers do
 * not rely on iv being advanced. Blocks go through a bounce buffer because
 * PSA providers may refuse overlapping input and output, and a provider may
 * hold a block back until finish, so output trails input; it never overtakes
 * it, which keeps the in-place write safe.
 */
static void aes_crypt(bool encrypt, const uint8_t *key, const uint8_t *iv,
		      uint8_t *data, int len)
{
	psa_key_attributes_t attr = PSA_KEY_ATTRIBUTES_INIT;
	psa_cipher_operation_t op = PSA_CIPHER_OPERATION_INIT;
	psa_algorithm_t alg =
		iv ? PSA_ALG_CBC_NO_PADDING : PSA_ALG_ECB_NO_PADDING;
	uint8_t block[AES_BLOCK_SIZE];
	psa_key_id_t key_id;
	size_t out_len, produced = 0;
	int pos;

	psa_ensure_init();

	if (len % AES_BLOCK_SIZE != 0) {
		osdp_psa_fatal();
	}

	psa_set_key_type(&attr, PSA_KEY_TYPE_AES);
	psa_set_key_bits(&attr, 128);
	psa_set_key_usage_flags(&attr, encrypt ? PSA_KEY_USAGE_ENCRYPT :
						 PSA_KEY_USAGE_DECRYPT);
	psa_set_key_algorithm(&attr, alg);
	psa_check(psa_import_key(&attr, key, AES_BLOCK_SIZE, &key_id));

	if (encrypt) {
		psa_check(psa_cipher_encrypt_setup(&op, key_id, alg));
	} else {
		psa_check(psa_cipher_decrypt_setup(&op, key_id, alg));
	}

	if (iv != NULL) {
		psa_check(psa_cipher_set_iv(&op, iv, AES_BLOCK_SIZE));
	}

	for (pos = 0; pos < len; pos += AES_BLOCK_SIZE) {
		psa_check(psa_cipher_update(&op, data + pos, AES_BLOCK_SIZE,
					    block, sizeof(block), &out_len));
		memcpy(data + produced, block, out_len);
		produced += out_len;
	}

	psa_check(psa_cipher_finish(&op, block, sizeof(block), &out_len));
	memcpy(data + produced, block, out_len);
	produced += out_len;
	if (produced != (size_t)len) {
		osdp_psa_fatal();
	}
	psa_check(psa_destroy_key(key_id));
	osdp_fill_zeros(block, sizeof(block));
}

void osdp_encrypt(uint8_t *key, uint8_t *iv, uint8_t *data, int len)
{
	aes_crypt(true, key, iv, data, len);
}

void osdp_decrypt(uint8_t *key, uint8_t *iv, uint8_t *data, int len)
{
	aes_crypt(false, key, iv, data, len);
}

void osdp_fill_random(uint8_t *buf, int len)
{
	psa_ensure_init();
	psa_check(psa_generate_random(buf, (size_t)len));
}

void osdp_crypt_teardown()
{
}
