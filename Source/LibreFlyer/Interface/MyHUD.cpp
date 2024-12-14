// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"

void AMyHUD::BeginPlay() {
	Super::BeginPlay();
	EnableInput(PlayerOwner);
	InputComponent->BindAction(TEXT("EscapeMenu"), IE_Pressed, this, &ThisClass::EscapeMenu);
}

void AMyHUD::EscapeMenu() {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Test");

}
