// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyHUD.h"
#include "RTSCharacter.h"
#include "Kismet/GameplayStatics.h"

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AMyPlayerController::BeginPlay()
{
	HUD=Cast<AMyHUD>(GetHUD());

	TArray<AActor*> TempEnemies;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ARTSCharacter::StaticClass(),TempEnemies);

	TArray<ARTSCharacter*> Enemies;

	for (auto& Element : TempEnemies)
	{
		Enemies.Add(Cast<ARTSCharacter>(Element));
	}

	int32 EnemyArraySize = Enemies.Num();
				
	if (EnemyArraySize>0)
	{
		for (int32 i=0; i<EnemyArraySize; ++i)
		{
			if (Enemies[i]->GetAttitude() != EAttitude::Enemy)
			{
				Enemies.RemoveAt(i);
				--i;
				--EnemyArraySize;
			}
		}

		if (EnemyArraySize>1)
		{
			for (int32 i=0; i<EnemyArraySize; ++i)
			{
				for (int32 j=0; j<EnemyArraySize; ++j)
				{
					if (j != i && (Enemies[i]->GetActorLocation() - Enemies[j]->GetActorLocation()).Size() < 500.f)
					{
						int32 Index;

						if (!Enemies[i]->SquadMembers.Find(Enemies[j],Index))
						{
							int32 SquadSize = Enemies[i]->SquadMembers.Num();
							
							for (int32 k=0; k<SquadSize; ++k)
							{
								if (!Enemies[i]->SquadMembers[k]->SquadMembers.Find(Enemies[j],Index))
								{
									Enemies[i]->SquadMembers[k]->SquadMembers.Add(Enemies[j]);
									Enemies[j]->SquadMembers.Add(Enemies[i]->SquadMembers[k]);
								}
							}
							
							Enemies[i]->SquadMembers.Add(Enemies[j]);
							Enemies[j]->SquadMembers.Add(Enemies[i]);
						}
					}
				}
			}
		}
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("LeftMouseClick",IE_Pressed,this,&AMyPlayerController::SelectionPressed);
	InputComponent->BindAction("LeftMouseClick",IE_Released,this,&AMyPlayerController::SelectionReleased);
	
	InputComponent->BindAction("RightMouseClick",IE_Pressed,this,&AMyPlayerController::MoveOrAttack);
}

void AMyPlayerController::SelectionPressed()
{
	if (HUD.IsValid())
	{
		HUD->InitialPoint=HUD->GetMousePosition();

		HUD->bLeftMousePressed=true;
	}
}

void AMyPlayerController::SelectionReleased()
{
	if (HUD.IsValid())
	{
		HUD->bLeftMousePressed=false;

		SelectedActors=HUD->SelectedActors;
	}
}

void AMyPlayerController::MoveOrAttack()
{
	const int32 ArraySize=SelectedActors.Num();
	
	if (ArraySize>0)
	{
		FHitResult Hit;

		GetHitResultUnderCursor(ECollisionChannel::ECC_Pawn,false,Hit);

		if (Hit.Actor.IsValid())
		{
			bool bAttack = false;
			
			const auto& EnemyActor = Cast<ARTSCharacter>(Hit.Actor);

			

			if (EnemyActor)
			{
				if (EnemyActor->GetAttitude() == EAttitude::Enemy)
				{
					bAttack=true;
				}
			}

			if (bAttack)
			{
				auto Enemies = EnemyActor->SquadMembers;

				Enemies.Add(EnemyActor);

				int32 EnemyArraySize = Enemies.Num();

				Enemies.Swap(0,EnemyArraySize-1);

				UE_LOG(LogTemp, Warning, TEXT("Enemy Array Size: %d"), EnemyArraySize);

				if (EnemyArraySize>0)
				{
					for (int32 i=0, j=0; i<ArraySize; ++i, ++j)
					{
						if (j >= EnemyArraySize)
						{
							j=0;
						}

						SelectedActors[i]->Attack(Cast<ARTSCharacter>(Enemies[j]));
					}
				}
			}
			else
			{
				for (int32 i=0; i<ArraySize; ++i)
				{
					FVector Destination=Hit.Location;

					if (ArraySize>1)
					{
						// Random placement
						//int32 Offset=ArraySize*50;
						//Destination+=FVector(FMath::RandRange(-Offset,Offset),FMath::RandRange(-Offset,Offset),0);
						//Destination+=FVector(0,(-100*(ArraySize-1))+(200*i),0);

						int32 Offset=150;
						
						Destination+=FVector(i % 2 * Offset - Offset/2,i / 2 * Offset - Offset*(ArraySize % 2 + ArraySize-2)/4,0);
					}

					SelectedActors[i]->Move(Destination);
				}
			}
		}
	}
}
