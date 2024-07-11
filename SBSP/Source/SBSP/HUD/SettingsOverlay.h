// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SBSP/Controllers/SBSPPlayerController.h"
#include "SettingsOverlay.generated.h"

class USpinBox;
class UButton;
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

	UPROPERTY(meta = (BindWidget))
	USpinBox* StructureRadiusInput;
	UPROPERTY(meta = (BindWidget))
	USpinBox* HarbourRadiusInput;

protected:
	UFUNCTION()
	void OnStartButtonPressed();

private:
	UPROPERTY()
	ASBSPPlayerController* SBSPPlayerController;
	UPROPERTY()
	FSimSettings SimSettings;
};
