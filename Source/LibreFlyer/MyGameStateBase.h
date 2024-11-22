// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

class ACheckpoint;
class AQuadcopter;
class AMyPlayerState;

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

	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Checkpoints, EditAnywhere, BlueprintReadWrite)
	TArray<ACheckpoint*> Checkpoints;
	UFUNCTION()
	virtual void OnRep_Checkpoints();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberOfCheckpoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bReceivedCheckpoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AQuadcopter* LocalQuadcopter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMyPlayerState* LocalPlayerState;


	UFUNCTION()
	void OnCheckpointTriggered(UPrimitiveComponent* OverlappedComp,
		AActor* Other, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

};
