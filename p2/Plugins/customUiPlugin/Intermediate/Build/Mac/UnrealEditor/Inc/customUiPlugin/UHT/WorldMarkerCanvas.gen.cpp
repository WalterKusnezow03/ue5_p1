// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/screens/WorldToScreenOverlays/canvas/WorldMarkerCanvas.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeWorldMarkerCanvas() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UCanvasScreen();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWorldMarker_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWorldMarkerCanvas();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UWorldMarkerCanvas_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_APlayerController_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UWorldMarkerCanvas::StaticRegisterNativesUWorldMarkerCanvas()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UWorldMarkerCanvas);
	UClass* Z_Construct_UClass_UWorldMarkerCanvas_NoRegister()
	{
		return UWorldMarkerCanvas::StaticClass();
	}
	struct Z_Construct_UClass_UWorldMarkerCanvas_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_trackedMarkers_Inner;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_trackedMarkers_MetaData[];
#endif
		static const UECodeGen_Private::FArrayPropertyParams NewProp_trackedMarkers;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Playercontroller_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_Playercontroller;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UWorldMarkerCanvas_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UCanvasScreen,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWorldMarkerCanvas_Statics::Class_MetaDataParams[] = {
		{ "Comment", "///@brief will save any kind of \"WidgetSlateWrapperBase\" tied to a world position\n///and render it to thu UCanvasScreen\n///can for example display interaction keys \"E to pickup etc..\"\n/// ---> Will tick world marker manually, no other widget add needed here!\n" },
		{ "IncludePath", "ui/screens/WorldToScreenOverlays/canvas/WorldMarkerCanvas.h" },
		{ "ModuleRelativePath", "ui/screens/WorldToScreenOverlays/canvas/WorldMarkerCanvas.h" },
		{ "ToolTip", "@brief will save any kind of \"WidgetSlateWrapperBase\" tied to a world position\nand render it to thu UCanvasScreen\ncan for example display interaction keys \"E to pickup etc..\"\n ---> Will tick world marker manually, no other widget add needed here!" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_trackedMarkers_Inner = { "trackedMarkers", nullptr, (EPropertyFlags)0x0000000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, 0, Z_Construct_UClass_UWorldMarker_NoRegister, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_trackedMarkers_MetaData[] = {
		{ "ModuleRelativePath", "ui/screens/WorldToScreenOverlays/canvas/WorldMarkerCanvas.h" },
	};
#endif
	const UECodeGen_Private::FArrayPropertyParams Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_trackedMarkers = { "trackedMarkers", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWorldMarkerCanvas, trackedMarkers), EArrayPropertyFlags::None, METADATA_PARAMS(Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_trackedMarkers_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_trackedMarkers_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_Playercontroller_MetaData[] = {
		{ "ModuleRelativePath", "ui/screens/WorldToScreenOverlays/canvas/WorldMarkerCanvas.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_Playercontroller = { "Playercontroller", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UWorldMarkerCanvas, Playercontroller), Z_Construct_UClass_APlayerController_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_Playercontroller_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_Playercontroller_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UWorldMarkerCanvas_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_trackedMarkers_Inner,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_trackedMarkers,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UWorldMarkerCanvas_Statics::NewProp_Playercontroller,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UWorldMarkerCanvas_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UWorldMarkerCanvas>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UWorldMarkerCanvas_Statics::ClassParams = {
		&UWorldMarkerCanvas::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UWorldMarkerCanvas_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UWorldMarkerCanvas_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UWorldMarkerCanvas_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UWorldMarkerCanvas_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UWorldMarkerCanvas()
	{
		if (!Z_Registration_Info_UClass_UWorldMarkerCanvas.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UWorldMarkerCanvas.OuterSingleton, Z_Construct_UClass_UWorldMarkerCanvas_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UWorldMarkerCanvas.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UWorldMarkerCanvas>()
	{
		return UWorldMarkerCanvas::StaticClass();
	}
	UWorldMarkerCanvas::UWorldMarkerCanvas(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UWorldMarkerCanvas);
	UWorldMarkerCanvas::~UWorldMarkerCanvas() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_canvas_WorldMarkerCanvas_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_canvas_WorldMarkerCanvas_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UWorldMarkerCanvas, UWorldMarkerCanvas::StaticClass, TEXT("UWorldMarkerCanvas"), &Z_Registration_Info_UClass_UWorldMarkerCanvas, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UWorldMarkerCanvas), 3811694772U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_canvas_WorldMarkerCanvas_h_3044695560(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_canvas_WorldMarkerCanvas_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_screens_WorldToScreenOverlays_canvas_WorldMarkerCanvas_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
