//
// Copyright (C) 2024 Intel Corporation.
// SPDX-License-Identifier: Apache 2.0
//

#pragma once

#include <openvino/runtime/tensor.hpp>

namespace vpux {

/**
 * @brief Copies the contents of one tensor into another one which bears the same shape and precision.
 *
 * @param in The source tensor
 * @param out The destination tensor
 */
void copyTensor(const ov::Tensor& in, const ov::Tensor& out);

/**
 * @brief Copies the contents of one tensor into another one which bears the same shape. Precision conversions from
 * source type to target type will be performed if required.
 *
 * @param in The source tensor
 * @param out The destination tensor
 */
void convertTensorPrecision(const ov::Tensor& in, const ov::Tensor& out);

/**
 * @brief Constructs a tensor with the same content as the source but with the precision converted to the specified
 * target.
 *
 * @param in The source tensor
 * @param precision The target precision
 * @param ptr Optional, the constructed tensor will use this address for its buffer if specified
 * @return The tensor obtained upon converting the precision.
 */
ov::Tensor toPrecision(const ov::Tensor& in, const ov::element::Type& precision, void* ptr = nullptr);

/**
 * @brief Constructs a tensor with the same content as the source but with the precision converted to FP32.
 *
 * @param in The source tensor
 * @param ptr Optional, the constructed tensor will use this address for its buffer if specified
 * @return The tensor obtained upon converting the precision.
 */
inline ov::Tensor toFP32(const ov::Tensor& in, void* ptr = nullptr) {
    return toPrecision(in, ov::element::Type_t::f32, ptr);
}

/**
 * @brief Converts the precision used by a batch of tensors to FP32 and returns their buffers. The original tensors
 * remain unchanged.
 *
 * @param tensors The source tensors
 * @return The buffers of the tensors obtained upon precision conversion
 */
std::vector<std::vector<float>> parseTensorsAsFP32(const std::map<std::string, ov::Tensor>& tensors);

}  // namespace vpux