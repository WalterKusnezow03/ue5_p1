// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/screens/WorldToScreenOverlays/item/WorldMarker.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWorldMarker() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWorldMarker();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWorldMarker_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWorldMarkerCanvas_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UWidget_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UWorldMarker::StaticRegisterNativesUWorldMarker()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWorldMarker);
	UClass* Z_Construct_UClass_UWorldMarker_NoRegister()
	{
		return UWorldMarker::StaticClass();
	}
	struct Z_Construct_UClass_UWorldMarker_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_widget_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_widget;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_parent_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_parent;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWorldMarker_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWorldMarker_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief Will hold any UWidget or UcustomUiComponentBase inside this world marker,\n/// IS A NORMAL 2D WIDGET MANUALLY PROJECTED!\n/// with a position, max distance from player, used by the WorldMarkerCanvas class\n/// can be constructed outside the ui and added later to have dynamic markers (for example, following aactor!)\n" },
		{ "IncludePath", "ui/screens/WorldToScreenOverlays/item/WorldMarker.h" },
		{ "ModuleRelativePath", "ui/screens/WorldToScreenOverlays/item/WorldMarker.h" },
		{ "ToolTip", "@brief Will hold any UWidget or UcustomUiComponentBase inside this world marker,\nIS A NORMAL 2D WIDGET MANUALLY PROJECTED!\nwith a position, max distance from player, used by the WorldMarkerCanvas class\ncan be constructed outside the ui and added later to have dynamic markers (for example, following aactor!)" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWorldMarker_Statics::NewProp_widget_MetaData[] = {
		{ "Comment", "//from interface or other. Ownership not taken.\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/screens/WorldToScreenOverlays/item/WorldMarker.h" },
		{ "ToolTip", "from interface or other. Ownership not taken." },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWorldMarker_Statics::NewProp_widget = { "widget", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWorldMarker, widget), Z_Construct_UClass_UWidget_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UWorldMarker_Statics::NewProp_widget_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWorldMarker_Statics::NewProp_widget_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWorldMarker_Statics::NewProp_parent_MetaData[] = {
		{ "Comment", "//refernce to parent\n" },
		{ "ModuleRelativePath", "ui/screens/WorldToScreenOverlays/item/WorldMarker.h" },
		{ "ToolTip", "refernce to parent" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWorldMarker_Statics::NewProp_parent = { "parent", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWorldMarker, parent), Z_Construct_UClass_UWorldMarkerCanvas_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UWorldMarker_Statics::NewProp_parent_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWorldMarker_Statics::NewProp_parent_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWorldMarker_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWorldMarker_Statics::NewProp_widget,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWorldMarker_Statics::NewProp_parent,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWorldMarker_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWorldMarker>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWorldMarker_Statics::ClassParams = {
		&UWorldMarker::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UWorldMarker_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UWorldMarker_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UWorldMarker_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWorldMarker_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWorldMarker()
	{
		if (!Z_Registration_Info_UClass_UWorldMarker.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWorldMarker.OuterSingleton, Z_Construct_UClass_UWorldMarker_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWorldMarker.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UWorldMarker>()
	{
		return UWorldMarker::StaticClass();
	}
	UWorldMarker::UWorldMarker(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWorldMarker);
	UWorldMarker::~UWorldMarker() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_item_WorldMarker_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_item_WorldMarker_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWorldMarker, UWorldMarker::StaticClass, TEXT("UWorldMarker"), &Z_Registration_Info_UClass_UWorldMarker, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWorldMarker), 1451481056U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_item_WorldMarker_h_1010342968(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_item_WorldMarker_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_item_WorldMarker_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
