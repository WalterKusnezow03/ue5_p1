// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Vbox.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeVbox() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UAutoBoxBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UVbox();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UVbox_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UVerticalBox_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UVbox::StaticRegisterNativesUVbox()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UVbox);
	UClass* Z_Construct_UClass_UVbox_NoRegister()
	{
		return UVbox::StaticClass();
	}
	struct Z_Construct_UClass_UVbox_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_baseVBox_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_baseVBox;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UVbox_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UAutoBoxBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVbox_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief vbox wrapper to track custom visibility and click listening\n" },
		{ "IncludePath", "ui/Widgets/autoContainer/Vbox.h" },
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/Vbox.h" },
		{ "ToolTip", "@brief vbox wrapper to track custom visibility and click listening" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UVbox_Statics::NewProp_baseVBox_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/Vbox.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UVbox_Statics::NewProp_baseVBox = { "baseVBox", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UVbox, baseVBox), Z_Construct_UClass_UVerticalBox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UVbox_Statics::NewProp_baseVBox_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UVbox_Statics::NewProp_baseVBox_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UVbox_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UVbox_Statics::NewProp_baseVBox,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UVbox_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UVbox>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UVbox_Statics::ClassParams = {
		&UVbox::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UVbox_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UVbox_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UVbox_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UVbox_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UVbox()
	{
		if (!Z_Registration_Info_UClass_UVbox.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UVbox.OuterSingleton, Z_Construct_UClass_UVbox_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UVbox.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UVbox>()
	{
		return UVbox::StaticClass();
	}
	UVbox::UVbox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UVbox);
	UVbox::~UVbox() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_Vbox_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_Vbox_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UVbox, UVbox::StaticClass, TEXT("UVbox"), &Z_Registration_Info_UClass_UVbox, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UVbox), 4199585988U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_Vbox_h_2977295000(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_Vbox_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_Vbox_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
