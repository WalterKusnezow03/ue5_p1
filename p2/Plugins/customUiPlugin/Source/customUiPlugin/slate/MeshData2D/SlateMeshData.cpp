#include "SlateMeshData.h"

SlateMeshData::SlateMeshData(){

}

SlateMeshData::~SlateMeshData(){

}

TArray<FVector2D> &SlateMeshData::VerteciesRef(){
    return Vertecies;
}


TArray<SlateIndex> &SlateMeshData::TrianglesRef(){
    return Triangles;
}


const TArray<FVector2D> &SlateMeshData::VerteciesRefConst()const {
    return Vertecies;
}



const TArray<SlateIndex> &SlateMeshData::TrianglesRefConst()const{
    return Triangles;
}






void SlateMeshData::Append(FVector2D &a, FVector2D &b, FVector2D &c){
    if(!TriangleCanBeAdded()){
        return;
    }

    int numCurrent = Vertecies.Num();
    /*
    1-->2
    |   |
    0<--3
    */
    uint16 indexA = numCurrent;
    uint16 indexB = numCurrent + 1;
    uint16 indexC = numCurrent + 2;

    Vertecies.Add(a);
    Vertecies.Add(b);
    Vertecies.Add(c);

    Triangles.Add(indexA);
    Triangles.Add(indexB);
    Triangles.Add(indexC);
}

void SlateMeshData::AppendEfficent(FVector2D &a, FVector2D &b, FVector2D &c){
    if(!TriangleCanBeAdded()){
        return;
    }


    uint16 indexA = Vertecies.Num(); //saved index before adding to buffer. Will be valid.
    if(HasVertex(a, indexA)){
        Triangles.Add(indexA);
    }else{
        Vertecies.Add(a);
        Triangles.Add(indexA);
    }



    uint16 indexB = Vertecies.Num(); //saved index before adding to buffer. Will be valid.
    if(HasVertex(b, indexB)){
        Triangles.Add(indexB);
    }else{
        Vertecies.Add(b);
        Triangles.Add(indexB);
    }



    uint16 indexC = Vertecies.Num(); //saved index before adding to buffer. Will be valid.
    if(HasVertex(c, indexC)){
        Triangles.Add(indexC);
    }else{
        Vertecies.Add(c);
        Triangles.Add(indexC);
    }


}

void SlateMeshData::AppendClosedShape(TArray<FVector2D> &shape){
    if(shape.Num() > 0){
        FVector2D center = CenterOf(shape);
        for (int i = 0; i < shape.Num(); i++)
        {
            int nextIndex = (i + 1) % shape.Num();

            //Buffer expected to be CLOCKWISE
            /*
            triangle added:
            1------->2
            |        |
            center<---

            next triangle:
            2------->3
            |        |
            center<---

            ...

            last triangle closing to front
            n------->0
            |        |
            center<---
            */

            FVector2D &v1 = shape[i];
            FVector2D &v2 = shape[nextIndex];
            AppendEfficent(center, v1, v2);
        }
    }
}


FVector2D SlateMeshData::CenterOf(TArray<FVector2D> &buffer){
    FVector2D sum(0, 0);
    int count = buffer.Num();
    if(count <= 0){
        return sum;
    }
    for (int i = 0; i < buffer.Num(); i++){
        sum += buffer[i];
    }
    return sum / count;
}

bool SlateMeshData::HasVertex(FVector2D &target, uint16 &outIndex){
    if(Vertecies.Num() <= 0){
        return false;
    }

    int index = ClosestVertexBelowEpsilon(target);
    if(isValidVertexIndex(index)){
        outIndex = index;
        return true;
    }
    return false;
}




int SlateMeshData::ClosestVertexBelowEpsilon(FVector2D &target){
    int index = ClosestVertex(target);
    if(isValidVertexIndex(index)){
        float dist = (target - Vertecies[index]).Size();
        if(dist < epsilon){
            return index;
        }
    }
    return -1;
}

int SlateMeshData::ClosestVertex(FVector2D &target){
    if(Vertecies.Num() <= 0){
        return -1;
    }

    
    int closest = 0;
    float dist = (Vertecies[0] - target).SizeSquared();
    
    for (int i = 1; i < Vertecies.Num(); i++){
        FVector2D &current = Vertecies[i];
        float distanceCurrent = (current - target).SizeSquared();
        if(distanceCurrent < dist){
            closest = i;
            dist = distanceCurrent;
        }
    }
    return closest;
}


bool SlateMeshData::TriangleCanBeAdded(){
    return Vertecies.Num() + 3 < MaxSizeVertexBuffer;
}

bool SlateMeshData::isValidVertexIndex(int i){
    return i >= 0 && i < Vertecies.Num();
}






// ---- SLATE Vertecies / MAKE BUFFER FOR DRAW -----
const TArray<FSlateVertex> SlateMeshData::MakeSlateVertexBuffer(
    FSlateRenderTransform &RenderTransform
)const{

    //set size to prevent copy
    TArray<FSlateVertex> outBuffer;
    outBuffer.SetNumUninitialized(Vertecies.Num());

    for (int i = 0; i < Vertecies.Num(); i++){
        outBuffer[i] = makeSlateVertex(Vertecies[i], RenderTransform);
    }
    return outBuffer;
}

FSlateVertex SlateMeshData::makeSlateVertex(
    const FVector2D &Position,
    const FSlateRenderTransform &RenderTransform
) const {
    FVector2f PosAs2F = FVector2f(Position.X, Position.Y);
    FVector2f UV(0,0); // wenn keine Textur, 0,0 ok
    FColor Color = InterpolatedColorFor(Position).ToFColor(true);

    //static FSlateVertex Make(
    //  const FSlateRenderTransform& RenderTransform, 
    //  const FVector2f InLocalPosition, 
    //  const FVector2f InTexCoord, 
    //  const FVector2f InTexCoord2, 
    //  const FColor InColor, 
    //  const FColor SecondaryColor = FColor()
    //)
    

    FSlateVertex Vertex = FSlateVertex::Make<ESlateVertexRounding::Disabled>(
        RenderTransform,
        PosAs2F,
        UV,
        Color,
        Color
        //,ESlateVertexRounding::Disabled
    );
    return Vertex;
}







// ----- Color interpolation ------





FLinearColor SlateMeshData::InterpolatedColorFor(
    const FVector2D &pos
) const {
    //A + r (B - A) color needed!

    return FLinearColor::Red;
}