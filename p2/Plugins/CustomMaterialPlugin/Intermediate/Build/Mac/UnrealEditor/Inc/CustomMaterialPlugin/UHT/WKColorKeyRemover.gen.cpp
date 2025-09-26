// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CustomMaterialPlugin/Materials/HLSLNodes/ColorRelated/ColorKey/WKColorKeyRemover.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWKColorKeyRemover() {}
// Cross Module References
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKColorKeyRemover();
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKColorKeyRemover_NoRegister();
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKVertexShaderBase();
	ENGINE_API UClass* Z_Construct_UClass_UMaterialExpressionCustom_NoRegister();
	UPackage* Z_Construct_UPackage__Script_CustomMaterialPlugin();
// End Cross Module References
	void UWKColorKeyRemover::StaticRegisterNativesUWKColorKeyRemover()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWKColorKeyRemover);
	UClass* Z_Construct_UClass_UWKColorKeyRemover_NoRegister()
	{
		return UWKColorKeyRemover::StaticClass();
	}
	struct Z_Construct_UClass_UWKColorKeyRemover_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_customExpression_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_customExpression;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWKColorKeyRemover_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UWKVertexShaderBase,
		(UObject* (*)())Z_Construct_UPackage__Script_CustomMaterialPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKColorKeyRemover_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief deriving from material expression node for materials.\n" },
		{ "HideCategories", "Object Object Object Object" },
		{ "IncludePath", "Materials/HLSLNodes/ColorRelated/ColorKey/WKColorKeyRemover.h" },
		{ "ModuleRelativePath", "Materials/HLSLNodes/ColorRelated/ColorKey/WKColorKeyRemover.h" },
		{ "ToolTip", "@brief deriving from material expression node for materials." },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKColorKeyRemover_Statics::NewProp_customExpression_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Materials/HLSLNodes/ColorRelated/ColorKey/WKColorKeyRemover.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWKColorKeyRemover_Statics::NewProp_customExpression = { "customExpression", nullptr, (EPropertyFlags)0x0012000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWKColorKeyRemover, customExpression), Z_Construct_UClass_UMaterialExpressionCustom_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UWKColorKeyRemover_Statics::NewProp_customExpression_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWKColorKeyRemover_Statics::NewProp_customExpression_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWKColorKeyRemover_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWKColorKeyRemover_Statics::NewProp_customExpression,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWKColorKeyRemover_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWKColorKeyRemover>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWKColorKeyRemover_Statics::ClassParams = {
		&UWKColorKeyRemover::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UWKColorKeyRemover_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UWKColorKeyRemover_Statics::PropPointers),
		0,
		0x009020B0u,
		METADATA_PARAMS(Z_Construct_UClass_UWKColorKeyRemover_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWKColorKeyRemover_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWKColorKeyRemover()
	{
		if (!Z_Registration_Info_UClass_UWKColorKeyRemover.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWKColorKeyRemover.OuterSingleton, Z_Construct_UClass_UWKColorKeyRemover_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWKColorKeyRemover.OuterSingleton;
	}
	template<> CUSTOMMATERIALPLUGIN_API UClass* StaticClass<UWKColorKeyRemover>()
	{
		return UWKColorKeyRemover::StaticClass();
	}
	UWKColorKeyRemover::UWKColorKeyRemover(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWKColorKeyRemover);
	UWKColorKeyRemover::~UWKColorKeyRemover() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_HLSLNodes_ColorRelated_ColorKey_WKColorKeyRemover_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_HLSLNodes_ColorRelated_ColorKey_WKColorKeyRemover_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWKColorKeyRemover, UWKColorKeyRemover::StaticClass, TEXT("UWKColorKeyRemover"), &Z_Registration_Info_UClass_UWKColorKeyRemover, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWKColorKeyRemover), 1568770188U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_HLSLNodes_ColorRelated_ColorKey_WKColorKeyRemover_h_1719634701(TEXT("/Script/CustomMaterialPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_HLSLNodes_ColorRelated_ColorKey_WKColorKeyRemover_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_HLSLNodes_ColorRelated_ColorKey_WKColorKeyRemover_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
