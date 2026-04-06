#pragma once

#include "CoreMinimal.h"

#include "SharedAnyMeshWidgetComponentSettings.generated.h"

UCLASS()
class ANYMESHWIDGETPLUGIN_API ASharedAnyMeshWidgetComponentSettings : public AActor{
    GENERATED_BODY()

public:
    static void MakeInstanceIfNeeded(UWorld *world);
    static ASharedAnyMeshWidgetComponentSettings *MakeInstance(UWorld *world);

    
    static bool BShowColoredUVMap();
    static void SetShowColorMap(bool flag);

protected:
    virtual void BeginPlay();
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    bool bShowColoredUVMap = false; 

    static ASharedAnyMeshWidgetComponentSettings *instancePtr;

private:
};