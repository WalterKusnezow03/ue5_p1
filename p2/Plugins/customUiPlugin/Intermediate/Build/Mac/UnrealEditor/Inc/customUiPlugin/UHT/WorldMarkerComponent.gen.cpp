// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/screens/WorldToScreenOverlays/actorComponent/WorldMarkerComponent.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWorldMarkerComponent() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWorldMarker_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWorldMarkerComponent();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWorldMarkerComponent_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_UActorComponent();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UWorldMarkerComponent::StaticRegisterNativesUWorldMarkerComponent()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWorldMarkerComponent);
	UClass* Z_Construct_UClass_UWorldMarkerComponent_NoRegister()
	{
		return UWorldMarkerComponent::StaticClass();
	}
	struct Z_Construct_UClass_UWorldMarkerComponent_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_MyMarker_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_MyMarker;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWorldMarkerComponent_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UActorComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWorldMarkerComponent_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief derived from UActorComponent, not a scene component, more performance.\n" },
		{ "IncludePath", "ui/screens/WorldToScreenOverlays/actorComponent/WorldMarkerComponent.h" },
		{ "ModuleRelativePath", "ui/screens/WorldToScreenOverlays/actorComponent/WorldMarkerComponent.h" },
		{ "ToolTip", "@brief derived from UActorComponent, not a scene component, more performance." },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWorldMarkerComponent_Statics::NewProp_MyMarker_MetaData[] = {
		{ "ModuleRelativePath", "ui/screens/WorldToScreenOverlays/actorComponent/WorldMarkerComponent.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWorldMarkerComponent_Statics::NewProp_MyMarker = { "MyMarker", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWorldMarkerComponent, MyMarker), Z_Construct_UClass_UWorldMarker_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UWorldMarkerComponent_Statics::NewProp_MyMarker_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWorldMarkerComponent_Statics::NewProp_MyMarker_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWorldMarkerComponent_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWorldMarkerComponent_Statics::NewProp_MyMarker,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWorldMarkerComponent_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWorldMarkerComponent>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWorldMarkerComponent_Statics::ClassParams = {
		&UWorldMarkerComponent::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UWorldMarkerComponent_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UWorldMarkerComponent_Statics::PropPointers),
		0,
		0x00B000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UWorldMarkerComponent_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWorldMarkerComponent_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWorldMarkerComponent()
	{
		if (!Z_Registration_Info_UClass_UWorldMarkerComponent.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWorldMarkerComponent.OuterSingleton, Z_Construct_UClass_UWorldMarkerComponent_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWorldMarkerComponent.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UWorldMarkerComponent>()
	{
		return UWorldMarkerComponent::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWorldMarkerComponent);
	UWorldMarkerComponent::~UWorldMarkerComponent() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_actorComponent_WorldMarkerComponent_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_actorComponent_WorldMarkerComponent_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWorldMarkerComponent, UWorldMarkerComponent::StaticClass, TEXT("UWorldMarkerComponent"), &Z_Registration_Info_UClass_UWorldMarkerComponent, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWorldMarkerComponent), 793876335U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_actorComponent_WorldMarkerComponent_h_1324511543(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_actorComponent_WorldMarkerComponent_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_actorComponent_WorldMarkerComponent_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
