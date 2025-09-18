// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/slateDerived/TextBased/TextInputSupport/WidgetSlateEditableText.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWidgetSlateEditableText() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetSlateEditableText();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetSlateEditableText_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetSlateText();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UWidgetSlateEditableText::StaticRegisterNativesUWidgetSlateEditableText()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWidgetSlateEditableText);
	UClass* Z_Construct_UClass_UWidgetSlateEditableText_NoRegister()
	{
		return UWidgetSlateEditableText::StaticClass();
	}
	struct Z_Construct_UClass_UWidgetSlateEditableText_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWidgetSlateEditableText_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UWidgetSlateText,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWidgetSlateEditableText_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// --- Same as base class but listens for text input ---\n" },
		{ "IncludePath", "slateDerived/TextBased/TextInputSupport/WidgetSlateEditableText.h" },
		{ "ModuleRelativePath", "slateDerived/TextBased/TextInputSupport/WidgetSlateEditableText.h" },
		{ "ToolTip", "--- Same as base class but listens for text input ---" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWidgetSlateEditableText_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWidgetSlateEditableText>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWidgetSlateEditableText_Statics::ClassParams = {
		&UWidgetSlateEditableText::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UWidgetSlateEditableText_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWidgetSlateEditableText_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWidgetSlateEditableText()
	{
		if (!Z_Registration_Info_UClass_UWidgetSlateEditableText.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWidgetSlateEditableText.OuterSingleton, Z_Construct_UClass_UWidgetSlateEditableText_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWidgetSlateEditableText.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UWidgetSlateEditableText>()
	{
		return UWidgetSlateEditableText::StaticClass();
	}
	UWidgetSlateEditableText::UWidgetSlateEditableText(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWidgetSlateEditableText);
	UWidgetSlateEditableText::~UWidgetSlateEditableText() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_TextBased_TextInputSupport_WidgetSlateEditableText_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_TextBased_TextInputSupport_WidgetSlateEditableText_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWidgetSlateEditableText, UWidgetSlateEditableText::StaticClass, TEXT("UWidgetSlateEditableText"), &Z_Registration_Info_UClass_UWidgetSlateEditableText, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWidgetSlateEditableText), 1865361122U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_TextBased_TextInputSupport_WidgetSlateEditableText_h_3930674346(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_TextBased_TextInputSupport_WidgetSlateEditableText_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_TextBased_TextInputSupport_WidgetSlateEditableText_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
