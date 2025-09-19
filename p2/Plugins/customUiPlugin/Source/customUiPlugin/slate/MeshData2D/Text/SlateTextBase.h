#pragma once 

#include "CoreMinimal.h"
#include "Engine/Font.h" 
#include "CoreMath/Matrix/2D/MMatrix2D.h"
#include "Fonts/SlateFontInfo.h"

///@brief text abstraction class for easier text making.
///owned by SlatePolygonMap
class CUSTOMUIPLUGIN_API SlateTextBase {


public:
    SlateTextBase();
    ~SlateTextBase();

    SlateTextBase(const SlateTextBase &other);
    SlateTextBase &operator=(const SlateTextBase &other);


    void SetFont(UFont *font);
    void SetTextSizePixels(float size);
    void SetTextSize(float size);

    void SetColor(FColor colorIn);
    
    
    
    /// @brief max size in pixels
    /// @param maxSize 
    void SetFitMaxSize(const FVector2D &maxSize);
    void TransformFitMaxSize(const MMatrix2D &scale);

    void SetText(FString textIn);
    void AppendChar(TCHAR &character);
    void RemoveChar();

    int Lenght() const;
    const FString &GetText() const;
    const FSlateFontInfo &FontInfo() const;
    FVector2D Bounds() const;
    FColor Color() const;

    //alignment
    void SetCenteredInWidget(bool bCentered);

    ///disables centered flag and flags vertical only.
    void SetCenteredVertical(bool bCentered);
    bool bShouldCenteredInWidget() const;
    bool bShouldCenterVertical() const;

protected:
    FVector2D BoundsSlate() const;
    void LoadDefaultFont();

    float sizeSaved = 24.0f;
    UFont *fontSet = nullptr;

    FString text;


    //new max size
    bool maxSizeSetup = false;
    FVector2D maxSizePixels;

    FSlateFontInfo fontInfo;

    FColor colorSet;

    bool bCenteredFlag = false;
    bool bCenteredVertical = false;

    /// @brief FSlateFontInfo refreshed.
    void RefreshFont();

    float PixelToSlateSize(float pixels) const;
    float SlateToPixelSize(float size) const;
};
