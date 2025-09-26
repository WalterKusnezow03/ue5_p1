// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CustomMaterialPlugin/Materials/Nodes/ShaderBase/PixelShaderBase/WKPixelShaderBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWKPixelShaderBase() {}
// Cross Module References
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKPixelShaderBase();
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKPixelShaderBase_NoRegister();
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKVertexShaderBase();
	UPackage* Z_Construct_UPackage__Script_CustomMaterialPlugin();
// End Cross Module References
	void UWKPixelShaderBase::StaticRegisterNativesUWKPixelShaderBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWKPixelShaderBase);
	UClass* Z_Construct_UClass_UWKPixelShaderBase_NoRegister()
	{
		return UWKPixelShaderBase::StaticClass();
	}
	struct Z_Construct_UClass_UWKPixelShaderBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWKPixelShaderBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UWKVertexShaderBase,
		(UObject* (*)())Z_Construct_UPackage__Script_CustomMaterialPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKPixelShaderBase_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Object Object Object Object" },
		{ "IncludePath", "Materials/Nodes/ShaderBase/PixelShaderBase/WKPixelShaderBase.h" },
		{ "ModuleRelativePath", "Materials/Nodes/ShaderBase/PixelShaderBase/WKPixelShaderBase.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWKPixelShaderBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWKPixelShaderBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWKPixelShaderBase_Statics::ClassParams = {
		&UWKPixelShaderBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x001020B0u,
		METADATA_PARAMS(Z_Construct_UClass_UWKPixelShaderBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWKPixelShaderBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWKPixelShaderBase()
	{
		if (!Z_Registration_Info_UClass_UWKPixelShaderBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWKPixelShaderBase.OuterSingleton, Z_Construct_UClass_UWKPixelShaderBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWKPixelShaderBase.OuterSingleton;
	}
	template<> CUSTOMMATERIALPLUGIN_API UClass* StaticClass<UWKPixelShaderBase>()
	{
		return UWKPixelShaderBase::StaticClass();
	}
	UWKPixelShaderBase::UWKPixelShaderBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWKPixelShaderBase);
	UWKPixelShaderBase::~UWKPixelShaderBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_ShaderBase_PixelShaderBase_WKPixelShaderBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_ShaderBase_PixelShaderBase_WKPixelShaderBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWKPixelShaderBase, UWKPixelShaderBase::StaticClass, TEXT("UWKPixelShaderBase"), &Z_Registration_Info_UClass_UWKPixelShaderBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWKPixelShaderBase), 2505185919U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_ShaderBase_PixelShaderBase_WKPixelShaderBase_h_3735228640(TEXT("/Script/CustomMaterialPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_ShaderBase_PixelShaderBase_WKPixelShaderBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_ShaderBase_PixelShaderBase_WKPixelShaderBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
