// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/alignmentPresets/PresetCornersLayout.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePresetCornersLayout() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UPresetCornersLayout();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UPresetCornersLayout_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UPresetCornersLayout::StaticRegisterNativesUPresetCornersLayout()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UPresetCornersLayout);
	UClass* Z_Construct_UClass_UPresetCornersLayout_NoRegister()
	{
		return UPresetCornersLayout::StaticClass();
	}
	struct Z_Construct_UClass_UPresetCornersLayout_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UPresetCornersLayout_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UcustomUiComponentBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPresetCornersLayout_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * preset corners layout with vertical boxes\n */" },
		{ "IncludePath", "ui/alignmentPresets/PresetCornersLayout.h" },
		{ "ModuleRelativePath", "ui/alignmentPresets/PresetCornersLayout.h" },
		{ "ToolTip", "preset corners layout with vertical boxes" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UPresetCornersLayout_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPresetCornersLayout>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UPresetCornersLayout_Statics::ClassParams = {
		&UPresetCornersLayout::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UPresetCornersLayout_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UPresetCornersLayout_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UPresetCornersLayout()
	{
		if (!Z_Registration_Info_UClass_UPresetCornersLayout.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UPresetCornersLayout.OuterSingleton, Z_Construct_UClass_UPresetCornersLayout_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UPresetCornersLayout.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UPresetCornersLayout>()
	{
		return UPresetCornersLayout::StaticClass();
	}
	UPresetCornersLayout::UPresetCornersLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPresetCornersLayout);
	UPresetCornersLayout::~UPresetCornersLayout() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetCornersLayout_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetCornersLayout_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UPresetCornersLayout, UPresetCornersLayout::StaticClass, TEXT("UPresetCornersLayout"), &Z_Registration_Info_UClass_UPresetCornersLayout, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPresetCornersLayout), 2216155054U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetCornersLayout_h_4065496359(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetCornersLayout_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetCornersLayout_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
