#pragma once

#include "CoreMinimal.h"
#include "p2/PlateCarrier/MagSocket/MagSocketType.h"
#include "PlateCarrierMagSocket.generated.h"

//----> TODO DERIVE FROM SCENE COMPONENT! 

//UCLASS()
UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2_API UPlateCarrierMagSocket : public USceneComponent{
    GENERATED_BODY()


public:

    UPROPERTY(EditAnywhere, Category = "_OPTIONS")
    EMagSocketType socketType = EMagSocketType::AR_MAG;


    bool HasMag();
    bool IsType(EMagSocketType typeIn);

    USceneComponent *GetPouchedMag();

    

    //replace current ptr, no destory of original mag
    void InsertMagSnap(USceneComponent *mag);

    //original mag gets destroyed if available and
    //replaced with new duplicate of the passed scene component
    void ReplaceMag(USceneComponent *magIn);

    FVector MeshOffsetFromOrigin(){
        return offsetRemoved;
    }

    void RemoveOffset(USceneComponent *comp);

private:

    UPROPERTY()
    USceneComponent *insertedMag = nullptr;

    FVector offsetRemoved;

    void DestroyMag();

    USceneComponent *Duplicate(USceneComponent *OriginalMeshComp);
    USceneComponent *Duplicate(USceneComponent *root, USceneComponent *OriginalMeshComp);
    USceneComponent *DuplicateStaticMesh(USceneComponent *root, UStaticMeshComponent *OriginalMeshComp);
    USceneComponent *DuplicateSkeletal(
        USceneComponent *Root,
        USkeletalMeshComponent *OriginalMeshComp
    );

    void DisableCollision();
    
    void GetBoundsCenter(USceneComponent *comp, FVector &center);
};