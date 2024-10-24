//
// Copyright (C) 2024 Intel Corporation.
// SPDX-License-Identifier: Apache 2.0
//

#include "vpux/compiler/dialect/IE/IR/ops.hpp"
#include "vpux/compiler/dialect/VPU/transforms/factories/barrier_variant_constraint.hpp"
#include "vpux/compiler/dialect/VPU/transforms/passes.hpp"
#include "vpux/compiler/dialect/VPU/utils/barrier_variant_constraint_utils.hpp"
#include "vpux/compiler/utils/analysis.hpp"
#include "vpux/utils/core/error.hpp"
using namespace vpux;

namespace {

//
// SetupPerBarrierVariantConstraintPass
//

class SetupPerBarrierVariantConstraintPass final :
        public VPU::SetupPerBarrierVariantConstraintBase<SetupPerBarrierVariantConstraintPass> {
public:
    SetupPerBarrierVariantConstraintPass() = default;
    SetupPerBarrierVariantConstraintPass(const VPU::InitCompilerOptions& initCompilerOptions, Logger log)
            : _enablePartialWorkloadManagement(enablePartialWorkloadManagement) {
        Base::initLogger(log, Base::getArgumentName());
        Base::copyOptionValuesFrom(initCompilerOptions);

        initializeFromOptions();
    }

private:
    mlir::LogicalResult initializeOptions(StringRef options) final;
    void safeRunOnModule() final;

private:
    // Initialize fields from pass options
    void initializeFromOptions();

private:
    bool _enablePartialWorkloadManagement = false;
    bool _allowCustomValues = false;
};

void addConstraint(mlir::OpBuilder optionsBuilder, IE::PipelineOptionsOp pipelineOptionsOp,
                   mlir::StringRef constraintName, size_t constraintValue, bool allowCustomValues) {
    auto hasPipelineOption = pipelineOptionsOp.lookupSymbol<IE::OptionOp>(constraintName) != nullptr;
    VPUX_THROW_WHEN(!allowCustomValues && hasPipelineOption,
                    "Barrier constraint is already defined, probably you run '--init-compiler' twice");

    if (hasPipelineOption) {
        return;
    }

    auto* ctx = optionsBuilder.getContext();
    mlir::IntegerType sizeType = mlir::IntegerType::get(ctx, sizeof(void*) * 8, mlir::IntegerType::Unsigned);
    const auto constraintAttr = mlir::StringAttr::get(ctx, constraintName);
    optionsBuilder.create<IE::OptionOp>(optionsBuilder.getUnknownLoc(), constraintAttr,
                                        mlir::IntegerAttr::get(sizeType, constraintValue));
}

mlir::LogicalResult SetupPerBarrierVariantConstraintPass::initializeOptions(StringRef options) {
    if (mlir::failed(Base::initializeOptions(options))) {
        return mlir::failure();
    }

    initializeFromOptions();

    return mlir::success();
}

void SetupPerBarrierVariantConstraintPass::initializeFromOptions() {
    if (enablePartialWorkloadManagement.hasValue()) {
        _log.trace("Overloading the default value {0} of the '_enablePartialWorkloadManagement' field to the value {1} "
                   "of the pass option 'enableWorkloadManagement' generated by MLIR",
                   _enablePartialWorkloadManagement, enablePartialWorkloadManagement);
        _enablePartialWorkloadManagement = enablePartialWorkloadManagement;
    }

    if (wlmRollback.hasValue() && wlmRollback.getValue() == true) {
        // Using non-WLM values might result in slightly worse inference latency but is
        // safer in case compilation with WLM enabled fails
        _log.trace("Disabling partial workload management due to enabled WLM rollback");
        _enablePartialWorkloadManagement = false;
    }

    if (allowCustomValues.hasValue()) {
        _allowCustomValues = allowCustomValues.getValue();
    }
}

void SetupPerBarrierVariantConstraintPass::safeRunOnModule() {
    auto moduleOp = getModuleOp(getOperation());
    auto optionsBuilder = mlir::OpBuilder::atBlockBegin(moduleOp.getBody());
    auto pipelineOptionsOp = VPU::getPipelineOptionsOp(getContext(), moduleOp);
    optionsBuilder =
            mlir::OpBuilder::atBlockBegin(&pipelineOptionsOp.getOptions().front(), optionsBuilder.getListener());

    auto perBarrierVariantConstraint =
            vpux::VPU::getPerBarrierVariantConstraint(VPU::getArch(getOperation()), _enablePartialWorkloadManagement);
    auto barrVariantSum = perBarrierVariantConstraint.getPerBarrierMaxVariantSum();
    auto barrVariantCount = perBarrierVariantConstraint.getPerBarrierMaxVariantCount();

    addConstraint(optionsBuilder, pipelineOptionsOp, VPU::BARR_MAX_VARIANT_SUM, barrVariantSum, _allowCustomValues);
    addConstraint(optionsBuilder, pipelineOptionsOp, VPU::BARR_MAX_VARIANT_COUNT, barrVariantCount, _allowCustomValues);
}

}  // namespace

//
// createSetupPerBarrierVariantConstraintPass
//

std::unique_ptr<mlir::Pass> vpux::VPU::createSetupPerBarrierVariantConstraintPass() {
    return std::make_unique<SetupPerBarrierVariantConstraintPass>();
}

std::unique_ptr<mlir::Pass> vpux::VPU::createSetupPerBarrierVariantConstraintPass(
        const VPU::InitCompilerOptions& initCompilerOptions, Logger log) {
    return std::make_unique<SetupPerBarrierVariantConstraintPass>(initCompilerOptions, log);
}
