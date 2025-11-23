// MyTensorBridge.mm

#import <Metal/Metal.h>
#import <MetalPerformanceShaders/MetalPerformanceShaders.h>
#include "tensorflow/c/tf_tensor.h"

#include "TensorTextureBridge.h"

static id<MTLBuffer> staticSharedBuffer = nil;
extern "C" void BridgeTextureRead(
    id<MTLTexture> metalTex,
    int width,
    int height,
    void*& outPtr,
    bool &bFlag
){

    // Alignment: bytesPerRow muss Vielfaches von 256
    NSUInteger bytesPerRow = ((width * 4 + 255)/256) * 256;
    NSUInteger bufferSize = bytesPerRow * height;

    // Shared Buffer erstellen
    if(!staticSharedBuffer){
        staticSharedBuffer = [metalTex.device newBufferWithLength:bufferSize
                                                            options:MTLResourceStorageModeShared];
    }
    //id<MTLBuffer> sharedBuffer = [metalTex.device newBufferWithLength:bufferSize
    //                                                        options:MTLResourceStorageModeShared];

    // Blit Setup
    id<MTLCommandQueue> queue = [metalTex.device newCommandQueue];
    id<MTLCommandBuffer> cmd = [queue commandBuffer];
    id<MTLBlitCommandEncoder> blit = [cmd blitCommandEncoder];

    // Textur -> Shared Buffer kopieren
    [blit copyFromTexture:metalTex
            sourceSlice:0
            sourceLevel:0
            sourceOrigin:MTLOriginMake(0,0,0)
            sourceSize:MTLSizeMake(width,height,1)
                toBuffer:staticSharedBuffer
                
                //toBuffer:sharedBuffer
    destinationOffset:0
    destinationBytesPerRow:bytesPerRow
    destinationBytesPerImage:bytesPerRow*height];

    [blit endEncoding];
    [cmd commit];
    [cmd waitUntilCompleted]; // Optional, wenn du synchron auf die Daten warten willst

    // CPU-Zeiger auf die Daten
    outPtr = staticSharedBuffer.contents;
    //sharedBuffer.contents;
    bFlag = true;
}




//MTLBuffer* gSharedBuffer = nil;
#import <Metal/Metal.h>
#include "TensorTextureBridge.h"
#include "DebugPlugin/DebugHelper.h"
#include "CoreMinimal.h"
static id<MTLCommandQueue> sharedQueue = nil;
static id<MTLBuffer> sharedBuffer = nil;
static NSUInteger sharedBufferSize = 0;

extern "C" void BridgeTextureReadSharedBuffer(
    id<MTLTexture> tex,
    int width,
    int height,
    void*& outPtr,
    bool &bFlag
){
    NSUInteger bytesPerRow = ((width * 4 + 255)/256) * 256;
    NSUInteger size = bytesPerRow * height;

    // SINGLE COMMAND QUEUE
    if(!sharedQueue){
        sharedQueue = [tex.device newCommandQueue];
    }

    // RESIZE BUFFER IF NECESSARY
    if(!sharedBuffer || sharedBufferSize < size){
        sharedBuffer = [tex.device newBufferWithLength:size
                                               options:MTLResourceStorageModeShared];
        sharedBufferSize = size;
    }

    id<MTLCommandBuffer> cmd = [sharedQueue commandBuffer];
    id<MTLBlitCommandEncoder> blit = [cmd blitCommandEncoder];

    [blit copyFromTexture:tex
        sourceSlice:0
        sourceLevel:0
        sourceOrigin:MTLOriginMake(0,0,0)
        sourceSize:MTLSizeMake(width,height,1)
        toBuffer:sharedBuffer
        destinationOffset:0
        destinationBytesPerRow:bytesPerRow
        destinationBytesPerImage:size
    ];

    [blit endEncoding];
    [cmd commit];
    [cmd waitUntilCompleted]; // sync = simple but expensive
    
    outPtr = sharedBuffer.contents;
    bFlag = true;
}