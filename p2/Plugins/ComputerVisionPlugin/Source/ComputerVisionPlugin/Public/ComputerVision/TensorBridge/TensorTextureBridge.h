// MyTensorBridge.h

#pragma once

#include <stdint.h>

/*#if __OBJC__
@class MTLTexture;
#else
typedef void* MTLTexture;
#endif*/

#if __OBJC__
@class MTLTexture;
@class MTLBuffer;
#else
typedef void* MTLTexture;
typedef void* MTLBuffer;
#endif

// Pointer zum Buffer (extern, Definition kommt in CPP)
//extern MTLBuffer* gSharedBuffer;
// Buffer nur als void* deklarieren
extern "C" void* gSharedBuffer;

extern "C" void BridgeTextureRead(
    id<MTLTexture> metalTex,
    int width,
    int height,
    void *&outPtr,
    bool &bFlag
);

extern "C" void BridgeTextureReadSharedBuffer(
    id<MTLTexture> tex,
    int width,
    int height,
    void *&outPtr,
    bool &bFlag
);