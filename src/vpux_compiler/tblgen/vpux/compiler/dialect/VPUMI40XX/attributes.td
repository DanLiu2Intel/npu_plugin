//
// Copyright (C) 2022 Intel Corporation.
// SPDX-License-Identifier: Apache 2.0
//

#ifndef VPUX_COMPILER_DIALECT_VPUMI40XX_ATTRIBUTES
#define VPUX_COMPILER_DIALECT_VPUMI40XX_ATTRIBUTES

include "vpux/compiler/core/attributes.td"
include "vpux/compiler/dialect/VPUMI40XX/dialect.td"
include "vpux/compiler/dialect/VPUMI40XX/attr_interfaces.td"

//
// Base classes
//

class VPUMI40XX_I64EnumAttr <string name, string summary, list<I64EnumAttrCase> cases> :
        I64EnumAttr<name, summary, cases> {
    let cppNamespace = "vpux::VPUMI40XX";
    let genSpecializedAttr = 0;
}

class VPUMI40XX_EnumAttr <EnumAttrInfo enumInfo, string name = "", list <Trait> traits = []> :
        EnumAttr<VPUMI40XX_Dialect, enumInfo, name, traits> {
    let assemblyFormat = "`<`$value`>`";
}

class VPUMI40XX_Attr<string name, list<Trait> traits = []> :
        AttrDef<VPUMI40XX_Dialect, name, traits> {
    let mnemonic = name;
    let assemblyFormat = "`<` struct(params) `>`";
}

//
// PPEType
//

def VPUMI40XX_PPELayerType :
    VPUMI40XX_I64EnumAttr<
        "PPELayerType",
        "Post Processing Element Type",
        [
            // Low-level instructions
            I64EnumAttrCase<"STORE", 0>,
            I64EnumAttrCase<"LOAD",  1>,
            I64EnumAttrCase<"CLEAR", 2>,
            I64EnumAttrCase<"NOOP",  3>,
            I64EnumAttrCase<"HALT",  4>,

            // Element-Wise Operations
            I64EnumAttrCase<"ADD",     5>,
            I64EnumAttrCase<"SUB",     6>,
            I64EnumAttrCase<"MULT",    7>,
            I64EnumAttrCase<"MAXIMUM", 8>,
            I64EnumAttrCase<"MINIMUM", 9>,
            I64EnumAttrCase<"AND",     10>,
            I64EnumAttrCase<"OR",      11>,
            I64EnumAttrCase<"XOR",     12>,

            // Activations
            I64EnumAttrCase<"LRELU",   13>,
            I64EnumAttrCase<"LRELUX",  14>,
            I64EnumAttrCase<"LPRELU",  15>,
            I64EnumAttrCase<"CEIL",    16>,
            I64EnumAttrCase<"FLOOR",   17>,
            I64EnumAttrCase<"POW",     18>,
            I64EnumAttrCase<"EXP",     19>,
            I64EnumAttrCase<"SIGMOID", 20>,
            I64EnumAttrCase<"TANH",    21>,
            I64EnumAttrCase<"SQRT",    22>,
            I64EnumAttrCase<"RSQRT",   23>,
            I64EnumAttrCase<"FLEXARB", 24>,
            I64EnumAttrCase<"NOT",     25>,
            I64EnumAttrCase<"ABS",     26>,
            I64EnumAttrCase<"NEG",     27>
        ]
    > {
}

def VPUMI40XX_PPELayerTypeAttr : VPUMI40XX_EnumAttr<VPUMI40XX_PPELayerType, "ppe_layer_type">;

//
// MPEMode
//

def VPUMI40XX_MPEMode :
    VPUMI40XX_I64EnumAttr<
        "MPEMode",
        "MPE Mode",
        [
            I64EnumAttrCase<"VECTOR",       0>,
            I64EnumAttrCase<"MATRIX",       1>,
            I64EnumAttrCase<"VECTOR_FP16",  2>,
            I64EnumAttrCase<"CUBOID_16x16", 3>,
            I64EnumAttrCase<"CUBOID_8x16",  4>,
            I64EnumAttrCase<"NOP",          5>
        ]
    > {
}

def VPUMI40XX_MPEModeAttr : VPUMI40XX_EnumAttr<VPUMI40XX_MPEMode, "mpe_mode">;

//
// DMATransactionAttr
//

class VPUMI40XX_DMATransactionAttrBase<Pred condition, string summary> : Attr<condition, summary> {
    let storageType = [{ vpux::VPUMI40XX::DMATransactionAttr }];
    let returnType = [{ vpux::VPUMI40XX::DMATransactionAttr }];
    let convertFromStorage = "$_self";
}

def VPUMI40XX_DMATransactionAttr : VPUMI40XX_DMATransactionAttrBase<CPred<"$_self.isa<vpux::VPUMI40XX::DMATransactionAttr>()">,
                                      "DMA transaction attribute"> {
    string cppType = "vpux::VPU::DMATransactionAttr";
}

//
// NNDMATransactionAttr
//

def VPUMI40XX_NNDMATransactionAttr : VPUMI40XX_Attr<"NNDMATransaction",
    [DeclareAttrInterfaceMethods<VPUMI40XX_DMATransactionAttrInterface>]> {

    let parameters = (ins
        "mlir::Type":$inputType,
        "mlir::Type":$outputType
    );
}

#endif
