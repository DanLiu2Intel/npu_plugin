//
// Copyright (C) 2024 Intel Corporation.
// SPDX-License-Identifier: Apache 2.0
//

#include "vpux/compiler/dialect/IE/transforms/factories/weights_dequantize_to_fakequantize_strategy_getter.hpp"
#include <mlir/Dialect/Func/IR/FuncOps.h>
#include "vpux/compiler/VPU30XX/dialect/IE/impl/weights_dequantize_to_fakequantize_strategy.hpp"
#include "vpux/compiler/dialect/VPU/IR/attributes.hpp"
#include "vpux/compiler/utils/logging.hpp"
#include "vpux/utils/core/logger.hpp"

using namespace vpux;

std::unique_ptr<IGreedilyPassStrategy> IE::createWeightsDequantizeToFakeQuantizeStrategyGetter(
        mlir::func::FuncOp funcOp) {
    const auto arch = VPU::getArch(funcOp);

    switch (arch) {
    case VPU::ArchKind::NPU30XX:
    case VPU::ArchKind::NPU37XX:
    case VPU::ArchKind::NPU40XX: {
        return std::make_unique<arch30xx::WeightsDequantizeToFakeQuantizeStrategy>();
    }
    case VPU::ArchKind::UNKNOWN:
    default: {
        VPUX_THROW("Unable to get WeightsDequantizeToFakeQuantizeStrategy for arch {0}", arch);
    }
    }
}