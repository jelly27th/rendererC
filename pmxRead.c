#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <locale.h>
#include "vector/vector.h"

unsigned char buffer[65536];
typedef  unsigned char uint8_t;
typedef  unsigned short uint16_t;
typedef  unsigned int uint32_t;
typedef  unsigned long long uint64_t;
typedef  float float32_t;
typedef  double float64_t;
typedef  short int16_t;
typedef  int   int32_t;

/* pmx header struct */
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

/* pmx vertex struct*/

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
    vector3d_t position; // Vertex position (x, y, z)
    vector3d_t normal;   // Vertex normal (nx, ny, nz)
    vector2d_t uv;       // Texture coordinates (u, v)
    
    vector4d_t* addUV; // Additional data for vec4 if applicable
    
    uint8_t weightType;    // Weight type: 0=BDEF1, 1=BDEF2, 2=BDEF4, 3=SDEF, 4=QDEF
    int32_t boneIndices[4]; // Bone indices for skinning
    float32_t boneWeights[4]; // Bone weights for skinning
    vector3d_t sdefC; // SDEF center
    vector3d_t sdefR0; // SDEF reference point 0
    vector3d_t sdefR1; // SDEF reference point 1

    float32_t edgeMag; // Edge magnitude for SDEF

} pmx_vertex_data_t;
typedef struct {
    int32_t count;
    pmx_vertex_data_t* data; // Pointer to vertex data
} pmx_vertex_t;

typedef struct {
    uint32_t indices[3]; // Indices of the face vertices
} pmx_face_data_t;

typedef struct {
    int32_t count; // Number of faces
    pmx_face_data_t* data; // Pointer to face data
} pmx_face_t;

typedef struct {
    int32_t Number;
    pmx_text_t* path; // Texture file path
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
    blend_None,
    blend_Mul,
    blend_Add,
    blend_SubTexture,
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

    uint8_t drawMode; // Draw mode flags

    vector4d_t edgeColor; // Edge color (RGBA)
    float32_t edgeRatio; // Edge ratio

    int32_t textureIndex; // Texture index
    int32_t specularTextureIndex; // Sphere texture index
    uint8_t blendMode;             // Toon texture factor

    uint8_t toonMode;         // Texture reference type
    int32_t toonTextureIndex; // Toon texture index

    pmx_text_t memo; // Material memo

    int32_t numFace; // Number of face vertices
} pmx_material_data_t;

typedef struct {
    int32_t count;
    pmx_material_data_t* data;
} pmx_material_t;

typedef enum {
    TargetShowMode = 0x0001,
    AllowRotate = 0x0002,
    AllowTranslate = 0x0004,
    Visible = 0x0008,
    AllowControl = 0x0010,
    IK = 0x0020,
    AppendLocal = 0x0080,
    AppendRotate = 0x0100,
    AppendTranslate = 0x0200,
    FixedAxis = 0x0400,
    LocalAxis = 0x800,
    DeformAfterPhysics = 0x1000,
    DeformOuterParent = 0x2000,
}pmx_bone_flags_t;

typedef struct {
    int32_t boneIndex; // Bone index
    unsigned char angleLimit; // Angle limit for the bone

    vector3d_t minLimit; // Minimum angle limit (x, y, z)
    vector3d_t maxLimit; // Maximum angle limit (x, y, z)
}pmx_lk_link_t;

typedef struct {
    pmx_text_t localBoonName; // Bone name
    pmx_text_t generalBoonName; // English bone name

    vector3d_t position; // Bone position (x, y, z)
    int32_t    parentBoneIndex; // Parent bone index
    int32_t    deformDepth;
    uint16_t   boneflag;  // Bone flags

    /* tail position */
    vector3d_t tailPosition; // Tail position (x, y, z)
    int32_t    tailBoneIndex; // Target bone index for IK

    /* Bone Inheritance */
    int32_t inheritBoneIndex; // Bone index for inheritance
    float32_t inheritWeight; // Inheritance weight

    /* stationary shaft */ 
    vector3d_t fixedAxis; // Fixed axis (x, y, z)

    /* Local axis */ 
    vector3d_t localAxisX; // Local axis X (x, y, z)
    vector3d_t localAxisY; // Local axis Y (x, y, z

    /* External relatives */ 
    int32_t externalParentKey; // Key value for external parent deformation

    /* IK */ 
    int32_t ikTargetBoneIndex; // IK target bone index
    int32_t ikIterationCount; // IK iteration count
    float32_t ikLimitAngle; // IK limit angle
    int32_t ikLinkCount; // Number of IK links
    pmx_lk_link_t* ikLinks; // Pointer to IK links

} pmx_bone_data_t;

typedef struct {
    int32_t count;
    pmx_bone_data_t* data;
} pmx_bone_t;

typedef	enum {
    Group,
    Position,
    Bone,
    UV,
    AddUV1,
    AddUV2,
    AddUV3,
    AddUV4,
    Material,
    Flip,
    Impluse,
} pmx_morph_type_t;

typedef struct  {
    int32_t	morphIndex;
    float	weight;
}pmx_group_morph_t;

typedef struct {
    int32_t		vertexIndex;
    vector3d_t	position;
} pmx_vertex_morph_t;

typedef struct  {
    int32_t		boneIndex;
    vector3d_t	position;
    vector4d_t	quaternion;
}pmx_bone_morph_t;

typedef struct {
    int32_t		vertexIndex;
    vector4d_t	uv;
}pmx_uv_morph_t;

typedef enum {
    op_Mul,
    op_Add,
}pmx_op_type_t;
typedef struct  {

    int32_t		materialIndex;
    uint8_t		opType;	
    vector4d_t	diffuse;
    vector3d_t	specular;
    float		specularPower;
    vector3d_t	ambient;
    vector4d_t	edgeColor;
    float		edgeSize;
    vector4d_t	textureFactor;
    vector4d_t	sphereTextureFactor;
    vector4d_t	toonTextureFactor;
}pmx_material_morph_t;
typedef struct {
    int32_t	morphIndex;
    float	m_weight;
}pmx_flip_morph_t;

typedef struct {
    int32_t		rigidbodyIndex;
    uint8_t		localFlag;	//0:OFF 1:ON
    vector3d_t	translateVelocity;
    vector3d_t	rotateTorque;
}pmx_impulse_morph_t;
typedef struct {
    pmx_text_t localMorphName; // Morph name
    pmx_text_t generalMorphName; // English morph name

    uint8_t controlPanel; // Control panel index
    uint8_t morphType; // Morph type

    int32_t morphCount; // Number of morphs
    pmx_vertex_morph_t* positionMorphs; // Position morphs
    pmx_uv_morph_t* uvMorphs; // UV morphs
    pmx_bone_morph_t* boneMorphs; // Bone morphs
    pmx_material_morph_t* materialMorphs; // Material morphs
    pmx_group_morph_t* groupMorphs; // Group morphs
    pmx_flip_morph_t* flipMorphs; // Flip morphs
    pmx_impulse_morph_t* impulseMorphs; // Impulse morphs

} pmx_morph_data_t;

typedef struct {
    int32_t count; // Number of morphs
    pmx_morph_data_t* data; // Pointer to morph data
} pmx_morph_t;

typedef enum {
    DefaultFrame,
    SpecialFrame,
} pmx_frame_type_t;

typedef enum{
    BoneIndex,
    MorphIndex,
} pmx_target_type_t;

typedef struct {
    uint8_t targetType; // Target type: BoneIndex or MorphIndex
    int32_t targetIndex; // Index of the target (bone or morph)
} pmx_frame_target_t;

typedef struct {

    pmx_text_t localFrameName; // Frame name
    pmx_text_t generalFrameName; // English frame name

    uint8_t frameType; // Frame type

    int32_t frameCount; // Number of targets in the frame
    pmx_frame_target_t* targets; // Pointer to target data

} pmx_displayFrame_data_t;

typedef struct {
    int32_t count; // Number of display frames
    pmx_displayFrame_data_t* data; // Pointer to display frame data
} pmx_displayFrame_t;

typedef enum {
    Sphere,
    Box,
    Capsule,
}pmx_rigidShape_t;

typedef enum{
    Static,
    Dynamic,
    DynamicAndBoneMerge
}pmx_rigidOperation_t;
typedef struct {
    pmx_text_t localName; // Local name
    pmx_text_t generalName; // General name

    int32_t boneIndex; // Bone index
    uint8_t groupID; // Group index
    uint16_t collisionGroup; // Collision group index

    pmx_rigidShape_t shape; // Shape type: Sphere, Box, Capsule
    vector3d_t shapeSize; // Shape size (x, y, z)
    vector3d_t translate; // Position (x, y, z)
    vector3d_t rotate; // Rotation (x, y, z in radians)

    float32_t mass; // Mass of the rigid body
    float32_t translateDimmer; // Damping factor for translation
    float32_t rotateDimmer; // Damping factor for rotation
    float32_t repulsion; // Repulsion factor
    float32_t friction; // Friction factor
    pmx_rigidOperation_t operation; // Operation type: Static, Dynamic, DynamicAndBoneMerge

} pmx_rigidBody_t;

typedef enum {
    SpringDOF6,
    DOF6,
    P2P,
    ConeTwist,
    Slider,
    Hinge,
} pmx_jointType_t; // Joint type
typedef struct {
    pmx_text_t localJointName; // Joint name
    pmx_text_t generalJointName; // English joint name

    pmx_jointType_t type; // Joint type
    int32_t rigidBodyAIndex; // Index of the first rigid body
    int32_t rigidBodyBIndex; // Index of the second rigid body

    vector3d_t translate; // Translation (x, y, z)
    vector3d_t rotate; // Rotation (x, y, z)

    vector3d_t translateLowerLimit; // Lower limit for translation (x, y, z)
    vector3d_t translateUpperLimit; // Upper limit for translation (x, y, z)
    vector3d_t rotateLowerLimit; // Lower limit for rotation (x, y, z)
    vector3d_t rotateUpperLimit; // Upper limit for rotation (x, y, z)

    vector3d_t springTranslateFactor; // Spring factor for translation (x, y, z)
    vector3d_t springRotateFactor; // Spring factor for rotation (x, y, z)

} pmx_joint_t;

typedef enum {
    TriMesh,
    Rope,
}pmx_softbodyType_t;

typedef	enum {
	BLink = 0x01,
	Cluster = 0x02,
	HybridLink = 0x04,
} pmx_softbodyMask_t; // Softbody mask flags

typedef struct {
    int32_t rigidBodyIndex; // Index of the rigid body
    int32_t vertexIndex; // Index of the vertex
    uint8_t nearMode; // Near mode: 0=FF, 1=ON
}pmx_anchorRigidbody_t;
typedef struct {
    pmx_text_t localSoftbodyName; // Softbody name
    pmx_text_t generalSoftbodyName; // English softbody name

    pmx_softbodyType_t type; // Softbody type: TriMesh or Rope
    int32_t materialIndex; // Material index
    
    uint8_t group; // Group index
    uint16_t collisionGroup; // Collision group index

    pmx_softbodyMask_t flag; // Softbody mask flags

    int32_t bLinkLength; // B-Link length
    int32_t numClusters; // Number of clusters

    float32_t totalMass; // Total mass of the softbody
    float32_t collisionMargin; // Collision margin

    /*
        0:V_TwoSided
        1:V_OneSided
        2:F_TwoSided
        3:F_OneSided
    */
    int32_t AeroModel; // Aero model type

    float32_t VCF; // Volume conservation factor
    float32_t DP; // Drag coefficient
    float32_t DG; // Lift coefficient
    float32_t LF; // Lift factor
    float32_t PR; // Pressure factor
    float32_t VC; // Volume factor
    float32_t DF; // Dynamic friction factor
    float32_t MT; // Material factor
    float32_t CHR; // Cluster hardness
    float32_t KHR; // Kinematic hardness
    float32_t AHR; // Aero hardness

    float32_t SRHR_CL; // Softbody rigid hardness
    float32_t SKHR_CL; // Softbody kinematic hardness
    float32_t SSHR_CL; // Softbody self hardness
    float32_t SR_SPLT_CL;
	float32_t SK_SPLT_CL;
	float32_t SS_SPLT_CL;

    int32_t V_IT; // Iteration count for volume
    int32_t P_IT; // Iteration count for pressure
    int32_t D_IT; // Iteration count for drag
    int32_t C_IT; // Iteration count for collision

    int32_t LST; // Linear stiffness
    int32_t AST; // Angular stiffness
    int32_t VST; // Volume stiffness

    int32_t AnchorRigidbodyCount; // Number of anchor rigidbodies
    pmx_anchorRigidbody_t* anchorRigidbodies; // Pointer to anchor rigidbodies

    int32_t pinVertexCount; // Number of pinned vertices
    int32_t* pinVertexIndices; // Pointer to pinned vertex indices
} pmx_softbody_t;
typedef struct {
    pmx_header_t header; // PMX file header
    pmx_vertex_t vertex; // Vertex data
    pmx_face_t face; // Face data
    pmx_texture_t texture; // Texture data
    pmx_material_t material; // Material data
    pmx_bone_t bone; // Bone data
    pmx_morph_t morph; // Morph data
    pmx_displayFrame_t displayFrame; // Display frame data
    pmx_rigidBody_t rigidBody; // Rigid body data
    pmx_joint_t joint; // Joint data
    pmx_softbody_t softbody; // Softbody data
} pmx_t;

void pmxRead(void* buffer, uint32_t size, FILE* fd) {
    fread(buffer, size, 1, fd);
}

void pmxReadString(pmx_text_t* string, FILE* fd) {
    pmxRead(&(string->len), sizeof(string->len), fd);
    string->data_wide = (wchar_t *)malloc(sizeof(wchar_t) * (string->len / 2 + 1));
    pmxRead(string->data_wide, string->len, fd);
    string->data_wide[string->len / 2] = L'\0'; // Null-terminate the string
}

void pmxReadHeader(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->header.sign[0]), sizeof(pmx->header.sign), fd);

    pmxRead(&(pmx->header.version), sizeof(pmx->header.version), fd);

    pmxRead(&(pmx->header.goalCount), sizeof(pmx->header.goalCount), fd);

    pmxRead(&(pmx->header.encode), sizeof(pmx->header.encode), fd);
    pmxRead(&(pmx->header.addUV4Num), sizeof(pmx->header.addUV4Num), fd);
    pmxRead(&(pmx->header.vertexIndexSize), sizeof(pmx->header.vertexIndexSize), fd);
    pmxRead(&(pmx->header.textureIndexSize), sizeof(pmx->header.textureIndexSize), fd);
    pmxRead(&(pmx->header.materialIndexSize), sizeof(pmx->header.materialIndexSize), fd);
    pmxRead(&(pmx->header.boneIndexSize), sizeof(pmx->header.boneIndexSize), fd);
    pmxRead(&(pmx->header.morphIndexSize), sizeof(pmx->header.morphIndexSize), fd);
    pmxRead(&(pmx->header.rigidbodyIndexSize), sizeof(pmx->header.rigidbodyIndexSize), fd);

    pmxReadString(&pmx->header.localModelName, fd);
    pmxReadString(&pmx->header.generalModelName, fd);
    pmxReadString(&pmx->header.localModelComment, fd);
    pmxReadString(&pmx->header.generalModelComment, fd);

}

void pmxReadIndex(int32_t* index, uint8_t Type, FILE* fd) {
    switch (Type) {
        case 1: {
            uint8_t idx;
            pmxRead(&idx, sizeof(idx), fd);
            if (idx != 0xFF) {
                *index = (int32_t)idx;
            } else {
                *index = -1;
            }
        } 
        break;
        case 2: {
            uint16_t idx;
            pmxRead(&idx, sizeof(idx), fd);
            if (idx != 0xFFFF) {
                *index = (int32_t)idx;
            } else {
                *index = -1;
            }
        } 
        break;
        case 4: {
            uint32_t idx;
            pmxRead(&idx, sizeof(idx), fd);
            if (idx != 0xFFFFFFFF) {
                *index = (int32_t)idx;
            } else {
                *index = -1;
            }
        } 
        break;
        default:
        break;
    }
}

void pmxReadVertex(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->vertex.count), sizeof(pmx->vertex.count), fd);
    pmx->vertex.data = (pmx_vertex_data_t *)malloc(sizeof(pmx_vertex_data_t) * (pmx->vertex.count));

    for (uint32_t index = 0; index < pmx->vertex.count; index++) {
        pmx_vertex_data_t* vertex = &(pmx->vertex.data[index]);

        pmxRead(&(vertex->position), sizeof(vertex->position), fd);
        pmxRead(&(vertex->normal), sizeof(vertex->normal), fd);
        pmxRead(&(vertex->uv), sizeof(vertex->uv), fd);

        if (0 != pmx->header.addUV4Num) {
            vertex->addUV = (vector4d_t*)malloc(sizeof(vector4d_t) * (pmx->header.addUV4Num));
            for (uint32_t i = 0; i < pmx->header.addUV4Num; i++) {
              pmxRead(&(vertex->addUV[i]), sizeof(vertex->addUV[i]), fd);
            }
        }

        pmxRead(&(vertex->weightType), sizeof(vertex->weightType), fd);

        switch (vertex->weightType) {
          case BDEF1:
            pmxReadIndex(&vertex->boneIndices[0], pmx->header.boneIndexSize, fd);
            break;
          case BDEF2:            
            pmxReadIndex(&vertex->boneIndices[0], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[1], pmx->header.boneIndexSize, fd);
            pmxRead(&vertex->boneWeights[0], sizeof(vertex->boneWeights[0]), fd);
            break;
          case BDEF4:
            pmxReadIndex(&vertex->boneIndices[0], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[1], pmx->header.boneIndexSize, fd);            
            pmxReadIndex(&vertex->boneIndices[2], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[3], pmx->header.boneIndexSize, fd);
            pmxRead(&vertex->boneWeights[0], sizeof(vertex->boneWeights[0]), fd);
            pmxRead(&vertex->boneWeights[1], sizeof(vertex->boneWeights[1]), fd);
            pmxRead(&vertex->boneWeights[2], sizeof(vertex->boneWeights[2]), fd);
            pmxRead(&vertex->boneWeights[3], sizeof(vertex->boneWeights[3]), fd);
            break;
          case SDEF:
            pmxReadIndex(&vertex->boneIndices[0], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[1], pmx->header.boneIndexSize, fd);            
            pmxRead(&vertex->boneWeights[0], sizeof(vertex->boneWeights[0]), fd);
            pmxRead(&vertex->sdefC, sizeof(vertex->sdefC), fd);
            pmxRead(&vertex->sdefR0, sizeof(vertex->sdefR0), fd);
            pmxRead(&vertex->sdefR1, sizeof(vertex->sdefR1), fd);
            break;
          case QDEF:            
            pmxReadIndex(&vertex->boneIndices[0], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[1], pmx->header.boneIndexSize, fd);            
            pmxReadIndex(&vertex->boneIndices[2], pmx->header.boneIndexSize, fd);
            pmxReadIndex(&vertex->boneIndices[3], pmx->header.boneIndexSize, fd);
            pmxRead(&vertex->boneWeights[0], sizeof(vertex->boneWeights[0]), fd);
            pmxRead(&vertex->boneWeights[1], sizeof(vertex->boneWeights[1]), fd);
            pmxRead(&vertex->boneWeights[2], sizeof(vertex->boneWeights[2]), fd);
            pmxRead(&vertex->boneWeights[3], sizeof(vertex->boneWeights[3]), fd);
            break;
        default:
            break;
        }

        pmxRead(&vertex->edgeMag, sizeof(vertex->edgeMag), fd);
    }
}

void pmxReadFace(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->face.count), sizeof(pmx->face.count), fd);
    pmx->face.count /= 3; // Each face has 3 vertices
    
    pmx->face.data = (pmx_face_data_t *)malloc(sizeof(pmx_face_data_t) * pmx->face.count);
    switch (pmx->header.vertexIndexSize) {
        case 1: {
            //uint8_t indices[3 * pmx->face.count];
            //pmxRead(&indices[0], sizeof(indices), fd);
            uint8_t indices[100000] = { 0 };
            pmxRead(&indices[0], sizeof(uint8_t)* pmx->face.count * 3, fd);
            for (uint32_t i = 0; i < pmx->face.count; i++) {
                pmx_face_data_t* face = &(pmx->face.data[i]);
                face->indices[0] = (uint32_t)indices[i * 3];
                face->indices[1] = (uint32_t)indices[i * 3 + 1];
                face->indices[2] = (uint32_t)indices[i * 3 + 2];
            }
        }
        break;
        case 2: {
            //uint16_t indices[3 * pmx->face.count];
            //pmxRead(&indices[0], sizeof(indices), fd);            
            uint16_t indices[100000] = { 0 };
            pmxRead(&indices[0], sizeof(uint16_t) * pmx->face.count * 3, fd);
            for (uint32_t i = 0; i < pmx->face.count; i++) {
                pmx_face_data_t* face = &(pmx->face.data[i]);
                face->indices[0] = (uint32_t)indices[i * 3];
                face->indices[1] = (uint32_t)indices[i * 3 + 1];
                face->indices[2] = (uint32_t)indices[i * 3 + 2];
            }
        }
        break;
        case 4: {
            //uint32_t indices[3 * pmx->face.count];
            //pmxRead(&indices[0], sizeof(indices), fd);
            uint32_t indices[100000] = { 0 };
            pmxRead(&indices[0], sizeof(uint32_t) * pmx->face.count * 3, fd);
            for (uint32_t i = 0; i < pmx->face.count; i++) {
                pmx_face_data_t* face = &(pmx->face.data[i]);
                face->indices[0] = indices[i * 3];
                face->indices[1] = indices[i * 3 + 1];
                face->indices[2] = indices[i * 3 + 2];
            }
        }
        break;
        default:
            break;
    }

}

void pmxReadTexture(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->texture.Number), sizeof(pmx->texture.Number), fd);

    pmx->texture.path = (pmx_text_t *)malloc(sizeof(pmx_text_t) * pmx->texture.Number);
    for (uint32_t i = 0; i < pmx->texture.Number; i++) {
        pmxReadString(&pmx->texture.path[i], fd);    
        //wprintf(L"texture%d:%ls\n", i, pmx->texture.path[i].data_wide);

    }

}

void pmxReadMaterial(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->material.count), sizeof(pmx->material.count), fd);

    pmx->material.data = (pmx_material_data_t *)malloc(sizeof(pmx_material_data_t) * pmx->material.count);
    for (uint32_t index = 0; index < pmx->material.count; index++) {
      pmx_material_data_t* material = &pmx->material.data[index];
      pmxReadString(&material->localMaterialName, fd);
      pmxReadString(&material->generalMaterialName, fd);

      pmxRead(&material->diffuse, sizeof(material->diffuse), fd);
      pmxRead(&material->specular, sizeof(material->specular), fd);
      pmxRead(&material->specularPower, sizeof(material->specularPower), fd);

      pmxRead(&material->ambient, sizeof(material->ambient), fd);

      pmxRead(&material->drawMode, sizeof(material->drawMode), fd);

      pmxRead(&material->edgeColor, sizeof(material->edgeColor), fd);
      pmxRead(&material->edgeRatio, sizeof(material->edgeRatio), fd);

      pmxReadIndex(&material->textureIndex, pmx->header.textureIndexSize, fd);
      pmxReadIndex(&material->specularTextureIndex, pmx->header.textureIndexSize, fd);
      pmxRead(&material->blendMode, sizeof(material->blendMode), fd);

      pmxRead(&material->toonMode, sizeof(material->toonMode), fd);
      if (external == material->toonMode) {
        pmxReadIndex(&material->toonTextureIndex, pmx->header.textureIndexSize, fd);
      } else if (internal == material->toonMode) {
        uint8_t toonTextureIndex;
        pmxRead(&toonTextureIndex, sizeof(toonTextureIndex), fd);
        material->textureIndex = (int32_t)toonTextureIndex;
      }

      pmxReadString(&material->memo, fd);

      pmxRead(&material->numFace, sizeof(material->numFace), fd);
    }
}

void pmxReadBone(pmx_t* pmx, FILE* fd) {
  pmxRead(&(pmx->bone.count), sizeof(pmx->bone.count), fd);

  pmx->bone.data = (pmx_bone_data_t*)malloc(sizeof(pmx_bone_data_t) * pmx->bone.count);
  for (uint32_t index = 0; index < pmx->bone.count; index++) {
      pmx_bone_data_t* bone = &pmx->bone.data[index];
      
      pmxReadString(&bone->localBoonName, fd);
      pmxReadString(&bone->generalBoonName, fd);

      pmxRead(&bone->position, sizeof(bone->position), fd);

      pmxReadIndex(&bone->parentBoneIndex, pmx->header.boneIndexSize, fd);

      pmxRead(&bone->deformDepth, sizeof(bone->deformDepth), fd);

      pmxRead(&bone->boneflag, sizeof(bone->boneflag), fd);
        
      if (false == (bone->boneflag & TargetShowMode)){
        pmxRead(&bone->tailPosition, sizeof(bone->tailPosition), fd);
      } else {
        pmxReadIndex(&bone->tailBoneIndex, pmx->header.boneIndexSize, fd);
      }

      if ((bone->boneflag & AppendRotate) || 
          (bone->boneflag & AppendTranslate)) {
        pmxReadIndex(&bone->inheritBoneIndex, pmx->header.boneIndexSize, fd);
        pmxRead(&bone->inheritWeight, sizeof(bone->inheritWeight), fd);
      }

      if (bone->boneflag & FixedAxis) {
        pmxRead(&bone->fixedAxis, sizeof(bone->fixedAxis), fd);
      }

      if (bone->boneflag & LocalAxis) {
        pmxRead(&bone->localAxisX, sizeof(bone->localAxisX), fd);
        pmxRead(&bone->localAxisY, sizeof(bone->localAxisY), fd);
      }

      if (bone->boneflag & DeformOuterParent) {
        pmxRead(&bone->externalParentKey, sizeof(bone->externalParentKey), fd);
      }

      if (bone->boneflag & IK) {
        pmxReadIndex(&bone->ikTargetBoneIndex, pmx->header.boneIndexSize, fd);
        pmxRead(&bone->ikIterationCount, sizeof(bone->ikIterationCount), fd);
        pmxRead(&bone->ikLimitAngle, sizeof(bone->ikLimitAngle), fd);

        pmxRead(&bone->ikLinkCount, sizeof(bone->ikLinkCount), fd);

        bone->ikLinks = (pmx_lk_link_t*)malloc(sizeof(pmx_lk_link_t) * bone->ikLinkCount);
        for (uint32_t i = 0; i < bone->ikLinkCount; i++) {
            pmx_lk_link_t* link = &bone->ikLinks[i];
            pmxReadIndex(&link->boneIndex, pmx->header.boneIndexSize, fd);
            pmxRead(&link->angleLimit, sizeof(link->angleLimit), fd);

            if (0 != link->angleLimit) {
                pmxRead(&link->minLimit, sizeof(link->minLimit), fd);
                pmxRead(&link->maxLimit, sizeof(link->maxLimit), fd);
            }
        }
      }
    }
}

void pmxReadMorph(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->morph.count), sizeof(pmx->morph.count), fd);

    pmx->morph.data = (pmx_morph_data_t*)malloc(sizeof(pmx_morph_data_t) * pmx->morph.count);
    for (uint32_t index = 0; index < pmx->morph.count; index++) {
        pmx_morph_data_t* morph = &pmx->morph.data[index];

        pmxReadString(&morph->localMorphName, fd);
        pmxReadString(&morph->generalMorphName, fd);

        pmxRead(&morph->controlPanel, sizeof(morph->controlPanel), fd);
        pmxRead(&morph->morphType, sizeof(morph->morphType), fd);

        pmxRead(&morph->morphCount, sizeof(morph->morphCount), fd);
        if (Group == morph->morphType) {
            morph->groupMorphs = (pmx_group_morph_t*)malloc(sizeof(pmx_group_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_group_morph_t* groupMorph = &morph->groupMorphs[i];
                pmxReadIndex(&groupMorph->morphIndex, pmx->header.morphIndexSize, fd);
                pmxRead(&groupMorph->weight, sizeof(groupMorph->weight), fd);
            }
        } else if (Position == morph->morphType) {
            morph->positionMorphs = (pmx_vertex_morph_t*)malloc(sizeof(pmx_vertex_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_vertex_morph_t* vertexMorph = &morph->positionMorphs[i];
                pmxReadIndex(&vertexMorph->vertexIndex, pmx->header.vertexIndexSize, fd);
                pmxRead(&vertexMorph->position, sizeof(vertexMorph->position), fd);
            }
        } else if (Bone == morph->morphType) {
            morph->boneMorphs = (pmx_bone_morph_t*)malloc(sizeof(pmx_bone_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_bone_morph_t* boneMorph = &morph->boneMorphs[i];
                pmxReadIndex(&boneMorph->boneIndex, pmx->header.boneIndexSize, fd);
                pmxRead(&boneMorph->position, sizeof(boneMorph->position), fd);
                pmxRead(&boneMorph->quaternion, sizeof(boneMorph->quaternion), fd);
            }
        } else if (UV == morph->morphType || 
                   AddUV1 == morph->morphType || 
                   AddUV2 == morph->morphType || 
                   AddUV3 == morph->morphType || 
                   AddUV4 == morph->morphType) {
            morph->uvMorphs = (pmx_uv_morph_t*)malloc(sizeof(pmx_uv_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_uv_morph_t* uvMorph = &morph->uvMorphs[i];
                pmxReadIndex(&uvMorph->vertexIndex, pmx->header.vertexIndexSize, fd);
                pmxRead(&uvMorph->uv, sizeof(uvMorph->uv), fd);
            }
        } else if (Material == morph->morphType) {
            morph->materialMorphs = (pmx_material_morph_t*)malloc(sizeof(pmx_material_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_material_morph_t* materialMorph = &morph->materialMorphs[i];
                pmxReadIndex(&materialMorph->materialIndex, pmx->header.materialIndexSize, fd);
                pmxRead(&materialMorph->opType, sizeof(materialMorph->opType), fd);
                pmxRead(&materialMorph->diffuse, sizeof(materialMorph->diffuse), fd);
                pmxRead(&materialMorph->specular, sizeof(materialMorph->specular), fd);
                pmxRead(&materialMorph->specularPower, sizeof(materialMorph->specularPower), fd);
                pmxRead(&materialMorph->ambient, sizeof(materialMorph->ambient), fd);
                pmxRead(&materialMorph->edgeColor, sizeof(materialMorph->edgeColor), fd);
                pmxRead(&materialMorph->edgeSize, sizeof(materialMorph->edgeSize), fd);
                pmxRead(&materialMorph->textureFactor, sizeof(materialMorph->textureFactor), fd);
                pmxRead(&materialMorph->sphereTextureFactor, sizeof(materialMorph->sphereTextureFactor), fd);
                pmxRead(&materialMorph->toonTextureFactor, sizeof(materialMorph->toonTextureFactor), fd);
            }

        } else if (Flip == morph->morphType) {
            morph->flipMorphs = (pmx_flip_morph_t*)malloc(sizeof(pmx_flip_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_flip_morph_t* flipMorph = &morph->flipMorphs[i];
                pmxReadIndex(&flipMorph->morphIndex, pmx->header.morphIndexSize, fd);
                pmxRead(&flipMorph->m_weight, sizeof(flipMorph->m_weight), fd);
            }
        } else if (Impluse == morph->morphType) {
            morph->impulseMorphs = (pmx_impulse_morph_t*)malloc(sizeof(pmx_impulse_morph_t) * morph->morphCount);
            for (uint32_t i = 0; i < morph->morphCount; i++) {
                pmx_impulse_morph_t* impulseMorph = &morph->impulseMorphs[i];
                pmxReadIndex(&impulseMorph->rigidbodyIndex, pmx->header.rigidbodyIndexSize, fd);
                pmxRead(&impulseMorph->localFlag, sizeof(impulseMorph->localFlag), fd);
                pmxRead(&impulseMorph->translateVelocity, sizeof(impulseMorph->translateVelocity), fd);
                pmxRead(&impulseMorph->rotateTorque, sizeof(impulseMorph->rotateTorque), fd);
            }
        }
    }
}

void pmxReadDisplayFrame(pmx_t* pmx, FILE* fd) {
    pmxRead(&(pmx->displayFrame.count), sizeof(pmx->displayFrame.count), fd);

    pmx->displayFrame.data = (pmx_displayFrame_data_t*)malloc(sizeof(pmx_displayFrame_data_t) * pmx->displayFrame.count);
    for (uint32_t index = 0; index < pmx->displayFrame.count; index++) {
        pmx_displayFrame_data_t* frame = &pmx->displayFrame.data[index];

        pmxReadString(&frame->localFrameName, fd);
        pmxReadString(&frame->generalFrameName, fd);

        pmxRead(&frame->frameType, sizeof(frame->frameType), fd);

        pmxRead(&frame->frameCount, sizeof(frame->frameCount), fd);
        frame->targets = (pmx_frame_target_t*)malloc(sizeof(pmx_frame_target_t) * frame->frameCount);
        for (uint32_t i = 0; i < frame->frameCount; i++) {
            pmx_frame_target_t* target = &frame->targets[i];
            pmxRead(&target->targetType, sizeof(target->targetType), fd);
            if (BoneIndex == target->targetType) {
                pmxReadIndex(&target->targetIndex, pmx->header.boneIndexSize, fd);
            } else if (MorphIndex == target->targetType) {
                pmxReadIndex(&target->targetIndex, pmx->header.morphIndexSize, fd);
            }
        }
    }
}

pmx_t pmx;
void read_file(char *filename)
{
    setlocale(LC_ALL, "");// suggested print to windows terminal

    FILE *fd = fopen(filename,"rb"); 
    if(fd == NULL)
    {
        perror("open failed!");
        exit(1);        //error, exit the program
    }

    pmxReadHeader(&pmx, fd);
    pmxReadVertex(&pmx, fd);
    pmxReadFace(&pmx, fd);
    pmxReadTexture(&pmx, fd);
    pmxReadMaterial(&pmx, fd);
    pmxReadBone(&pmx, fd);

    pmxReadMorph(&pmx, fd);
    pmxReadDisplayFrame(&pmx, fd);
    fread(&buffer[0],sizeof(buffer),1,fd);
    fclose(fd);
}

int main()
{
  read_file("C:\\Users\\dong\\Downloads\\xiao\\xiao.pmx");

  return 0;
}