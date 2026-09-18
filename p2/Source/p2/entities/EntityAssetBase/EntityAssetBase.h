#pragma once

#include "CoreMinimal.h"


#include "AssetEnumCollection/assetEnums/EntityAsset.h"
#include "p2/entetiesBase/miniMapRegisteredActor/MiniMapRegisteredActor.h"
#include "p2/entityManager/EntityManager.h"

#include "EntityAssetBase.generated.h"

/// is not registered to minimap by default.
/// Special class for Entities which are not complete (not Alive.) But still need to be added
/// to the custom entity manager gc, and have nice methods like TMakeInstance.
UCLASS()
class P2_API AEntityAssetBase : public AMiniMapRegisteredActor {
    GENERATED_BODY()

public:

    template <typename T>
    static T *TMakeInstance(EntityAsset type){
        if(EntityManager *ptr = EntityManager::instance()){
            T *created = ptr->TESpawnActor<T, EntityAsset>(type);
            if(AEntityAssetBase *casted = Cast<AEntityAssetBase>(created)){
                casted->enableActiveStatus(true);
            }
            return created;
        }
        return nullptr;
    }



    
    virtual void BeginPlay() override;

    virtual void despawn();
    virtual void enableActiveStatus(bool enable);

    virtual EntityAsset GetAssetType(){
        return EntityAsset::None;
    }

private:
    bool activated = true;
};