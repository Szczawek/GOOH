#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Player/Gamer.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/Border.h>
#include <Components/VerticalBox.h>
#include "MenuUI.generated.h"

UCLASS()
class GOOH_API UMenuUI : public	UUserWidget
{
	GENERATED_BODY()

protected: 
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> MenuBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> SettingsBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> SettingsText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> ResumeBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ResumeText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UButton> QuitBtn;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> QuitText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional));
	UCanvasPanel* Canvas;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional));
	UBorder* Background;

	UPROPERTY()
	AGamer* GamerRef;

public:
	UFUNCTION()
	void OnClickResume();
	
	UFUNCTION()
	void OnClickSettings();
	
	UFUNCTION()
	void OnClickQuit();
}; 
