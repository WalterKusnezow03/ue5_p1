// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CustomMaterialPlugin/Materials/Nodes/ShaderBase/vertexShaderBase/WKVertexShaderBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWKVertexShaderBase() {}
// Cross Module References
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UNodeBase();
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKVertexShaderBase();
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKVertexShaderBase_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UMaterialExpressionCeil_NoRegister();
	UPackage* Z_Construct_UPackage__Script_CustomMaterialPlugin();
// End Cross Module References
	void UWKVertexShaderBase::StaticRegisterNativesUWKVertexShaderBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWKVertexShaderBase);
	UClass* Z_Construct_UClass_UWKVertexShaderBase_NoRegister()
	{
		return UWKVertexShaderBase::StaticClass();
	}
	struct Z_Construct_UClass_UWKVertexShaderBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ceilExpression_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_ceilExpression;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWKVertexShaderBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UNodeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_CustomMaterialPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKVertexShaderBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief base class for any expression, has a lot of helper functions\n" },
		{ "HideCategories", "Object Object Object" },
		{ "IncludePath", "Materials/Nodes/ShaderBase/vertexShaderBase/WKVertexShaderBase.h" },
		{ "ModuleRelativePath", "Materials/Nodes/ShaderBase/vertexShaderBase/WKVertexShaderBase.h" },
		{ "ToolTip", "@brief base class for any expression, has a lot of helper functions" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKVertexShaderBase_Statics::NewProp_ceilExpression_MetaData[] = {
		{ "Comment", "//internal Expression Abusing\n" },
		{ "ModuleRelativePath", "Materials/Nodes/ShaderBase/vertexShaderBase/WKVertexShaderBase.h" },
		{ "ToolTip", "internal Expression Abusing" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWKVertexShaderBase_Statics::NewProp_ceilExpression = { "ceilExpression", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWKVertexShaderBase, ceilExpression), Z_Construct_UClass_UMaterialExpressionCeil_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UWKVertexShaderBase_Statics::NewProp_ceilExpression_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWKVertexShaderBase_Statics::NewProp_ceilExpression_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWKVertexShaderBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWKVertexShaderBase_Statics::NewProp_ceilExpression,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWKVertexShaderBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWKVertexShaderBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWKVertexShaderBase_Statics::ClassParams = {
		&UWKVertexShaderBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UWKVertexShaderBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UWKVertexShaderBase_Statics::PropPointers),
		0,
		0x001020B0u,
		METADATA_PARAMS(Z_Construct_UClass_UWKVertexShaderBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWKVertexShaderBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWKVertexShaderBase()
	{
		if (!Z_Registration_Info_UClass_UWKVertexShaderBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWKVertexShaderBase.OuterSingleton, Z_Construct_UClass_UWKVertexShaderBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWKVertexShaderBase.OuterSingleton;
	}
	template<> CUSTOMMATERIALPLUGIN_API UClass* StaticClass<UWKVertexShaderBase>()
	{
		return UWKVertexShaderBase::StaticClass();
	}
	UWKVertexShaderBase::UWKVertexShaderBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWKVertexShaderBase);
	UWKVertexShaderBase::~UWKVertexShaderBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_ShaderBase_vertexShaderBase_WKVertexShaderBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_ShaderBase_vertexShaderBase_WKVertexShaderBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWKVertexShaderBase, UWKVertexShaderBase::StaticClass, TEXT("UWKVertexShaderBase"), &Z_Registration_Info_UClass_UWKVertexShaderBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWKVertexShaderBase), 492649514U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_ShaderBase_vertexShaderBase_WKVertexShaderBase_h_3873374027(TEXT("/Script/CustomMaterialPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_ShaderBase_vertexShaderBase_WKVertexShaderBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_ShaderBase_vertexShaderBase_WKVertexShaderBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
