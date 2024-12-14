// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "HudWidget.generated.h"

class LIBREFLYER_API SCheckpointNumber : public SCompoundWidget {
public:
	SLATE_BEGIN_ARGS(SCheckpointNumber) {
		}
	SLATE_END_ARGS()
	void Construct(const FArguments& InArgs);
	TSharedPtr<STextBlock> CheckpointNumberTextBlock;
};
class LIBREFLYER_API SSlateHud : public SCompoundWidget {
public:
	SLATE_BEGIN_ARGS(SSlateHud) 
		: _OwningPlayerController(nullptr)
		{}
			SLATE_ARGUMENT(TWeakObjectPtr<APlayerController>, OwningPlayerController)
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);
	TWeakObjectPtr<UHudWidget> HudWidget;
	void OnCheckpointTriggered();
};
UCLASS()
class LIBREFLYER_API UCheckpointNumberWidget : public UUserWidget {
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetCheckpointNumber(int NewNumber);

	TSharedPtr<SCheckpointNumber> CheckpointNumber;
	FNumberFormattingOptions NumberFormat;
	virtual void NativeOnInitialized() override;
	int CheckpointIndex = -1;
protected:
	virtual TSharedRef<SWidget> RebuildWidget() override final;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	virtual void SynchronizeProperties() override;

	/** The font to render the text with */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Appearance)
	FSlateFontInfo Font;
};
UCLASS(HideDropDown)
class LIBREFLYER_API UHudWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//void OnCheckpointTriggered();
	//virtual void OnCheckpointTriggered_Implementation() override;

};
