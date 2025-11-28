// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class ComputerVisionPlugin : ModuleRules
{
	public ComputerVisionPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"CoreMath",
				// ... add private dependencies that you statically link with here ...	
				"RenderCore",
				"RHI",
				"Renderer",
				"DebugPlugin",
				"MetalRHI",
				"ImageWrapper",
				"Sockets",
    			"Networking"
				//new
				//"ComputeShaderPlugin"
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);



		PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/TensorFlow/include"));

		PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/TensorFlow/lib/libtensorflow.dylib"));
		
		
		
		//PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/TensorFlow/lib/libtensorflow_framework.dylib"));

		//RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/TensorFlow/lib/libtensorflow.dylib"));
		//RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "../../ThirdParty/TensorFlow/lib/libtensorflow_framework.dylib"));
	}
}
