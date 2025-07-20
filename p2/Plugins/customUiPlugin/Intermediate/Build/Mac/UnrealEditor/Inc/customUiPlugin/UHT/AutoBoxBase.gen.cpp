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
	const FCppClassTypeInfoStatic Z_Construct_UClass_UAutoBoxBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UAutoBoxBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UAutoBoxBase_Statics::ClassParams = {
		&UAutoBoxBase::StaticClass,
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
		{ Z_Construct_UClass_UAutoBoxBase, UAutoBoxBase::StaticClass, TEXT("UAutoBoxBase"), &Z_Registration_Info_UClass_UAutoBoxBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UAutoBoxBase), 2939456097U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_AutoBoxBase_h_3681848785(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_AutoBoxBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_AutoBoxBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
