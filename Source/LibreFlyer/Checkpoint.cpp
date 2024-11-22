// Fill out your copyright notice in the Description page of Project Settings.

#include "Checkpoint.h"
#include "MyEngineSubsystem.h"
#include "Components/ArrowComponent.h"

ACheckpoint::ACheckpoint() {
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("CheckpointRoot");

	CheckpointTrigger = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointTrigger");
	CheckpointTrigger->SetupAttachment(RootComponent);
	CheckpointTrigger->SetHiddenInGame(true);
	CheckpointTrigger->SetCollisionProfileName("OverlapAll");

	CheckpointModel = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointModel");
	CheckpointModel->SetupAttachment(RootComponent);

	CheckpointArrow = CreateDefaultSubobject<UArrowComponent>("CheckpointArrow");
	CheckpointArrow->SetupAttachment(CheckpointTrigger);

}
void ACheckpoint::BeginPlay() {
	Super::BeginPlay();	
}
void ACheckpoint::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}
