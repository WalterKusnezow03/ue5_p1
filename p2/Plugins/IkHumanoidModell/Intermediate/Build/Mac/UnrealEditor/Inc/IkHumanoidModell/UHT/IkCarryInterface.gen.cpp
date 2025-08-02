// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "IkHumanoidModell/carryItems/Interface/IkCarryInterface.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeIkCarryInterface() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
	IKHUMANOIDMODELL_API UClass* Z_Construct_UClass_UIkCarryInterface();
	IKHUMANOIDMODELL_API UClass* Z_Construct_UClass_UIkCarryInterface_NoRegister();
	UPackage* Z_Construct_UPackage__Script_IkHumanoidModell();
// End Cross Module References
	void UIkCarryInterface::StaticRegisterNativesUIkCarryInterface()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UIkCarryInterface);
	UClass* Z_Construct_UClass_UIkCarryInterface_NoRegister()
	{
		return UIkCarryInterface::StaticClass();
	}
	struct Z_Construct_UClass_UIkCarryInterface_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UIkCarryInterface_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UInterface,
		(UObject* (*)())Z_Construct_UPackage__Script_IkHumanoidModell,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UIkCarryInterface_Statics::Class_MetaDataParams[] = {
		{ "ModuleRelativePath", "carryItems/Interface/IkCarryInterface.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UIkCarryInterface_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<IIkCarryInterface>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UIkCarryInterface_Statics::ClassParams = {
		&UIkCarryInterface::StaticClass,
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
		0x001840A1u,
		METADATA_PARAMS(Z_Construct_UClass_UIkCarryInterface_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UIkCarryInterface_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UIkCarryInterface()
	{
		if (!Z_Registration_Info_UClass_UIkCarryInterface.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UIkCarryInterface.OuterSingleton, Z_Construct_UClass_UIkCarryInterface_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UIkCarryInterface.OuterSingleton;
	}
	template<> IKHUMANOIDMODELL_API UClass* StaticClass<UIkCarryInterface>()
	{
		return UIkCarryInterface::StaticClass();
	}
	UIkCarryInterface::UIkCarryInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UIkCarryInterface);
	UIkCarryInterface::~UIkCarryInterface() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_carryItems_Interface_IkCarryInterface_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_carryItems_Interface_IkCarryInterface_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UIkCarryInterface, UIkCarryInterface::StaticClass, TEXT("UIkCarryInterface"), &Z_Registration_Info_UClass_UIkCarryInterface, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UIkCarryInterface), 4042842476U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_carryItems_Interface_IkCarryInterface_h_3009183502(TEXT("/Script/IkHumanoidModell"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_carryItems_Interface_IkCarryInterface_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_IkHumanoidModell_Source_IkHumanoidModell_carryItems_Interface_IkCarryInterface_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
