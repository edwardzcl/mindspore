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

#include "src/runtime/kernel/arm/fp32/embedding_lookup.h"
#include "include/errorcode.h"
#include "src/kernel_registry.h"
#include "src/runtime/runtime_api.h"

using mindspore::lite::KernelRegistrar;
using mindspore::lite::RET_ERROR;
using mindspore::lite::RET_OK;
using mindspore::schema::PrimitiveType_EmbeddingLookup;

namespace mindspore::kernel {
int EmbeddingLookupCPUKernel::Init() {
  embedding_lookup_parameter_ = reinterpret_cast<EmbeddingLookupParameter *>(opParameter);
  embedding_lookup_parameter_->thread_num = thread_count_;
  embedding_lookup_parameter_->ids_size_ = inputs_.back()->ElementsNum();

  embedding_lookup_parameter_->layer_size_ = 1;
  auto in_shape = inputs_.front()->shape();
  for (int i = 1; i < in_shape.size(); ++i) {
    embedding_lookup_parameter_->layer_size_ *= in_shape[i];
  }

  embedding_lookup_parameter_->layer_num_ = 0;
  for (int i = 0; i < inputs_.size() - 1; ++i) {
    embedding_lookup_parameter_->layer_num_ += inputs_[i]->shape()[0];
  }

  input_addr_ = reinterpret_cast<float *>(
    std::malloc(sizeof(float) * embedding_lookup_parameter_->layer_size_ * embedding_lookup_parameter_->layer_num_));
  if (input_addr_ == nullptr) {
    MS_LOG(ERROR) << "Create memory failed";
    return mindspore::lite::RET_MEMORY_FAILED;
  }

  embedding_lookup_parameter_->is_regulated_ =
    reinterpret_cast<bool *>(std::malloc(sizeof(bool) * embedding_lookup_parameter_->layer_num_));
  if (embedding_lookup_parameter_->is_regulated_ == nullptr) {
    MS_LOG(ERROR) << "Create memory failed";
    return mindspore::lite::RET_MEMORY_FAILED;
  }

  for (int i = 0; i < embedding_lookup_parameter_->layer_num_; ++i) {
    embedding_lookup_parameter_->is_regulated_[i] = embedding_lookup_parameter_->max_norm_ == 0;
  }

  return RET_OK;
}

int EmbeddingLookupCPUKernel::ReSize() { return RET_OK; }

int EmbeddingLookupCPUKernel::DoExcute(int task_id) {
  int error_code = EmbeddingLookup(input_addr_, ids_addr_, output_addr_, embedding_lookup_parameter_, task_id);
  if (error_code != RET_OK) {
    MS_LOG(ERROR) << "embedding lookup error error_code[" << error_code << "]";
    return RET_ERROR;
  }
  return RET_OK;
}

int EmbeddingLookupRun(int task_id, LiteParallelGroupEnv *penv, void *cdata) {
  auto EmbeddingLookupData = reinterpret_cast<EmbeddingLookupCPUKernel *>(cdata);
  auto ret = EmbeddingLookupData->DoExcute(task_id);
  if (ret != RET_OK) {
    MS_LOG(ERROR) << "EmbeddingLookupRun error task_id[" << task_id << "] error_code[" << ret << "]";
    return RET_ERROR;
  }
  return RET_OK;
}

int EmbeddingLookupCPUKernel::Run() {
  int dest_loc = 0;
  for (int i = 0; i < inputs_.size() - 1; i++) {
    auto input_t = reinterpret_cast<float *>(inputs_.at(i)->Data());
    memcpy(input_addr_ + dest_loc, input_t, sizeof(float) * inputs_.at(i)->ElementsNum());
    dest_loc += inputs_.at(i)->ElementsNum();
  }
  output_addr_ = reinterpret_cast<float *>(outputs_.front()->Data());
  ids_addr_ = reinterpret_cast<int *>(inputs_.back()->Data());

  auto ret = LiteBackendParallelLaunch(EmbeddingLookupRun, this, embedding_lookup_parameter_->thread_num);
  if (ret != RET_OK) {
    MS_LOG(ERROR) << "EmbeddingLookup error: error_code[" << ret << "]";
    return RET_ERROR;
  }
  return RET_OK;
}

kernel::LiteKernel *CpuEmbeddingLookupFp32KernelCreator(const std::vector<lite::tensor::Tensor *> &inputs,
                                                        const std::vector<lite::tensor::Tensor *> &outputs,
                                                        OpParameter *parameter, const lite::Context *ctx,
                                                        const KernelKey &desc) {
  if (parameter == nullptr || ctx == nullptr) {
    MS_LOG(ERROR) << "parameter or ctx is nullptr";
    return nullptr;
  }
  MS_ASSERT(desc.type == PrimitiveType_EmbeddingLookup);
  auto *kernel = new (std::nothrow) EmbeddingLookupCPUKernel(parameter, inputs, outputs, ctx);
  if (kernel == nullptr) {
    MS_LOG(ERROR) << "Create Kernel failed, name: " << parameter->name_;
    return nullptr;
  }

  auto ret = kernel->Init();
  if (ret != RET_OK) {
    MS_LOG(ERROR) << "Init Kernel failed, name: " << parameter->name_
                  << ", type: " << schema::EnumNamePrimitiveType(static_cast<schema::PrimitiveType>(parameter->type_));
    delete kernel;
    return nullptr;
  }
  return kernel;
}

REG_KERNEL(kCPU, kNumberTypeFloat32, PrimitiveType_EmbeddingLookup, CpuEmbeddingLookupFp32KernelCreator)
}  // namespace mindspore::kernel
