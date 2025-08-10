#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <InputAction.h>
#include <Blueprint/UserWidget.h>
#include <Camera/CameraComponent.h>
#include <Components/CanvasPanel.h>
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <InputMappingContext.h>
#include "Gamer.generated.h"


UENUM()
enum class ECurrentAction : uint8 {
	Jumping,
	Sprinting,
	Walking,
	Sneaking,
	Idle
};

UENUM()
enum class EActiveWeapon : uint8 {
	Fist,
	Weapon,
	WhiteWeapon
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
	UInputMappingContext* GamerContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* FovCamera;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCharacterMovementComponent* MoveComponent;
	
	//#Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> GameWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* GameWidget;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* MenuWidget;

public: 

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Action")
	FGamerAction Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	ECurrentAction CurrentAction = ECurrentAction::Idle;

	// Development Option, to change;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type of weapon")
	EActiveWeapon ActiveWeapon = EActiveWeapon::WhiteWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gamer Stats")
	FGamerStats GamerStats;

	void Move(const FInputActionValue& Value); 
	void Look(const FInputActionValue& Value);
	void BeginJump();
	void EndJump();
	void Sprint();
	void EndSprint();
	void Sneak();
	void Idle();
	void Walk();
	void EndSneak();
	void Attack();
	void MenuWindow();

protected: 
	FTimerHandle JumpTimmer;

public:
	UFUNCTION()
	void SetAction(ECurrentAction ActionType) {
		Action.bIsJumping = false;
		Action.bIsSneaking = false;
		Action.bIsSprinting = false;
		Action.bIsWalking = false;
		Action.bIsIdle = false;

		if (ActionType != ECurrentAction::Jumping) { 
			CurrentAction = ActionType;
		};
		
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
		case ECurrentAction::Jumping:
			Action.bIsJumping = true;
			MoveComponent->MaxWalkSpeed = GamerSpeed.Normal;
			break;
		case ECurrentAction::Idle:
			Action.bIsIdle = true;
			break;
		}
	}

	struct {
		float Normal = 500.f;
		float Sprint = 900.f;
		float Sneak = 300.f;
	} GamerSpeed;

};