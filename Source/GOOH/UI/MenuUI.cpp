#include "MenuUI.h"
#include <Kismet/GameplayStatics.h>
#include "../Player/Gamer.h"
#include "Components/Button.h"

void UMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	if (ResumeBtn) {
		ResumeBtn->OnClicked.AddDynamic(this, &UMenuUI::OnClickResume);
	}
	if (QuitBtn) {
		QuitBtn->OnClicked.AddDynamic(this, &UMenuUI::OnClickQuit);
	}
	if (SettingsBtn) {
		SettingsBtn->OnClicked.AddDynamic(this, &UMenuUI::OpenSettings);
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

void UMenuUI::OpenSettings()
{
}

void UMenuUI::OnClickResume() 
{      
	
}

