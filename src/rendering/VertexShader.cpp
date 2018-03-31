//
// Created by Zaiyang Li on 19/03/2018.
//

#include <iostream>
#include "VertexShader.hpp"

namespace McRenderer {
    void interpolate(const VertexShaderOutputParams &v1,
                     const VertexShaderOutputParams &v2,
                     float t,
                     VertexShaderOutputParams &result) {
        float oneMinusT = 1 - t;
        result.position = v1.position * oneMinusT + v2.position * t;
        result.viewPosition = v1.viewPosition * oneMinusT + v2.viewPosition * t;
        result.normal = v1.normal * oneMinusT + v2.normal * t;
        result.colour = v1.colour * oneMinusT + v2.colour * t;
        result.textCoord = v1.textCoord * oneMinusT + v2.textCoord * t;
    }

    void Interpolate(const VertexShaderOutputParams &v1,
                            const VertexShaderOutputParams &v2,
                            float t1, float t2,
                            VertexShaderOutputParams &result) {
        result.position = v1.position * t1 + v2.position * t2;
        result.viewPosition = v1.viewPosition * t1 + v2.viewPosition * t2;
        result.normal = v1.normal * t1 + v2.normal * t2;
        result.colour = v1.colour * t1 + v2.colour * t2;
        result.textCoord = v1.textCoord * t1 + v2.textCoord * t2;
    }

    void McRenderer::interpolateBarycentric(const VertexShaderOutputParams *triangleAttributes, vec3 barycentric,
                                            VertexShaderOutputParams &result) {

        result.position = triangleAttributes[0].position * barycentric[0];
        result.viewPosition = triangleAttributes[0].viewPosition * barycentric[0];
        result.normal = triangleAttributes[0].normal * barycentric[0];
        result.colour = triangleAttributes[0].colour * barycentric[0];
        result.textCoord = triangleAttributes[0].textCoord * barycentric[0];
        for(int j = 1; j < 3; j++) {
            result.position += triangleAttributes[j].position * barycentric[j];
            result.viewPosition += triangleAttributes[j].viewPosition * barycentric[j];
            result.normal += triangleAttributes[j].normal * barycentric[j];
            result.colour += triangleAttributes[j].colour * barycentric[j];
            result.textCoord += triangleAttributes[j].textCoord * barycentric[j];
        }

    }
    /**
     * Perspective correct interpolation
     * @param v1
     * @param v2
     * @param t
     * @param result
     */
    void perspectiveInterpolate(const VertexShaderOutputParams &v1, const VertexShaderOutputParams &v2, float t,
                                VertexShaderOutputParams &result) {
        // C = Z [C0/Z0*(1-t) + C1/Z1*t]
        // => C = Z/Z0*(1-t) * C0 + Z/Z1*t* C1
        // => C = A * C0 + B * C1 where A = Z/Z0*(1-t), B = Z/Z1*t

        result.position = v1.position * (1 - t) + v2.position * (t);

        float A = result.position.z / v1.position.z * (1-t);
        float B = result.position.z / v2.position.z * t;

        result.viewPosition = v1.viewPosition * A + v2.viewPosition * B;
        result.normal = v1.normal * A + v2.normal * B;
        result.colour = v1.colour * A + v2.colour * B;
        result.textCoord = v1.textCoord * A + v2.textCoord * B;
    }

    // f = a1 * f1 / z1 + a2 * f2 / z2 + a3 * f3 / z3 / (a1 / z1 + a2 / z2 + a3 / z3)
    void perspectiveInterpolateBarycentric(const VertexShaderOutputParams *triangleAttributes,
                                           vec3 barycentricCoords,
                                           VertexShaderOutputParams &result) {
        vec3 perspetiveCorrectBarycentricCoords = vec3(
                barycentricCoords.x / triangleAttributes[0].position.z,
                barycentricCoords.y / triangleAttributes[1].position.z,
                barycentricCoords.z / triangleAttributes[2].position.z
        );
        float sum = perspetiveCorrectBarycentricCoords[0];

        result.position = triangleAttributes[0].position * barycentricCoords[0];
        result.viewPosition = triangleAttributes[0].viewPosition * perspetiveCorrectBarycentricCoords[0];
        result.normal = triangleAttributes[0].normal * perspetiveCorrectBarycentricCoords[0];
        result.colour = triangleAttributes[0].colour * perspetiveCorrectBarycentricCoords[0];
        result.textCoord = triangleAttributes[0].textCoord * perspetiveCorrectBarycentricCoords[0];

        for(int j = 1; j < 3; j++) {
            result.position += triangleAttributes[j].position * barycentricCoords[j];
            result.viewPosition += triangleAttributes[j].viewPosition * perspetiveCorrectBarycentricCoords[j];
            result.normal += triangleAttributes[j].normal * perspetiveCorrectBarycentricCoords[j];
            result.colour += triangleAttributes[j].colour * perspetiveCorrectBarycentricCoords[j];
            result.textCoord += triangleAttributes[j].textCoord * perspetiveCorrectBarycentricCoords[j];
            sum += perspetiveCorrectBarycentricCoords[j];
        }
        sum = 1 / sum;
        result.viewPosition * sum;
        result.normal * sum;
        result.colour * sum;
        result.textCoord * sum;
    }
}