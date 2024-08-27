// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Engine/StaticMesh.h"
#include "StaticMeshResources.h" 
#include "GameFramework/Actor.h"

#include "AActorUtil.h"

AActorUtil::AActorUtil()
{
}

AActorUtil::~AActorUtil()
{
}


/// @brief finds all components of a type starting from
/// UObject
///		UActorComponent
///			USceneComponent
///				UPrimitiveComponent
///					UStaticMeshComponent
///					to USkeletalMeshComponent
/// @tparam T derived from UActorComponent or more 
/// @param actor 
/// @param container 
template <typename T>
void AActorUtil::findAllComponentsByType(AActor &actor, std::vector<T *> container){
    static_assert(std::is_base_of<UActorComponent, T>::value, "must be UActorComponent derived");
    /**
	 * UObject
		UActorComponent
			USceneComponent
				UPrimitiveComponent
					UStaticMeshComponent
					USkeletalMeshComponent
	 */

    //check childs
    TArray<UChildActorComponent *> childActors;
    actor.GetComponents<T>(childActors);
    for (int i = 0; i < childActors.Num(); i++){
        if(UChildActorComponent *c = childActors[i]){
            AActor *casted = Cast<AActor>(c);
            if(casted != nullptr){
                findAllComponentsByType<T>(casted, container);
            }
        }
    }

    //check own
    TArray<T *> foundOfType;
    actor.GetComponents<T>(foundOfType);
    for (int i = 0; i < foundOfType.Num(); i++){
        if(T *t = foundOfType[i]){
            container.push_back(t);
        }
    }
}




void AActorUtil::findDirectChildsByName(AActor &actor, FString namepart, TArray<UChildActorComponent*> container){
    TArray<UChildActorComponent *> childs;
    actor.GetComponents<UChildActorComponent>(childs);
    for (int i = 0; i < childs.Num(); i++){
        if(childs[i] != nullptr){
			FString name = childs[i]->GetName();
			if(name.Contains(namepart)){
                container.Add(childs[i]);
            }
        }
    
    }
}