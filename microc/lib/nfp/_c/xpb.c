/*
 * Copyright (C) 2014-2015,  Netronome Systems, Inc.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @file          lib/nfp/_c/xpb.c
 * @brief         XPB related functions
 */

#include <assert.h>
#include <nfp.h>
#include <nfp/xpb.h>

__intrinsic unsigned int
xpb_read(unsigned int addr)
{
    SIGNAL sig;
    __xread unsigned int xfer;

    __asm ct[xpb_read, *xfer, addr, 0, 1], ctx_swap[sig]

    return xfer;
}

__intrinsic void
xpb_write(unsigned int addr, unsigned int val)
{
    SIGNAL sig;
    __xwrite unsigned int xfer = val;

    __asm ct[xpb_write, *xfer, addr, 0, 1], ctx_swap[sig]
}
