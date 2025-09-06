// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

// IWYU pragma: private, include "Collection/interface/CollectableInterface.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef GCGAMECORE_CollectableInterface_generated_h
#error "CollectableInterface.generated.h already included, missing '#pragma once' in CollectableInterface.h"
#endif
#define GCGAMECORE_CollectableInterface_generated_h

#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_SPARSE_DATA
#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_RPC_WRAPPERS
#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_RPC_WRAPPERS_NO_PURE_DECLS
#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_ACCESSORS
#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	GCGAMECORE_API UCollectableInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UCollectableInterface) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(GCGAMECORE_API, UCollectableInterface); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCollectableInterface); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	GCGAMECORE_API UCollectableInterface(UCollectableInterface&&); \
	GCGAMECORE_API UCollectableInterface(const UCollectableInterface&); \
public: \
	GCGAMECORE_API virtual ~UCollectableInterface();


#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	GCGAMECORE_API UCollectableInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	GCGAMECORE_API UCollectableInterface(UCollectableInterface&&); \
	GCGAMECORE_API UCollectableInterface(const UCollectableInterface&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(GCGAMECORE_API, UCollectableInterface); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UCollectableInterface); \
	DEFINE_ABSTRACT_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UCollectableInterface) \
	GCGAMECORE_API virtual ~UCollectableInterface();


#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_GENERATED_UINTERFACE_BODY() \
private: \
	static void StaticRegisterNativesUCollectableInterface(); \
	friend struct Z_Construct_UClass_UCollectableInterface_Statics; \
public: \
	DECLARE_CLASS(UCollectableInterface, UInterface, COMPILED_IN_FLAGS(CLASS_Abstract | CLASS_Interface), CASTCLASS_None, TEXT("/Script/GcGameCore"), GCGAMECORE_API) \
	DECLARE_SERIALIZER(UCollectableInterface)


#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_GENERATED_BODY_LEGACY \
		PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_GENERATED_UINTERFACE_BODY() \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_STANDARD_CONSTRUCTORS \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_GENERATED_BODY \
	PRAGMA_DISABLE_DEPRECATION_WARNINGS \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_GENERATED_UINTERFACE_BODY() \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_ENHANCED_CONSTRUCTORS \
private: \
	PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_INCLASS_IINTERFACE_NO_PURE_DECLS \
protected: \
	virtual ~ICollectableInterface() {} \
public: \
	typedef UCollectableInterface UClassType; \
	typedef ICollectableInterface ThisClass; \
	virtual UObject* _getUObject() const { return nullptr; }


#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_INCLASS_IINTERFACE \
protected: \
	virtual ~ICollectableInterface() {} \
public: \
	typedef UCollectableInterface UClassType; \
	typedef ICollectableInterface ThisClass; \
	virtual UObject* _getUObject() const { return nullptr; }


#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_13_PROLOG
#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_25_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_SPARSE_DATA \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_RPC_WRAPPERS \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_ACCESSORS \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_INCLASS_IINTERFACE \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_25_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_SPARSE_DATA \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_RPC_WRAPPERS_NO_PURE_DECLS \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_ACCESSORS \
	FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h_16_INCLASS_IINTERFACE_NO_PURE_DECLS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GCGAMECORE_API UClass* StaticClass<class UCollectableInterface>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID FID_UnrealProjects_p2_Plugins_GcGameCore_Source_GcGameCore_Collection_interface_CollectableInterface_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
