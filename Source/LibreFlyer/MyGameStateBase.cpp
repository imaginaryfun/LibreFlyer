// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "MyEngineSubsystem.h"

void AMyGameStateBase::BeginPlay() {
	Super::BeginPlay();
	for (auto& i : GetDefault<ULibreFlyerSettings>()->WorldStartupConsoleCommands) {
		GEngine->Exec(GetWorld(), *i);
	}
}

void AMyGameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	for (auto& i : GetDefault<ULibreFlyerSettings>()->WorldShutdownConsoleCommands) {
		GEngine->Exec(GetWorld(), *i);
	}
	Super::EndPlay(EndPlayReason);
}
