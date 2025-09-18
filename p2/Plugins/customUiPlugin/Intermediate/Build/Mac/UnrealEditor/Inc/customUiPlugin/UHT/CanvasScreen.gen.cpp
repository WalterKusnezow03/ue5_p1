// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/screens/CanvasScreen.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCanvasScreen() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UCanvasScreen();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UCanvasScreen_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UUiComponentParent();
	UMG_API UClass* Z_Construct_UClass_UBackgroundBlur_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UBorder_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UCanvasPanel_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UOverlay_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UCanvasScreen::StaticRegisterNativesUCanvasScreen()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCanvasScreen);
	UClass* Z_Construct_UClass_UCanvasScreen_NoRegister()
	{
		return UCanvasScreen::StaticClass();
	}
	struct Z_Construct_UClass_UCanvasScreen_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_baseCanvas_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_baseCanvas;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_baseOverlay_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_baseOverlay;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_backgroundBlur_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_backgroundBlur;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_backgroundColor_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_backgroundColor;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCanvasScreen_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUiComponentParent,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCanvasScreen_Statics::Class_MetaDataParams[] = {
		{ "Comment", "///@brief Base Canvas screen class, use AddChild() method to add any child to the canvas.\n" },
		{ "IncludePath", "ui/screens/CanvasScreen.h" },
		{ "ModuleRelativePath", "ui/screens/CanvasScreen.h" },
		{ "ToolTip", "@brief Base Canvas screen class, use AddChild() method to add any child to the canvas." },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCanvasScreen_Statics::NewProp_baseCanvas_MetaData[] = {
		{ "Comment", "//not protected for a reason!\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/screens/CanvasScreen.h" },
		{ "ToolTip", "not protected for a reason!" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCanvasScreen_Statics::NewProp_baseCanvas = { "baseCanvas", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UCanvasScreen, baseCanvas), Z_Construct_UClass_UCanvasPanel_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UCanvasScreen_Statics::NewProp_baseCanvas_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCanvasScreen_Statics::NewProp_baseCanvas_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCanvasScreen_Statics::NewProp_baseOverlay_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/screens/CanvasScreen.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCanvasScreen_Statics::NewProp_baseOverlay = { "baseOverlay", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UCanvasScreen, baseOverlay), Z_Construct_UClass_UOverlay_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UCanvasScreen_Statics::NewProp_baseOverlay_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCanvasScreen_Statics::NewProp_baseOverlay_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCanvasScreen_Statics::NewProp_backgroundBlur_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/screens/CanvasScreen.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCanvasScreen_Statics::NewProp_backgroundBlur = { "backgroundBlur", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UCanvasScreen, backgroundBlur), Z_Construct_UClass_UBackgroundBlur_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UCanvasScreen_Statics::NewProp_backgroundBlur_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCanvasScreen_Statics::NewProp_backgroundBlur_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCanvasScreen_Statics::NewProp_backgroundColor_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/screens/CanvasScreen.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCanvasScreen_Statics::NewProp_backgroundColor = { "backgroundColor", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UCanvasScreen, backgroundColor), Z_Construct_UClass_UBorder_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UCanvasScreen_Statics::NewProp_backgroundColor_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCanvasScreen_Statics::NewProp_backgroundColor_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCanvasScreen_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCanvasScreen_Statics::NewProp_baseCanvas,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCanvasScreen_Statics::NewProp_baseOverlay,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCanvasScreen_Statics::NewProp_backgroundBlur,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCanvasScreen_Statics::NewProp_backgroundColor,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCanvasScreen_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCanvasScreen>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UCanvasScreen_Statics::ClassParams = {
		&UCanvasScreen::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UCanvasScreen_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UCanvasScreen_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UCanvasScreen_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UCanvasScreen_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCanvasScreen()
	{
		if (!Z_Registration_Info_UClass_UCanvasScreen.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCanvasScreen.OuterSingleton, Z_Construct_UClass_UCanvasScreen_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UCanvasScreen.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UCanvasScreen>()
	{
		return UCanvasScreen::StaticClass();
	}
	UCanvasScreen::UCanvasScreen(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCanvasScreen);
	UCanvasScreen::~UCanvasScreen() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_CanvasScreen_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_CanvasScreen_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UCanvasScreen, UCanvasScreen::StaticClass, TEXT("UCanvasScreen"), &Z_Registration_Info_UClass_UCanvasScreen, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCanvasScreen), 2226316377U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_CanvasScreen_h_3491892579(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_CanvasScreen_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_CanvasScreen_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
