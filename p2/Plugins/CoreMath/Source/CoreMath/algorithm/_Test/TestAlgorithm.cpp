#include "TestAlgorithm.h"

#include "CoreMath/algorithm/PolygonFit/InverseHull/InnerHullFinder.h"
#include "CoreMath/algorithm/PolygonFit/polygon/MPolygon.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"
#include "CoreMath/Debug/CoreMathDebugHelper.h"


void TestAlgorithm::LogMessage(FString s){
    FString appendFront = TEXT("TestAlgorithm: ");
    appendFront += s;
    CoreMathDebugHelper::logMessage(appendFront);
}

void TestAlgorithm::LogMessage(FString prefix, FString s){
    prefix += TEXT(" ");
    prefix += s;
    LogMessage(prefix);
}

void TestAlgorithm::LogMessage(FString prefix, bool testSucess){

    FString resultTest = TEXT(" Test ");
    if (testSucess){
        resultTest += TEXT("->sucess");
    }
    else{
        resultTest += TEXT("->FAILED");
    }

    LogMessage(prefix, resultTest);
}



// ---- TESTS ----

void TestAlgorithm::Test(){

    TestInnerHullFinder();
    TestPolygonHit();

    //CoreMathDebugHelper
}

void TestAlgorithm::TestInnerHullFinder(){

    // --- test looks good, as expected ---

    LogMessage("--- InnerhullFinder Start ---");

    TArray<FVector2D> outer{
        FVector2D(0, 0),
        FVector2D(0, 100),
        FVector2D(100, 100),
        FVector2D(100, 0),
    };
    TArray<FVector2D> expectedInner{
        outer[0] + FVector2D(10,10),
        outer[1] + FVector2D(10,-10),
        outer[2] + FVector2D(-10,-10),
        outer[3] + FVector2D(-10,10),
    };

    outer.Append(expectedInner);
    outer.Add(FVector2D(50, 50)); //if a single vertex is inside the middle, it wont be detected as shape

    bool addInnerInner = true;
    if(addInnerInner){
        TArray<FVector2D> expectedInnerInner{
            expectedInner[0] + FVector2D(10,10),
            expectedInner[1] + FVector2D(10,-10),
            expectedInner[2] + FVector2D(-10,-10)
        };
        outer.Append(expectedInnerInner);
        expectedInner = expectedInnerInner;
    }

    FString expected = makeString<FVector2D>(expectedInner);

    InnerHullFinder finder;
    finder.MakeMostInnerHull(outer);
    FString result = makeString<FVector2D>(outer);

    LogMessage("Result inner hull", result);
    LogMessage("Result expected hull", expected);

    LogMessage("InnerhullFinder inner hull ", result == expected); //true: sucess

    LogMessage("--- InnerhullFinder End ---");
}




void TestAlgorithm::TestPolygonHit(){

    LogMessage("--- Mpolygon intersect tests Start ---");

    //setup polygon
    MPolygon polygon;
    TArray<FVector2D> shapeA{
        FVector2D(0, 0),
        FVector2D(0, 100),
        FVector2D(100, 100),
        FVector2D(100, 0),
    };

    TArray<FVector2D> shapeB = shapeA;
    MMatrix2D R;
    R.RadAdd(MMatrix2D::degToRadian(45));
    for (int i = 0; i < shapeB.Num(); i++){
        shapeB[i] = R * shapeB[i];
    };

    shapeA.Append(shapeB);
    polygon.SetShape(shapeA);

    FVector2D A(200, 200);
    FVector2D B(-100, -100);

    // -- polygon intersect test --
    LogMessage("Mpolygon intersect line ", polygon.DoesIntersect(A,B)); //true: sucess




    // -- polygon hit test 1 --
    MPolygon transformed = polygon;
    FVector2D move(20, 10);
    transformed.SetTranslation(move);

    LogMessage("Mpolygon intersect polygon 1 ", polygon.DoesIntersect(transformed));  //true: sucess

    
    // -- polygon hit test 2 --
    move = FVector2D(1000, 1000);
    transformed.SetTranslation(move);
    LogMessage("Mpolygon intersect polygon 2 ", !polygon.DoesIntersect(transformed));  //false: sucess
   
   
    LogMessage("--- Mpolygon intersect tests End ---");
}