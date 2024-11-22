// Fill out your copyright notice in the Description page of Project Settings.


#include "LibreFlyerGameModeBase.h"
#include "MyGameStateBase.h"
#include "MyEngineSubsystem.h"
#include "MyPlayerState.h"

ALibreFlyerGameModeBase::ALibreFlyerGameModeBase() {
	if (GetDefault<ULibreFlyerSettings>()->PlayInEditorPawnClass) {
		DefaultPawnClass = GetDefault<ULibreFlyerSettings>()->PlayInEditorPawnClass;
	}
	GameStateClass = AMyGameStateBase::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
}
