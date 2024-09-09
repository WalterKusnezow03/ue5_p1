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



/// @brief shows or hides the item
/// @param show 
void AActorUtil::showActor(AActor & actor, bool show){
	actor.SetActorHiddenInGame(!show);  // Hides the actor if 'show' is false
}


/// @brief shows or hides a child actor if not nullptr
/// @param component child actor to hide
/// @param show show true false
void AActorUtil::showChildActorComponent(UChildActorComponent &component, bool show){
	component.SetHiddenInGame(!show);  // Hides the actor if 'show' is false
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
void AActorUtil::findAllComponentsByType(AActor &actor, std::vector<T *> &container){
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

            //get aactor from the uchildactor component
            AActor *fromChild = c->GetChildActor();
            if (fromChild != nullptr)
            {
                findAllComponentsByType<T>(fromChild, container);
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



/// @brief will find all direct childs by name
/// @param actor to check
/// @param namepart name which is contained in the child actor name
/// @param container container to save in
void AActorUtil::findDirectChildsByName(AActor &actor, FString namepart, TArray<UChildActorComponent*> &container){

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


/// @brief not tested!
/// @param actor 
/// @param namepart 
/// @param container 
void AActorUtil::findAllChildsByName(AActor &actor, FString namepart, TArray<UChildActorComponent*> &container){
    
    
    //check all child for being actos
    TArray<UChildActorComponent *> childs;
    actor.GetComponents<UChildActorComponent>(childs);
    for (int i = 0; i < childs.Num(); i++){
        if(childs[i] != nullptr){
            
            FString currName = childs[i]->GetName();
            if(currName.Contains(namepart)){
                container.Add(childs[i]);
            }

            AActor *actorFromChild = childs[i]->GetChildActor();
            if(actorFromChild != nullptr){
                findAllChildsByName(*actorFromChild, namepart, container);
            }
        }
    }

    
}



/// @brief will calculate the x y and z scale of the aactor
/// @param actor actor to check, must not be nullptr
/// @param x x scale out
/// @param y y scale out
/// @param z z scale out
void AActorUtil::calculateActorBounds(AActor *actor, int &x, int &y, int &z){
    if(actor != nullptr){
        FVector Origin;
        FVector Extent;
        actor->GetActorBounds(true, Origin, Extent); //leg mir das da rein prinzip


        int xScale = (int)(Extent.X * 2);
        int yScale = (int)(Extent.Y * 2);
        int zScale = (int)(Extent.Z * 2);

        x = xScale;
        y = yScale;
        z = zScale;
    }
}


/// @brief will calculate the x y and z scale of the uclass
/// @param uclass uclass -> actor to check, must not be nullptr
/// @param x x scale out
/// @param y y scale out
/// @param z z scale out
void AActorUtil::calculateActorBounds(UWorld *world, UClass *uclassIn, int &x, int &y, int &z){
    if(uclassIn != nullptr){
        if(EntityManager *e = EntityManager::instance()){

            FVector debugLocation(0, 0, -1000);
            AActor *tmp = e->spawnAactor(world, uclassIn, debugLocation);
            if(tmp != nullptr){
                calculateActorBounds(tmp, x, y, z);
                tmp->Destroy();
            }
        }
    }
}