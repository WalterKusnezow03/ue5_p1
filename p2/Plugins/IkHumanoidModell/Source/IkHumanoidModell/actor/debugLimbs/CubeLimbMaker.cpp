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
            oberschenkel->setTeam(teamEnum::neutralTeam);

            bool haveRaycast = true;// with raycast, is ignored by humanoid controller manually
            MeshData &ref = oberschenkel->findMeshDataReference(materialEnum::wallMaterial, haveRaycast); 

            //smaller distances allowed:
            ref.MofidyEpsilon(0.1f); //smaller distances allowed, prevents removing triangles

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

float CubeLimbMaker::min(float a, float b, float c){
    float ab = std::min(a, b);
    float abc = std::min(ab, c);
    return abc;
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