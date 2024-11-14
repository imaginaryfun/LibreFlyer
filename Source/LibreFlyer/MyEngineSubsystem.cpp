// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEngineSubsystem.h"
#include "MyGameStateBase.h"
ULibreFlyerSettings::ULibreFlyerSettings() {
	QuadcopterMouseSensitivity = 8.f;
	KeyboardYawRate = 1.f;
	DefaultPawnClass = LoadClass<APawn>(
		nullptr,
		L"BlueprintGeneratedClass'/Game/QuadcopterBP.QuadcopterBP_C''"
	);
	GameStateClass = AGameStateBase::StaticClass();
}
void UMyEngineSubsystem::Initialize(FSubsystemCollectionBase& Collection) {

}

void UMyEngineSubsystem::Deinitialize() {
}

