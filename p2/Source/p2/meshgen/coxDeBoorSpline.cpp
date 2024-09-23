// Fill out your copyright notice in the Description page of Project Settings.


#include "p2/meshgen/coxDeBoorSpline.h"

coxDeBoorSpline::coxDeBoorSpline()
{
}

coxDeBoorSpline::~coxDeBoorSpline()
{
}

void coxDeBoorSpline::calculatecurve(
    std::vector<FVector2D> &controls, 
    TVector<FVector2D> &output,
    int totalLength,
    int gapBetweenInterpolatedPoints
){

    //create the knot points, use first and last 4 times (3 times duplicated)
    int p_degree = 3;
    int n_nodes = controls.size();
    std::vector<float> knots = generateClampedKnotVector(n_nodes, p_degree);

    createOutput(controls, p_degree, knots, output, totalLength, gapBetweenInterpolatedPoints);
}

float coxDeBoorSpline::CoxDeBoor(int i, int p, float t, const std::vector<float>& knotVector) {
    std::vector<float> N(p + 1, 0.0f); // N will store the basis function values

    // Base case for p = 0
    for (int j = 0; j <= p; ++j) {
        if (t >= knotVector[i + j] && t < knotVector[i + j + 1]) {
            N[j] = 1.0f;
        } else {
            N[j] = 0.0f;
        }
    }

    // Iteratively calculate higher-degree basis functions
    for (int k = 1; k <= p; ++k) {
        for (int j = 0; j <= p - k; ++j) {
            float leftDenom = knotVector[i + j + k] - knotVector[i + j];
            float left = (leftDenom != 0) ? (t - knotVector[i + j]) / leftDenom * N[j] : 0.0f;

            float rightDenom = knotVector[i + j + k + 1] - knotVector[i + j + 1];
            float right = (rightDenom != 0) ? (knotVector[i + j + k + 1] - t) / rightDenom * N[j + 1] : 0.0f;

            N[j] = left + right;
        }
    }

    // The desired basis function value for the degree p is now in N[0]
    return N[0];
}


FVector2D coxDeBoorSpline::interpolateBSpline(
    float t, 
    std::vector<FVector2D>& controlPoints, 
    int degree, 
    const std::vector<float>& knotVector
){
    FVector2D result(0, 0); // Resulting point on the spline
    int n = controlPoints.size() - 1;

    // Sum the weighted control points
    for (int i = 0; i <= n; ++i) {
        float basis = CoxDeBoor(i, degree, t, knotVector); // Compute N_{i, p}(t)
        result = result + controlPoints[i] * basis; // Weight the control point by its basis function
    }

    return result;
}


void coxDeBoorSpline::createOutput(
    std::vector<FVector2D>& controlPoints, 
    int degree, 
    const std::vector<float>& knotVector,
    TVector<FVector2D> &output,
    int distanceTotal,
    int gapBetween 
){
    
    //step must be created from the distance of tbe whole curve
    //or something similar to ensure the correct t value increasing

    float rate = (distanceTotal / gapBetween);
    float tincrease = 1.0f / rate;
    if(tincrease == 0){
        return;
    }

    for (float t = 0; t < 1.0f; t += tincrease){
        FVector2D point = interpolateBSpline(
            t,
            controlPoints,
            degree,
            knotVector);

        output.push_back(point);
    }
}

 std::vector<float> coxDeBoorSpline::generateClampedKnotVector(int n, int p) {
    int m = n + p + 1;
    std::vector<float> knotVector(m, 0.0f);

    // Set first p+1 knots to 0 (clamped start)
    for(int i = 0; i <= p; ++i){
        knotVector[i] = 0.0f;
    }

    // Set last p+1 knots to 1 (clamped end)
    for(int i = m - p -1; i < m; ++i){
        knotVector[i] = 1.0f;
    }

    // Set middle knots uniformly spaced between 0 and 1
    int numMiddleKnots = m - 2*(p+1);
    if(numMiddleKnots > 0){
        for(int i = 1; i <= numMiddleKnots; ++i){
            knotVector[p + i] = static_cast<float>(i) / (numMiddleKnots + 1);
        }
    }

    return knotVector;
}
