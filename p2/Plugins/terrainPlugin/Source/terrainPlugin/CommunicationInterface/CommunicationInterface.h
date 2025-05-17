#include "CoreMinimal.h"

class TERRAINPLUGIN_API CommunicationInterface {

public:
    static void UpdatePlayerPosition(FVector &position);

private:
    CommunicationInterface();
    ~CommunicationInterface();

    static FVector playerPosition;
};