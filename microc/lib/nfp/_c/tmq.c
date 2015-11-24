/*
 * Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved.
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
 * @file          lib/nfp/tmq.c
 * @brief         TM queue utilities.
 *
 */

#include <nfp6000/nfp_me.h>
#include <assert.h>
#include <nfp/xpb.h>
#include <nfp/mem_bulk.h>
#include <nfp/mem_atomic.h>
#include <nfp/tmq.h>

#define MAX_NBI_NUMBER      1
#define MAX_TM_QUEUE_NUM    1023

/* Base XPB addresses to access the TMQ drop counters */
#define TMQ_XPB_READ_BASE       0x08152000
#define TMQ_XPB_READ_CLEAR_BASE 0x08153000


/* Reads, and optionally clears, a single queue drop counter. */
__intrinsic int
tmq_cnt_read(uint32_t nbi, __gpr uint32_t *counter, uint32_t qnum, int clear)
{
    __gpr uint32_t addr;
    int ret = 0;

    if (nbi > MAX_NBI_NUMBER) {
        ret = -1;
        goto out;
    }

    if (qnum > MAX_TM_QUEUE_NUM) {
        ret = -1;
        goto out;
    }

    if (clear)
        addr = TMQ_XPB_READ_CLEAR_BASE | (nbi << 24) |
               (qnum << 2);
    else
        addr = TMQ_XPB_READ_BASE | (nbi << 24) |
               (qnum << 2);

    *counter = xpb_read(addr);

out:
    return ret;
}
