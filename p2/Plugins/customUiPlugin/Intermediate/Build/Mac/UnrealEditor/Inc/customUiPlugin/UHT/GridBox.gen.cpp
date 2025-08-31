// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/autoContainer/GridBox.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGridBox() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UAutoBoxBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UGridBox();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UGridBox_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UGridPanel_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UGridBox::StaticRegisterNativesUGridBox()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UGridBox);
	UClass* Z_Construct_UClass_UGridBox_NoRegister()
	{
		return UGridBox::StaticClass();
	}
	struct Z_Construct_UClass_UGridBox_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_gridBoxLayout_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_gridBoxLayout;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UGridBox_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UAutoBoxBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UGridBox_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief is a grid, can add childs to the next free position, or at a wanted position\n/// can remove columns or rows, can increase its size, but new items exceeding the size will be added\n/// as a new row\n" },
		{ "IncludePath", "ui/Widgets/autoContainer/GridBox.h" },
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/GridBox.h" },
		{ "ToolTip", "@brief is a grid, can add childs to the next free position, or at a wanted position\ncan remove columns or rows, can increase its size, but new items exceeding the size will be added\nas a new row" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UGridBox_Statics::NewProp_gridBoxLayout_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/GridBox.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UGridBox_Statics::NewProp_gridBoxLayout = { "gridBoxLayout", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UGridBox, gridBoxLayout), Z_Construct_UClass_UGridPanel_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UGridBox_Statics::NewProp_gridBoxLayout_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UGridBox_Statics::NewProp_gridBoxLayout_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UGridBox_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UGridBox_Statics::NewProp_gridBoxLayout,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UGridBox_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGridBox>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UGridBox_Statics::ClassParams = {
		&UGridBox::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UGridBox_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UGridBox_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UGridBox_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UGridBox_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UGridBox()
	{
		if (!Z_Registration_Info_UClass_UGridBox.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UGridBox.OuterSingleton, Z_Construct_UClass_UGridBox_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UGridBox.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UGridBox>()
	{
		return UGridBox::StaticClass();
	}
	UGridBox::UGridBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UGridBox);
	UGridBox::~UGridBox() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_GridBox_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_GridBox_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UGridBox, UGridBox::StaticClass, TEXT("UGridBox"), &Z_Registration_Info_UClass_UGridBox, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UGridBox), 1746044253U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_GridBox_h_3886378137(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_GridBox_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_GridBox_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
