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
    pmx_bone_flags_t flags[2]; // Bone flags

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
    uint8_t ikLinkCount; // Number of IK links
    pmx_lk_link_t* ikLinks; // Pointer to IK links

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
}pmx_vertex_morph_t;

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
    Mul,
    Add,
}pmx_op_type_t;
typedef struct  {

    int32_t		materialIndex;
    pmx_op_type_t	opType;	
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
    pmx_morph_type_t morphType; // Morph type

    pmx_vertex_morph_t* positionMorphs; // Position morphs
    pmx_uv_morph_t* uvMorphs; // UV morphs
    pmx_bone_morph_t* boneMorphs; // Bone morphs
    pmx_material_morph_t* materialMorphs; // Material morphs
    pmx_group_morph_t* groupMorphs; // Group morphs
    pmx_flip_morph_t* flipMorphs; // Flip morphs
    pmx_impulse_morph_t* impulseMorphs; // Impulse morphs

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
    pmx_target_type_t targetType; // Target type: BoneIndex or MorphIndex
    int32_t targetIndex; // Index of the target (bone or morph)
} pmx_frame_target_t;

typedef struct {

    pmx_text_t localFrameName; // Frame name
    pmx_text_t generalFrameName; // English frame name

    pmx_frame_type_t frameType; // Frame type

    int32_t frameCount; // Number of targets in the frame
    pmx_frame_target_t* targets; // Pointer to target data

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