// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LibreFlyerStructs.h"

#include "MyGameStateBase.generated.h"

class ACheckpoint;
class AQuadcopter;
class AMyPlayerState;
//class UHudWidget;
class SSlateHud;

UCLASS()
class LIBREFLYER_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
public:
	AMyGameStateBase();
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void PostBeginPlay();

	UFUNCTION(Server, Reliable)
	void ServerPostBeginPlay();
	void TryReady();
	void ForceReady();

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Checkpoints, EditAnywhere, BlueprintReadWrite)
	TArray<ACheckpoint*> Checkpoints;
	UFUNCTION()
	virtual void OnRep_Checkpoints();

	void OnCheckpointTriggered(const FCheckpointTriggeredEvent& CheckpointEvent);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberOfCheckpoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReceivedCheckpoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReceivedStartingBlocks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFinishedBeginPlay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AQuadcopter* LocalQuadcopter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMyPlayerState* LocalPlayerState;
	TSharedPtr<SSlateHud> LocalHud;

	UPROPERTY(ReplicatedUsing = OnRep_StartingBlocks, Replicated, EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> StartingBlocks;
	UFUNCTION()
	virtual void OnRep_StartingBlocks();
};
