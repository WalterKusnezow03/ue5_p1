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
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UHbox_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UPresetHalfSplitLayout();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UPresetHalfSplitLayout_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UVbox_NoRegister();
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
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_baseVertical_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_baseVertical;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_rightLeftContainer_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_rightLeftContainer;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_rightVertical_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_rightVertical;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_leftVertical_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_leftVertical;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_headLineHorizontal_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_headLineHorizontal;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
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
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_baseVertical_MetaData[] = {
		{ "Comment", "/// @brief TRACKS ALL CHILDS ALSO FOR CLICK LISTENING AND TICK!!\n" },
		{ "ModuleRelativePath", "ui/alignmentPresets/PresetHalfSplitLayout.h" },
		{ "ToolTip", "@brief TRACKS ALL CHILDS ALSO FOR CLICK LISTENING AND TICK!!" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_baseVertical = { "baseVertical", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UPresetHalfSplitLayout, baseVertical), Z_Construct_UClass_UVbox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_baseVertical_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_baseVertical_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_rightLeftContainer_MetaData[] = {
		{ "Comment", "//INNER CONTAINERS DO NOT TICK! AGREGATION NOT OWNERSHIP\n" },
		{ "ModuleRelativePath", "ui/alignmentPresets/PresetHalfSplitLayout.h" },
		{ "ToolTip", "INNER CONTAINERS DO NOT TICK! AGREGATION NOT OWNERSHIP" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_rightLeftContainer = { "rightLeftContainer", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UPresetHalfSplitLayout, rightLeftContainer), Z_Construct_UClass_UHbox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_rightLeftContainer_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_rightLeftContainer_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_rightVertical_MetaData[] = {
		{ "ModuleRelativePath", "ui/alignmentPresets/PresetHalfSplitLayout.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_rightVertical = { "rightVertical", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UPresetHalfSplitLayout, rightVertical), Z_Construct_UClass_UVbox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_rightVertical_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_rightVertical_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_leftVertical_MetaData[] = {
		{ "Comment", "//iterierbar / changable machen\n" },
		{ "ModuleRelativePath", "ui/alignmentPresets/PresetHalfSplitLayout.h" },
		{ "ToolTip", "iterierbar / changable machen" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_leftVertical = { "leftVertical", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UPresetHalfSplitLayout, leftVertical), Z_Construct_UClass_UVbox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_leftVertical_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_leftVertical_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_headLineHorizontal_MetaData[] = {
		{ "ModuleRelativePath", "ui/alignmentPresets/PresetHalfSplitLayout.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_headLineHorizontal = { "headLineHorizontal", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UPresetHalfSplitLayout, headLineHorizontal), Z_Construct_UClass_UHbox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_headLineHorizontal_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_headLineHorizontal_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UPresetHalfSplitLayout_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_baseVertical,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_rightLeftContainer,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_rightVertical,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_leftVertical,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UPresetHalfSplitLayout_Statics::NewProp_headLineHorizontal,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UPresetHalfSplitLayout_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPresetHalfSplitLayout>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UPresetHalfSplitLayout_Statics::ClassParams = {
		&UPresetHalfSplitLayout::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UPresetHalfSplitLayout_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UPresetHalfSplitLayout_Statics::PropPointers),
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
		{ Z_Construct_UClass_UPresetHalfSplitLayout, UPresetHalfSplitLayout::StaticClass, TEXT("UPresetHalfSplitLayout"), &Z_Registration_Info_UClass_UPresetHalfSplitLayout, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPresetHalfSplitLayout), 1919191179U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetHalfSplitLayout_h_2176906875(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetHalfSplitLayout_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_alignmentPresets_PresetHalfSplitLayout_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
