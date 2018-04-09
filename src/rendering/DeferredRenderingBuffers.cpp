//
// Created by Zaiyang Li on 09/04/2018.
//

#include "DeferredRenderingBuffers.hpp"

namespace McRenderer {
    using namespace glm;
    void DeferredRenderingBuffers::allocateMemory() {
        int size = width * height;
        if(size <= 0) {
            diffuse.reset();
            specular.reset();
            normal.reset();
            depth.reset();
            position.reset();
            lightAccumulation.reset();
            ambientOcclusion.reset();
        } else {
            diffuse.reset(new vec4[size]);
            specular.reset(new vec4[size]);
            normal.reset(new vec3[size]);
            depth.reset(new float[size]);
            position.reset(new vec4[size]);
            lightAccumulation.reset(new vec4[size]);
            ambientOcclusion.reset(new float[size]);
        }
    }

    void DeferredRenderingBuffers::resize(int newWdith, int newHeight) {
        width = newWdith;
        height = newHeight;
        allocateMemory();
    }
}