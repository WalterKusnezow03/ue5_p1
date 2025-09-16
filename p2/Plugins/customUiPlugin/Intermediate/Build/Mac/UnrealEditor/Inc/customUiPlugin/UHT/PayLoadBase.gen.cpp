// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/_baseClass/payload/PayLoadBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePayLoadBase() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UPayLoadBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UPayLoadBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UPayLoadBase::StaticRegisterNativesUPayLoadBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UPayLoadBase);
	UClass* Z_Construct_UClass_UPayLoadBase_NoRegister()
	{
		return UPayLoadBase::StaticClass();
	}
	struct Z_Construct_UClass_UPayLoadBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UPayLoadBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPayLoadBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief base class to attach payloads to any ucustomUiComponent\n/// derived from uobject for easier mem management.\n/// NEEDS A COPY MECHANISM!\n" },
		{ "IncludePath", "ui/_baseClass/payload/PayLoadBase.h" },
		{ "ModuleRelativePath", "ui/_baseClass/payload/PayLoadBase.h" },
		{ "ToolTip", "@brief base class to attach payloads to any ucustomUiComponent\nderived from uobject for easier mem management.\nNEEDS A COPY MECHANISM!" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UPayLoadBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPayLoadBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UPayLoadBase_Statics::ClassParams = {
		&UPayLoadBase::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UPayLoadBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UPayLoadBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UPayLoadBase()
	{
		if (!Z_Registration_Info_UClass_UPayLoadBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UPayLoadBase.OuterSingleton, Z_Construct_UClass_UPayLoadBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UPayLoadBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UPayLoadBase>()
	{
		return UPayLoadBase::StaticClass();
	}
	UPayLoadBase::UPayLoadBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPayLoadBase);
	UPayLoadBase::~UPayLoadBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_payload_PayLoadBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_payload_PayLoadBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UPayLoadBase, UPayLoadBase::StaticClass, TEXT("UPayLoadBase"), &Z_Registration_Info_UClass_UPayLoadBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPayLoadBase), 3424624053U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_payload_PayLoadBase_h_2522020787(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_payload_PayLoadBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_payload_PayLoadBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
