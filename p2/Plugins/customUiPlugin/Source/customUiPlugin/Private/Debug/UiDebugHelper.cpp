#include "UiDebugHelper.h"


void UiDebugHelper::logMessage(FString printing){
	UE_LOG(LogTemp, Log, TEXT("%s"), *printing); //for whatever reason the string must be dereferenced
}


void UiDebugHelper::showScreenMessage(FString printing, FColor color){
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, color, printing);
	}
}
