// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "PathFinder/pathFinding/PathFinder.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePathFinder() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	PATHFINDER_API UClass* Z_Construct_UClass_APathFinder();
	PATHFINDER_API UClass* Z_Construct_UClass_APathFinder_NoRegister();
	UPackage* Z_Construct_UPackage__Script_PathFinder();
// End Cross Module References
	void APathFinder::StaticRegisterNativesAPathFinder()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(APathFinder);
	UClass* Z_Construct_UClass_APathFinder_NoRegister()
	{
		return APathFinder::StaticClass();
	}
	struct Z_Construct_UClass_APathFinder_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_APathFinder_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_PathFinder,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APathFinder_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * a modified a* version with tangential checks along convex polygons for minimzed graphs.\n * Nodes are found in O(1)!\n * \n * 2 modes:\n * \n * 1)\n * which operates on subgraphs and rechecks edges on runtime\n * the edges are build and checked on runtime because we want to operate on subgraphs\n * efficently. Also it adds supports for dynamic actors like vehicles blocking paths temporarily\n * \n * it also eliminates all non tangential edges during runtime because they are never part \n * of a shortest path!\n * \n * 2)\n * A node which will prebuild all tangential edges when adding a node\n * toggle the according boolean in this header file:\n * will automatically connect nodes, reduces runtime overhead because minimal tangential graph is already build\n * \n */" },
		{ "IncludePath", "pathFinding/PathFinder.h" },
		{ "ModuleRelativePath", "pathFinding/PathFinder.h" },
		{ "ToolTip", "a modified a* version with tangential checks along convex polygons for minimzed graphs.\nNodes are found in O(1)!\n\n2 modes:\n\n1)\nwhich operates on subgraphs and rechecks edges on runtime\nthe edges are build and checked on runtime because we want to operate on subgraphs\nefficently. Also it adds supports for dynamic actors like vehicles blocking paths temporarily\n\nit also eliminates all non tangential edges during runtime because they are never part\nof a shortest path!\n\n2)\nA node which will prebuild all tangential edges when adding a node\ntoggle the according boolean in this header file:\nwill automatically connect nodes, reduces runtime overhead because minimal tangential graph is already build" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_APathFinder_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<APathFinder>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_APathFinder_Statics::ClassParams = {
		&APathFinder::StaticClass,
		"Engine",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x009000A4u,
		METADATA_PARAMS(Z_Construct_UClass_APathFinder_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_APathFinder_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_APathFinder()
	{
		if (!Z_Registration_Info_UClass_APathFinder.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_APathFinder.OuterSingleton, Z_Construct_UClass_APathFinder_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_APathFinder.OuterSingleton;
	}
	template<> PATHFINDER_API UClass* StaticClass<APathFinder>()
	{
		return APathFinder::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(APathFinder);
	APathFinder::~APathFinder() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_PathFinder_Source_PathFinder_pathFinding_PathFinder_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_PathFinder_Source_PathFinder_pathFinding_PathFinder_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_APathFinder, APathFinder::StaticClass, TEXT("APathFinder"), &Z_Registration_Info_UClass_APathFinder, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(APathFinder), 860018738U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_PathFinder_Source_PathFinder_pathFinding_PathFinder_h_1433060029(TEXT("/Script/PathFinder"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_PathFinder_Source_PathFinder_pathFinding_PathFinder_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_PathFinder_Source_PathFinder_pathFinding_PathFinder_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
