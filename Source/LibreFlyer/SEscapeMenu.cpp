// Fill out your copyright notice in the Description page of Project Settings.


#include "SEscapeMenu.h"
#include "SlateOptMacros.h"
#include "Widgets/Layout/SConstraintCanvas.h"

//FLinearColor SEscapeMenu::LibreForegroundColor = FLinearColor(0.7f, 0.7f, 0.7f, 1.f);
//FSpinBoxStyle SEscapeMenu::LibreSpinboxStyle = *new FSpinBoxStyle(FCoreStyle::Get().GetWidgetStyle<FSpinBoxStyle>("SpinBox"));
//FSpinBoxStyle SEscapeMenu::LibreSpinboxStyle.BackgroundBrush = *FCoreStyle::Get().GetBrush("GenericWhiteBox");
//FSpinBoxStyle SEscapeMenu::LibreSpinboxStyle.BackgroundBrush.TintColor = FLinearColor::Black;
//FSpinBoxStyle SEscapeMenu::LibreSpinboxStyle.HoveredBackgroundBrush = FSpinBoxStyle SEscapeMenu::LibreSpinboxStyle.BackgroundBrush;
//FSpinBoxStyle SEscapeMenu::LibreSpinboxStyle.ActiveFillBrush = *FCoreStyle::Get().GetBrush("GenericWhiteBox");
//FSpinBoxStyle SEscapeMenu::LibreSpinboxStyle.InactiveFillBrush = *FCoreStyle::Get().GetBrush("GenericWhiteBox");
//FSpinBoxStyle SEscapeMenu::LibreSpinboxStyle.TextPadding = FMargin(5.f, 5.f, 0.f, 5.f);
//FSpinBoxStyle SEscapeMenu::LibreSpinboxStyle.ForegroundColor = LibreForegroundColor;


BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSettingsEntry::Construct(const FArguments& InArgs) {
	ChildSlot
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.Padding(10.f, 5.f)
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
						.Text(FText::FromString("Setting Name"))
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 17))
				]
				+ SHorizontalBox::Slot()
				.Padding(10.f, 5.f)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Center)
				[
					SNew(SSpinBox<float>)
						.Style(&FCoreStyle::Get().GetWidgetStyle<FSpinBoxStyle>("SpinBox"))
						//.Style(&SEscapeMenu::LibreSpinboxStyle)
						.Font(FCoreStyle::GetDefaultFontStyle("Regular", 16))
						.MinDesiredWidth(120.f)
				]
		];
}


void SEscapeMenu::Construct(const FArguments& InArgs)
{
	TSharedPtr<SVerticalBox> VerticalBox;
	ChildSlot
	[
		SNew(SConstraintCanvas)
			+SConstraintCanvas::Slot()
			.AutoSize(true)
			.Alignment(FVector2D(1.f, 0.5f))
			.Anchors(FAnchors(1.f, 0.5f))
			[
				SNew(SBorder)
					.BorderImage(FCoreStyle::Get().GetBrush("GenericWhiteBox"))
					.BorderBackgroundColor(FLinearColor(0.f, 0.f, 0.f, 0.7f))
					[
						SAssignNew(VerticalBox, SVerticalBox)
					]
			]
	];
	for (size_t i = 0; i < 10; i++) {
		VerticalBox->AddSlot() [SNew(SSettingsEntry)];
	}
	
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION


