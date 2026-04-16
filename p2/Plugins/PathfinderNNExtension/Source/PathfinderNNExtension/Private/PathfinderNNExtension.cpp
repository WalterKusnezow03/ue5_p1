// Copyright Epic Games, Inc. All Rights Reserved.

#include "PathfinderNNExtension.h"

#define LOCTEXT_NAMESPACE "FPathfinderNNExtensionModule"

void FPathfinderNNExtensionModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FPathfinderNNExtensionModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPathfinderNNExtensionModule, PathfinderNNExtension)