// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class LIBREFLYER_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	AMyPlayerState();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	int CurrentCheckpointIndex;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};
