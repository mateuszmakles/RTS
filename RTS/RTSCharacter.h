// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "RTSCharacter.generated.h"

UENUM()
enum class EAttitude : int32 // fix later
{
	Friendly,
	Ally,
	Enemy,
	Neutral
};

UENUM()
enum class EState : int32
{
	Idle,
	Move,
	Hold,
	Patrol,
	Attack
};

UCLASS(Blueprintable)
class ARTSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARTSCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }

	TArray<TWeakObjectPtr<ARTSCharacter>> SquadMembers;

	void Select() const;
	
	void Deselect() const;

	const EAttitude& GetAttitude() const;

	const EState& GetState() const;

	void ChangeState(const EState& NewState);

	void Move(const FVector& MoveDestination);

	void Attack(const TWeakObjectPtr<ARTSCharacter>& Target);

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	UPROPERTY(EditAnywhere, Category = "Unit Properties")
	EAttitude Attitude = EAttitude::Friendly;

	UPROPERTY(EditAnywhere, Category = "Unit Properties")
	EState State = EState::Idle;

	FVector Destination;
	
	TWeakObjectPtr<ARTSCharacter> EnemyTargetActor;
};

