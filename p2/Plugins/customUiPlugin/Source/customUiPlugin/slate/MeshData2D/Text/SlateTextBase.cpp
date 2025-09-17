#include "SlateTextBase.h"
#include "Rendering/SlateRenderer.h"
#include "Fonts/FontMeasure.h"
#include "customUiPlugin/ui/Widgets/Text/UiTextStyle.h"
#include "customUiPlugin/Private/Debug/UiDebugHelper.h"

#include "Framework/Application/SlateApplication.h"


SlateTextBase::SlateTextBase(){
    LoadDefaultFont();
    colorSet = FColor::White;
}

SlateTextBase::~SlateTextBase(){

}


void SlateTextBase::LoadDefaultFont(){
    //load default font here and set
    UFont *none = UiTextStyle::DefaultUFont();
    SetFont(none);
    SetTextSize(24.0f);
}

void SlateTextBase::SetColor(FColor colorIn){
    colorSet = colorIn;
}

FColor SlateTextBase::Color() const {
    return colorSet;
}

int SlateTextBase::Lenght()const{
    return text.Len();
}

void SlateTextBase::SetCenteredInWidget(bool bCentered){
    bCenteredFlag = bCentered;
}

bool SlateTextBase::bShouldCenteredInWidget() const{
    return bCenteredFlag;
}

void SlateTextBase::SetFont(UFont *font){
    if(font){
        fontSet = font;
        RefreshFont();
        UiDebugHelper::logMessage("SlateTextBase did set font");
    }else{
        UiDebugHelper::logMessage("SlateTextBase could not set font");
    }
}

void SlateTextBase::SetTextSizePixels(float pixels){
    SetTextSize(PixelToSlateSize(pixels));    
}   

float SlateTextBase::PixelToSlateSize(float pixels) const {
    //basis
    //float PixelSize = FontInfo.Size * FSlateApplication::Get().GetApplicationScale();
    //umstellen:
    //fontInfoSize = pixels / FSlateApplication::Get().GetApplicationScale()
    float scale = FSlateApplication::Get().GetApplicationScale();
    return pixels / scale;
}

float SlateTextBase::SlateToPixelSize(float size) const {
    //fontInfoSize = pixels / FSlateApplication::Get().GetApplicationScale()
    //pixels = fontInfoSize * FSlateApplication::Get().GetApplicationScale()
    return size * FSlateApplication::Get().GetApplicationScale();
}



void SlateTextBase::SetTextSize(float size){
    if(size > 0.0f){
        sizeSaved = size;
        RefreshFont();
    }
}



void SlateTextBase::SetText(FString textIn){
    text = textIn;

    //will refresh size, and font if needed.
    if (maxWidthSetup)
    {
        SetFitMaxWidthPixels(maxWidthPixelSaved); //refresh with cached known value
    }
}


void SlateTextBase::SetFitMaxWidthPixels(int widthPixels){

    if(widthPixels > 0){

        FSlateRenderer *renderer = FSlateApplication::Get().GetRenderer();
        if(renderer){
            //FSlateRenderer::GetFontMeasureService
            FVector2D TextSize = BoundsSlate();
            if (TextSize.X > 0.0f)
            {
                float foundWidthPixels = SlateToPixelSize(TextSize.X);

                //size.X * a = max
                //a = max / size.X
                float scalar = widthPixels / foundWidthPixels;
                float heightSlateSize = scalar * TextSize.Y;
                SetTextSize(heightSlateSize);


                //Copy pixel data for max width
                maxWidthSetup = true;
                maxWidthPixelSaved = widthPixels;
            }
        }
    }

    
}



void SlateTextBase::RefreshFont(){
    //FSlateFontInfo FontInfo(UFont*, sizeBasedOnDpi);
    if(sizeSaved <= 0.0f){
        sizeSaved = 24.0f;
    }
    if(fontSet != nullptr){
        //FSlateFontInfo( 
        //  const UObject* InFontObject,
        //  const int32 InSize, 
        //  const FName& InTypefaceFontName = NAME_None, 
        //  const FFontOutlineSettings& InOutlineSettings = FFontOutlineSettings()
        //);
        fontInfo = FSlateFontInfo(fontSet, sizeSaved);
    }
}


FVector2D SlateTextBase::Bounds() const {
    FVector2D boundsSlate = BoundsSlate();
    return FVector2D(
        SlateToPixelSize(boundsSlate.X),
        SlateToPixelSize(boundsSlate.Y)
    );
}

FVector2D SlateTextBase::BoundsSlate() const {
    FSlateRenderer *renderer = FSlateApplication::Get().GetRenderer();
    if(renderer){
        FVector2D TextSize = renderer->GetFontMeasureService()->Measure(text, fontInfo);
        return TextSize;
    }
    return FVector2D(0, 0);
}


const FString &SlateTextBase::GetText() const{
    return text;
}

const FSlateFontInfo &SlateTextBase::FontInfo() const{
    return fontInfo;
}