#pragma once


#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "p2/ui/3Dui/HUD/Widget/MinimapWidgetData/EMarkerType.h"

#include "MiniMapRegisteredActor.generated.h"

UCLASS()
class P2_API AMiniMapRegisteredActor : public AActorBase {
    GENERATED_BODY()

public:
    AMiniMapRegisteredActor();

protected:
    
    // -- TO BE OVERRIDEN ! -- 
    virtual EMarkerType GetMarkerType(){
        return EMarkerType::EEnemy;
    }



    void UnRegisterFromMiniMap();
    void RegisterToMiniMap();
    
    //refresh if needed
    void UpdateMiniMapRegistration();

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    


private:



};
