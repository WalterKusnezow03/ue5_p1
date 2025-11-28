// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "../../../Source/ComputerVisionPlugin/Public/ComputerVision/Communication/Connection/PythonSocket.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePythonSocket() {}
// Cross Module References
	COMPUTERVISIONPLUGIN_API UClass* Z_Construct_UClass_APythonSocket();
	COMPUTERVISIONPLUGIN_API UClass* Z_Construct_UClass_APythonSocket_NoRegister();
	ENGINE_API UClass* Z_Construct_UClass_AActor();
	UPackage* Z_Construct_UPackage__Script_ComputerVisionPlugin();
// End Cross Module References
	void APythonSocket::StaticRegisterNativesAPythonSocket()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(APythonSocket);
	UClass* Z_Construct_UClass_APythonSocket_NoRegister()
	{
		return APythonSocket::StaticClass();
	}
	struct Z_Construct_UClass_APythonSocket_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_APythonSocket_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AActor,
		(UObject* (*)())Z_Construct_UPackage__Script_ComputerVisionPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_APythonSocket_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "ComputerVision/Communication/Connection/PythonSocket.h" },
		{ "ModuleRelativePath", "Public/ComputerVision/Communication/Connection/PythonSocket.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_APythonSocket_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<APythonSocket>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_APythonSocket_Statics::ClassParams = {
		&APythonSocket::StaticClass,
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
		METADATA_PARAMS(Z_Construct_UClass_APythonSocket_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_APythonSocket_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_APythonSocket()
	{
		if (!Z_Registration_Info_UClass_APythonSocket.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_APythonSocket.OuterSingleton, Z_Construct_UClass_APythonSocket_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_APythonSocket.OuterSingleton;
	}
	template<> COMPUTERVISIONPLUGIN_API UClass* StaticClass<APythonSocket>()
	{
		return APythonSocket::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(APythonSocket);
	APythonSocket::~APythonSocket() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_ComputerVisionPlugin_Source_ComputerVisionPlugin_Public_ComputerVision_Communication_Connection_PythonSocket_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_ComputerVisionPlugin_Source_ComputerVisionPlugin_Public_ComputerVision_Communication_Connection_PythonSocket_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_APythonSocket, APythonSocket::StaticClass, TEXT("APythonSocket"), &Z_Registration_Info_UClass_APythonSocket, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(APythonSocket), 700803804U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_ComputerVisionPlugin_Source_ComputerVisionPlugin_Public_ComputerVision_Communication_Connection_PythonSocket_h_1806211371(TEXT("/Script/ComputerVisionPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_ComputerVisionPlugin_Source_ComputerVisionPlugin_Public_ComputerVision_Communication_Connection_PythonSocket_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_ComputerVisionPlugin_Source_ComputerVisionPlugin_Public_ComputerVision_Communication_Connection_PythonSocket_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
