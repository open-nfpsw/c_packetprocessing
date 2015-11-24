/*
 * Copyright (C) 2012-2015,  Netronome Systems, Inc.  All rights reserved.
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
 * @file          lib/nfp/mem_atomic.h
 * @brief         NFP memory atomics interface
 */
#ifndef _NFP__MEM_ATOMIC_H_
#define _NFP__MEM_ATOMIC_H_

#include <nfp.h>
#include <stdint.h>
#include <types.h>

/**
 * Atomic read/write multiples of 4B in memory (EMEM, IMEM, or CTM).
 * @param data      Pointer to sufficient transfer registers for the op
 * @param addr      40-bit pointer to the memory start address
 * @param size      Size of the op, must be a multiple of 4
 * @param max_size  Used to determine largest op, if size is not a constant
 * @param sync      Type of synchronisation (sig_done or ctx_swap)
 * @param sig       Signal to use
 *
 * These functions provide basic atomic read/write in NFP memory.
 * Atomic read/write functions are used rather than the bulk read/write
 * functions if an address is used by other atomic functions.
 * No special alignment is assumed about address when converting from a 40bit
 * pointer to the "src_op" fields in the __asm command. This makes the function
 * general, but suboptimal if it is guaranteed to be 256B aligned.
 * mem_*_atomic() functions provide a simplified interface where size is
 * assumed to be compile time constant, and the context swaps on an internal
 * signal while waiting for the operation to complete.
 */

__intrinsic void __mem_read_atomic(__xread void *data, __mem void *addr,
                                   size_t size, const size_t max_size,
                                   sync_t sync, SIGNAL *sig);

__intrinsic void mem_read_atomic(__xread void *data,
                                 __mem void *addr, size_t size);

__intrinsic void __mem_write_atomic(__xwrite void *data, __mem void *addr,
                                    size_t size, const size_t max_size,
                                    sync_t sync, SIGNAL *sig);

__intrinsic void mem_write_atomic(__xwrite void *data,
                                  __mem void *addr, size_t size);

/**
 * Atomic increment/decrement of a value in memory (EMEM, IMEM, or CTM).
 * @param addr      40-bit pointer to the value in memory start address
 *
 * These functions increment or decrement a single 32 bit or 64 bit
 * word in NFP memory.
 */
__intrinsic void mem_incr32(__mem void *addr);

__intrinsic void mem_decr32(__mem void *addr);

__intrinsic void mem_incr64(__mem void *addr);

__intrinsic void mem_decr64(__mem void *addr);

/**
 * Atomic add immediate to a value in memory (EMEM, IMEM, or CTM).
 * @param val       16-bit integer
 * @param addr      40-bit pointer to the value in memory start address
 *
 * These functions provide the ability to increment a single 32 bit or 64 bit
 * word in memory by a 16 bit number without the need for transfer registers.
 */
__intrinsic void mem_add32_imm(unsigned int val, __mem void *addr);

__intrinsic void mem_add64_imm(unsigned int val, __mem void *addr);

/**
 * Atomic bit set/clr immediate (EMEM, IMEM, or CTM).
 * @param val       16-bit integer
 * @param addr      40-bit pointer to the value in memory
 *
 * These functions provide the ability to bit set/clr a value in memory
 * without the need for transfer registers.
 */
__intrinsic void mem_bitset_imm(unsigned int val, __mem void *addr);

__intrinsic void mem_bitclr_imm(unsigned int val, __mem void *addr);

/**
 * Atomic add/sub to/from multiple 4B or 8B values (EMEM, IMEM, or CTM).
 * @param data      Pointer to sufficient transfer registers for the op
 * @param addr      40-bit pointer to first value in memory
 * @param size      Size of the op, must be a multiple of 4 or 8
 * @param max_size  Used to determine largest op, if size is not a constant
 * @param sync      Type of synchronisation (sig_done or ctx_swap)
 * @param sig       Signal to use
 *
 * @note add/sub32 size max = 16
 * @note add/sub64 size max = 32
 * These functions provide basic atomic add/sub in NFP memory. No special
 * alignment is assumed about address when converting from a 40bit pointer to
 * the "src_op" fields in the __asm command. This makes the function general,
 * but suboptimal if it is guaranteed to be 256B aligned.
 * mem_*() functions provide a simplified interface where size is
 * assumed to be compile time constant, and the context swaps on an internal
 * signal while waiting for the operation to complete.
 */
__intrinsic void __mem_add32(__xwrite void *data, __mem void *addr,
                             size_t size, const size_t max_size,
                             sync_t sync, SIGNAL *sig);

__intrinsic void mem_add32(__xwrite void *data, __mem void *addr,
                           size_t size);

__intrinsic void __mem_add64(__xwrite void *data, __mem void *addr,
                             size_t size, const size_t max_size,
                             sync_t sync, SIGNAL *sig);

__intrinsic void mem_add64(__xwrite void *data, __mem void *addr,
                           size_t size);

__intrinsic void __mem_sub32(__xwrite void *data, __mem void *addr,
                             size_t size, const size_t max_size,
                             sync_t sync, SIGNAL *sig);

__intrinsic void mem_sub32(__xwrite void *data, __mem void *addr,
                           size_t size);

__intrinsic void __mem_sub64(__xwrite void *data, __mem void *addr,
                             size_t size, const size_t max_size,
                             sync_t sync, SIGNAL *sig);

__intrinsic void mem_sub64(__xwrite void *data, __mem void *addr,
                           size_t size);

/**
 * Atomic set/clear bits in multiple of 4B values (EMEM, IMEM, or CTM).
 * @param data      Pointer to sufficient transfer registers for the op
 * @param addr      40-bit pointer to the first value in memory
 * @param size      Size of the op, must be a multiple of 4
 * @param max_size  Used to determine largest op, if size is not a constant
 * @param sync      Type of synchronisation (sig_done or ctx_swap)
 * @param sig       Signal to use
 *
 * @note size max = 32
 * These functions provide basic atomic bit set/clr in NFP memory.  No special
 * alignment is assumed about address when converting from a 40bit pointer to
 * the "src_op" fields in the __asm command. This makes the function general,
 * but suboptimal if it is guaranteed to be 256B aligned.
 * mem_*() functions provide a simplified interface where size is
 * assumed to be compile time constant, and the context swaps on an internal
 * signal while waiting for the operation to complete.
 */
__intrinsic void __mem_bitset(__xwrite void *data, __mem void *addr,
                              size_t size, const size_t max_size,
                              sync_t sync, SIGNAL *sig);

__intrinsic void mem_bitset(__xwrite void *data,
                            __mem void *addr, size_t size);

__intrinsic void __mem_bitclr(__xwrite void *data, __mem void *addr,
                              size_t size, const size_t max_size,
                              sync_t sync, SIGNAL *sig);

__intrinsic void mem_bitclr(__xwrite void *data,
                            __mem void *addr, size_t size);

/**
 * Atomic test and set/clr bits in multiple of 4B values (EMEM, IMEM, or CTM).
 * @param data      Pointer to sufficient RW transfer registers for the op
 * @param addr      40-bit pointer to the first value in memory
 * @param size      Size of the op, must be a multiple of 4
 * @param max_size  Used to determine largest op, if size is not a constant
 * @param sync      Type of synchronization (must be sig_done)
 * @param sig_pair  Signal pair to use
 *
 * Note: Maximum for @max_size is 32.
 *
 * These functions provide basic atomic bit test and set/clr in NFP EMEM,
 * IMEM or CTM memory types.
 */
__intrinsic void __mem_test_set(__xrw void *data, __mem void *addr,
                                size_t size, const size_t max_size,
                                sync_t sync, SIGNAL_PAIR *sig_pair);

__intrinsic void mem_test_set(__xrw void *data, __mem void *addr,
                              size_t size);

__intrinsic void __mem_test_clr(__xrw void *data, __mem void *addr,
                                size_t size, const size_t max_size,
                                sync_t sync, SIGNAL_PAIR *sig);

__intrinsic void mem_test_clr(__xrw void *data, __mem void *addr,
                              size_t size);

/**
 * Atomic test and add/sub in multiple of 4B values (EMEM, IMEM, or CTM).
 * @param data      Pointer to sufficient RW transfer registers for the op
 * @param addr      40-bit pointer to the first value in memory
 * @param size      Size of the op, must be a multiple of 4
 * @param max_size  Used to determine largest op, if size is not a constant
 * @param sync      Type of synchronization (must be sig_done)
 * @param sig_pair  Signal pair to use
 *
 * Note: Maximum for @max_size is 16.
 *
 * These functions will add or subtract data in NFP EMEM, IMEM or CTM memory
 * types, and return the contents of memory before the operation.
 */
__intrinsic void __mem_test_add(__xrw void *data, __mem void *addr,
                                size_t size, const size_t max_size,
                                sync_t sync, SIGNAL_PAIR *sig);

__intrinsic void mem_test_add(__xrw void *data, __mem void *addr,
                              size_t size);

__intrinsic void __mem_test_sub(__xrw void *data, __mem void *addr,
                                size_t size, const size_t max_size,
                                sync_t sync, SIGNAL_PAIR *sig);

__intrinsic void mem_test_sub(__xrw void *data, __mem void *addr,
                              size_t size);

#endif /* !_NFP__MEM_ATOMIC_H_ */
