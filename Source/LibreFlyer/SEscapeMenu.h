// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Input/SSpinBox.h"
#include "Widgets/SCompoundWidget.h"


class LIBREFLYER_API SSettingsEntry : public SCompoundWidget {
public:
	SLATE_BEGIN_ARGS(SSettingsEntry) {
		}
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};

class LIBREFLYER_API SEscapeMenu : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SEscapeMenu)
	{}
	SLATE_END_ARGS()

	static FLinearColor LibreForegroundColor;
	static FSpinBoxStyle LibreSpinboxStyle;
	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
};
