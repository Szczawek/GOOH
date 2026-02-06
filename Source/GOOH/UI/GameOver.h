#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "../Player/Gamer.h"
#include "GameOver.generated.h"

UCLASS()
class GOOH_API UGameOver : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UButton> RestartGameBtn;
public:
	UFUNCTION()
	void OnClickRestart();
protected:
	AGamer* Player;
};
