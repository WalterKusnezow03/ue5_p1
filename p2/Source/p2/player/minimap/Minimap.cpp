#include "Minimap.h"
#include "p2/renderTargets/customRenderTarget/CustomRenderedTexture.h"
#include "p2/entityManager/AlertManager.h"
#include "p2/gamestart/assetEnums/textureEnum.h"
#include "p2/DebugHelper.h"
#include "p2/entities/customIk/MMatrix.h"


AMinimap* AMinimap::Construct(
    UWorld *world, 
    UCameraComponent *cameraComponent
){
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

            if(spawned){
                spawned->attachToBottomLeftCorner(cameraComponent);
            }

            return spawned;
        }
    }
    return nullptr;
}


/// @brief attaches the minimap to the camera on the bottom right direction ray of the camera
/// @param camera player camera
void AMinimap::attachToBottomLeftCorner(UCameraComponent *camera){
    if(camera){
        /*
        0 1
        3 2
        */
        TArray<FVector> cornerDirections;
        deprojectCamera(camera, cornerDirections);
        if(cornerDirections.Num() >= 4){
            float distance = 100.0f;
            FVector cornerDirection = findDir(cornerDirections, -1.0f, -1.0f); //bottom left
            attachTo(camera, cornerDirection, distance);
        }
    }
}

FVector AMinimap::findDir(TArray<FVector> &directions, float signY, float signZ){
    bool signYFlag = signY < 0.0f;
    bool signZFlag = signZ < 0.0f;
    for(int i = 0; i < directions.Num(); i++){
        FVector &current = directions[i];
        bool signYCurrent = current.Y < 0.0f;
        bool signZCurrent = current.Z < 0.0f;
        if(signYCurrent == signYFlag && signZCurrent == signZFlag){
            return current;
        }
    }
    return FVector(0,0,0);
}


/// @brief finds the corner directions of the camera / viewport (rays vom viewport matrix)
/// @param Camera 
/// @param outCornerDirections 
void AMinimap::deprojectCamera(
    UCameraComponent *Camera,
    TArray<FVector> &outCornerDirections
){
    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if(!PC){
        return;
    }
    int32 SizeX, SizeY;
    PC->GetViewportSize(SizeX, SizeY);

    DebugHelper::logMessage("ViewPort size", FVector(SizeX, SizeY,0));

    FVector2D ScreenCorners[4] = {
        FVector2D(0, SizeY),            
        FVector2D(0, 0),                  
        FVector2D(SizeX, 0),              
        FVector2D(SizeX, SizeY)
    };

    for (int i = 0; i < 4; ++i)
    {
        FVector WorldOrigin, WorldDirection;
        PC->DeprojectScreenPositionToWorld(ScreenCorners[i].X, ScreenCorners[i].Y, WorldOrigin, WorldDirection);
        WorldDirection.X = std::abs(WorldDirection.X);
        outCornerDirections.Add(WorldDirection);
        DebugHelper::logMessage("corner dir: ", WorldDirection);
    }
}

void AMinimap::attachTo(
    UCameraComponent *camera,
    FVector &cornerDirection,
    float distance
){
    if(camera){
        
        //problem aktuell:pivot falsch gesetzt

        //mesh pivot neu setzen
        FVector offset;
        if(Mesh){
            DebugHelper::logMessage("Map RelativeLocation Before", Mesh->GetRelativeLocation());

            float scaleInitial = 100.0f;
            FVector dir;
            dir.X = 0.0f;
            dir.Y = cornerDirection.Y > 0.0f ? -1.0f : 1.0f;
            dir.Z = cornerDirection.Z > 0.0f ? -1.0f : 1.0f;
            dir *= (scaleInitial / 2.0f); //Half offset
        
            float scaleRelative = 0.2f;
            offset = dir * scaleRelative; //this is correct dont touch
            
            DebugHelper::logMessage("Map Relative Location", offset);
            Mesh->SetRelativeScale3D(FVector(scaleRelative));
        }

        this->AttachToComponent(camera, FAttachmentTransformRules::KeepRelativeTransform);
        FVector gx = cornerDirection.GetSafeNormal() * distance;
        gx += offset;

        SetActorRelativeLocation(gx);
        
    }
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
    }

    // Turn off collision entirely
    Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // Optional: also set collision response to ignore everything (not strictly needed if NoCollision is set)
    Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

void AMinimap::BeginPlay(){
    Super::BeginPlay();
    initialRotation();
    initTexture();
    addPlayerMarker();
}

void AMinimap::initialRotation(){
    FRotator rotation;
    rotation.Roll = 0.0f;
    rotation.Yaw = 0.0f;
    rotation.Pitch = 90.0f;
    Mesh->SetRelativeRotation(rotation);
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

            texture->enableBackground(true);
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

    //debug draw
    if(false){
        FVector location = GetActorLocation();
        DebugHelper::showLineBetween(
            GetWorld(),
            location,
            location + FVector(100,0,0),
            FColor::Green
        );
    }

}


void AMinimap::updatePlayerPositionAndRotation(FVector &pos, FRotator &rot){
    playerPosition = pos;
    rot.Pitch = 0.0f;
    rot.Roll = 0.0f;
    if(debugMessages){
        DebugHelper::showScreenMessage("player rotation yaw ", (float) rot.Yaw);
    }
    

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

    //add to texture draw enemies
    texture->replaceMarkers(
        worldSpacePositions,//already local //replace by matrix
        textureEnum::enemyMarkerIcon
    );
   

}

void AMinimap::addPlayerMarker(){
    //replace player
    if(texture){
        FVector2D canvasScale = texture->canvasScale();
        FVector2D playerLocationCenterOfCanvas = canvasScale / 2.0f;
        FVector playerLocationCenterOfCanvas3D(
            playerLocationCenterOfCanvas.X, 
            playerLocationCenterOfCanvas.Y, 
            0.0f
        );
        TArray<FVector> playerMarker = {playerLocationCenterOfCanvas3D};
        texture->replaceMarkers(
            playerMarker, //replace by matrix
            textureEnum::playerMarkerIcon
        );
    }
    
}



void AMinimap::transformToPlayerSpace(TArray<FVector> &positions){
    for(int i = 0; i < positions.Num(); i++){
        FVector &ref = positions[i];
        ref = playerMatrixInverted * ref;

        ref.Z = 0.0f;
        if(debugMessages){
            DebugHelper::showScreenMessage("entity relative pos", ref);
        }
        
    }
}

void AMinimap::transformToPlayerSpace(TArray<MMatrix> &positions){
    for(int i = 0; i < positions.Num(); i++){
        MMatrix &ref = positions[i];
        ref = playerMatrixInverted * ref;
        if(debugMessages){
            DebugHelper::showScreenMessage("entity relative pos", ref.getTranslation());
        }
        
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

