#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuUI.generated.h"

class UButton;
class AGamer;

UCLASS()
class GOOH_API UMenuUI : public	UUserWidget
{
	GENERATED_BODY()

protected: 
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> SettingsBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ResumeBtn;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> QuitBtn;

	UPROPERTY()
	AGamer* GamerRef;

public:
	UFUNCTION()
	void OnClickResume();
	
	UFUNCTION()
	void OnClickSettings();
	
	UFUNCTION()
	void OnClickQuit();

	UFUNCTION()
	void OpenSettings();
}; 
