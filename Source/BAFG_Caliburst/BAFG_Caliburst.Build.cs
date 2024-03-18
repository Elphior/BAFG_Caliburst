// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BAFG_Caliburst : ModuleRules
{
	public BAFG_Caliburst(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
