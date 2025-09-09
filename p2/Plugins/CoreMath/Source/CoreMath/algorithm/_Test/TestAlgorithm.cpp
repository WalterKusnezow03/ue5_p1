#include "TestAlgorithm.h"

#include "CoreMath/algorithm/PolygonFit/InverseHull/InnerHullFinder.h"
#include "CoreMath/algorithm/PolygonFit/polygon/MPolygon.h"
#include "CoreMath/Matrix/2D/MMatrix2D.h"
#include "CoreMath/Debug/CoreMathDebugHelper.h"


void TestAlgorithm::LogMessage(FString s){
    LogMessage(s, " ");
}

void TestAlgorithm::LogMessage(FString prefix, FString s){
    FString appendFront = TEXT("TestAlgorithm: ");
    appendFront += prefix;
    CoreMathDebugHelper::logMessage(appendFront, s);
}

// ---- TESTS ----

void TestAlgorithm::Test(){

    TestInnerHullFinder();
    TestPolygonHit();

    //CoreMathDebugHelper
}

void TestAlgorithm::TestInnerHullFinder(){

    // --- test looks good, as expected ---

    LogMessage("InnerhullFinder", "Start");

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

    LogMessage("InnerhullFinder", "End");;
}




void TestAlgorithm::TestPolygonHit(){

    LogMessage("Mpolygon intersect", "Start");
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

    FVector2D A(210, 201);
    FVector2D B(-110, -130);

    if(polygon.DoesIntersect(A,B)){
        LogMessage("Mpolygon intersect ","test sucess");
    }else{
        LogMessage("Mpolygon intersect ","test failed");
    }


    LogMessage("Mpolygon intersect", "End");
}