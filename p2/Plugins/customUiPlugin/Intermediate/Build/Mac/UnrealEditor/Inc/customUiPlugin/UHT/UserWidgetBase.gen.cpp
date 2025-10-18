// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/WorldWidgets/UUserWidgetBase/UserWidgetBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUserWidgetBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UUserWidgetBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UUserWidgetBase_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UTextBlock_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UUserWidget();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UUserWidgetBase::StaticRegisterNativesUUserWidgetBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUserWidgetBase);
	UClass* Z_Construct_UClass_UUserWidgetBase_NoRegister()
	{
		return UUserWidgetBase::StaticClass();
	}
	struct Z_Construct_UClass_UUserWidgetBase_Statics
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
	UObject* (*const Z_Construct_UClass_UUserWidgetBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUserWidget,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUserWidgetBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief will be the base class for 3D World User Widgets\n" },
		{ "IncludePath", "WorldWidgets/UUserWidgetBase/UserWidgetBase.h" },
		{ "ModuleRelativePath", "WorldWidgets/UUserWidgetBase/UserWidgetBase.h" },
		{ "ToolTip", "@brief will be the base class for 3D World User Widgets" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUserWidgetBase_Statics::NewProp_TextBlock_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "WorldWidgets/UUserWidgetBase/UserWidgetBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUserWidgetBase_Statics::NewProp_TextBlock = { "TextBlock", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UUserWidgetBase, TextBlock), Z_Construct_UClass_UTextBlock_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UUserWidgetBase_Statics::NewProp_TextBlock_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UUserWidgetBase_Statics::NewProp_TextBlock_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUserWidgetBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUserWidgetBase_Statics::NewProp_TextBlock,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UUserWidgetBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUserWidgetBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UUserWidgetBase_Statics::ClassParams = {
		&UUserWidgetBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UUserWidgetBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UUserWidgetBase_Statics::PropPointers),
		0,
		0x00B010A0u,
		METADATA_PARAMS(Z_Construct_UClass_UUserWidgetBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UUserWidgetBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UUserWidgetBase()
	{
		if (!Z_Registration_Info_UClass_UUserWidgetBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUserWidgetBase.OuterSingleton, Z_Construct_UClass_UUserWidgetBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UUserWidgetBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UUserWidgetBase>()
	{
		return UUserWidgetBase::StaticClass();
	}
	UUserWidgetBase::UUserWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UUserWidgetBase);
	UUserWidgetBase::~UUserWidgetBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_WorldWidgets_UUserWidgetBase_UserWidgetBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_WorldWidgets_UUserWidgetBase_UserWidgetBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UUserWidgetBase, UUserWidgetBase::StaticClass, TEXT("UUserWidgetBase"), &Z_Registration_Info_UClass_UUserWidgetBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUserWidgetBase), 3948545271U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_WorldWidgets_UUserWidgetBase_UserWidgetBase_h_2310741014(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_WorldWidgets_UUserWidgetBase_UserWidgetBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_WorldWidgets_UUserWidgetBase_UserWidgetBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
