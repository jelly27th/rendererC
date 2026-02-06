#ifndef GLTF_H
#define GLTF_H

#include "../thirdparty/cgltf/cgltf.h"

cgltf_data* gltfLoadFromFile(const char* path);
void gltfFree(cgltf_data* data);

#endif