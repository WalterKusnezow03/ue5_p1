// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/TextBased/TextAndImageBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTextAndImageBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UTextAndImageBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UTextAndImageBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UTextAndImageBase::StaticRegisterNativesUTextAndImageBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UTextAndImageBase);
	UClass* Z_Construct_UClass_UTextAndImageBase_NoRegister()
	{
		return UTextAndImageBase::StaticClass();
	}
	struct Z_Construct_UClass_UTextAndImageBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UTextAndImageBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UcustomUiComponentBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTextAndImageBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * label with image like class in an horizontal layout\n */" },
		{ "IncludePath", "ui/Widgets/TextBased/TextAndImageBase.h" },
		{ "ModuleRelativePath", "ui/Widgets/TextBased/TextAndImageBase.h" },
		{ "ToolTip", "label with image like class in an horizontal layout" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UTextAndImageBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTextAndImageBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UTextAndImageBase_Statics::ClassParams = {
		&UTextAndImageBase::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UTextAndImageBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UTextAndImageBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UTextAndImageBase()
	{
		if (!Z_Registration_Info_UClass_UTextAndImageBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UTextAndImageBase.OuterSingleton, Z_Construct_UClass_UTextAndImageBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UTextAndImageBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UTextAndImageBase>()
	{
		return UTextAndImageBase::StaticClass();
	}
	UTextAndImageBase::UTextAndImageBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UTextAndImageBase);
	UTextAndImageBase::~UTextAndImageBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_TextBased_TextAndImageBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_TextBased_TextAndImageBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UTextAndImageBase, UTextAndImageBase::StaticClass, TEXT("UTextAndImageBase"), &Z_Registration_Info_UClass_UTextAndImageBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UTextAndImageBase), 3564310577U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_TextBased_TextAndImageBase_h_2422184660(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_TextBased_TextAndImageBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_TextBased_TextAndImageBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
