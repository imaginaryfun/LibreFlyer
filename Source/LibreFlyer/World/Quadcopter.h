// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BetaflightStuff.h"
#include "LibreFlyerStructs.h"
#include "Quadcopter.generated.h"

class UCameraComponent;
class UBoxComponent;
class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCheckpointTriggered, const FCheckpointTriggeredEvent&, CheckpointEvent);


UCLASS()
class LIBREFLYER_API AQuadcopter : public APawn
{
	GENERATED_BODY()

public:
	AQuadcopter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* QuadcopterRoot;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//UBoxComponent* QuadcopterRoot;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//USphereComponent* QuadcopterRoot;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* QuadcopterCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* QuadcopterModel;	


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrottleInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasThrottleInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator AbsoluteRotationInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RelativeRotationInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasAbsoluteRotationInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasRelativeRotationInput;

	UFUNCTION(BlueprintCallable)
	void SetMouseSensitivity(float NewSensitivity);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config)
	float ThrottleForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config)
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config)
	float FlyingMouseSensitivity;
	/*
		0 = Roll
		1 = Pitch
		2 = Yaw
		You can delete the second and third entry if you want your rates applied to all axes
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config)
	TArray<float> KeyboardRates;
	/*
		Betaflight rates for each rotation axis
		0 = Roll
		1 = Pitch
		2 = Yaw
		You can delete the second and third entry if you want your rates applied to all axes
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config)
	FBetaflightRates BetaflightRates;


	FVector SpawnLocation;
	FRotator SpawnRotation;
	void Respawn();

	UPROPERTY(BlueprintAssignable)
	FCheckpointTriggered CheckpointTriggered;

	FCollisionQueryParams HitParameters;

	FVector PreviousLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrusterDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrusterOffset;

};
