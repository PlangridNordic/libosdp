/*
 * Copyright (c) 2021-2026 Siddharth Chandrasekaran <sidcha@osdp.dev>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <utils/utils.h>

#include <mbedtls/aes.h>
#include <mbedtls/platform_util.h>

#ifdef MBEDTLS_PSA_CRYPTO_C
#include <psa/crypto.h>
#else
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#endif

#include <osdp.h>

/*
 * The crypto interface cannot report errors, and carrying on after one would
 * leave predictable nonces or garbage ciphertext behind. assert() compiles
 * out in release builds, so stop explicitly, as the other backends do.
 */
static void __noreturn osdp_mbedtls_fatal(void)
{
	abort();
}

/* Mbed TLS and PSA both report success as 0. */
static void crypt_check(int rc)
{
	if (rc != 0) {
		osdp_mbedtls_fatal();
	}
}

#ifndef MBEDTLS_PSA_CRYPTO_C
static mbedtls_entropy_context entropy_ctx;
static mbedtls_ctr_drbg_context ctr_drbg_ctx;
static bool drbg_seeded;
#endif

/*
 * Runs from context setup, before any refresh loop can draw, so the
 * unsynchronised seeded check only races if contexts are set up from several
 * threads at once, which is not supported.
 *
 * The DRBG is seeded once and never freed: every context draws from it, and
 * re-seeding it under a context that is still using it would pull its state
 * away. With MBEDTLS_THREADING_C, mbedtls_ctr_drbg_random() serialises
 * concurrent draws itself; without it, draws from several threads race.
 * Prediction resistance pulls fresh entropy into every draw, so a forked
 * process never repeats its parent's output; a draw happens once per secure
 * channel handshake.
 */
void osdp_crypt_setup()
{
#ifdef MBEDTLS_PSA_CRYPTO_C
	crypt_check(psa_crypto_init());
#else
	const char *version;

	if (drbg_seeded) {
		return;
	}
	version = osdp_get_version();
	mbedtls_entropy_init(&entropy_ctx);
	mbedtls_ctr_drbg_init(&ctr_drbg_ctx);
	crypt_check(mbedtls_ctr_drbg_seed(&ctr_drbg_ctx, mbedtls_entropy_func,
					  &entropy_ctx,
					  (const unsigned char *)version,
					  strlen(version)));
	mbedtls_ctr_drbg_set_prediction_resistance(&ctr_drbg_ctx,
						   MBEDTLS_CTR_DRBG_PR_ON);
	drbg_seeded = true;
#endif
}

/*
 * Contexts may run their secure channels on different threads, so AES keeps
 * no state between calls; each call sets up its own key schedule.
 */
static void aes_crypt(int mode, uint8_t *key, uint8_t *iv, uint8_t *data,
		      int len)
{
	mbedtls_aes_context aes_ctx;
	int rc;

	mbedtls_aes_init(&aes_ctx);
	if (mode == MBEDTLS_AES_ENCRYPT) {
		rc = mbedtls_aes_setkey_enc(&aes_ctx, key, 128);
	} else {
		rc = mbedtls_aes_setkey_dec(&aes_ctx, key, 128);
	}
	crypt_check(rc);
	if (iv != NULL) {
		rc = mbedtls_aes_crypt_cbc(&aes_ctx, mode, len, iv, data, data);
	} else {
		assert(len <= 16);
		rc = mbedtls_aes_crypt_ecb(&aes_ctx, mode, data, data);
	}
	crypt_check(rc);
	mbedtls_aes_free(&aes_ctx);
}

void osdp_encrypt(uint8_t *key, uint8_t *iv, uint8_t *data, int len)
{
	aes_crypt(MBEDTLS_AES_ENCRYPT, key, iv, data, len);
}

void osdp_decrypt(uint8_t *key, uint8_t *iv, uint8_t *data, int len)
{
	aes_crypt(MBEDTLS_AES_DECRYPT, key, iv, data, len);
}

void osdp_fill_random(uint8_t *buf, int len)
{
#ifdef MBEDTLS_PSA_CRYPTO_C
	crypt_check(psa_generate_random(buf, len));
#else
	/* An unseeded DRBG draws through a NULL entropy callback. */
	if (!drbg_seeded) {
		osdp_mbedtls_fatal();
	}
	crypt_check(mbedtls_ctr_drbg_random(&ctr_drbg_ctx, buf, len));
#endif
}

void osdp_fill_zeros(void *buf, int len)
{
	mbedtls_platform_zeroize(buf, (size_t)len);
}

void osdp_crypt_teardown()
{
}
