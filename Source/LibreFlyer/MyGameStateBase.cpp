// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "MyEngineSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "SEscapeMenu.h"
#include "Checkpoint.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerState.h"
#include "Quadcopter.h"

AMyGameStateBase::AMyGameStateBase() {
	Checkpoints = TArray<ACheckpoint*>();
	NumberOfCheckpoints = 0;
	bReceivedCheckpoints = false;
	LocalQuadcopter = nullptr;
	LocalPlayerState = nullptr;
}

void AMyGameStateBase::BeginPlay() {
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &AMyGameStateBase::PostBeginPlay);
	Super::BeginPlay();
}
void AMyGameStateBase::PostBeginPlay() {
	for (auto& i : GetDefault<ULibreFlyerSettings>()->PlayInEditorStartupConsoleCommands) {
		GEngine->Exec(GetWorld(), *i);
	}
	GEngine->Exec(
		GetWorld(),
		*FString::Printf(
			TEXT("t.maxfps %s"),
			*LexToSanitizedString(GetDefault<ULibreFlyerSettings>()->PlayInEditorMaxFramerate
			)));
	if (GetDefault<ULibreFlyerSettings>()->PlayInEditorUserWidget) {
		auto NewHud = UUserWidget::CreateWidgetInstance(
			*GetWorld(),
			GetDefault<ULibreFlyerSettings>()->PlayInEditorUserWidget,
			"PlayInEditorUserWidget"
		);
		NewHud->AddToPlayerScreen();
	}
	if (!GetDefault<ULibreFlyerSettings>()->bPlayInEditorSupressGameHud) {
		GEngine->GameViewport->AddViewportWidgetContent(
			SNew(SEscapeMenu)
		);
	}
	if (GetDefault<ULibreFlyerSettings>()->bShowPlayInEditorFramerate) {
		GEngine->Exec(
			GetWorld(),
			TEXT("stat fps")
		);
	}
	for (TActorIterator<APlayerController> i(GetWorld()); i; ++i) {
		if (i->GetPawn()) {
			LocalQuadcopter = Cast<AQuadcopter>(i->GetPawn());
			if (LocalQuadcopter) {
				LocalPlayerState = Cast<AMyPlayerState>(LocalQuadcopter->GetPlayerState());
				if (HasAuthority()) {
					if (LocalPlayerState) {
						ServerPostBeginPlay();
					}
				}
				break;
			}
		}
	}

}
void AMyGameStateBase::ServerPostBeginPlay_Implementation() {
	for (TActorIterator<ACheckpoint> i(GetWorld()); i; ++i) {
		Checkpoints.Add(*i);
	}
	OnRep_Checkpoints();
}
void AMyGameStateBase::TryReady() {
	if (bReceivedCheckpoints) {
		NumberOfCheckpoints = Checkpoints.Num();
		if (NumberOfCheckpoints > 1) {
			LocalPlayerState->CurrentCheckpointIndex = 0;
			for (auto& i : Checkpoints) {
				i->CheckpointTrigger->OnComponentBeginOverlap.AddDynamic(this, &AMyGameStateBase::OnCheckpointTriggered);
			}
		}
	}
}
void AMyGameStateBase::OnRep_Checkpoints() {
	bReceivedCheckpoints = true;
	TryReady();
}
void AMyGameStateBase::OnCheckpointTriggered(UPrimitiveComponent* OverlappedComp,
	AActor* Other, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult) {

	if (auto TriggeredCheckpoint = Cast<ACheckpoint>(SweepResult.GetActor())) {
		int32 TriggeredCheckpointIndex = -1;
		Checkpoints.Find(TriggeredCheckpoint, TriggeredCheckpointIndex);

		if (auto TriggeringPawn = Cast<APawn>(Other)) {
			if (auto MyPlayerState = Cast<AMyPlayerState>(TriggeringPawn->GetPlayerState())) {
				if (MyPlayerState->CurrentCheckpointIndex == TriggeredCheckpointIndex) {
					if (FVector::DotProduct(TriggeringPawn->GetVelocity().GetSafeNormal(), TriggeredCheckpoint->CheckpointTrigger->GetForwardVector()) > 0.f) {
						UGameplayStatics::PlaySound2D(
							GetWorld(),
							GetDefault<ULibreFlyerSettings>()->PlayInEditorCheckpointSound
						);
						GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Checkpoint Triggered");
					}
				}
			}
		}
	}
}
void AMyGameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	for (auto& i : GetDefault<ULibreFlyerSettings>()->PlayInEditorShutdownConsoleCommands) {
		GEngine->Exec(GetWorld(), *i);
	}
	GEngine->Exec(
		GetWorld(),
		*FString::Printf(
			TEXT("t.maxfps %s"),
			*LexToSanitizedString(GetDefault<ULibreFlyerSettings>()->EditorMaxFramerate
			)));
	if (GetDefault<ULibreFlyerSettings>()->bShowPlayInEditorFramerate) {
		GEngine->Exec(
			GetWorld(),
			TEXT("stat none")
		);
	}

	Super::EndPlay(EndPlayReason);
}

void AMyGameStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyGameStateBase, Checkpoints);
}