// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/callback/callback.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodecallback() {}
// Cross Module References
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UCallback();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UCallback_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	DEFINE_FUNCTION(UCallback::execUCallbackFunction)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->UCallbackFunction();
		P_NATIVE_END;
	}
	void UCallback::StaticRegisterNativesUCallback()
	{
		UClass* Class = UCallback::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "UCallbackFunction", &UCallback::execUCallbackFunction },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UCallback_UCallbackFunction_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UCallback_UCallbackFunction_Statics::Function_MetaDataParams[] = {
		{ "Comment", "//works fine, button sometimes buggy\n" },
		{ "ModuleRelativePath", "ui/Widgets/callback/callback.h" },
		{ "ToolTip", "works fine, button sometimes buggy" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UCallback_UCallbackFunction_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UCallback, nullptr, "UCallbackFunction", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UCallback_UCallbackFunction_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UCallback_UCallbackFunction_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UCallback_UCallbackFunction()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UCallback_UCallbackFunction_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UCallback);
	UClass* Z_Construct_UClass_UCallback_NoRegister()
	{
		return UCallback::StaticClass();
	}
	struct Z_Construct_UClass_UCallback_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UCallback_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UCallback_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UCallback_UCallbackFunction, "UCallbackFunction" }, // 4290753587
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UCallback_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "ui/Widgets/callback/callback.h" },
		{ "ModuleRelativePath", "ui/Widgets/callback/callback.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UCallback_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UCallback>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UCallback_Statics::ClassParams = {
		&UCallback::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UCallback_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UCallback_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UCallback()
	{
		if (!Z_Registration_Info_UClass_UCallback.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UCallback.OuterSingleton, Z_Construct_UClass_UCallback_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UCallback.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UCallback>()
	{
		return UCallback::StaticClass();
	}
	UCallback::UCallback(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UCallback);
	UCallback::~UCallback() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_callback_callback_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_callback_callback_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UCallback, UCallback::StaticClass, TEXT("UCallback"), &Z_Registration_Info_UClass_UCallback, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UCallback), 2822266854U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_callback_callback_h_2523044477(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_callback_callback_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_callback_callback_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
