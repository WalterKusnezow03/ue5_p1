#include "CubeLimbMaker.h"
#include "GameCore/MeshGenBase/MeshData/MeshData.h"
#include "AssetPlugin/gamestart/assetEnums/materialEnum.h"


AActor *CubeLimbMaker::createLimbPivotAtTop(
    int x, 
    int y, 
    int height,
    UWorld *world
){

	height = std::abs(height) * -1; //orient downwardss
	
	if(world != nullptr){
		FVector location(0, 0, 0);
		AcustomMeshActorBase *oberschenkel = spawnAcustomMeshActor(world);
		if(oberschenkel != nullptr){

            MeshData &ref = oberschenkel->findMeshDataReference(materialEnum::wallMaterial, false); //kein raycast.

            float xHalf = x / 2.0f;
			float yHalf = y / 2.0f;

            FVector pivot(-xHalf, -yHalf, 0.0f); //because pivot should be centered and cube is sized from pivot.

            ref.appendCubeAt(pivot, x, y, height);
            ref.calculateNormals();

            oberschenkel->ReloadMeshAndApplyAllMaterials();

            return oberschenkel;
		}
	}
	return nullptr;
}



AcustomMeshActorBase *CubeLimbMaker::spawnAcustomMeshActor(UWorld *world){
    if(world != nullptr){

        // --- new testing spawn by static class ---
        // Spawn the actor
        FVector location;
        FActorSpawnParameters params;
        AcustomMeshActorBase *SpawnedActor = world->SpawnActor<AcustomMeshActorBase>(
            AcustomMeshActorBase::StaticClass(),
            location,
            FRotator::ZeroRotator,
            params
        );
        return SpawnedActor;

    }
    return nullptr;
}