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


/// ---- todo: flip x and y axis completly! ----

int Image::widthX()const{
    //return data.Num();
    if(heightY() > 0){
        return data[0].Num();
    }
    return 0;
}

int Image::heightY()const{
    /*if(widthX() > 0){
        return data[0].Num();
    }
    return 0;*/
    return data.Num();
}


void Image::Setup(int x, int y){
    if(x > 0 && y > 0){
        /*data.SetNum(x);
        for (int i = 0; i < x; i++){
            data[i].SetNum(y);
        }*/
        data.SetNum(y);
        for (int i = 0; i < y; i++){
            data[i].SetNum(x);
        }
    }
}

void Image::SetPixel(int x, int y, FColor color){
    if(x >= 0 && y >= 0){
        /*if(x < data.Num()){
            if(y < data[x].Num()){
                data[x][y] = color;
            }
        }*/
        if(y < data.Num()){
            if(x < data[y].Num()){
                data[y][x] = color;
            }
        }
    }
}

FColor Image::GetPixel(int x, int y) const{
    if(y >= 0 && y < data.Num()){
        if(x >= 0 && x < data[y].Num()){
            return data[y][x];
        }
    }
    return FColor::Black;
}


void Image::AddPixel(int x, int y, FColor color){
    if(InBound(x,y)){
        data[y][x] = ClampedSum(data[y][x], color);
    }
}

FColor Image::ClampedSum(const FColor &a, const FColor &b){
    FColor result;
    result.R = CampedSumInt(a.R, b.R);
    result.G = CampedSumInt(a.G, b.G);
    result.B = CampedSumInt(a.B, b.B);
    result.A = CampedSumInt(a.A, b.A);
    return result;
}

uint8 Image::CampedSumInt(uint8 a, uint8 b){
    int intA = a;
    int intB = b;
    int result = std::max(a + b, 0);
    result = std::min(result, 255);
    return result;
}




bool Image::InBound(int x, int y){
    if(x >= 0 && y >= 0){
        if(y < heightY()){
            if(x < widthX()){
                return true;
            }
        }
    }
    return false;
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


void Image::Transpose(){
    if(SizeValid()){
        TArray<TArray<FColor>> newData;
        int w = widthX();
        int h = heightY();
        newData.SetNum(w);
        for (int x = 0; x < w; x++){
            newData[x].SetNum(h);
            for (int y = 0; y < h; y++){
                newData[x][y] = data[y][x];
            }
        }
        data = MoveTemp(newData);
    }
}


void Image::FlipX(){
    if(SizeValid()){
        TArray<TArray<FColor>> newData = data;
        int w = widthX();
        int h = heightY();

        for (int x = 0; x < w; x++){
            int xOther = w - x - 1;

            for (int y = 0; y < h; y++){
                newData[y][x] = data[y][xOther];
            }
        }
        data = MoveTemp(newData);
    }
}


void Image::SetAlpha(int alpha){
    alpha = std::abs(alpha);
    alpha = std::max(std::min(alpha, 255), 0);
    for (int i = 0; i < data.Num(); i++){
        TArray<FColor> &row = data[i];
        for (int j = 0; j < row.Num(); j++){
            FColor &color = row[j];
            color.A = alpha;
        }
    }
}