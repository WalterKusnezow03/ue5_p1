// Fill out your copyright notice in the Description page of Project Settings.


#include "carriedItem.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "p2/entities/EntityScript.h"
#include <list>

// Sets default values
AcarriedItem::AcarriedItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	cameraPointer = nullptr;
	botPointer = nullptr;

	offset = FVector(-100, 100.0f, 0);

}

// Called when the game starts or when spawned
void AcarriedItem::BeginPlay()
{
	Super::BeginPlay();
	enableCollider(true);
	isVisible = true; //inital setting of visibilty, do not remove!
}

// Called every frame
void AcarriedItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	followPlayer();

}



void AcarriedItem::showScreenMessage(FString s){
	if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, s);
    }
}


/// @brief follows the player or bot around when picked up
void AcarriedItem::followPlayer(){

	if(cameraPointer == nullptr && botPointer == nullptr){
		return;
	}

	//player follow if player picked up
	if (isPickedupByPlayer())
	{
		FVector targetPos = cameraPointer->GetComponentLocation() + 
							cameraPointer->GetForwardVector() * 100.0f +
							getOffsetVector(); //sight offset and hipfire

        FRotator targetRotation = cameraPointer->GetComponentRotation();
        
        // Smoothly interpolate position and rotation
        FVector currentPos = GetActorLocation();
        FRotator currentRotation = GetActorRotation();

		//works a bit better than lerping, but still jitters sometimes sadly
		SetActorLocation(targetPos); 
        
        //SetActorLocation(FMath::VInterpTo(currentPos, targetPos, GetWorld()->GetDeltaSeconds(), 50.0f));
        SetActorRotation(FMath::RInterpTo(currentRotation, targetRotation, GetWorld()->GetDeltaSeconds(), 50.0f));
		return; //dont check for bot.
	}

	//bot follow if bot picked up
	if(botPointer != nullptr){
		FVector targetPos = botPointer->GetActorLocation() +
							botPointer->GetActorForwardVector() * 100.0f;
							//+ getOffsetVector();

		FRotator targetRotation = botPointer->GetActorRotation();
        
        // Smoothly interpolate position and rotation
        FVector currentPos = GetActorLocation();
        FRotator currentRotation = GetActorRotation();
		SetActorLocation(targetPos);

		//SetActorLocation(FMath::VInterpTo(currentPos, targetPos, GetWorld()->GetDeltaSeconds(), 50.0f));
        SetActorRotation(FMath::RInterpTo(currentRotation, targetRotation, GetWorld()->GetDeltaSeconds(), 50.0f));

	}

	
}

/// @brief Only for player:
/// returns the offset vector of the sight and hipfire by value
/// @return offset vector
FVector AcarriedItem::getOffsetVector(){
	FVector pos = FVector(0, 0, 0);
	if(cameraPointer != nullptr){
		pos += cameraPointer->GetRightVector().GetSafeNormal() * 25;
	}
	return pos;
}



/// @brief will enable and disable the collider for the actor
/// @param enable 
void AcarriedItem::enableCollider(bool enable){
	SetActorEnableCollision(enable);
}



/// @brief allows the player to pickup the weapon
/// @param cameraRefIn 
void AcarriedItem::pickup(UCameraComponent *cameraIn){
	if(!isPickedupByPlayer() && cameraIn != nullptr && botPointer == nullptr){
		cameraPointer = cameraIn; // Assign the address of cameraRefIn to cameraRef
		botPointer = nullptr;
		enableCollider(false);

		showItem(true);
		renderOnTop(true);

		//attachment to actor / component
		//this->AttachToActor(cameraIn, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
		this->AttachToComponent(cameraIn, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	}
}

/// @brief pickup emthod for bot
/// @param actorIn actor bot  
void AcarriedItem::pickupBot(AActor *actorIn){
	if(botPointer == nullptr && actorIn != nullptr && cameraPointer == nullptr){
		botPointer = actorIn;
		cameraPointer = nullptr;
		enableCollider(false);

		showItem(true);

		this->AttachToActor(botPointer, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
	}
}


bool AcarriedItem::isPickedupByPlayer(){
	return cameraPointer != nullptr; //one must be set
	// return cameraPointer != nullptr;
}



/// @brief shows or hides the item
/// @param show 
void AcarriedItem::showItem(bool show){
	isVisible = show;

	SetActorHiddenInGame(!show);  // Hides the actor if 'show' is false
}


/// @brief shows or hides a child actor if not nullptr
/// @param component child actor to hide
/// @param show show true false
void AcarriedItem::showChildActor(UChildActorComponent* component, bool show){
	if(component != nullptr){
		component->SetHiddenInGame(!show);  // Hides the actor if 'show' is false
	}
}


/// @brief returns is active status for player (hidden or not)
/// @return is active (visible status)
bool AcarriedItem::isActive(){
	return isVisible;
}

/// @brief drops the item and enables the collider, removes all owners
void AcarriedItem::drop(){

	if(cameraPointer != nullptr){
		//detach from camera
		this->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false)); 
		//false for detaching complete physcics? / designed to uncouple everything
	}
	if(botPointer != nullptr){
		//detach from bot
		//ChildActor->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
		this->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, false));
	}


	cameraPointer = nullptr;
	botPointer = nullptr; //reset bot too, for both actors designed
	enableCollider(true);
	showItem(true);
	renderOnTop(false); //testing needed
}

void AcarriedItem::renderOnTop(bool enable){

	//DebugHelper::showScreenMessage("changed priority");

   /**
	* general hierachy
	* UObject
		UActorComponent
			USceneComponent
				UPrimitiveComponent
					UStaticMeshComponent
					USkeletalMeshComponent
	*
	* Change render priority:
	*
	* TranslucencySortPriority = Priority; //is a public Iv of every UPrimtiveComponent
    */

	/*
	//not needed because only custom depth is used by now on its own
    int stencilValue = 0;
	if(enable){
		stencilValue = 1;
	}
	
	
	//this approach works but the weapons look bad.
	
	std::list<USkeletalMeshComponent *> list;
	this->findAllOfType<USkeletalMeshComponent>(*this, list);

	int counted = list.size();
	DebugHelper::showScreenMessage(FString::Printf(TEXT("COLLECTED SKELETONS %d"), counted), FColor::Yellow);
	

	for (USkeletalMeshComponent *s : list){
		if(s != nullptr){
			s->SetRenderCustomDepth(enable); //just true always, only change layer
			//s->SetCustomDepthStencilValue(stencilValue); // Higher value can render in front
		}
	}*/
}

template <typename T>
void AcarriedItem::findAllOfType(AActor &a , std::list<T*> & items)
{
	//upper type bound must be set inside here if the class is not generic!
	static_assert(std::is_base_of<UActorComponent, T>::value, "must be UActorComponent component");

    //find other actors first and search inside
    TArray<UChildActorComponent *> actors;
    a.template GetComponents<UChildActorComponent>(actors);
    if(actors.Num() > 0){
        for (int i = 0; i < actors.Num(); i++){
            AActor *a1 = actors[i]->GetChildActor();
            if (a1)
            {
                findAllOfType<T>(*a1, items);
            }
        }
    }

    //then layer own
	TArray<T *> array;
	a.template GetComponents<T>(array); //only provided by aactor
	if(array.Num() > 0){
		for (int i = 0; i < array.Num(); i++){
			items.push_back(array[i]);
		}
	}




	/**
	 * UObject
		UActorComponent
			USceneComponent
				UPrimitiveComponent
					UStaticMeshComponent
					USkeletalMeshComponent
	 */
}






void AcarriedItem::leftMouseDown(){

}

void AcarriedItem::leftMouseUp(){

}