/*
 * Copyright © 2017 Utkarsh Agrawal <utkarsh.agrawal@wsu.edu> <uautkarsh@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define STR_TRACE_USER_TA "INV_CTRL"

#include <string.h>

#include <tee_internal_api.h>
#include <tee_internal_api_extensions.h>

#include "ta_function.h"
#include "inv_ctrl_ta.h"

/*
 * Called when the instance of the TA is created. This is the first call in
 * the TA.
 */
TEE_Result TA_CreateEntryPoint(void) {
  #ifdef DEBUG
  #if DEBUG > 0
  DMSG("\nTA_CreateEntryPoint called\n");
  #endif
  #endif
  return TEE_SUCCESS;
}

/*
 * Called when the instance of the TA is destroyed if the TA has not
 * crashed or panicked. This is the last call in the TA.
 */
void TA_DestroyEntryPoint(void) {
  #ifdef DEBUG
  #if DEBUG > 0
  DMSG("\nTA_DestroyEntryPoint called\n");
  #endif
  #endif
}

/*
 * Called when a new session is opened to the TA. *sess_ctx can be updated
 * with a value to be able to identify this session in subsequent calls to the
 * TA. In this function you will normally do the global initialization for the
 * TA.
 */
TEE_Result TA_OpenSessionEntryPoint(
  uint32_t nParamTypes,
  TEE_Param pParams[4],
  void **ppSessionContext
) {
  (void)nParamTypes;
  (void)pParams;
  (void)ppSessionContext;

  #ifdef DEBUG
  #if DEBUG > 0
  DMSG("\nTA_OpenSessionEntryPoint called\n");
  #endif
  #endif

  return TEE_SUCCESS;
}

/*
 * Called when a session is closed, sess_ctx hold the value that was
 * assigned by TA_OpenSessionEntryPoint().
 */
void TA_CloseSessionEntryPoint(void *pSessionContext) {
  (void)pSessionContext;
  #ifdef DEBUG
  #if DEBUG > 0
  DMSG("\nTA_CloseSessionEntryPoint called\n");
  #endif
  #endif

}

TEE_Result ta_seedCoil(
  void
){
  return ta_overwriteCoilWithData((void *)coilData, ARRAY_SIZE(coilData));
}

TEE_Result ta_overwriteCoilWithData(
  void *newCoilData,
  int newCoilDataLen
){
  TEE_ObjectHandle objectHandle;
  TEE_Result res;
  
  DMSG("\nTA_MESSAGE: SEEDING ALL COIL DATA\n");

  res = ta_createPersistentObject(
      TEE_STORAGE_PRIVATE_REE,
      (void *)filename, ARRAY_SIZE(filename),
      TEE_DATA_FLAG_ACCESS_WRITE | TEE_DATA_FLAG_OVERWRITE,
      (TEE_ObjectHandle)(uintptr_t)0,
      (void *)newCoilData,
      newCoilDataLen,
      &objectHandle
    );

  res = ta_closePersistentObject(objectHandle);

  return res;
}

TEE_Result ta_getAllCoilData(
  TEE_Param params[4]
) {
  return ta_getAllCoilData_unpacked(params[0].memref.buffer, params[0].memref.size, &params[1].value.a);
}

TEE_Result ta_getAllCoilData_unpacked(
  void* buffer, uint32_t size,
  uint32_t *count
) {
  TEE_ObjectHandle objectHandle;
  TEE_Result res;

  DMSG("\nTA_MESSAGE: FETCHING ALL COIL DATA\n");

  res = ta_openPersistentObject(
      TEE_STORAGE_PRIVATE_REE,
      (void *)filename, ARRAY_SIZE(filename),
      TEE_DATA_FLAG_ACCESS_READ,
      &objectHandle
    );

  res = ta_readPersistentObject(
      objectHandle,
      buffer,
      size,
      count
    );

  res = ta_closePersistentObject(objectHandle);

  return res;
}

TEE_Result ta_getDataByCoilAddress(
  TEE_Param params[4]
) {
  TEE_Result res;

  uint8_t allCoilData[ARRAY_SIZE(coilData)] = {0x00};
  int allCoilDataLen = ARRAY_SIZE(coilData);
  uint32_t nb;

  int bitNumber = 0;
  uint8_t coilBitValue = 0;
  uint8_t modbusCoilValue = 0;

  int dataLen = 3;
  uint8_t dataToSign[dataLen];

  DMSG("\nTA_MESSAGE: FETCHING DATA BY COIL ADDRESS\n");
  
  (void)params;

  res = ta_getAllCoilData_unpacked(allCoilData, allCoilDataLen, &nb);

  bitNumber = ta_getBitNumberForCoil(params[0].value.b);
  coilBitValue = ta_getCoilValue(bitNumber, allCoilData, allCoilDataLen);
  // // modbusCoilValue = coilBitValue ? COIL_VALUE_1 : COIL_VALUE_0;
  modbusCoilValue = coilBitValue;

  params[2].value.a = modbusCoilValue;

  dataToSign[0] = modbusCoilValue;
  dataToSign[1] = params[0].value.a >> 8;
  dataToSign[2] = params[0].value.a & 0x00ff;

  res = ta_sign(dataToSign, dataLen, params[1].memref.buffer, (int *)&params[1].memref.size);
  
  return res;
}

TEE_Result ta_setDataByCoilAddress(
  TEE_Param params[4]
){
  TEE_Result res;

  int signatureVerified = 0;

  uint8_t allCoilData[ARRAY_SIZE(coilData)] = {0x00};
  int allCoilDataLen = ARRAY_SIZE(coilData);
  uint32_t nb;

  int bitNumber = 0;
  
  res = ta_verify(
    params[1].memref.buffer, params[1].memref.size,
    params[2].memref.buffer, params[2].memref.size,
    &signatureVerified
  );

  params[3].value.a = signatureVerified;
  if(signatureVerified == -1) {
    return res;
  }

  res = ta_getAllCoilData_unpacked(allCoilData, allCoilDataLen, &nb);
  bitNumber = ta_getBitNumberForCoil(params[0].value.a);
  ta_updateCoilData(bitNumber, params[0].value.b, allCoilData);
  res = ta_overwriteCoilWithData((void*)allCoilData, allCoilDataLen);

  return res;
}

TEE_Result ta_verifyCoilDAta(
  TEE_Param params[4]
){
  return ta_verify(
    params[0].memref.buffer, params[0].memref.size,
    params[1].memref.buffer, params[1].memref.size,
    (int *)&params[2].value.a
  );
}

TEE_Result ta_signCoilData(
  TEE_Param params[4]
){
  return ta_sign(
    params[0].memref.buffer, params[0].memref.size,
    params[1].memref.buffer, (int *)&params[1].memref.size
  );
}



TEE_Result ta_sign(
  uint8_t *dataToSign, int dataLen, uint8_t *signature_ptr, int *signatureLen_ptr
){
  return ta_signAndVerify(TEE_MODE_SIGN, dataToSign, dataLen, signature_ptr, signatureLen_ptr, NULL);
}

TEE_Result ta_verify(
  uint8_t *dataToSign, int dataLen, uint8_t *signature_ptr, int signatureLen, int * signatureVerified
){
  return ta_signAndVerify(TEE_MODE_VERIFY, dataToSign, dataLen, signature_ptr, &signatureLen, signatureVerified);
}

TEE_Result ta_signAndVerify(
  TEE_OperationMode mode,
  uint8_t *dataToSign, int dataLen,
  uint8_t *signature_ptr, int *signatureLen_ptr, 
  int *signatureVerified
){
  TEE_Result res;

  TEE_OperationHandle operationHandle = TEE_HANDLE_NULL;
  
  uint32_t hash_algo = TEE_ALG_SHA256;
  
  int8_t ptx_hash[64];
  size_t ptx_hash_size = 0;

  TEE_Attribute algo_params[1];
  size_t num_algo_params = 0;

  size_t max_key_size = 0;

  TEE_Attribute key_attrs[8];
  size_t num_key_attrs = 0;

  TEE_ObjectHandle pub_key_handle = TEE_HANDLE_NULL;
  TEE_ObjectHandle priv_key_handle = TEE_HANDLE_NULL;

  bool boolRes; 
  
  size_t out_size;
  uint8_t out[512];
  
  uint32_t main_algo = TEE_ALG_RSASSA_PKCS1_V1_5_SHA256;

  size_t loop;

  (void) boolRes;
  
  max_key_size = 0;
  res = ta_allocateOperation(&operationHandle, hash_algo, TEE_MODE_DIGEST, max_key_size);

  ptx_hash_size = sizeof(ptx_hash);

  res = ta_digestDoFinal(operationHandle, (void*)dataToSign, dataLen, (void*)ptx_hash, &ptx_hash_size);

  algo_params[0].attributeID = TEE_ATTR_RSA_PSS_SALT_LENGTH;
  algo_params[0].content.value.a = 0;
  algo_params[0].content.value.b = 0;
  num_algo_params = 1;

  max_key_size = ARRAY_SIZE(data_modulus) * 8;

  xtest_add_attr(&num_key_attrs, key_attrs,
    TEE_ATTR_RSA_MODULUS,
    data_modulus,
    ARRAY_SIZE(data_modulus));
  xtest_add_attr(&num_key_attrs, key_attrs,
    TEE_ATTR_RSA_PUBLIC_EXPONENT,
    data_pub_exp,
    ARRAY_SIZE(data_pub_exp));

  boolRes = create_key(
    max_key_size,
    TEE_TYPE_RSA_PUBLIC_KEY,
    key_attrs, num_key_attrs,
    &pub_key_handle);

  xtest_add_attr(&num_key_attrs, key_attrs,
    TEE_ATTR_RSA_PRIVATE_EXPONENT,
    data_priv_exp,
    ARRAY_SIZE(data_priv_exp));

  create_key(
    max_key_size,
    TEE_TYPE_RSA_KEYPAIR,
    key_attrs, num_key_attrs,
    &priv_key_handle);

  out_size = sizeof(out);
  memset(out, 0, sizeof(out));

  switch (mode) {
    case TEE_MODE_VERIFY:

      res = ta_allocateOperation(&operationHandle, main_algo, TEE_MODE_VERIFY, max_key_size);

      res = ta_setOperationKey(operationHandle, pub_key_handle);

      res = ta_freeTransientObject(pub_key_handle);

      pub_key_handle = TEE_HANDLE_NULL;

      res = ta_asymmetricVerifyDigest(
        operationHandle,
        algo_params, num_algo_params,
        ptx_hash, ptx_hash_size,
        signature_ptr, *signatureLen_ptr
      );
      if (res == TEE_SUCCESS) {
        *signatureVerified = 1;
      } else {
        *signatureVerified = -1;
      }

      break;

    case TEE_MODE_SIGN:
      res = ta_allocateOperation(&operationHandle, main_algo, TEE_MODE_SIGN, max_key_size);

      res = ta_setOperationKey(operationHandle, priv_key_handle);

      res = ta_freeTransientObject(priv_key_handle);
      
      priv_key_handle = TEE_HANDLE_NULL;

      res = ta_asymmetricSignDigest(
        operationHandle,
        algo_params, num_algo_params,
        ptx_hash, ptx_hash_size,
        out, &out_size
      );

      for(loop = 0; loop < out_size; loop++) {
        signature_ptr[loop] = out[loop];
      }
      *signatureLen_ptr = out_size;
      
      break;

    default:
      break;
  }

  res = ta_freeOperation(operationHandle);

  pub_key_handle = TEE_HANDLE_NULL;

  priv_key_handle = TEE_HANDLE_NULL;

  return res;
}

bool create_key(
  uint32_t max_key_size,
  uint32_t key_type,
  TEE_Attribute *attrs, size_t num_attrs,
  TEE_ObjectHandle *handle
){
  TEE_Result res;
  res = ta_allocateTransientObject(key_type, max_key_size, handle);
  res = ta_populateTransientObject(*handle, attrs, num_attrs);
  return res;
}

/*
UTIL FUNCTIONS
*/
int ta_getBitNumberForCoil(
  int coilAddress
) {
  switch(coilAddress) {
    case COIL_ADDRESS_INVERTER_STATUS:
      return BIT_ADDRESS_INVERTER_STATUS;
    default:
      return -1;  
  }
}

uint8_t ta_getCoilValue(
  int bitNumber, uint8_t *coilData_ptr, int coilDataLen
){
  int bitNumberAddressIndex;
  int bitNumberAddressOffset;
  int bitValue;

  bitNumberAddressIndex = bitNumber/8;
  bitNumberAddressOffset = (7 - bitNumber%8);

  (void)coilDataLen;
  // Check
  // if(bitNumberAddressIndex >= coilDataLen) {
  //  return -1;
  // }

  bitValue = (coilData_ptr[bitNumberAddressIndex] >> bitNumberAddressOffset) & 0x01;
  
  return bitValue;
}

int ta_updateCoilData(int bitNumber, uint8_t coilValue, uint8_t * allCoilData) {
  int bitNumberAddressIndex;
  int bitNumberAddressOffset;

  bitNumberAddressIndex = bitNumber/8;
  bitNumberAddressOffset = (7 - bitNumber%8);

  // Check
  // if(bitNumberAddressIndex >= coilDataLen) {
  //  return -1;
  // }
  if(coilValue == 0) {
    CLEAR_BIT(allCoilData[bitNumberAddressIndex], bitNumberAddressOffset);
  } else {
    SET_BIT(allCoilData[bitNumberAddressIndex], bitNumberAddressOffset);
  }
  return 0;
}

void xtest_add_attr(
  size_t *attr_count, TEE_Attribute *attrs, uint32_t attr_id,
  const void *buf, size_t len
) {
  attrs[*attr_count].attributeID = attr_id;
  attrs[*attr_count].content.ref.buffer = (void *)buf;
  attrs[*attr_count].content.ref.length = len;
  (*attr_count)++;
}

/*
TEE API FUNCTIONS
*/
/****************************** PERSISTENT OBJECT FUNCTIONS ******************************/
TEE_Result ta_createPersistentObject(
  uint32_t storageId,
  void* objectID, uint32_t objectIDLen,
  uint32_t flags, TEE_ObjectHandle attributes,
  void* initialData, uint32_t initialDataLen,
  TEE_ObjectHandle *object
) {
  TEE_Result res;
  TEE_ObjectHandle o;

  DMSG("\nTA_MESSAGE: CREATING PERSISTENT OBJECT\n");
  (void)object;

  res = TEE_CreatePersistentObject(
      storageId,
      objectID, objectIDLen,
      flags,
      attributes,
      initialData, initialDataLen,
      &o
    );
  if(res != TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: CREATING PERSISTENT OBJECT FAILED\n");
  }
  if(res == TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: CREATING PERSISTENT OBJECT SUCCESS\n");
    *object = o;
  }
  return res;
}

TEE_Result ta_openPersistentObject(
  uint32_t storageId,
  void* objectID, uint32_t objectIDLen,
  uint32_t flags,
  TEE_ObjectHandle *object
){
  TEE_Result res;
  TEE_ObjectHandle o;

  DMSG("\nTA_MESSAGE: OPENING PERSISTENT OBJECT\n");
  
  (void)object;

  res = TEE_OpenPersistentObject(
      storageId,
      objectID, objectIDLen,
      flags,
      &o
    );

  if(res != TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: OPENING PERSISTENT OBJECT FAILED\n");
  }
  if(res == TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: OPENING PERSISTENT OBJECT SUCCESS\n");
    *object = o;
  }

  return res;
}

TEE_Result ta_readPersistentObject(
  TEE_ObjectHandle object,
  void* buffer, uint32_t size,
  uint32_t *count
){
  TEE_Result res;

  DMSG("\nTA_MESSAGE: READING PERSISTENT OBJECT\n");

  res = TEE_ReadObjectData(
      object,
      buffer, size,
      count
    );

  if(res != TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: READING PERSISTENT OBJECT FAILED\n");
  }
  if(res == TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: READING PERSISTENT OBJECT SUCCESS\n");
  }
  
  return res;
}

TEE_Result ta_closePersistentObject(
  TEE_ObjectHandle object
){
  DMSG("\nTA_MESSAGE: CLOSING PERSISTENT OBJECT\n");
  
  TEE_CloseObject((TEE_ObjectHandle)(uintptr_t)object);

  return TEE_SUCCESS;
}

/****************************** TRANSIENT OBJECT FUNCTIONS ******************************/
TEE_Result ta_allocateTransientObject(
  uint32_t objectType,
  uint32_t maxObjectSize,
  TEE_ObjectHandle* object
){
  TEE_Result res;
  TEE_ObjectHandle o;

  DMSG("\nTA_MESSAGE: ALLOCATE TRANSIENT OBJECT\n");
  
  res = TEE_AllocateTransientObject(objectType, maxObjectSize, &o);
  if(res != TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: ALLOCATE TRANSIENT OBJECT FAILED\n");
  }
  if(res == TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: ALLOCATE TRANSIENT OBJECT SUCCESS\n");
    *object = o;
  }

  return res;
}

TEE_Result ta_populateTransientObject(
  TEE_ObjectHandle object,
  TEE_Attribute* attrs, uint32_t attrCount
){
  TEE_Result res;
  
  DMSG("\nTA_MESSAGE: POPULATE TRANSIENT OBJECT\n");

  res = TEE_PopulateTransientObject(object, attrs, attrCount);
  if(res != TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: POPULATE TRANSIENT OBJECT FAILED\n");
  }
  if(res == TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: POPULATE TRANSIENT OBJECT SUCCESS\n");
  }

  return res;
}


TEE_Result ta_freeTransientObject(
  TEE_ObjectHandle object
){
  DMSG("\nTA_MESSAGE: FREE TRANSIENT OBJECT\n");
  
  TEE_FreeTransientObject(object);
  return TEE_SUCCESS;
}

/****************************** COMMON CRYPTO FUNCTIONS ******************************/
TEE_Result ta_allocateOperation(
  TEE_OperationHandle* operation,
  uint32_t algorithm,
  uint32_t mode,
  uint32_t maxKeySize
) {
  TEE_Result res;
  TEE_OperationHandle oph;

  DMSG("\nTA_MESSAGE: ALLOCATE CRYPTO OPERATION\n");

  res = TEE_AllocateOperation(&oph, algorithm, mode, maxKeySize);

  if(res != TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: ALLOCATE CRYPTO OPERATION FAILED\n");
  }
  if(res == TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: ALLOCATE CRYPTO OPERATION SUCCESS\n");
    *operation = oph;
  }

  return res;
}

TEE_Result ta_freeOperation(
  TEE_OperationHandle operation
){
  DMSG("\nTA_MESSAGE: FREE CRYPTO OPERATION\n");

  TEE_FreeOperation(operation);
  return TEE_SUCCESS;
}


TEE_Result ta_setOperationKey(
  TEE_OperationHandle operation,
  TEE_ObjectHandle key
){
  TEE_Result res;

  DMSG("\nTA_MESSAGE: SET OPERATION KEY OPERATION\n");
  
  res = TEE_SetOperationKey(operation, key);

  if(res != TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: SET OPERATION KEY OPERATION FAILED\n");
  }
  if(res == TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: SET OPERATION KEY OPERATION SUCCESS\n");
  }

  return res;
}

/****************************** SHA CRYPTO FUNCTIONS ******************************/
TEE_Result ta_digestDoFinal(
  TEE_OperationHandle operation,
  void* chunk, uint32_t chunkLen,
  void* hash, uint32_t* hashLen
) {
  TEE_Result res;
  
  DMSG("\nTA_MESSAGE: CALCULATE DIGEST OPERATION\n");

  res = TEE_DigestDoFinal(operation, chunk, chunkLen, hash, hashLen);

  if(res != TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: CALCULATE DIGEST OPERATION FAILED\n");
  }
  if(res == TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: CALCULATE DIGEST OPERATION SUCCESS\n");
  }

  return res;
}

/****************************** ASYMMMETRIC CRYPTO FUNCTIONS ******************************/
TEE_Result ta_asymmetricVerifyDigest(
  TEE_OperationHandle operation,
  TEE_Attribute* params, uint32_t paramCount,
  void* digest, uint32_t digestLen,
  void* signature, uint32_t signatureLen
){
  TEE_Result res;

  DMSG("\nTA_MESSAGE: SIGN VERIFY OPERATION\n");

  res = TEE_AsymmetricVerifyDigest(operation, params, paramCount, digest, digestLen, signature, signatureLen);
  
  if(res != TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: SIGN VERIFY OPERATION FAILED\n");
  }
  if(res == TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: SIGN VERIFY OPERATION SUCCESS\n");
  }

  return res;
}

TEE_Result ta_asymmetricSignDigest(
  TEE_OperationHandle operation,
  TEE_Attribute* params, uint32_t paramCount,
  void* digest, uint32_t digestLen,
  void* signature, uint32_t *signatureLen
){
  TEE_Result res;

  DMSG("\nTA_MESSAGE: SIGN DIGEST OPERATION\n");

  res = TEE_AsymmetricSignDigest(operation, params, paramCount, digest, digestLen, signature, signatureLen);
  
  if(res != TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: SIGN DIGEST OPERATION FAILED\n");
  }
  if(res == TEE_SUCCESS) {
    DMSG("\nTA_MESSAGE: SIGN DIGEST OPERATION SUCCESS\n");
  }

  return res;
}

/*
 * Called when a TA is invoked. sess_ctx hold that value that was
 * assigned by TA_OpenSessionEntryPoint(). The rest of the paramters
 * comes from normal world.
 */
TEE_Result TA_InvokeCommandEntryPoint(
  void *pSessionContext,
  uint32_t nCommandID, uint32_t nParamTypes,
  TEE_Param pParams[4]
) {
  (void)&pSessionContext; /* Unused parameter */
  (void)nParamTypes; /* Unused parameter */

  #ifdef DEBUG
  #if DEBUG > 0
  DMSG("\n\n\nTA_InvokeCommandEntryPoint called\n");
  #endif
  #endif

  switch (nCommandID) {
    case TA_SEED_COIL:
      return ta_seedCoil();
    case TA_GET_ALL_COIL_DATA:
      return ta_getAllCoilData(pParams);
    case TA_GET_DATA_BY_COIL_ADDRESS:
      return ta_getDataByCoilAddress(pParams);
    case TA_SET_DATA_BY_COIL_ADDRESS:   
      return ta_setDataByCoilAddress(pParams);
    case TA_VERIFY_COIL_DATA:
      return ta_verifyCoilDAta(pParams);
    case TA_SIGN_COIL_DATA:
      return ta_signCoilData(pParams);


    default:
      return TEE_ERROR_BAD_PARAMETERS;
  }
}
