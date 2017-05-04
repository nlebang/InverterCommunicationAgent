/*
 * Copyright © 2017 Utkarsh Agrawal <utkarsh.agrawal@wsu.edu> <uautkarsh@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

int seed(void);

int getDataByCoilAddress(
  int transactionId,
  int coilAddress,
  uint8_t * coilValue,
  uint8_t * signature_ptr,
  int * signatureLen_ptr
);
int verifyCoilData1(
  int transactionId,
  uint8_t coilValue,
  uint8_t * signature_ptr,
  int signatureLen,
  int * signatureVerified
);
int setCoilData1(
  int transactionId,
  int coilAddress,
  uint8_t coilValue,
  uint8_t * signature_ptr,
  int signatureLen
);
int signCoilValue1(
  int transactionId,
  uint8_t coilValue,
  uint8_t *signature_ptr,
  int *signatureLen_ptr
);

int invCtrl_getDataByCoilAddress(
  int transactionId, int coilAddress,
  uint8_t* req, int req_length, int serverTamper
);

int invCtrl_setCoilData(
  int transactionId,
  int coilAddress,
  uint8_t coilValue,
  uint8_t * signature_ptr
);

int invCtrl_verifyCoilData(
  int transactionId,
  uint8_t coilValue,
  uint8_t * signature_ptr,
  int signatureLen
);

int invCtrl_signCoilValue(
  uint8_t transactionIdHigh, uint8_t transactionIdLow,
  uint8_t valueHigh, uint8_t valueLow,
  uint8_t* req, int req_length, int clientTamper
);