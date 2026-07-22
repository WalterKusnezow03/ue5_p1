#include "OutpostAlarmPole.h"

#include "CoreMinimal.h"

#include "AssetEnumCollection/assetEnums/materialEnum.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"
#include "CoreMath/Matrix/MMatrix.h"
#include "DebugPlugin/DebugHelper.h"
#include "GameCore/MeshGenBase/ELod.h"
#include "p2/entityManager/EntityManager.h"
#include "p2/_world/worldLevel.h"



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
    if (EntityManager *e = AworldLevel::entityManager()){
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
void AOutpostAlarmPole::takedamage(FCustomHitResult &result){
    Super::takedamage(result);
    
    if(AlarmFunctionEnabled() && result.HasHitPoint()){
        //Super
        if(doesHitWorld(result.HitPoint(), materialEnum::prop_alarmBoxMaterial)){
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