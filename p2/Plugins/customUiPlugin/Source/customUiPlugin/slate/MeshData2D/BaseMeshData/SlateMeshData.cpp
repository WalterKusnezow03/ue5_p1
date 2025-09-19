#include "SlateMeshData.h"
#include "CoreMath/vector/bound/FBoundingBox2D.h"

SlateMeshData::SlateMeshData(){
    bCursorColorEnabled = false;
}

SlateMeshData::~SlateMeshData(){

}

SlateMeshData::SlateMeshData(const SlateMeshData &other){
    if(this != &other){
        *this = other;
    }
}
SlateMeshData &SlateMeshData::operator=(const SlateMeshData &other){
    if(this != &other){
        Vertecies = other.Vertecies;
        Triangles = other.Triangles;
        boundingBox = other.boundingBox;
        slateVertexCache = other.slateVertexCache;

        ambientColorsInvertedSpace = other.ambientColorsInvertedSpace;
        bCursorColorEnabled = other.bCursorColorEnabled;
        cursorColorPair = other.cursorColorPair;

        fullColorEnabled = other.fullColorEnabled;
        fullColor = other.fullColor;

        bHasRuntimeTransformation = other.bHasRuntimeTransformation;
        transformationRuntimeMatrix = other.transformationRuntimeMatrix;
    }
    return *this;
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

TArray<FVector2f> SlateMeshData::VerteciesAs2f() const{//for line draw
    TArray<FVector2f> outBuffer;
    outBuffer.SetNumUninitialized(Triangles.Num());
    for (int t = 2; t < Triangles.Num(); t++)
    {
        const FVector2D &v0 = Vertecies[Triangles[t - 2]];
        const FVector2D &v1 = Vertecies[Triangles[t - 1]];
        const FVector2D &v2 = Vertecies[Triangles[t]];

        outBuffer[t - 2] = ConvertTo2f(v0);
        outBuffer[t - 1] = ConvertTo2f(v1);
        outBuffer[t] = ConvertTo2f(v2);
    }

    return outBuffer;
}

void SlateMeshData::Clear(){
    boundingBox.Reset();
    Vertecies.Empty();
    Triangles.Empty();
    FlagCacheUpdateNeeded();
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

    boundingBox.Update(a, b, c);

    FlagCacheUpdateNeeded();

}

void SlateMeshData::Append(FVector2D &a, FVector2D &b, FVector2D &c, FVector2D &d){
    AppendEfficent(a, b, c);
    AppendEfficent(a, c, d);
}

void SlateMeshData::AppendQuad(FVector2D &bottomLeft, FVector2D &topRight){
    /*
    1-->2
    |   |
    0<--3
    */
    FVector2D v0(
        std::min(bottomLeft.X, topRight.X),
        std::min(bottomLeft.Y, topRight.Y)
    );
    FVector2D v1(
        std::min(bottomLeft.X, topRight.X),
        std::max(bottomLeft.Y, topRight.Y)
    );
    FVector2D v2(
        std::max(bottomLeft.X, topRight.X),
        std::max(bottomLeft.Y, topRight.Y)
    );
    FVector2D v3(
        std::max(bottomLeft.X, topRight.X),
        std::min(bottomLeft.Y, topRight.Y)
    );
    Append(v0, v1, v2, v3);
}

void SlateMeshData::AppendEfficentTriangleShapedBuffer(TArray<FVector2D> &triangleShapedBuffer){
    for (int i = 2; i < triangleShapedBuffer.Num(); i += 3){
        AppendEfficent(
            triangleShapedBuffer[i - 2],
            triangleShapedBuffer[i - 1],
            triangleShapedBuffer[i]
        );
    }
}

void SlateMeshData::AppendEfficentQuadShapedBuffer(TArray<FVector2D> &quadShapedBuffer){
    for (int i = 3; i < quadShapedBuffer.Num(); i += 4){
        AppendEfficent(
            quadShapedBuffer[i - 3],
            quadShapedBuffer[i - 2],
            quadShapedBuffer[i - 1],
            quadShapedBuffer[i]
        );
    }
}


void SlateMeshData::AppendEfficent(FVector2D &a, FVector2D &b, FVector2D &c, FVector2D &d){
    /*
    1b-->2c
    |   |
    0a<--3d
    */
    AppendEfficent(a, b, c);
    AppendEfficent(a, c, d);
}

void SlateMeshData::AppendEfficent(FVector2D &a, FVector2D &b, FVector2D &c){
    if(!TriangleCanBeAdded()){
        return;
    }

    bool bNewVerteciesMade = false;

    uint16 indexA = Vertecies.Num(); //saved index before adding to buffer. Will be valid.
    if(HasVertex(a, indexA)){
        Triangles.Add(indexA);
    }else{
        Vertecies.Add(a);
        Triangles.Add(indexA);
        boundingBox.Update(a);
    }



    uint16 indexB = Vertecies.Num(); //saved index before adding to buffer. Will be valid.
    if(HasVertex(b, indexB)){
        Triangles.Add(indexB);
    }else{
        Vertecies.Add(b);
        Triangles.Add(indexB);
        boundingBox.Update(b);
    }



    uint16 indexC = Vertecies.Num(); //saved index before adding to buffer. Will be valid.
    if(HasVertex(c, indexC)){
        Triangles.Add(indexC);
    }else{
        Vertecies.Add(c);
        Triangles.Add(indexC);
        boundingBox.Update(c);
    }

    FlagCacheUpdateNeeded();


}

void SlateMeshData::AppendEfficent(
    FVector2D &v0,
    FVector2D &v1,
    FVector2D &v2,
    int detail //detail to split the triangle in
){
    if(detail <= 0){
        AppendEfficent(v0, v1, v2);
    }

    TArray<FVector2D> triangleShapedBuffer = {v0, v1, v2};
    for (int i = 0; i < detail; i++){
        TArray<FVector2D> modifiedBuffer;
        Split(triangleShapedBuffer, modifiedBuffer);
        triangleShapedBuffer = modifiedBuffer;
    }
    AppendEfficentTriangleShapedBuffer(triangleShapedBuffer);
}

void SlateMeshData::Split(
    TArray<FVector2D> &originalBuffer, //all triangles splitted in half along longest edge
    TArray<FVector2D> &appendToTriangleShapedBuffer
){
    for (int i = 2; i < originalBuffer.Num(); i += 3){
        FVector2D &v0 = originalBuffer[i - 2];
        FVector2D &v1 = originalBuffer[i - 1];
        FVector2D &v2 = originalBuffer[i];
        Split(v0, v1, v2, appendToTriangleShapedBuffer);
    }
}


void SlateMeshData::Split(
    FVector2D &a,
    FVector2D &b,
    FVector2D &c,
    TArray<FVector2D> &appendToTriangleShapedBuffer //buffer contains triangles with duplicates
){
    /*
    1-->2
    |  |
    0<|

    to

    1-->2
    |   |
    new|
    | |
    0<-
    (new,1,2)(0,new,2)
    */

    int v0Index = 0;
    int v1Index = 1;
    int v2IndexLeft = 2;
    float maxEdge = 0.0f;
    TArray<FVector2D *> verts = {&a, &b, &c};
    for (int i = 0; i < verts.Num(); i++){
        int nextIndex = (i + 1) % verts.Num();
        float dist = FVector2D::DistSquared(*verts[i], *verts[nextIndex]);
        if(dist > maxEdge){
            v0Index = i;
            v1Index = nextIndex;
            v2IndexLeft = (nextIndex + 1) % verts.Num(); //left index, not part of split
            maxEdge = dist;
        }
    }

    //es wird immer zwischen 2 vertecies geteilt.
    //verbunden wird immer über (v0, new, v2) und (new, v1, v2)
    //je einsetzbar

    FVector2D &v0 = *verts[v0Index];
    FVector2D &v1 = *verts[v1Index];
    FVector2D &v2 = *verts[v2IndexLeft];
    FVector2D vNew = (v0 + v1) / 2.0f;

    int32 num = appendToTriangleShapedBuffer.Num();
    int32 increased = num + 6;
    appendToTriangleShapedBuffer.SetNumUninitialized(increased);
    appendToTriangleShapedBuffer[num] = v0;
    appendToTriangleShapedBuffer[num + 1] = vNew;
    appendToTriangleShapedBuffer[num + 2] = v2;

    int32 numOffset = num + 3; //clearer to read
    appendToTriangleShapedBuffer[numOffset] = vNew;
    appendToTriangleShapedBuffer[numOffset + 1] = v1;
    appendToTriangleShapedBuffer[numOffset + 2] = v2;

}


void SlateMeshData::AppendClosedShape(TArray<FVector2D> &shape, int detail){
    if(shape.Num() > 0){

        //precalculate count triangles, cap detail if buffer overflow of 65535
        //not precise, taking duplicate vertecies ok
        
        //vertecies closed at center at worst = v(n) = n * 3

        //recursion for triangle: r(n) = 2^n * 3

        //combined  v(n,m) = v(n) * r(m)
        //          v(n,m) = (n * 3) * (2^m * 3)
        int pow = FMath::Pow(2.0f, detail);
        int inner = shape.Num() * 3 * 3;
        int estimatedCountWorst = inner * pow;
        if(estimatedCountWorst >= MaxSizeVertexBuffer){
            //inner * x = max
            //x = max / inner
            float x = MaxSizeVertexBuffer / inner;

            //2^n = x
            //log2(x) = n
            detail = FMath::Log2(x) - 1.0f; //-1 for safety

            if(false){
                UiDebugHelper::logMessage(
                    FString::Printf(
                        TEXT("SlateMeshData kept buffer size, detail: (%d)"),
                        detail
                    )
                );
            }
            
        }

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
            AppendEfficent(center, v1, v2, detail); //detail if extra split needed.
        }
    }
}




void SlateMeshData::AppendClosedShape(TArray<FVector2D> &shape){
    AppendClosedShape(shape, 0); //no inner split
}

FVector2D SlateMeshData::CenterOfMesh()const{
    return CenterOf(Vertecies);
}

FVector2D SlateMeshData::CenterOf(const TArray<FVector2D> &buffer) const{
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
void SlateMeshData::FlagCacheUpdateNeeded(){
    slateVertexCache.MarkCacheUpdateNeededFlagTrue();
}

const TArray<FSlateVertex> &SlateMeshData::GetSlateVertexBuffer(
    FSlateRenderTransform &RenderTransform
)const{

    //get ref, update if changes made
    TArray<FSlateVertex> &outBuffer = slateVertexCache.mutableCachedBufferRef(Vertecies.Num());
    
    //update referenced buffer, is mutable.
    if(slateVertexCache.CacheUpdateNeeded()){
        slateVertexCache.ResetCacheUpdateNeededFlag(); //new calculation made, reset

        for (int i = 0; i < Vertecies.Num(); i++){
            outBuffer[i] = makeSlateVertex(Vertecies[i], RenderTransform);
        }
    }
    
    return outBuffer;
}

///@brief makes slate vertex and applies runtime transformation matrix (internal extra matrix)
/// After color is set!
FSlateVertex SlateMeshData::makeSlateVertex(
    const FVector2D &Position,
    const FSlateRenderTransform &RenderTransform
) const {
    //apply color
    FColor Color = InterpolatedColorFor(Position).ToFColor(true); //true gamma korrektur.(?)

    //UV - ignored
    FVector2f UV = MakeUV(Position);

    //apply runtime transform
    FVector2f PosAs2F(Position.X, Position.Y);
    if (bHasRuntimeTransformation){
        ApplyTransformationConst(transformationRuntimeMatrix, PosAs2F);
    }
   
    

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
        Color
        //,Color
        //,ESlateVertexRounding::Disabled
    );
    return Vertex;
}





// ----- External Color -----
void SlateMeshData::UpdateCursorPosition(
    FVector2D &position, 
    bool bDynamicColoring
){
    if(bCursorColorEnabled != bDynamicColoring){
        FlagCacheUpdateNeeded();
    }
    if(cursorColorPair.UpdatePosition(position)){
        FlagCacheUpdateNeeded();
    }

    bCursorColorEnabled = bDynamicColoring;
}

void SlateMeshData::UpdateCursorColor(FLinearColor &color){
    if(cursorColorPair.UpdateColor(color)){
        FlagCacheUpdateNeeded();
    }
}


void SlateMeshData::AddAmbientUvColor(
    FVector2D uvPos, 
    FLinearColor color
){
    FVector2D widgetSpace = convertUVInvertedToVertexBufferSpace(uvPos);
    FPairColorPosition newPair(color, widgetSpace, uvPos);
    ambientColorsInvertedSpace.Add(newPair);
    FlagCacheUpdateNeeded();
}

void SlateMeshData::ClearAmbientColors(){
    ambientColorsInvertedSpace.Empty();
    FlagCacheUpdateNeeded();
}

void SlateMeshData::ReCalculateAmbientUvColors(){
    TArray<FPairColorPosition> copy = ambientColorsInvertedSpace;
    ClearAmbientColors();
    for (int i = 0; i < copy.Num(); i++)
    {
        FPairColorPosition &current = copy[i];
        AddAmbientUvColor(current.uv, current.color);
    }
    FlagCacheUpdateNeeded();
}

void SlateMeshData::SetFullColor(FLinearColor color){
    fullColorEnabled = true;
    fullColor = color;
}

void SlateMeshData::ResetFullColor(){
    fullColorEnabled = false;
}

///@brief converts a uv, to inverted uv and then to vertex buffer space, speeds
///up color calculation because the scalar is found by one division (less operations)
///is converted to a real position, because mesh data might change but the
///color vertex should stay in place.
FVector2D SlateMeshData::convertUVInvertedToVertexBufferSpace(const FVector2D &uv){

    FVector2D uvCopy = uv;
    uvCopy.X = 1.0f - uvCopy.X;
    uvCopy.Y = 1.0f - uvCopy.Y;

    //gx = A + t (B - A)
    float dirX = boundingBox.sizeX();
    float dirY = boundingBox.sizeY();

    FVector2D gx = boundingBox.topLeft;
    gx.X += dirX * uvCopy.X; //go in direciton on x and y axis.
    gx.Y += dirY * uvCopy.Y;

    return gx;
}

FVector2f SlateMeshData::MakeUV(const FVector2D &vertex) const {
    FVector2f outUv;

    FVector2D relative = vertex - boundingBox.min(); // AB = B - A, from min coordinate

    //scalar = distTarget / distAll
    int sizeX = boundingBox.sizeX();
    int sizeY = boundingBox.sizeY();

    outUv.X = relative.X / sizeX;
    outUv.Y = relative.Y / sizeY;

    outUv.X = FMath::Clamp(outUv.X, 0.0f, 1.0f);
    outUv.Y = FMath::Clamp(outUv.Y, 0.0f, 1.0f);
    return outUv;
}

// ----- Color interpolation ------

FLinearColor SlateMeshData::InterpolatedColorFor(
    const FVector2D &pos
) const {
    if(fullColorEnabled){
        return fullColor;
    }


    //if empty color buffer and no cursor enabled, return default Color
    if(!fullColorEnabled && !bCursorColorEnabled && ambientColorsInvertedSpace.Num() <= 0){
        return FLinearColor::Blue;
    }

    //A + r (B - A) color needed!

    //dist from ambient colors makes a mixed color

    //closest color: 1.0
    //furthest color: 0.0
    FLinearColor accumulatedColor = FLinearColor::Black; //start with black color, otherwise its broken!

    if(!fullColorEnabled){
        float totalDistance = 0.0f;
        if(ambientColorsInvertedSpace.Num() > 0){
            TArray<float> distances;
            distances.SetNumUninitialized(ambientColorsInvertedSpace.Num());
            for (int i = 0; i < ambientColorsInvertedSpace.Num(); i++)
            {
                const FPairColorPosition &pair = ambientColorsInvertedSpace[i];
                distances[i] = pair.DistanceSquared(pos);
                totalDistance += distances[i];
            }

            //skalar = distTarget / distAll
            ConvertToScalarValuesNormalized(distances, totalDistance);

            for (int i = 0; i < ambientColorsInvertedSpace.Num(); i++){
                float scalar = distances[i];
                const FPairColorPosition &pair = ambientColorsInvertedSpace[i];
                accumulatedColor += scalar * pair.color;

            
                if(bLogColor){
                    UiDebugHelper::logMessage(
                        FString::Printf(
                            TEXT("Slate: SlateMeshData: Color scaled %.2f from R_%.2f G_%.2f B_%.2f A_%.2f to R_%.2f G_%.2f B_%.2f A_%.2f"),
                            scalar,
                            pair.color.R,
                            pair.color.G,
                            pair.color.B,
                            pair.color.A,
                            accumulatedColor.R,
                            accumulatedColor.G,
                            accumulatedColor.B,
                            accumulatedColor.A 
                        )
                    );
                }
            }
        }
    }

    //cursor has more weight
    if(bCursorColorEnabled){
        
        bool bDebugSimple = false;
        if(bDebugSimple){
            if(ambientColorsInvertedSpace.Num() <= 0){
                accumulatedColor = FLinearColor::Blue;
            }
            else
            {
                accumulatedColor = ambientColorsInvertedSpace[0].color;
            }
            return accumulatedColor;
        }

        // lin drop
        // float max = 100 * 100;
        // scalar = distTarget / distAll;
        // scalarInv = 1.0 - scalar;

        //nach 1/x abschwächen   
        //scalar = 1 / dist, wenn dist = 1: max color 
        float distCursor = cursorColorPair.DistanceSquared(pos);
        float eps = 1.0f; // Schutz gegen Division durch 0
        float scalar = 1.0f / (distCursor + eps);

        float intensity = 100.0f;
        scalar *= intensity;

        scalar = std::min(scalar, 1.0f); //1.0 obere grenze!

        accumulatedColor += cursorColorPair.color * scalar;
        accumulatedColor /= 2.0f; // oder dynamisch normalisieren
    }


    //clamp color very important
    accumulatedColor = accumulatedColor.GetClamped();
    accumulatedColor.A = 1.0f;
    // FMath::Clamp(accumulatedColor.A, 0.0f, 1.0f);

    if(bLogColor){
        UiDebugHelper::logMessage(
            FString::Printf(
                TEXT("Slate: SlateMeshData: Color Result R_%.2f G_%.2f B_%.2f A_%.2f"),
                accumulatedColor.R,
                accumulatedColor.G,
                accumulatedColor.B,
                accumulatedColor.A 
            )
        );
    }

    return accumulatedColor;
}

///@brief converts the whole buffer to scalar values and devides its strength by the size
///of the buffer
void SlateMeshData::ConvertToScalarValuesNormalized(
    TArray<float> &numbers, //distances are already in inverted space
    float totalDistance
)const{

    //inverts numbers and builds the sum,
    //then assigns its fraction from [0,1]
    /*
    float sum = 0.0f;
    for (int i = 0; i < numbers.Num(); i++){
        float inverted = totalDistance - numbers[i];
        sum += numbers[i];
    }

    for (int i = 0; i < numbers.Num(); i++){
        float &d = numbers[i];
        if(sum > 0.0f){
            d = d / sum; //weight by all
        }else{
            d = 1.0f / numbers.Num();
        }
    }*/

    //Gewichtung schon in richtigem space, UV Colors werden GESPIEGELT eingefügt,
    //deshalb ist der skalar schon im korrekten space (distTarget / distAll) nicht 1.0 - scalar!
    for (int i = 0; i < numbers.Num(); i++){
        numbers[i] /= totalDistance;
    }

    //check sum
    if(bLogColor){
        float sum = 0.0f;
        for (int i = 0; i < numbers.Num(); i++){
            sum += numbers[i];
        }
        UiDebugHelper::logMessage(
            FString::Printf(
                TEXT("Slate: SlateMeshData: Color Sum Scalar %.2f"),
                sum
            )
        );
    }
}




// ---- transformation ----
void SlateMeshData::ApplyTransformationImmidiate(const MMatrix2D &other){
    if(other.IsZeroScaleMatrix()){
        return; //invalid, bricks mesh data
    }

    for (int i = 0; i < Vertecies.Num(); i++){
        ApplyTransformation(other, Vertecies[i]);
    }
    FlagCacheUpdateNeeded();
}

void SlateMeshData::CenterAroundPivotImmidiate(FVector2D &pivot){
    FVector2D center = CenterOfMesh();
    FVector2D dir = pivot - center; // AB = B - A

    MMatrix2D move(dir);
    ApplyTransformationImmidiate(pivot);
}

void SlateMeshData::SetRuntimeTransformation(MMatrix2D &other){
    transformationRuntimeMatrix = other;
    bHasRuntimeTransformation = true;
    FlagCacheUpdateNeeded(); //updated transform

    //UiDebugHelper::logMessage("slate: Set RuntimeTransformation");
}

void SlateMeshData::ResetRuntimeTransformation(){
    bHasRuntimeTransformation = false;
}

void SlateMeshData::ApplyTransformation(const MMatrix2D &mat, FVector2D &vertex){
    vertex = mat * vertex;
}

/// just if modification is ok because the vertex data belongs to the 
/// object and slate render data is not cached, also
/// draw data is cached inside the SlateVertexBufferCache as SlateVertex and NOT
/// raw Vertexbuffer
void SlateMeshData::ApplyTransformationConst(
    const MMatrix2D &mat, 
    FVector2f &vertex
) const {
    FVector2f copy = vertex;
    vertex = mat * vertex;

    //debug
    if(false){
        UiDebugHelper::logMessage(
            FString::Printf(TEXT(
                "slate: SlateMeshData runtime transform (%.2f, %2f) -> (%.2f, %2f)"
            ),
            copy.X, copy.Y, vertex.X, vertex.Y
            )
        );
    }
    


}



FVector2f SlateMeshData::ConvertTo2f(const FVector2D &other) const {
    FVector2f result(
        other.X,
        other.Y
    );
    return result;
}






/// ------------- drawing texture -------------

void SlateMeshData::SetTexture(UTexture2D *inTexture){
    if(inTexture){
        texturePtr = inTexture;
        textureBrush.SetResourceObject(inTexture);
    }
}

const FSlateResourceHandle &SlateMeshData::drawingHandle() const {
    if(texturePtr != nullptr){
        //FSlateBrush
        //const FSlateResourceHandle &
        //GetRenderingResource ()
        UiDebugHelper::logMessage("SlateMeshData texture ok");
        return textureBrush.GetRenderingResource();
    }
    //fixes color mismatch: mix color with white
    const FSlateBrush* WhiteBrush = FCoreStyle::Get().GetBrush("WhiteBrush");
    if(WhiteBrush){
        return WhiteBrush->GetRenderingResource();
    }

    return emptyHandle;
}




// --- helper ---
FVector2D SlateMeshData::SizeBounds(){
    return boundingBox.size();
}