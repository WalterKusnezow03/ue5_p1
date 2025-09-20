// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CustomMaterialPlugin/Materials/Nodes/NodeBase.h"
#include "../../Source/Runtime/Engine/Classes/Materials/MaterialExpression.h"
#include "CustomMaterialPlugin/Materials/Nodes/InputExpression/ExpressionInputNamed.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNodeBase() {}
// Cross Module References
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UNodeBase();
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UNodeBase_NoRegister();
	CUSTOMMATERIALPLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FExpressionInputNamed();
	ENGINE_API UClass* Z_Construct_UClass_UMaterialExpression();
	ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FExpressionInput();
	UPackage* Z_Construct_UPackage__Script_CustomMaterialPlugin();
// End Cross Module References
	void UNodeBase::StaticRegisterNativesUNodeBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UNodeBase);
	UClass* Z_Construct_UClass_UNodeBase_NoRegister()
	{
		return UNodeBase::StaticClass();
	}
	struct Z_Construct_UClass_UNodeBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_expressionArray_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_expressionArray_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_expressionArray;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FallbackInput_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_FallbackInput;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UNodeBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UMaterialExpression,
		(UObject* (*)())Z_Construct_UPackage__Script_CustomMaterialPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UNodeBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief deriving from material expression node for materials.\n" },
		{ "HideCategories", "Object Object" },
		{ "IncludePath", "Materials/Nodes/NodeBase.h" },
		{ "ModuleRelativePath", "Materials/Nodes/NodeBase.h" },
		{ "ToolTip", "@brief deriving from material expression node for materials." },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UNodeBase_Statics::NewProp_expressionArray_Inner = { "expressionArray", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, 0, Z_Construct_UScriptStruct_FExpressionInputNamed, METADATA_PARAMS(nullptr, 0) }; // 563481622
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UNodeBase_Statics::NewProp_expressionArray_MetaData[] = {
		{ "ModuleRelativePath", "Materials/Nodes/NodeBase.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UNodeBase_Statics::NewProp_expressionArray = { "expressionArray", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UNodeBase, expressionArray), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_UNodeBase_Statics::NewProp_expressionArray_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UNodeBase_Statics::NewProp_expressionArray_MetaData)) }; // 563481622
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UNodeBase_Statics::NewProp_FallbackInput_MetaData[] = {
		{ "Comment", "//input example\n" },
		{ "ModuleRelativePath", "Materials/Nodes/NodeBase.h" },
		{ "ToolTip", "input example" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UNodeBase_Statics::NewProp_FallbackInput = { "FallbackInput", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UNodeBase, FallbackInput), Z_Construct_UScriptStruct_FExpressionInput, METADATA_PARAMS(Z_Construct_UClass_UNodeBase_Statics::NewProp_FallbackInput_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UNodeBase_Statics::NewProp_FallbackInput_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UNodeBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNodeBase_Statics::NewProp_expressionArray_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNodeBase_Statics::NewProp_expressionArray,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UNodeBase_Statics::NewProp_FallbackInput,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UNodeBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UNodeBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UNodeBase_Statics::ClassParams = {
		&UNodeBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UNodeBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UNodeBase_Statics::PropPointers),
		0,
		0x001020B0u,
		METADATA_PARAMS(Z_Construct_UClass_UNodeBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UNodeBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UNodeBase()
	{
		if (!Z_Registration_Info_UClass_UNodeBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UNodeBase.OuterSingleton, Z_Construct_UClass_UNodeBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UNodeBase.OuterSingleton;
	}
	template<> CUSTOMMATERIALPLUGIN_API UClass* StaticClass<UNodeBase>()
	{
		return UNodeBase::StaticClass();
	}
	UNodeBase::UNodeBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UNodeBase);
	UNodeBase::~UNodeBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_NodeBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_NodeBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UNodeBase, UNodeBase::StaticClass, TEXT("UNodeBase"), &Z_Registration_Info_UClass_UNodeBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UNodeBase), 2261231137U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_NodeBase_h_1785855750(TEXT("/Script/CustomMaterialPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_NodeBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_NodeBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
