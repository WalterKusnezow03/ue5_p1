#pragma once

#include "CoreMinimal.h"

#include "PayLoadBase.generated.h"

/// @brief base class to attach payloads to any ucustomUiComponent
/// derived from uobject for easier mem management.
/// NEEDS A COPY MECHANISM!
UCLASS()
class CUSTOMUIPLUGIN_API UPayLoadBase : public UObject{

    GENERATED_BODY()
public:

    // KEEP EMPTY!

};

