#pragma once  
#include "CoreMinimal.h" 
#include "GameCore/util/AActorUtil.h"
#include "ActorBase.generated.h"  

UCLASS() 
class GAMECORE_API AActorBase : public AActor {
    GENERATED_BODY()

public:

    void FindAllChildsByName(FString namepart, TArray<USceneComponent *> &container);
    void FindAllChildsByName(
        USceneComponent *comp,
        FString namepart,
        TArray<USceneComponent *> &container
    );


    template <typename T>
    void TFindAllChildsByName(FString name, TArray<T*> &outarray){
        TArray<USceneComponent *> found;
        FindAllChildsByName(name, found);
        for (int i = 0; i < found.Num(); i++){
            if(USceneComponent *current = found[i]){
                if(T* castedComponent = Cast<T>(current)){
                    outarray.Add(castedComponent);
                }
            }
        }
    }

    //is not tested
    template <typename T>
    void TTryAssignByName(FString name, T *&ptrToAssign){
        TArray<T *> array;
        TFindAllChildsByName(name, array);
        for (int i = 0; i < array.Num(); i++){
            if(T *current = array[i]){
                if(USceneComponent *casted = Cast<T>(current)){
                    if(casted->GetName() == name){
                        ptrToAssign = current;
                        return;
                    }
                }
            }
        }
    }

private:



};