#pragma once

#include "CoreMinimal.h"
#include "DebugPlugin/DebugHelper.h"


//appends and reads data by template type from a 
//byte buffer
class STORAGEPLUGIN_API TemplateBufferStorageInterface {


public:


    // ---- SIZE T MUST BE FIXED ----
    template <typename T>
    static void TAppendSingleValue(
        const T &value,
        TArray<uint8> &bytes
    ){
        //TArray<T> asBuffer = {value};
        //TAppendBuffer<FVector>(asBuffer, bytes);
        
        
        int32 oldSize = bytes.Num();
        bytes.SetNumUninitialized(bytes.Num() + sizeof(T));
        uint8 *Ptr = bytes.GetData() + oldSize;
        /*
        FMemory::Memcpy( 
            void* Dest,
            const void* Src,
            SIZE_T Count
        )
        */
        FMemory::Memcpy(Ptr, &value, sizeof(T));
        Ptr += sizeof(T);
    }

    template <typename T>
    static void TLoadSingleValue(
        T &someValue,
        uint8 *& Ptr
    ){
        //example: //[count][buffer]
        FMemory::Memcpy(&someValue, Ptr, sizeof(T));
        Ptr += sizeof(T);
    }
    // ---- SIZE T MUST BE FIXED ----


    template <typename T>
    static uint8 *TEndPtr(TArray<T> &array){
        uint8 *Ptr = (uint8*) array.GetData();
        Ptr += array.Num() * sizeof(T); //move to end
        return Ptr;
    }


    static void LoadCount(
        int32 &someCount,
        uint8 *& Ptr
    );

    static void AppendCount(
        int32 someCount,
        TArray<uint8> &bytes
    );

    static bool EndReached(
        uint8 *ptr,
        TArray<uint8> &bytes
    );

    static uint8 *EndPtr(TArray<uint8> &bytes);

    template <typename T>
    static void TAppendGrid(
        TArray<TArray<T>> &data,
        TArray<uint8> &bytes
    ){
        int32 sizeGrid = data.Num();
        TAppendSingleValue<int32>(sizeGrid, bytes);
        for (int32 i = 0; i < data.Num(); i++){
            TArray<T> &current = data[i];
            TAppendBuffer<T>(current, bytes);
        }
    }

    //appends a buffer including size
    template <typename T>
    static void TAppendBuffer(
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
    static void TLoadGrid(
        TArray<TArray<T>> &data,
        uint8 *& Ptr
    ){
        int32 sizeGrid = 0;
        TLoadSingleValue<int32>(sizeGrid, Ptr);
        if(sizeGrid > 0){
            //DebugHelper::logMessage("TLoadGrid Size Buffer ", sizeGrid);
            data.SetNum(sizeGrid);
            for (int32 i = 0; i < data.Num(); i++){
                TArray<T> &current = data[i];
                TLoadBuffer<T>(current, Ptr);
            }
        }
    }


    //loads a buffer 
    template <typename T>
    static void TLoadBuffer(
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


};