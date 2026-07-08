// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
public class NNCommunicationPlugin : ModuleRules
{
	





	public NNCommunicationPlugin(ReadOnlyTargetRules Target) : base(Target)
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
				"DebugPlugin",
				"PythonPlugin",
				"SharedMemoryPlugin",
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
				"Sockets",
    			"Networking"
				// ... add private dependencies that you statically link with here ...	
			}
			);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
		

		if (Target.Type != TargetType.Editor)
		{
			string PythonSourceDir = Path.Combine(PluginDirectory, "Python");

			if (Directory.Exists(PythonSourceDir))
			{
				// Wir holen uns jede einzelne Datei aus dem Python- & venv-Ordner.
				// Das umgeht den Bug mit der ".../" Wildcard unter macOS vollständig!
				string[] AllFiles = Directory.GetFiles(PythonSourceDir, "*.*", SearchOption.AllDirectories);
				
				foreach (string FilePath in AllFiles)
				{
					// Jede Datei wird einzeln als Runtime-Abhängigkeit hinterlegt.
					// Unreal kopiert sie dadurch exakt an die relative Position im App-Bundle.
					RuntimeDependencies.Add(FilePath);
				}
			}
		}
	}
}
