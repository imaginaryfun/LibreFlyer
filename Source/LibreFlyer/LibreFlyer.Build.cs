// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class LibreFlyer : ModuleRules
{
	public LibreFlyer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"DeveloperSettings"
        });
		PrivateDependencyModuleNames.AddRange(new string[] {
			"PhysicsCore",
			"UMG",
            "NetCore",
			"Slate",
			"SlateCore"
        });
        PublicIncludePaths.AddRange(new string[] {
			"LibreFlyer\\Game",
            "LibreFlyer\\Interface",
            "LibreFlyer\\Misc",
            "LibreFlyer\\World",
        });		
	}
}
