// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "RTSCharacter.h"

void AMyHUD::DrawHUD()
{	
	if (bLeftMousePressed)
	{
		if (SelectedActors.Num()>0)
		{
			for (auto& Actor : SelectedActors)
			{
				Actor->Deselect();
			}
		}
		
		SelectedActors.Empty();
		
		CurrentPoint=GetMousePosition();

		DrawRect(FLinearColor(0.f,0.f,1.f,0.1f),InitialPoint.X,InitialPoint.Y,
			CurrentPoint.X-InitialPoint.X,CurrentPoint.Y-InitialPoint.Y);

		GetActorsInSelectionRectangle<ARTSCharacter>(InitialPoint,CurrentPoint,SelectedActors,false);

		int32 ArraySize = SelectedActors.Num();

		if (ArraySize>0)
		{
			for (int32 i=0; i<ArraySize; ++i)
			{
				if (SelectedActors[i]->GetAttitude() == EAttitude::Friendly)
				{
					SelectedActors[i]->Select();
				}
				else
				{
					SelectedActors.RemoveAt(i);
					--i;
					--ArraySize;
				}
			}
		}
	}
}

FVector2D AMyHUD::GetMousePosition() const
{
	float X;
	float Y;

	GetOwningPlayerController()->GetMousePosition(X,Y);

	return FVector2D(X,Y);
}
