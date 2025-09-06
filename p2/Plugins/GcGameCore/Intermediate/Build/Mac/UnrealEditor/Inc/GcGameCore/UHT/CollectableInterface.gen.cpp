// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GcGameCore/Collection/interface/CollectableInterface.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCollectableInterface() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UInterface();
	GCGAMECORE_API UClass* Z_Construct_UClass_UCollectableInterface();
	GCGAMECORE_API UClass* Z_Construct_UClass_UCollectableInterface_NoRegister();
	UPackage* Z_Construct_UPackage__Script_GcGameCore();
// End Cross Module References
	void UCollectableInterface::StaticRegisterNativesUCollectableInterface()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCollectableInterface);
	UClass* Z_Construct_UClass_UCollectableInterface_NoRegister()
	{
		return UCollectableInterface::StaticClass();
	}
	struct Z_Construct_UClass_UCollectableInterface_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCollectableInterface_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UInterface,
		(UObject* (*)())Z_Construct_UPackage__Script_GcGameCore,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCollectableInterface_Statics::Class_MetaDataParams[] = {
		{ "ModuleRelativePath", "Collection/interface/CollectableInterface.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCollectableInterface_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ICollectableInterface>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UCollectableInterface_Statics::ClassParams = {
		&UCollectableInterface::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UCollectableInterface_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UCollectableInterface_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCollectableInterface()
	{
		if (!Z_Registration_Info_UClass_UCollectableInterface.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCollectableInterface.OuterSingleton, Z_Construct_UClass_UCollectableInterface_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UCollectableInterface.OuterSingleton;
	}
	template<> GCGAMECORE_API UClass* StaticClass<UCollectableInterface>()
	{
		return UCollectableInterface::StaticClass();
	}
	UCollectableInterface::UCollectableInterface(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCollectableInterface);
	UCollectableInterface::~UCollectableInterface() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UCollectableInterface, UCollectableInterface::StaticClass, TEXT("UCollectableInterface"), &Z_Registration_Info_UClass_UCollectableInterface, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCollectableInterface), 1447346186U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_3921912405(TEXT("/Script/GcGameCore"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
