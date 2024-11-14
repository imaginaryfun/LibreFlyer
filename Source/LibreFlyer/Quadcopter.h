// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* QuadcopterRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCameraComponent* QuadcopterCamera;

	UFUNCTION(BlueprintCallable)
	void Throttle(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void MouseRoll(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void MousePitch(float AxisInput);
	UFUNCTION(BlueprintCallable)
	void KeyboardYaw(float AxisInput);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrottleInput;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ThrottleForce;
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

	UPROPERTY(EditAnywhere)
	float KeyboardYawRate;

private:
	UPROPERTY(EditAnywhere)
	float MouseSensitivity;

};
