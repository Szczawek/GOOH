#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <Components/VerticalBox.h>
#include "MenuUI.generated.h"

UCLASS()
class GOOH_API UMenuUI : public UUserWidget
{
	GENERATED_BODY()
	
protected: 
	virtual void NativeConstruct() override;

public:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SettingsBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ResumeGameBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> SettingsText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> ResumeText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	TObjectPtr<UVerticalBox> MenuBox;

}; 
