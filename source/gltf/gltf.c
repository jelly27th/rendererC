#include "gltf.h"

#define CGLTF_IMPLEMENTATION
#include "../thirdparty/cgltf/cgltf.h"

cgltf_data* gltfLoadFromFile(const char* path) {
  cgltf_options options = {0};
  cgltf_data* data = NULL;
  cgltf_result result = cgltf_parse_file(&options, path, &data);
  if (result == cgltf_result_success)
  {
    return data;
  }
  else {
    perror("open failed!");
    exit(1);
  }
  return NULL;
}

void gltfFree(cgltf_data* data) {
    cgltf_free(data);
}