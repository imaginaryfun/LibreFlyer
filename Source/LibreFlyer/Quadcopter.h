// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BetaflightStuff.h"
#include "Quadcopter.generated.h"

class UCameraComponent;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* QuadcopterCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* QuadcopterModel;

	UFUNCTION(BlueprintCallable)
	void Throttle(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void MouseRoll(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void MousePitch(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void MouseYaw(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void KeyboardRoll(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void KeyboardPitch(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void KeyboardYaw(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void StickRoll(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void StickPitch(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void StickYaw(float AxisInput);

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
};
