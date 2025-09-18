// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/autoContainer/DropDownBase/VerticalDropDownBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVerticalDropDownBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UHbox_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UTextButton_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UVbox();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UVbox_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UVerticalDropDownBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UVerticalDropDownBase_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetArrowBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UVerticalDropDownBase::StaticRegisterNativesUVerticalDropDownBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVerticalDropDownBase);
	UClass* Z_Construct_UClass_UVerticalDropDownBase_NoRegister()
	{
		return UVerticalDropDownBase::StaticClass();
	}
	struct Z_Construct_UClass_UVerticalDropDownBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_topSelection_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_topSelection;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_selectableList_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_selectableList;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_topTextButton_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_topTextButton;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_topArrow_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_topArrow;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UVerticalDropDownBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UVbox,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVerticalDropDownBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief removing childs wont be supported for now.\n/// Selectable items in dropdown list\n" },
		{ "IncludePath", "ui/Widgets/autoContainer/DropDownBase/VerticalDropDownBase.h" },
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/DropDownBase/VerticalDropDownBase.h" },
		{ "ToolTip", "@brief removing childs wont be supported for now.\nSelectable items in dropdown list" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topSelection_MetaData[] = {
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/DropDownBase/VerticalDropDownBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topSelection = { "topSelection", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UVerticalDropDownBase, topSelection), Z_Construct_UClass_UHbox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topSelection_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topSelection_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_selectableList_MetaData[] = {
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/DropDownBase/VerticalDropDownBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_selectableList = { "selectableList", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UVerticalDropDownBase, selectableList), Z_Construct_UClass_UVbox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_selectableList_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_selectableList_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topTextButton_MetaData[] = {
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/DropDownBase/VerticalDropDownBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topTextButton = { "topTextButton", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UVerticalDropDownBase, topTextButton), Z_Construct_UClass_UTextButton_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topTextButton_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topTextButton_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topArrow_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/DropDownBase/VerticalDropDownBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topArrow = { "topArrow", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UVerticalDropDownBase, topArrow), Z_Construct_UClass_UWidgetArrowBase_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topArrow_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topArrow_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVerticalDropDownBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topSelection,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_selectableList,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topTextButton,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVerticalDropDownBase_Statics::NewProp_topArrow,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UVerticalDropDownBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVerticalDropDownBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UVerticalDropDownBase_Statics::ClassParams = {
		&UVerticalDropDownBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UVerticalDropDownBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UVerticalDropDownBase_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UVerticalDropDownBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UVerticalDropDownBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UVerticalDropDownBase()
	{
		if (!Z_Registration_Info_UClass_UVerticalDropDownBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVerticalDropDownBase.OuterSingleton, Z_Construct_UClass_UVerticalDropDownBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UVerticalDropDownBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UVerticalDropDownBase>()
	{
		return UVerticalDropDownBase::StaticClass();
	}
	UVerticalDropDownBase::UVerticalDropDownBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UVerticalDropDownBase);
	UVerticalDropDownBase::~UVerticalDropDownBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_DropDownBase_VerticalDropDownBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_DropDownBase_VerticalDropDownBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UVerticalDropDownBase, UVerticalDropDownBase::StaticClass, TEXT("UVerticalDropDownBase"), &Z_Registration_Info_UClass_UVerticalDropDownBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVerticalDropDownBase), 1418517838U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_DropDownBase_VerticalDropDownBase_h_845793876(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_DropDownBase_VerticalDropDownBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_DropDownBase_VerticalDropDownBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
