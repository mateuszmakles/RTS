// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class RTS_API AMyHUD : public AHUD
{
	GENERATED_BODY()

protected:

	virtual void DrawHUD() override;

public:

	bool bLeftMousePressed = false;

	FVector2D InitialPoint;

	FVector2D CurrentPoint;

	FVector2D GetMousePosition() const;

	UPROPERTY()
	TArray<class ARTSCharacter*> SelectedActors;
	
};
