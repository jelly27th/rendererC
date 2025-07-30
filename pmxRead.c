#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "vector/vector.h"

typedef  unsigned char uint8_t;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t;
typedef  unsigned long long uint64_t;
typedef  float float32_t;
typedef  double float64_t;
typedef  short int16_t;
typedef  int   int32_t;

typedef struct
{
    uint32_t len;
    wchar_t *data_wide; // For wide character support
} pmx_text_t;
typedef struct {
    
    uint8_t sign[4]; // PMX file signature
    float32_t version; // PMX file version
    uint8_t goalCount; // Number of goals
    
    uint8_t	encode;	//0:UTF16 1:UTF8
    uint8_t	addUV4Num;
    uint8_t	vertexIndexSize;
    uint8_t	textureIndexSize;
    uint8_t	materialIndexSize;
    uint8_t	boneIndexSize;
    uint8_t	morphIndexSize;
    uint8_t	rigidbodyIndexSize;

    pmx_text_t localModelName; // Local model name
    pmx_text_t generalModelName; // General model name
    pmx_text_t localModelComment; // Local model comment
    pmx_text_t generalModelComment; // General model comment
} pmx_header_t;

/*
    BDEF1
    boneIndices[0]

    BDEF2
    boneIndices[0-1]
    boneWeights[0]

    BDEF4
    boneIndices[0-3]
    boneWeights[0-3]

    SDEF
    boneIndices[0-1]
    boneWeights[0]
    sdefC
    sdefR0
    sdefR1

    QDEF
    boneIndices[0-3]
    boneWeights[0-3]
*/
typedef enum {
    BDEF1,
    BDEF2,
    BDEF4,
    SDEF,
    QDEF,
}pmx_weight_t;
typedef struct
{
    vector2d_t position[3]; // Vertex position (x, y, z)
    vector3d_t normal;   // Vertex normal (nx, ny, nz)
    vector2d_t uv;       // Texture coordinates (u, v)
    
    vector4d_t* addUV; // Additional data for vec4 if applicable
    
    pmx_weight_t weightType;    // Weight type: 0=BDEF1, 1=BDEF2, 2=BDEF4, 3=SDEF, 4=QDEF
    int32_t boneIndices[4]; // Bone indices for skinning
    float32_t boneWeights[4]; // Bone weights for skinning
    vector3d_t sdefC; // SDEF center
    vector3d_t sdefR0; // SDEF reference point 0
    vector3d_t sdefR1; // SDEF reference point 1

    float32_t edgeMag; // Edge magnitude for SDEF

} pmx_vertex_data_t;
typedef struct {
    uint32_t count;
    pmx_vertex_data_t* data; // Pointer to vertex data
} pmx_vertex_t;

typedef struct {
    uint32_t indices[3]; // Indices of the face vertices
} pmx_face_data_t;

typedef struct {
    uint32_t count; // Number of faces
    pmx_face_data_t* data; // Pointer to face data
} pmx_face_t;

typedef struct {
    pmx_text_t path; // Texture file path
} pmx_texture_t;

typedef enum {
    NoCull = 0x01,
    GroundShadow = 0x02,
    DrawSelfShadow = 0x04,
    RecieveSelfShadow = 0x08,
    DrawEdge = 0x10,
    VertexColor = 0x20,
    DrawPoint = 0x40,
    DrawLine = 0x80,
} pmx_draw_mode_flags_t;

typedef enum {
    None,
    Mul,
    Add,
    SubTexture,
} pmx_blend_mode_t;

typedef enum {
    external,
    internal
} pmx_toon_mode_t;

typedef struct {
    pmx_text_t localMaterialName; 
    pmx_text_t generalMaterialName; 

    vector4d_t diffuse; // Diffuse color (RGBA)
    vector3d_t specular; // Specular color (RGB)
    float32_t specularPower; // Specular power
    vector3d_t ambient; // Ambient color (RGB)

    pmx_draw_mode_flags_t drawMode; // Draw mode flags

    vector4d_t edgeColor; // Edge color (RGBA)
    float32_t edgeRatio; // Edge ratio

    uint32_t textureIndex; // Texture index
    uint32_t specularTextureIndex; // Sphere texture index
    pmx_blend_mode_t blendMode; // Toon texture factor

    pmx_toon_mode_t toonMode; // Texture reference type
    int32_t toonTextureIndex; // Toon texture index

    pmx_text_t memo; // Material memo

    uint32_t numFace; // Number of face vertices
} pmx_material_t;
typedef struct {
    pmx_header_t header; // PMX file header
    pmx_vertex_t vertex; // Vertex data
    pmx_face_t face; // Face data
    pmx_material_t material; // Material data
} pmx_t;

pmx_t pmx;
unsigned char buffer[65536];
void read_file(char *filename)
{
    setlocale(LC_ALL, "");// suggested print to windows terminal

    FILE *fd = fopen(filename,"rb"); 
    if(fd == NULL)
    {
        perror("open failed!");
        exit(1);        //出错、退出
    }


    fread(&(pmx.sign[0]), sizeof(pmx.sign), 1, fd);

    fread(&(pmx.version), sizeof(pmx.version), 1, fd);
    
    fread(&(pmx.goalCount), sizeof(pmx.goalCount), 1, fd);
    
    fread(&(pmx.goalType[0]), sizeof(pmx.goalType), 1, fd);
    
    fread(&(pmx.localModelName.len), sizeof(pmx.localModelName.len), 1, fd);
    pmx.localModelName.data_wide = (wchar_t  *)malloc(sizeof(wchar_t ) * (pmx.localModelName.len / 2 + 1));
    fread(pmx.localModelName.data_wide, pmx.localModelName.len, 1, fd);
    pmx.localModelName.data_wide[pmx.localModelName.len / 2] = L'\0'; // Null-terminate the string
    
    fread(&(pmx.generalModelName.len), sizeof(pmx.generalModelName.len), 1, fd);
    pmx.generalModelName.data_wide = (wchar_t  *)malloc(sizeof(wchar_t ) * (pmx.generalModelName.len / 2 + 1));
    fread(pmx.generalModelName.data_wide, pmx.generalModelName.len, 1, fd);
    pmx.generalModelName.data_wide[pmx.generalModelName.len / 2] = L'\0'; // Null-terminate the string
    
    fread(&(pmx.localModelComment.len), sizeof(pmx.localModelComment.len), 1, fd);
    pmx.localModelComment.data_wide = (wchar_t  *)malloc(sizeof(wchar_t ) * (pmx.localModelComment.len / 2 + 1));
    fread(pmx.localModelComment.data_wide, pmx.localModelComment.len, 1, fd);
    pmx.localModelComment.data_wide[pmx.localModelComment.len / 2] = L'\0'; // Null-terminate the string
    wprintf(L"Local Model Comment: %ls\n", pmx.localModelComment.data_wide);
    
    fread(&(pmx.generalModelComment.len), sizeof(pmx.generalModelComment.len), 1, fd);
    pmx.generalModelComment.data_wide = (wchar_t  *)malloc(sizeof(wchar_t ) * (pmx.generalModelComment.len / 2 + 1));
    fread(pmx.generalModelComment.data_wide, pmx.generalModelComment.len, 1, fd);
    pmx.generalModelComment.data_wide[pmx.generalModelComment.len / 2] = L'\0'; // Null-terminate the string
    wprintf(L"General Model Comment: %ls\n", pmx.generalModelComment.data_wide);

    fread(&(pmx.vertex.count), sizeof(pmx.vertex.count), 1, fd);
    pmx.vertex.size = 3 * 4 + 3 * 4 + 2 * 4; //32 position, normal, and UV coordinates
    if (pmx.goalType[1] !=0 ) {
      pmx.vertex.size += 4 * 4 * pmx.goalType[1]; // Additional data for vec4
    }
    pmx.vertex.size += 1; // variable weight type 0=BDEF1，1=BDEF2，2=BDEF4，3=SDEF，4=QDEF

    fread(&buffer[0],sizeof(buffer),1,fd);
    printf("Read %d bytes from file.\n", buffer[33]);
    fclose(fd);
}

int main()
{
  read_file("C:\\Users\\dong\\Downloads\\xiao\\xiao.pmx");

  return 0;
}