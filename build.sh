#!/bin/bash

# Copyright © 2017 Utkarsh Agrawal <utkarsh.agrawal@wsu.edu> <uautkarsh@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

CURDIR=`pwd`

# This expects that this is place as a first level folder relative to the other
# OP-TEE folder in a setup using default repo configuration as described by the
# documentation in optee_os (README.md)
ROOT=${PWD}
ROOT=`dirname $ROOT`

# Set this according to your environment
OPTEE_ROOT='/home/wsu/devel/optee'
MODBUS_ROOT=$CURDIR/../libmodbus

# Path to the toolchain
export PATH=$OPTEE_ROOT/toolchains/aarch32/bin:$PATH

# Path to the TA-DEV-KIT coming from optee_os
export TA_DEV_KIT_DIR=$OPTEE_ROOT/optee_os/out/arm/export-ta_arm32

# Path to the client library (GP Client API)
export TEEC_EXPORT=$OPTEE_ROOT/optee_client/out/export

export PLATFORM=vexpress
export PLATFORM_FLAVOR=qemu_virt

# Toolchain prefix for user space code (normal world)
HOST_CROSS_COMPILE=arm-linux-gnueabihf-
# Toolchain prefix for the Trusted Applications
TA_CROSS_COMPILE=arm-linux-gnueabihf-


# Create output directories
cd $CURDIR
rm -rf $CURDIR/output
mkdir -p $CURDIR/output/all
mkdir -p $CURDIR/output/ta
mkdir -p $CURDIR/output/host/headers
mkdir -p $CURDIR/output/libinvctrl/headers
mkdir -p $CURDIR/output/reseed
mkdir -p $CURDIR/output/modbus
mkdir -p $CURDIR/output/ModbusServerClient
# mkdir -p $CURDIR/output/test


# Build the host application
cd $CURDIR/src/host
make CROSS_COMPILE=$HOST_CROSS_COMPILE $@
# Copy the host application
cp -rf libinv_ctrl_host.a $CURDIR/output/host/
cp -rf ./include/*.h $CURDIR/output/host/headers
# Clean up host application
make clean

# Build the Trusted Application
cd $CURDIR/src/ta
make CROSS_COMPILE=$TA_CROSS_COMPILE $@
# Copy trusted application
cp -rf $CURDIR/src/ta/*.ta $CURDIR/output/ta/
# # Copy other files generated for trusted application
# cp -rf $CURDIR/src/ta/*.dmp $CURDIR/output/all/
# cp -rf $CURDIR/src/ta/*.elf $CURDIR/output/all/
# cp -rf $CURDIR/src/ta/*.map $CURDIR/output/all/
# cp -rf $CURDIR/src/ta/*.ta $CURDIR/output/all/
make clean
# Remove other generated files
rm -rf $CURDIR/src/ta/.inv_ctrl_ta.o.cmd
rm -rf $CURDIR/src/ta/.inv_ctrl_ta.o.d
rm -rf $CURDIR/src/ta/.ta.ld.d
rm -rf $CURDIR/src/ta/.user_ta_header.o.cmd
rm -rf $CURDIR/src/ta/.user_ta_header.o.d
rm -rf $CURDIR/src/ta/ta.lds
# Remove generated trusted application files
rm -rf $CURDIR/src/ta/*.dmp
rm -rf $CURDIR/src/ta/*.elf
rm -rf $CURDIR/src/ta/*.map
rm -rf $CURDIR/src/ta/*.ta


# Build the inverter control library
cd $CURDIR/src/agent
make CROSS_COMPILE=$HOST_CROSS_COMPILE
# Copy inverter control files
cp -rf ./invCtrl.h $CURDIR/output/libinvctrl/headers/
cp -rf libinvctrl.so $CURDIR/output/libinvctrl/
# Clean up inverter control library
make clean

# Build the reseed files
cd $CURDIR/src/agent
make other CROSS_COMPILE=$HOST_CROSS_COMPILE
# Copy the reseed files
cp -rf reseed $CURDIR/output/reseed/
# Clean up inverter control library
make clean

# Build Modbus
cd $MODBUS_ROOT
# Configure
./autogen.sh
./configure \
ac_cv_func_malloc_0_nonnull=yes \
--prefix=$CURDIR/output/modbus \
--host=arm-linux-gnueabihf \
LDFLAGS='-L'"$CURDIR"'/output/libinvctrl -L'"$TEEC_EXPORT"'/lib' \
LIBS='-linvctrl -lteec' \
CPPFLAGS='-I'"$CURDIR"'/output/libinvctrl/headers -I'"$TEEC_EXPORT"'/include'
# Make and install
make
make install

# Build Modbus Server and Client
cd $CURDIR/src/ModbusServerClient
arm-linux-gnueabihf-gcc server.c -o server \
\
-I$CURDIR/output/modbus/include/modbus \
-I$TEEC_EXPORT/include \
\
-L$CURDIR/output/modbus/lib \
-L$CURDIR/output/libinvctrl \
-L$TEEC_EXPORT/lib \
\
-lmodbus -lteec -linvctrl 

arm-linux-gnueabihf-gcc client.c -o client \
\
-I$CURDIR/output/modbus/include/modbus \
-I$TEEC_EXPORT/include \
\
-L$CURDIR/output/modbus/lib \
-L$CURDIR/output/libinvctrl \
-L$TEEC_EXPORT/lib \
\
-lmodbus -lteec -linvctrl 
# Copy Modbus Server and Client
cp -rf server $CURDIR/output/ModbusServerClient/
cp -rf client $CURDIR/output/ModbusServerClient/
# Clean up Modbus Server and Client
rm -rf server client

cd $CURDIR/src/
cp -rf run.sh $CURDIR/output/

rm -rf $OPTEE_ROOT/myPrograms/
mkdir -p $OPTEE_ROOT/myPrograms/
cp -rf $CURDIR/output $OPTEE_ROOT/myPrograms/