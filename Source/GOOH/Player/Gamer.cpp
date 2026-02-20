#include "Gamer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "../UI/GameUI.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AGamer::AGamer()
{
	PrimaryActorTick.bCanEverTick = true;

	MoveComponent = GetCharacterMovement();
	MoveComponent->MaxWalkSpeed = GamerSpeed.Normal;
	MoveComponent->JumpZVelocity = 580.f;
	MoveComponent->GravityScale = 1.7f;

	FullMesh = GetMesh();
	FullMesh->bCastHiddenShadow = true;

	FovCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FovCamera"));
	BodyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("BodyCamera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));

	BodyCamera->SetupAttachment(BodyMesh);
	FovCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	BodyMesh->SetupAttachment(RootComponent);
	SpringArm->SetupAttachment(RootComponent);

	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	BodyCamera->bUsePawnControlRotation = true;

	BodyMesh->CastShadow = false;
	BodyMesh->SetOnlyOwnerSee(true);
	BodyMesh->SetOwnerNoSee(true);
}

void AGamer::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = Cast<APlayerController>(Controller);
	if(PlayerController) {
		if (UEnhancedInputLocalPlayerSubsystem* System = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) {
			System->AddMappingContext(GamerContext, 0);
		}
	}

	if (GameWidgetClass) {
		GameWidget = CreateWidget<UGameUI>(GetWorld(), GameWidgetClass);
		GameWidget->AddToViewport();
	}
}

void AGamer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Action.bIsSprinting && !Action.bIsJumping) {
		float Stamina = GamerStats.Stamina - .1f * DeltaTime;
		SetStamina(Stamina);
	}

	if (GamerStats.Stamina < 1.0f && !Action.bIsSprinting) {
		float Stamina = GamerStats.Stamina + .1f * DeltaTime;
		SetStamina(Stamina);
	}
}

void AGamer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGamer::Look);
		Input->BindAction(ScrollViewAction, ETriggerEvent::Triggered, this, &AGamer::ScrollView);
		Input->BindAction(SwitchViewAction, ETriggerEvent::Started, this, &AGamer::SwitchView);

		Input->BindAction(MoveAction, ETriggerEvent::Started, this, &AGamer::Walk);
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGamer::Move);
		Input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AGamer::Idle);

		Input->BindAction(SprintAction, ETriggerEvent::Started, this, &AGamer::Sprint);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGamer::EndSprint);

		Input->BindAction(SneakAction, ETriggerEvent::Started, this, &AGamer::Sneak);
		Input->BindAction(SneakAction, ETriggerEvent::Completed, this, &AGamer::EndSneak);

		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &AGamer::BeginJump);

		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &AGamer::Attack);
		Input->BindAction(AttackAction, ETriggerEvent::Completed, this, &AGamer::StopAttack);

		Input->BindAction(MenuWindowAction, ETriggerEvent::Started, this,&AGamer::OpenMenu);
	}
}

void AGamer::Idle() {
	if (Action.bIsJumping) {
		bWasWalkStarted = false;
		Action.bIsWalking = false;
		Action.bIsSprinting = false;
		Action.bIsIdle = true;
		return;
	}
	SetAction(ECurrentAction::Idle);
}

void AGamer::Walk() {
	if (bActiveSprint) {
		SetAction(ECurrentAction::Sprinting);
		return;	 
	}
	if (bActiveSneak) {
		SetAction(ECurrentAction::Sneaking);
		return;
	}
	SetAction(ECurrentAction::Walking);
}

void AGamer::Move(const FInputActionValue& Value)
{
	if (bIsGameFrozen) return;
	if (!bWasWalkStarted) { 
		Walk();
		return;
	}
	const FVector2D VectorValue = Value.Get<FVector2D>();
	const FRotator MRotator = Controller->GetControlRotation();
	const FRotator YawRotator(0.f, MRotator.Yaw, 0.f);
	const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector,VectorValue.Y);
	AddMovementInput(RightVector,VectorValue.X);	
}

void AGamer::BeginJump()
{
	if (bIsFalling || bIsGameFrozen) return;
	if (GamerStats.Stamina < .1f) {
		MoveComponent->JumpZVelocity = 300.f;
		bSmallJump = true;
	}
	else {
		float Stamina = GamerStats.Stamina - .1f;
		SetStamina(Stamina);
	}
	Action.bIsJumping = true;
	Jump();
}

void AGamer::EndJump()
{
	StopJumping();
	if (bSmallJump) {
		MoveComponent->JumpZVelocity = 620.f;
		bSmallJump = false;
	}
	Action.bIsJumping = false;
	if (!Action.bIsIdle) {
		if (Action.bIsSprinting) {
			SetAction(ECurrentAction::Sprinting);
			return;
		}
		if (Action.bIsSneaking) {
			SetAction(ECurrentAction::Sneaking);
			return;
		}
		if (Action.bIsWalking) {
			SetAction(ECurrentAction::Walking);
			return;
		}
		return;
	}
	SetAction(ECurrentAction::Idle);

}

void AGamer::Sneak()
{
	bActiveSneak = true;
	bActiveSprint = false;
	if (!bWasWalkStarted) return;
	SetAction(ECurrentAction::Sneaking);
}

void AGamer::EndSneak()
{
	bActiveSneak = false;
	if (!bWasWalkStarted || !Action.bIsSneaking) return;
	Action.bIsSneaking = false;
	SetAction(ECurrentAction::Walking);
}

void AGamer::Sprint()
{	
	bActiveSneak = false;	
	bActiveSprint = true;
	if (!bWasWalkStarted) return;
	SetAction(ECurrentAction::Sprinting);
}
void AGamer::EndSprint()
{
	bActiveSprint = false;
	if (!Action.bIsSprinting) return;
	Action.bIsSprinting = false;
	SetAction(ECurrentAction::Walking);
 }

void AGamer::Look(const FInputActionValue& Value)
{
	if (bIsGameFrozen) return;
	const FVector2D ValueVector = Value.Get<FVector2D>();
	AddControllerPitchInput(ValueVector.Y * -1);
	AddControllerYawInput(ValueVector.X);
}

void AGamer::ScrollView(const FInputActionValue& Value)
{
	float Length = SpringArm->TargetArmLength;
	////The -1.f is controlling direction;
	float Direct = Value.Get<float>() * -1.f;
	//if (Direct == -1.f && Length >= 350.f) return;
	const float SpeedValue = 25.f;
	Length += SpeedValue * Direct;
	if (Length >= MaxArmLength || Length <= MinArmLength) return;
	SpringArm->TargetArmLength = Length;
}

void AGamer::Attack() {
	if (bIsGameFrozen) return;
	TObjectPtr<UCameraComponent> Camera;
	FVector Start;
	FRotator Rotation;
	float Distance = 300.f;
	if (BodyCamera->IsActive()) {
		Camera = BodyCamera;
		Start = Camera->GetComponentLocation();
	} else {
		Distance += 300.f;
		Camera = FovCamera;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(Start,Rotation);
	}
	FVector ForwardVector = Camera->GetForwardVector();
	float Range = bIsCarringWeapon ? 2000.f : Distance;
	FVector End = Start + (ForwardVector * Range);

	FHitResult Hit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, TraceChannel, QueryParams);
	//DrawDebugLine(GetWorld(), Start, End, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 1.0f);
	
	if (Hit.bBlockingHit && IsValid(Hit.GetActor())) {
		AActor* HitActor = Hit.GetActor();
		if (HitActor) {
			if (HitActor->ActorHasTag(FName("Destructable"))) {
				HitActor->Destroy();
			}
		}
	}
	if (!bIsCarringWeapon) {
		if (GamerStats.Stamina > 0.1f) {
			float Value = GamerStats.Stamina - 0.05f;
			SetStamina(Value);
			//Animation aren't the most important things, so ...
			if (AttackMontage) {
				bIsAttacking = true;
				PlayAnimMontage(AttackMontage);
			}
		}
	}
}

void AGamer::StopAttack()
{
	bIsAttacking = false;
}

void AGamer::OpenMenu()
{
	GameWidget->SetWidgetByIndex(0);
	//SetGamePause();
}

void AGamer::InteractEvent()
{
	if (!bInteractionEnable) return;
}

void AGamer::SwitchView()
{
	switch (ActiveCamera) {
	case EActiveCamera::Fov:
		ActiveCamera = EActiveCamera::Body;
		FovCamera->SetActive(false);
		BodyCamera->SetActive(true);
		FullMesh->SetOwnerNoSee(true);
		BodyMesh->SetOwnerNoSee(false);
		break;
	case EActiveCamera::Body:
		ActiveCamera = EActiveCamera::Fov;
		BodyCamera->SetActive(false);
		FovCamera->SetActive(true);
		FullMesh->SetOwnerNoSee(false);
		BodyMesh->SetOwnerNoSee(true);
	}
}

void AGamer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bIsFalling = false;
	if (Action.bIsJumping) {
		EndJump();
	}

	float EndTimmer = GetWorld()->GetTimeSeconds();
	float Dif = EndTimmer - FallingTimeStart;
	if (Dif < 1.1f) return;
	float Health = GamerStats.Health;
	Health -= Dif * 0.3f;
	SetHealth(Health);
	if (Health <= 0.f) {
		SetStamina(0.0f);
		GameWidget->SetWidgetByIndex(1);
		ActionReset();
		//bIsGameFrozen = true;
	}
}

void AGamer::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCutomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PrevCutomMode);
	if (MoveComponent->IsFalling()) {
		bIsFalling = true;
		FallingTimeStart = GetWorld()->GetTimeSeconds();
	}
}

void AGamer::SetStamina(float Value)
{
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

void AGamer::SetHealth(float Value)
{
	float Current = GamerStats.Health;
	GamerStats.Health = Value;
	GameWidget->SetHealthBar(Value);
}

void AGamer::RestartGame()
{
	GameWidget->BackToGameWidget();
	SetStamina(1.f);
	SetHealth(1.f);
}
