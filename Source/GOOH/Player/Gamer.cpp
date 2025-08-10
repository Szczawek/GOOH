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
		GameWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidgetClass);
		GameWidget->AddToViewport();
	}

	if (MenuWidgetClass) {
		MenuWidget = CreateWidget<UUserWidget>(GetWorld(), GameWidgetClass);
	}
}

// Called every frame
void AGamer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

void AGamer::Idle() {
	SetAction(ECurrentAction::Idle);
}

void AGamer::Walk() {
	if (!Action.bIsIdle) return;
	SetAction(ECurrentAction::Walking);
}

void AGamer::EndSneak()
{
	if (!Action.bIsSneaking) return;
	SetAction(ECurrentAction::Walking);
}

void AGamer::Sprint()
{
	if (!Action.bIsWalking) return;
	SetAction(ECurrentAction::Sprinting);
}

void AGamer::EndSprint()
{
	if (!Action.bIsSprinting) return;
	SetAction(ECurrentAction::Walking);
 }

void AGamer::Sneak()
{
	if (!Action.bIsWalking) return;
	SetAction(ECurrentAction::Sneaking);
}

void AGamer::Look(const FInputActionValue& Value)
{
	const FVector2D ValueVector = Value.Get<FVector2D>();
	AddControllerPitchInput(ValueVector.Y * -1);
	AddControllerYawInput(ValueVector.X);
}

// Change it to Tick mechanizm::
// In beginJUMP set status to leviting, and set logic in Tick to checks if Person was jumping or fell from clif, next logic in the same block is, if person touch the ground 
// then call function End Jump();
void AGamer::BeginJump()
{
	if (Action.bIsJumping) return;
	SetAction(ECurrentAction::Jumping);
	Jump();
	GetWorld()->GetTimerManager().SetTimer(JumpTimmer, this, &AGamer::EndJump, 0.4f, false);
}

void AGamer::EndJump()
{
	if (!Action.bIsJumping) return;
	StopJumping();
	GetWorld()->GetTimerManager().ClearTimer(JumpTimmer);
	SetAction(CurrentAction);
}

void AGamer::Attack() {
	switch (ActiveWeapon) {
	case EActiveWeapon::WhiteWeapon:
	case EActiveWeapon::Fist:
		if (GamerStats.Stamina <= 0.f) return;
			GamerStats.Stamina -= .1f;
			GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Red, TEXT("Test"));
			break;
		case EActiveWeapon::Weapon:
		    break;
	}

}

void AGamer::MenuWindow() {
	if (GameWidget->CanvasPanel) {
		
	}
	if(UCanvasPanel* Panel = Cast<UCanvasPanel>(GameWidget->GetWidgetFromName(TEXT("CanvasPanel")))) {
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Red, TEXT("Panel"));
		Panel->AddChild(MenuWidget);
	}
	GEngine->AddOnScreenDebugMessage(1, 5.0f, FColor::Red, TEXT("Open"));
}