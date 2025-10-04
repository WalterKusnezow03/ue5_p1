// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "NNCommunicationPlugin/Communication/Connection/NNSocket.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeNNSocket() {}
// Cross Module References
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	NNCOMMUNICATIONPLUGIN_API UClass* Z_Construct_UClass_ANNSocket();
	NNCOMMUNICATIONPLUGIN_API UClass* Z_Construct_UClass_ANNSocket_NoRegister();
	UPackage* Z_Construct_UPackage__Script_NNCommunicationPlugin();
// End Cross Module References
	void ANNSocket::StaticRegisterNativesANNSocket()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(ANNSocket);
	UClass* Z_Construct_UClass_ANNSocket_NoRegister()
	{
		return ANNSocket::StaticClass();
	}
	struct Z_Construct_UClass_ANNSocket_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ANNSocket_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_NNCommunicationPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ANNSocket_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "Communication/Connection/NNSocket.h" },
		{ "ModuleRelativePath", "Communication/Connection/NNSocket.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ANNSocket_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ANNSocket>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_ANNSocket_Statics::ClassParams = {
		&ANNSocket::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_ANNSocket_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ANNSocket_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ANNSocket()
	{
		if (!Z_Registration_Info_UClass_ANNSocket.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_ANNSocket.OuterSingleton, Z_Construct_UClass_ANNSocket_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_ANNSocket.OuterSingleton;
	}
	template<> NNCOMMUNICATIONPLUGIN_API UClass* StaticClass<ANNSocket>()
	{
		return ANNSocket::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(ANNSocket);
	ANNSocket::~ANNSocket() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_NNCommunicationPlugin_Source_NNCommunicationPlugin_Communication_Connection_NNSocket_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_NNCommunicationPlugin_Source_NNCommunicationPlugin_Communication_Connection_NNSocket_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_ANNSocket, ANNSocket::StaticClass, TEXT("ANNSocket"), &Z_Registration_Info_UClass_ANNSocket, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(ANNSocket), 2349512682U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_NNCommunicationPlugin_Source_NNCommunicationPlugin_Communication_Connection_NNSocket_h_2659479520(TEXT("/Script/NNCommunicationPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_NNCommunicationPlugin_Source_NNCommunicationPlugin_Communication_Connection_NNSocket_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_NNCommunicationPlugin_Source_NNCommunicationPlugin_Communication_Connection_NNSocket_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
