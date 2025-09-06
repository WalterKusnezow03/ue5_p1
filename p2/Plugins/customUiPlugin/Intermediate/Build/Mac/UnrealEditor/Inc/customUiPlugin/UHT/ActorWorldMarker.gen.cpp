// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/slateDerived/WorldToScreenOverlays/item/actorBased/ActorWorldMarker.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeActorWorldMarker() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UActorWorldMarker();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UActorWorldMarker_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWorldMarker();
	ENGINE_API UClass* Z_Construct_UClass_AActor_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UActorWorldMarker::StaticRegisterNativesUActorWorldMarker()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UActorWorldMarker);
	UClass* Z_Construct_UClass_UActorWorldMarker_NoRegister()
	{
		return UActorWorldMarker::StaticClass();
	}
	struct Z_Construct_UClass_UActorWorldMarker_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_attachedActor_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_attachedActor;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UActorWorldMarker_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UWorldMarker,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UActorWorldMarker_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief Actor marker can be added to WorldMarker canvas, will track AActor by ptr to \n/// update location manually and no tick from an aatcor instance is needed.\n" },
		{ "IncludePath", "slateDerived/WorldToScreenOverlays/item/actorBased/ActorWorldMarker.h" },
		{ "ModuleRelativePath", "slateDerived/WorldToScreenOverlays/item/actorBased/ActorWorldMarker.h" },
		{ "ToolTip", "@brief Actor marker can be added to WorldMarker canvas, will track AActor by ptr to\nupdate location manually and no tick from an aatcor instance is needed." },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UActorWorldMarker_Statics::NewProp_attachedActor_MetaData[] = {
		{ "ModuleRelativePath", "slateDerived/WorldToScreenOverlays/item/actorBased/ActorWorldMarker.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UActorWorldMarker_Statics::NewProp_attachedActor = { "attachedActor", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UActorWorldMarker, attachedActor), Z_Construct_UClass_AActor_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UActorWorldMarker_Statics::NewProp_attachedActor_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UActorWorldMarker_Statics::NewProp_attachedActor_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UActorWorldMarker_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UActorWorldMarker_Statics::NewProp_attachedActor,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UActorWorldMarker_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UActorWorldMarker>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UActorWorldMarker_Statics::ClassParams = {
		&UActorWorldMarker::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UActorWorldMarker_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UActorWorldMarker_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UActorWorldMarker_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UActorWorldMarker_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UActorWorldMarker()
	{
		if (!Z_Registration_Info_UClass_UActorWorldMarker.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UActorWorldMarker.OuterSingleton, Z_Construct_UClass_UActorWorldMarker_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UActorWorldMarker.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UActorWorldMarker>()
	{
		return UActorWorldMarker::StaticClass();
	}
	UActorWorldMarker::UActorWorldMarker(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UActorWorldMarker);
	UActorWorldMarker::~UActorWorldMarker() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_WorldToScreenOverlays_item_actorBased_ActorWorldMarker_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_WorldToScreenOverlays_item_actorBased_ActorWorldMarker_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UActorWorldMarker, UActorWorldMarker::StaticClass, TEXT("UActorWorldMarker"), &Z_Registration_Info_UClass_UActorWorldMarker, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UActorWorldMarker), 3849091737U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_WorldToScreenOverlays_item_actorBased_ActorWorldMarker_h_2664193177(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_WorldToScreenOverlays_item_actorBased_ActorWorldMarker_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_slateDerived_WorldToScreenOverlays_item_actorBased_ActorWorldMarker_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
