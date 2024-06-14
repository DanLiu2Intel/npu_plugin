//
// Copyright (C) 2023 Intel Corporation.
// SPDX-License-Identifier: Apache 2.0
//

#pragma once

#include "vpux/compiler/core/pipelines_options.hpp"

namespace vpux {
namespace arch40xx {

//
// DefaultHWOptionsDeviceBase (for all dialects in 40xx)
// This class must be inherited by all dialect-base options
// to avoid confusion when we have the same option for IE and the VPU dialect, but with a different value
//

struct DefaultHWOptionsDeviceBase : public virtual vpux::DefaultHWOptionsBase {
    StrOption enableActivationSparsity{*this, "enable-activation-sparsity",
                                       llvm::cl::desc("Enable activation sparsity"), llvm::cl::init("auto")};

    BoolOption enableWeightsSparsity{*this, "enable-weights-sparsity", llvm::cl::desc("Enable weights sparsity"),
                                     llvm::cl::init(true)};

    BoolOption enableSEPtrsOperations{*this, "enable-se-ptrs-operations",
                                      llvm::cl::desc("Enable storage element pointer operations"),
                                      llvm::cl::init(true)};

    BoolOption enableExperimentalSEPtrsOperations{*this, "enable-experimental-se-ptrs-operations",
                                                  llvm::cl::desc("Enable the experimental operation of SEP"),
                                                  llvm::cl::init(false)};

    BoolOption enableStartBarrier{*this, "enable-start-barrier", llvm::cl::desc("Enable start barrier"),
                                  llvm::cl::init(true)};

    BoolOption enableFinalBarrier{*this, "enable-final-barrier", llvm::cl::desc("Enable final barrier"),
                                  llvm::cl::init(true)};

    BoolOption enableM2I{*this, "enable-m2i", llvm::cl::desc("Enable M2I passes"), llvm::cl::init(false)};

    BoolOption enableExplicitDistributedTensorAttr{
            *this, "enable-explicit-distributed-attr",
            llvm::cl::desc("Enable DistributedTensorAttr with explicit per cluster memory/compute shapes & offsets"),
            llvm::cl::init(true)};

    StrOption dpuDryRun{*this, "dpu-dry-run",
                        llvm::cl::desc("Patch DPU tasks to disable their functionality (none|stub|strip)"),
                        llvm::cl::init("none")};

    BoolOption shaveDryRun{*this, "shave-dry-run", llvm::cl::desc("Enable shave dry run stripping"),
                           llvm::cl::init(false)};
};

//
// MCAndTilingOptionsDevice options
//

struct MCAndTilingOptionsDevice : public vpux::MCAndTilingOptionsBase {
    MCAndTilingOptionsDevice() {
        enableExplicitDistributedTensorAttr = true;
    }
};

}  // namespace arch40xx
}  // namespace vpux