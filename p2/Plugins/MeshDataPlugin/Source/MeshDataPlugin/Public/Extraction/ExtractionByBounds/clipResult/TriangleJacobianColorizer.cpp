#include "TriangleJacobianColorizer.h"


TriangleJacobianColorizer::TriangleJacobianColorizer(){

}

TriangleJacobianColorizer::~TriangleJacobianColorizer(){
    
}



FColor TriangleJacobianColorizer::DistortionColorFor(
    const FVector &vertex,
    const FVector &v1In,
    const FVector &v2In,
    const FVector2D &uvVertex,
    const FVector2D &uv1,
    const FVector2D &uv2,
    const FColor &distortColorNone,
    const FColor &distortColorFull
    //distortion Color All, distort color none!
){

    //beschreibt wie gross ein dreieck auf eine UV fläche verteilt ist.
    //hat auch grössen verzerrung.
    
    //aus ableitung der position und uv mit
    /*
    \begin{equation}
    e_1 = p_1 - p_0, \qquad e_2 = p_2 - p_0
    \end{equation}

    sowie die UV-Kanten

    \begin{equation}
    t_1 = uv_1 - uv_0, \qquad t_2 = uv_2 - uv_0,
    \end{equation}.

    ...
    $$
    p(\alpha, \beta) = p_0 + 
    \begin{bmatrix}
        e_1 & e_2
    \end{bmatrix} \cdot
    \begin{bmatrix}
        t_{1x} & t_{2x} \\
        t_{1y} & t_{2y}
    \end{bmatrix}^{-1}
    \cdot 
    (uv_{out} - uv_0)
    $$

    Mit 
    $$
    \frac{\partial p}{\partial uv} =
    \begin{bmatrix}
        e_1 & e_2
    \end{bmatrix} \cdot
    \begin{bmatrix}
        t_{1x} & t_{2x} \\
        t_{1y} & t_{2y}
    \end{bmatrix}^{-1} = J 
    $$
    ...
    
    wird
    $$
    A = \sqrt{det(J^TJ)} = ||u||\cdot ||v|| \cdot sin(\theta)
    $$
    */

    FVector e1 = v1In - vertex;
    FVector e2 = v2In - vertex;
    FVector2D t1 = uv1 - uvVertex;
    FVector2D t2 = uv2 - uvVertex;

    //mit
    /*
    J = 
    \begin{bmatrix}
        e_1 & e_2
    \end{bmatrix} \cdot
    \begin{bmatrix}
        t_{1x} & t_{2x} \\
        t_{1y} & t_{2y}
    \end{bmatrix}^{-1}
    =
    e1x e2x   t1x t2x
    e1y e2y * t1y t2y
    e1z e2z
    */

    /*
    J =\begin{pmatrix}
        \frac{\partial x}{\partial u} & \frac{\partial x}{\partial v} \\
        \frac{\partial y}{\partial u} & \frac{\partial y}{\partial v} \\
        \frac{\partial z}{\partial u} & \frac{\partial z}{\partial v}
    \end{pmatrix}
    = [dp/du,dp/dv]
    
    */


    //A = \sqrt{det(J^TJ)} = ||u||\cdot ||v|| \cdot sin(\theta)
    FVector JacobianU(
        e1.X * t1.X + e2.X * t1.Y,
        e1.Y * t1.X + e2.Y * t1.Y,
        e1.Z * t1.X + e2.Z * t1.Y
    );

    FVector JacobianV(
        e1.X * t2.X + e2.X * t2.Y,
        e1.Y * t2.X + e2.Y * t2.Y,
        e1.Z * t2.X + e2.Z * t2.Y
    );

    //sin(\theta) = cos^-1(JacobianUNormalized * JacobianVNormalized)
    float theta = std::acos(FVector::DotProduct(
        JacobianU.GetSafeNormal(),
        JacobianV.GetSafeNormal()
    ));
    float sinTheta = std::sin(theta);

    float A = JacobianU.Size() * JacobianV.Size() * sinTheta;

    //--- todo: metrik mit A und sin Theta ---
    //sin theta ggf hinreichend!

    bool fromArea = false;
    float scalar = 0.0f;
    if(fromArea){
        scalar = FMath::Clamp(A - 1.0f, -1.0f, 1.0f);
    }else{
        scalar = FMath::Clamp(sinTheta, -1.0f, 1.0f);
    }
    scalar = std::abs(scalar);

    /*
    const FColor &distortColorNone,
    const FColor &distortColorFull
    */
    return MetrikColor(distortColorNone, distortColorFull, scalar);
}

FColor TriangleJacobianColorizer::MetrikColor(
    const FColor &distortColorNone,
    const FColor &distortColorFull,
    float scalar
){
    scalar = std::abs(scalar);
    FColor result;
    result.R = FMath::Clamp<int32>(distortColorNone.R + distortColorFull.R * scalar, 0, 255);
    result.G = FMath::Clamp<int32>(distortColorNone.G + distortColorFull.G * scalar, 0, 255);
    result.B = FMath::Clamp<int32>(distortColorNone.B + distortColorFull.B * scalar, 0, 255);
    result.A = 255; // Alpha unverändert, 1.0f

    return result;
}




