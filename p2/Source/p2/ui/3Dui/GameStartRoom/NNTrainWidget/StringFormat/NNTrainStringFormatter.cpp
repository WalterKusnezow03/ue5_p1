#include "NNTrainStringFormatter.h"


FString NNTrainStringFormatter::FormatLossScientificCustom(
    const FString& InLossString, 
    int32 SignificantDigits
){
    int32 Exponent = 0;
    FString MantissaStr;
    if(!ExtractMantissaAndExponent(InLossString, Exponent, MantissaStr)){
        return InLossString;
    }

    // Clean mantissa digits (remove sign and decimal point)
    FString Sign = FindSign(MantissaStr);
    MantissaStr = RemoveSignAndDecimalPoints(MantissaStr);
    MantissaStr = RemoveNonNumericFrom(MantissaStr);
    return ReplaceLeadingZeros(SignificantDigits, Exponent, MantissaStr, Sign);
}

FString NNTrainStringFormatter::ReplaceLeadingZeros(
    int32 SignificantDigits,
    int32 Exponent,
    FString MantissaStr,
    const FString& Sign
){
    FString outString;
    if (Exponent < 0)
    {
        int32 LeadingZerosCount = FMath::Abs(Exponent) - 1;

        // Ensure we take only the requested significant digits starting from the first non-zero representation
        if (MantissaStr.Len() > SignificantDigits){
            MantissaStr = MantissaStr.Left(SignificantDigits);
        }

        if (LeadingZerosCount > 0){
            outString = FString::Printf(TEXT("%s0.(%d)%s"), *Sign, LeadingZerosCount, *MantissaStr);
        }
        else{
            outString = FString::Printf(TEXT("%s0.%s"), *Sign, *MantissaStr);
        }
    }
    return outString;
}

bool NNTrainStringFormatter::ExtractMantissaAndExponent(
    FString inString,
    int32 &outExponent,
    FString &outMantissa
){
    int32 EIndex = INDEX_NONE;
    if (!inString.FindChar('e', EIndex) && !inString.FindChar('E', EIndex))
    {
        // Not scientific notation; return as-is
        return false;
    }

    //Split into Mantissa and Exponent
    FString ExponentStr = inString.RightChop(EIndex + 1);

    outExponent = FCString::Atoi(*ExponentStr);
    outMantissa = inString.Left(EIndex);
    return true;
}

FString NNTrainStringFormatter::FindSign(FString s){
    return s.StartsWith(TEXT("-")) ? TEXT("-") : TEXT("");
}

FString NNTrainStringFormatter::RemoveSignAndDecimalPoints(FString inString){
    inString = inString.Replace(TEXT("-"), TEXT(""));
    inString = inString.Replace(TEXT("."), TEXT(""));
    return inString;
}

FString NNTrainStringFormatter::RemoveNonNumericFrom(FString string){
    FString cleaned = "";
    for (int32 i = 0; i < string.Len(); i++){
        TCHAR c = string[i];
        if (FChar::IsDigit(c)){
            cleaned.AppendChar(c);
        }
    }

    return cleaned;
}