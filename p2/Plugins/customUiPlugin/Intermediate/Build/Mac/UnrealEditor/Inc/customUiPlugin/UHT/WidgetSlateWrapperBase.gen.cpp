// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWidgetSlateWrapperBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetSlateWrapperBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetSlateWrapperBase_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UWidget();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UWidgetSlateWrapperBase::StaticRegisterNativesUWidgetSlateWrapperBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWidgetSlateWrapperBase);
	UClass* Z_Construct_UClass_UWidgetSlateWrapperBase_NoRegister()
	{
		return UWidgetSlateWrapperBase::StaticClass();
	}
	struct Z_Construct_UClass_UWidgetSlateWrapperBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWidgetSlateWrapperBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UWidget,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWidgetSlateWrapperBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// --- SLATE TO UWIDGET WRAPPER---\n/// @brief makes the slate base usuable as a UWidget for UMG\n" },
		{ "IncludePath", "slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h" },
		{ "ModuleRelativePath", "slate/UWidgetConversion/UWigetBase/WidgetSlateWrapperBase.h" },
		{ "ToolTip", "--- SLATE TO UWIDGET WRAPPER---\n@brief makes the slate base usuable as a UWidget for UMG" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWidgetSlateWrapperBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWidgetSlateWrapperBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWidgetSlateWrapperBase_Statics::ClassParams = {
		&UWidgetSlateWrapperBase::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UWidgetSlateWrapperBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWidgetSlateWrapperBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWidgetSlateWrapperBase()
	{
		if (!Z_Registration_Info_UClass_UWidgetSlateWrapperBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWidgetSlateWrapperBase.OuterSingleton, Z_Construct_UClass_UWidgetSlateWrapperBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWidgetSlateWrapperBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UWidgetSlateWrapperBase>()
	{
		return UWidgetSlateWrapperBase::StaticClass();
	}
	UWidgetSlateWrapperBase::UWidgetSlateWrapperBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWidgetSlateWrapperBase);
	UWidgetSlateWrapperBase::~UWidgetSlateWrapperBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slate_UWidgetConversion_UWigetBase_WidgetSlateWrapperBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slate_UWidgetConversion_UWigetBase_WidgetSlateWrapperBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWidgetSlateWrapperBase, UWidgetSlateWrapperBase::StaticClass, TEXT("UWidgetSlateWrapperBase"), &Z_Registration_Info_UClass_UWidgetSlateWrapperBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWidgetSlateWrapperBase), 4129691149U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slate_UWidgetConversion_UWigetBase_WidgetSlateWrapperBase_h_2943577656(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slate_UWidgetConversion_UWigetBase_WidgetSlateWrapperBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slate_UWidgetConversion_UWigetBase_WidgetSlateWrapperBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
