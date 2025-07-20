// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/alignmentPresets/PresetHalfSplitLayout.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePresetHalfSplitLayout() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UPresetHalfSplitLayout();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UPresetHalfSplitLayout_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UPresetHalfSplitLayout::StaticRegisterNativesUPresetHalfSplitLayout()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UPresetHalfSplitLayout);
	UClass* Z_Construct_UClass_UPresetHalfSplitLayout_NoRegister()
	{
		return UPresetHalfSplitLayout::StaticClass();
	}
	struct Z_Construct_UClass_UPresetHalfSplitLayout_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UPresetHalfSplitLayout_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UcustomUiComponentBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPresetHalfSplitLayout_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * screen for the player to choose weapons and attachments for example, layout A and B side to side\n * \n * headline \n * -------- | ---------------------\n * weapons  | weapon or attachment \n * - picked | picker (iterable(?))\n *   ONE    |     MUTLIPLE\n *          |\n */" },
		{ "IncludePath", "ui/alignmentPresets/PresetHalfSplitLayout.h" },
		{ "ModuleRelativePath", "ui/alignmentPresets/PresetHalfSplitLayout.h" },
		{ "ToolTip", "screen for the player to choose weapons and attachments for example, layout A and B side to side\n\nheadline\n-------- | ---------------------\nweapons  | weapon or attachment\n- picked | picker (iterable(?))\n  ONE    |     MUTLIPLE\n         |" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UPresetHalfSplitLayout_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPresetHalfSplitLayout>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UPresetHalfSplitLayout_Statics::ClassParams = {
		&UPresetHalfSplitLayout::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UPresetHalfSplitLayout()
	{
		if (!Z_Registration_Info_UClass_UPresetHalfSplitLayout.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UPresetHalfSplitLayout.OuterSingleton, Z_Construct_UClass_UPresetHalfSplitLayout_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UPresetHalfSplitLayout.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UPresetHalfSplitLayout>()
	{
		return UPresetHalfSplitLayout::StaticClass();
	}
	UPresetHalfSplitLayout::UPresetHalfSplitLayout(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPresetHalfSplitLayout);
	UPresetHalfSplitLayout::~UPresetHalfSplitLayout() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetHalfSplitLayout_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetHalfSplitLayout_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UPresetHalfSplitLayout, UPresetHalfSplitLayout::StaticClass, TEXT("UPresetHalfSplitLayout"), &Z_Registration_Info_UClass_UPresetHalfSplitLayout, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPresetHalfSplitLayout), 2896223182U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetHalfSplitLayout_h_1271888054(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetHalfSplitLayout_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetHalfSplitLayout_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
