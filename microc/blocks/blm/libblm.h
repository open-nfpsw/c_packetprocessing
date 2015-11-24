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
 * @file        libblm.h
 * @brief
 */
#ifndef __NFP_BLM_IFACE_H__
#define __NFP_BLM_IFACE_H__

#if defined(__NFP_LANG_ASM)

#include "blm_iface_asm.h"

#elif defined(__NFP_LANG_MICROC)

#include "blm_iface_c.h"

#else

#error "__NFP_LANG_ASM or __NFP_LANG_MICROC must befined"

#endif

#endif // __NFP_BLM_IFACE_H__
