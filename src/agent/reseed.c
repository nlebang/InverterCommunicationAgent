/*
 * Copyright © 2017 Utkarsh Agrawal <utkarsh.agrawal@wsu.edu> <uautkarsh@gmail.com>
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "invCtrl.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

int main() {
  uint8_t signature[512] = {0x00};
  int signatureLen = 0;
  int signatureVerified = 0;

  uint8_t coilValue;

  int loop;

  printf("SEEDING DATA...\n");
  seed();
  printf("Done...\n\n");
  

  // printf("Server: Requesting TA for value of coil at address 0x13 and its signature...\n");
  // getDataByCoilAddress(0x0001, 0x13, &coilValue, signature, &signatureLen);
  
  // printf("Value of Coil at Address 0x13: %.2X\n", coilValue);
  // printf("Signature:\n");
  // for(loop = 0; loop < signatureLen; loop++) {
  //  printf("%.2X ", signature[loop]);
  // }
  // printf("\n");
  // printf("Done...\n\n");
  
  // printf("Client: Verifying Value of coil with its signature...\n");
  // signatureVerified = 0;
  // verifyCoilData1(0x0001, coilValue, signature, signatureLen, &signatureVerified);
  // printf(signatureVerified ? "Signature Verified\n" : "Signature Could not be Verified\n");
  // printf("Done...\n\n");
  
  // printf("Client: Instruct server to set value of coil at address 0x13 with 0...\n");
  // printf("Signature for server:\n");
  // memset(signature, 0, 512);
  // signCoilValue1(0x0001, 0, signature, &signatureLen);
  // for(loop = 0; loop < signatureLen; loop++) {
  //  printf("%.2X ", signature[loop]);
  // }
  // printf("\n");
  // printf("Done...\n\n");
  
  // printf("Server: Set value of coil at address 0x13 with 0...\n");
  // setCoilData1(0x0001, 0x13, 0, signature, 128);
  // printf("Done...\n\n");


  // printf("Server: Requesting TA for value of coil at address 0x13 and its signature...\n");
  // getDataByCoilAddress(0x0001, 0x13, &coilValue, signature, &signatureLen);
  // printf("Done...\n\n");
  
  // printf("Client: Verifying Value of coil with its signature...\n");
  // signatureVerified = 0;
  // verifyCoilData1(0x0001, coilValue, signature, signatureLen, &signatureVerified);
  // printf(signatureVerified ? "Signature Verified\n" : "Signature Could not be Verified\n");
  // printf("Value of Coil at Address 0x13: %.2X\n", coilValue);
  // printf("Done...\n\n");






  // printf("Client: Instruct server to set value of coil at address 0x13 with 1...\n");
  // printf("Signature for server:\n");
  // memset(signature, 0, 512);
  // signCoilValue1(0x0001, 1, signature, &signatureLen);
  // for(loop = 0; loop < signatureLen; loop++) {
  //  printf("%.2X ", signature[loop]);
  // }
  // printf("\n");
  // printf("Done...\n\n");
  
  // printf("Server: Set value of coil at address 0x13 with 1...\n");
  // setCoilData1(0x0001, 0x13, 1, signature, 128);
  // printf("Done...\n\n");


  // printf("Server: Requesting TA for value of coil at address 0x13 and its signature...\n");
  // getDataByCoilAddress(0x0001, 0x13, &coilValue, signature, &signatureLen);
  // printf("Done...\n\n");
  
  // printf("Client: Verifying Value of coil with its signature...\n");
  // signatureVerified = 0;
  // verifyCoilData1(0x0001, coilValue, signature, signatureLen, &signatureVerified);
  // printf(signatureVerified ? "Signature Verified\n" : "Signature Could not be Verified\n");
  // printf("Value of Coil at Address 0x13: %.2X\n", coilValue);
  // printf("Done...\n\n");

  return 0;
}

// int main1() {
//  // uint8_t data[] = {
//  //  0x45, 0x6e, 0x74, 0x65, 0x72, 0x20, 0x74, 0x68, 0x65, 0x20, 0x6d, 0x61,
//  //  0x74, 0x72, 0x69, 0x78
//  // };

//  uint8_t coilData[512] = {0x00};
//  int coilDataLen = 0;

//  int loop;

//  printf("POPULATING DATA\n");
//  populateCoilData();

//  printf("Fetching all coil data\n");
//  getAllCoilData(coilData, &coilDataLen);
//  for(loop = 0; loop < coilDataLen; loop++) {
//    printf("%.2X ", coilData[loop]);
//  }
//  // testFunction(data, ARRAY_SIZE(data), signature, &signatureLen);

//  // for(loop = 0; loop < signatureLen; loop++) {
//  //  printf("%.2X ", signature[loop]);
//  // }

//  return 0;
// }

// #include <stdio.h>
// #include <err.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <stdbool.h>
// #include <string.h>

// int main() {
//  printf("LOL\n");

//  return 0;
// }