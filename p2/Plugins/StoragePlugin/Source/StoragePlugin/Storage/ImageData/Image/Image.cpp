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

bool Image::SizeValid(){
    return widthX() > 0 && heightY() > 0;
}

int Image::widthX(){
    return data.Num();
}

int Image::heightY(){
    if(widthX() > 0){
        return data[0].Num();
    }
    return 0;
}