// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class MultiplayerMenu : ModuleRules
{
	public MultiplayerMenu(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


        string SteamworksPath = Path.Combine(ModuleDirectory, "../../ThirdParty/Steamworks/");

        // Add the Unreal Engine OnlineSubsystem module
        PublicDependencyModuleNames.AddRange(new string[] { "Core", 
            "CoreUObject", 
            "Engine", 
            "InputCore", 
            "EnhancedInput", 
            "OnlineSubsystem", 
            "OnlineSubsystemSteam", 
            "UMG",
            "Slate",           // ? Needed for UMG widgets
            "SlateCore",
            "MediaAssets",
            "Json",
            "JsonUtilities"
        });

        // If using Steam, add OnlineSubsystemSteam
        PublicDependencyModuleNames.Add("OnlineSubsystemSteam");

        // Add the Steamworks SDK include path (if applicable)
        PublicIncludePaths.Add(Path.Combine(SteamworksPath, "public"));
  

        // Add the Steam API library (if applicable)
        string SteamApiLib = Path.Combine(SteamworksPath, "lib", "steam_api64.lib");

      
        PublicAdditionalLibraries.Add(SteamApiLib);
        

    }
}
