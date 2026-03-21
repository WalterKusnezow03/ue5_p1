#pragma once

#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"



class TERRAINPLUGIN_API TreeMeshData : public MeshData {

public:
    /**
     * 
     * foliage helper
     * 
     */
    materialEnum targetMaterial(){
        return materialPreferred;
    }
    void setTargetMaterial(materialEnum inMaterial){
        materialPreferred = inMaterial;
    }

protected:
    materialEnum materialPreferred = materialEnum::stoneMaterial;
};