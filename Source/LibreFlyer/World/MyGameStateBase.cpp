// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "LibreMain.h"
#include "Blueprint/UserWidget.h"
#include "SEscapeMenu.h"
#include "Checkpoint.h"
#include "EngineUtils.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerState.h"
#include "Quadcopter.h"
#include "HudWidget.h"
#include "Net/Core/PushModel/PushModel.h"
#include "Blueprint/UserWidget.h"

AMyGameStateBase::AMyGameStateBase() {
	Checkpoints = TArray<ACheckpoint*>();
	NumberOfCheckpoints = 0;
	bReceivedCheckpoints = false;
	bReceivedStartingBlocks = false;
	bFinishedBeginPlay = false;
	LocalQuadcopter = nullptr;
	LocalPlayerState = nullptr;
}
void AMyGameStateBase::BeginPlay() {
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &AMyGameStateBase::PostBeginPlay);
	Super::BeginPlay();
}
void AMyGameStateBase::PostBeginPlay() {
	for (auto& i : GetDefault<ULibreFlyerEditorSettings>()->PlayInEditorStartupConsoleCommands) {
		GEngine->Exec(GetWorld(), *i);
	}
	GEngine->Exec(
		GetWorld(),
		*FString::Printf(
			TEXT("t.maxfps %s"),
			*LexToSanitizedString(GetDefault<ULibreFlyerEditorSettings>()->PlayInEditorMaxFramerate
			)));
	if (GetDefault<ULibreFlyerEditorSettings>()->PlayInEditorUserWidget) {
		auto NewHud = UUserWidget::CreateWidgetInstance(
			*GetWorld(),
			GetDefault<ULibreFlyerEditorSettings>()->PlayInEditorUserWidget,
			"PlayInEditorUserWidget"
		);
		NewHud->AddToPlayerScreen();
	}
	if (GetDefault<ULibreFlyerEditorSettings>()->bShowPlayInEditorFramerate) {
		GEngine->Exec(
			GetWorld(),
			TEXT("stat fps")
		);
	}
	for (TActorIterator<APlayerController> i(GetWorld()); i; ++i) {
		if (auto CurrentPlayer = i->GetLocalPlayer()) {
			if (i->GetPawn()) {
				LocalQuadcopter = Cast<AQuadcopter>(i->GetPawn());
				if (LocalQuadcopter) {
					LocalQuadcopter->CheckpointTriggered.AddDynamic(this, &AMyGameStateBase::OnCheckpointTriggered);
					LocalPlayerState = Cast<AMyPlayerState>(LocalQuadcopter->GetPlayerState());
					if (LocalPlayerState) {
						if (!GetDefault<ULibreFlyerEditorSettings>()->bPlayInEditorSupressGameHud) {
							GEngine->GameViewport->AddViewportWidgetForPlayer(
								CurrentPlayer,
								SAssignNew(LocalHud, SSlateHud)
								.OwningPlayerController(TWeakObjectPtr<APlayerController>(*i)),
								0
							);
							if (GetDefault<ULibreFlyerEditorSettings>()->HudWidget) {
								auto HudWidget = UUserWidget::CreateWidgetInstance(
									**i,
									GetDefault<ULibreFlyerEditorSettings>()->HudWidget,
									"HudWidget"
								);
								HudWidget->AddToPlayerScreen();
							}
						}
					}
					break;
				}
			}
		}
	}
	bFinishedBeginPlay = true;
	TryReady();
}
void AMyGameStateBase::ServerPostBeginPlay_Implementation() {
	for (TActorIterator<ACheckpoint> i(GetWorld()); i; ++i) {
		Checkpoints.Add(*i);
	}
	OnRep_Checkpoints();
}
void AMyGameStateBase::TryReady() {
	if (bFinishedBeginPlay && bReceivedCheckpoints && bReceivedStartingBlocks) {
		ForceReady();
	}
}
void AMyGameStateBase::ForceReady() {
	NumberOfCheckpoints = Checkpoints.Num();
	if (NumberOfCheckpoints > 0) {
		LocalPlayerState->CurrentCheckpointIndex = 0;
		for (size_t i = 0; i < NumberOfCheckpoints; i++) {
			Checkpoints[i]->SetCheckpointIndex(i);
		}
	}
}
void AMyGameStateBase::OnRep_Checkpoints() {
	bReceivedCheckpoints = true;
	TryReady();
}
void AMyGameStateBase::OnCheckpointTriggered(const FCheckpointTriggeredEvent& CheckpointEvent) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Checkpoint Triggered");
	if (auto TriggeredCheckpoint = Cast<ACheckpoint>(CheckpointEvent.Hit.GetActor())) {
		int32 TriggeredCheckpointIndex = -1;
		Checkpoints.Find(TriggeredCheckpoint, TriggeredCheckpointIndex);

		if (auto MyPlayerState = Cast<AMyPlayerState>(CheckpointEvent.TriggeringPawn->GetPlayerState())) {
			if (MyPlayerState->CurrentCheckpointIndex == TriggeredCheckpointIndex) {
				if (FVector::DotProduct(CheckpointEvent.TriggeringPawn->GetVelocity().GetSafeNormal(), TriggeredCheckpoint->CheckpointTrigger->GetForwardVector()) > 0.f) {
					UGameplayStatics::PlaySound2D(
						GetWorld(),
						GetDefault<ULibreFlyerEditorSettings>()->PlayInEditorCheckpointSound
					);
					LocalHud->OnCheckpointTriggered();
				}
			}
		}
	}


	//auto Checkpoint = Cast<ACheckpoint>(CheckpointEvent.Hit.Actor);
	//auto CheckpointIndex = Checkpoint->GetCheckpointIndex();
	//if (CheckpointIndex == LocalPlayerState->CurrentCheckpointIndex) {
	//	if (FVector::DotProduct(CurrentCheckpointRay.Direction, LocalQuadcopter->GetVelocity().GetSafeNormal()) > 0.f) {
	//		// todo - make a new variable LastCheckpointIndex and set it once, instead of  doing (TotalNumberOfCheckpoints - 1) every time
	//		auto NextCheckpointIndex = (LocalPlayerState->NextCheckpointIndex == (TotalNumberOfCheckpoints - 1)) ? 0 : (LocalPlayerState->NextCheckpointIndex + 1);
	//		LocalPlayerState->OnCheckpointTriggered(NextCheckpointIndex);
	//		Checkpoints[LocalPlayerState->CurrentCheckpointIndex]->GetCheckpointRay(CurrentCheckpointRay);
	//		FRay NextCheckpointRay;
	//		Checkpoints[LocalPlayerState->NextCheckpointIndex]->GetCheckpointRay(NextCheckpointRay);
	//		LocalHud->OnCheckpointTriggered(NextCheckpointRay);
	//	}
	//}
}
void AMyGameStateBase::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	for (auto& i : GetDefault<ULibreFlyerEditorSettings>()->PlayInEditorShutdownConsoleCommands) {
		GEngine->Exec(GetWorld(), *i);
	}
	GEngine->Exec(
		GetWorld(),
		*FString::Printf(
			TEXT("t.maxfps %s"),
			*LexToSanitizedString(GetDefault<ULibreFlyerEditorSettings>()->EditorMaxFramerate
			)));
	if (GetDefault<ULibreFlyerEditorSettings>()->bShowPlayInEditorFramerate) {
		GEngine->Exec(
			GetWorld(),
			TEXT("stat none")
		);
	}
	Super::EndPlay(EndPlayReason);
}
void AMyGameStateBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	FDoRepLifetimeParams SharedParams;

	SharedParams.bIsPushBased = true;
	DOREPLIFETIME_WITH_PARAMS_FAST(AMyGameStateBase, Checkpoints, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AMyGameStateBase, StartingBlocks, SharedParams);
	//SharedParams.bIsPushBased = false;
	//DOREPLIFETIME_WITH_PARAMS_FAST(AMyGameStateBase, bResetTrack, SharedParams);
}
void AMyGameStateBase::OnRep_StartingBlocks() {
	bReceivedStartingBlocks = true;
	TryReady();
}