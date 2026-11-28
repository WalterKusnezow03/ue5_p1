#pragma once

#include "CoreMinimal.h"


class P2_API NNTrainStringFormatter {

public:
    static FString FormatLossScientificCustom(
        const FString &InLossString,
        int32 SignificantDigits
    );

private:
    static bool ExtractMantissaAndExponent(
        FString inString,
        int32 &outExponent,
        FString &outMantissa
    );
    static FString ReplaceLeadingZeros(
        int32 SignificantDigits,
        int32 Exponent,
        FString MantissaStr,
        const FString& Sign
    );
    static FString FindSign(FString s);
    static FString RemoveSignAndDecimalPoints(FString inString);
    static FString RemoveNonNumericFrom(FString string);
};