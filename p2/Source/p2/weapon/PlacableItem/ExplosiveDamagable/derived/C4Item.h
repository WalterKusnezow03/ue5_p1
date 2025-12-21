#pragma once

#include "CoreMinimal.h"
#include "p2/weapon/PlacableItem/ExplosiveDamagable/ExplosiveDamagableBase.h"
#include "Components/PointLightComponent.h"
#include "GameCore/util/timer/FlipTimer.h"


#include "C4Item.generated.h"

//can be placed, detonated and carried by player
UCLASS()
class P2_API AC4Item : public AExplosiveDamagableBase{
    GENERATED_BODY()

public:
    AC4Item();


    //void show hide
    //show c4 carried location
    //show c4 detonator location
    //place c4
    //detonate including children
    //release all children

    //detonate
    virtual void aim(bool aimstatus) override;

    virtual void showWeapon(bool show) override;

    void SetLocationAndLookDir(FVector &location, FVector &normal);

    virtual void ResetFlagsAndProperties() override;

protected:
    UPROPERTY()
    USceneComponent *c4SceneComponent = nullptr;

    UPROPERTY()
    USceneComponent *detonatorSceneComponent = nullptr;

    UPROPERTY()
    USceneComponent *triggerSceneComponent = nullptr;

    UPROPERTY()
    USceneComponent *c4light = nullptr;

    UPROPERTY()
    UPointLightComponent *c4PointLight = nullptr;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="lightSetup")
    UMaterial* materialLightPrefab;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="lightSetup")
    float PointLightMaxIntensity = 100.0f;

protected:

    UPROPERTY()
    UMaterialInstanceDynamic *lightMaterialInstance = nullptr;

    FlipTimer lightTimer;

    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;
    virtual void SpawnItemAtLocation(FVector &location, FVector &normal) override;

    void FindComponentsOnBeginPlay();
    void CreateMaterialOnBeginPlay();

    void ShowPressAnimation();

    virtual void DetonateChildren();
    virtual void Detonate() override;

    void DetonateByDamage();
    void DetonateByParent();

    void TickLightStatus(float deltatime);
    void UpdateMaterialEmmision(float scalar);
    void UpdatePointLightEmmision(float scalar);

    void SetupTimer();

    void SetParent(AC4Item *parentPtr);

    void RemoveFromParent();
    void RemoveFromChildList(AC4Item *item);

    virtual void takedamage(int d) override;

    void ShowComponent(USceneComponent *comp, bool flag);

private:

    TArray<AC4Item *> childs;
    AC4Item *parent = nullptr;


    bool markedDetonateByParent = true;
    


};