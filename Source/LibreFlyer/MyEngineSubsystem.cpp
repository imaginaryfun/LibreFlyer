// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEngineSubsystem.h"
#include "Blueprint/UserWidget.h"


ULibreFlyerSettings::ULibreFlyerSettings() {
	PlayInEditorStartupConsoleCommands = TArray<FString>();
	PlayInEditorShutdownConsoleCommands = TArray<FString>();
	PlayInEditorMaxFramerate = 0.f;
	EditorMaxFramerate = 0.f;
	bShowPlayInEditorFramerate = false;
	PlayInEditorCheckpointSound = nullptr;
	// todo - dont load pawn or userwidget when this is constructed. Check to see if
	// developer has set this variable (in PostInitProperties()), then load them. Might have to use TSoftClassPtr because
	// I couldnt find a way to check if developer set TSubclassOf to None in the config, 
	// so im just loading by default for now
	PlayInEditorPawnClass = LoadClass<APawn>(
		nullptr,
		L"BlueprintGeneratedClass'/Game/QuadcopterBP.QuadcopterBP_C'"
	);
	PlayInEditorUserWidget = LoadClass<UUserWidget>(
		nullptr,
		L"WidgetBlueprint'/Game/HudWidget.HudWidget'"
	);
	bPlayInEditorSupressGameHud = false;
}
#if WITH_EDITOR
void ULibreFlyerSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif


void UMyEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
}

void UMyEngineSubsystem::Deinitialize() {
}

