#include "GlobalFeatureVector.h"




void GlobalFeatureVector::Init(
    TArray<ImagePatch> &images, 
    int maxImages,
    int dimension
){
    
    int N = 1000; // Anzahl Deskriptoren
    int d = 128;   // Dimension pro Deskriptor
    std::vector<float> data;
    data.reserve(N * d);

    // Alle Patch-Features in data kopieren
    for (int i = 0; i < images.Num(); i++) {
        ImagePatch &patch = images[i];
        TArray<float> &feat = patch.FeatureVectorRef();
        data.insert(data.end(), feat.GetData(), feat.GetData() + feat.Num());
    }
}

