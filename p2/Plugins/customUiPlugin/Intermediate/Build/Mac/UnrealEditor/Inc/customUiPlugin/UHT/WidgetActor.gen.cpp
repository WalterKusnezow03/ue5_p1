// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/WorldWidgets/Actor/WidgetActor.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWidgetActor() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_AWidgetActor();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_AWidgetActor_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UMG_API UClass* Z_Construct_UClass_UUserWidget_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UWidgetComponent_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void AWidgetActor::StaticRegisterNativesAWidgetActor()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(AWidgetActor);
	UClass* Z_Construct_UClass_AWidgetActor_NoRegister()
	{
		return AWidgetActor::StaticClass();
	}
	struct Z_Construct_UClass_AWidgetActor_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_CreateDefaultWidgetOnBeginPlay_MetaData[];
#endif
		static void NewProp_CreateDefaultWidgetOnBeginPlay_SetBit(void* Obj);
		static const UECodeGen_Private::FBoolPropertyParams NewProp_CreateDefaultWidgetOnBeginPlay;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_internalWidget_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_internalWidget;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_WidgetComponent_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_WidgetComponent;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AWidgetActor_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWidgetActor_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief will handle a UWidgetComponent as base, (3D World Widgets: Analogue to UCanvasRenderTarget and Textures and stuff.)\n" },
		{ "IncludePath", "WorldWidgets/Actor/WidgetActor.h" },
		{ "ModuleRelativePath", "WorldWidgets/Actor/WidgetActor.h" },
		{ "ToolTip", "@brief will handle a UWidgetComponent as base, (3D World Widgets: Analogue to UCanvasRenderTarget and Textures and stuff.)" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWidgetActor_Statics::NewProp_CreateDefaultWidgetOnBeginPlay_MetaData[] = {
		{ "Category", "LaunchProperties" },
		{ "ModuleRelativePath", "WorldWidgets/Actor/WidgetActor.h" },
	};
#endif
	void Z_Construct_UClass_AWidgetActor_Statics::NewProp_CreateDefaultWidgetOnBeginPlay_SetBit(void* Obj)
	{
		((AWidgetActor*)Obj)->CreateDefaultWidgetOnBeginPlay = 1;
	}
	const UECodeGen_Private::FBoolPropertyParams Z_Construct_UClass_AWidgetActor_Statics::NewProp_CreateDefaultWidgetOnBeginPlay = { "CreateDefaultWidgetOnBeginPlay", nullptr, (EPropertyFlags)0x0010000000000001, UECodeGen_Private::EPropertyGenFlags::Bool | UECodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, sizeof(bool), sizeof(AWidgetActor), &Z_Construct_UClass_AWidgetActor_Statics::NewProp_CreateDefaultWidgetOnBeginPlay_SetBit, METADATA_PARAMS(Z_Construct_UClass_AWidgetActor_Statics::NewProp_CreateDefaultWidgetOnBeginPlay_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AWidgetActor_Statics::NewProp_CreateDefaultWidgetOnBeginPlay_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWidgetActor_Statics::NewProp_internalWidget_MetaData[] = {
		{ "Comment", "//must be a uuser widget, default UWidgets do not work!\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "WorldWidgets/Actor/WidgetActor.h" },
		{ "ToolTip", "must be a uuser widget, default UWidgets do not work!" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWidgetActor_Statics::NewProp_internalWidget = { "internalWidget", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AWidgetActor, internalWidget), Z_Construct_UClass_UUserWidget_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AWidgetActor_Statics::NewProp_internalWidget_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AWidgetActor_Statics::NewProp_internalWidget_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AWidgetActor_Statics::NewProp_WidgetComponent_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "WorldWidgets/Actor/WidgetActor.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_AWidgetActor_Statics::NewProp_WidgetComponent = { "WidgetComponent", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(AWidgetActor, WidgetComponent), Z_Construct_UClass_UWidgetComponent_NoRegister, METADATA_PARAMS(Z_Construct_UClass_AWidgetActor_Statics::NewProp_WidgetComponent_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_AWidgetActor_Statics::NewProp_WidgetComponent_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_AWidgetActor_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWidgetActor_Statics::NewProp_CreateDefaultWidgetOnBeginPlay,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWidgetActor_Statics::NewProp_internalWidget,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_AWidgetActor_Statics::NewProp_WidgetComponent,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_AWidgetActor_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AWidgetActor>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_AWidgetActor_Statics::ClassParams = {
		&AWidgetActor::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_AWidgetActor_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_AWidgetActor_Statics::PropPointers),
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_AWidgetActor_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AWidgetActor_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AWidgetActor()
	{
		if (!Z_Registration_Info_UClass_AWidgetActor.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_AWidgetActor.OuterSingleton, Z_Construct_UClass_AWidgetActor_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_AWidgetActor.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<AWidgetActor>()
	{
		return AWidgetActor::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(AWidgetActor);
	AWidgetActor::~AWidgetActor() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_WorldWidgets_Actor_WidgetActor_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_WorldWidgets_Actor_WidgetActor_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_AWidgetActor, AWidgetActor::StaticClass, TEXT("AWidgetActor"), &Z_Registration_Info_UClass_AWidgetActor, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(AWidgetActor), 3512663658U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_WorldWidgets_Actor_WidgetActor_h_332259721(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_WorldWidgets_Actor_WidgetActor_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_WorldWidgets_Actor_WidgetActor_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
