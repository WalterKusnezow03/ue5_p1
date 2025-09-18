// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/slateDerived/TextBased/Text/WidgetSlateText.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWidgetSlateText() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetSlateText();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetSlateText_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetSlateWrapperBase();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UWidgetSlateText::StaticRegisterNativesUWidgetSlateText()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWidgetSlateText);
	UClass* Z_Construct_UClass_UWidgetSlateText_NoRegister()
	{
		return UWidgetSlateText::StaticClass();
	}
	struct Z_Construct_UClass_UWidgetSlateText_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWidgetSlateText_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UWidgetSlateWrapperBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWidgetSlateText_Statics::Class_MetaDataParams[] = {
		{ "Comment", "///@brief wont do anything except providing a simple box with a text.\n///will setup design, font etc on its own. Default design.\n" },
		{ "IncludePath", "slateDerived/TextBased/Text/WidgetSlateText.h" },
		{ "ModuleRelativePath", "slateDerived/TextBased/Text/WidgetSlateText.h" },
		{ "ToolTip", "@brief wont do anything except providing a simple box with a text.\nwill setup design, font etc on its own. Default design." },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWidgetSlateText_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWidgetSlateText>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWidgetSlateText_Statics::ClassParams = {
		&UWidgetSlateText::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UWidgetSlateText_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWidgetSlateText_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWidgetSlateText()
	{
		if (!Z_Registration_Info_UClass_UWidgetSlateText.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWidgetSlateText.OuterSingleton, Z_Construct_UClass_UWidgetSlateText_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWidgetSlateText.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UWidgetSlateText>()
	{
		return UWidgetSlateText::StaticClass();
	}
	UWidgetSlateText::UWidgetSlateText(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWidgetSlateText);
	UWidgetSlateText::~UWidgetSlateText() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_TextBased_Text_WidgetSlateText_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_TextBased_Text_WidgetSlateText_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWidgetSlateText, UWidgetSlateText::StaticClass, TEXT("UWidgetSlateText"), &Z_Registration_Info_UClass_UWidgetSlateText, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWidgetSlateText), 3535314442U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_TextBased_Text_WidgetSlateText_h_3662762466(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_TextBased_Text_WidgetSlateText_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_TextBased_Text_WidgetSlateText_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
