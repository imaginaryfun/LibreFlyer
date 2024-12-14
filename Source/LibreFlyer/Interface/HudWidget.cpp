// Fill out your copyright notice in the Description page of Project Settings.


#include "HudWidget.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSlateHud::Construct(const FArguments& InArgs) {
	//if (InArgs._OwningPlayerController.IsValid()) {
	//	HudWidget = TWeakObjectPtr<UHudWidget>(Cast<UHudWidget>(UUserWidget::CreateWidgetInstance(
	//		*InArgs._OwningPlayerController,
	//		GetDefault<ULibreFlyerEditorSettings>()->HudWidget,
	//		"HudWidget"
	//	)));
	//	HudWidget->AddToPlayerScreen();
	//}
}
void SSlateHud::OnCheckpointTriggered() {

}
#include "Widgets/Layout/SScaleBox.h"
void SCheckpointNumber::Construct(const FArguments& InArgs) {
	ChildSlot
	[
		SNew(SScaleBox)
		.Stretch(EStretch::ScaleToFit)
			[
				SAssignNew(CheckpointNumberTextBlock, STextBlock)
				.Font(FCoreStyle::GetDefaultFontStyle("Regular", 17))						
			]
	];
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

//void UHudWidget::OnCheckpointTriggered_Implementation() {
//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "UHudWidget::OnCheckpointTriggered");
//}

void UCheckpointNumberWidget::SetCheckpointNumber(int NewNumber) {
	CheckpointIndex = NewNumber;
	SynchronizeProperties();
}
#include "Fonts/SlateFontInfo.h"

void UCheckpointNumberWidget::NativeOnInitialized() {
	//CheckpointIndex = -1;
	NumberFormat = FNumberFormattingOptions();
	NumberFormat.SetMinimumIntegralDigits(2);
	NumberFormat.SetUseGrouping(false);
}
TSharedRef<SWidget> UCheckpointNumberWidget::RebuildWidget() {
	if (!CheckpointNumber) CheckpointNumber = SNew(SCheckpointNumber);
	SynchronizeProperties();
	return CheckpointNumber.ToSharedRef();
}
void UCheckpointNumberWidget::ReleaseSlateResources(bool bReleaseChildren) {
	CheckpointNumber.Reset();
	Super::ReleaseSlateResources(bReleaseChildren);
}
void UCheckpointNumberWidget::SynchronizeProperties() {
	if (CheckpointNumber) {
		CheckpointNumber->CheckpointNumberTextBlock->SetText(FText::AsNumber(CheckpointIndex, &NumberFormat));
	}
	Super::SynchronizeProperties();
}


