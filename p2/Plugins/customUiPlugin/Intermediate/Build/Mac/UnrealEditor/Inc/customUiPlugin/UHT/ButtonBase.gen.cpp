// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/buttons/ButtonBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeButtonBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UButtonBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UButtonBase_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UCallback_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	UMG_API UClass* Z_Construct_UClass_UButton_NoRegister();
	UMG_API UClass* Z_Construct_UClass_UScaleBox_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	void UButtonBase::StaticRegisterNativesUButtonBase()
	{
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UButtonBase);
	UClass* Z_Construct_UClass_UButtonBase_NoRegister()
	{
		return UButtonBase::StaticClass();
	}
	struct Z_Construct_UClass_UButtonBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_button_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_button;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_scalebox_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_scalebox;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_callbackPointer_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_callbackPointer;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_callbackPointerOnHovered_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_callbackPointerOnHovered;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_callbackPointerOnUnHovered_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_callbackPointerOnUnHovered;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UButtonBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UcustomUiComponentBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UButtonBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * button with scaled content\n * \n * button (base)\n * -->scale box (scales content to parent)\n * --->content inside scale box\n * \n */" },
		{ "IncludePath", "ui/Widgets/buttons/ButtonBase.h" },
		{ "ModuleRelativePath", "ui/Widgets/buttons/ButtonBase.h" },
		{ "ToolTip", "button with scaled content\n\nbutton (base)\n-->scale box (scales content to parent)\n--->content inside scale box" },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UButtonBase_Statics::NewProp_button_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/buttons/ButtonBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UButtonBase_Statics::NewProp_button = { "button", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UButtonBase, button), Z_Construct_UClass_UButton_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UButtonBase_Statics::NewProp_button_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UButtonBase_Statics::NewProp_button_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UButtonBase_Statics::NewProp_scalebox_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/buttons/ButtonBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UButtonBase_Statics::NewProp_scalebox = { "scalebox", nullptr, (EPropertyFlags)0x0020080000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UButtonBase, scalebox), Z_Construct_UClass_UScaleBox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UButtonBase_Statics::NewProp_scalebox_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UButtonBase_Statics::NewProp_scalebox_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointer_MetaData[] = {
		{ "Comment", "//inside button. Use to add your childs, macht button sichtbar\n" },
		{ "ModuleRelativePath", "ui/Widgets/buttons/ButtonBase.h" },
		{ "ToolTip", "inside button. Use to add your childs, macht button sichtbar" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointer = { "callbackPointer", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UButtonBase, callbackPointer), Z_Construct_UClass_UCallback_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointer_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointer_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointerOnHovered_MetaData[] = {
		{ "Comment", "//callback on click\n" },
		{ "ModuleRelativePath", "ui/Widgets/buttons/ButtonBase.h" },
		{ "ToolTip", "callback on click" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointerOnHovered = { "callbackPointerOnHovered", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UButtonBase, callbackPointerOnHovered), Z_Construct_UClass_UCallback_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointerOnHovered_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointerOnHovered_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointerOnUnHovered_MetaData[] = {
		{ "ModuleRelativePath", "ui/Widgets/buttons/ButtonBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointerOnUnHovered = { "callbackPointerOnUnHovered", nullptr, (EPropertyFlags)0x0020080000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UButtonBase, callbackPointerOnUnHovered), Z_Construct_UClass_UCallback_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointerOnUnHovered_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointerOnUnHovered_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UButtonBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UButtonBase_Statics::NewProp_button,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UButtonBase_Statics::NewProp_scalebox,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointer,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointerOnHovered,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UButtonBase_Statics::NewProp_callbackPointerOnUnHovered,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UButtonBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UButtonBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UButtonBase_Statics::ClassParams = {
		&UButtonBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_UButtonBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_UButtonBase_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UButtonBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UButtonBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UButtonBase()
	{
		if (!Z_Registration_Info_UClass_UButtonBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UButtonBase.OuterSingleton, Z_Construct_UClass_UButtonBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UButtonBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UButtonBase>()
	{
		return UButtonBase::StaticClass();
	}
	UButtonBase::UButtonBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UButtonBase);
	UButtonBase::~UButtonBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_ButtonBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_ButtonBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UButtonBase, UButtonBase::StaticClass, TEXT("UButtonBase"), &Z_Registration_Info_UClass_UButtonBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UButtonBase), 2238872622U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_ButtonBase_h_1641438082(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_ButtonBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_buttons_ButtonBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
