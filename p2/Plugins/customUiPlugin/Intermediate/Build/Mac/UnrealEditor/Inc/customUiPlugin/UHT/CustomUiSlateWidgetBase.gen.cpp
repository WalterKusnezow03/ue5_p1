// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/slate/UWidgetConversion/customUiBase/CustomUiSlateWidgetBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCustomUiSlateWidgetBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UCustomUiSlateWidgetBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UCustomUiSlateWidgetBase_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetSlateWrapperBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UCustomUiSlateWidgetBase::StaticRegisterNativesUCustomUiSlateWidgetBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCustomUiSlateWidgetBase);
	UClass* Z_Construct_UClass_UCustomUiSlateWidgetBase_NoRegister()
	{
		return UCustomUiSlateWidgetBase::StaticClass();
	}
	struct Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_widget_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_widget;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UcustomUiComponentBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// --- UWIDGET : SLATE to UCUSTOM UI COMPONENT\n/// @brief makes the uwidget wrapper for slate base usuable as a UcustomUiComponent for this plugin\n/// is first of all a debug class!\n" },
		{ "IncludePath", "slate/UWidgetConversion/customUiBase/CustomUiSlateWidgetBase.h" },
		{ "ModuleRelativePath", "slate/UWidgetConversion/customUiBase/CustomUiSlateWidgetBase.h" },
		{ "ToolTip", "--- UWIDGET : SLATE to UCUSTOM UI COMPONENT\n@brief makes the uwidget wrapper for slate base usuable as a UcustomUiComponent for this plugin\nis first of all a debug class!" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::NewProp_widget_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "slate/UWidgetConversion/customUiBase/CustomUiSlateWidgetBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::NewProp_widget = { "widget", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UCustomUiSlateWidgetBase, widget), Z_Construct_UClass_UWidgetSlateWrapperBase_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::NewProp_widget_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::NewProp_widget_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::NewProp_widget,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCustomUiSlateWidgetBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::ClassParams = {
		&UCustomUiSlateWidgetBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCustomUiSlateWidgetBase()
	{
		if (!Z_Registration_Info_UClass_UCustomUiSlateWidgetBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCustomUiSlateWidgetBase.OuterSingleton, Z_Construct_UClass_UCustomUiSlateWidgetBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UCustomUiSlateWidgetBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UCustomUiSlateWidgetBase>()
	{
		return UCustomUiSlateWidgetBase::StaticClass();
	}
	UCustomUiSlateWidgetBase::UCustomUiSlateWidgetBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCustomUiSlateWidgetBase);
	UCustomUiSlateWidgetBase::~UCustomUiSlateWidgetBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slate_UWidgetConversion_customUiBase_CustomUiSlateWidgetBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slate_UWidgetConversion_customUiBase_CustomUiSlateWidgetBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UCustomUiSlateWidgetBase, UCustomUiSlateWidgetBase::StaticClass, TEXT("UCustomUiSlateWidgetBase"), &Z_Registration_Info_UClass_UCustomUiSlateWidgetBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCustomUiSlateWidgetBase), 1506709121U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slate_UWidgetConversion_customUiBase_CustomUiSlateWidgetBase_h_684108349(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slate_UWidgetConversion_customUiBase_CustomUiSlateWidgetBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slate_UWidgetConversion_customUiBase_CustomUiSlateWidgetBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
