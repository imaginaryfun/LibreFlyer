// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "MyEngineSubsystem.generated.h"


UCLASS(config = Game, notplaceable)
class LIBREFLYER_API ULibreFlyerSettings : public UDeveloperSettings {
	GENERATED_BODY()
public:
	ULibreFlyerSettings();
	UPROPERTY(config, EditAnywhere)
	TArray<FString> WorldStartupConsoleCommands;
	UPROPERTY(config, EditAnywhere)
	TArray<FString> WorldShutdownConsoleCommands;
	UPROPERTY(config, EditAnywhere)
	TSubclassOf<APawn> DefaultPawnClass;
	UPROPERTY(config, EditAnywhere)
	TSubclassOf<AGameStateBase> GameStateClass;
	UPROPERTY(config, EditAnywhere)
	float QuadcopterMouseSensitivity;
	UPROPERTY(config, EditAnywhere)
	float KeyboardYawRate;
};

UCLASS()
class LIBREFLYER_API UMyEngineSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;	
};
