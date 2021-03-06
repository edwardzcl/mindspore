/**
 * Copyright 2020 Huawei Technologies Co., Ltd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_NNACL_INT8_ARITHMETIC_SELF_INT8_H_
#define MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_NNACL_INT8_ARITHMETIC_SELF_INT8_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include "src/runtime/kernel/arm/nnacl/op_base.h"
#include "src/runtime/kernel/arm/nnacl/errorcode.h"

int ElementRound(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int ElementFloor(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int ElementCeil(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int ElementAbs(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int ElementSin(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int ElementCos(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int ElementLog(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int ElementSqrt(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int ElementRsqrt(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int ElementSquare(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

int ElementLogicalNot(int8_t *input, int8_t *output, int element_size, ArithSelfQuantArg para);

#endif  // MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_NNACL_INT8_ARITHMETIC_SELF_INT8_H_
