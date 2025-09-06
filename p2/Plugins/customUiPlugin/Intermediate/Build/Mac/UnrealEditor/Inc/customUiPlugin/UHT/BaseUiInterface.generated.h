// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "baseInterface/BaseUiInterface.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef CUSTOMUIPLUGIN_BaseUiInterface_generated_h
#error "BaseUiInterface.generated.h already included, missing '#pragma once' in BaseUiInterface.h"
#endif
#define CUSTOMUIPLUGIN_BaseUiInterface_generated_h

#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_SPARSE_DATA
#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_RPC_WRAPPERS
#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_RPC_WRAPPERS_NO_PURE_DECLS
#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_ACCESSORS
#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	CUSTOMUIPLUGIN_API UBaseUiInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBaseUiInterface) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(CUSTOMUIPLUGIN_API, UBaseUiInterface); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBaseUiInterface); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	CUSTOMUIPLUGIN_API UBaseUiInterface(UBaseUiInterface&&); \
	CUSTOMUIPLUGIN_API UBaseUiInterface(const UBaseUiInterface&); \
public: \
	CUSTOMUIPLUGIN_API virtual ~UBaseUiInterface();


#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	CUSTOMUIPLUGIN_API UBaseUiInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	CUSTOMUIPLUGIN_API UBaseUiInterface(UBaseUiInterface&&); \
	CUSTOMUIPLUGIN_API UBaseUiInterface(const UBaseUiInterface&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(CUSTOMUIPLUGIN_API, UBaseUiInterface); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UBaseUiInterface); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UBaseUiInterface) \
	CUSTOMUIPLUGIN_API virtual ~UBaseUiInterface();


#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_GENERATED_UINTERFACE_BODY() \
private: \
	static void StaticRegisterNativesUBaseUiInterface(); \
	friend struct Z_Construct_UClass_UBaseUiInterface_Statics; \
public: \
	DECLARE_CLASS(UBaseUiInterface, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), CASTCLASS_None, TEXT("/Script/customUiPlugin"), CUSTOMUIPLUGIN_API) \
	DECLARE_SERIALIZER(UBaseUiInterface)


#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_GENERATED_BODY_LEGACY \
		PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_GENERATED_UINTERFACE_BODY() \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_STANDARD_CONSTRUCTORS \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_GENERATED_UINTERFACE_BODY() \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~IBaseUiInterface() {} \
public: \
	typedef UBaseUiInterface UClassType; \
	typedef IBaseUiInterface ThisClass; \
	virtual UObject* _getUObject() const { return nullptr; }


#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_INCLASS_IINTERFACE \
protected: \
	virtual ~IBaseUiInterface() {} \
public: \
	typedef UBaseUiInterface UClassType; \
	typedef IBaseUiInterface ThisClass; \
	virtual UObject* _getUObject() const { return nullptr; }


#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_10_PROLOG
#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_22_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_SPARSE_DATA \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_RPC_WRAPPERS \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_ACCESSORS \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_INCLASS_IINTERFACE \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_22_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_SPARSE_DATA \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_ACCESSORS \
	FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h_13_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> CUSTOMUIPLUGIN_API UClass* StaticClass<class UBaseUiInterface>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_customUiPlugin_Source_customUiPlugin_baseInterface_BaseUiInterface_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
