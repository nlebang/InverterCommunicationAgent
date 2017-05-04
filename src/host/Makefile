# Copyright © 2017 Utkarsh Agrawal <utkarsh.agrawal@wsu.edu> <uautkarsh@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

CC      = $(CROSS_COMPILE)gcc
LD      = $(CROSS_COMPILE)ld
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
READELF = $(CROSS_COMPILE)readelf

OBJS = inv_ctrl_host.o

CFLAGS += -Wall -fPIC -I../ta/include -I$(TEEC_EXPORT)/include -I./include

LDADD += -lteec -L$(TEEC_EXPORT)/lib

BINARY=inv_ctrl_host

.PHONY: all
all: $(BINARY)

$(BINARY): $(OBJS)
	ar rcv lib$(BINARY).a $(OBJS)

inv_ctrl_host.o:
	$(CC) $(CFLAGS) -c inv_ctrl_host.c $(INCLUDES) $(LDADD)

.PHONY: clean
clean:
	rm -f $(OBJS) $(BINARY) *.a
