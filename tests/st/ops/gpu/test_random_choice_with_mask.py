# Copyright 2020 Huawei Technologies Co., Ltd
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ============================================================================

import numpy as np
import pytest

import mindspore.context as context
import mindspore.nn as nn
from mindspore import Tensor
from mindspore.ops import operations as P

class RCWM_count_in(nn.Cell):
    def __init__(self):
        super(RCWM_count_in, self).__init__()
        self.RCWM_count_in = P.RandomChoiceWithMask(count=4, seed=1)

    def construct(self, x):
        return self.RCWM_count_in(x)

class RCWM_count_out(nn.Cell):
    def __init__(self):
        super(RCWM_count_out, self).__init__()
        self.RCWM_count_out = P.RandomChoiceWithMask(count=10, seed=1)

    def construct(self, x):
        return self.RCWM_count_out(x)

class RCWM_3D(nn.Cell):
    def __init__(self):
        super(RCWM_3D, self).__init__()
        self.RCWM_3D = P.RandomChoiceWithMask(count=10, seed=1)

    def construct(self, x):
        return self.RCWM_3D(x)

@pytest.mark.level0
@pytest.mark.platform_x86_gpu_training
@pytest.mark.env_onecard
def test_RCWM_3D():
    context.set_context(mode=context.PYNATIVE_MODE, device_target="GPU")
    input_tensor = Tensor(np.ones([3, 4, 5]).astype(np.bool))
    expect1 = [[0, 1, 1], [0, 2, 1], [0, 2, 2], [1, 0, 1], [0, 1, 3], [0, 3, 0], [1, 3, 2], \
    [0, 0, 0], [1, 1, 2], [1, 3, 4]]
    expect2 = [True, True, True, True, True, True, True, True, True, True]
    rcwm = RCWM_3D()
    output1, output2 = rcwm(input_tensor)
    assert np.all(output1.asnumpy() == np.array(expect1)), "output: {}, expect: {}".format(output1, expect1)
    assert np.all(output2.asnumpy() == np.array(expect2)), "output: {}, expect: {}".format(output2, expect2)

@pytest.mark.level0
@pytest.mark.platform_x86_gpu_training
@pytest.mark.env_onecard
def test_RCWM_count_out():
    context.set_context(mode=context.PYNATIVE_MODE, device_target="GPU")
    input_tensor = Tensor(np.array([[1, 0, 1, 0], [0, 0, 0, 1], [1, 1, 1, 1], [0, 0, 0, 1]]).astype(np.bool))
    expect1 = [[0, 2], [2, 2], [2, 1], [2, 0], [0, 0], [3, 3], [2, 3], [1, 3], [0, 0], [0, 0]]
    expect2 = [True, True, True, True, True, True, True, True, False, False]
    rcwm = RCWM_count_out()
    output1, output2 = rcwm(input_tensor)
    assert np.all(output1.asnumpy() == np.array(expect1)), "output: {}, expect: {}".format(output1, expect1)
    assert np.all(output2.asnumpy() == np.array(expect2)), "output: {}, expect: {}".format(output2, expect2)

@pytest.mark.level0
@pytest.mark.platform_x86_gpu_training
@pytest.mark.env_onecard
def test_RCWM_count_in():
    context.set_context(mode=context.PYNATIVE_MODE, device_target="GPU")
    input_tensor = Tensor(np.array([[1, 0, 1, 0], [0, 0, 0, 1], [1, 1, 1, 1], [0, 0, 0, 1]]).astype(np.bool))
    expect1 = [[0, 2], [2, 2], [2, 1], [2, 0]]
    expect2 = [True, True, True, True]
    rcwm = RCWM_count_in()
    output1, output2 = rcwm(input_tensor)
    assert np.all(output1.asnumpy() == np.array(expect1)), "output: {}, expect: {}".format(output1, expect1)
    assert np.all(output2.asnumpy() == np.array(expect2)), "output: {}, expect: {}".format(output2, expect2)
