#pragma once


#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "MinimapPlugin/Public/Widget/MinimapWidgetData/EMarkerType.h"
#include "StoragePlugin/Storage/ImageData/Image/Image.h"

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

    virtual void Tick(float deltatime) override;

public:
    
private:
    bool queuedForAddToMinimap = false;
    bool queuedForRemoveFromMinimap = false;
};
