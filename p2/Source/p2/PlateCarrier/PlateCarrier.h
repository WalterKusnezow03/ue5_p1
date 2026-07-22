#pragma once

#include "GameCore/util/ActorBase/ActorBase.h"
#include "GameCore/interfaces/DamageInterface/Damageinterface.h"
#include "CoreMinimal.h"
#include "p2/PlateCarrier/MagSocket/PlateCarrierMagSocket.h"
#include "p2/PlateCarrier/virtualHand/MagSwapProcess.h"
#include "p2/PlateCarrier/MagSocket/MagSocketType.h"

#include "PlateCarrier.generated.h"

UCLASS()
class P2_API APlateCarrier : 
public AActorBase,
public IDamageinterface
{
    GENERATED_BODY()

public:
    static APlateCarrier *MakeInstance();

    APlateCarrier();

    void SetPlateCarrierOwner(IDamageinterface *interface);
    

    // --- IDamageInterface override ---
    virtual void takedamage(FCustomHitResult &result) override;

    //actor location might not be enough for explosive / alert management
    virtual bool IsInRange(const FVector &position, float maxDistance) override;

    virtual void setTeam(teamEnum t) override;
    virtual teamEnum getTeam() override;
    // --- IDamageInterface override ---


    //get mag static mesh / scene componenent
    //kontrolle wird abgegeben: externe intepolation des objektes
    //eject aus parenting systems, direkte eingabe ins target system


    //insert mag / scene component
    //kontrolle wird übergeben: interne interpolation
    //des objektes
    //inject sofort an sub hierachie

    //callback wenn animation fertig?

    //-> pass eject mag: 
    //--> merke parent system für wieder einsetzen
    //---> eject aus parent system: reinstecken
    //----> rausholen: inject ins parent system
    //-----> dort hin animieren 



    //Tick muss dazu angestellt sein!
    //der plate carrier muss die animationen abspielen!

    //ptr will be kept inside until finish anim!
    //returns whether process started sucessfully
    bool StartSwapMag(EMagSocketType socketType, MagSwapProcess *process);

    //will on weapon switch
    void FillMags(EMagSocketType socketType, USceneComponent *magPrefab);

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float deltatime) override;

    void DisableCollision();
    void CollectSockets();

    //mag sockets simple erstmal
    TArray<UPlateCarrierMagSocket *> magSockets;

    //swap ticked process, will be reset on finish!
    MagSwapProcess *swapProcess = nullptr;

    UPlateCarrierMagSocket *FindMagFilledSlot();
    UPlateCarrierMagSocket *FindMagFilledSlot(EMagSocketType type);
    UPlateCarrierMagSocket *FindMagFilledSlot(TArray<UPlateCarrierMagSocket *> &inSockets);

    void FindAllMagSlots(
        EMagSocketType type,
        TArray<UPlateCarrierMagSocket *> &outArray
    );

    FString SocketTypeToString(EMagSocketType type);

protected:
    IDamageinterface *ownerDamageInterface = nullptr;
    teamEnum cachedTeam = teamEnum::none;

    void TickSwapProcess(float deltatime);

public:
    //attachment to target actor
    void AttachToActor(AActor *other, const FVector &relativeLocationOverride);

private:
    void AttachToActor(AActor *other);
};