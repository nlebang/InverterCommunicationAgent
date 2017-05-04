# Copyright © 2017 Utkarsh Agrawal <utkarsh.agrawal@wsu.edu> <uautkarsh@gmail.com>
# SPDX-License-Identifier: BSD-3-Clause

global-incdirs-y += include
global-incdirs-y += ../host/include
#global-incdirs-y += ../host/include
srcs-y += inv_ctrl_ta.c

# To remove a certain compiler flag, add a line like this
#cflags-template_ta.c-y += -Wno-strict-prototypes
