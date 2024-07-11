// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsOverlay.generated.h"

class UButton;
class ASBSPPlayerController;
/**
 * 
 */
UCLASS()
class SBSP_API USettingsOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

protected:
	UFUNCTION()
	void OnStartButtonPressed();

private:
	UPROPERTY()
	ASBSPPlayerController* SBSPPlayerController;
};
