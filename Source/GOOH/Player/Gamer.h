#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <InputAction.h>
#include <Blueprint/UserWidget.h>
#include "../UI/GameUI.h"
#include <Camera/CameraComponent.h>
#include <Components/CanvasPanel.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <InputMappingContext.h>
#include "Gamer.generated.h"


// All new properties should be on end of enum. 
// By default it is loading index 0 #IDLE

UENUM()
enum class ECurrentAction : uint8 {
	Idle,
	Jumping,
	Sprinting,
	Walking,
	Sneaking
};

UENUM()
enum class EActiveWeapon : uint8 {
	Fist,
	Weapon,
	WhiteWeapon
};

UENUM()
enum class EActiveCamera : uint8 {
	Fov,
	Body
};

USTRUCT(BlueprintType)
struct FGamerStats {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float Stamina;

	UPROPERTY(BlueprintReadWrite)
	float Health;

	FGamerStats() : Stamina(1.f), Health(1.f) {}
};

USTRUCT(BlueprintType)
struct FGamerAction {
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	bool bIsJumping;
	UPROPERTY(BlueprintReadWrite)
	bool bIsSprinting;
	UPROPERTY(BlueprintReadWrite)
	bool bIsSneaking;
	UPROPERTY(BlueprintReadWrite)
	bool bIsWalking;
	UPROPERTY(BlueprintReadWrite)
	bool bIsIdle;

	FGamerAction()
		: bIsJumping(false), bIsSprinting(false), bIsSneaking(false), bIsWalking(false), bIsIdle(true)
	{
	}
};

UCLASS()
class GOOH_API AGamer : public ACharacter
{
	GENERATED_BODY()

public:

	AGamer();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* SneakAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* MenuWindowAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* ScrollViewAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputAction* SwitchViewAction;

	UPROPERTY(EditAnywhere, Category = "Inputs")
	UInputMappingContext* GamerContext;

	//Cameras 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* FovCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* BodyCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCharacterMovementComponent* MoveComponent;

	//#Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UGameUI> GameWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UGameUI* GameWidget;

public:
	UPROPERTY(EditAnywhere, Category = "Animations")
	UAnimMontage* AttackMontage;

	// Enums/Structures
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	FGamerAction Action;

	// Development Option, to change;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type of weapon")
	EActiveWeapon ActiveWeapon = EActiveWeapon::WhiteWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamer Stats")
	FGamerStats GamerStats;

	// By default, it should be always on Fov;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera type")
	EActiveCamera ActiveCamera = EActiveCamera::Fov;

	UPROPERTY(EditAnywhere, Category = "Collision")
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_PhysicsBody;

	void Move(const FInputActionValue& Value); 
	void Look(const FInputActionValue& Value);
	void ScrollView(const FInputActionValue& Value);
	void BeginJump();
	void EndJump();
	void Sprint();
	void EndSprint();
	void Sneak();
	void Idle();
	void Walk();
	void EndSneak();
	void Attack();
	void StopAttack();
	void MenuWindow();
	void SwitchView();

protected:
	virtual void Landed(const FHitResult& Hit) override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCutomMode) override;

	float FallingTimeStart;
public:
	UFUNCTION()
	void SetAction(ECurrentAction ActionType) {
		if (bIsGameFrozen) return;
		ResetMovement();
		switch (ActionType) {
			case ECurrentAction::Walking:
				Action.bIsWalking = true;
				MoveComponent->MaxWalkSpeed = GamerSpeed.Normal;
				break;
			case ECurrentAction::Sneaking:
				Action.bIsSneaking = true;
				MoveComponent->MaxWalkSpeed = GamerSpeed.Sneak;
				break;
			case ECurrentAction::Sprinting:
				Action.bIsSprinting = true;
				MoveComponent->MaxWalkSpeed = GamerSpeed.Sprint;
				break;
			case ECurrentAction::Idle:
				ActionReset();	
				break;
			}
	}

	void ResetMovement() {
		bWasWalkStarted = true;
		Action.bIsIdle = false;
		Action.bIsWalking = false;
		Action.bIsSneaking = false;
		Action.bIsSprinting = false;
	}
	struct {
		float Normal = 500.f;
		float Sprint = 900.f;
		float Sneak = 300.f;
	} GamerSpeed;

	void SetStamina(float Value) {
		float Current = GamerStats.Stamina;
		if (Value <= 0.0f) {
			if (Action.bIsSprinting) {
				SetAction(ECurrentAction::Walking);
			}
			return;
		}
		if (Value >= 1.f) return;
		GamerStats.Stamina = Value;
		GameWidget->SetStaminaBar(Value);
	}

	void SetHealth(float Value) {
		float Current = GamerStats.Health;
		if (Current - Value < 0.f) {
			Value = 0.f;
		}
		GamerStats.Health = Value;
		GameWidget->SetHealthBar(Value);
	}

	void ActionReset() {
		bWasWalkStarted = false;
		Action.bIsJumping = false;	
		Action.bIsWalking = false;
		Action.bIsSprinting = false;
		Action.bIsIdle = true;
	}

	bool bSmallJump = false;
	bool bIsAttacking = false;
	bool bActiveSprint = false;
	bool bActiveSneak = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "status")
	bool bWasWalkStarted = false;

	bool bIsFalling = false;
	bool bIsGameFrozen = false;
	bool bIsCarringWeapon = false;
};