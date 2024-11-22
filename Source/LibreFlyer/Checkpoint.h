// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

class UArrowComponent;

UCLASS()
class LIBREFLYER_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CheckpointTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CheckpointModel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* CheckpointArrow;
};
