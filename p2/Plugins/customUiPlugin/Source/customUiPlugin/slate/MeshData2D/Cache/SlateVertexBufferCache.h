#pragma once

#include "CoreMinimal.h"



/// @brief will cache the Slate vertex buffer and allow mutating it, 
/// leaving the const context on purpose!
class CUSTOMUIPLUGIN_API SlateVertexBufferCache {

public:
    SlateVertexBufferCache();
    ~SlateVertexBufferCache();

    SlateVertexBufferCache(const SlateVertexBufferCache &other);
    SlateVertexBufferCache &operator=(const SlateVertexBufferCache &other);

    void UpdateBufferSizeIfNeeded(int32 num) const;

    ///@brief escapes const and returns reference to cached buffer.
    TArray<FSlateVertex> &mutableCachedBufferRef(int32 num) const;

    ///@brief escapes const and updates flag.
    void ResetCacheUpdateNeededFlag() const;

    ///@brief escapes const and updates flag.
    void MarkCacheUpdateNeededFlagTrue();

    bool CacheUpdateNeeded() const;

private:
    mutable TArray<FSlateVertex> cachedBuffer; //removes const cast

    bool bCacheUpdateNeeded = true;
};