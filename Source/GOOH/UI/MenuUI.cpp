// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuUI.h"

void UMenuUI::NativeConstruct()
{
	Super::NativeConstruct();
	if (ResumeGameBtn) {
		if (ResumeText) {
			ResumeText->SetText(FText::FromString(TEXT("Resume")));
			ResumeGameBtn->SetBackgroundColor(FLinearColor::Red);
		}
	}
	if (SettingsBtn) {
		if (SettingsText) {
			SettingsText->SetText(FText::FromString(TEXT("Settings")));
			SettingsBtn->SetContent(SettingsText);
		}
	}
}

