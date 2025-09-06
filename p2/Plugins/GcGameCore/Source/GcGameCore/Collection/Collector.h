#pragma once

#include "CoreMinimal.h"
#include "GcGameCore/Collection/interface/CollectableInterface.h"
#include "GcGameCore/Collection/TRecursiveMap/TRecursiveMap.h"



template class TRecursiveMap<UObject>;

/// @brief Will collect all objects implementing the collector interface.
///is designed around AActor and UObjects, pointers!
class GCGAMECORE_API Collector {

private:
    TRecursiveMap<UObject> pointerCollection;

    

private:
    UWorld *world = nullptr;

public:
    void SetWorld(UWorld *worldIn){
        if(worldIn){
            world = worldIn;
        }
    }

    /// @brief calls destructor on all maps / clears all maps and tracked objects
    void Clear(){
        pointerCollection.Clear();
    }




    Collector() {

    };
    ~Collector() {
        Clear();
        world = nullptr;
    };

    void Add(UObject *item, ICollectableInterface *interface){
        if(item && interface){
            CollectionProperties &properties = interface->getCollectionProperties();
            Add(item, properties);
        }
    }

    void Add(UObject *item, CollectionProperties &properties){
        if(item){

            //obj*
            //int keyIndex,
            //CollectionProperties &properties
            pointerCollection.Add(item, 0, properties);

            //debug
            properties.ShowKey("Collector: Tried Add to gc");
        }
    }



    //no e at all
    void Add(UObject *someClass){
        CollectionProperties properties;
        properties.SetFirstKey(someClass);
        Add(someClass, properties);
    }

    // --- template add to gc with more information ---
    template <typename E> //E UEnum!!
    void Add(UObject *someClass, E e){
        CollectionProperties properties;
        properties.SetFirstKey(someClass);
        properties.AddEnumProperty<E>(e);
        Add(someClass, properties);
    }

    template <typename E, typename F> //E UEnum!!
    void Add(UObject *someClass, E e, F f){
        CollectionProperties properties;
        properties.SetFirstKey(someClass);
        properties.AddEnumProperty<E>(e);
        properties.AddEnumProperty<F>(f);
        Add(someClass, properties);
    }

    // --- template get from gc ---

    //tries to get from collection, if not found, and aactor based: spawned.
    template <typename T>
    T* Get(CollectionProperties &properties){
        UObject *ptr = pointerCollection.Pop(
            0, // int keyIndex,
            properties
        );
        if(ptr != nullptr){

            T *casted = Cast<T>(ptr);
            if(casted){
                properties.ShowKey("Collector: found from gc");
                return casted;
            }
        }else{

            //spawn if could not be found from garbage collection
            UClass *ClassOfTargetedObj = properties.getType();
            if(world != nullptr && ClassOfTargetedObj != nullptr){
                //if is a aactor:
                if (ClassOfTargetedObj->IsChildOf(AActor::StaticClass())){
                    FActorSpawnParameters SpawnParams;
                    FVector Location;
                    AActor *spawned = world->SpawnActor<AActor>(
                        ClassOfTargetedObj, Location, FRotator::ZeroRotator, SpawnParams
                    ); 
                    if(spawned){

                        T *casted = Cast<T>(spawned);
                        if(casted){
                            properties.ShowKey("Collector: Spawned!");
                            return casted;
                        }
                    }
                }
            }
        }
        return nullptr;
    }

    
    // --- helper to construct properties ---

    // T* gc->Get<T>(UClass_T_toSpawn*); 
    template <typename T> //E UEnum!!
    T* Get(UClass *someClass){
        CollectionProperties properties;
        properties.SetFirstKey((UClass *)someClass);
        return Get<T>(properties);
    }




    /// @brief 
    /// @tparam T 
    /// @tparam E 
    /// @param someClass someClass Some Class isA T to SPAWN if find T,E,F fails
    /// @param e 
    /// @return 
    template <typename T, typename E> //E UEnum!!
    T* Get(UClass *someClass, E e){
        CollectionProperties properties;
        properties.SetFirstKey((UClass *)someClass);
        properties.AddEnumProperty<E>(e);
        return Get<T>(properties);
    }


    /// @brief 
    /// @tparam T 
    /// @tparam E 
    /// @tparam F 
    /// @param someClass Some Class isA T to SPAWN if find T,E,F fails
    /// @param e 
    /// @param f 
    /// @return 
    template <typename T, typename E, typename F> //E, F UEnum!!
    T* Get(UClass *someClass, E e, F f){
        CollectionProperties properties;
        properties.SetFirstKey((UClass *) someClass);
        properties.AddEnumProperty<E>(e);
        properties.AddEnumProperty<F>(f);

        return Get<T>(properties);
    }




};