#include <Kismet/GameplayStatics.h>
#include "GameOver.h"


void UGameOver::NativeConstruct()
{
	Super::NativeConstruct();
	if (RestartGameBtn) {
		RestartGameBtn->OnClicked.AddDynamic(this, &UGameOver::OnClickRestart);
	}
	Player = Cast<AGamer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void UGameOver::OnClickRestart()
{
	Player->RestartGame();
}
