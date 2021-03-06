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
#ifndef MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_NNACL_RESIZE_H_
#define MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_NNACL_RESIZE_H_

#ifdef ENABLE_NEON
#include <arm_neon.h>
#endif
#include <memory.h>
#include "src/runtime/kernel/arm/nnacl/op_base.h"
#include "schema/ops_generated.h"
#include "src/runtime/kernel/arm/nnacl/errorcode.h"

using mindspore::schema::ResizeMethod;

struct ResizeParameter {
  OpParameter op_parameter_;
  ResizeMethod method_;
  int64_t new_height_;
  int64_t new_width_;
  bool align_corners_;
  bool preserve_aspect_ratio_;
};

int ResizeBilinear(const float *input_data, float *output_data, const int *input_shape, const int *output_shape,
                   bool align_corners, int tid, int thread_num);

int ResizeNearestNeighbor(const float *input_data, float *output_data, const int *input_shape, const int *output_shape,
                          int tid, int thread_num);

#endif  // MINDSPORE_LITE_SRC_RUNTIME_KERNEL_ARM_NNACL_RESIZE_H_

