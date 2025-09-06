// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/buttons/subtypes/TextButton.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTextButton() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UButtonBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UTextButton();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UTextButton_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UTextBlock_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UTextButton::StaticRegisterNativesUTextButton()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UTextButton);
	UClass* Z_Construct_UClass_UTextButton_NoRegister()
	{
		return UTextButton::StaticClass();
	}
	struct Z_Construct_UClass_UTextButton_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TextBlock_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_TextBlock;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UTextButton_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UButtonBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTextButton_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "ui/Widgets/buttons/subtypes/TextButton.h" },
		{ "ModuleRelativePath", "ui/Widgets/buttons/subtypes/TextButton.h" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTextButton_Statics::NewProp_TextBlock_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/buttons/subtypes/TextButton.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UTextButton_Statics::NewProp_TextBlock = { "TextBlock", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UTextButton, TextBlock), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UTextButton_Statics::NewProp_TextBlock_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTextButton_Statics::NewProp_TextBlock_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UTextButton_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTextButton_Statics::NewProp_TextBlock,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UTextButton_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTextButton>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UTextButton_Statics::ClassParams = {
		&UTextButton::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UTextButton_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UTextButton_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UTextButton_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UTextButton_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UTextButton()
	{
		if (!Z_Registration_Info_UClass_UTextButton.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UTextButton.OuterSingleton, Z_Construct_UClass_UTextButton_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UTextButton.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UTextButton>()
	{
		return UTextButton::StaticClass();
	}
	UTextButton::UTextButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UTextButton);
	UTextButton::~UTextButton() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_subtypes_TextButton_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_subtypes_TextButton_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UTextButton, UTextButton::StaticClass, TEXT("UTextButton"), &Z_Registration_Info_UClass_UTextButton, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UTextButton), 3777836343U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_subtypes_TextButton_h_302329738(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_subtypes_TextButton_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_subtypes_TextButton_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
