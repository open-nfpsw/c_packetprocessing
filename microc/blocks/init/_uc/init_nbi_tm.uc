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
 * @file:  init_nbi_tm.uc
 * @brief: NBI Traffic Manager initialisation settings
 *
 */

#ifndef _INIT_NBI_TM_UC_
#define _INIT_NBI_TM_UC_

#include "../init_config.h"
#include "init_nbi_tm_csr.uc"

#ifndef NBI_TM_NUM_SEQUENCERS
    #error "NBI_TM_NUM_SEQUENCERS not defined"
#endif
#ifndef NBI_TM_ENABLE_SEQUENCER0
    #error "NBI_TM_ENABLE_SEQUENCER0 not defined"
#endif
#ifndef NBI_TM_CHANNEL_LEVEL_SELECT
    #error "NBI_TM_CHANNEL_LEVEL_SELECT not defined"
#endif
#ifndef NBI_TM_L1_INPUT_SELECT
    #error "NBI_TM_L1_INPUT_SELECT not defined"
#endif


/** Nbi_TrafficManager_Init
 *
 * Traffic Manager initialisation
 *
 * @param NBI_ID        NBI number, can be 0 or 1
 * @param QUEUE_NUM     Number of queues
 */
#macro Nbi_TrafficManager_Init(NBI_ID, H0_Q_NUM, H1_Q_NUM)
    #if (NBI_ID < 0) || (NBI_ID > 1)
        #error "NBI_ID can only be 0 or 1"
    #endif
    #if ((H0_Q_NUM + H1_Q_NUM) > 1024)
        #error "Number of queues cannot be greater than 1024"
    #endif
    #if ((H0_Q_NUM + H1_Q_NUM) > 128)
        #if ((NBI_TM_CHANNEL_LEVEL_SELECT == 1) && (NBI_TM_L1_INPUT_SELECT == 0))
            #error "Can't use more then 128 Queues when ChannelLevelSelect=0 and L1InputSelect=0"
        #endif
    #endif

    #define_eval NBI_TM_INIT_ISLAND_ID (NBI_ID+8)
    Nbi_TrafficManager_TrafficManagerReg_TrafficManagerConfig(NBI_ID,
    NBI_TM_CHANNEL_LEVEL_SELECT,//ChannelLevelSelect
    0,                          //ErrPoisonEna
    0,                          //L0ChannelMap
    NBI_TM_L1_INPUT_SELECT,     //L1InputSelect
    1,                          //MiniPacketFCEnable
    NBI_TM_NUM_SEQUENCERS,      //NumSequencers
    0,                          //OOBFCEnable
    1,                          //SchedulerEnable
    NBI_TM_ENABLE_SEQUENCER0,   //Sequencer0Enable
    0                           //ShaperEnable
    )


    #define_eval QUEUE_SIZE (LOG2((16384 / (H0_Q_NUM + H1_Q_NUM)), -1))
    #if (QUEUE_SIZE > 0xe)
        #define_eval QUEUE_SIZE (0xe)
    #elif (QUEUE_SIZE < 0x3)
        #define_eval QUEUE_SIZE (0x3)
    #endif

    #define_eval NBI_TM_Q (0)
    #while (NBI_TM_Q < H0_Q_NUM)

        Nbi_TrafficManager_TMQueueReg_QueueConfig(NBI_ID,NBI_TM_Q,
        1,    //QueueEnable
        1,    //DropEnable
        0,    //REDEnable
        0,    //DMAPacketThreshold
        QUEUE_SIZE,    //QueueSize
        0     //DropRateRangeSelect
        )
        NBITMCPP_TMHeadTailSram_TMHeadTailEntry(NBI_TM_INIT_ISLAND_ID,NBI_TM_Q,
        (NBI_TM_Q << QUEUE_SIZE),    //TailPointer
        (NBI_TM_Q << QUEUE_SIZE)     //HeadPointer
        )
        Nbi_TrafficManager_TMQueueReg_QueueDropCountClear(NBI_ID,NBI_TM_Q)
        #define_eval NBI_TM_Q (NBI_TM_Q + 1)

    #endloop

    #define_eval NBI_TM_Q (512)
    #while (NBI_TM_Q < (512 + H1_Q_NUM))

        Nbi_TrafficManager_TMQueueReg_QueueConfig(NBI_ID,NBI_TM_Q,
        1,    //QueueEnable
        1,    //DropEnable
        0,    //REDEnable
        0,    //DMAPacketThreshold
        QUEUE_SIZE,    //QueueSize
        0     //DropRateRangeSelect
        )
        NBITMCPP_TMHeadTailSram_TMHeadTailEntry(NBI_TM_INIT_ISLAND_ID,NBI_TM_Q,
        (NBI_TM_Q << QUEUE_SIZE),    //TailPointer
        (NBI_TM_Q << QUEUE_SIZE)     //HeadPointer
        )
        Nbi_TrafficManager_TMQueueReg_QueueDropCountClear(NBI_ID,NBI_TM_Q)
        #define_eval NBI_TM_Q (NBI_TM_Q + 1)

    #endloop


    Nbi_TrafficManager_TrafficManagerReg_MiniPktCreditConfig(NBI_ID,
    37,      //CreditLimit
    0x200,          //FPCreditLimit
    1               //MiniPacketFCMode
    )

    Nbi_TrafficManager_TrafficManager_TrafficManagerReg_BlqEvent(NBI_ID,
    0xf,           //BLQEventStatusEnable
    0,             //BLQEventNotEmptyEnable
    0,             //BLQEventThreshold0
    0,             //BLQEventThreshold1
    0,             //BLQEventThreshold2
    0,             //BLQEventThreshold3
    0              //DisBLQEventFifoFullAssert
    )

    #undef NBI_TM_Q
    #undef NBI_TM_INIT_ISLAND_ID
#endm


#endif /* _INIT_NBI_TM_UC_ */
