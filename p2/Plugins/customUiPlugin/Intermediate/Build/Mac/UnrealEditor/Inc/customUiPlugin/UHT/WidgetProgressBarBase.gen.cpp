// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/slateDerived/bar/WidgetProgressBarBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWidgetProgressBarBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetProgressBarBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetProgressBarBase_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetSlateWrapperBase();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UWidgetProgressBarBase::StaticRegisterNativesUWidgetProgressBarBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWidgetProgressBarBase);
	UClass* Z_Construct_UClass_UWidgetProgressBarBase_NoRegister()
	{
		return UWidgetProgressBarBase::StaticClass();
	}
	struct Z_Construct_UClass_UWidgetProgressBarBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWidgetProgressBarBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UWidgetSlateWrapperBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWidgetProgressBarBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief direves from UWidgetSlateWrapperBase to get the mesh data from internal pointer.\n/// tick still needed to listen for cursor position and animations!\n" },
		{ "IncludePath", "slateDerived/bar/WidgetProgressBarBase.h" },
		{ "ModuleRelativePath", "slateDerived/bar/WidgetProgressBarBase.h" },
		{ "ToolTip", "@brief direves from UWidgetSlateWrapperBase to get the mesh data from internal pointer.\ntick still needed to listen for cursor position and animations!" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWidgetProgressBarBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWidgetProgressBarBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWidgetProgressBarBase_Statics::ClassParams = {
		&UWidgetProgressBarBase::StaticClass,
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
		0x00B000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UWidgetProgressBarBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWidgetProgressBarBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWidgetProgressBarBase()
	{
		if (!Z_Registration_Info_UClass_UWidgetProgressBarBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWidgetProgressBarBase.OuterSingleton, Z_Construct_UClass_UWidgetProgressBarBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWidgetProgressBarBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UWidgetProgressBarBase>()
	{
		return UWidgetProgressBarBase::StaticClass();
	}
	UWidgetProgressBarBase::UWidgetProgressBarBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWidgetProgressBarBase);
	UWidgetProgressBarBase::~UWidgetProgressBarBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_bar_WidgetProgressBarBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_bar_WidgetProgressBarBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWidgetProgressBarBase, UWidgetProgressBarBase::StaticClass, TEXT("UWidgetProgressBarBase"), &Z_Registration_Info_UClass_UWidgetProgressBarBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWidgetProgressBarBase), 2009352595U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_bar_WidgetProgressBarBase_h_1422961333(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_bar_WidgetProgressBarBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_bar_WidgetProgressBarBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
