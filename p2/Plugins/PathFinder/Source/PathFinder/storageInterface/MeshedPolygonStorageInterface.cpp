#include "MeshedPolygonStorageInterface.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"

#include "DebugPlugin/DebugHelper.h"

#include "StoragePlugin/Storage/Template/TemplateBufferStorageInterface.h"

void MeshedPolygonStorageInterface::AppendPolygons(
    TArray<uint8> &bytes,
    APathFinder *pathFinder
){
    if(pathFinder){
        std::vector<FMeshedPolygon *> polygons = pathFinder->GetAllPolygons();
        AppendPolygons(bytes, polygons);
        DebugHelper::logMessage("MeshedPolygonStorageInterface::Saved Polygons", polygons.size());
    }
}

void MeshedPolygonStorageInterface::AppendPolygons(
    TArray<uint8> &bytes,
    std::vector<FMeshedPolygon *> &polygons
){
    int countPolygons = polygons.size();
    if(countPolygons <= 0){
        return;
    }

    //new
    TemplateBufferStorageInterface::AppendCount(countPolygons, bytes);
    for (int i = 0; i < polygons.size(); i++){
        if(FMeshedPolygon *current = polygons[i]){
            current->AppendAsBinary(bytes);
        }
    }


    /*
    AppendCount(countPolygons, bytes);
    for (int i = 0; i < countPolygons; i++){
        AppendPolygon(bytes, polygons[i]);
    }*/
}


void MeshedPolygonStorageInterface::LoadPolygons(
    TArray<uint8> &bytes,
    uint8 *&Ptr,
    APathFinder *pathFinder
){
    if(pathFinder){
        std::vector<FMeshedPolygon *> polygonsOut;
        LoadPolygons(bytes, Ptr, polygonsOut);
        pathFinder->addAllPolygons(polygonsOut);
        DebugHelper::logMessage("MeshedPolygonStorageInterface::Loaded Polygons", polygonsOut.size());
    }
}

void MeshedPolygonStorageInterface::LoadPolygons(
    TArray<uint8> &bytes,
    uint8 *&Ptr,
    std::vector<FMeshedPolygon *> &polygonsOut
){
    if(TemplateBufferStorageInterface::EndReached(Ptr, bytes)){
        return;
    }
    int32 countPolygons = 0;
    TemplateBufferStorageInterface::LoadCount(countPolygons, Ptr);
    for (int i = 0; i < countPolygons; i++){
        FMeshedPolygon *newPolygon = new FMeshedPolygon();
        if(newPolygon->LoadFromBinary(bytes, Ptr)){
            polygonsOut.push_back(newPolygon);
        }else{
            //exceed size
            delete newPolygon;
            newPolygon = nullptr;
        }
    }

    /*
    virtual void AppendAsBinary(
        TArray<uint8> &buffer
    );
    virtual bool LoadFromBinary(
        TArray<uint8> &buffer,
        uint8 *& Ptr
    );

    */




    /*if(EndReached(Ptr, bytes)){
        //doesnt have polygons
        return;
    }
    int countPolygons = 0;
    LoadCount(countPolygons, Ptr);

    for (int i = 0; i < countPolygons; i++){
        FMeshedPolygon *newPolygon = new FMeshedPolygon();
        if(LoadPolygon(bytes, Ptr, *newPolygon)){
            polygonsOut.push_back(newPolygon);
        }else{
            //exceed size
            delete newPolygon;
            newPolygon = nullptr;
        }
    }*/
}









//deprecated
void MeshedPolygonStorageInterface::AppendPolygon(
    TArray<uint8> &bytes,
    FMeshedPolygon *polygon
){
    if(polygon){
        if(polygon->IsValid()){
            TArray<TArray<uint8>> &flags = polygon->GetFlagGrid();
            
            float stepSize = polygon->GetStepSizeSaved();

            FVector min, max;
            polygon->GetMinMax(min, max);
            // OverrideMinMax(FVector &minIn, FVector &maxIn)
            // GetMinMax(FVector &minOut, FVector &maxOut)

            //save polygon binary as
            //[stepSize][min,max][boolFlags][positions]
            AppendStepSize(stepSize, bytes);
            AppendMinMax(min, max, bytes);

            //append count buffers
            //append buffers
            AppendCount(flags.Num(), bytes);
            for (int i = 0; i < flags.Num(); i++){
                TArray<uint8> &flagBuffer = flags[i];
                TAppendBuffer<uint8>(flagBuffer, bytes);
            }


            //append coutn buffers
            /*TArray<TArray<FVector>> &positions = polygon->GetPositionGrid();
            AppendCount(positions.Num(), bytes);
            for (int i = 0; i < positions.Num(); i++){
                TArray<FVector> &posBuffer = positions[i];
                TAppendBuffer<FVector>(posBuffer, bytes);
            }*/
        }
    }
}

uint8 *MeshedPolygonStorageInterface::EndPtr(TArray<uint8> &bytes){
    uint8 *Ptr = bytes.GetData();
    Ptr += bytes.Num(); //move to end
    return Ptr;
}

void MeshedPolygonStorageInterface::AppendStepSize(
    float stepSize,
    TArray<uint8> &bytes
){
    int32 oldSize = bytes.Num();
    int32 bytesFloat = sizeof(float);
    bytes.SetNumUninitialized(bytes.Num() + bytesFloat);

    uint8 *Ptr = bytes.GetData() + oldSize; //if array reallocatde, mak new ptr
    
    /*
    FMemory::Memcpy( 
        void* Dest,
        const void* Src,
        SIZE_T Count
    )
    */
    FMemory::Memcpy(Ptr, &stepSize, bytesFloat);
    Ptr += bytesFloat;
}


void MeshedPolygonStorageInterface::LoadStepSize(
    float &stepSize,
    uint8 *& Ptr
){
    int32 bytesFloat = sizeof(float);
    /*
    FMemory::Memcpy( 
        void* Dest,
        const void* Src,
        SIZE_T Count
    )
    */
    FMemory::Memcpy(&stepSize, Ptr, bytesFloat);
    Ptr += bytesFloat;
}



void MeshedPolygonStorageInterface::AppendMinMax(
    const FVector &min,
    const FVector &max,
    TArray<uint8> &bytes
){
    TArray<FVector> asBuffer = {min, max};
    TAppendBuffer<FVector>(asBuffer, bytes);
}

void MeshedPolygonStorageInterface::LoadMinMax(FVector &min, FVector &max, uint8 *& Ptr){
    TArray<FVector> outBuffer;
    TLoadBuffer<FVector>(outBuffer, Ptr);
    if(outBuffer.Num() == 2){
        min = outBuffer[0];
        max = outBuffer[1];
    }
}

void MeshedPolygonStorageInterface::AppendCount(
    int32 someCount,
    TArray<uint8> &bytes
){
    int32 oldSize = bytes.Num();
    bytes.SetNumUninitialized(bytes.Num() + sizeof(int32));
    uint8 *Ptr = bytes.GetData() + oldSize;
    /*
    FMemory::Memcpy( 
        void* Dest,
        const void* Src,
        SIZE_T Count
    )
    */
    FMemory::Memcpy(Ptr, &someCount, sizeof(int32));
    Ptr += sizeof(int32);
}



void MeshedPolygonStorageInterface::LoadCount(
    int32 &someCount,
    uint8 *& Ptr
){ 
    /*
    FMemory::Memcpy( 
        void* Dest,
        const void* Src,
        SIZE_T Count
    )
    */
    FMemory::Memcpy(&someCount, Ptr, sizeof(int32));
    Ptr += sizeof(int32);
}



bool MeshedPolygonStorageInterface::EndReached(
    uint8 *ptr,
    TArray<uint8> &bytes
){
    if(ptr){
        uint8 *end = EndPtr(bytes);
        return ptr >= end; //moving oob and equals
    }
    return true;
}





bool MeshedPolygonStorageInterface::LoadPolygon(
    TArray<uint8> &bytes,
    uint8 *& Ptr,
    FMeshedPolygon &polygon
){
    if(EndReached(Ptr, bytes)){
        return false;
    }

    float stepSize = 0.0f;
    LoadStepSize(stepSize, Ptr);
    polygon.OverrideStepSize(stepSize);
    //min max

    FVector min, max;
    LoadMinMax(min, max, Ptr);
    polygon.OverrideMinMax(min, max);
    // GetMinMax(FVector &minOut, FVector &maxOut)




    //flags
    TArray<TArray<uint8>> &flags = polygon.GetFlagGrid();


    //load flag count buffers
    int32 countFlagBuffers = 0;
    LoadCount(countFlagBuffers, Ptr);
    if(countFlagBuffers > 0){
        flags.SetNum(countFlagBuffers);
        // load flags
        for (int i = 0; i < countFlagBuffers; i++){
            TArray<uint8> &flagsColumn = flags[i];
            TLoadBuffer<uint8>(flagsColumn, Ptr);
        }
    }
    
    /*
    TArray<TArray<FVector>> &positions = polygon.GetPositionGrid();
    //load pos count buffers
    int32 countPosBuffers = 0;
    LoadCount(countPosBuffers, Ptr);
    if(countPosBuffers > 0){
        positions.SetNum(countPosBuffers);
        // load pos buffers
        for (int i = 0; i < countPosBuffers; i++){
            TArray<FVector> &posColumn = positions[i];
            TLoadBuffer<FVector>(posColumn, Ptr);
        }
    }*/
    return true;
}