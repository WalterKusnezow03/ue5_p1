#include "Image.h"
#include "StoragePlugin/Storage/ImageData/ImageLoader/ImageLoader.h"


bool Image::LoadFromPath(FString localPath){
    ImageLoader loader;
    wasLoaded = loader.LoadPNGLocalPath(localPath, data);
    return wasLoaded;
}

const TArray<TArray<FColor>> &Image::GetData(){
    return data;
}

bool Image::IsValid(){
    return wasLoaded;
}

bool Image::SizeValid()const{
    return widthX() > 0 && heightY() > 0;
}

int Image::widthX()const{
    return data.Num();
}

int Image::heightY()const{
    if(widthX() > 0){
        return data[0].Num();
    }
    return 0;
}


void Image::Setup(int x, int y){
    if(x > 0 && y > 0){
        data.SetNum(x);
        for (int i = 0; i < x; i++){
            data[i].SetNum(y);
        }
    }
}

void Image::SetPixel(int x, int y, FColor color){
    if(x >= 0 && y >= 0){
        if(x < data.Num()){
            if(y < data[x].Num()){
                data[x][y] = color;
            }
        }
    }
}


bool Image::GetColorBuffer(TArray<FColor> &buffer) const {
    if (SizeValid()){
        buffer.SetNumUninitialized(widthX() * heightY());
        uint8 *Dest = (uint8*) buffer.GetData();
        for (int i = 0; i < data.Num(); i++){
            const TArray<FColor> &current = data[i];
            void *src = (void*) current.GetData();
            int32 bytes = current.Num() * sizeof(FColor);
            FMemory::Memcpy( 
                Dest,
                src,
                bytes
            );
            Dest += bytes;
        }
        return true;
    }
    return false;
}