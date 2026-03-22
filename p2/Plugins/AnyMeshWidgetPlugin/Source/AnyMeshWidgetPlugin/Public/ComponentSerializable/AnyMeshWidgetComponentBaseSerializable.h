#pragma once

#include "CoreMinimal.h"

#include "AnyMeshWidgetComponentBaseSerializable.generated.h"

/// @brief CAN SAVE MESH DATA TO STORAGE AND LOAD
UCLASS()
class ANYMESHWIDGETPLUGIN_API UAnyMeshWidgetComponentBaseSerializable : public UAnyMeshWidgetComponentBase {
    GENERATED_BODY()

public:
    


protected:

    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    //innerpath
    UPROPERTY(EditAnywhere, Category = "_SerializeOption")
    FString filePath;

    UPROPERTY(EditAnywhere, Category ="_SerializeOption")
    FString innerFileName;

    UPROPERTY(EditAnywhere, Category = "_SerializeOption")
    bool loadSerialized = true;

    bool wasLoaded = false;
    bool alreadySaved = false;

    bool meshDataWasLoaded();

    void LoadMeshDataFromDisk();
    void SaveMeshDataToDisk();

    FString makePath(){
        return BaseDir() + filePath + innerFileName + ".bin";
    }

    FString BaseDir(){
        return FPaths::ProjectSavedDir() //has "/" at end
        + TEXT("StorageInterfaceSaved/");
    }
};