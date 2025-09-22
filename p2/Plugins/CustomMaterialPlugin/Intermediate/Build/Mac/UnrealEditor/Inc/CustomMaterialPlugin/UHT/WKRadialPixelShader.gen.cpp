// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CustomMaterialPlugin/Materials/Nodes/PixelShaders/WKRadialPixelShader.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWKRadialPixelShader() {}
// Cross Module References
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKPixelShaderBase();
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKRadialPixelShader();
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKRadialPixelShader_NoRegister();
	UPackage* Z_Construct_UPackage__Script_CustomMaterialPlugin();
// End Cross Module References
	void UWKRadialPixelShader::StaticRegisterNativesUWKRadialPixelShader()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWKRadialPixelShader);
	UClass* Z_Construct_UClass_UWKRadialPixelShader_NoRegister()
	{
		return UWKRadialPixelShader::StaticClass();
	}
	struct Z_Construct_UClass_UWKRadialPixelShader_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_mixScalar_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_mixScalar;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWKRadialPixelShader_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UWKPixelShaderBase,
		(UObject* (*)())Z_Construct_UPackage__Script_CustomMaterialPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKRadialPixelShader_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Object Object Object Object Object" },
		{ "IncludePath", "Materials/Nodes/PixelShaders/WKRadialPixelShader.h" },
		{ "ModuleRelativePath", "Materials/Nodes/PixelShaders/WKRadialPixelShader.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKRadialPixelShader_Statics::NewProp_mixScalar_MetaData[] = {
		{ "Category", "UWKRadialPixelShader" },
		{ "ModuleRelativePath", "Materials/Nodes/PixelShaders/WKRadialPixelShader.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWKRadialPixelShader_Statics::NewProp_mixScalar = { "mixScalar", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWKRadialPixelShader, mixScalar), METADATA_PARAMS(Z_Construct_UClass_UWKRadialPixelShader_Statics::NewProp_mixScalar_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWKRadialPixelShader_Statics::NewProp_mixScalar_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWKRadialPixelShader_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWKRadialPixelShader_Statics::NewProp_mixScalar,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWKRadialPixelShader_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWKRadialPixelShader>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWKRadialPixelShader_Statics::ClassParams = {
		&UWKRadialPixelShader::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UWKRadialPixelShader_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UWKRadialPixelShader_Statics::PropPointers),
		0,
		0x001020B0u,
		METADATA_PARAMS(Z_Construct_UClass_UWKRadialPixelShader_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWKRadialPixelShader_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWKRadialPixelShader()
	{
		if (!Z_Registration_Info_UClass_UWKRadialPixelShader.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWKRadialPixelShader.OuterSingleton, Z_Construct_UClass_UWKRadialPixelShader_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWKRadialPixelShader.OuterSingleton;
	}
	template<> CUSTOMMATERIALPLUGIN_API UClass* StaticClass<UWKRadialPixelShader>()
	{
		return UWKRadialPixelShader::StaticClass();
	}
	UWKRadialPixelShader::UWKRadialPixelShader(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWKRadialPixelShader);
	UWKRadialPixelShader::~UWKRadialPixelShader() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_PixelShaders_WKRadialPixelShader_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_PixelShaders_WKRadialPixelShader_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWKRadialPixelShader, UWKRadialPixelShader::StaticClass, TEXT("UWKRadialPixelShader"), &Z_Registration_Info_UClass_UWKRadialPixelShader, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWKRadialPixelShader), 3505508849U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_PixelShaders_WKRadialPixelShader_h_819236737(TEXT("/Script/CustomMaterialPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_PixelShaders_WKRadialPixelShader_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_PixelShaders_WKRadialPixelShader_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
