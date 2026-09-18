#pragma once

#include "CoreMinimal.h"
#include "p2/entities/EntityAssetBase/EntityAssetBase.h"

#include "FlagActor.generated.h"

//is not registered to minimap, but might be in future
UCLASS()
class P2_API AFlagActor : public AEntityAssetBase {
    GENERATED_BODY()

public:
    static AFlagActor *MakeInstance();
    static AFlagActor *MakeInstanceAt(const FVector &pos);

    virtual void BeginPlay() override;

    virtual EntityAsset GetAssetType() override {
        return EntityAsset::Flag;
    }

private:

};