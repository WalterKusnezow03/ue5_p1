#pragma once

#include "CoreMinimal.h"
#include "MeshDataPlugin/Public/MeshGenBase/MeshData/MeshData.h"

#include <map>

class ProceduralMeshComponentPair; // forward declaration

/// @brief base class to store meshdata for raycast enabled and non enabled layers
template <typename T>
class MESHDATAPLUGIN_API TMeshDataMap {
    //Allow ProceduralMeshComponentPair derived from this, using the protected members of another object.
    friend class ProceduralMeshComponentPair;

public:
    

    MeshData &meshDataReference(T type, bool raycast){
        if(raycast){
            return meshDataReferenceRaycast(type);
        }
        return meshDataReferenceNoRaycast(type);
    }

    MeshData &meshDataReferenceRaycast(T material){
        if(raycastMeshData.find(material) == raycastMeshData.end()){
            MeshData empty;
            raycastMeshData[material] = empty;
        }
        return raycastMeshData[material];
    }


    MeshData &meshDataReferenceNoRaycast(T material){
        if(noRaycastMeshData.find(material) == noRaycastMeshData.end()){
            MeshData empty;
            noRaycastMeshData[material] = empty;
        }
        return noRaycastMeshData[material];
    }



    void replaceMeshDataRaycast(MeshData &data, T type){
        raycastMeshData[type] = data;
    }

    void replaceMeshDataNoRaycast(MeshData &data, T type){
        noRaycastMeshData[type] = data;
    }



    /**
     * append mesh data
     */
    void appendMeshDataRaycast(
        MeshData &meshdata,
        T type
    ){
        MeshData &found = meshDataReferenceRaycast(type);
        found.append(meshdata);
    }

    void appendMeshDataNoRaycast(
        MeshData &meshdata,
        T type
    ){
        MeshData &found = meshDataReferenceNoRaycast(type);
        found.append(meshdata);
    }


    void appendMeshData(TMeshDataMap<T> &other){
        
        for(auto &pair : other.raycastMeshData){
            T type = pair.first;
            MeshData &data = pair.second;
            appendMeshDataRaycast(data, type);
        }
        for(auto &pair : other.noRaycastMeshData){
            T type = pair.first;
            MeshData &data = pair.second;
            appendMeshDataNoRaycast(data, type);
        }
    }



    ///@brief will check the whole meshdata map for intersection
    /// (use this from uproeceduralmeshcomponentpair - acustommeshactor)
    /// for raycast alternative
    /// ONLY CHECKS raycastMeshData MAP, Not no raycast!
    /// returns hitpoint with first intersected layer!
    //ray intersect checker
    bool RayIntersectFirstHit(
        const FVector &origin,
        const FVector &direction,
        FVector &outIntersectionPoint
    ){
        for (auto& pair : raycastMeshData){
            MeshData &data = pair.second;
            if(data.RayIntersect(origin, direction, outIntersectionPoint)){
                return true;
            }
        }
        return false;
    }


    bool RayIntersectClosestHit(
        const FVector &origin,
        const FVector &direction,
        FVector &outIntersectionPoint
    ){
        TArray<FVector> Hitpoints;
        for (auto& pair : raycastMeshData)
        {
            MeshData &data = pair.second;
            FVector hitTracked;
            if (data.RayIntersect(origin, direction, hitTracked))
            {
                Hitpoints.Add(hitTracked);
            }
        }

        //o(n)
        if(Hitpoints.Num() > 0){
            float distSquaredClosest = FVector::DistSquared(origin, Hitpoints[0]);
            int index = 0;
            for (int i = 1; i < Hitpoints.Num(); i++){
                FVector &current = Hitpoints[i];
                if (float f = FVector::DistSquared(Hitpoints[i], origin); f < distSquaredClosest){
                    distSquaredClosest = f;
                    index = i;
                }
            }
            
            //index valid in any case (safety check)
            if(index >= 0 && index < Hitpoints.Num()){
                outIntersectionPoint = Hitpoints[index];
                return true;
            }
        }

        return false;
    }



    ///checks if any intersection happened.
    bool RayIntersect(
        const FVector &origin,
        const FVector &direction
    ){
        FVector none;
        for (auto& pair : raycastMeshData){
            MeshData &data = pair.second;
            if(data.RayIntersect(origin, direction, none)){
                return true;
            }
        }
        return false;
    }




    ///----> could return all or closest hitpoint too!

    virtual void ClearAllMeshData(){
        for (auto& pair : raycastMeshData){
            MeshData &data = pair.second;
            data.clearMesh();
        }
        for (auto& pair1 : noRaycastMeshData){
            MeshData &data = pair1.second;
            data.clearMesh();
        }        
    }

    bool IsInBound(const FVector &localPosition){
        return IsInBound(localPosition, true) || IsInBound(localPosition, false);
    }

    bool IsInBound(const FVector &localPosition, bool raycastEnabledMesh){
        if(raycastEnabledMesh){
            return IsInBound(localPosition, raycastMeshData);
        }else{
            return IsInBound(localPosition, noRaycastMeshData);
        }
    }

    bool IsInBound(
        const FVector &localPosition, 
        std::map<T, MeshData> &mapToCheck
    ){
        for(auto &pair : mapToCheck){
            MeshData &data = pair.second;
            if(data.isInsideBoundingbox(localPosition)){
                return true;
            }
        }

        return false;
    }


    virtual void transformAllVertecies(MMatrix &other){
        transformAllVertecies(other, raycastMeshData);
        transformAllVertecies(other, noRaycastMeshData);
    }

    

    void transformAllVertecies(MMatrix &other, std::map<T, MeshData> &mapTotransform){
        for(auto &pair : mapTotransform){
            MeshData &data = pair.second;
            data.transformAllVertecies(other);
        }
    }

protected:
    

    std::map<T, MeshData> raycastMeshData;
    std::map<T, MeshData> noRaycastMeshData; //no physics mesh at all

};