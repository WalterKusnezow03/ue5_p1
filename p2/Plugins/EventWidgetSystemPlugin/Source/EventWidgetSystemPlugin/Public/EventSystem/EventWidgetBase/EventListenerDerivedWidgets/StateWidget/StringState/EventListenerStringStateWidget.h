// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"
#include "EventWidgetSystemPlugin/Public/EventSystem/EventWidgetBase/EventListenerDerivedWidgets/StateWidget/IndexState/EventListenerIndexStateWidget.h"
#include "EventListenerStringStateWidget.generated.h"

/// MAPPED INDEX BY STRING WIDGET
/// @brief Will show a UWidget (for example a image) based on a index
/// BUT the index must be mapped by a string!
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class EVENTWIDGETSYSTEMPLUGIN_API UEventListenerStringStateWidget : public UEventListenerIndexStateWidget {
    GENERATED_BODY()

public:

protected:

    //extracts the second index of buffer as MAPPED number by string!
    virtual int ExtractPayload(TArray<FString> &message) override;
    int ConvertToIndex(const FString &payload);

    UPROPERTY(EditAnywhere, Category = "_EventOption")
    TMap<FName, int32> TranslationMap;

};
