// Fill out your copyright notice in the Description page of Project Settings.


#include "Quadcopter.h"
#include "Camera/CameraComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AQuadcopter::AQuadcopter() {
	PrimaryActorTick.bCanEverTick = true;

	QuadcopterRoot = CreateDefaultSubobject<UStaticMeshComponent>("QuadcopterRoot");
	auto PhysicalMaterial = NewObject<UPhysicalMaterial>();
	PhysicalMaterial->Friction = 1.f;
	PhysicalMaterial->StaticFriction = 1.f;
	PhysicalMaterial->Restitution = 1.f;
	PhysicalMaterial->RaiseMassToPower = 1.f;
	QuadcopterRoot->SetSimulatePhysics(true);
	QuadcopterRoot->SetMassOverrideInKg(NAME_None, 1.f, true);
	//QuadcopterRoot->SetPhysicsMaxAngularVelocity(0.f);
	QuadcopterRoot->SetLinearDamping(1.f);
	QuadcopterRoot->SetAngularDamping(1.f);
	QuadcopterRoot->SetPhysMaterialOverride(PhysicalMaterial);
	QuadcopterRoot->GetBodyInstance()->PositionSolverIterationCount = 255;
	QuadcopterRoot->GetBodyInstance()->VelocitySolverIterationCount = 255;
	QuadcopterRoot->GetBodyInstance()->InertiaTensorScale = FVector::ZeroVector;
	QuadcopterRoot->GetBodyInstance()->StabilizationThresholdMultiplier = 1.f;
	QuadcopterRoot->GetBodyInstance()->UpdateMassProperties();
	RootComponent = QuadcopterRoot;

	QuadcopterModel = CreateDefaultSubobject<UStaticMeshComponent>("QuadcopterModel");
	QuadcopterModel->SetCollisionProfileName("NoCollision");
	QuadcopterModel->SetOwnerNoSee(true);
	QuadcopterModel->SetupAttachment(RootComponent);

	QuadcopterCamera = CreateDefaultSubobject<UCameraComponent>("QuadcopterCamera");
	QuadcopterCamera->SetupAttachment(RootComponent);
	QuadcopterCamera->SetRelativeRotation(FRotator(55.f, 0.f, 0.f));
	QuadcopterCamera->SetFieldOfView(130.f);
	QuadcopterCamera->SetConstraintAspectRatio(true);

	ThrottleInput = 0.f;
	bHasThrottleInput = false;
	AbsoluteRotationInput = FRotator::ZeroRotator;
	RelativeRotationInput = FRotator::ZeroRotator;
	bHasAbsoluteRotationInput = false;
	bHasRelativeRotationInput = false;

	FlyingMouseSensitivity = 0.7f;
	ThrottleForce = 12000.f;
	MaxSpeed = 2800.f;
	BetaflightRates = FBetaflightRates();
	KeyboardRates.Add(200.f);
	KeyboardRates.Add(200.f);
	KeyboardRates.Add(200.f);
}
void AQuadcopter::BeginPlay() {
	Super::BeginPlay();
	AddActorLocalRotation(FRotator(-QuadcopterCamera->GetRelativeRotation().Pitch, 0.f, 0.f));
	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();
}
void AQuadcopter::Tick(float DeltaTime) {
	if(bHasThrottleInput){
		QuadcopterRoot->AddForce(GetActorUpVector() * ThrottleInput * ThrottleForce, NAME_None, true);
		bHasThrottleInput = false;

		auto CurrentSpeed = QuadcopterRoot->GetPhysicsLinearVelocity().Size();
		if (CurrentSpeed > MaxSpeed) {
			QuadcopterRoot->SetPhysicsLinearVelocity((QuadcopterRoot->GetPhysicsLinearVelocity() / CurrentSpeed) * MaxSpeed);
		}
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
	PlayerInputComponent->BindAxis("MouseYaw", this, &AQuadcopter::MouseYaw);

	PlayerInputComponent->BindAxis("KeyboardRoll", this, &AQuadcopter::KeyboardRoll);
	PlayerInputComponent->BindAxis("KeyboardPitch", this, &AQuadcopter::KeyboardPitch);
	PlayerInputComponent->BindAxis("KeyboardYaw", this, &AQuadcopter::KeyboardYaw);

	PlayerInputComponent->BindAxis("StickRoll", this, &AQuadcopter::StickRoll);
	PlayerInputComponent->BindAxis("StickPitch", this, &AQuadcopter::StickPitch);
	PlayerInputComponent->BindAxis("StickYaw", this, &AQuadcopter::StickYaw);

	PlayerInputComponent->BindAction(TEXT("Respawn"), IE_Pressed, this, &ThisClass::Respawn);
}
void AQuadcopter::Throttle(float AxisInput) {
	if (AxisInput > 0.f) {
		ThrottleInput = AxisInput;
		bHasThrottleInput = true;
	}
}
void AQuadcopter::MouseRoll(float AxisInput) {
	if (AxisInput != 0.f) {
		AbsoluteRotationInput.Roll += AxisInput * FlyingMouseSensitivity;
		bHasAbsoluteRotationInput = true;
	}
}
void AQuadcopter::MousePitch(float AxisInput) {
	if (AxisInput != 0.f) {
		AbsoluteRotationInput.Pitch += AxisInput * FlyingMouseSensitivity;
		bHasAbsoluteRotationInput = true;
	}
}
void AQuadcopter::MouseYaw(float AxisInput) {
	if (AxisInput != 0.f) {
		AbsoluteRotationInput.Yaw += AxisInput * FlyingMouseSensitivity;
		bHasAbsoluteRotationInput = true;
	}
}
void AQuadcopter::KeyboardRoll(float AxisInput) {
	if (AxisInput != 0.f) {
		RelativeRotationInput.Roll += AxisInput * KeyboardRates[0];
		bHasRelativeRotationInput = true;
	}
}void AQuadcopter::KeyboardPitch(float AxisInput) {
	if (AxisInput != 0.f) {
		RelativeRotationInput.Pitch += AxisInput * KeyboardRates[1];
		bHasRelativeRotationInput = true;
	}
}
void AQuadcopter::KeyboardYaw(float AxisInput) {
	if (AxisInput != 0.f) {
		RelativeRotationInput.Yaw += AxisInput * KeyboardRates[2];
		bHasRelativeRotationInput = true;
	}
}
void AQuadcopter::StickRoll(float AxisInput) {
	if (AxisInput != 0.f) {
		BetaflightRates.Apply(0, AxisInput, FMath::Abs(AxisInput));
		RelativeRotationInput.Roll = AxisInput;
		bHasRelativeRotationInput = true;
	}
}
void AQuadcopter::StickPitch(float AxisInput) {
	if (AxisInput != 0.f) {
		BetaflightRates.Apply(1, AxisInput, FMath::Abs(AxisInput));
		RelativeRotationInput.Pitch = AxisInput;
		bHasRelativeRotationInput = true;
	}
}
void AQuadcopter::StickYaw(float AxisInput) {
	if (AxisInput != 0.f) {
		BetaflightRates.Apply(2, AxisInput, FMath::Abs(AxisInput));
		RelativeRotationInput.Yaw = AxisInput;
		bHasRelativeRotationInput = true;
	}
}
void AQuadcopter::SetMouseSensitivity(float NewSensitivity) {
	FlyingMouseSensitivity = 360.f / 800.f / NewSensitivity * 2.54f;
}
void AQuadcopter::Respawn() {
	SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
	QuadcopterRoot->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	QuadcopterRoot->SetPhysicsLinearVelocity(FVector::ZeroVector);
}
