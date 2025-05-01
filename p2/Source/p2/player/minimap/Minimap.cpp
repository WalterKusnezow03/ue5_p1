#include "Minimap.h"
#include "p2/renderTargets/customRenderTarget/CustomRenderedTexture.h"
#include "p2/entityManager/AlertManager.h"
#include "p2/gamestart/assetEnums/textureEnum.h"
#include "p2/DebugHelper.h"
#include "p2/entities/customIk/MMatrix.h"


AMinimap* AMinimap::Construct(UWorld *world, AActor *attachTo){
    if(world != nullptr){

        UClass *toSpawn = AMinimap::StaticClass();
        if (toSpawn){
            //Initialize SpawnParams if needed
            FActorSpawnParameters SpawnParams;

            // Spawn the actor
            FVector location(0,0,0);
            AMinimap *spawned = world->SpawnActor<AMinimap>(
                toSpawn, 
                location, 
                FRotator::ZeroRotator, 
                SpawnParams
            );

            if(attachTo && spawned){
                FVector relativeLocation(100,0,0);
                //ChildActor->AttachToActor(ParentActor, FAttachmentTransformRules::KeepRelativeTransform);
                spawned->AttachToActor(attachTo, FAttachmentTransformRules::KeepRelativeTransform);
                spawned->SetActorRelativeLocation(relativeLocation);
            }
            

            return spawned;
        }
    }
    return nullptr;
}


AMinimap::AMinimap(){
    PrimaryActorTick.bCanEverTick = true;


    // Create the ProceduralMeshComponent
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GeneratedMesh"));
    RootComponent = Mesh;


    static ConstructorHelpers::FObjectFinder<UStaticMesh> PlaneMeshAsset(TEXT("/Engine/BasicShapes/Plane"));

    if (PlaneMeshAsset.Succeeded())
    {
        Mesh->SetStaticMesh(PlaneMeshAsset.Object);
        DebugHelper::logMessage("loaded plane mesh");
    }

    // Turn off collision entirely
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // Optional: also set collision response to ignore everything (not strictly needed if NoCollision is set)
    Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AMinimap::BeginPlay(){
    Super::BeginPlay();
    initialRotationAndScale();
    initTexture();
}

void AMinimap::initialRotationAndScale(){
    FRotator rotation;
    rotation.Pitch = 90.0f;
    //this->SetActorRotation(rotation);

    //relativ zu ein meter
    //FVector scale(0.1f, 0.1f, 1.0f);
    //Mesh->SetRelativeScale3D(scale);
}

void AMinimap::initTexture(){
    if(texture == nullptr){
        texture = UCustomRenderedTexture::Construct(this);
        if(texture != nullptr){
            UMaterialInterface *material = texture->getMaterial();
            if(material){
                int layer = 0;
                Mesh->SetMaterial(layer, material);
            }
        }
    }
}


void AMinimap::Tick(float DeltaTime){
    Super::Tick(DeltaTime);

    //update minimap
    updateMiniMapItems();

    //tick minimap texture
    if(texture != nullptr){
        texture->Tick(DeltaTime);
    }

}


void AMinimap::updatePlayerPositionAndRotation(FVector &pos, FRotator &rot){
    playerPosition = pos;
    rot.Pitch = 0.0f;
    rot.Roll = 0.0f;
    DebugHelper::showScreenMessage("player rotation yaw ", (float) rot.Yaw);

    //M = T * R
    //M^-1 = R^-1 * T^-1
    MMatrix r(rot);
    MMatrix t(pos); 
    MMatrix tr = t * r;
    playerMatrixInverted = tr.createInverse();
    
}

void AMinimap::updateMiniMapItems(){
    if(!texture){
        return;
    }

    TArray<MMatrix> worldSpacePositions;
    //AlertManager::EntitiesInRadius(
    AlertManager::EntitiesInRadiusAsTransform(
		playerPosition,
		minimapRadius,
		worldSpacePositions
	);

    transformToPlayerSpace(worldSpacePositions);
    
    FVector2D canvasScale = texture->canvasScale();
    transformToCanvasSpace(worldSpacePositions, canvasScale);

    //add to texture draw dots
    texture->replaceMarkers(
        worldSpacePositions, 
        textureEnum::enemyMarkerIcon
    );

}

void AMinimap::transformToPlayerSpace(TArray<FVector> &positions){
    for(int i = 0; i < positions.Num(); i++){
        FVector &ref = positions[i];
        ref = playerMatrixInverted * ref;

        ref.Z = 0.0f;
        DebugHelper::showScreenMessage("entity relative pos", ref);
    }
}

void AMinimap::transformToPlayerSpace(TArray<MMatrix> &positions){
    for(int i = 0; i < positions.Num(); i++){
        MMatrix &ref = positions[i];
        ref = playerMatrixInverted * ref;
        DebugHelper::showScreenMessage("entity relative pos", ref.getTranslation());
    }
}


void AMinimap::transformToCanvasSpace(TArray<FVector> &positions, FVector2D &canvasScale){
    //pos * x = scale
    //x = ?
    //scale / pos = x
    float scaleX = canvasScale.X;
    float scaleY = canvasScale.Y;
    for(int i = 0; i < positions.Num(); i++){
        //move to local scale space
        FVector &current = positions[i];

        current.X = scaleToCanvasSpace(current.X, scaleX, minimapRadius);
        current.Y = scaleToCanvasSpace(current.Y, scaleY, minimapRadius);


        //DebugHelper::showScreenMessage("entity canvas pos", current);

        //move to align with center
        current.X += scaleX / 2.0f;
        current.Y += scaleY / 2.0f;

    }
    
}


float AMinimap::scaleToCanvasSpace(float xPos, float xCanvasScale, float mapsize){
    float normalized = xPos / mapsize; // skalar [0,1]
    return normalized * xCanvasScale; // auf canvas skaliert
}






void AMinimap::transformToCanvasSpace(TArray<MMatrix> &positions, FVector2D &canvasScale){
    //pos * x = scale
    //x = ?
    //scale / pos = x
    float scaleX = canvasScale.X;
    float scaleY = canvasScale.Y;
    for(int i = 0; i < positions.Num(); i++){
        //move to local scale space
        FVector current = positions[i].getTranslation();
        current.X = scaleToCanvasSpace(current.X, scaleX, minimapRadius);
        current.Y = scaleToCanvasSpace(current.Y, scaleY, minimapRadius);
        current.X += scaleX / 2.0f;
        current.Y += scaleY / 2.0f;
        positions[i].setTranslation(current);
    }
    
}

