// Copyright Epic Games, Inc. All Rights Reserved.





using UnrealBuildTool;
using System.IO;

public class PathfinderNNExtension : ModuleRules
{
	public PathfinderNNExtension(ReadOnlyTargetRules Target) : base(Target)
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

		PublicIncludePaths.AddRange(
			new string[] {
                // ... add public include paths required here ...
            }
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"NNCommunicationPlugin",
				"PathFinder",
				"DebugPlugin",
				"CoreMath",
				"StoragePlugin",
				"PolygonPlugin",
				"SharedMemoryPlugin",
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				
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



		// 1. Unreal-Standardabhängigkeiten (KEIN NNE, KEIN ORTCpu!)
		//PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Slate", "SlateCore" });

		// 2. Pfade zur externen Microsoft ONNX Runtime definieren
		//string ThirdPartyPath = Path.Combine(ModuleDirectory, "..", "ThirdParty");
		string PluginRootDir = Path.GetFullPath(Path.Combine(ModuleDirectory, "..", ".."));
		string ThirdPartyPath = Path.Combine(PluginRootDir, "ThirdParty");


		//string ORXIncludePath = Path.Combine(ThirdPartyPath, "ONNXRuntime", "Include");
		string ORXIncludePath = Path.Combine(ThirdPartyPath, "ONNXRuntime", "include");

		//string ORXLibPath = Path.Combine(ThirdPartyPath, "ONNXRuntime", "Lib", "Mac");
		string ORXLibPath = Path.Combine(ThirdPartyPath, "ONNXRuntime", "lib");

		// 3. Header für den Compiler freigeben
		PublicIncludePaths.Add(ORXIncludePath);

		// 4. Bibliothek für macOS verlinken
		if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			//clang: error: no such file or directory: 
			//'/Users/walterk/Documents/UnrealProjects/p2/Plugins/PathfinderNNExtension/Source/ThirdParty/
			// ONNXRuntime/lib/libonnxruntime.dylib'

			//Plugins/PathfinderNNExtension/ThirdParty/ONNXRuntime/lib/libonnxruntime.dylib
			string DylibPath = Path.Combine(ORXLibPath, "libonnxruntime.dylib");

			PublicAdditionalLibraries.Add(DylibPath);
			RuntimeDependencies.Add(DylibPath);
		}


		if (Target.Type == TargetType.Editor)
		{
			// Code wird im Unreal Editor kompiliert
			PublicDefinitions.Add("PLUGIN_GLOBAL_FLAG_USE_PYTHON_NN=1");
		}
		else
		{
			PublicDefinitions.Add("PLUGIN_GLOBAL_FLAG_USE_PYTHON_NN=0");
		}

		//PLUGIN FLAG BEI BUILD
		// Flag direkt im Plugin-Modul definieren
		//PublicDefinitions.Add("PLUGIN_GLOBAL_FLAG_USE_PYTHON_NN=1");

		AddONNXToBuild(ORXLibPath);
	}
	
	void AddONNXToBuild(string ORXLibPath)
	{
		if (Target.Platform == UnrealTargetPlatform.Mac)
		{
			// Wir verweisen auf die Datei, die das System laut Log sucht
			string DylibName = "libonnxruntime.1.dylib"; //die hier. mit der 1. Ja.
			string DylibPath = Path.Combine(ORXLibPath, DylibName);

			// Falls ONNX in deinem Ordner nur als "libonnxruntime.dylib" liegt, 
			// passe den Namen hier an, aber wir MÜSSEN sie als ".1.dylib" in den Build kopieren:
			if (!File.Exists(DylibPath))
			{
				DylibPath = Path.Combine(ORXLibPath, "libonnxruntime.dylib");
			}

			// Linker mitteilen
			PublicAdditionalLibraries.Add(DylibPath);

			// WICHTIG: Streng als "libonnxruntime.1.dylib" in den App-Ordner kopieren
			string TargetDir = Path.Combine("$(BinaryOutputDir)", "libonnxruntime.1.dylib");
			RuntimeDependencies.Add(TargetDir, DylibPath, StagedFileType.NonUFS);
		}

		
	}




}
