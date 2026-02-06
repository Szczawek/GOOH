#include "MenuUI.h"
#include <Kismet/GameplayStatics.h>

void UMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	if (ResumeBtn) {
		ResumeBtn->OnClicked.AddDynamic(this, &UMenuUI::OnClickResume);
		if (ResumeText) {
			ResumeText->SetText(FText::FromString(TEXT("Resume")));
		}
	}
	if (QuitBtn) {
		QuitBtn->OnClicked.AddDynamic(this, &UMenuUI::OnClickQuit);
		if (QuitText) {
			QuitText->SetText(FText::FromString(TEXT("Quit")));
		}
	}
	if (SettingsBtn) {
		if (SettingsText) {
			SettingsText->SetText(FText::FromString(TEXT("Settings")));
		}
	}
	if (Background) {
		Background->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
	GamerRef = Cast<AGamer>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
}

void UMenuUI::OnClickSettings()
{
}

void UMenuUI::OnClickQuit()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GamerRef->PlayerController,EQuitPreference::Quit,true);
}

void UMenuUI::OnClickResume()
{                                                                                            
	GamerRef->SetMenuWindow();
}

