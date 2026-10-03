#include "MeshedPolygonTrajectoryRayModel.h"


void FMeshedPolygonTrajectoryRayModel::Reset(){
    ClearTrajectoryData();
    ClearEnemyVisionData();
    playerHitsGroundTruth.Empty();
}

void FMeshedPolygonTrajectoryRayModel::ClearTrajectoryData(){
    playerTrajectories.Empty();
}

//immer in local coordinates
void FMeshedPolygonTrajectoryRayModel::EmbedResultPosition(FVector &position){
    playerHitsGroundTruth.Empty();
    Trace360(position, playerHitsGroundTruth);

    //playerGroundTruth = position - BottomLeft();  // AB = B - A

    int outX, outY = 1;
    ToIndexRaw(position, outX, outY);
    FIntPoint playerGroundTruth = FIntPoint(outX, outY);
    playerHitsGroundTruth[playerHitsGroundTruth.Num() -1 ] = playerGroundTruth; //359 + 1
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

    //oder nur einer am ende
    if(trajectories.Num() > 0){
        Trajectory &last = trajectories.Last();
        FVector pos = last.GetPosition();
        Trace360(pos, playerHits);
    }
}

void FMeshedPolygonTrajectoryRayModel::Trace360(
    const FVector &pos,
    TArray<FIntPoint> &hitsCollected
){
    FVisionCone cone;
    cone.UpdateAs360(pos);

    TArray<FIntPoint> hitsCollectedTmp;
    TraceConeCollectHits(
        cone.ActorLocation(),
        cone.GetLookDir(),
        cone.GetAngle(),
        maxRaysPlayerVision,
        hitsCollectedTmp,
        true // dir is centered forward.
    );
    hitsCollected.Append(hitsCollectedTmp);
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

void FMeshedPolygonTrajectoryRayModel::ValidatePlayerGTBuffer(){
    playerHitsGroundTruth.SetNum(maxRaysPlayerVision); //360 + pos
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

        TemplateBufferStorageInterface::TAppendBuffer<FIntPoint>(playerHitsGroundTruth, buffer);
        //TemplateBufferStorageInterface::TAppendSingleValue<FIntPoint>(playerGroundTruth, buffer);
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
            TemplateBufferStorageInterface::TLoadBuffer<FIntPoint>(playerHitsGroundTruth, Ptr);
            //TemplateBufferStorageInterface::TLoadSingleValue<FIntPoint>(playerGroundTruth, Ptr);
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

    //split into 2 different channels (x1...xn)(y1...yn)
    for (int i = 0; i < bufferIn.Num(); i++){
        const FIntPoint &current = bufferIn[i];
        outBuffer[innerIndex] = current.X;
        outBuffer[bufferIn.Num() + innerIndex] = current.Y;
        innerIndex ++;
    }
}

void FMeshedPolygonTrajectoryRayModel::ToFloatBufferChannels(
    const TArray<FVector> &bufferIn, 
    TArray<float> &outBuffer
){
    outBuffer.Empty();
    outBuffer.SetNum(bufferIn.Num() * 3);
    int innerIndex = 0;

    //split into 3 different channels (x1...xn)(y1...yn)(t1...tn)
    for (int i = 0; i < bufferIn.Num(); i++){
        const FVector &current = bufferIn[i];
        outBuffer[innerIndex] = current.X;
        outBuffer[bufferIn.Num() + innerIndex] = current.Y;
        outBuffer[bufferIn.Num() * 2 + innerIndex] = current.Z;
        innerIndex ++;
    }
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
    ValidateAllBuffers();
    // --- todo! ---

    return false;
}




void FMeshedPolygonTrajectoryRayModel::ValidateAllBuffers(){
    ValidateEnemyHitsBuffer();
    ValidatePlayerHitsBuffer();
    ValidatePlayerTrajectoryBuffer();
    ValidatePlayerGTBuffer();
}

//append (float, float) to ground truth buffer
void FMeshedPolygonTrajectoryRayModel::AppendGroundTruth(TArray<uint8> &buffer){
    TArray<float> asFloat;
    ToFloatBufferChannels(playerHitsGroundTruth, asFloat);
    TemplateBufferStorageInterface::TAppendBuffer<float>(asFloat, buffer);

    
    
    /*float x = playerGroundTruth.X;
    float y = playerGroundTruth.Y;
    TemplateBufferStorageInterface::TAppendSingleValue<float>(x, buffer);
    TemplateBufferStorageInterface::TAppendSingleValue<float>(y, buffer);*/
}


int FMeshedPolygonTrajectoryRayModel::ResultDataSizeBytes(){
    return sizeof(float) * maxRaysPlayerVision;
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

    DebugHelper::logMessage("FMeshedPolygonTrajectoryRayModel::Process Prediction Bins");

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

    DebugHelper::logMessage("FMeshedPolygonTrajectoryRayModel::Process Prediction Floats");

    //result expected as (x,y) as float
    //moved to world space coordinates
    if(buffer.Num() % 2 == 0){
        
        //(x1...xn)(y1...yN), last of both
        int last = buffer.Num();
        int half = last / 2.0f;

        localPrediciton = FVector(buffer[half-1], buffer[last-1], 0.0f);

        //GT is in index space, therefore output / Pred is aswell
        //ove to worldspace
        playerPrediction = PositionFromIndex(localPrediciton.X, localPrediciton.Y);

        //playerPrediction = BottomLeft() + localPrediciton;
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




#include "CoreMath/algorithm/Vector/FChamferDistance.h"
float FMeshedPolygonTrajectoryRayModel::SimilarityOfSample(FMeshedPolygonTrajectoryLayeredInterface &other){
    FMeshedPolygonTrajectoryRayModel *ptr =
        TSampleCast<FMeshedPolygonTrajectoryRayModel>(other);
    if(ptr && false){ //debug false
        //do compare here (later, but needed!)
        float normFaktor = 4.0f; //4 sets

        float Loss = 0.0f;
        FIntPointChamferDistance algIntPoint;
        Loss += algIntPoint.Loss(playerHits, ptr->playerHits);
        Loss += algIntPoint.Loss(enemyHits, ptr->enemyHits);
        Loss += algIntPoint.Loss(playerHitsGroundTruth, ptr->playerHitsGroundTruth);

        FVectorChamferDistance algVec;
        Loss += algVec.Loss(playerTrajectories, ptr->playerTrajectories);

        float lossNormalized = Loss / normFaktor;
        float similarity = FMath::Clamp(1.0f - lossNormalized, 0.0f, 1.0f);
        return similarity;
    }

    return 0.0f; //by default, no deletion for now at all.
}

float FMeshedPolygonTrajectoryRayModel::sizeOfAllSets(){
    return playerHits.Num() + enemyHits.Num() + playerTrajectories.Num() + playerHitsGroundTruth.Num();
}
