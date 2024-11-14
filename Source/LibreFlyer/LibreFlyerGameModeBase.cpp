// Fill out your copyright notice in the Description page of Project Settings.


#include "LibreFlyerGameModeBase.h"
#include "MyGameStateBase.h"
#include "MyEngineSubsystem.h"

ALibreFlyerGameModeBase::ALibreFlyerGameModeBase() {
	DefaultPawnClass = GetDefault<ULibreFlyerSettings>()->DefaultPawnClass;
	GameStateClass = GetDefault<ULibreFlyerSettings>()->GameStateClass;
}
