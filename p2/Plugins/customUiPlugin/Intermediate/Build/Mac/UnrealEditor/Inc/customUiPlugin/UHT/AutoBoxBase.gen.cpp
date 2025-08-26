// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/autoContainer/AutoBoxBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeAutoBoxBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UAutoBoxBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UAutoBoxBase_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UAutoBoxBase::StaticRegisterNativesUAutoBoxBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UAutoBoxBase);
	UClass* Z_Construct_UClass_UAutoBoxBase_NoRegister()
	{
		return UAutoBoxBase::StaticClass();
	}
	struct Z_Construct_UClass_UAutoBoxBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_attachedItems_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_attachedItems_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_attachedItems;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UAutoBoxBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UcustomUiComponentBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAutoBoxBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief ABSTRACT designed as a abstract base class for hbox and vbox containers\n" },
		{ "IncludePath", "ui/Widgets/autoContainer/AutoBoxBase.h" },
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/AutoBoxBase.h" },
		{ "ToolTip", "@brief ABSTRACT designed as a abstract base class for hbox and vbox containers" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UAutoBoxBase_Statics::NewProp_attachedItems_Inner = { "attachedItems", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, 0, Z_Construct_UClass_UcustomUiComponentBase_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UAutoBoxBase_Statics::NewProp_attachedItems_MetaData[] = {
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/AutoBoxBase.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UAutoBoxBase_Statics::NewProp_attachedItems = { "attachedItems", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UAutoBoxBase, attachedItems), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_UAutoBoxBase_Statics::NewProp_attachedItems_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UAutoBoxBase_Statics::NewProp_attachedItems_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UAutoBoxBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UAutoBoxBase_Statics::NewProp_attachedItems_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UAutoBoxBase_Statics::NewProp_attachedItems,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UAutoBoxBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAutoBoxBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UAutoBoxBase_Statics::ClassParams = {
		&UAutoBoxBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UAutoBoxBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UAutoBoxBase_Statics::PropPointers),
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UAutoBoxBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UAutoBoxBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UAutoBoxBase()
	{
		if (!Z_Registration_Info_UClass_UAutoBoxBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UAutoBoxBase.OuterSingleton, Z_Construct_UClass_UAutoBoxBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UAutoBoxBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UAutoBoxBase>()
	{
		return UAutoBoxBase::StaticClass();
	}
	UAutoBoxBase::UAutoBoxBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UAutoBoxBase);
	UAutoBoxBase::~UAutoBoxBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_AutoBoxBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_AutoBoxBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UAutoBoxBase, UAutoBoxBase::StaticClass, TEXT("UAutoBoxBase"), &Z_Registration_Info_UClass_UAutoBoxBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UAutoBoxBase), 463711618U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_AutoBoxBase_h_1342255603(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_AutoBoxBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_AutoBoxBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
