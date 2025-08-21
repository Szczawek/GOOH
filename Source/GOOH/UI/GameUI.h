
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include <Components/CanvasPanel.h>
#include <Components/WidgetSwitcher.h>
#include <Components/ProgressBar.h>
#include "GameUI.generated.h"


UCLASS()
class GOOH_API UGameUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGameUI(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UTextBlock> UserName;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UProgressBar> HealthBar;
	 
	UPROPERTY(BlueprintReadWrite, meta=(BindWidgetOptional))
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UCanvasPanel> CanvasPanel;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widgets")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UFUNCTION()
	void SetStaminaBar(float Value) const;

	UFUNCTION()
	void SetHealthBar(float Value) const;

	UFUNCTION()
	void SetWidgetOnDisplay(int8 Index) const;

	UFUNCTION()
	void TakeWidgetFromDisplay() const;
};
