#pragma once

class IKHUMANOIDMODELL_API FAbstractPropertyBase {

public:
    float Validate(float size){
        return std::max(std::abs(size), 1.0f);
    }

};