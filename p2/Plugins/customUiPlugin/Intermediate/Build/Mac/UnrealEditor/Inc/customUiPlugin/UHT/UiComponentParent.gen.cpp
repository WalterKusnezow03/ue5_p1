// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/_baseClass/baseParent/UiComponentParent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUiComponentParent() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UUiComponentParent();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UUiComponentParent_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UWidget_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UUiComponentParent::StaticRegisterNativesUUiComponentParent()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UUiComponentParent);
	UClass* Z_Construct_UClass_UUiComponentParent_NoRegister()
	{
		return UUiComponentParent::StaticClass();
	}
	struct Z_Construct_UClass_UUiComponentParent_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_trackedChildsAsUWidget_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_trackedChildsAsUWidget_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_trackedChildsAsUWidget;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UUiComponentParent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UcustomUiComponentBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUiComponentParent_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief Abstract Class to track children and dispatch click, tick, user input\n" },
		{ "IncludePath", "ui/_baseClass/baseParent/UiComponentParent.h" },
		{ "ModuleRelativePath", "ui/_baseClass/baseParent/UiComponentParent.h" },
		{ "ToolTip", "@brief Abstract Class to track children and dispatch click, tick, user input" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UUiComponentParent_Statics::NewProp_trackedChildsAsUWidget_Inner = { "trackedChildsAsUWidget", nullptr, (EPropertyFlags)0x0000000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, 0, Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UUiComponentParent_Statics::NewProp_trackedChildsAsUWidget_MetaData[] = {
		{ "Comment", "//cannot be marked Uproperty\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/_baseClass/baseParent/UiComponentParent.h" },
		{ "ToolTip", "cannot be marked Uproperty" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UUiComponentParent_Statics::NewProp_trackedChildsAsUWidget = { "trackedChildsAsUWidget", nullptr, (EPropertyFlags)0x0020088000000008, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UUiComponentParent, trackedChildsAsUWidget), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_UUiComponentParent_Statics::NewProp_trackedChildsAsUWidget_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UUiComponentParent_Statics::NewProp_trackedChildsAsUWidget_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UUiComponentParent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUiComponentParent_Statics::NewProp_trackedChildsAsUWidget_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UUiComponentParent_Statics::NewProp_trackedChildsAsUWidget,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UUiComponentParent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UUiComponentParent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UUiComponentParent_Statics::ClassParams = {
		&UUiComponentParent::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UUiComponentParent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UUiComponentParent_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UUiComponentParent_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UUiComponentParent_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UUiComponentParent()
	{
		if (!Z_Registration_Info_UClass_UUiComponentParent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UUiComponentParent.OuterSingleton, Z_Construct_UClass_UUiComponentParent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UUiComponentParent.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UUiComponentParent>()
	{
		return UUiComponentParent::StaticClass();
	}
	UUiComponentParent::UUiComponentParent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UUiComponentParent);
	UUiComponentParent::~UUiComponentParent() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_baseParent_UiComponentParent_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_baseParent_UiComponentParent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UUiComponentParent, UUiComponentParent::StaticClass, TEXT("UUiComponentParent"), &Z_Registration_Info_UClass_UUiComponentParent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UUiComponentParent), 1479589489U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_baseParent_UiComponentParent_h_2381242600(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_baseParent_UiComponentParent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_baseParent_UiComponentParent_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
