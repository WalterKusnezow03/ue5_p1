// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "customUiPlugin/ui/Widgets/textBox/TextBoxBase.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeTextBoxBase() {}
// Cross Module References
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UCallback_NoRegister();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UcustomUiComponentBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UTextBoxBase();
	CUSTOMUIPLUGIN_API UClass* Z_Construct_UClass_UTextBoxBase_NoRegister();
	SLATECORE_API UEnum* Z_Construct_UEnum_SlateCore_ETextCommit();
	UMG_API UClass* Z_Construct_UClass_UEditableTextBox_NoRegister();
	UMG_API UClass* Z_Construct_UClass_USizeBox_NoRegister();
	UPackage* Z_Construct_UPackage__Script_customUiPlugin();
// End Cross Module References
	DEFINE_FUNCTION(UTextBoxBase::execOnTextCommittedHandler)
	{
		P_GET_PROPERTY_REF(FTextProperty,Z_Param_Out_Text);
		P_GET_PROPERTY(FByteProperty,Z_Param_CommitMethod);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->OnTextCommittedHandler(Z_Param_Out_Text,ETextCommit::Type(Z_Param_CommitMethod));
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(UTextBoxBase::execOnTextChangedHandler)
	{
		P_GET_PROPERTY_REF(FTextProperty,Z_Param_Out_Text);
		P_FINISH;
		P_NATIVE_BEGIN;
		P_THIS->OnTextChangedHandler(Z_Param_Out_Text);
		P_NATIVE_END;
	}
	void UTextBoxBase::StaticRegisterNativesUTextBoxBase()
	{
		UClass* Class = UTextBoxBase::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "OnTextChangedHandler", &UTextBoxBase::execOnTextChangedHandler },
			{ "OnTextCommittedHandler", &UTextBoxBase::execOnTextCommittedHandler },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics
	{
		struct TextBoxBase_eventOnTextChangedHandler_Parms
		{
			FText Text;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Text_MetaData[];
#endif
		static const UECodeGen_Private::FTextPropertyParams NewProp_Text;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::NewProp_Text_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::NewProp_Text = { "Text", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(TextBoxBase_eventOnTextChangedHandler_Parms, Text), METADATA_PARAMS(Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::NewProp_Text_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::NewProp_Text_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::NewProp_Text,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "ui/Widgets/textBox/TextBoxBase.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTextBoxBase, nullptr, "OnTextChangedHandler", nullptr, nullptr, sizeof(Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::TextBoxBase_eventOnTextChangedHandler_Parms), Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00440401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics
	{
		struct TextBoxBase_eventOnTextCommittedHandler_Parms
		{
			FText Text;
			TEnumAsByte<ETextCommit::Type> CommitMethod;
		};
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_Text_MetaData[];
#endif
		static const UECodeGen_Private::FTextPropertyParams NewProp_Text;
		static const UECodeGen_Private::FBytePropertyParams NewProp_CommitMethod;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::NewProp_Text_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UECodeGen_Private::FTextPropertyParams Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::NewProp_Text = { "Text", nullptr, (EPropertyFlags)0x0010000008000182, UECodeGen_Private::EPropertyGenFlags::Text, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(TextBoxBase_eventOnTextCommittedHandler_Parms, Text), METADATA_PARAMS(Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::NewProp_Text_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::NewProp_Text_MetaData)) };
	const UECodeGen_Private::FBytePropertyParams Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::NewProp_CommitMethod = { "CommitMethod", nullptr, (EPropertyFlags)0x0010000000000080, UECodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(TextBoxBase_eventOnTextCommittedHandler_Parms, CommitMethod), Z_Construct_UEnum_SlateCore_ETextCommit, METADATA_PARAMS(nullptr, 0) }; // 1116915171
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::NewProp_Text,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::NewProp_CommitMethod,
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "ui/Widgets/textBox/TextBoxBase.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UTextBoxBase, nullptr, "OnTextCommittedHandler", nullptr, nullptr, sizeof(Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::TextBoxBase_eventOnTextCommittedHandler_Parms), Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00440401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UTextBoxBase);
	UClass* Z_Construct_UClass_UTextBoxBase_NoRegister()
	{
		return UTextBoxBase::StaticClass();
	}
	struct Z_Construct_UClass_UTextBoxBase_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_callbackPointer_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_callbackPointer;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_callbackPointerTextChanged_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_callbackPointerTextChanged;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_TextBox_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_TextBox;
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam NewProp_sizeBox_MetaData[];
#endif
		static const UECodeGen_Private::FObjectPropertyParams NewProp_sizeBox;
		static const UECodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UTextBoxBase_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UcustomUiComponentBase,
		(UObject* (*)())Z_Construct_UPackage__Script_customUiPlugin,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UTextBoxBase_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UTextBoxBase_OnTextChangedHandler, "OnTextChangedHandler" }, // 2592705321
		{ &Z_Construct_UFunction_UTextBoxBase_OnTextCommittedHandler, "OnTextCommittedHandler" }, // 2244888214
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTextBoxBase_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/// @brief is a textbox abstracktion, saves the current string typed.\n" },
		{ "IncludePath", "ui/Widgets/textBox/TextBoxBase.h" },
		{ "ModuleRelativePath", "ui/Widgets/textBox/TextBoxBase.h" },
		{ "ToolTip", "@brief is a textbox abstracktion, saves the current string typed." },
	};
#endif
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTextBoxBase_Statics::NewProp_callbackPointer_MetaData[] = {
		{ "Comment", "//callback on click\n" },
		{ "ModuleRelativePath", "ui/Widgets/textBox/TextBoxBase.h" },
		{ "ToolTip", "callback on click" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UTextBoxBase_Statics::NewProp_callbackPointer = { "callbackPointer", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UTextBoxBase, callbackPointer), Z_Construct_UClass_UCallback_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UTextBoxBase_Statics::NewProp_callbackPointer_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTextBoxBase_Statics::NewProp_callbackPointer_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTextBoxBase_Statics::NewProp_callbackPointerTextChanged_MetaData[] = {
		{ "ModuleRelativePath", "ui/Widgets/textBox/TextBoxBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UTextBoxBase_Statics::NewProp_callbackPointerTextChanged = { "callbackPointerTextChanged", nullptr, (EPropertyFlags)0x0040000000000000, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UTextBoxBase, callbackPointerTextChanged), Z_Construct_UClass_UCallback_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UTextBoxBase_Statics::NewProp_callbackPointerTextChanged_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTextBoxBase_Statics::NewProp_callbackPointerTextChanged_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTextBoxBase_Statics::NewProp_TextBox_MetaData[] = {
		{ "Comment", "//ui components\n" },
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/textBox/TextBoxBase.h" },
		{ "ToolTip", "ui components" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UTextBoxBase_Statics::NewProp_TextBox = { "TextBox", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UTextBoxBase, TextBox), Z_Construct_UClass_UEditableTextBox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UTextBoxBase_Statics::NewProp_TextBox_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTextBoxBase_Statics::NewProp_TextBox_MetaData)) };
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UTextBoxBase_Statics::NewProp_sizeBox_MetaData[] = {
		{ "EditInline", "true" },
		{ "ModuleRelativePath", "ui/Widgets/textBox/TextBoxBase.h" },
	};
#endif
	const UECodeGen_Private::FObjectPropertyParams Z_Construct_UClass_UTextBoxBase_Statics::NewProp_sizeBox = { "sizeBox", nullptr, (EPropertyFlags)0x0040000000080008, UECodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, nullptr, nullptr, STRUCT_OFFSET(UTextBoxBase, sizeBox), Z_Construct_UClass_USizeBox_NoRegister, METADATA_PARAMS(Z_Construct_UClass_UTextBoxBase_Statics::NewProp_sizeBox_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_UTextBoxBase_Statics::NewProp_sizeBox_MetaData)) };
	const UECodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_UTextBoxBase_Statics::PropPointers[] = {
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTextBoxBase_Statics::NewProp_callbackPointer,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTextBoxBase_Statics::NewProp_callbackPointerTextChanged,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTextBoxBase_Statics::NewProp_TextBox,
		(const UECodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_UTextBoxBase_Statics::NewProp_sizeBox,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_UTextBoxBase_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UTextBoxBase>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UTextBoxBase_Statics::ClassParams = {
		&UTextBoxBase::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		Z_Construct_UClass_UTextBoxBase_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		UE_ARRAY_COUNT(Z_Construct_UClass_UTextBoxBase_Statics::PropPointers),
		0,
		0x009000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UTextBoxBase_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UTextBoxBase_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UTextBoxBase()
	{
		if (!Z_Registration_Info_UClass_UTextBoxBase.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UTextBoxBase.OuterSingleton, Z_Construct_UClass_UTextBoxBase_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UTextBoxBase.OuterSingleton;
	}
	template<> CUSTOMUIPLUGIN_API UClass* StaticClass<UTextBoxBase>()
	{
		return UTextBoxBase::StaticClass();
	}
	UTextBoxBase::UTextBoxBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UTextBoxBase);
	UTextBoxBase::~UTextBoxBase() {}
	struct Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_textBox_TextBoxBase_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_textBox_TextBoxBase_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UTextBoxBase, UTextBoxBase::StaticClass, TEXT("UTextBoxBase"), &Z_Registration_Info_UClass_UTextBoxBase, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UTextBoxBase), 3223892433U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_textBox_TextBoxBase_h_192874536(TEXT("/Script/customUiPlugin"),
		Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_textBox_TextBoxBase_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_ui_Widgets_textBox_TextBoxBase_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
