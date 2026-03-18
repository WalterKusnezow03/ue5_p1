#include "SphereCaster.h"
#include <limits>
#include "Kismet/KismetSystemLibrary.h"


//if the distance is lower, the passed distance is updated and true returned.
bool SphereCaster::DistSquaredLower(const FVector &location, AActor *actor, float &distUpdate){
    if(actor){
        float checkDistance = DistSquared(location, actor);
        if (checkDistance < distUpdate)
        {
            distUpdate = checkDistance;
            return true;
        }
    }
    return false;
}   

float SphereCaster::DistSquared(const FVector &location, AActor *actor){
    if(actor){
        return FVector::DistSquared(location, actor->GetActorLocation());
    }
    return std::numeric_limits<float>::max();
}



TArray<AActor *> SphereCaster::getAActorsInRadius(
    UWorld *world, 
    const FVector &location, 
    float SphereRadius,
    TArray<AActor*> &IgnoreActors
){
    if(world != nullptr){
        SphereRadius = std::abs(SphereRadius);

        // Array to store overlapping actors
        TArray<AActor*> OverlappingActors;

        // Collision query parameters
        TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes; // You can specify what kind of objects to detect
        //TArray<AActor*> IgnoreActors; // Actors to ignore in the overlap check
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
        ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));

        // Perform the overlap query
        UKismetSystemLibrary::SphereOverlapActors(
            world,
            location,
            SphereRadius,
            ObjectTypes,
            //AActor::StaticClass(),  // Class of actors to detect, use NULL for all classes
            AActor::StaticClass(),  // Class of actors to detect, use NULL for all classes
            IgnoreActors,
            OverlappingActors
        );

        return OverlappingActors;
    }

    //empty array
    TArray<AActor*> none;
    return none;
}


TArray<AActor *> SphereCaster::getAActorsInRadius(
    UWorld *world, 
    const FVector &location, 
    float SphereRadius
){
    TArray<AActor *> emptyArray;
    if(world){
        return getAActorsInRadius(world, location, SphereRadius, emptyArray);
    }
    return emptyArray;
}





bool SphereCaster::AnyHitInRadius(
    UWorld *world,
    const FVector &location,
    float SphereRadius,
    AActor *excludedActor
){
    TArray<AActor *> array;
    if(excludedActor){
        array.Add(excludedActor);
    }
    return AnyHitInRadius(world, location, SphereRadius, array);
}

bool SphereCaster::AnyHitInRadius(
    UWorld *world,
    const FVector &location,
    float SphereRadius,
    TArray<AActor*> &excludedActors
){
    if(!world){
        return false;
    }

    FCollisionQueryParams Params;
    Params.AddIgnoredActors(excludedActors);

    FCollisionObjectQueryParams ObjectParams;
    ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);
    ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

    bool bHit = world->OverlapAnyTestByObjectType(
        location,
        FQuat::Identity,
        ObjectParams,
        FCollisionShape::MakeSphere(SphereRadius),
        Params
    );
    return bHit;
}