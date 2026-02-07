#include "mesh.h"
#include "../../common/type.h"
#include "../../thirdparty/fast_obj/fast_obj.h"

static mesh_t* obj_mesh_load(const char* filePath) {
    mesh_t* mesh = (mesh_t*)malloc(sizeof(mesh_t));
    fastObjMesh* obj = fast_obj_read(filePath);

    /* load vertex */
    mesh->vertexCount = obj->position_count;
    mesh->vertices = (vector3d_t*)malloc(sizeof(vector3d_t) * mesh->vertexCount);
    memset(mesh->vertices, 0, sizeof(vector3d_t) * mesh->vertexCount);
    for (unsigned int i = 0; i < mesh->vertexCount; i++) {
        mesh->vertices[i].x = obj->positions[i * 3];
        mesh->vertices[i].y = obj->positions[i * 3 + 1];
        mesh->vertices[i].z = obj->positions[i * 3 + 2];
    }

    /* load face */
    mesh->indexCount = obj->index_count / 3;
    mesh->indices = (VertexIndex_t*)malloc(sizeof(VertexIndex_t) * obj->index_count);
    memset(mesh->indices, 0, sizeof(VertexIndex_t) * obj->index_count);
    for (unsigned int i = 0; i < obj->index_count; i++) {
        mesh->indices[i].vertexIdx = obj->indices[i].p;
        mesh->indices[i].uvIdx = obj->indices[i].t;
        mesh->indices[i].normalIdx = obj->indices[i].n;
    }

    fast_obj_destroy(obj);
    return mesh;
}

mesh_t* mesh_load(const char* filePath) {
    return obj_mesh_load(filePath);
}
