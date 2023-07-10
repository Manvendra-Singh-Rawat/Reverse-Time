// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Self_ReverseTime : ModuleRules
{
	public Self_ReverseTime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
