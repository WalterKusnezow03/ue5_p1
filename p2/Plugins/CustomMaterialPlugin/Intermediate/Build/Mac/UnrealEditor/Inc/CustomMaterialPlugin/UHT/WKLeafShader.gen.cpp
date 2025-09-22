// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "CustomMaterialPlugin/Materials/Nodes/foliage/WKLeafShader.h"
#include "../../Source/Runtime/Engine/Classes/Materials/MaterialExpression.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWKLeafShader() {}
// Cross Module References
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKLeafShader();
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKLeafShader_NoRegister();
	CUSTOMMATERIALPLUGIN_API UClass* Z_Construct_UClass_UWKVertexShaderBase();
	ENGINE_API UScriptStruct* Z_Construct_UScriptStruct_FExpressionInput();
	UPackage* Z_Construct_UPackage__Script_CustomMaterialPlugin();
// End Cross Module References
	void UWKLeafShader::StaticRegisterNativesUWKLeafShader()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWKLeafShader);
	UClass* Z_Construct_UClass_UWKLeafShader_NoRegister()
	{
		return UWKLeafShader::StaticClass();
	}
	struct Z_Construct_UClass_UWKLeafShader_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Amplitude_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_Amplitude;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_IntervallSlowDown_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_IntervallSlowDown;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_DistanceBetweenWaves_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_DistanceBetweenWaves;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_lowZInput_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_lowZInput;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_highZInput_MetaData[];
#endif
		static const UECodeGen_Private::FStructPropertyParams NewProp_highZInput;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWKLeafShader_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UWKVertexShaderBase,
		(UObject* (*)())Z_Construct_UPackage__Script_CustomMaterialPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKLeafShader_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Object Object Object Object" },
		{ "IncludePath", "Materials/Nodes/foliage/WKLeafShader.h" },
		{ "ModuleRelativePath", "Materials/Nodes/foliage/WKLeafShader.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKLeafShader_Statics::NewProp_Amplitude_MetaData[] = {
		{ "Category", "UWKWaterShader" },
		{ "Comment", "/// ---- Changable attributes of the water ----\n" },
		{ "ModuleRelativePath", "Materials/Nodes/foliage/WKLeafShader.h" },
		{ "ToolTip", "---- Changable attributes of the water ----" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWKLeafShader_Statics::NewProp_Amplitude = { "Amplitude", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWKLeafShader, Amplitude), METADATA_PARAMS(Z_Construct_UClass_UWKLeafShader_Statics::NewProp_Amplitude_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWKLeafShader_Statics::NewProp_Amplitude_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKLeafShader_Statics::NewProp_IntervallSlowDown_MetaData[] = {
		{ "Category", "UWKWaterShader" },
		{ "ModuleRelativePath", "Materials/Nodes/foliage/WKLeafShader.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWKLeafShader_Statics::NewProp_IntervallSlowDown = { "IntervallSlowDown", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWKLeafShader, IntervallSlowDown), METADATA_PARAMS(Z_Construct_UClass_UWKLeafShader_Statics::NewProp_IntervallSlowDown_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWKLeafShader_Statics::NewProp_IntervallSlowDown_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKLeafShader_Statics::NewProp_DistanceBetweenWaves_MetaData[] = {
		{ "Category", "UWKWaterShader" },
		{ "ModuleRelativePath", "Materials/Nodes/foliage/WKLeafShader.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_UWKLeafShader_Statics::NewProp_DistanceBetweenWaves = { "DistanceBetweenWaves", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWKLeafShader, DistanceBetweenWaves), METADATA_PARAMS(Z_Construct_UClass_UWKLeafShader_Statics::NewProp_DistanceBetweenWaves_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWKLeafShader_Statics::NewProp_DistanceBetweenWaves_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKLeafShader_Statics::NewProp_lowZInput_MetaData[] = {
		{ "Comment", "//max distortion\n" },
		{ "ModuleRelativePath", "Materials/Nodes/foliage/WKLeafShader.h" },
		{ "ToolTip", "max distortion" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UWKLeafShader_Statics::NewProp_lowZInput = { "lowZInput", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWKLeafShader, lowZInput), Z_Construct_UScriptStruct_FExpressionInput, METADATA_PARAMS(Z_Construct_UClass_UWKLeafShader_Statics::NewProp_lowZInput_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWKLeafShader_Statics::NewProp_lowZInput_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWKLeafShader_Statics::NewProp_highZInput_MetaData[] = {
		{ "ModuleRelativePath", "Materials/Nodes/foliage/WKLeafShader.h" },
	};
#endif
	const UECodeGen_Private::FStructPropertyParams Z_Construct_UClass_UWKLeafShader_Statics::NewProp_highZInput = { "highZInput", nullptr, (EPropertyFlags)0x0010000000000000, UECodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWKLeafShader, highZInput), Z_Construct_UScriptStruct_FExpressionInput, METADATA_PARAMS(Z_Construct_UClass_UWKLeafShader_Statics::NewProp_highZInput_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWKLeafShader_Statics::NewProp_highZInput_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWKLeafShader_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWKLeafShader_Statics::NewProp_Amplitude,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWKLeafShader_Statics::NewProp_IntervallSlowDown,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWKLeafShader_Statics::NewProp_DistanceBetweenWaves,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWKLeafShader_Statics::NewProp_lowZInput,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWKLeafShader_Statics::NewProp_highZInput,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWKLeafShader_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWKLeafShader>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWKLeafShader_Statics::ClassParams = {
		&UWKLeafShader::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UWKLeafShader_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UWKLeafShader_Statics::PropPointers),
		0,
		0x001020B0u,
		METADATA_PARAMS(Z_Construct_UClass_UWKLeafShader_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWKLeafShader_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWKLeafShader()
	{
		if (!Z_Registration_Info_UClass_UWKLeafShader.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWKLeafShader.OuterSingleton, Z_Construct_UClass_UWKLeafShader_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWKLeafShader.OuterSingleton;
	}
	template<> CUSTOMMATERIALPLUGIN_API UClass* StaticClass<UWKLeafShader>()
	{
		return UWKLeafShader::StaticClass();
	}
	UWKLeafShader::UWKLeafShader(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWKLeafShader);
	UWKLeafShader::~UWKLeafShader() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_foliage_WKLeafShader_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_foliage_WKLeafShader_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWKLeafShader, UWKLeafShader::StaticClass, TEXT("UWKLeafShader"), &Z_Registration_Info_UClass_UWKLeafShader, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWKLeafShader), 1975694630U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_foliage_WKLeafShader_h_199367515(TEXT("/Script/CustomMaterialPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_foliage_WKLeafShader_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_CustomMaterialPlugin_Source_CustomMaterialPlugin_Materials_Nodes_foliage_WKLeafShader_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
