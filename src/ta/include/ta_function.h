/*
 * Copyright © 2017 Utkarsh Agrawal <utkarsh.agrawal@wsu.edu> <uautkarsh@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef TA_FUNCTION_H
#define TA_FUNCTION_H


static const uint8_t coilData[] = {
  // Bit number 0 1 2 3 4 5 6 7
  // Value    1 1 1 1 0 0 0 0 
  0x00
};

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


#define TA_INV_CTRL_UUID { 0x8aaaf200, 0x2450, 0x11e4, \
    { 0xab, 0xe2, 0x00, 0x02, 0xa5, 0xd5, 0xc5, 0x1b} }

#define TA_SEED_COIL          9001
#define TA_GET_ALL_COIL_DATA      9002
#define TA_GET_DATA_BY_COIL_ADDRESS   9003
#define TA_VERIFY_COIL_DATA       9004
#define TA_SET_DATA_BY_COIL_ADDRESS   9005
#define TA_SIGN_COIL_DATA       9006

#endif /*TA_FUNCTION_H*/
