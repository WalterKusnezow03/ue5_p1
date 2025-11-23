// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "../../../Source/ComputerVisionPlugin/Public/ComputerVision/Actor/ComputerVisionActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeComputerVisionActor() {}
// Cross Module References
	COMPUTERVISIONPLUGIN_API UClass* Z_Construct_UClass_AComputerVisionActor();
	COMPUTERVISIONPLUGIN_API UClass* Z_Construct_UClass_AComputerVisionActor_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	ENGINE_API UClass* Z_Construct_UClass_USceneCaptureComponent2D_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UTextureRenderTarget2D_NoRegister();
	UPackage* Z_Construct_UPackage__Script_ComputerVisionPlugin();
// End Cross Module References
	void AComputerVisionActor::StaticRegisterNativesAComputerVisionActor()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AComputerVisionActor);
	UClass* Z_Construct_UClass_AComputerVisionActor_NoRegister()
	{
		return AComputerVisionActor::StaticClass();
	}
	struct Z_Construct_UClass_AComputerVisionActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ResolutionX_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_ResolutionX;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_ResolutionY_MetaData[];
#endif
		static const UECodeGen_Private::FIntPropertyParams NewProp_ResolutionY;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_FOV_MetaData[];
#endif
		static const UECodeGen_Private::FFloatPropertyParams NewProp_FOV;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SceneCapture_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SceneCapture;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_RenderTarget_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_RenderTarget;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AComputerVisionActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_ComputerVisionPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AComputerVisionActor_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "ComputerVision/Actor/ComputerVisionActor.h" },
		{ "ModuleRelativePath", "Public/ComputerVision/Actor/ComputerVisionActor.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_ResolutionX_MetaData[] = {
		{ "Category", "Bot Vision" },
		{ "Comment", "// Parameter\n" },
		{ "ModuleRelativePath", "Public/ComputerVision/Actor/ComputerVisionActor.h" },
		{ "ToolTip", "Parameter" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_ResolutionX = { "ResolutionX", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AComputerVisionActor, ResolutionX), METADATA_PARAMS(Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_ResolutionX_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_ResolutionX_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_ResolutionY_MetaData[] = {
		{ "Category", "Bot Vision" },
		{ "ModuleRelativePath", "Public/ComputerVision/Actor/ComputerVisionActor.h" },
	};
#endif
	const UECodeGen_Private::FIntPropertyParams Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_ResolutionY = { "ResolutionY", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AComputerVisionActor, ResolutionY), METADATA_PARAMS(Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_ResolutionY_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_ResolutionY_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_FOV_MetaData[] = {
		{ "Category", "Bot Vision" },
		{ "ModuleRelativePath", "Public/ComputerVision/Actor/ComputerVisionActor.h" },
	};
#endif
	const UECodeGen_Private::FFloatPropertyParams Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_FOV = { "FOV", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AComputerVisionActor, FOV), METADATA_PARAMS(Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_FOV_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_FOV_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_SceneCapture_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "Public/ComputerVision/Actor/ComputerVisionActor.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_SceneCapture = { "SceneCapture", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AComputerVisionActor, SceneCapture), Z_Construct_UClass_USceneCaptureComponent2D_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_SceneCapture_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_SceneCapture_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_RenderTarget_MetaData[] = {
		{ "ModuleRelativePath", "Public/ComputerVision/Actor/ComputerVisionActor.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_RenderTarget = { "RenderTarget", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AComputerVisionActor, RenderTarget), Z_Construct_UClass_UTextureRenderTarget2D_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_RenderTarget_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_RenderTarget_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AComputerVisionActor_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_ResolutionX,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_ResolutionY,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_FOV,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_SceneCapture,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AComputerVisionActor_Statics::NewProp_RenderTarget,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AComputerVisionActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AComputerVisionActor>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AComputerVisionActor_Statics::ClassParams = {
		&AComputerVisionActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_AComputerVisionActor_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_AComputerVisionActor_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AComputerVisionActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AComputerVisionActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AComputerVisionActor()
	{
		if (!Z_Registration_Info_UClass_AComputerVisionActor.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AComputerVisionActor.OuterSingleton, Z_Construct_UClass_AComputerVisionActor_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AComputerVisionActor.OuterSingleton;
	}
	template<> COMPUTERVISIONPLUGIN_API UClass* StaticClass<AComputerVisionActor>()
	{
		return AComputerVisionActor::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AComputerVisionActor);
	AComputerVisionActor::~AComputerVisionActor() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_ComputerVisionPlugin_Source_ComputerVisionPlugin_Public_ComputerVision_Actor_ComputerVisionActor_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_ComputerVisionPlugin_Source_ComputerVisionPlugin_Public_ComputerVision_Actor_ComputerVisionActor_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AComputerVisionActor, AComputerVisionActor::StaticClass, TEXT("AComputerVisionActor"), &Z_Registration_Info_UClass_AComputerVisionActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AComputerVisionActor), 973311914U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_ComputerVisionPlugin_Source_ComputerVisionPlugin_Public_ComputerVision_Actor_ComputerVisionActor_h_2075379541(TEXT("/Script/ComputerVisionPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_ComputerVisionPlugin_Source_ComputerVisionPlugin_Public_ComputerVision_Actor_ComputerVisionActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_ComputerVisionPlugin_Source_ComputerVisionPlugin_Public_ComputerVision_Actor_ComputerVisionActor_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
