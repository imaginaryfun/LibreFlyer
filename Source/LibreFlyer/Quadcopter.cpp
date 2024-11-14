// Fill out your copyright notice in the Description page of Project Settings.


#include "Quadcopter.h"
#include "Camera/CameraComponent.h"
#include "MyEngineSubsystem.h"

AQuadcopter::AQuadcopter()
{
	PrimaryActorTick.bCanEverTick = true;

	QuadcopterRoot = CreateDefaultSubobject<UStaticMeshComponent>("QuadcopterRoot");
	QuadcopterRoot->SetSimulatePhysics(true);
	QuadcopterRoot->SetMassOverrideInKg();
	QuadcopterRoot->SetLinearDamping(1.f);
	QuadcopterRoot->SetAngularDamping(1.f);
	QuadcopterRoot->SetPhysicsMaxAngularVelocity(0.f);
	RootComponent = QuadcopterRoot;

	QuadcopterCamera = CreateDefaultSubobject<UCameraComponent>("QuadcopterCamera");
	QuadcopterCamera->SetupAttachment(RootComponent);
	QuadcopterCamera->SetRelativeRotation(FRotator(55.f, 0.f, 0.f));
	QuadcopterCamera->SetFieldOfView(130.f);
	QuadcopterCamera->SetConstraintAspectRatio(true);


	ThrottleInput = 0.f;
	ThrottleForce = 280000.f;
	bHasThrottleInput = false;
	AbsoluteRotationInput = FRotator::ZeroRotator;
	RelativeRotationInput = FRotator::ZeroRotator;
	bHasAbsoluteRotationInput = false;
	bHasRelativeRotationInput = false;

	MouseSensitivity = 0.f;
	KeyboardYawRate = 0.f;
}

void AQuadcopter::BeginPlay()
{
	Super::BeginPlay();

#if WITH_EDITOR
	SetMouseSensitivity(GetDefault<ULibreFlyerSettings>()->QuadcopterMouseSensitivity);
#endif
	KeyboardYawRate = GetDefault<ULibreFlyerSettings>()->KeyboardYawRate;

}

void AQuadcopter::Tick(float DeltaTime)
{
	if(bHasThrottleInput){
		QuadcopterRoot->AddForce(GetActorUpVector() * ThrottleInput * ThrottleForce * DeltaTime, NAME_None, true);
		bHasThrottleInput = false;
	}
	if (bHasAbsoluteRotationInput || bHasRelativeRotationInput) {
		FRotator InstantaneousRotation = FRotator::ZeroRotator;
		if (bHasRelativeRotationInput) {
			InstantaneousRotation += RelativeRotationInput * DeltaTime;
			RelativeRotationInput = FRotator::ZeroRotator;
			bHasRelativeRotationInput = false;
		}
		if (bHasAbsoluteRotationInput) {
			InstantaneousRotation += AbsoluteRotationInput;
			AbsoluteRotationInput = FRotator::ZeroRotator;
			bHasAbsoluteRotationInput = false;
		}
		AddActorLocalRotation(InstantaneousRotation);
	}
	Super::Tick(DeltaTime);
}

void AQuadcopter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Throttle", this, &AQuadcopter::Throttle);
	PlayerInputComponent->BindAxis("MouseRoll", this, &AQuadcopter::MouseRoll);
	PlayerInputComponent->BindAxis("MousePitch", this, &AQuadcopter::MousePitch);
	PlayerInputComponent->BindAxis("KeyboardYaw", this, &AQuadcopter::KeyboardYaw);
}

void AQuadcopter::Throttle(float AxisInput) {
	if (AxisInput != 0.f) {
		ThrottleInput = AxisInput;
		bHasThrottleInput = true;
	}
}

void AQuadcopter::MouseRoll(float AxisInput) {
	if (AxisInput != 0.f) {
		AbsoluteRotationInput.Roll += AxisInput * MouseSensitivity;
		bHasAbsoluteRotationInput = true;
	}
}

void AQuadcopter::MousePitch(float AxisInput) {
	if (AxisInput != 0.f) {
		AbsoluteRotationInput.Pitch += AxisInput * MouseSensitivity;
		bHasAbsoluteRotationInput = true;
	}
}

void AQuadcopter::KeyboardYaw(float AxisInput) {
	if (AxisInput != 0.f) {
		RelativeRotationInput.Yaw += AxisInput * KeyboardYawRate;
		bHasRelativeRotationInput = true;
	}
}

void AQuadcopter::SetMouseSensitivity(float NewSensitivity) {
	MouseSensitivity = 360.f / 800.f / NewSensitivity * 2.54f;
}

