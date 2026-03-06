#pragma once

#include "CoreMinimal.h"

#include "IkHumanoidModell/actor/SceneComponent/BoneTransformInterface/BoneTransformInterface.h"
#include "GameCore/MeshGenBase/lodHelper/ProceduralMeshComponentPair.h"
//#include "IkHumanoidModell/Ik/Controller/ControllerSetup/Properties/FSingleLimbProperty.h"
//#include "GameCore/MeshGenBase/MeshData/MeshData.h"

#include "BoneProceduralMeshComponent.generated.h"

class MeshData;
class FSingleLimbProperty;

// class to tick scene - No Additional actors per bones
// turns out to be SLOWER than aactor based bones.
UCLASS()
class IKHUMANOIDMODELL_API UBoneProceduralMeshComponent : public USceneComponent {
    GENERATED_BODY()

public:
    static void MakeInstance(
        FSingleLimbProperty &property, 
        IBoneTransformInterface *bone, 
        int id
    );
protected:
    static UBoneProceduralMeshComponent *TryMakeInstance(AActor *owner);
    static int staticId;
    static FString GetNextBoneName();

public:
    UBoneProceduralMeshComponent();
    
    void Init(
        IBoneTransformInterface *bone, 
        int id,
        FSingleLimbProperty &property
    );

    
    virtual void BeginPlay() override;  // <-- Hier override
    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction *ThisTickFunction
    ) override;

    virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
    

private:
    int limbId = -1;
    IBoneTransformInterface *boneInterface = nullptr;

    ProceduralMeshComponentPair meshComponentPair;
    MeshData &FindMeshData(materialEnum type);
    void CreateLimb(FSingleLimbProperty &property);
    void CreateLimb(MeshData &data, FSingleLimbProperty &property);

    void InitProceduralMesh();
    void SetPhysicsEnabled(bool flag);

    bool GetTransformFromInterface(
        FVector &location,
        FRotator &rotation
    );


    //debug
    void LogActorAndComponentLocation();
    void LogProceduralMeshValid();

    void DebugShowComponentLocation();
    void DebugShowProceduralMeshComponentLocation();
    void DebugShowLocation(FVector location, FColor color);
    
};
