#ifndef _MATH_H
#define _MATH_H

typedef struct {
    union {
        struct {
            float x, y;
        };    
        float M[2];
    };
} VECTOR2D_T, POINT2D_T, *VECTOR2D_PTR, *POINT2D_PTR;

typedef struct {
    union {
        struct {
            float x, y, z;
        };
        float M[3];
    };
} VECTOR3D_T, POINT3D_T, *VECTOR3D_PTR, *POINT3D_PTR;

typedef struct {
    union {
        struct {
            float x, y, z, w;
        };
        float M[4];
    };
} VECTOR4D_T, POINT4D_T, *VECTOR4D_PTR, *POINT4D_PTR;

typedef struct {
    int x, y;
} VERTEX2D_T, *VERTEX2D_PTU;

typedef struct {
    float x, y;
} VERTEX2DF_T, *VERTEX2DF_PTR;

typedef struct {
    POINT2D_T p0, p1;
    VECTOR2D_T v; // v = P1 - P0
} PARAMLINE2D_T, *PARAMLINE2D_PTR;

typedef struct {
    POINT3D_T p0, p1;
    VECTOR3D_T v; // v = P1 - P0
} PARAMLINE3D_T, *PARAMLINE3D_PTR;

typedef struct {
    POINT3D_T p0, p1, p2;
    VECTOR3D_T n;
} PLANE3D_T, *PLANE3D_PTR;

typedef struct {
    union {
        struct {
            float M00, M01, M02, M03;
            float M10, M11, M12, M13;
            float M20, M21, M22, M23;
            float M30, M31, M32, M33;
        };
        float M[4][4];
    };
    
} MATRIX4X4_T, *MATRIX4X4_PTR;

typedef struct {
    union {
        struct {
            float M00, M01, M02;
            float M10, M11, M12;
            float M20, M21, M22;
        };
        float M[3][3];
    };
} MATRIX3X3_T, *MATRIX3X3_PTR;

typedef struct {
    union {
        struct {
            float M00, M01, M02, M03;
        };
        float M[4];
    };
    
} MATRIX1X4_T, *MATRIX1X4_PTR;

typedef struct {
    union {
        struct {
            float M00, M01, M02;
        };
        float M[3];
    };
} MATRIX1X3_T, *MATRIX1X3_PTR;

typedef struct {
    union {
        struct {
            float w, x, y, z;
        };
        struct {
            float q0;
            VECTOR3D_T qv;
        };
        float M[4];
    };
} QUAT_T, *QUAT_PTR;

typedef struct {
    float r, theta; // r = radius, theta = angle in radians
} POLAR2D_T, *POLAR2D_PTR;

typedef struct {
    float p, theta, phi; 
} SPHERICAL3D_T, *SPHERICAL3D_PTR;

#define PI ((float)3.141592654f)

const MATRIX4X4_T IMAT_4X4 = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

const MATRIX3X3_T IMAT_3X3 = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};

#define EPSILON_E3 (float)(1E-3) 
#define EPSILON_E4 (float)(1E-4) 
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)

#define PARM_LINE_NO_INTERSECT          0
#define PARM_LINE_INTERSECT_IN_SEGMENT  1
#define PARM_LINE_INTERSECT_OUT_SEGMENT 2
#define PARM_LINE_INTERSECT_EVERYWHERE  3

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define SWAP(a, b, t) {t = a; a = b; b = t;}
#define DEG_TO_RAD(ang) ((ang) * PI / 180.0f)
#define RAD_TO_DEG(rad) ((rad) * 180.0f / PI)
#define RAND_RANGE(x, y) ((x) + (rand() % ((y) - (x) + 1)))

#define MAT_ZERO_4X4(m) {memset((void*)(m), 0, sizeof(MATRIX4X4_T));}
#define MAT_ZERO_3X3(m) {memset((void*)(m), 0, sizeof(MATRIX3X3_T));}
#define MAT_IDENTITY_4X4(m) {memcpy((void*)(m), (void*)&IMAT_4X4, sizeof(MATRIX4X4_T));}
#define MAT_IDENTITY_3X3(m) {memcpy((void*)(m), (void*)&IMAT_3X3, sizeof(MATRIX3X3_T));}
#define MAT_COPY_4X4(mdest, msrc) {memcpy((void*)(mdest), (void*)(msrc), sizeof(MATRIX4X4_T));}
#define MAT_COPY_3X3(mdest, msrc) {memcpy((void*)(mdest), (void*)(msrc), sizeof(MATRIX3X3_T));}

void Build_Sin_Cos_Tables(void);
float Fast_Sin(float theta);
float Fast_Cos(float theta);

void POLAR2D_To_POINT2D(POLAR2D_PTR polar, POINT2D_PTR rect);
void POLAR2D_To_RectXY(POLAR2D_PTR polar, float *x, float *y);
void POINT2D_To_POLAR2D(POINT2D_PTR rect, POLAR2D_PTR polar);
void POINT2D_To_PolarRTh(POINT2D_PTR rect, float *r, float *theta);
void SPHERICAL3D_To_POINT3D(SPHERICAL3D_PTR sph, POINT3D_PTR rect);
void SPHERICAL3D_To_RectXYZ(SPHERICAL3D_PTR sph, float *x, float *y, float *z);
void POINT3D_To_SPHERICAL3D(POINT3D_PTR rect, SPHERICAL3D_PTR sph);
void POINT3D_To_SphericalPThPh(POINT3D_PTR rect, float *p, float *theta, float *phi);

VECTOR2D_T VECTOR2D_Add(VECTOR2D_PTR va, VECTOR2D_PTR vb);
VECTOR2D_T VECTOR2D_Sub(VECTOR2D_PTR va, VECTOR2D_PTR vb);
VECTOR2D_T VECTOR2D_Scale(float k, VECTOR2D_PTR va);
float VECTOR2D_Dot(VECTOR2D_PTR va, VECTOR2D_PTR vb);
float VECTOR2D_Length(VECTOR2D_PTR va);
float VECTOR2D_Length_Fast(VECTOR2D_PTR va);
void VECTOR2D_Normalize(VECTOR2D_PTR va, VECTOR2D_PTR vn);
void VECTOR2D_Build(VECTOR2D_PTR init, VECTOR2D_PTR term, VECTOR2D_PTR result);
float VECTOR2D_CosTh(VECTOR2D_PTR va, VECTOR2D_PTR vb);

VECTOR3D_T VECTOR3D_Add(VECTOR3D_PTR va, VECTOR3D_PTR vb);
VECTOR3D_T VECTOR3D_Sub(VECTOR3D_PTR va, VECTOR3D_PTR vb);
VECTOR3D_T VECTOR3D_Scale(float k, VECTOR3D_PTR va);
float VECTOR3D_Dot(VECTOR3D_PTR va, VECTOR3D_PTR vb);
VECTOR3D_T VECTOR3D_Cross(VECTOR3D_PTR va, VECTOR3D_PTR vb);
float VECTOR3D_Length(VECTOR3D_PTR va);
float VECTOR3D_Length_Fast(VECTOR3D_PTR va);
void VECTOR3D_Normalize(VECTOR3D_PTR va, VECTOR3D_PTR vn);
void VECTOR3D_Build(VECTOR3D_PTR init, VECTOR3D_PTR term, VECTOR3D_PTR result);
float VECTOR3D_CosTh(VECTOR3D_PTR va, VECTOR3D_PTR vb);

VECTOR4D_T VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb);
VECTOR4D_T VECTOR4D_Sub(VECTOR4D_PTR va, VECTOR4D_PTR vb);
VECTOR4D_T VECTOR4D_Scale(float k, VECTOR4D_PTR va);
float VECTOR4D_Dot(VECTOR4D_PTR va, VECTOR4D_PTR vb);
VECTOR4D_T VECTOR4D_Cross(VECTOR4D_PTR va, VECTOR4D_PTR vb);
float VECTOR4D_Length(VECTOR4D_PTR va);
float VECTOR4D_Length_Fast(VECTOR4D_PTR va);
void VECTOR4D_Normalize(VECTOR4D_PTR va, VECTOR4D_PTR vn);
void VECTOR4D_Build(VECTOR4D_PTR init, VECTOR4D_PTR term, VECTOR4D_PTR result);
float VECTOR4D_CosTh(VECTOR4D_PTR va, VECTOR4D_PTR vb);

void Mat_Add_3X3(MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR msum);
void Mat_Mul_VECTOR3D_3X3(VECTOR3D_PTR va, MATRIX3X3_PTR mb,VECTOR3D_PTR vprod);
int Mat_Inverse_3X3(MATRIX3X3_PTR m, MATRIX3X3_PTR mi);
void Mat_Init_3X3(MATRIX3X3_PTR ma, float m00, float m01, float m02,
                                   float m10, float m11, float m12,
                                   float m20, float m21, float m22);
float Mat_Det_3X3(MATRIX3X3_PTR m);
int Mat_Mul_3X3(MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR mprod);

void Mat_Add_4X4(MATRIX4X4_PTR ma, MATRIX4X4_PTR mb, MATRIX4X4_PTR msum);
void Mat_Mul_4X4(MATRIX4X4_PTR ma, MATRIX4X4_PTR mb, MATRIX4X4_PTR mprod);
void Mat_Mul_1X4_4X4(MATRIX1X4_PTR ma, MATRIX4X4_PTR mb, MATRIX1X4_PTR mprod);
void Mat_Mul_VECTOR3D_4X4(VECTOR3D_PTR  va, MATRIX4X4_PTR mb, VECTOR3D_PTR vprod);
void Mat_Mul_VECTOR4D_4X4(VECTOR4D_PTR  va, MATRIX4X4_PTR mb, VECTOR4D_PTR vprod);
int Mat_Inverse_4X4(MATRIX4X4_PTR m, MATRIX4X4_PTR mi);
void Mat_Init_4X4(MATRIX4X4_PTR ma, float m00, float m01, float m02, float m03,
                                   float m10, float m11, float m12, float m13,
                                   float m20, float m21, float m22, float m23,
                                   float m30, float m31, float m32, float m33);

void Init_Parm_Line2D(POINT2D_PTR p_init, POINT2D_PTR p_term, PARAMLINE2D_PTR p);
void Compute_Parm_Line2D(PARAMLINE2D_PTR p, float t, POINT2D_PTR pt);
int Intersect_Parm_Lines2D(PARAMLINE2D_PTR p1, PARAMLINE2D_PTR p2, POINT2D_PTR pt);

void Init_Parm_Line3D(POINT3D_PTR p_init, POINT3D_PTR p_term, PARAMLINE3D_PTR p);
void Compute_Parm_Line3D(PARAMLINE3D_PTR p, float t, POINT3D_PTR pt);

void PLANE3D_Init(PLANE3D_PTR plane, POINT3D_PTR p0, 
                         VECTOR3D_PTR normal, int normalize);
float Compute_Point_In_Plane3D(POINT3D_PTR pt, PLANE3D_PTR plane);
int Intersect_Parm_Line3D_Plane3D(PARAMLINE3D_PTR pline, PLANE3D_PTR plane, 
                                         float *t, POINT3D_PTR pt);

void QUAT_Add(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qsum);
void QUAT_Sub(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qdiff);
void QUAT_Conjugate(QUAT_PTR q, QUAT_PTR qconj);
void QUAT_Scale(QUAT_PTR q, float scale, QUAT_PTR qs);
float QUAT_Norm(QUAT_PTR q);
float QUAT_Norm2(QUAT_PTR q);
void QUAT_Normalize(QUAT_PTR q, QUAT_PTR qn);
void QUAT_Inverse(QUAT_PTR q, QUAT_PTR qi);
void QUAT_Mul(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qprod);
void QUAT_Triple_Product(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR q3, QUAT_PTR qprod);
void VECTOR3D_Theta_To_QUAT(QUAT_PTR q, VECTOR3D_PTR v, float theta);
void VECTOR4D_Theta_To_QUAT(QUAT_PTR q, VECTOR4D_PTR v, float theta);
void EulerZYX_To_QUAT(QUAT_PTR q, float theta_z, float theta_y, float theta_x);
void QUAT_To_VECTOR3D_Theta(QUAT_PTR q, VECTOR3D_PTR v, float *theta);

#endif