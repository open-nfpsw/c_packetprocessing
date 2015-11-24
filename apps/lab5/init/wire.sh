#!/bin/bash

#
# Copyright (C) 2015,  Netronome Systems, Inc.  All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
# @file         apps/wire/init/wire.sh
# @brief        init script
#

NFP_SDK_DIR=${NFP_SDK_DIR:-/opt/netronome}
export PATH=${NFP_SDK_DIR}/bin:$PATH
export LD_LIBRARY_PATH=${NFP_SDK_DIR}/lib:$LD_LIBRARY_PATH

CONFIG_DIR=$(dirname $0)
PREPEND_0=17
OUTPUT=/dev/null

Usage() {
        echo
        echo -e "\t ****** Error: $1 ****** "
        echo "Usage: $0 < start <mefw> | stop | restart <mefw> >"
        echo -e "\tstart   : Load ME fw and init MAC/TM/DMA"
        echo -e "\tstop    : Unload ME fw and stop MAC interfaces"
        echo -e "\trestart : stop & start"
        echo
        exit 1
}

case "$1" in
    start)
        echo "Starting FW:"
        if [ $# -lt 2 ]
        then
            Usage "Insufficient Parameters"
        fi

        # Reset Islands to restore NBI events and packet engine work queues
        echo -n " - Reset Islands..."
        nfp-power nbi=reset nbi=on
        nfp-power fpc=reset fpc=on
        nfp-power imu=reset imu=on
        nfp-power ila=reset ila=on
        nfp-power crp=reset crp=on
        nfp-power pci_0.meg0=reset pci_0.meg0=on
        nfp-power pci_0.meg1=reset pci_0.meg1=on
        nfp-power pci_1.meg0=reset pci_1.meg0=on
        nfp-power pci_1.meg1=reset pci_1.meg1=on
        nfp-power pci_2.meg0=reset pci_2.meg0=on
        nfp-power pci_2.meg1=reset pci_2.meg1=on
        nfp-power pci_3.meg0=reset pci_3.meg0=on
        nfp-power pci_3.meg1=reset pci_3.meg1=on
        echo "done"

        # Load firmware, but no start
        echo -n " - Loading FW (no-start)..."
        nfp-nffw $2 load --no-start || exit 1
        echo "done"

        # NBI DMA init
        echo -n " - Init DMA..."
        nfp init dma 0 ${CONFIG_DIR}/nbi_0_dma.json > $OUTPUT || exit 1
        echo "done"

        # NBI TM init
        echo -n " - Init TM..."
        nfp init tm 0 ${CONFIG_DIR}/nbi_tm.json &> $OUTPUT || exit 1
        echo "done"

        # Increase TM size to 4096 (3)
        nfp-reg nbi:i8.NBITMCPP.TMBufferList.BLQueCtrl0=0x3000000000

        # NBI PC init
        nfp-xpb 0x0049290000 0x32ff0000 || exit 1
        nfp-xpb 0x0048290000 0x32ff0000 || exit 1

        # Set NBI PC prepend bits to 0
        for ((i=0;i<128;i++));do
            nfp-reg xpb:Nbi0IsldXpbMap.NbiTopXpbMap.PktPreclassifier.Characterization.PortCfg$i=$PREPEND_0
        done
	nfp-reg xpbm:IntMu0IsldXpbmMap.Island.IntMuXpbMap.MuStatsReg.MUSEBaseAddr0=0x56000

        # Start ME's
        echo -n " - Starting FW..."
        nfp-nffw start || exit 1
        echo "done"

        # NBI MAC init
        echo -n " - Init MAC for SF..."
        nfp-macinit \
            -0 ${CONFIG_DIR}/sf1-2x40GE.json \
            -p ${NFP_SDK_DIR}/share/nbi/nfp_nbi_phy_tuning_SF.json -m0 \
            &> $OUTPUT || exit 1
        nfp -m mac -e set port rx 0 0 enable &> $OUTPUT || exit 1
        nfp -m mac -e set port rx 0 4 enable &> $OUTPUT || exit 1
        echo "done"

        echo ""
        ;;
    reload|restart)
        if [ $# -lt 2 ]
        then
            Usage "Insufficient Parameters"
        fi
        $0 stop
        sleep 2
        $0 start $2
        ;;
    stop)

        echo "Stopping FW:"
        #ifdown interfaces
        echo -n " - Bringing down interfaces..."
        nfp -m mac -e set port ifdown 0  0 > $OUTPUT
        nfp -m mac -e set port ifdown 0  4 > $OUTPUT
        echo "done"

        echo -n " - Remove net_dev and load nfp driver..."
        (rmmod nfp_netvf || true) 2>/dev/null
        (rmmod nfp_net || true) 2>/dev/null
        (rmmod nfp || true) 2>/dev/null
        sleep 1
        if [ -f ./nfp.ko ]
        then
            insmod ./nfp.ko nfp_reset=1
        else
            modprobe nfp nfp_reset=1
        fi
        echo "done"

        echo -n " - Unload FW..."
        nfp-nffw unload || exit 1
        echo "done"
        echo ""
        ;;
        *)
            Usage "Invalid option"
esac
exit 0
