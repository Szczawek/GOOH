#include "Gamer.h"
#include "EnhancedInputComponent.h"
#include <EnhancedInputSubsystems.h>

// Sets default values
AGamer::AGamer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MoveComponent = GetCharacterMovement();
	FovCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Fov Camera"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	FovCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	BodyCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Body Camera"));
	BodyCamera->SetupAttachment(GetMesh());
	MoveComponent->MaxWalkSpeed = GamerSpeed.Normal;
}

// Called when the game starts or when spawned
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

// Called every frame
void AGamer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentAction == ECurrentAction::Sprinting && !Action.bIsJumping) {
		float Stamina = GamerStats.Stamina - .1f * DeltaTime;
		SetStamina(Stamina);
	}

	if (GamerStats.Stamina < 1.0f && CurrentAction != ECurrentAction::Sprinting) {
		float Stamina = GamerStats.Stamina + .1f * DeltaTime;
		SetStamina(Stamina);
	}
}

void AGamer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGamer::Look);

		Input->BindAction(MoveAction, ETriggerEvent::Started, this, &AGamer::Walk);
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGamer::Move);
		Input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AGamer::Idle);

		Input->BindAction(SprintAction, ETriggerEvent::Started, this, &AGamer::Sprint);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGamer::EndSprint);

		Input->BindAction(SneakAction, ETriggerEvent::Started, this, &AGamer::Sneak);
		Input->BindAction(SneakAction, ETriggerEvent::Completed, this, &AGamer::EndSneak);

		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &AGamer::BeginJump);

		Input->BindAction(ScrollViewAction, ETriggerEvent::Triggered, this, &AGamer::ScrollView);
		Input->BindAction(SwitchViewAction, ETriggerEvent::Started, this, &AGamer::SwitchView);

		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &AGamer::Attack);

		Input->BindAction(MenuWindowAction, ETriggerEvent::Started, this, &AGamer::MenuWindow);
	}
}

void AGamer::Idle() {
	if (Action.bIsJumping) {
		Action.bIsWalking = false;
		Action.bIsSneaking = false;
		Action.bIsSneaking = false;
		return;
	}
	SetAction(ECurrentAction::Idle);
}

void AGamer::Walk() {
	if (!Action.bIsIdle) return;
	SetAction(ECurrentAction::Walking);
}

//#Idea but not enough
//GetCharacterMovement()->DisabledMovement();  
//It is instand of couple if block, but it isn't worth. It is make the same thing.
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
	if (bIsFalling || bIsGameFrozen || GamerStats.Stamina < .15f) return;
	float Stamina = GamerStats.Stamina - .15f;
	SetStamina(Stamina);
	Action.bIsJumping = true;
	Jump();
}

void AGamer::EndJump()
{
	StopJumping();
	Action.bIsJumping = false;
	if (Action.bIsSprinting) {
		SetAction(ECurrentAction::Sprinting);
		return;
	}
	if (Action.bIsSneaking) {
		SetAction(ECurrentAction::Sneaking);
		return;
	}
	if (Action.bIsWalking) {
		GEngine->AddOnScreenDebugMessage(-1, .5f, FColor::Red, TEXT("Wa"));
		SetAction(ECurrentAction::Walking);
		return;
	}
	SetAction(ECurrentAction::Idle);

}

void AGamer::Sneak()
{
	if (!bWasWalkStarted) return;
	SetAction(ECurrentAction::Sneaking);
}

void AGamer::EndSneak()
{
	if (!bWasWalkStarted) return;
	Action.bIsSneaking = false;
	SetAction(ECurrentAction::Walking);
}

void AGamer::Sprint()
{	
	if (!bWasWalkStarted) return;
	SetAction(ECurrentAction::Sprinting);
}

void AGamer::EndSprint()
{
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
	const FVector2D ValueVector = Value.Get<FVector2D>();
}

void AGamer::Attack() {
	switch (ActiveWeapon) {
		case EActiveWeapon::WhiteWeapon:
		case EActiveWeapon::Fist:
		break;
		case EActiveWeapon::Weapon:
		    break;
	}
}

void AGamer::MenuWindow() {
	if (!bIsGameFrozen) {
		ActionReset();
		bIsGameFrozen = true;
		GameWidget->SetWidgetOnDisplay(0);
		return;
	}
	GameWidget->TakeWidgetFromDisplay();
	bIsGameFrozen = false;
}

void AGamer::SwitchView()
{
	switch(ActiveCamera) {
		case EActiveCamera::Fov:
			ActiveCamera = EActiveCamera::Body;
			FovCamera->SetActive(false);
			BodyCamera->SetActive(true);
			GetMesh()->SetVisibility(false, true);
			GetMesh()->SetCastHiddenShadow(true);
			break;
		case EActiveCamera::Body:
			ActiveCamera = EActiveCamera::Fov;
			BodyCamera->SetActive(false);
			FovCamera->SetActive(true);
			GetMesh()->SetVisibility(true, true);
			GetMesh()->SetCastHiddenShadow(false);
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
	float Health = GamerStats.Health;
	if (Dif >= 1.1f) {
		Health -= .2f;
	} else if (Dif >= 1.4f) {
		Health -= .4f;
	}
	else if (Dif >= 2.2f) {
		Health = 0.f;
	}
	if (Health < 0.0f) {
		Health = 0.0f;
	}
	SetHealth(Health);
}

void AGamer::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PrevCutomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PrevCutomMode);
	if (MoveComponent->IsFalling()) {
		bIsFalling = true;
		FallingTimeStart = GetWorld()->GetTimeSeconds();
	}
}
