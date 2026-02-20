#include "GameOver.h"
#include "Components/Button.h"
#include "../Player/Gamer.h"
#include <Kismet/GameplayStatics.h>



void UGameOver::NativeConstruct()
{
	Super::NativeConstruct();
	if (RestartBtn) {
		RestartBtn->OnClicked.AddDynamic(this, &UGameOver::OnClickRestart);
	}
}

void UGameOver::OnClickRestart()
{
	if(TObjectPtr<AGamer> Player = Cast<AGamer>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0))) {
		Player->RestartGame();
	}
}
