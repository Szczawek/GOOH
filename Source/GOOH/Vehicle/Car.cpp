#include "Car.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

ACar::ACar(const FObjectInitializer& ObjectInitializer) 
	:Super(ObjectInitializer)
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	WideCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("WideCamera"));
	WideCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->TargetArmLength = 300.f;
}
