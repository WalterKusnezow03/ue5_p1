// Copyright Walter Kusnezow All Rights Reserved.
#include "AnimationWidget.h"



void UAnimationWidget::Init(){
    Super::Init();
    LoadAnimationFramesOnInit();

    DebugHelper::logMessage(
        FString::Printf(
            TEXT("UAnimationWidget::loaded frames %d (%s)(%s)"),
            CachedBrushes.Num(),
            *EventName, //optional but for debug!
            *pathToAnimation
        )
    ); //is called

    reachedEnd = false;
    currentindex = 0;
    //UpdateImage();
    ResetTimer();
}

// ---- LOADING -----

void UAnimationWidget::LoadAnimationFramesOnInit(){
    for (int i = 0; i < countFrames; i++){
        LoadTextureAndSaveFrame(i);
    }
}

void UAnimationWidget::LoadTextureAndSaveFrame(int frameNumber){
    if(UTexture2D *loaded = LoadTexture(frameNumber)){
        frames.Add(loaded);

        FSlateBrush Brush;
        Brush.SetResourceObject(loaded); // Texture setzen
        Brush.ImageSize = FVector2D(loaded->GetSizeX(), loaded->GetSizeY());
        CachedBrushes.Add(Brush);
    }
}


UTexture2D *UAnimationWidget::LoadTexture(int frameNumber){
    FString path = FString::Printf(TEXT("%s/%s_%d"), *pathToAnimation, *frameName, frameNumber);
    return LoadTexture(path);
}

UTexture2D *UAnimationWidget::LoadTexture(FString path){
    if(path.IsEmpty()){
        return nullptr;
    }
    UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *path);
    return texture;
}
// ---- LOADING -----


// ---- TICK -----

//resets the animation
void UAnimationWidget::SetVisible(bool flag){
    Super::SetVisible(flag);
    reachedEnd = false;
    currentindex = 0;
    UpdateImage();
    ResetTimer();
}

bool UAnimationWidget::CanTick(){
    if(Super::CanTick()){
        if(!loopAnimation && reachedEnd){
            return false;
        }
        return true;
    }
    return false;
}

void UAnimationWidget::TickExternal(float deltatime){
    Super::TickExternal(deltatime);
    //DebugHelper::showScreenMessage("UAnimationWidget::tick!"); //is called
    if (CanTick())
    {
        //DebugHelper::showScreenMessage("UAnimationWidget::tick!"); //is called
        if(timer.timesUp()){
            UpdateIndex();
            UpdateImage();
            ResetTimer();
        }
        timer.Tick(deltatime);
    }
}

void UAnimationWidget::UpdateIndex(){
    if(CachedBrushes.Num() == 0){
        return;
    }

    currentindex++;
    currentindex = currentindex % CachedBrushes.Num();
    if(!loopAnimation){
        reachedEnd = currentindex == 0;
    }
}

void UAnimationWidget::ResetTimer(){
    float framesPerSecondSafe = std::abs(framesPerSecond);
    framesPerSecondSafe = std::max(framesPerSecondSafe, 0.1f);
    float time = 1.0f / framesPerSecond;
    timer.Begin(time, false);
}




void UAnimationWidget::UpdateImage(){
    //DEBUG
    //return;

    if(currentindex >= 0 && currentindex < CachedBrushes.Num()){
        //DebugHelper::logMessage("UAnimationWidget::update image A!"); //is called
        if(UImage *image = FindImageWidget()){

            if(image->GetVisibility() == ESlateVisibility::Visible){
                //DebugHelper::showScreenMessage("UAnimationWidget::VISIBLE! ", FColor::Red);
            }

            
            image->SetBrush(CachedBrushes[currentindex]);

            if(debugforceVisible){
                Super::SetVisible(true);
                image->SetVisibility(ESlateVisibility::Visible); //debug
            }
            
        }
    }
}

