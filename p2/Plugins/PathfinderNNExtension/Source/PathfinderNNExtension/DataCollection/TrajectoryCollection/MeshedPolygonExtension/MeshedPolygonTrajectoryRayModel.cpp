#include "MeshedPolygonTrajectoryRayModel.h"


void FMeshedPolygonTrajectoryRayModel::Reset(){
    ClearTrajectoryData();
    ClearEnemyVisionData();
}

void FMeshedPolygonTrajectoryRayModel::ClearTrajectoryData(){
    playerTrajectories.Empty();
}

//immer in local coordinates
void FMeshedPolygonTrajectoryRayModel::EmbedResultPosition(FVector &position){
    int outX, outY = 1;
    ToIndexRaw(position, outX, outY);
    playerGroundTruth = FIntPoint(outX, outY);
}


void FMeshedPolygonTrajectoryRayModel::EmbedTrajectories(TArray<Trajectory> &trajectories){
    if(IsValid()){
        ClearTrajectoryData();
        OverrideTime(trajectories);
        //todo.
        EmbedRayModelFromTrajectories(trajectories);
    }
    else
    {
        DebugHelper::logMessage("FMeshedPolygonTrajectoryRayModel::TimeMap Cant Embed Trajectories");
    }
}

//todo
void FMeshedPolygonTrajectoryRayModel::EmbedRayModelFromTrajectories(TArray<Trajectory> &trajectories){
    //kann ja sein dass die letzten 4 punkte hinreichend sind
    //und von hier aus die samples gezogen werden.
}





/*
//TODO!
//player movement cone from trajectories
void FMeshedPolygonTrajectoryRayModel::EmbedConeFromTrajectories(
    TArray<Trajectory> &trajectories
){
    CreateOrClearTrajectoryConeGrid();
    if(TrajectoryConeGridIsValid()){
        FVector2D endDir, globalDir;
        if(trajectories.Num() > 0){
            FVector endPositon = trajectories.Last().GetPosition();
            if(
                TrajectoryCollection::EndDir(trajectories, endDir) && 
                TrajectoryCollection::GlobalDir(trajectories, globalDir)
            ){
                TraceConeOnGridBetweenDirections(
                    endPositon,
                    globalDir,
                    endDir,
                    trajectoryConePrecited
                );
                
                //Blur
                float sigma = 2.0f;
                int sizeMask = 5;
                ConvolutionOperatorGauss gaussian(sigma, sizeMask);
                gaussian.ApplyOperator(trajectoryConePrecited);
            }
        }


        
    }
}
*/

void FMeshedPolygonTrajectoryRayModel::OverrideTime(TArray<Trajectory> &trajectories){
    for (int i = 0; i < trajectories.Num(); i++){
        OverrideTime(trajectories[i]);
    }
}

void FMeshedPolygonTrajectoryRayModel::OverrideTime(Trajectory &current){
    FVector pos = current.GetPosition();
    float time = current.GetTime(); //will be negative, time relative to 0.
    
    int outX, outY = 1;
    if(IsInBound(pos, outX, outY)){
        OverrideTime(outX, outY, time);
    }
}

void FMeshedPolygonTrajectoryRayModel::OverrideTime(int x, int y, float time){
    FVector constructed(x, y, time);
    playerTrajectories.Add(constructed);
}




//embed enemy vision - but keep hit data raw

void FMeshedPolygonTrajectoryRayModel::ClearEnemyVisionData(){
    enemyHits.Empty();
}

void FMeshedPolygonTrajectoryRayModel::EmbedEnemyVision(const TArray<FVisionCone *> &cones){
    ClearEnemyVisionData();
    if (cones.Num() > 0){
        int raysPerEnemy = maxRaysPlayerVision / cones.Num();
        for (int i = 0; i < cones.Num(); i++){
            EmbedEnemyVision(cones[i], raysPerEnemy);
        }
    }
    //must be initialized to fixed size in any case!
    ValidateEnemyHitsBuffer();
}

void FMeshedPolygonTrajectoryRayModel::ValidateEnemyHitsBuffer(){
    enemyHits.SetNum(maxRaysPlayerVision);
}

void FMeshedPolygonTrajectoryRayModel::ValidatePlayerHitsBuffer(){
    playerHits.SetNum(maxRaysPlayerVision);
}

void FMeshedPolygonTrajectoryRayModel::ValidatePlayerTrajectoryBuffer(){
    playerTrajectories.SetNum(maxPlayerTrajectories);
}




void FMeshedPolygonTrajectoryRayModel::EmbedEnemyVision(FVisionCone *cone, int rays){
    if(cone){
        TArray<FIntPoint> hitsCollected;
        TraceConeCollectHits(
            cone->ActorLocation(),
            cone->GetLookDir(),
            cone->GetAngle(),
            rays,
            hitsCollected,
            true // dir is centered forward.
        );
        enemyHits.Append(hitsCollected);
    }
}


// ---- STORAGE INTERFACE ----
void FMeshedPolygonTrajectoryRayModel::AppendAsBinary(
    TArray<uint8> &buffer
){
    if(IsValid()){
        FMeshedPolygonRaytracable::AppendAsBinary(buffer);
        TemplateBufferStorageInterface::TAppendBuffer<FIntPoint>(playerHits, buffer);
        TemplateBufferStorageInterface::TAppendBuffer<FIntPoint>(enemyHits, buffer);

        TemplateBufferStorageInterface::TAppendBuffer<FVector>(playerTrajectories, buffer);
        TemplateBufferStorageInterface::TAppendSingleValue<FIntPoint>(playerGroundTruth, buffer);
    }

}

bool FMeshedPolygonTrajectoryRayModel::LoadFromBinary(
    TArray<uint8> &buffer,
    uint8 *& Ptr //reference to a pointer. Pointer by reference.
){

    
    if(FMeshedPolygonRaytracable::LoadFromBinary(buffer, Ptr)){
        if(!TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            TemplateBufferStorageInterface::TLoadBuffer<FIntPoint>(playerHits, Ptr);
        }
        if(!TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            TemplateBufferStorageInterface::TLoadBuffer<FIntPoint>(enemyHits, Ptr);
        }
        if(!TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            TemplateBufferStorageInterface::TLoadBuffer<FVector>(playerTrajectories, Ptr);
        }
        if(!TemplateBufferStorageInterface::EndReached(Ptr, buffer)){
            TemplateBufferStorageInterface::TLoadSingleValue<FIntPoint>(playerGroundTruth, Ptr);
        }
        return true;
    }
    return false;

}
// ---- STORAGE INTERFACE ----





// ---- REQUEST NN Conversion ----
//converts the buffer of (x,y) pairs to (x...x)(y..y) buffer
void FMeshedPolygonTrajectoryRayModel::ToFloatBufferChannels(
    const TArray<FIntPoint> &bufferIn, 
    TArray<float> &outBuffer
){
    outBuffer.Empty();
    outBuffer.SetNum(bufferIn.Num() * 2);
    int innerIndex = 0;

    //split into different channels (x1...xn)(y1...yn)
    for (int i = 0; i < bufferIn.Num(); i++){
        const FIntPoint &current = bufferIn[i];
        outBuffer[innerIndex] = current.X;
        outBuffer[bufferIn.Num() + innerIndex] = current.Y;
        innerIndex ++;
    }
    
    
    /*for (int i = 0; i < bufferIn.Num(); i++){
        const FIntPoint &current = bufferIn[i];
        outBuffer[innerIndex] = current.X;
        innerIndex ++;
    }

    for (int i = 0; i < bufferIn.Num(); i++){
        const FIntPoint &current = bufferIn[i];
        outBuffer[innerIndex] = current.Y;
        innerIndex++;
    }*/


}

void FMeshedPolygonTrajectoryRayModel::ToFloatBufferChannels(
    const TArray<FVector> &bufferIn, 
    TArray<float> &outBuffer
){
    outBuffer.Empty();
    outBuffer.SetNum(bufferIn.Num() * 3);
    int innerIndex = 0;

    //split into different channels (x1...xn)(y1...yn)(t1...tn)
    for (int i = 0; i < bufferIn.Num(); i++){
        const FVector &current = bufferIn[i];
        outBuffer[innerIndex] = current.X;
        outBuffer[bufferIn.Num() + innerIndex] = current.Y;
        outBuffer[bufferIn.Num() * 2 + innerIndex] = current.Z;
        innerIndex ++;
    }

    /*
    for (int i = 0; i < bufferIn.Num(); i++){
        const FIntPoint &current = bufferIn[i];
        outBuffer[innerIndex] = current.X;
        innerIndex ++;
    }

    for (int i = innerIndex; i < bufferIn.Num(); i++){
        const FIntPoint &current = bufferIn[i];
        outBuffer[innerIndex] = current.Y;
        innerIndex++;
    }
    
    for (int i = innerIndex; i < bufferIn.Num(); i++){
        const FIntPoint &current = bufferIn[i];
        outBuffer[innerIndex] = current.Z;
        innerIndex++;
    }*/
}

// ---- REQUEST TO NN SIMPLE ACCESS ----
bool FMeshedPolygonTrajectoryRayModel::PrepareAppendRequestBinary(TArray<uint8> &buffer){
    PrepareFitData();
    //all as FVector binary or flaot ?
    //unclear. Float might be better. / simple float.
       
    //nicht einfach float untereinander
    //es braucht seperate channels für (x,y)
    //dann kann man evt das time feature gesondert auf 0 setzen, also (x,y,t)

    //die trajektorien können auch in einen anderen
    //channel und dann später die branches gemerged werden

    //es macht das problem konsistenter zu lernen
    
    //trajectorien könnten trotzdem in einen getrennten branch

    //die input daten der outlines müssen trotzdem fixiert sein
    //um richtig aus dem binary zu lesen

    ValidateAllBuffers();

    // ---- CHANNEL 1 -----
    //append player ray model
    TArray<float> asFloat;
    ToFloatBufferChannels(playerHits, asFloat);
    TemplateBufferStorageInterface::TAppendBuffer<float>(asFloat, buffer);
    // ---- CHANNEL 1 -----

    // ---- CHANNEL 2 -----
    //append enemy ray model
    ToFloatBufferChannels(enemyHits, asFloat);
    TemplateBufferStorageInterface::TAppendBuffer<float>(asFloat, buffer);
    // ---- CHANNEL 2 -----

    // ---- SEPERATE CHANNEL ----
    //append player trajectoris (x,y,t)
    ToFloatBufferChannels(playerTrajectories, asFloat);
    TemplateBufferStorageInterface::TAppendBuffer<float>(asFloat, buffer);
    // ---- SEPERATE CHANNEL ----
   
    return true;
}


bool FMeshedPolygonTrajectoryRayModel::PrepareAppendRequestBinary(FONNXModelInput &input){
    PrepareFitData();
    // --- todo! ---

    return false;
}




void FMeshedPolygonTrajectoryRayModel::ValidateAllBuffers(){
    ValidateEnemyHitsBuffer();
    ValidatePlayerHitsBuffer();
    ValidatePlayerTrajectoryBuffer();
}

//append (float, float) to ground truth buffer
void FMeshedPolygonTrajectoryRayModel::AppendGroundTruth(TArray<uint8> &buffer){
    float x = playerGroundTruth.X;
    float y = playerGroundTruth.Y;
    TemplateBufferStorageInterface::TAppendSingleValue<float>(x, buffer);
    TemplateBufferStorageInterface::TAppendSingleValue<float>(y, buffer);
}


int FMeshedPolygonTrajectoryRayModel::ResultDataSizeBytes(){
    return sizeof(float) * 2;
}

bool FMeshedPolygonTrajectoryRayModel::PrepareRequestAndResultBatchBinary(TArray<uint8> &buffer){
    if(PrepareAppendRequestBinary(buffer)){
        AppendGroundTruth(buffer);
        return true;
    }
    return false;
}
// ---- REQUEST TO NN SIMPLE ACCESS ----

// ---- Respone Process ----
void FMeshedPolygonTrajectoryRayModel::ProcessFromPredictionBytes(const TArray<uint8> &buffer){
    //1.Ensure the byte size is divisible by 4 (sizeof(float))
    if (buffer.Num() % sizeof(float) == 0){
        int32 FloatCount = buffer.Num() / sizeof(float);
        TArray<float> FloatArray;
        FloatArray.SetNumUninitialized(FloatCount);

        //copy
        FMemory::Memcpy(FloatArray.GetData(), buffer.GetData(), buffer.Num());
        ProcessFromPredictionFloats(FloatArray);
    }
}

void FMeshedPolygonTrajectoryRayModel::ProcessFromPredictionFloats(const TArray<float> &buffer){
    //todo
    //to float
    //to local pos
    //to world pos

    //result expected as (x,y) as float
    if(buffer.Num() >= 2){
        localPrediciton = FVector(buffer[0], buffer[1], 0.0f);
        playerPrediction = BottomLeft() + localPrediciton;
    }
}

TArray<FVector> FMeshedPolygonTrajectoryRayModel::GetPlayerResultPositions(){
    TArray<FVector> outData;
    outData.Add(playerPrediction);
    return outData;
}

void FMeshedPolygonTrajectoryRayModel::NotifyVisiblePositionsFor(
    IPathfinderNNInterface *interfaceIn,
    bool useVisiblity
){
    if(interfaceIn){
        TArray<FVector> ResultWorld = GetPlayerResultPositions();

        if(!useVisiblity){
            interfaceIn->ResponseNNPositions(ResultWorld);
            return;
        }
        //debug, retrun same position too. (Es gibt erstmal nur eine.)
        interfaceIn->ResponseNNPositions(ResultWorld);

    }
}




EPolygonSampleType FMeshedPolygonTrajectoryRayModel::GetType(){
    return EPolygonSampleType::EMeshedPolygonTrajectoryRayModel;
}





float FMeshedPolygonTrajectoryRayModel::SimilarityOfSample(FMeshedPolygonTrajectoryLayeredInterface &other){
    FMeshedPolygonTrajectoryRayModel *ptr =
        TSampleCast<FMeshedPolygonTrajectoryRayModel>(other);
    if(ptr){
        //do compare here (later, but needed!)

    }

    return 0.0f; //by default, no deletion for now at all.
}

//todo: similatrity of sample for this class, aus eingesammelten hits einsammeln,
//und aus result position ?




