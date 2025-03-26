// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EternalGrace_Proto : ModuleRules
{
	public EternalGrace_Proto(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "HairStrandsCore", "Niagara" });
	}
}
