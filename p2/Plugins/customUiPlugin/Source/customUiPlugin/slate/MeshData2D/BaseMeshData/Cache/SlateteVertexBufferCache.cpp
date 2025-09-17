#include "SlateVertexBufferCache.h"


SlateVertexBufferCache::SlateVertexBufferCache(){

}

SlateVertexBufferCache::~SlateVertexBufferCache(){

}

SlateVertexBufferCache::SlateVertexBufferCache(const SlateVertexBufferCache &other){
    if(this != &other){
        *this = other;
    }
}

SlateVertexBufferCache &SlateVertexBufferCache::operator=(const SlateVertexBufferCache &other){
    if(this != &other){
        cachedBuffer = other.cachedBuffer;
        bCacheUpdateNeeded = other.bCacheUpdateNeeded;
    }
    return *this;
}

void SlateVertexBufferCache::UpdateBufferSizeIfNeeded(int32 num) const {
    if(cachedBuffer.Num() < num){
        
        cachedBuffer.SetNum(num);
        
    }
}


TArray<FSlateVertex> &SlateVertexBufferCache::mutableCachedBufferRef(int32 num) const {
    UpdateBufferSizeIfNeeded(num);
    //TArray<FSlateVertex> &mutableBuffer = const_cast<TArray<FSlateVertex> &>(cachedBuffer);
    //return mutableBuffer;

    return cachedBuffer;
}

///@brief escapes const and updates flag.
void SlateVertexBufferCache::ResetCacheUpdateNeededFlag() const{
    bool &mutableBool = const_cast<bool&>(bCacheUpdateNeeded);
    mutableBool = false;
}

///@brief escapes const and updates flag.
void SlateVertexBufferCache::MarkCacheUpdateNeededFlagTrue(){
    bCacheUpdateNeeded = true;
}

bool SlateVertexBufferCache::CacheUpdateNeeded() const{
    return bCacheUpdateNeeded;
}