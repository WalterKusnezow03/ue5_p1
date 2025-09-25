// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "AssetMakerEditorPlugin/AssetMaker/MeshDataToStaticMesh/EditorMeshDataToStaticMeshTool.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeEditorMeshDataToStaticMeshTool() {}
// Cross Module References
	ASSETMAKEREDITORPLUGIN_API UClass* Z_Construct_UClass_UEditorMeshDataToStaticMeshTool();
	ASSETMAKEREDITORPLUGIN_API UClass* Z_Construct_UClass_UEditorMeshDataToStaticMeshTool_NoRegister();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_AssetMakerEditorPlugin();
// End Cross Module References
	void UEditorMeshDataToStaticMeshTool::StaticRegisterNativesUEditorMeshDataToStaticMeshTool()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UEditorMeshDataToStaticMeshTool);
	UClass* Z_Construct_UClass_UEditorMeshDataToStaticMeshTool_NoRegister()
	{
		return UEditorMeshDataToStaticMeshTool::StaticClass();
	}
	struct Z_Construct_UClass_UEditorMeshDataToStaticMeshTool_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UEditorMeshDataToStaticMeshTool_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_AssetMakerEditorPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UEditorMeshDataToStaticMeshTool_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief can save MeshData as UStaticMesh in a directory of your liking. use in editor only.\n/// do NOT remove macro, engine will crash otherwise :)\n" },
		{ "IncludePath", "AssetMaker/MeshDataToStaticMesh/EditorMeshDataToStaticMeshTool.h" },
		{ "ModuleRelativePath", "AssetMaker/MeshDataToStaticMesh/EditorMeshDataToStaticMeshTool.h" },
		{ "ToolTip", "@brief can save MeshData as UStaticMesh in a directory of your liking. use in editor only.\ndo NOT remove macro, engine will crash otherwise :)" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UEditorMeshDataToStaticMeshTool_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UEditorMeshDataToStaticMeshTool>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UEditorMeshDataToStaticMeshTool_Statics::ClassParams = {
		&UEditorMeshDataToStaticMeshTool::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_UEditorMeshDataToStaticMeshTool_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UEditorMeshDataToStaticMeshTool_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UEditorMeshDataToStaticMeshTool()
	{
		if (!Z_Registration_Info_UClass_UEditorMeshDataToStaticMeshTool.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UEditorMeshDataToStaticMeshTool.OuterSingleton, Z_Construct_UClass_UEditorMeshDataToStaticMeshTool_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UEditorMeshDataToStaticMeshTool.OuterSingleton;
	}
	template<> ASSETMAKEREDITORPLUGIN_API UClass* StaticClass<UEditorMeshDataToStaticMeshTool>()
	{
		return UEditorMeshDataToStaticMeshTool::StaticClass();
	}
	UEditorMeshDataToStaticMeshTool::UEditorMeshDataToStaticMeshTool(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UEditorMeshDataToStaticMeshTool);
	UEditorMeshDataToStaticMeshTool::~UEditorMeshDataToStaticMeshTool() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetMakerEditorPlugin_Source_AssetMakerEditorPlugin_AssetMaker_MeshDataToStaticMesh_EditorMeshDataToStaticMeshTool_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetMakerEditorPlugin_Source_AssetMakerEditorPlugin_AssetMaker_MeshDataToStaticMesh_EditorMeshDataToStaticMeshTool_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UEditorMeshDataToStaticMeshTool, UEditorMeshDataToStaticMeshTool::StaticClass, TEXT("UEditorMeshDataToStaticMeshTool"), &Z_Registration_Info_UClass_UEditorMeshDataToStaticMeshTool, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UEditorMeshDataToStaticMeshTool), 1786989635U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetMakerEditorPlugin_Source_AssetMakerEditorPlugin_AssetMaker_MeshDataToStaticMesh_EditorMeshDataToStaticMeshTool_h_2112015104(TEXT("/Script/AssetMakerEditorPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetMakerEditorPlugin_Source_AssetMakerEditorPlugin_AssetMaker_MeshDataToStaticMesh_EditorMeshDataToStaticMeshTool_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_AssetMakerEditorPlugin_Source_AssetMakerEditorPlugin_AssetMaker_MeshDataToStaticMesh_EditorMeshDataToStaticMeshTool_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
