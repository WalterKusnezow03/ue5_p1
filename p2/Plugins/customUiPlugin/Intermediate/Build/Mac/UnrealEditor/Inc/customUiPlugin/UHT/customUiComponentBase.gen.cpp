// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/_baseClass/customUiComponentBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodecustomUiComponentBase() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UBaseUiInterface_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UPayLoadBase_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UcustomUiComponentBase::StaticRegisterNativesUcustomUiComponentBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UcustomUiComponentBase);
	UClass* Z_Construct_UClass_UcustomUiComponentBase_NoRegister()
	{
		return UcustomUiComponentBase::StaticClass();
	}
	struct Z_Construct_UClass_UcustomUiComponentBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_payload_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_payload;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UECodeGen_Private::FImplementedInterfaceParams InterfaceParams[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UcustomUiComponentBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UcustomUiComponentBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * base functionality each custom ui element should have \n * (the pointer to the main owning UPlayerUi instance)\n */" },
		{ "IncludePath", "ui/_baseClass/customUiComponentBase.h" },
		{ "ModuleRelativePath", "ui/_baseClass/customUiComponentBase.h" },
		{ "ToolTip", "base functionality each custom ui element should have\n(the pointer to the main owning UPlayerUi instance)" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UcustomUiComponentBase_Statics::NewProp_payload_MetaData[] = {
		{ "Comment", "/// @brief payload item, ptr can be derived.\n" },
		{ "ModuleRelativePath", "ui/_baseClass/customUiComponentBase.h" },
		{ "ToolTip", "@brief payload item, ptr can be derived." },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UcustomUiComponentBase_Statics::NewProp_payload = { "payload", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UcustomUiComponentBase, payload), Z_Construct_UClass_UPayLoadBase_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UcustomUiComponentBase_Statics::NewProp_payload_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UcustomUiComponentBase_Statics::NewProp_payload_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UcustomUiComponentBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UcustomUiComponentBase_Statics::NewProp_payload,
	};
		const UECodeGen_Private::FImplementedInterfaceParams Z_Construct_UClass_UcustomUiComponentBase_Statics::InterfaceParams[] = {
			{ Z_Construct_UClass_UBaseUiInterface_NoRegister, (int32)VTABLE_OFFSET(UcustomUiComponentBase, IBaseUiInterface), false },  // 2471973962
		};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UcustomUiComponentBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UcustomUiComponentBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UcustomUiComponentBase_Statics::ClassParams = {
		&UcustomUiComponentBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UcustomUiComponentBase_Statics::PropPointers,
		InterfaceParams,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UcustomUiComponentBase_Statics::PropPointers),
		UE_ARRAY_COUNT(InterfaceParams),
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UcustomUiComponentBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UcustomUiComponentBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UcustomUiComponentBase()
	{
		if (!Z_Registration_Info_UClass_UcustomUiComponentBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UcustomUiComponentBase.OuterSingleton, Z_Construct_UClass_UcustomUiComponentBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UcustomUiComponentBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UcustomUiComponentBase>()
	{
		return UcustomUiComponentBase::StaticClass();
	}
	UcustomUiComponentBase::UcustomUiComponentBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UcustomUiComponentBase);
	UcustomUiComponentBase::~UcustomUiComponentBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_customUiComponentBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_customUiComponentBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UcustomUiComponentBase, UcustomUiComponentBase::StaticClass, TEXT("UcustomUiComponentBase"), &Z_Registration_Info_UClass_UcustomUiComponentBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UcustomUiComponentBase), 1817189574U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_customUiComponentBase_h_2769717778(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_customUiComponentBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui__baseClass_customUiComponentBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
