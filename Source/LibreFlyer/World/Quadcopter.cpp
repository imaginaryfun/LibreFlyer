// Fill out your copyright notice in the Description page of Project Settings.


#include "Quadcopter.h"
#include "Camera/CameraComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PhysicsEngine/BodyInstance.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"

AQuadcopter::AQuadcopter() {
	PrimaryActorTick.bCanEverTick = true;

	QuadcopterRoot = CreateDefaultSubobject<UStaticMeshComponent>("QuadcopterRoot");
	//QuadcopterRoot = CreateDefaultSubobject<UBoxComponent>("QuadcopterRoot");
	//QuadcopterRoot->SetBoxExtent(FVector::OneVector);
	//QuadcopterRoot = CreateDefaultSubobject<USphereComponent>("QuadcopterRoot");
	//QuadcopterRoot->SetSphereRadius(1.f);

	QuadcopterRoot->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
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

	ThrusterDistance = 7.f;
	ThrusterOffset = 0.f;

}
void AQuadcopter::BeginPlay() {
	Super::BeginPlay();
	// physics
	auto PhysicalMaterial = NewObject<UPhysicalMaterial>();
	PhysicalMaterial->Friction = 1.f;
	PhysicalMaterial->StaticFriction = 1.f;
	PhysicalMaterial->Restitution = 1.f;
	PhysicalMaterial->RaiseMassToPower = 1.f;
	QuadcopterRoot->SetPhysMaterialOverride(PhysicalMaterial);
	QuadcopterRoot->SetSimulatePhysics(true);
	QuadcopterRoot->SetMassOverrideInKg(NAME_None, 1.f, true);
	QuadcopterRoot->SetPhysicsMaxAngularVelocityInDegrees(0.f);
	QuadcopterRoot->SetLinearDamping(1.f);
	QuadcopterRoot->SetAngularDamping(1.f);
	QuadcopterRoot->GetBodyInstance()->PositionSolverIterationCount = 255;
	QuadcopterRoot->GetBodyInstance()->VelocitySolverIterationCount = 255;
	QuadcopterRoot->GetBodyInstance()->InertiaTensorScale = FVector::ZeroVector;
	QuadcopterRoot->GetBodyInstance()->StabilizationThresholdMultiplier = 1000.f;
	QuadcopterRoot->GetBodyInstance()->UpdateMassProperties();

	// spawn
	AddActorLocalRotation(FRotator(-QuadcopterCamera->GetRelativeRotation().Pitch, 0.f, 0.f));
	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();
}
void AQuadcopter::Tick(float DeltaTime) {
	// throttle
	ThrottleInput = InputComponent->GetAxisValue("Throttle");
	if (ThrottleInput != 0.f) {
		ThrottleInput = FMath::Max(0.f, ThrottleInput);
		FVector InstantaneousForce;

		QuadcopterRoot->AddForce(GetActorUpVector() * ThrottleInput * ThrottleForce, NAME_None, true);
		if (ThrusterDistance == 0.f) {
			InstantaneousForce = GetActorUpVector() * ThrottleInput * ThrottleForce;
			QuadcopterRoot->AddForce(InstantaneousForce * 4.f, NAME_None, true);
		}
		else {
			InstantaneousForce = FVector(0.f, 0.f, ThrottleInput * ThrottleForce);
			QuadcopterRoot->AddForceAtLocationLocal(InstantaneousForce, FVector(-ThrusterDistance, ThrusterDistance, ThrusterOffset));
			QuadcopterRoot->AddForceAtLocationLocal(InstantaneousForce, FVector(ThrusterDistance, ThrusterDistance, ThrusterOffset));
			QuadcopterRoot->AddForceAtLocationLocal(InstantaneousForce, FVector(ThrusterDistance, -ThrusterDistance, ThrusterOffset));
			QuadcopterRoot->AddForceAtLocationLocal(InstantaneousForce, FVector(-ThrusterDistance, -ThrusterDistance, ThrusterOffset));
		}
		auto CurrentSpeed = QuadcopterRoot->GetPhysicsLinearVelocity().Size();
		if (CurrentSpeed > MaxSpeed) {
			QuadcopterRoot->SetPhysicsLinearVelocity((QuadcopterRoot->GetPhysicsLinearVelocity() / CurrentSpeed) * MaxSpeed);
		}
	}
	// mouse rotation
	AbsoluteRotationInput.Roll += InputComponent->GetAxisValue("MouseRoll");
	AbsoluteRotationInput.Pitch += InputComponent->GetAxisValue("MousePitch");
	AbsoluteRotationInput.Yaw += InputComponent->GetAxisValue("MouseYaw");
	if (!AbsoluteRotationInput.IsZero()) {
		bHasAbsoluteRotationInput = true;
		AbsoluteRotationInput *= FlyingMouseSensitivity;
	}
	// keyboard rotation
	RelativeRotationInput.Roll += InputComponent->GetAxisValue("KeyboardRoll") * KeyboardRates[0];
	RelativeRotationInput.Pitch += InputComponent->GetAxisValue("KeyboardPitch") * KeyboardRates[1];
	RelativeRotationInput.Yaw += InputComponent->GetAxisValue("KeyboardYaw") * KeyboardRates[2];
	bHasRelativeRotationInput = !RelativeRotationInput.IsZero();
	// stick rotation
	RelativeRotationInput.Roll = InputComponent->GetAxisValue("StickRoll");
	RelativeRotationInput.Pitch = InputComponent->GetAxisValue("StickPitch");
	RelativeRotationInput.Yaw = InputComponent->GetAxisValue("StickYaw");
	if (!RelativeRotationInput.IsZero()) {
		bHasRelativeRotationInput = true;
		BetaflightRates.Apply(0, RelativeRotationInput.Roll, FMath::Abs(RelativeRotationInput.Roll));
		BetaflightRates.Apply(1, RelativeRotationInput.Pitch, FMath::Abs(RelativeRotationInput.Pitch));
		BetaflightRates.Apply(2, RelativeRotationInput.Yaw, FMath::Abs(RelativeRotationInput.Yaw));
	}
	// combine rotations
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
	// checkpoint triggering
	auto CurrentLocation = RootComponent->GetComponentTransform().GetTranslation();
	auto Hit = FHitResult();
	GetWorld()->LineTraceSingleByChannel(
		Hit,
		CurrentLocation,
		PreviousLocation,
		ECollisionChannel::ECC_GameTraceChannel1,
		HitParameters
	);
	PreviousLocation = CurrentLocation;
	if (Hit.bBlockingHit) {
		if (CheckpointTriggered.IsBound()) CheckpointTriggered.Broadcast(FCheckpointTriggeredEvent(Hit, this));
	}

	Super::Tick(DeltaTime);
}
void AQuadcopter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction(TEXT("Respawn"), IE_Pressed, this, &ThisClass::Respawn);
}
void AQuadcopter::SetMouseSensitivity(float NewSensitivity) {
	FlyingMouseSensitivity = 360.f / 800.f / NewSensitivity * 2.54f;
}
void AQuadcopter::Respawn() {
	SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
	QuadcopterRoot->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
	QuadcopterRoot->SetPhysicsLinearVelocity(FVector::ZeroVector);
}
