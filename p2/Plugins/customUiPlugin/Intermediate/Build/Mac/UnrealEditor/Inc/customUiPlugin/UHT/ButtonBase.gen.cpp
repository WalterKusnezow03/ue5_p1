// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/buttons/ButtonBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeButtonBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UButtonBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UButtonBase_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UButtonBase::StaticRegisterNativesUButtonBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UButtonBase);
	UClass* Z_Construct_UClass_UButtonBase_NoRegister()
	{
		return UButtonBase::StaticClass();
	}
	struct Z_Construct_UClass_UButtonBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UButtonBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UcustomUiComponentBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UButtonBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * button with scaled content\n * \n * button (base)\n * -->scale box (scales content to parent)\n * --->content inside scale box\n * \n */" },
		{ "IncludePath", "ui/Widgets/buttons/ButtonBase.h" },
		{ "ModuleRelativePath", "ui/Widgets/buttons/ButtonBase.h" },
		{ "ToolTip", "button with scaled content\n\nbutton (base)\n-->scale box (scales content to parent)\n--->content inside scale box" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UButtonBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UButtonBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UButtonBase_Statics::ClassParams = {
		&UButtonBase::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UButtonBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UButtonBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UButtonBase()
	{
		if (!Z_Registration_Info_UClass_UButtonBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UButtonBase.OuterSingleton, Z_Construct_UClass_UButtonBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UButtonBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UButtonBase>()
	{
		return UButtonBase::StaticClass();
	}
	UButtonBase::UButtonBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UButtonBase);
	UButtonBase::~UButtonBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_ButtonBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_ButtonBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UButtonBase, UButtonBase::StaticClass, TEXT("UButtonBase"), &Z_Registration_Info_UClass_UButtonBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UButtonBase), 2312814934U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_ButtonBase_h_993720808(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_ButtonBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_ButtonBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
