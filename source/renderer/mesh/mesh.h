#ifndef _MESH_H
#define _MESH_H

#include "../../vector/vector.h"

typedef struct {
    unsigned int vertexIdx;
    unsigned int uvIdx;    
    unsigned int normalIdx;
} VertexIndex_t;

typedef struct {
    vector3d_t* vertices;    // index from 1 start.
    unsigned int vertexCount; // Number of vertices

    VertexIndex_t* indices; // index from 0 start.
    unsigned int indexCount; // Number of indices
} mesh_t;

mesh_t* mesh_load(const char* filePath);

#endif