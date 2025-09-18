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


SlateTextBase::SlateTextBase(const SlateTextBase &other){
    if(this != &other){
        LoadDefaultFont();
        colorSet = FColor::White;
        *this = other;
    }
}

SlateTextBase &SlateTextBase::operator=(const SlateTextBase &other){
    if(this != &other){
        sizeSaved = other.sizeSaved;
        fontSet = other.fontSet;
        text = other.text;
        maxSizeSetup = other.maxSizeSetup;
        maxSizePixels = other.maxSizePixels;
        fontInfo = other.fontInfo;
        colorSet = other.colorSet;
        bCenteredFlag = other.bCenteredFlag;
    }
    return *this;
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
        //UiDebugHelper::logMessage("SlateTextBase did set font");
    }else{
        //UiDebugHelper::logMessage("SlateTextBase could not set font");
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
    if (maxSizeSetup){
        //SetFitMaxWidthPixels(maxWidthPixelSaved); //refresh with cached known value
        SetFitMaxSize(maxSizePixels);
    }
}

void SlateTextBase::AppendChar(TCHAR &character){
    text += character;
    //will refresh size, and font if needed.
    if (maxSizeSetup){
        //SetFitMaxWidthPixels(maxWidthPixelSaved); //refresh with cached known value
        SetFitMaxSize(maxSizePixels);
    }
}
void SlateTextBase::RemoveChar(){
    if(Lenght() > 0){
        text.LeftChopInline(1);
        if (maxSizeSetup){
            //SetFitMaxWidthPixels(maxWidthPixelSaved); //refresh with cached known value
            SetFitMaxSize(maxSizePixels);
        }
    }
}




void SlateTextBase::SetFitMaxSize(const FVector2D &maxSize){
    if(maxSize.X > 0.0f && maxSize.Y > 0.0f){
        maxSizeSetup = true;
        maxSizePixels = maxSize;

        FSlateRenderer *renderer = FSlateApplication::Get().GetRenderer();
        if(renderer){
            UiDebugHelper::logMessage("SlateTextBase UpdateText Width to fit 1");
            //FSlateRenderer::GetFontMeasureService
            FVector2D TextSize = BoundsSlate();
            if (TextSize.X > 0.0f)
            {
                float foundWidthPixels = SlateToPixelSize(TextSize.X);
                float foundHeightPixels = SlateToPixelSize(TextSize.Y);

                //size * a = max
                //a = max / size
                float scalarX = maxSize.X / foundWidthPixels;
                float scalarY = maxSize.Y / foundHeightPixels;

                // den kleineren Faktor nehmen, sonst läuft Text über
                float scalar = std::min(scalarX, scalarY);

                // Textgröße mit diesem Faktor skalieren
                float newHeightPixels = 0.7f * scalar * TextSize.Y; //scale down to prevent spill (0.7)

                SetTextSizePixels(newHeightPixels);
                // SetTextSize(newHeightSlate);
                // is not printed
                UiDebugHelper::logMessage("SlateTextBase UpdateText Width to fit 2");
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
        UiDebugHelper::logMessage(
            FString::Printf(
                TEXT("SlateTextBase Measured %s -> %s"),
                *text,
                *TextSize.ToString()
            )
        );

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