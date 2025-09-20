// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CustomMaterialPlugin/Materials/Nodes/InputExpression/ExpressionInputNamed.h"
#include "../../Source/Runtime/Engine/Classes/Materials/MaterialExpression.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeExpressionInputNamed() {}
// Cross Module References
	CUSTOMMATERIALPLUGIN_API UScriptStruct* Z_Construct_UScriptStruct_FExpressionInputNamed();
	ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FExpressionInput();
	UPackage* Z_Construct_UPackage__Script_CustomMaterialPlugin();
// End Cross Module References
	static FStructRegistrationInfo Z_Registration_Info_UScriptStruct_ExpressionInputNamed;
class UScriptStruct* FExpressionInputNamed::StaticStruct()
{
	if (!Z_Registration_Info_UScriptStruct_ExpressionInputNamed.OuterSingleton)
	{
		Z_Registration_Info_UScriptStruct_ExpressionInputNamed.OuterSingleton = GetStaticStruct(Z_Construct_UScriptStruct_FExpressionInputNamed, (UObject*)Z_Construct_UPackage__Script_CustomMaterialPlugin(), TEXT("ExpressionInputNamed"));
	}
	return Z_Registration_Info_UScriptStruct_ExpressionInputNamed.OuterSingleton;
}
template<> CUSTOMMATERIALPLUGIN_API UScriptStruct* StaticStruct<FExpressionInputNamed>()
{
	return FExpressionInputNamed::StaticStruct();
}
	struct Z_Construct_UScriptStruct_FExpressionInputNamed_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Input_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_Input;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "Comment", "// BlueprintType erlaubt Nutzung in Blueprints\n" },
		{ "ModuleRelativePath", "Materials/Nodes/InputExpression/ExpressionInputNamed.h" },
		{ "ToolTip", "BlueprintType erlaubt Nutzung in Blueprints" },
	};
#endif
	void* Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FExpressionInputNamed>();
	}
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::NewProp_Input_MetaData[] = {
		{ "ModuleRelativePath", "Materials/Nodes/InputExpression/ExpressionInputNamed.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::NewProp_Input = { "Input", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(FExpressionInputNamed, Input), Z_Construct_UScriptStruct_FExpressionInput, METADATA_PARAMS(Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::NewProp_Input_MetaData, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::NewProp_Input_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::NewProp_Input,
	};
	const UECodeGen_Private::FStructParams Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_CustomMaterialPlugin,
		nullptr,
		&NewStructOps,
		"ExpressionInputNamed",
		sizeof(FExpressionInputNamed),
		alignof(FExpressionInputNamed),
		Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::PropPointers,
		UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::Struct_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FExpressionInputNamed()
	{
		if (!Z_Registration_Info_UScriptStruct_ExpressionInputNamed.InnerSingleton)
		{
			UECodeGen_Private::ConstructUScriptStruct(Z_Registration_Info_UScriptStruct_ExpressionInputNamed.InnerSingleton, Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::ReturnStructParams);
		}
		return Z_Registration_Info_UScriptStruct_ExpressionInputNamed.InnerSingleton;
	}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_InputExpression_ExpressionInputNamed_h_Statics
	{
		static const FStructRegisterCompiledInInfo ScriptStructInfo[];
	};
	const FStructRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_InputExpression_ExpressionInputNamed_h_Statics::ScriptStructInfo[] = {
		{ FExpressionInputNamed::StaticStruct, Z_Construct_UScriptStruct_FExpressionInputNamed_Statics::NewStructOps, TEXT("ExpressionInputNamed"), &Z_Registration_Info_UScriptStruct_ExpressionInputNamed, CONSTRUCT_RELOAD_VERSION_INFO(FStructReloadVersionInfo, sizeof(FExpressionInputNamed), 563481622U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_InputExpression_ExpressionInputNamed_h_3246171420(TEXT("/Script/CustomMaterialPlugin"),
		nullptr, 0,
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_InputExpression_ExpressionInputNamed_h_Statics::ScriptStructInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_InputExpression_ExpressionInputNamed_h_Statics::ScriptStructInfo),
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
