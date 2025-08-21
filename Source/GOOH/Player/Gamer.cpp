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
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller)) {
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

// Called to bind functionality to input
void AGamer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGamer::Move);
		Input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AGamer::Idle);
		Input->BindAction(MoveAction, ETriggerEvent::Started, this, &AGamer::Walk);
		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &AGamer::BeginJump);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGamer::Look);
		Input->BindAction(SprintAction, ETriggerEvent::Started, this, &AGamer::Sprint);
		Input->BindAction(ScrollViewAction, ETriggerEvent::Triggered, this, &AGamer::ScrollView);
		Input->BindAction(SwitchViewAction, ETriggerEvent::Started, this, &AGamer::SwitchView);
		//Description:: Canceling the sprint event is activating the walk event, because when the character stops moving, it will
		// automatically change its status to idle.When the character is in idle status, all actions will be canceled, so cancelling 
		// the sprint event will not stop the character, but will allow the character to move forward.
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &AGamer::EndSprint);
		Input->BindAction(SneakAction, ETriggerEvent::Started, this, &AGamer::Sneak);
		Input->BindAction(SneakAction, ETriggerEvent::Completed, this, &AGamer::EndSneak);
		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &AGamer::Attack);

		Input->BindAction(MenuWindowAction, ETriggerEvent::Started, this, &AGamer::MenuWindow);
	}
}

void AGamer::Move(const FInputActionValue& Value)
{
	const FVector2D VectorValue = Value.Get<FVector2D>();
	const FRotator MRotator = Controller->GetControlRotation();
	const FRotator YawRotator(0.f, MRotator.Yaw, 0.f);
	const FVector ForwardVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightVector = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVector,VectorValue.Y);
	AddMovementInput(RightVector,VectorValue.X);	
}

// Queue In Movements component is temporary 
// Queue In Movements component is temporary 

void AGamer::Idle() {
	if (Action.bIsJumping){
		bQueueIsActive = true;
		QueueAction = ECurrentAction::Idle;
		return;
	}	
	if (!Action.bIsWalking) return;
	SetAction(ECurrentAction::Idle);
}

void AGamer::Walk() {
	if (Action.bIsJumping) {
		bQueueIsActive = true;
		QueueAction = ECurrentAction::Walking;
		return;
	}
	if (!Action.bIsIdle) return;
	SetAction(ECurrentAction::Walking);
}

void AGamer::EndSneak()
{
	if (Action.bIsJumping && !bQueueIsActive) {
		QueueAction = ECurrentAction::Walking;
		bQueueIsActive = true;
		return;
	}
	if (!Action.bIsSneaking) return;
	SetAction(ECurrentAction::Walking);
}

void AGamer::Sprint()
{	
	if (!Action.bIsWalking) return;
	if (bQueueIsActive) bQueueIsActive = false;
	SetAction(ECurrentAction::Sprinting);
}

void AGamer::EndSprint()
{
	if (Action.bIsJumping && !bQueueIsActive && MoveComponent->Velocity.Size() > 500.f) {
		QueueAction = ECurrentAction::Walking;
		bQueueIsActive = true;
		return;
	}
	if (!Action.bIsSprinting) return;
	SetAction(ECurrentAction::Walking);
 }

void AGamer::Sneak()
{
	if (!Action.bIsWalking) return;
	if (bQueueIsActive) bQueueIsActive = false;
	SetAction(ECurrentAction::Sneaking);
}

void AGamer::Look(const FInputActionValue& Value)
{
	const FVector2D ValueVector = Value.Get<FVector2D>();
	AddControllerPitchInput(ValueVector.Y * -1);
	AddControllerYawInput(ValueVector.X);
}

void AGamer::ScrollView(const FInputActionValue& Value)
{
	const FVector2D ValueVector = Value.Get<FVector2D>();
}

void AGamer::BeginJump()
{
	if (Action.bIsJumping || GamerStats.Stamina <= 0.0f) return;
	SetAction(ECurrentAction::Jumping);
	Jump();
}

void AGamer::EndJump()
{
	StopJumping();
	if (bQueueIsActive) {
		bQueueIsActive = false;
		SetAction(QueueAction);
		return;
	}
	SetAction(CurrentAction);
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
	if (!bIsMenuOpened) {
		bIsMenuOpened = true;
		GameWidget->SetWidgetOnDisplay(0);
		return;
	}
	GameWidget->TakeWidgetFromDisplay();
	bIsMenuOpened = false;
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

	if (Action.bIsJumping) {
		EndJump();
	}

	float EndTimmer = GetWorld()->GetTimeSeconds();
	float Dif = EndTimmer - FallingTimeStart;
	float Health = GamerStats.Health;
	if (Dif >= .9f) {
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
		FallingTimeStart = GetWorld()->GetTimeSeconds();
	}
}
