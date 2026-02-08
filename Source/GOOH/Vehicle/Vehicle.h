#pragma once

#include "CoreMinimal.h"
#include "ChaosModularVehicle/ModularVehicleClusterPawn.h"
#include "Vehicle.generated.h"

UCLASS()
class GOOH_API AVehicle : public AModularVehicleClusterPawn
{
	GENERATED_BODY()

public:
	AVehicle(const FObjectInitializer& ObjectInitializer);
	
};
