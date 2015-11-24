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
 * @file          libblm_pkt_fl.h
 * @brief         Buffers alloc and free using BLM EMU rings.
 *
 * A Micro-c implementation for buffer allocation and free from the BLM
 * EMU rings.
 * see blm_iface_c.h for RING IDs allocation and for Ring memory
 * allocation.
 * The BLM code will initialize the rings (i.e. write the queue descriptors)
 * when the compile time swith "BLM_INIT_EMU_RINGS" is defined.
 *
 */

#ifndef __LIBBLM_PKT_FL_H__
#define __LIBBLM_PKT_FL_H__

/* flowenv dependency */
#include <nfp/mem_ring.h>

/**
 * This file contains the API for accessing BLM-managed freelists of
 * MU buffers.  A client would typically use blm_buf_alloc() to allocate
 * an MU buffer and blm_buf_free() to release it.  The _bulk() variants
 * allow the user to allocate and free buffers in batches.  These APIs
 * do not maintain any local state in the client microengine and do
 * not require any explicit initialization.
 */

/* Not supporting SPLIT_EMU configuration */
#ifdef SPLIT_EMU_RINGS
    #error "SPLIT_EMU_RINGS configuration not supported."
#endif

/*
 * For optimization reasons the following assumptions are made:
 *  1. BLM ring IDs are always allocated in an ordered sequance.
 *  2. All BLM rings for a given NBI are allocated in the same island.
 *  3. BLM rings are big enough to never overflow.
 */

/* blm buffer type */
typedef unsigned int blm_buf_handle_t;

/* convert a blm buffer handle to a 40 bit address */
#define blm_buf_handle2ptr(_bh) (__addr40 __mem void *)\
                                ((unsigned long long)(_bh) << 11)

/* convert a 40 bit address to a blm buffer handle */
#define blm_buf_ptr2handle(_bp) (blm_buf_handle_t)\
                                ((unsigned long long)(_bp) >> 11)

/**
 * Allocate a single buffer from the BLM EMU rings.
 *
 * @param buf       [out] Transfer read register to hold the buffer handle.
 * @param blq       [in]  The blq number to allocate from (0-3).
 * @param sigpair   [in]  Signal pair to use
 * @param sync      [in]  Sync type to use (sig_done or sig_wait)
 *
 * @return - 0 on success, -1 on failure.
 */
__intrinsic int __blm_buf_alloc(__xread blm_buf_handle_t *buf,
                                unsigned int blq, SIGNAL_PAIR *sigpair,
                                sync_t sync);

/**
 * Allocate a single buffer from the BLM EMU rings.
 *
 * @param buf   [out] Transfer read register to hold the buffer handle.
 * @param blq   [in]  The blq number to allocate from (0-3).
 *
 * @return - 0 on success, -1 on failure.
 */
__intrinsic int blm_buf_alloc(__xread blm_buf_handle_t *buf,
                              unsigned int blq);

/**
 * Allocate a bulk of buffers from the BLM EMU rings.
 *
 * @param bufs      [out] Packet buffer transfer register array.
 * @param count     [in]  Number of buffers to allocate
 * @param blq       [in]  The blq number to allocate from (0-3).
 * @param sigpair   [in]  Signal pair to use
 * @param sync      [in]  Sync type to use (sig_done or sig_wait)
 *
 * @return - 0 on success, -1 on failure.
 */
__intrinsic int __blm_buf_alloc_bulk(__xread blm_buf_handle_t *bufs,
                                     unsigned int count, unsigned int blq,
                                     SIGNAL_PAIR *sigpair, sync_t sync);

/**
 * Allocate an array of buffers from the BLM EMU rings.
 *
 * @param bufs  [out] Packet buffer transfer register array.
 * @param count [in]  Number of buffers to allocate
 * @param blq   [in]  The blq number to allocate from (0-3).
 *
 * @return - 0 on success, -1 on failure.
 */
__intrinsic int blm_buf_alloc_bulk(__xread blm_buf_handle_t *bufs,
                                   unsigned int count, unsigned int blq);

/**
 * Free a single buffer back into the BLM EMU rings.
 *
 * @param buf   [in] The buffer handle to free.
 * @param blq   [in] The blq number to free into (0-3).
 */
__intrinsic void blm_buf_free(blm_buf_handle_t buf, unsigned int blq);

/**
 * Free an array of buffers back into the BLM EMU rings.
 *
 * @param buf   [in] The buffers array to free.
 * @param count [in] Number of buffers to free.
 * @param blq   [in] The blq number to free into (0-3).
 * @param sig   [in] Signal to use
 * @param sync  [in] Sync type to use (sig_done or sig_wait)
 */
__intrinsic void __blm_buf_free_bulk(__xwrite blm_buf_handle_t *bufs,
                                     unsigned int count, unsigned int blq,
                                     SIGNAL *sig, sync_t sync);

/**
 * Free an array of buffers back into the BLM EMU rings.
 *
 * @param bufs  [in] The buffers array to free.
 * @param count [in] Number of buffers to free.
 * @param blq   [in] The blq number to free into (0-3).
 */
__intrinsic void blm_buf_free_bulk(__xwrite blm_buf_handle_t *bufs,
                                   unsigned int count, unsigned int blq);

#endif /* __LIBBLM_PKT_FL_H__ */
