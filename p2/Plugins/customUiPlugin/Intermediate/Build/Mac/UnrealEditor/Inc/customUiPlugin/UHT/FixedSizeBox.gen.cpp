// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/autoContainer/sizing/FixedSizeBox.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeFixedSizeBox() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UFixedSizeBox();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UFixedSizeBox_NoRegister();
	UMG_API UClass* Z_Construct_UClass_USizeBox_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UFixedSizeBox::StaticRegisterNativesUFixedSizeBox()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UFixedSizeBox);
	UClass* Z_Construct_UClass_UFixedSizeBox_NoRegister()
	{
		return UFixedSizeBox::StaticClass();
	}
	struct Z_Construct_UClass_UFixedSizeBox_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_SizeBox_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_SizeBox;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_child_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_child;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UFixedSizeBox_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UcustomUiComponentBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UFixedSizeBox_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "ui/Widgets/autoContainer/sizing/FixedSizeBox.h" },
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/sizing/FixedSizeBox.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UFixedSizeBox_Statics::NewProp_SizeBox_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/sizing/FixedSizeBox.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UFixedSizeBox_Statics::NewProp_SizeBox = { "SizeBox", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UFixedSizeBox, SizeBox), Z_Construct_UClass_USizeBox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UFixedSizeBox_Statics::NewProp_SizeBox_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UFixedSizeBox_Statics::NewProp_SizeBox_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UFixedSizeBox_Statics::NewProp_child_MetaData[] = {
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/sizing/FixedSizeBox.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UFixedSizeBox_Statics::NewProp_child = { "child", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UFixedSizeBox, child), Z_Construct_UClass_UcustomUiComponentBase_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UFixedSizeBox_Statics::NewProp_child_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UFixedSizeBox_Statics::NewProp_child_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UFixedSizeBox_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UFixedSizeBox_Statics::NewProp_SizeBox,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UFixedSizeBox_Statics::NewProp_child,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UFixedSizeBox_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UFixedSizeBox>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UFixedSizeBox_Statics::ClassParams = {
		&UFixedSizeBox::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UFixedSizeBox_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UFixedSizeBox_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UFixedSizeBox_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UFixedSizeBox_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UFixedSizeBox()
	{
		if (!Z_Registration_Info_UClass_UFixedSizeBox.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UFixedSizeBox.OuterSingleton, Z_Construct_UClass_UFixedSizeBox_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UFixedSizeBox.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UFixedSizeBox>()
	{
		return UFixedSizeBox::StaticClass();
	}
	UFixedSizeBox::UFixedSizeBox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UFixedSizeBox);
	UFixedSizeBox::~UFixedSizeBox() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_sizing_FixedSizeBox_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_sizing_FixedSizeBox_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UFixedSizeBox, UFixedSizeBox::StaticClass, TEXT("UFixedSizeBox"), &Z_Registration_Info_UClass_UFixedSizeBox, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UFixedSizeBox), 3129947726U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_sizing_FixedSizeBox_h_3200938606(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_sizing_FixedSizeBox_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_sizing_FixedSizeBox_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
