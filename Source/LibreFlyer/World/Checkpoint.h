// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

class UArrowComponent;
class UWidgetComponent;
class SCheckpointNumber;

UCLASS()
class LIBREFLYER_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

	bool operator<(const ACheckpoint& Other) const {
		return CheckpointIndex < Other.CheckpointIndex;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void PostBeginPlay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CheckpointTrigger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CheckpointModel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UArrowComponent* CheckpointArrow;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* CheckpointNumber;

	TSharedPtr<SCheckpointNumber> CheckpointNumberWidget;


	virtual void OnConstruction(const FTransform& Transform) override;
	virtual bool Rename(const TCHAR* NewName = nullptr, UObject* NewOuter = nullptr, ERenameFlags Flags = REN_None);
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
	void MarkAsDormantCheckpoint();
	void GetCheckpointRay(FRay& Output);

	UFUNCTION(BlueprintCallable)
	void SetCheckpointIndex(int NewIndex);
	UFUNCTION(BlueprintCallable)
	int GetCheckpointIndex() { return CheckpointIndex; };

	FNumberFormattingOptions NumberFormat;



private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int CheckpointIndex;
	void ValidateCheckpointIndex();

};
