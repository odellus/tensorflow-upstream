/* Copyright 2015 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_CORE_COMMON_RUNTIME_GPU_GPU_ID_UTILS_H_
#define TENSORFLOW_CORE_COMMON_RUNTIME_GPU_GPU_ID_UTILS_H_

#include "tensorflow/core/common_runtime/gpu/gpu_id.h"
#include "tensorflow/core/common_runtime/gpu/gpu_id_manager.h"
#include "tensorflow/core/common_runtime/gpu/gpu_init.h"
#include "tensorflow/core/lib/gtl/int_type.h"
#include "tensorflow/core/platform/stream_executor.h"

namespace tensorflow {

// Utility methods for translation between Tensorflow GPU ids and physical GPU ids.
class GpuIdUtil {
 public:
  // Convenient methods for getting the associated executor given a TfGpuId or
  // PhysicalGpuId.
  static se::port::StatusOr<se::StreamExecutor*> ExecutorForPhysicalGpuId(
      se::Platform* gpu_manager, PhysicalGpuId physical_gpu_id) {
    return gpu_manager->ExecutorForDevice(physical_gpu_id.value());
  }
  static se::port::StatusOr<se::StreamExecutor*> ExecutorForPhysicalGpuId(
      PhysicalGpuId physical_gpu_id) {
    return ExecutorForPhysicalGpuId(GPUMachineManager(), physical_gpu_id);
  }
  static se::port::StatusOr<se::StreamExecutor*> ExecutorForTfGpuId(
      TfGpuId tf_gpu_id) {
    return ExecutorForPhysicalGpuId(GpuIdManager::TfToPhysicalGpuId(tf_gpu_id));
  }

  // Verify that the physical_gpu_id associated with a TfGpuId is legitimate.
  static void CheckValidTfGpuId(TfGpuId tf_gpu_id) {
    const PhysicalGpuId physical_gpu_id = GpuIdManager::TfToPhysicalGpuId(tf_gpu_id);
    const int visible_device_count = GPUMachineManager()->VisibleDeviceCount();
    CHECK_LT(physical_gpu_id.value(), visible_device_count)
        << "physical_gpu_id is outside discovered device range."
        << " TF GPU id: " << tf_gpu_id << " physical GPU id: " << physical_gpu_id
        << " visible device count: " << visible_device_count;
  }
};

}  // namespace tensorflow

#endif  // TENSORFLOW_CORE_COMMON_RUNTIME_GPU_GPU_ID_UTILS_H_
