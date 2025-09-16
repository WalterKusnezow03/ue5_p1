// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/slateDerived/Shapes/ArrowWidget/WidgetArrowBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWidgetArrowBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetArrowBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetArrowBase_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWidgetSlateWrapperBase();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UWidgetArrowBase::StaticRegisterNativesUWidgetArrowBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWidgetArrowBase);
	UClass* Z_Construct_UClass_UWidgetArrowBase_NoRegister()
	{
		return UWidgetArrowBase::StaticClass();
	}
	struct Z_Construct_UClass_UWidgetArrowBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWidgetArrowBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UWidgetSlateWrapperBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWidgetArrowBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief direves from UWidgetSlateWrapperBase to get the mesh data from internal pointer.\n/// tick still needed to listen for cursor position and animations!\n" },
		{ "IncludePath", "slateDerived/Shapes/ArrowWidget/WidgetArrowBase.h" },
		{ "ModuleRelativePath", "slateDerived/Shapes/ArrowWidget/WidgetArrowBase.h" },
		{ "ToolTip", "@brief direves from UWidgetSlateWrapperBase to get the mesh data from internal pointer.\ntick still needed to listen for cursor position and animations!" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWidgetArrowBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWidgetArrowBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWidgetArrowBase_Statics::ClassParams = {
		&UWidgetArrowBase::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UWidgetArrowBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWidgetArrowBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWidgetArrowBase()
	{
		if (!Z_Registration_Info_UClass_UWidgetArrowBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWidgetArrowBase.OuterSingleton, Z_Construct_UClass_UWidgetArrowBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWidgetArrowBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UWidgetArrowBase>()
	{
		return UWidgetArrowBase::StaticClass();
	}
	UWidgetArrowBase::UWidgetArrowBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWidgetArrowBase);
	UWidgetArrowBase::~UWidgetArrowBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_Shapes_ArrowWidget_WidgetArrowBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_Shapes_ArrowWidget_WidgetArrowBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWidgetArrowBase, UWidgetArrowBase::StaticClass, TEXT("UWidgetArrowBase"), &Z_Registration_Info_UClass_UWidgetArrowBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWidgetArrowBase), 182648121U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_Shapes_ArrowWidget_WidgetArrowBase_h_833946269(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_Shapes_ArrowWidget_WidgetArrowBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_Shapes_ArrowWidget_WidgetArrowBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
