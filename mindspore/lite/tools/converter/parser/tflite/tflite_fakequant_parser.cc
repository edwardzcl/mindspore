/**
 * Copyright 2019 Huawei Technologies Co., Ltd
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
#include "tools/converter/parser/tflite/tflite_fakequant_parser.h"
#include <vector>
#include <memory>

namespace mindspore {
namespace lite {
STATUS TfliteFakeQuantParser::Parse(const std::unique_ptr<tflite::OperatorT> &tfliteOp,
                                         const std::vector<std::unique_ptr<tflite::TensorT>> &tfliteTensors,
                                         const std::vector<std::unique_ptr<tflite::BufferT>> &tfliteModelBuffer,
                                         const std::vector<std::unique_ptr<tflite::OperatorCodeT>> &tfliteOpSet,
                                         schema::CNodeT *op, TensorCache *tensor_cache, bool quantizedModel) {
  MS_LOG(DEBUG) << "parse TfliteFullyConnectedParser";
  std::unique_ptr<schema::FullConnectionT> attr(new schema::FullConnectionT());

  auto weight_index = tfliteOp->inputs[1];
  const auto &weight_tensor = tfliteTensors[weight_index];
  if (weight_tensor == nullptr) {
    MS_LOG(ERROR) << "weight_tensor is null";
    return RET_NULL_PTR;
  }
  std::vector<tflite::TensorT *> weight_tensors{weight_tensor.get()};
  if (RET_OK != ParseWeight(weight_tensors, tfliteModelBuffer, tensor_cache, schema::Format_NHWC)) {
    MS_LOG(ERROR) << "parse weight failed";
    return RET_ERROR;
  }

  if (tfliteOp->inputs.size() == 3) {
    attr->hasBias = true;
    auto bias_index = tfliteOp->inputs[2];
    const auto &bias_tensor = tfliteTensors[bias_index];
    if (bias_tensor == nullptr) {
      MS_LOG(ERROR) << "bias_tensor is null";
      return RET_NULL_PTR;
    }
    std::vector<tflite::TensorT *> bias_tensors{bias_tensor.get()};
    if (RET_OK != ParseBias(bias_tensors, tfliteModelBuffer, tensor_cache)) {
      MS_LOG(ERROR) << "parse bias failed";
      return RET_ERROR;
    }
  }
  attr->axis = 1;

  if (op != nullptr) {
    op->primitive = std::make_unique<schema::PrimitiveT>();
    op->primitive->value.type = schema::PrimitiveType_FullConnection;
    op->primitive->value.value = attr.release();
  }
  return RET_OK;
}

TfliteNodeRegister g_tfliteFakeQuantParser("FakeQuant", new TfliteFakeQuantParser());
}  // namespace lite
}  // namespace mindspore
