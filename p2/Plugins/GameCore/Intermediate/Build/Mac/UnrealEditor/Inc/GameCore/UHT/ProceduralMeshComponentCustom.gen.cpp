// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameCore/MeshGenBase/ProceduralMeshComponentDerived/ProceduralMeshComponentCustom.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeProceduralMeshComponentCustom() {}
// Cross Module References
	GAMECORE_API UClass* Z_Construct_UClass_UProceduralMeshComponentCustom();
	GAMECORE_API UClass* Z_Construct_UClass_UProceduralMeshComponentCustom_NoRegister();
	PROCEDURALMESHCOMPONENT_API UClass* Z_Construct_UClass_UProceduralMeshComponent();
	UPackage* Z_Construct_UPackage__Script_GameCore();
// End Cross Module References
	void UProceduralMeshComponentCustom::StaticRegisterNativesUProceduralMeshComponentCustom()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UProceduralMeshComponentCustom);
	UClass* Z_Construct_UClass_UProceduralMeshComponentCustom_NoRegister()
	{
		return UProceduralMeshComponentCustom::StaticClass();
	}
	struct Z_Construct_UClass_UProceduralMeshComponentCustom_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UProceduralMeshComponentCustom_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UProceduralMeshComponent,
		(UObject* (*)())Z_Construct_UPackage__Script_GameCore,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UProceduralMeshComponentCustom_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief doesnt help.\n" },
		{ "HideCategories", "Object LOD Mobility Trigger" },
		{ "IncludePath", "MeshGenBase/ProceduralMeshComponentDerived/ProceduralMeshComponentCustom.h" },
		{ "ModuleRelativePath", "MeshGenBase/ProceduralMeshComponentDerived/ProceduralMeshComponentCustom.h" },
		{ "ToolTip", "@brief doesnt help." },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UProceduralMeshComponentCustom_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UProceduralMeshComponentCustom>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UProceduralMeshComponentCustom_Statics::ClassParams = {
		&UProceduralMeshComponentCustom::StaticClass,
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
		0x00B000A4u,
		METADATA_PARAMS(Z_Construct_UClass_UProceduralMeshComponentCustom_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UProceduralMeshComponentCustom_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UProceduralMeshComponentCustom()
	{
		if (!Z_Registration_Info_UClass_UProceduralMeshComponentCustom.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UProceduralMeshComponentCustom.OuterSingleton, Z_Construct_UClass_UProceduralMeshComponentCustom_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UProceduralMeshComponentCustom.OuterSingleton;
	}
	template<> GAMECORE_API UClass* StaticClass<UProceduralMeshComponentCustom>()
	{
		return UProceduralMeshComponentCustom::StaticClass();
	}
	UProceduralMeshComponentCustom::UProceduralMeshComponentCustom(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UProceduralMeshComponentCustom);
	UProceduralMeshComponentCustom::~UProceduralMeshComponentCustom() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_ProceduralMeshComponentDerived_ProceduralMeshComponentCustom_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_ProceduralMeshComponentDerived_ProceduralMeshComponentCustom_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UProceduralMeshComponentCustom, UProceduralMeshComponentCustom::StaticClass, TEXT("UProceduralMeshComponentCustom"), &Z_Registration_Info_UClass_UProceduralMeshComponentCustom, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UProceduralMeshComponentCustom), 4261782651U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_ProceduralMeshComponentDerived_ProceduralMeshComponentCustom_h_3090948490(TEXT("/Script/GameCore"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_ProceduralMeshComponentDerived_ProceduralMeshComponentCustom_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_GameCore_Source_GameCore_MeshGenBase_ProceduralMeshComponentDerived_ProceduralMeshComponentCustom_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
