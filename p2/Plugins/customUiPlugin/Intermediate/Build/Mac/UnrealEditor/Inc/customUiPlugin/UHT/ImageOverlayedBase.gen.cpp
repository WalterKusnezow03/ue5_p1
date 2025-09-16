// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/OverlayBased/ImageOverlayedBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeImageOverlayedBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UImageOverlayedBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UImageOverlayedBase_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UImage_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UOverlay_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UTextBlock_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UImageOverlayedBase::StaticRegisterNativesUImageOverlayedBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UImageOverlayedBase);
	UClass* Z_Construct_UClass_UImageOverlayedBase_NoRegister()
	{
		return UImageOverlayedBase::StaticClass();
	}
	struct Z_Construct_UClass_UImageOverlayedBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_baseOverlay_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_baseOverlay;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TextBlock_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_TextBlock;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Image_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Image;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UImageOverlayedBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UcustomUiComponentBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UImageOverlayedBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * will display an image in background and a text at the top\n */" },
		{ "IncludePath", "ui/Widgets/OverlayBased/ImageOverlayedBase.h" },
		{ "ModuleRelativePath", "ui/Widgets/OverlayBased/ImageOverlayedBase.h" },
		{ "ToolTip", "will display an image in background and a text at the top" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_baseOverlay_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/OverlayBased/ImageOverlayedBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_baseOverlay = { "baseOverlay", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UImageOverlayedBase, baseOverlay), Z_Construct_UClass_UOverlay_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_baseOverlay_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_baseOverlay_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_TextBlock_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/OverlayBased/ImageOverlayedBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_TextBlock = { "TextBlock", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UImageOverlayedBase, TextBlock), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_TextBlock_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_TextBlock_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_Image_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/OverlayBased/ImageOverlayedBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_Image = { "Image", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UImageOverlayedBase, Image), Z_Construct_UClass_UImage_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_Image_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_Image_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UImageOverlayedBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_baseOverlay,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_TextBlock,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UImageOverlayedBase_Statics::NewProp_Image,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UImageOverlayedBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UImageOverlayedBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UImageOverlayedBase_Statics::ClassParams = {
		&UImageOverlayedBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UImageOverlayedBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UImageOverlayedBase_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UImageOverlayedBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UImageOverlayedBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UImageOverlayedBase()
	{
		if (!Z_Registration_Info_UClass_UImageOverlayedBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UImageOverlayedBase.OuterSingleton, Z_Construct_UClass_UImageOverlayedBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UImageOverlayedBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UImageOverlayedBase>()
	{
		return UImageOverlayedBase::StaticClass();
	}
	UImageOverlayedBase::UImageOverlayedBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UImageOverlayedBase);
	UImageOverlayedBase::~UImageOverlayedBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_OverlayBased_ImageOverlayedBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_OverlayBased_ImageOverlayedBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UImageOverlayedBase, UImageOverlayedBase::StaticClass, TEXT("UImageOverlayedBase"), &Z_Registration_Info_UClass_UImageOverlayedBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UImageOverlayedBase), 1685939197U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_OverlayBased_ImageOverlayedBase_h_3310471251(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_OverlayBased_ImageOverlayedBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_OverlayBased_ImageOverlayedBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
