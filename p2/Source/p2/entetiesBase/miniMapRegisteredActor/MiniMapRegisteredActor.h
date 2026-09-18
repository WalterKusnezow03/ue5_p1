#pragma once


#include "CoreMinimal.h"
#include "GameCore/util/ActorBase/ActorBase.h"
#include "p2/ui/3Dui/HUD/Widget/MinimapWidgetData/EMarkerType.h"
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
    // -- Can be overriden, optional! --
    //can provide custom image data if needed (for minimap)
    //ptr must always be valid or nullptr!
    virtual Image *GetCustomMarkerImageData(){
        return nullptr;
    }

private:
    bool queuedForAddToMinimap = false;
    bool queuedForRemoveFromMinimap = false;
};
