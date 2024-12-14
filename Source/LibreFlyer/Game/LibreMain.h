// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"

#include "LibreMain.generated.h"

UCLASS(config = LibreFlyer, notplaceable, defaultconfig)
class LIBREFLYER_API ULibreFlyerEditorSettings : public UDeveloperSettings {
	GENERATED_BODY()
public:
	ULibreFlyerEditorSettings();
	// These are console commands that you want to be executed when you press Play in editor
	UPROPERTY(config, EditAnywhere, Category = "PlayInEditor")
	TArray<FString> PlayInEditorStartupConsoleCommands;
	// These are console commands that you want to be executed when Play-In-Editor is shutdown
	UPROPERTY(config, EditAnywhere, Category = "PlayInEditor")
	TArray<FString> PlayInEditorShutdownConsoleCommands;
	UPROPERTY(config, EditAnywhere, Category = "PlayInEditor")
	TSubclassOf<APawn> PlayInEditorPawnClass;
	UPROPERTY(config, EditAnywhere, Category = "PlayInEditor")
	float PlayInEditorMaxFramerate;
	UPROPERTY(config, EditAnywhere, Category = "PlayInEditor")
	bool bShowPlayInEditorFramerate;
	UPROPERTY(config, EditAnywhere, Category = "PlayInEditor")
	TSubclassOf<UUserWidget> PlayInEditorUserWidget;
	UPROPERTY(config, EditAnywhere, Category = "PlayInEditor")
	bool bPlayInEditorSupressGameHud;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayInEditor")
	USoundBase* PlayInEditorCheckpointSound;
	UPROPERTY(config, EditAnywhere, Category = "Editor")
	float EditorMaxFramerate;

	UPROPERTY(config, EditAnywhere, Category = "Project")
	TSubclassOf<UUserWidget> HudWidget;


	//UPROPERTY(config, EditAnywhere, Category = "QuadcopterConfiguration")
	//FQuadcopterConfiguration QuadcopterConfiguration;
	//UPROPERTY(config, EditAnywhere, Category = "SimulationConfiguration")
	//FSimulationConfiguration SimulationConfiguration;


	virtual FName GetCategoryName() const override { return TEXT("Project"); };

};

UCLASS()
class LIBREFLYER_API ULibreMain : public UEngineSubsystem
{
	GENERATED_BODY()
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	void LoadEditorSettings();
	void LoadProjectSettings();
};
