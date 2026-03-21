// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class p2 : ModuleRules
{
	public p2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore",

			//new
			"Slate", "SlateCore",

			//debug: plucker bonechains
			"PlueckerCore",

			//game plugins,
			"terrainPlugin",
			"CoreMath",
			"GameCore",
			"PathFinder",
			"AssetPlugin",
			"IkHumanoidModell",
			"customuipluginbase",
			"customUiPlugin",
			"StoragePlugin",
			"GcGameCore",
			"CustomMaterialPlugin",
			"AssetMakerEditorPlugin",
			"NNCommunicationPlugin",
			"DebugPlugin",
			"ComputerVisionPlugin",
			"AssetEnumCollection",
			"AeroDynamicPlugin",
			"MeshDataPlugin",
			"AnyMeshWidgetPlugin",
		});

		PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent" });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
