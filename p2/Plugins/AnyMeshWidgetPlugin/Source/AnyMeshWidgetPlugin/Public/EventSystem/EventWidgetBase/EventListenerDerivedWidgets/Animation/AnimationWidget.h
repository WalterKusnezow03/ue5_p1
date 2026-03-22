// Copyright Walter Kusnezow All Rights Reserved.
#pragma once

#include "CoreMinimal.h"

#include "Blueprint/UserWidget.h"
#include "AnyMeshWidgetPlugin/Public/EventSystem/EventWidgetBase/EventListenerBaseWidget/EventListenerBaseWidget.h"
#include "CoreMath/animation/timer/Timer.h"

#include "Components/Image.h"

#include "AnimationWidget.generated.h"

/// @brief ----- BASE CLASS FOR WIDGET ------
UCLASS(Blueprintable, ClassGroup="UserInterface", hidecategories=(Object,Activation,"Components|Activation",Sockets,Base,Lighting,LOD,Mesh), editinlinenew, meta=(BlueprintSpawnableComponent) )
class ANYMESHWIDGETPLUGIN_API UAnimationWidget : public UEventListenerBaseWidget {
    GENERATED_BODY()

public:
    

    virtual void SetVisible(bool flag) override; //resets the animation
    virtual void Init() override;

    //begin with plugin name (/Game/.../..) no trainling slash!
    UPROPERTY(EditAnywhere, Category = "_AnimationOption")
    FString pathToAnimation;

    UPROPERTY(EditAnywhere, Category = "_AnimationOption")
    FString frameName = "frame";

    UPROPERTY(EditAnywhere, Category = "_AnimationOption")
    int countFrames = 0;

    UPROPERTY(EditAnywhere, Category = "_AnimationOption")
    float framesPerSecond = 60.0f;

    UPROPERTY(EditAnywhere, Category = "_AnimationOption")
    bool loopAnimation = false;

    UPROPERTY(EditAnywhere, Category = "_AnimationOption")
    bool debugforceVisible = true;
    

    UFUNCTION(BlueprintImplementableEvent, Category = "_AnimationOption")
    UImage *FindImageWidget();


    virtual void TickExternal(float deltatime) override;

protected:
    void LoadAnimationFramesOnInit();
    void LoadTextureAndSaveFrame(int frameNumber);
    UTexture2D *LoadTexture(int frameNumber);
    UTexture2D *LoadTexture(FString path);

    UPROPERTY()
    TArray<UTexture2D *> frames;

    UPROPERTY(Transient) //nur zur Laufzeit
    TArray<FSlateBrush> CachedBrushes;

    Timer timer;
    int currentindex = 0;

    bool reachedEnd = false;
    virtual bool CanTick() override;

    void UpdateImage();
    void ResetTimer();
    void UpdateIndex();
};
