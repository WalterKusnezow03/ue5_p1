// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/ImageOverlayedButtonBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeImageOverlayedButtonBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UButtonBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UImageOverlayedButtonBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UImageOverlayedButtonBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UImageOverlayedButtonBase::StaticRegisterNativesUImageOverlayedButtonBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UImageOverlayedButtonBase);
	UClass* Z_Construct_UClass_UImageOverlayedButtonBase_NoRegister()
	{
		return UImageOverlayedButtonBase::StaticClass();
	}
	struct Z_Construct_UClass_UImageOverlayedButtonBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UImageOverlayedButtonBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UButtonBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UImageOverlayedButtonBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * is an button with an text and an image, wraps an imageOverlay class object\n */" },
		{ "IncludePath", "ui/Widgets/buttons/subtypes/ImageOverlayedButtonBase.h" },
		{ "ModuleRelativePath", "ui/Widgets/buttons/subtypes/ImageOverlayedButtonBase.h" },
		{ "ToolTip", "is an button with an text and an image, wraps an imageOverlay class object" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UImageOverlayedButtonBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UImageOverlayedButtonBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UImageOverlayedButtonBase_Statics::ClassParams = {
		&UImageOverlayedButtonBase::StaticClass,
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
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UImageOverlayedButtonBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UImageOverlayedButtonBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UImageOverlayedButtonBase()
	{
		if (!Z_Registration_Info_UClass_UImageOverlayedButtonBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UImageOverlayedButtonBase.OuterSingleton, Z_Construct_UClass_UImageOverlayedButtonBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UImageOverlayedButtonBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UImageOverlayedButtonBase>()
	{
		return UImageOverlayedButtonBase::StaticClass();
	}
	UImageOverlayedButtonBase::UImageOverlayedButtonBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UImageOverlayedButtonBase);
	UImageOverlayedButtonBase::~UImageOverlayedButtonBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_subtypes_ImageOverlayedButtonBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_subtypes_ImageOverlayedButtonBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UImageOverlayedButtonBase, UImageOverlayedButtonBase::StaticClass, TEXT("UImageOverlayedButtonBase"), &Z_Registration_Info_UClass_UImageOverlayedButtonBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UImageOverlayedButtonBase), 444082739U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_subtypes_ImageOverlayedButtonBase_h_2316415076(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_subtypes_ImageOverlayedButtonBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_subtypes_ImageOverlayedButtonBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
