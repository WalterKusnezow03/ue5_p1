#pragma once

#include "CoreMinimal.h"
#include "PolygonPlugin/Public/Polygons/MeshedPolygon.h"


/// ----> move into FMeshedPolygonClass !

/// @brief Appends and loads FMeshedPolygons data to and from byte arrays.
class PATHFINDER_API MeshedPolygonStorageInterface {

public:
    void AppendPolygons(
        TArray<uint8> &bytes,
        APathFinder *pathFinder
    );

    void LoadPolygons(
        TArray<uint8> &bytes,
        uint8 *&Ptr,
        APathFinder *pathFinder
    );

protected:

    void AppendPolygons(
        TArray<uint8> &bytes,
        std::vector<FMeshedPolygon *> &polygons
    );

    void LoadPolygons(
        TArray<uint8> &bytes,
        uint8 *&ptr,
        std::vector<FMeshedPolygon *> &polygonsOut
    );

    void AppendPolygon(
        TArray<uint8> &bytes,
        FMeshedPolygon *polygon
    );

    //returns true if end reached.
    bool LoadPolygon(
        TArray<uint8> &bytes,
        uint8 *& Ptr,
        FMeshedPolygon &polygon
    );


    uint8 *EndPtr(TArray<uint8> &array);

    void AppendStepSize(
        float stepSize,
        TArray<uint8> &bytes
    );

    void LoadStepSize(
        float &stepSize,
        uint8 *&Ptr
    );

    void AppendCount(
        int someCount,
        TArray<uint8> &bytes
    );

    void LoadCount(
        int &someCount,
        uint8 *&Ptr
    );


    template <typename T>
    void TAppendBuffer(
        TArray<T> &data,
        TArray<uint8> &bytes
    ){
        int32 oldSize = bytes.Num();

        int32 count = data.Num(); //count data
        int32 bytesSize = sizeof(T) * count; //siez for data
        bytes.SetNumUninitialized(bytes.Num() + bytesSize + sizeof(int32)); //appended bytes and single count 
        uint8 *Ptr = bytes.GetData() + oldSize;
        /*
        FMemory::Memcpy( 
            void* Dest,
            const void* Src,
            SIZE_T Count
        )
        */
        //[count][buffer]
        FMemory::Memcpy(Ptr, &count, sizeof(int32));
        Ptr += sizeof(int32);
        if(count > 0){
            /*FMemory::Memcpy( 
                void* Dest,
                const void* Src,
                SIZE_T Count
            )*/
            FMemory::Memcpy(Ptr, data.GetData(), bytesSize);
            Ptr += bytesSize;
        }
        
    }

    template <typename T>
    void TLoadBuffer(
        TArray<T> &data,
        uint8 *& Ptr
    ){
        int32 count = 0;
        //[count][buffer]
        FMemory::Memcpy(&count, Ptr, sizeof(int32));
        Ptr += sizeof(int32);

        if(count > 0){
            //increase buffer size
            int32 oldSize = data.Num() * sizeof(T);
            data.SetNumUninitialized(data.Num() + count);

            uint8 *ptrData = (uint8 *)data.GetData() + oldSize;
            
            /*FMemory::Memcpy( 
                void* Dest,
                const void* Src,
                SIZE_T Count
            )*/
            int32 bytesCopied = count * sizeof(T);
            FMemory::Memcpy(ptrData, Ptr, bytesCopied); // append data = count * sizeT
            Ptr += bytesCopied;
        }

        

    }

    bool EndReached(
        uint8 *ptr,
        TArray<uint8> &bytes
    );


    template <typename T>
    uint8 *TEndPtr(TArray<T> &array){
        uint8 *Ptr = (uint8*) array.GetData();
        Ptr += array.Num() * sizeof(T); //move to end
        return Ptr;
    }

    void AppendMinMax(const FVector &min, const FVector &max, TArray<uint8> &bytes);
    void LoadMinMax(FVector &min, FVector &max, uint8 *&Ptr);
};