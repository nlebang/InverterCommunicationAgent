/*
 * Copyright © 2017 Utkarsh Agrawal <utkarsh.agrawal@wsu.edu> <uautkarsh@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include <inv_ctrl_host.h>

#include "invCtrl.h"

int seed() {
  host_seedCoil();
  host_printAllCoilData();
  return 0;
}

int getDataByCoilAddress(
  int transactionId,
  int coilAddress,
  uint8_t * coilValue,
  uint8_t * signature_ptr,
  int * signatureLen_ptr
) {
  return host_getDataByCoilAddress(transactionId, coilAddress, coilValue, signature_ptr, signatureLen_ptr);
}
int verifyCoilData1(
  int transactionId,
  uint8_t coilValue,
  uint8_t * signature_ptr,
  int signatureLen,
  int * signatureVerified
) {
  return host_verifyCoilData1(transactionId, coilValue, signature_ptr, signatureLen, signatureVerified);
}

int setCoilData1(
  int transactionId,
  int coilAddress,
  uint8_t coilValue,
  uint8_t * signature_ptr,
  int signatureLen
) {
  return host_setCoilData1(
    transactionId,
    coilAddress,
    coilValue,
    signature_ptr,
    signatureLen
  );
}
int signCoilValue1(
  int transactionId,
  uint8_t coilValue,
  uint8_t *signature_ptr,
  int *signatureLen_ptr
) {
  return host_signCoilValue1(
    transactionId,
    coilValue,
    signature_ptr,
    signatureLen_ptr
  );
}



int invCtrl_getDataByCoilAddress(
  int transactionId, int coilAddress,
  uint8_t* req, int req_length, int serverTamper
) {

  int loop = 0;

  uint8_t coilValue;
  uint8_t signature[512] = {0x00};
  int signatureLen = 0;

  host_getDataByCoilAddress(transactionId, coilAddress, &coilValue, signature, &signatureLen);
  
  req[req_length] = (uint8_t)coilValue;
  req_length++;

  if(serverTamper) {
    printf("\nTAMPERING SIGNATURE \n");
    printf("Changing [%.2X] ", signature[0]);
    signature[0] = signature[0] ^ 0xFF;
    printf("to [%.2X] \n\n", signature[0]);
  }

  for(loop = 0; loop < signatureLen; loop++) {
    req[req_length] = (uint8_t)signature[loop];
    req_length++;
  }

  return req_length;
}

int invCtrl_setCoilData(
  int transactionId,
  int coilAddress,
  uint8_t coilValue,
  uint8_t * signature_ptr
) {
  return host_setCoilData1(
    transactionId,
    coilAddress,
    coilValue,
    signature_ptr,
    128
  );
}

int invCtrl_verifyCoilData(
  int transactionId,
  uint8_t coilValue,
  uint8_t * signature_ptr,
  int signatureLen
) {
  int signatureVerified = 0;
  host_verifyCoilData1(transactionId, coilValue, signature_ptr, signatureLen, &signatureVerified); 

  return signatureVerified;
}

int invCtrl_signCoilValue(
  uint8_t transactionIdHigh, uint8_t transactionIdLow,
  uint8_t valueHigh, uint8_t valueLow,
  uint8_t* req, int req_length, int clientTamper
) {

  int transactionId = transactionIdHigh << 8 | transactionIdLow;
  int coilValue = valueHigh << 8 | valueLow;
  
  uint8_t signature[512] = {0x00};
  int signatureLen = 0;
  
  int loop = 0;

  if(coilValue == 0xFF00) {
    coilValue = 1;
  } else {
    coilValue = 0;
  }

  host_signCoilValue1(transactionId, coilValue, signature, &signatureLen);

  if(clientTamper) {
    printf("\nTAMPERING SIGNATURE \n");
    printf("Changing [%.2X] ", signature[0]);
    signature[0] = signature[0] ^ 0xFF;
    printf("to [%.2X] \n\n", signature[0]);
  }

  for(loop = 0; loop < signatureLen; loop++) {
    req[req_length + loop] = (uint8_t)signature[loop];
  }

  return req_length + 128;
}

