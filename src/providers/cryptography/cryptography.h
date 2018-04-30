
/**
 * @file /magma/providers/cryptography/cryptography.h
 *
 * @brief	Functions used to perform cryptographic operations and provide truly random data.
 */

#ifndef MAGMA_PROVIDERS_CRYPTOGRAPHY_H
#define MAGMA_PROVIDERS_CRYPTOGRAPHY_H

/// random.c
bool_t        rand_start(void);
int16_t       rand_get_int16(void);
int32_t       rand_get_int32(void);
int64_t       rand_get_int64(void);
int8_t        rand_get_int8(void);
size_t        rand_write(stringer_t *s);
uint16_t      rand_get_uint16(void);
uint32_t      rand_get_uint32(void);
uint64_t      rand_get_uint64(void);
uint8_t       rand_get_uint8(void);
void          rand_stop(void);


#endif
