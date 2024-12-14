// Fill out your copyright notice in the Description page of Project Settings.

#include "Checkpoint.h"
#include "Components/ArrowComponent.h"
#include "Components/WidgetComponent.h"
#include "EngineUtils.h"
#include "HudWidget.h"

ACheckpoint::ACheckpoint() {
	bRunConstructionScriptOnDrag = false;
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("CheckpointRoot");

	CheckpointTrigger = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointTrigger");
	CheckpointTrigger->SetupAttachment(RootComponent);
	//CheckpointTrigger->SetHiddenInGame(true);
	CheckpointTrigger->SetCollisionProfileName("OverlapAll");
	CheckpointTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);

	CheckpointModel = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointModel");
	CheckpointModel->SetupAttachment(RootComponent);

	CheckpointArrow = CreateDefaultSubobject<UArrowComponent>("CheckpointArrow");
	CheckpointArrow->SetupAttachment(CheckpointTrigger);
	CheckpointArrow->SetUsingAbsoluteScale(true);
	CheckpointArrow->ArrowSize = 2.5f;

	CheckpointNumber = CreateDefaultSubobject<UWidgetComponent>("CheckpointNumber");
	CheckpointNumber->SetupAttachment(CheckpointTrigger);
	CheckpointNumber->SetDrawSize(FVector2D(200.f));
	CheckpointNumber->SetUsingAbsoluteRotation(true);
	CheckpointNumber->SetUsingAbsoluteScale(true);
	CheckpointNumber->SetHiddenInGame(true);
	//CheckpointNumber->SetEditTimeUsable(true);
	//CheckpointNumber->SetManuallyRedraw(true);
	//CheckpointNumber->PrimaryComponentTick.bTickEvenWhenPaused = true;

	auto CheckpointNumberMaterial = LoadObject<UMaterial>(
		nullptr,
		L"Material'/Game/CheckpointNumberMaterial.CheckpointNumberMaterial'"
	);
	CheckpointNumber->SetMaterial(0, CheckpointNumberMaterial);


	CheckpointIndex = -1;

	NumberFormat = FNumberFormattingOptions();
	NumberFormat.SetMinimumIntegralDigits(2);
	NumberFormat.SetUseGrouping(false);
}
void ACheckpoint::BeginPlay() {
	Super::BeginPlay();	
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &ACheckpoint::PostBeginPlay);

	//SAssignNew(CheckpointNumberWidget, SCheckpointNumber);
	//CheckpointNumber->SetSlateWidget(CheckpointNumberWidget);


}
void ACheckpoint::PostBeginPlay() {
	SetCheckpointIndex(CheckpointIndex);

}
void ACheckpoint::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	//CheckpointNumber->RequestRenderUpdate();
}
void ACheckpoint::OnConstruction(const FTransform& Transform) {
	if (auto World = GetWorld()) {
		if (CheckpointIndex == -1) {
			TArray<ACheckpoint*> Checkpoints;
			for (TActorIterator<ACheckpoint> i(World); i; ++i) {
				Checkpoints.Add(*i);
			}
			SetCheckpointIndex(Checkpoints.Num() - 1);
		}
		else {
			ValidateCheckpointIndex();
		}
		SetCheckpointIndex(CheckpointIndex);

	}
}
bool ACheckpoint::Rename(const TCHAR* NewName, UObject* NewOuter, ERenameFlags Flags) {
	auto Output = Super::Rename(NewName, NewOuter, Flags);
	// this is a bugfix for a specific case where you duplicate a checkpoint by holding Alt and dragging the gizmo
	if (auto World = GetWorld()) {
		ACheckpoint* NewCheckpoint = nullptr;
		ACheckpoint* OldCheckpoint = nullptr;

		TArray<ACheckpoint*> Checkpoints;
		for (TActorIterator<ACheckpoint> i(World); i; ++i) {
			Checkpoints.Add(*i);
			if (i->GetName().Contains(FString(NewName))) {
				NewCheckpoint = *i;
			}
			if (i->GetName().Contains(GetName())) {
				OldCheckpoint = *i;
			}
		}
		if (NewCheckpoint && OldCheckpoint) {
			NewCheckpoint->SetCheckpointIndex(Checkpoints.Num() - 1);
		}
	}
	return Output;
}
#if WITH_EDITOR
void ACheckpoint::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	const FName PropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);
	// if this checkpoint had it's index changed...
	if ((PropertyName == GET_MEMBER_NAME_STRING_CHECKED(ACheckpoint, CheckpointIndex))) {
		ValidateCheckpointIndex();
		SetCheckpointIndex(CheckpointIndex);
	}
}
#endif // WITH_EDITOR

void ACheckpoint::SetCheckpointIndex(int NewIndex) {
	CheckpointIndex = NewIndex;
	//if(CheckpointNumberWidget) CheckpointNumberWidget->CheckpointNumberTextBlock->SetText(
	//		FText::AsNumber(CheckpointIndex, &NumberFormat));
	if (auto NumberWidget = Cast<UCheckpointNumberWidget>(CheckpointNumber->GetWidget())) {
		UE_LOG(LogTemp, Error, TEXT("%s"), *LexToString(NewIndex));
		NumberWidget->SetCheckpointNumber(CheckpointIndex);
	}
}
void ACheckpoint::ValidateCheckpointIndex() {
	if (auto World = GetWorld()) {
		ACheckpoint* TestCheckpoint = nullptr;
		TArray<ACheckpoint*> Checkpoints;
		bool bNeedsFixedNumber = false;
		for (TActorIterator<ACheckpoint> i(World); i; ++i) {
			Checkpoints.Add(*i);
			if (*i != this) {
				// if some other checkpoint has this index already...
				if (i->CheckpointIndex == CheckpointIndex) {
					TestCheckpoint = *i;
					bNeedsFixedNumber = true;
				}
			}
		}
		if (bNeedsFixedNumber) {
			bNeedsFixedNumber = false;
			auto TestNumber = -1;
			Checkpoints.Sort();
			for (auto i2 : Checkpoints) {
				if ((i2->CheckpointIndex - TestNumber) > 1) {
					TestCheckpoint->SetCheckpointIndex(TestNumber + 1);
					bNeedsFixedNumber = true; // found a missing CheckpointIndex
					break;
				}
				TestNumber = i2->CheckpointIndex;
			}
			if (!bNeedsFixedNumber) { // did not find a missing CheckpointIndex
				TestCheckpoint->SetCheckpointIndex(Checkpoints.Num() - 1);
			}
		}
	}
}
void ACheckpoint::MarkAsDormantCheckpoint() {
	CheckpointTrigger->SetHiddenInGame(true);
}

void ACheckpoint::GetCheckpointRay(FRay& Output) {
	Output.Origin = CheckpointTrigger->GetComponentLocation();
	Output.Direction = CheckpointArrow->GetForwardVector();
}
