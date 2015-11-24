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
 * @file:   init_config.h
 * @brief:  Configuration which specify the allocation of various resources
 *          across the NFP at firmware load-time. These are meant to be
 *          overridden by a custom version of this header file.
 *
 * @note:   This configuration assumes the use of the the Catamaran Picocode
 *          running on the NBI
 *
 */

#ifndef _INIT_CONFIG_H_
#define _INIT_CONFIG_H_

/*************************************************************************
 * General NFP Configuration
 *
 * These values affect global NFP behaviour, not solely applicable to one
 * component/island of the NFP
 ************************************************************************/

/* Determines how many NBIs to use for traffic.
 * Whether they are physically present is not checked */
#ifndef NBI_COUNT
    #define NBI_COUNT 1
#endif

/*************************************************************************
 * NBI DMA Configuration
 *
 * A large portion of NBI DMA configuration is performed by the Buffer List
 * Manager, and thus we only expose non-BLM related items which one may tweak.
 *
 ************************************************************************/

/* The number of bytes prepended to the start of the packet */
#ifndef NBI_PKT_PREPEND_BYTES
    #define NBI_PKT_PREPEND_BYTES 0
#endif

/* Maxmimum split length boundary between a packets CTM buffer and it's backing
 * IMEM/EMEM buffer:
 * 0 - Split at 256B boundary
 * 1 - Split at 512B boundary
 * 2 - Split at 1024B boundary
 * 3 - Split at 2048B boundary
 */
#ifndef SPLIT_LENGTH
    #define SPLIT_LENGTH 3
#endif

/* Set this to zero if we want the NBI to back-up and start emitting pause
 * frames when buffers are full */
#ifndef NBI_DMA_BP_DROP_ENABLE
    #define NBI_DMA_BP_DROP_ENABLE 0
#endif

/* The allocation of Primary and Secondary Buffer List Queues targets amongst
 * the available Buffer Pools. The NFP offers four available Buffer List Queues
 * per NBI. For targets with muliple NBIs we assume identical NBI
 * configuration.
 *
 * As guidance, note the offical load-balancing the Catamaran Picocode uses:
 *     Buffer Pool 0: Packets <= 192B
 *     Buffer Pool 1: Packets <= 1984B
 *     Buffer Pool 2: Packets > 1984B
 *     Buffer Pool 3: Exceptiond Packets <= 192B
 *     Buffer Pool 4: Exceptiond Packets <= 1984B
 *     Buffer Pool 5: Exceptiond Packets > 1984B
 *     Buffer Pool 6: Unused
 *     Buffer Pool 7: Drop
 *
 *     Syntax:
 *     <Primary Buffer List, Secondary Buffer List>
 */
#ifndef NBI_DMA_BP0_BLQ_TARGET
    #define NBI_DMA_BP0_BLQ_TARGET 0,1
    #define NBI_DMA_BP1_BLQ_TARGET 1,0
    #define NBI_DMA_BP2_BLQ_TARGET 2,3
    #define NBI_DMA_BP3_BLQ_TARGET 0,1
    #define NBI_DMA_BP4_BLQ_TARGET 1,0
    #define NBI_DMA_BP5_BLQ_TARGET 2,3
    #define NBI_DMA_BP6_BLQ_TARGET 2,3
    #define NBI_DMA_BP7_BLQ_TARGET 0,1
#endif

/* Allocation of per-CTM Packet and Buffer Credits shared between CTM and NBI
 *
 *     Here we expose granular config of the credit splits and targeting of
 *     each ME Island. This same mechanism is also used it specify which
 *     islands will recieve traffic directly from the NBI upon ingress.
 *
 *     Syntax:
 *     <RX from NBIx ? 1=yes/0=no, NBIx Packet Credits , NBIx Buffer Credits>
 * */
#ifndef NBI0_DMA_BPE_CONFIG_ME_ISLAND0
    #if (NBI_COUNT == 1)
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND0 1,256,63
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND1 1,256,63
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND2 1,256,63
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND3 1,256,63
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND4 1,256,63
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND5 1,256,63
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND6 0,0,0
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND0 0,0,0
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND1 0,0,0
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND2 0,0,0
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND3 0,0,0
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND4 0,0,0
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND5 0,0,0
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND6 0,0,0
    #elif(NBI_COUNT == 2)
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND0 1,128,31
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND1 1,128,31
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND2 1,128,31
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND3 1,128,31
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND4 1,128,31
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND5 1,128,31
        #define NBI0_DMA_BPE_CONFIG_ME_ISLAND6 1,128,31
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND0 1,128,31
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND1 1,128,31
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND2 1,128,31
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND3 1,128,31
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND4 1,128,31
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND5 1,128,31
        #define NBI1_DMA_BPE_CONFIG_ME_ISLAND6 1,128,31
    #else
        #error "Unexpected NBI_COUNT"
    #endif
#endif

/*************************************************************************
 * NBI Traffic Manager Configuration
 *
 * The following TM Configuration are deemed unnecessary to tweak and have
 * default values:
 *     -Minipacket Credit based Flow Control is enabled.
 *     -Scheduling is enabled.
 *     -MAC out of band flow control is disabled.
 *     -Poisoning of data and other propagation mechanisms in case of errors is
 *      disabled.
 * The following items must be configured at runtime:
 *     -ShaperEnable bit and invidual shaper configurations. Shaping is off at
 *      init time.
 *     -Individual Scheduler settings (weightings, thresholds etc)
 *
 ************************************************************************/

/* Number of TM Queues to be used in each half of the total 1024 available
 * TM Queues.
 * Having each half at 512 will cause all 1024 to be enabled, and each Queue
 * will be of size 16 entries.*/
#ifndef NBI_TM_H_0_Q
    #define NBI_TM_H_0_Q 512
    #define NBI_TM_H_1_Q 512
#endif

/* Packet Sequencing/Ordering.
 *
 * Options:
 *      0: 0  Sequencers - No reordering
 *      1: 1  Sequencers - Reorder 1024 packets
 *      2: 2  Sequencers - Reorder  512 packets per sequencer
 *      3: 4  Sequencers - Reorder  256 packets per sequencer
 *      4: 8  Sequencers - Reorder  128 packets per sequencer
 *      5: 16 Sequencers - Reorder   64 packets per sequencer
 *      6: 32 Sequencers - Reorder   32 packets per sequencer
 * */
#ifndef NBI_TM_NUM_SEQUENCERS
    #define NBI_TM_NUM_SEQUENCERS 0
#endif
/* Keep Sequencer0 off as a target to send packets not requiring ordering */
#ifndef NBI_TM_ENABLE_SEQUENCER0
    #define NBI_TM_ENABLE_SEQUENCER0 0
#endif

/* Channel level select bit.
 *
 * Select the definition of the MAC channel number and the MAC XOFF Flow
 * control to be either the 128 level 0 scheduler ports, or 128 ports on the
 * first 16 level 1 schedulers (or level 2 schedulers depending on the value of
 * NBI_TM_L1_INPUT_SELECT). Note: When NBI_TM_L1_INPUT_SELECT=0 and
 * NBI_TM_CHANNEL_LEVEL_SELECT=1, only 128 queues are allowed to be active in
 * this configuration.
 *
 * 0: MAC Channel number defined to be level 0 ports
 * 1: MAC Channel number defined to be level 1 ports
 * */
#ifndef NBI_TM_CHANNEL_LEVEL_SELECT
    #define NBI_TM_CHANNEL_LEVEL_SELECT 1
#endif

/* Enable/Disable Level 1 schedulers. */
#ifndef NBI_TM_L1_INPUT_SELECT
    #define NBI_TM_L1_INPUT_SELECT 1
#endif


#endif /* _INIT_CONFIG_H_ */
