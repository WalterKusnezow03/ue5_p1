// Fill out your copyright notice in the Description page of Project Settings.


#include "room.h"

// Sets default values
Aroom::Aroom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void Aroom::BeginPlay()
{
	Super::BeginPlay();
	findDoors();
}

// Called every frame
void Aroom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void Aroom::findDoors(){
	TArray<UChildActorComponent *> childs; //create a TArray of the targeted type
	GetComponents<UChildActorComponent>(childs); //collect all types with GetComponents<dt>(array) method
	if(childs.Num() > 0){
		for (int i = 0; i < childs.Num(); i++){
			if(childs[i] != nullptr){
				FString name = childs[i]->GetName();


				if(name.Contains("door")){
					DebugHelper::showScreenMessage("door FOUND", FColor::Red);

					//doorPositions.push_back(childs[i]->GetActorLocation());
				}
			}
		}
	}
}


void Aroom::alignRoom(Aroom * other){
	//hier muss dann der andere raum angeschmiegt werden 

	//ACHTUNG
	//vielleicht kann ich die räume ja erstmal mit einem layout verbinden
	//---> und im ANSCHLUSS werden türen gebildet sodass alle räume mit einander verbunden sind
	//---> dabei sollte zum beispiel ein raum immer nur 1-3 türen haben (z.b.)


	//man könnte erstmal in einem 2d array ein layout fest legen und dann die türen einbauen
	//bzw ein 3d array und dann werden halt positionen blockiert
	//wenn es zum beispiel eine raum station ist kann man schon darauf achten dass es eher einer längliche struktur ist zb
	//oder ob sachen durch eine art aussen fassade verdeckt werden


	//man könnte echt gruselige strukturen bauen, alien pyramieden und sowas.





	//find 2 doors where distance = 0 bzw x == x1 oder y == y1 und dann distance




}
