// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/PlayerUiBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodePlayerUiBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UPlayerUiBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UPlayerUiBase_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UUserWidget();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	DEFINE_FUNCTION(UPlayerUiBase::execopenGameScreen)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->openGameScreen();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UPlayerUiBase::execcloseLatestScreen)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->closeLatestScreen();
		P_NATIVE_END;
	}
	void UPlayerUiBase::StaticRegisterNativesUPlayerUiBase()
	{
		UClass* Class = UPlayerUiBase::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "closeLatestScreen", &UPlayerUiBase::execcloseLatestScreen },
			{ "openGameScreen", &UPlayerUiBase::execopenGameScreen },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UPlayerUiBase_closeLatestScreen_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPlayerUiBase_closeLatestScreen_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "ui/PlayerUiBase.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UPlayerUiBase_closeLatestScreen_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UPlayerUiBase, nullptr, "closeLatestScreen", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UPlayerUiBase_closeLatestScreen_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UPlayerUiBase_closeLatestScreen_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UPlayerUiBase_closeLatestScreen()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UPlayerUiBase_closeLatestScreen_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UPlayerUiBase_openGameScreen_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UPlayerUiBase_openGameScreen_Statics::Function_MetaDataParams[] = {
		{ "Comment", "/// @brief designed to be overriden to hide the ui / show player HUD\n" },
		{ "ModuleRelativePath", "ui/PlayerUiBase.h" },
		{ "ToolTip", "@brief designed to be overriden to hide the ui / show player HUD" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UPlayerUiBase_openGameScreen_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UPlayerUiBase, nullptr, "openGameScreen", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00020400, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UPlayerUiBase_openGameScreen_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UPlayerUiBase_openGameScreen_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UPlayerUiBase_openGameScreen()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UPlayerUiBase_openGameScreen_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UPlayerUiBase);
	UClass* Z_Construct_UClass_UPlayerUiBase_NoRegister()
	{
		return UPlayerUiBase::StaticClass();
	}
	struct Z_Construct_UClass_UPlayerUiBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UPlayerUiBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UUserWidget,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UPlayerUiBase_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UPlayerUiBase_closeLatestScreen, "closeLatestScreen" }, // 1772344821
		{ &Z_Construct_UFunction_UPlayerUiBase_openGameScreen, "openGameScreen" }, // 817857396
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UPlayerUiBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * will be the base class for any ui, based on a blueprint with a single canvas inside,\n * because i couldnt create it from c++ :)\n * \n * Will be the only owner of ALL ui screens, \n * all classes, which you can find here by value, which provide panels, etc.\n * need an instance of THIS class, to bind all ui elements to this\n * class for proper unreal garbage collection\n * \n * \n * Derivative hierachy\n * \n * \x09UObject\n\x09\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80 UVisual (abstrakte Klasse, Basisklasse f\xc3\xbcr Widgets)\n\x09\x09\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80 UWidget (Basisklasse aller UMG-Widgets)\n\x09\x09\x09\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UPanelWidget (Basisklasse f\xc3\xbcr Container-Widgets)\n\x09\x09\x09\xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UCanvasPanel\n\x09\x09\x09\xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UGridPanel\n\x09\x09\x09\xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UHorizontalBox\n\x09\x09\x09\xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UVerticalBox\n\x09\x09\x09\xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UOverlay\n\x09\x09\x09\xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UScrollBox\n\x09\x09\x09\xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UWrapBox\n\x09\x09\x09\xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 USizeBox\n\x09\x09\x09\xe2\x94\x82   \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80 ...\n\x09\x09\x09\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UButton\n\x09\x09\x09\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UTextBlock\n\x09\x09\x09\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UImage\n\x09\x09\x09\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UProgressBar\n\x09\x09\x09\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 USlider\n\x09\x09\x09\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UCheckBox\n\x09\x09\x09\xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UUserWidget (f\xc3\xbcr komplexe UI-Logik)\n\x09\x09\x09\xe2\x94\x94\xe2\x94\x80\xe2\x94\x80 ...\n * \n * \n * \n */" },
		{ "IncludePath", "ui/PlayerUiBase.h" },
		{ "ModuleRelativePath", "ui/PlayerUiBase.h" },
		{ "ToolTip", "will be the base class for any ui, based on a blueprint with a single canvas inside,\nbecause i couldnt create it from c++ :)\n\nWill be the only owner of ALL ui screens,\nall classes, which you can find here by value, which provide panels, etc.\nneed an instance of THIS class, to bind all ui elements to this\nclass for proper unreal garbage collection\n\n\nDerivative hierachy\n\n    UObject\n       \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80 UVisual (abstrakte Klasse, Basisklasse f\xc3\xbcr Widgets)\n               \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80 UWidget (Basisklasse aller UMG-Widgets)\n                       \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UPanelWidget (Basisklasse f\xc3\xbcr Container-Widgets)\n                       \xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UCanvasPanel\n                       \xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UGridPanel\n                       \xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UHorizontalBox\n                       \xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UVerticalBox\n                       \xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UOverlay\n                       \xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UScrollBox\n                       \xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UWrapBox\n                       \xe2\x94\x82   \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 USizeBox\n                       \xe2\x94\x82   \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80 ...\n                       \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UButton\n                       \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UTextBlock\n                       \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UImage\n                       \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UProgressBar\n                       \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 USlider\n                       \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UCheckBox\n                       \xe2\x94\x9c\xe2\x94\x80\xe2\x94\x80 UUserWidget (f\xc3\xbcr komplexe UI-Logik)\n                       \xe2\x94\x94\xe2\x94\x80\xe2\x94\x80 ..." },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UPlayerUiBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UPlayerUiBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UPlayerUiBase_Statics::ClassParams = {
		&UPlayerUiBase::StaticClass,
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
		0x00B010A0u,
		METADATA_PARAMS(Z_Construct_UClass_UPlayerUiBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UPlayerUiBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UPlayerUiBase()
	{
		if (!Z_Registration_Info_UClass_UPlayerUiBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UPlayerUiBase.OuterSingleton, Z_Construct_UClass_UPlayerUiBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UPlayerUiBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UPlayerUiBase>()
	{
		return UPlayerUiBase::StaticClass();
	}
	UPlayerUiBase::UPlayerUiBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UPlayerUiBase);
	UPlayerUiBase::~UPlayerUiBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_PlayerUiBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_PlayerUiBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UPlayerUiBase, UPlayerUiBase::StaticClass, TEXT("UPlayerUiBase"), &Z_Registration_Info_UClass_UPlayerUiBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UPlayerUiBase), 2763717817U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_PlayerUiBase_h_3921982064(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_PlayerUiBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_PlayerUiBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
