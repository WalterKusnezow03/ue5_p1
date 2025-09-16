// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/autoContainer/Hbox.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeHbox() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UAutoBoxBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UHbox();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UHbox_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UHbox::StaticRegisterNativesUHbox()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UHbox);
	UClass* Z_Construct_UClass_UHbox_NoRegister()
	{
		return UHbox::StaticClass();
	}
	struct Z_Construct_UClass_UHbox_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UHbox_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UAutoBoxBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UHbox_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief hbox wrapper to track custom visibility and click listening\n" },
		{ "IncludePath", "ui/Widgets/autoContainer/Hbox.h" },
		{ "ModuleRelativePath", "ui/Widgets/autoContainer/Hbox.h" },
		{ "ToolTip", "@brief hbox wrapper to track custom visibility and click listening" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UHbox_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UHbox>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UHbox_Statics::ClassParams = {
		&UHbox::StaticClass,
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
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UHbox_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UHbox_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UHbox()
	{
		if (!Z_Registration_Info_UClass_UHbox.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UHbox.OuterSingleton, Z_Construct_UClass_UHbox_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UHbox.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UHbox>()
	{
		return UHbox::StaticClass();
	}
	UHbox::UHbox(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UHbox);
	UHbox::~UHbox() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_Hbox_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_Hbox_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UHbox, UHbox::StaticClass, TEXT("UHbox"), &Z_Registration_Info_UClass_UHbox, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UHbox), 1614223999U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_Hbox_h_1381504776(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_Hbox_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_autoContainer_Hbox_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
