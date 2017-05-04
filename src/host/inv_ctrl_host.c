/*
 * Copyright © 2017 Utkarsh Agrawal <utkarsh.agrawal@wsu.edu> <uautkarsh@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// Standard Headers
#include <stdio.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// OPTEE Headers
#include <tee_client_api.h>
#include <tee_client_api_extensions.h>

#include <inv_ctrl_host.h>

// Host and TA shared header
#include "ta_function.h"
// #include <inv_ctrl_ta.h>

#ifndef DEBUG
#define DEBUG false
#endif

#ifdef DEBUG
  #if DEBUG > 0
    #define DEBUG_PRINT(x) printf x
  # else
    #define DEBUG_PRINT(x) do {} while(0)
  #endif
#endif


int host_seedCoil() {
  TEEC_Context ctx;
  TEEC_Result res;  
  TEEC_Session session;
  uint32_t err_origin;
  TEEC_UUID uuid = TA_INV_CTRL_UUID;

  TEEC_Operation op;

  res = TEEC_InitializeContext(NULL, &ctx);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
    #endif
    #endif
    return res;
  }

  res = TEEC_OpenSession(&ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res, err_origin);
    #endif
    #endif
    return res;
  }

  memset(&op, 0, sizeof(op));
  op.paramTypes = TEEC_PARAM_TYPES(TEEC_NONE, TEEC_NONE, TEEC_NONE, TEEC_NONE);

  res = TEEC_InvokeCommand(&session, TA_SEED_COIL, &op, &err_origin);
  
  TEEC_CloseSession(&session);

  TEEC_FinalizeContext(&ctx);
  return 0;
}

int host_printAllCoilData() {
  TEEC_Context ctx;
  TEEC_Result res;  
  TEEC_Session session;
  uint32_t err_origin;
  TEEC_UUID uuid = TA_INV_CTRL_UUID;

  TEEC_Operation op;

  char buffer[ARRAY_SIZE(coilData)];
  uint32_t bufferLen = ARRAY_SIZE(coilData);

  int loop;

  res = TEEC_InitializeContext(NULL, &ctx);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
    #endif
    #endif
    return res;
  }

  res = TEEC_OpenSession(&ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res, err_origin);
    #endif
    #endif
    return res;
  }

  memset(&op, 0, sizeof(op));
  op.params[0].tmpref.buffer = &buffer;
  op.params[0].tmpref.size = bufferLen;
  op.params[1].value.a = 0;

  op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_VALUE_INOUT, TEEC_NONE, TEEC_NONE);

  res = TEEC_InvokeCommand(&session, TA_GET_ALL_COIL_DATA, &op, &err_origin);

  (void)loop;
  // for(loop = 0; loop < ARRAY_SIZE(coilData); loop++) {
  //  printf("%.2X ", buffer[loop]);
  // }

  TEEC_CloseSession(&session);

  TEEC_FinalizeContext(&ctx);
  return 0;
}

int host_getDataByCoilAddress(
  int transactionId,
  int coilAddress,
  uint8_t * coilValue,
  uint8_t * signature_ptr,
  int * signatureLen_ptr
) {
  TEEC_Context ctx;
  TEEC_Result res;  
  TEEC_Session session;
  uint32_t err_origin;
  TEEC_UUID uuid = TA_INV_CTRL_UUID;

  TEEC_Operation op;

  char buffer[512];
  uint32_t bufferLen = 512;

  int loop;

  res = TEEC_InitializeContext(NULL, &ctx);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
    #endif
    #endif
    return res;
  }

  res = TEEC_OpenSession(&ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res, err_origin);
    #endif
    #endif
    return res;
  }

  memset(&op, 0, sizeof(op));
  op.params[0].value.a = transactionId;
  op.params[0].value.b = coilAddress;
  op.params[1].tmpref.buffer = &buffer;
  op.params[1].tmpref.size = bufferLen;
  op.params[2].value.a = 0;
  op.params[2].value.b = 0;

  op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_MEMREF_TEMP_OUTPUT, TEEC_VALUE_INOUT, TEEC_NONE);
  
  res = TEEC_InvokeCommand(&session, TA_GET_DATA_BY_COIL_ADDRESS, &op, &err_origin);
  
  *coilValue = op.params[2].value.a;
  printf("VALUEEEEEE %.2X\n", *coilValue);
  *signatureLen_ptr = op.params[1].tmpref.size;
  for(loop = 0; loop < *signatureLen_ptr; loop++) {
    signature_ptr[loop] = buffer[loop];
  }

  TEEC_CloseSession(&session);

  TEEC_FinalizeContext(&ctx);
  
  return 0;
}

int host_setCoilData1(
  int transactionId,
  int coilAddress,
  uint8_t coilValue,
  uint8_t * signature_ptr,
  int signatureLen
) {
  TEEC_Context ctx;
  TEEC_Result res;  
  TEEC_Session session;
  uint32_t err_origin;
  TEEC_UUID uuid = TA_INV_CTRL_UUID;

  TEEC_Operation op;

  int dataLen = 3;
  uint8_t data[dataLen];

  char buffer[512];
  uint32_t bufferLen = 512;

  int loop;

  res = TEEC_InitializeContext(NULL, &ctx);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
    #endif
    #endif
    return res;
  }

  res = TEEC_OpenSession(&ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res, err_origin);
    #endif
    #endif
    return res;
  }
  data[0] = coilValue;
  data[1] = transactionId >> 8;
  data[2] = transactionId & 0x00ff;

  bufferLen = signatureLen;
  for(loop = 0; loop < signatureLen; loop++) {
    buffer[loop] = signature_ptr[loop];
  }

  memset(&op, 0, sizeof(op));
  op.params[0].value.a = coilAddress;
  op.params[0].value.b = coilValue;
  op.params[1].tmpref.buffer = &data;
  op.params[1].tmpref.size = dataLen;
  op.params[2].tmpref.buffer = &buffer;
  op.params[2].tmpref.size = bufferLen;
  op.params[3].value.a = 0;

  op.paramTypes = TEEC_PARAM_TYPES(TEEC_VALUE_INOUT, TEEC_MEMREF_TEMP_OUTPUT, TEEC_MEMREF_TEMP_OUTPUT, TEEC_VALUE_INOUT);
  
  res = TEEC_InvokeCommand(&session, TA_SET_DATA_BY_COIL_ADDRESS, &op, &err_origin);
  TEEC_CloseSession(&session);

  TEEC_FinalizeContext(&ctx);
  
  return op.params[3].value.a;
}

int host_verifyCoilData1(
  int transactionId,
  uint8_t coilValue,
  uint8_t * signature_ptr,
  int signatureLen,
  int * signatureVerified
) {
  int dataLen = 3;
  uint8_t data[dataLen];

  TEEC_Context ctx;
  TEEC_Result res;  
  TEEC_Session session;
  uint32_t err_origin;
  TEEC_UUID uuid = TA_INV_CTRL_UUID;

  TEEC_Operation op;

  // int loop;

  res = TEEC_InitializeContext(NULL, &ctx);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
    #endif
    #endif
    return res;
  }

  res = TEEC_OpenSession(&ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res, err_origin);
    #endif
    #endif
    return res;
  }

  data[0] = coilValue;
  data[1] = transactionId >> 8;
  data[2] = transactionId & 0x00ff;

  memset(&op, 0, sizeof(op));
  op.params[0].tmpref.buffer = data;
  op.params[0].tmpref.size = dataLen;
  op.params[1].tmpref.buffer = signature_ptr;
  op.params[1].tmpref.size = signatureLen;
  op.params[2].value.a = 0;

  op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_MEMREF_TEMP_OUTPUT, TEEC_VALUE_INOUT, TEEC_NONE);
  
  res = TEEC_InvokeCommand(&session, TA_VERIFY_COIL_DATA, &op, &err_origin);

  *signatureVerified = op.params[2].value.a;

  TEEC_CloseSession(&session);

  TEEC_FinalizeContext(&ctx);
  
  return 0;
}

int host_signCoilValue1(
  int transactionId,
  uint8_t coilValue,
  uint8_t *signature_ptr,
  int *signatureLen_ptr
) {
  int dataLen = 3;
  uint8_t data[dataLen];
  
  char buffer[512];
  uint32_t bufferLen = 512;

  TEEC_Context ctx;
  TEEC_Result res;  
  TEEC_Session session;
  uint32_t err_origin;
  TEEC_UUID uuid = TA_INV_CTRL_UUID;

  TEEC_Operation op;

  int loop;

  res = TEEC_InitializeContext(NULL, &ctx);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_InitializeContext failed with code 0x%x", res);
    #endif
    #endif
    return res;
  }

  res = TEEC_OpenSession(&ctx, &session, &uuid, TEEC_LOGIN_PUBLIC, NULL, NULL, &err_origin);
  if (res != TEEC_SUCCESS) {
    #ifdef DEBUG
    #if (DEBUG > 0)
    errx(1, "TEEC_Opensession failed with code 0x%x origin 0x%x", res, err_origin);
    #endif
    #endif
    return res;
  }

  data[0] = coilValue;
  data[1] = transactionId >> 8;
  data[2] = transactionId & 0x00ff;

  memset(&op, 0, sizeof(op));
  op.params[0].tmpref.buffer = data;
  op.params[0].tmpref.size = dataLen;
  op.params[1].tmpref.buffer = buffer;
  op.params[1].tmpref.size = bufferLen;

  op.paramTypes = TEEC_PARAM_TYPES(TEEC_MEMREF_TEMP_OUTPUT, TEEC_MEMREF_TEMP_OUTPUT, TEEC_NONE, TEEC_NONE);
  
  res = TEEC_InvokeCommand(&session, TA_SIGN_COIL_DATA, &op, &err_origin);
  
  *signatureLen_ptr = op.params[1].tmpref.size;
  for(loop = 0; loop < *signatureLen_ptr; loop++) {
    signature_ptr[loop] = buffer[loop];
  }

  TEEC_CloseSession(&session);

  TEEC_FinalizeContext(&ctx);

  // host_sign(data, dataLen, signature_ptr, signatureLen_ptr);
  
  return 0;
}