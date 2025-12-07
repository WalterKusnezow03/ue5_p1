#include "ImagePatch.h"
#include "ComputerVisionPlugin/Public/ComputerVision/Actor/Storage/ImageWriter.h"

ImagePatch::ImagePatch(){

}

ImagePatch::~ImagePatch(){

}

ImagePatch::ImagePatch(const ImagePatch &other){
    if(this != &other){
        *this = other;
    }
}
ImagePatch &ImagePatch::operator=(const ImagePatch &other){
    if(this != &other){
        colorSaved = other.colorSaved;

        sizeXSaved = other.sizeXSaved;
        sizeYSaved = other.sizeYSaved;
        id = other.id;
        screenPosition = other.screenPosition;
        FeatureVector = other.FeatureVector;
    }
    return *this;
}

void ImagePatch::SavePatch(TArray<FColor> &colorIn, int sizePatchX, int sizePatchY, int32 idIn){
    colorSaved = colorIn;

    sizeXSaved = sizePatchX;
    sizeYSaved = sizePatchY;
    id = idIn;

    //for debugging --> looks ok.
    //SaveToStorage();
}

void ImagePatch::SavePatch(
    TArray<FColor> &colorIn, 
    int sizePatchX, 
    int sizePatchY, 
    int32 idIn,
    FVector2D &screenPositonIn
){
    SavePatch(colorIn, sizePatchX, sizePatchY, idIn);
    screenPosition = screenPositonIn;
}

FVector2D &ImagePatch::GetOriginalImagePosition(){
    return screenPosition;
}

void ImagePatch::SaveToStorage(){
    if(id > 20){
        return; //MAX DEBUG
    }
    if(colorSaved.Num() <= 0){
        return;
    }

    uint8 *ptr = (uint8*) colorSaved.GetData();
    ImageWriter::SaveColorBufferAsPngFromName(
        ptr,
        sizeXSaved,
        sizeYSaved,
        FString::Printf(TEXT("Patches/patch_%d.png"), id)
    );
}




void ImagePatch::ScaleDown(float scalePixels){
    //skalar = distTarget / DistAll

    //KEEP BORDER FOR PROPER GRADIENT CALCULATION!
    //--- DO NOT REMOVE ---
    scalePixels += 2.0f;
    //--- DO NOT REMOVE ---

    //frac = scalePixels / all
    TArray<FColor> newBuffer;
    int numPixels = scalePixels * scalePixels;


    //bricked
    //newBuffer.SetNum(numPixels);
    float stepX = sizeXSaved / scalePixels;
    float stepY = sizeYSaved / scalePixels;

    /*DebugHelper::logMessage(
        FString::Printf(
            TEXT("ImagePatch::ScaleDown Sample from (%d, %d) to (%.2f %.2f) with (%.2f %.2f)"),
            sizeXSaved,
            sizeYSaved,
            scalePixels,
            scalePixels,
            stepX,
            stepY
        )
    );*/

    for (int i = 0; i < scalePixels; i++)
    {
        for (int j = 0; j < scalePixels; j++){
            
            //in original skallierung
            float iStartF = stepX * i;
            float jStartF = stepY * j;
            float iEndF = stepX * (i+1);
            float jEndF = stepY * (j+1);

            int iStart = FMath::FloorToInt(iStartF);
            int jStart = FMath::FloorToInt(jStartF);
            int iEnd = FMath::CeilToInt(iEndF);
            int jEnd = FMath::CeilToInt(jEndF);

            //weil nur 14 manchmal kopiert werden.
            if(iEnd == iStart){
                iEnd++;
            }
            if(jEnd == jStart){
                jEnd++;
            }

            //ToIndex(i, j);
            //int index1D = j * scalePixels + i;
            newBuffer.Add(Sample(iStart, jStart, iEnd, jEnd));
        }
    }

    //DebugHelper::logMessage("ImagePatch::DownSample Size New ", std::sqrt(newBuffer.Num())); //ist ja 16.

    //override patch with new data
    SavePatch(newBuffer, scalePixels, scalePixels, id);
}

FColor ImagePatch::Sample(int iStart, int jStart, int iEnd, int jEnd){
    
    float meanDenominator = (iEnd - iStart) * (jEnd - jStart);

    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    for (int i = iStart; i < iEnd; i++)
    {
        for (int j = jStart; j < jEnd; j++){
            int index = ToIndex(i, j);
            if(index >= 0 && index < colorSaved.Num()){
                FColor &current = colorSaved[index];
                r += current.R;
                g += current.G;
                b += current.B;
            }
        }
    }
    FColor sampled(0, 0, 0, 0);
    sampled.R = FMath::Clamp(FMath::RoundToInt(r / meanDenominator), 0, 255);
    sampled.G = FMath::Clamp(FMath::RoundToInt(g / meanDenominator), 0, 255);
    sampled.B = FMath::Clamp(FMath::RoundToInt(b / meanDenominator), 0, 255);
    sampled.A = 255;
        
    return sampled;
}

int ImagePatch::ToIndex(int i, int j){
    int index = ToIndex(i, j, sizeXSaved);
    if (index >= 0 && index < colorSaved.Num())
    {
        return index;
    }
    return -1;
}


int ImagePatch::ToIndex(int i, int j, int sizeXIn){
    int index = j * sizeXIn + i;
    return index;
}






void ImagePatch::ComputeGradients(){



    //m(x,y) = sqrt(gx^2 + gy^2)
    //m(x,y) = sqrt((L(x + 1,y)− L(x − 1,y))2 + (L(x,y + 1)− L(x,y− 1))2)
    
    //θ(x,y) = tan−1(gx)/(gy)) //einlocken relativ zu X achse.
    //θ(x,y) = tan−1((L(x,y + 1)− L(x,y− 1))/(L(x + 1,y)− L(x− 1,y)))
    TArray<float> theta;
    TArray<float> magnitude;

    //HIER - 1 UM RAND WEG ZU MACHEN, ACHTUNG: DOWN SAMPLE MUSS + 2 SEIN! SIEHE OBEN!
    for (int i = 1; i < sizeXSaved - 1; i++)
    {
        for (int j = 1; j < sizeYSaved - 1; j++){
            FVector2D gradient = ComputeGradient(i, j);
            float magnitudeCurrent = gradient.Size();
            //if(std::abs(gradient.X) < 0.00001){
            //    gradient.X = 0.00001;
            //}
            //float thetaCurrent = FMath::Atan(gradient.Y / gradient.X);

            //winkel relativ zur x achse
            float thetaCurrent = FMath::Atan2(gradient.Y, gradient.X); // [-pi, +pi] //radian

            /**
             * ACHTUNG GAUSSIAN WEIGHT FEHLT HIER NOCH!
             */


            magnitude.Add(magnitudeCurrent);
            theta.Add(thetaCurrent);
        }
    }

    
    float outDominantGradientMagnitude = 0.0f;
    float angleForDerotation = 0.0f;
    FindDominantGradientAngle(
        magnitude,
        theta,
        36,
        angleForDerotation,
        outDominantGradientMagnitude
    );

    //winkel der gradienten drehen, nicht das bild selber
    for (int i = 0; i < theta.Num(); i++) {
        theta[i] -= angleForDerotation; // Rotation-Invarianz
        // Optional wrap-around: theta[i] = fmod(theta[i] + 2*PI, 2*PI);
    }


    //DEBUG
    //ImagePatch::ComputeGradients imageSize:  49 ????
    

    

    //return;

    // safety.
    if(std::sqrt(magnitude.Num()) != 16){
        return;
    }

    //return;

    //Remove Border from pixels!
    //buffer is 18 -2 = 16 here.
    int sizeXNoBorder = sizeXSaved - 2;
    int sizeYNoBorder = sizeYSaved - 2;

    //OK
    int sqrt = std::sqrt(magnitude.Num());
    /*DebugHelper::logMessage(
        FString::Printf(
            TEXT("ImagePatch::ComputeGradients imageSize: %d %d  or (sqrt(magnitude = %d)"),
            sizeXNoBorder,
            sizeYNoBorder,
            sqrt
        )
    );*/

    //return;

    //für jeden 4x4 block gibt es ein neues histogram um den feature vector zu bauen
    TArray<float> featureVector;
    for (int i = 0; i < sizeXNoBorder; i += 4)
    {
        for (int j = 0; j < sizeYNoBorder; j += 4){
            int iEnd = i + 4;
            int jEnd = j + 4;
            iEnd = std::min(iEnd, sizeXNoBorder -1);
            jEnd = std::min(jEnd, sizeYNoBorder -1);

            Copy4x4FromAndBuildDescriptor(
                magnitude, 
                theta,
                sizeXNoBorder, //int sizeXIn, (all buffer size axis)
                i,//int iStart
                j,//jStart
                iEnd, //int iEnd
                jEnd,
                featureVector
            );
            //featureVector.Add(result);
        }
    }


    //is 128. Good.
    FeatureVector = featureVector;
    //DebugHelper::logMessage("ImagePatch::ComputeGradients made imageDescriptor (128?)", featureVector.Num());
}

void ImagePatch::Copy4x4FromAndBuildDescriptor(
    const TArray<float> &magnitude, 
    const TArray<float> &theta,
    int sizeXOnBuffer,
    int iStart, 
    int jStart,
    int iEnd,
    int jEnd,
    TArray<float> &appendToVector
){
    //ToIndex(int i, int j, int sizeXIn)

    //copy 4x4 patch.
    TArray<float> magnitudePart;
    TArray<float> thetaPart;
    for (int i = iStart; i < iEnd; i++){
        for (int j = jStart; j < jEnd; j++){
            int indexOriginal = ToIndex(i, j, sizeXOnBuffer);
            magnitudePart.Add(magnitude[indexOriginal]);
            thetaPart.Add(theta[indexOriginal]);
        }
    }

    /*
    //compute max magnitude again.
    float outDominantAngle = 0.0f;
    float outDominantGradientMagnitude = 0.0f;
    FindDominantGradientAngle(
        magnitudePart, 
        thetaPart, 
        8, //hier jetzt 8 bins.
        outDominantAngle, 
        outDominantGradientMagnitude
    ); 

    / **
     * ACHTUNG GAUSSIAN WEIGHT FEHLT HIER NOCH!
     * /

    //8 * 4 * 4 = 128 D
    return outDominantGradientMagnitude;
    */

    int numBins = 8;//8 * 4 * 4 = 128 D
    TArray<float> histogram = MakeHistogramOfMagnitudePerAngleBin(magnitudePart, thetaPart, numBins);
    if (histogram.Num() > 0){
        appendToVector.Append(histogram);
    }
}

/// @brief makes the histogram of angle mapped to magnitude. Magnitude summed together.
/// @param magnitude 
/// @param theta 
/// @param numBins 
/// @return 
TArray<float> ImagePatch::MakeHistogramOfMagnitudePerAngleBin(
    TArray<float> &magnitude,
    TArray<float> &theta,
    int numBins
){

    //histogram erstellen (list die zählt was es wie viel gibt a(2) b(10) usw.)
    TArray<float> histogram;
    if(magnitude.Num() != theta.Num()){
        return histogram;
    }

    // 360 / x = numBins
    // 1 /x = numBins / 360
    //x = numBins / 360

    float denominator = numBins / 360.0f;

    
    
    //histogram.SetNum(36); //360 deg / 10 = 36 bins für jeden abschnitt.

    histogram.SetNum(numBins);
    for (int i = 0; i < magnitude.Num(); i++)
    {
        float magnitudeCurrent = magnitude[i];
        float angleCurrent = theta[i];
        float angleDeg = (180.0f / M_PI) * angleCurrent;

        //int bin = (int)(angleDeg / 10.0f);

        float binSizeDeg = 360.0f / numBins; // z.B. 10 Grad pro Bin bei 36 Bins
        int bin = (int)(angleDeg / binSizeDeg);
        bin = (bin + numBins) % numBins; // wrap-around

        //int bin = (int)(angleDeg / denominator);
        //bin = (bin + 36) % 36; // wraparound
        //bin = (bin + numBins) % numBins; // wraparound

        histogram[bin] += magnitudeCurrent;
    }
    return histogram;
}


void ImagePatch::FindDominantGradientAngle(
    TArray<float> &magnitude,
    TArray<float> &theta,
    int numBins,
    float &outDominantAngle,
    float &outDominantGradientMagnitude
){
    float denominator = numBins / 360.0f;
    TArray<float> histogram = MakeHistogramOfMagnitudePerAngleBin(magnitude, theta, numBins);

    //find dominant angle for rotation invariance
    //dieser angle * 10 ist dann deg.
    int maxBin = 0; //index in 360 / 10
    for (int i = 1; i < histogram.Num(); i++){
        if (histogram[i] > histogram[maxBin]){
            maxBin = i;
        }   
    }

    //copy sum magnitude
    outDominantGradientMagnitude = histogram[maxBin]; //UNKLAR!

    //dominanter angle wird quasi erraten und fest gesetzt. Auf 10 grad schritt
    float dominantAngleDeg = maxBin * denominator; // 10.0f;  //* 10 um auf grad zu kommen.

    float dominantAngleRad = (M_PI / 180) * dominantAngleDeg;
    outDominantAngle = dominantAngleRad;
    // return dominantAngleRad;
}


//NUR ZUR VISUALISIERUNG DER ROT INVARIANZ, NOT TESTED
void ImagePatch::RotateImage(float angleRad){
    //new color buffer
    TArray<FColor> bufferRotated;
    bufferRotated.SetNum(colorSaved.Num());

    //luminance buffer rotation
    TArray<float> luminanceBufferRotated;
    luminanceBufferRotated.SetNum(colorSaved.Num());

    MMatrix2D mat;
    mat.RadAdd(angleRad);
    for (int i = 0; i < sizeXSaved; i++)
    {
        for (int j = 0; j < sizeYSaved; j++)
        {
            int index = ToIndex(i, j);
            FColor &colorCurrent = colorSaved[index];

            FVector2D asVector(i, j);
            RotatePixelCoordintaes(asVector, mat); //is already to int
            int updateIndex = ToIndex(asVector.X, asVector.Y);

            //erstmal einfach override
            bufferRotated[updateIndex] = colorCurrent;
            luminanceBufferRotated[updateIndex] = luminance(i, j);
        }
    }
}


void ImagePatch::RotatePixelCoordintaes(
    FVector2D &pixel,
    MMatrix2D &rotator
){
    FVector2D rotated = rotator * pixel;
    pixel.X = FMath::Floor(rotated.X);
    pixel.Y = FMath::Floor(rotated.Y);
}

//gradient über pixel helligkeit
FVector2D ImagePatch::ComputeGradient(int x, int y){
    FVector2D outGradient;

    //m(x,y) = sqrt(gx^2 + gy^2)
    //m(x,y) = sqrt((L(x + 1,y)− L(x − 1,y))2 + (L(x,y + 1)− L(x,y− 1))2) 
    float gx = luminance(x + 1, y) - luminance(x - 1, y);
    float gy = luminance(x, y + 1) - luminance(x, y - 1);
    outGradient.X = gx;
    outGradient.Y = gy;
    return outGradient;
}

void ImagePatch::ComputeLuminance(){
    luminanceBuffer.SetNum(colorSaved.Num());
    for (int i = 0; i < colorSaved.Num(); i++){
        luminanceBuffer[i] = luminance(colorSaved[i]);
    }
}   

float ImagePatch::luminance(int x, int y){
    if(luminanceBuffer.Num() == colorSaved.Num()){
        int toIndex = ToIndex(x, y);
        if(toIndex >= 0 && toIndex < luminanceBuffer.Num()){
            return luminanceBuffer[toIndex];
        }
    }
    return luminance(GetPixel(x, y));
}

float ImagePatch::luminance(const FColor &color){
    float L = 0.299f*color.R + 0.587f*color.G + 0.114f*color.B;
    return L;
}

FColor &ImagePatch::GetPixel(int x, int y){
    int index = ToIndex(x, y);
    if(index >= 0 && index < colorSaved.Num()){
        return colorSaved[index];
    }
    return fallback;
}





//distance euklid
float ImagePatch::Distance(ImagePatch &other){
    float a = DistanceSquared(other);
    if(a >= 0.0f){
        return std::sqrt(a);
    }
    return -1;//issue
}

float ImagePatch::DistanceSquared(ImagePatch &other){
    float sum = 0.0f;
    TArray<float> &featureVec1 = FeatureVectorRef();
    TArray<float> &featureVec2 = other.FeatureVectorRef();
    if(featureVec1.Num() != featureVec2.Num()){
        return -1; //issue
    }
    for (int i = 0; i < featureVec1.Num(); i++){
        //(a_i - b_i)^2 as sum, sqrt
        float dist = featureVec1[i] - featureVec2[i];
        sum += dist * dist;
    }
    return sum;
}