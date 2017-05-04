/*
 * Copyright © 2017 Utkarsh Agrawal <utkarsh.agrawal@wsu.edu> <uautkarsh@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef INV_CTRL_HOST_H
#define INV_CTRL_HOST_H

int host_seedCoil();
int host_printAllCoilData();
int host_getDataByCoilAddress(
  int transactionId,
  int coilAddress,
  uint8_t * coilValue,
  uint8_t * signature_ptr,
  int * signatureLen_ptr
);
int host_verifyCoilData1(
  int transactionId,
  uint8_t coilValue,
  uint8_t * signature_ptr,
  int signatureLen,
  int * signatureVerified
);
int host_setCoilData1(
  int transactionId,
  int coilAddress,
  uint8_t coilValue,
  uint8_t * signature_ptr,
  int signatureLen
);
int host_signCoilValue1(
  int transactionId,
  uint8_t coilValue,
  uint8_t *signature_ptr,
  int *signatureLen_ptr
);
#endif /*INV_CTRL_HOST_H*/
