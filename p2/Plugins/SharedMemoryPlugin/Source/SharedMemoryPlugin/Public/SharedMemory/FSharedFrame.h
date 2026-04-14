#pragma pack(push, 1)
struct SHAREDMEMORYPLUGIN_API FSharedFrame
{
    void Init(int sizeX, int sizeY){
    
    }

    int Ready;

    unsigned char Data[1920 * 1080 * 3];
};
#pragma pack(pop)