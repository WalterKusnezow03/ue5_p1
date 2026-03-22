// Copyright Walter Kusnezow All Rights Reserved.


#pragma once

#include "CoreMinimal.h"
#include "AnyMeshWidgetPlugin/Public/ComponentSerializable/AnyMeshWidgetComponentBaseSerializable.h"

//widget mesh data extracted from a static mesh by name.
//this component cant have rotation but the child plane can have
//this component must be in relative space 0,0,0 to the static mesh
//extracting data from

#include "AnyMeshWidgetExtracted.generated.h"

UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class ANYMESHWIDGETPLUGIN_API UAnyMeshWidgetExtracted : public UAnyMeshWidgetComponentBaseSerializable {
    GENERATED_BODY()

public:
    


protected:
    virtual void BeginPlay() override;

    virtual void TickComponent(
        float DeltaTime,
        ELevelTick TickType,
        FActorComponentTickFunction *ThisTickFunction) override;

    UPROPERTY(EditAnywhere, Category = "_ExtractOption")
    FString extractFromMeshName;

    UPROPERTY(EditAnywhere, Category = "_ExtractOption")
    FString planeChildName;

    void FindComponentsOnBeginPlayAndLoadMeshData();
    void SetResolution(UStaticMeshComponent *plane);

protected:
    UStaticMeshComponent *FindStaticMeshComponent(FString subString);
    
    UStaticMeshComponent *FindStaticMeshComponent(
        FString subString,
        USceneComponent *child
    );
    UStaticMeshComponent *FindStaticMeshComponent(
        FString subString,
        TArray<USceneComponent *> &childs
    );

    UStaticMeshComponent *FindStaticMeshComponent(
        FString subString,
        AActor *parent
    );
};