#pragma once

#include "CoreMinimal.h"

#include "ColorAsset.generated.h"


/// @brief will store ui colors for all buttons to be applied to automatically.
/// (see p2 implementation)
UCLASS(BlueprintType)
class CUSTOMUIPLUGINBASE_API UColorAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	// Beispiel: UI-Farben
	UPROPERTY(EditAnywhere, Category="Colors")
	FLinearColor Primary;

	UPROPERTY(EditAnywhere, Category="Colors")
	FLinearColor Hovered;

	UPROPERTY(EditAnywhere, Category="Colors")
	FLinearColor Pressed;

    void CopyColors(
        FLinearColor &PrimaryIn,
        FLinearColor &HoveredIn,
        FLinearColor &PressedIn
    ){
        PrimaryIn = Primary;
        HoveredIn = Hovered;
        PressedIn = Pressed;
    }
};