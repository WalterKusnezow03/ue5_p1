#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/PlacableItem/ExplosiveDamagable/ExplosiveDamagableBase.h"
#include "GameCore/MeshGenBase/lodHelper/ProceduralMeshComponentPair.h"

#include "AssetEnumCollection/assetEnums/materialEnum.h"

#include "TripWire.generated.h"

//doesnt do anything yet, must check for entities all the time somehow.
UCLASS()
class P2_API ATripWire : public AExplosiveDamagableBase {
    GENERATED_BODY()

public:
    

    void ShowWire(bool flag);

protected:
    USceneComponent *FindHandCarriedScene(EArmType type) override;

    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    //should detonate by line intersect test instead!
    float TriggerDistance = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="_tripWireSetup")
    FString wireStartSceneComponentName = "wireStartSceneComponent";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="_tripWireSetup")
    FString PCMSceneComponentName = "PCMSceneComponent";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="_tripWireSetup")
    float widthWire = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="_tripWireSetup")
    float lengthWire = 300.0f;

    
    virtual void OnPickup() override;

private:
    void FindPCMComponent();
    void FindWireStartingComponent();
    void TickUpdateBound();

    bool AnyIntersectWithWireWorld(TArray<FVector> &positions);
    bool AnyIntersectWithWireWorld(FVector &position);
    bool AnyIntersectWithWire(FVector &position);

    ProceduralMeshComponentPair wireMesh;

    UPROPERTY()
    USceneComponent *rootScene = nullptr;

    UPROPERTY()
    UProceduralMeshComponent *Mesh = nullptr;

    UPROPERTY()
    USceneComponent *wireStart = nullptr;

    materialEnum meshMaterial();

    FVector WireStartLocation();

    void SetupWire();
    void generateVertexBuffer(TArray<FVector> &vertecies);

    void UpdateWireLocations();
    void UpdateWireLocations(TArray<FVector> &rawMeshData);
    void UpdateWireLocationStartingQuad(
        FVector &v0,
        FVector &v1,
        FVector &v2,
        FVector &v3
    );
    void UpdateWireLocationEndingQuad(
        FVector &v0,
        FVector &v1,
        FVector &v2,
        FVector &v3
    );

    MeshData &TripWireMeshData();

    bool RaycastForwardLocal(FVector &outpostion);
    bool RaycastForwardAndDownLocal(FVector &outpostion);
    bool PerformRaycast(FVector &start, FVector &end, FVector &outposition);
    bool PerformRaycastLocalHit(FVector &start, FVector &end, FVector &outposition);

    virtual void SpawnItemAtLocation(FVector &location, FVector &normal) override;
};
