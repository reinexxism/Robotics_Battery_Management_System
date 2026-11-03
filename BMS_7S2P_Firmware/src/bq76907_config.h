/*
 * bq76907_config.h
 *
 *  Created on: 2026. 11. 3.
 *      Author: nosta
 */

#ifndef BQ76907_CONFIG_H_
#define BQ76907_CONFIG_H_

#include <stdint.h>

typedef enum
{
    BQ_CONFIG_NOT_READ = 0,
    BQ_CONFIG_READ_OK,
    BQ_CONFIG_READ_FAILED,
    BQ_CONFIG_ALREADY_MATCHED,
    BQ_CONFIG_WRITE_FAILED,
    BQ_CONFIG_VERIFIED,
    BQ_CONFIG_VERIFY_FAILED
} Bq76907_ConfigResult;

typedef struct
{
    const char *name;
    uint16_t address;
    uint8_t width;          /* Register size in bytes */

    uint32_t before;        /* Value read before applying settings */
    uint32_t target;        /* Desired value */
    uint32_t applyMask;     /* Bits allowed to change; 0 = read only */
    uint32_t after;         /* Value read back after applying settings */

    Bq76907_ConfigResult result;
} Bq76907_ConfigEntry;

void Bq76907_ConfigReadPending(void);

typedef enum
{
    BQ_7S_UNKNOWN = 0,
    BQ_7S_MATCH,
    BQ_7S_MISMATCH
} Bq76907_7sCheck;

Bq76907_7sCheck Bq76907_ConfigCheck7s(void);

#endif /* BQ76907_CONFIG_H_ */
