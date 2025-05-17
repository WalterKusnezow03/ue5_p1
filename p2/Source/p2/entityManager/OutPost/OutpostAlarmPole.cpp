#include "CoreMinimal.h"

#include "p2/gamestart/assetEnums/materialEnum.h"
#include "p2/meshgen/MeshData/MeshData.h"
#include "p2/entities/customIk/MMatrix.h"
#include "GameCore/DebugHelper.h"
#include "p2/meshgen/ELod.h"
#include "OutpostAlarmPole.h"


AOutpostAlarmPole::AOutpostAlarmPole() : AcustomMeshActor(){

}

AOutpostAlarmPole *AOutpostAlarmPole::Construct(UWorld *world, FVector &location){
    if(world){
        FRotator rotation;
        FActorSpawnParameters params;
        AOutpostAlarmPole *SpawnedActor = world->SpawnActor<AOutpostAlarmPole>(
            AOutpostAlarmPole::StaticClass(),
            location,
            FRotator::ZeroRotator,
            params
        );

        return SpawnedActor;
    
    }
    return nullptr;
}



void AOutpostAlarmPole::projectActorToGround(){
	FVector Start = GetActorLocation() + FVector(0, 0, 50000);
	FVector End = GetActorLocation() - FVector(0, 0, 50000);

	FHitResult HitResult;

    FCollisionQueryParams ignoreParams;
    if (EntityManager *e = worldLevel::entityManager()){
		// ignoreParams = e->getIgnoredRaycastParams(); //example for getting all
		ignoreParams = e->getIgnoredRaycastParams();
	}
	ignoreParams.bTraceComplex = false;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, ignoreParams);
	
	// Check If the raycast hit something
	if (bHit){
		FVector output = HitResult.ImpactPoint;
		SetActorLocation(output);
	}
}





void AOutpostAlarmPole::BeginPlay(){
    Super::BeginPlay();
    init();
}

void AOutpostAlarmPole::Tick(float DeltaTime){
    Super::Tick(DeltaTime);
}

/// @brief called on begin play
void AOutpostAlarmPole::init(){
    outpostParentPointer = nullptr;
    bAlarmFunctionEnabled = false;
    projectActorToGround();
    createMesh();

    DebugHelper::logMessage("debugalarmpole init");

    DebugHelper::showLineBetween(
        GetWorld(),
        GetActorLocation(),
        GetActorLocation() + FVector(0, 0, 10000),
        FColor::Black
    );
}

bool AOutpostAlarmPole::isDestructable(){
    return false;
}

///can react to destruction only if hitpoint is given! (for now)
void AOutpostAlarmPole::takedamage(int d, FVector &hitpoint, bool surpressed){
    Super::takedamage(d, hitpoint, surpressed);

    //react to hit on layer kasten

    if(AlarmFunctionEnabled()){
        //Super
        if(doesHitWorld(hitpoint, materialEnum::prop_alarmBoxMaterial)){
            disableAlarmFunction();
        }
    }
    
}


bool AOutpostAlarmPole::AlarmFunctionEnabled(){
    return bAlarmFunctionEnabled;
}

void AOutpostAlarmPole::enableAlarmFunction(){
    bAlarmFunctionEnabled = true;
}

void AOutpostAlarmPole::disableAlarmFunction(){
    bAlarmFunctionEnabled = false;
}



void AOutpostAlarmPole::createMesh(){

    //base pole
    int xscale = 30;
    int yscale = 30;
    int zheight = 500; //5m
    bool raycastOn = true;
    FVector pivot(0, 0, 0);

    MeshData &poleMesh = findMeshDataReference(
        materialEnum::treeMaterial, 
        ELod::lodNear, 
        raycastOn
    );

    poleMesh.appendCubeAt(
	    pivot,
		xscale, 
		yscale,
		zheight
	);

    //alarmbox
    int alarmBoxZscale = 30;
    FVector pivotAlarmBox(20, 0, 150);
    MeshData &alarmBoxMesh = findMeshDataReference(
        materialEnum::prop_alarmBoxMaterial, 
        ELod::lodNear, 
        raycastOn
    );

    alarmBoxMesh.appendCubeAt(
	    pivotAlarmBox,
		xscale, 
		yscale,
		alarmBoxZscale
	);




    //reload all
    ReloadMeshAndApplyAllMaterials();
}