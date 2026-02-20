#pragma once

/// @brief INTERFACE ONLY
class TERRAINPLUGINBASE_API TerrainInterfaceBase {

public:
    TerrainInterfaceBase() {};
    ~TerrainInterfaceBase() {};

    // ---- ALL METHODS TO BE OVERRIDEN, SPLIT UP FOR PLUGIN TASK CLARITY ----

    //to be overriden. Will now perform a raycast or none
    virtual float getHeightFor(FVector &position);

    virtual std::pair<int, int> Index2DFromWorldPosition(
		const FVector &worldPos
	);

    virtual bool ChunkPositionFromIndexPair(FVector &outPos, const std::pair<int, int> &index);

    virtual void lockQuadsFromParalellArrayLines(
		const TArray<FVector> &line0,
		const TArray<FVector> &line1
	){};




private:


};