#pragma once

#include "CoreMinimal.h"
#include "WheeledVehiclePawn.h"
#include "Car.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class GOOH_API ACar : public AWheeledVehiclePawn
{
	GENERATED_BODY()

public:
	ACar(const FObjectInitializer& ObjectInitializer);
public:

	UPROPERTY(EditAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> WideCamera;

	UPROPERTY(EditAnyWhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArm;
};
