#include "math.h"

inline void VECTOR2D_ZERO(VECTOR2D_PTR v) { v->x = 0.0f; v->y = 0.0f;}
inline void VECTOR3D_ZERO(VECTOR3D_PTR v) { v->x = 0.0f; v->y = 0.0f; v->z = 0.0f;}
inline void VECTOR4D_ZERO(VECTOR4D_PTR v) { v->x = 0.0f; v->y = 0.0f; v->z = 0.0f; v->w = 1.0f;}

inline void VECTOR2D_INITXY(VECTOR2D_PTR v, float x, float y) { v->x = x; v->y = y;}
inline void VECTOR3D_INITXYZ(VECTOR3D_PTR v, float x, float y, float z) { v->x = x; v->y = y; v->z = z;}
inline void VECTOR4D_INITXYZW(VECTOR4D_PTR v, float x, float y, float z) { v->x = x; v->y = y; v->z = z; v->w = 1.0f;}

inline void VECTOR2D_INIT(VECTOR2D_PTR vdest, VECTOR2D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y;}
inline void VECTOR3D_INIT(VECTOR3D_PTR vdest, VECTOR3D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y; vdest->z = vsrc->z;}
inline void VECTOR4D_INIT(VECTOR4D_PTR vdest, VECTOR4D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y; 
     vdest->z = vsrc->z; vdest->w = vsrc->w;}

inline void VECTOR2D_COPY(VECTOR2D_PTR vdest, VECTOR2D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y;}
inline void VECTOR3D_COPY(VECTOR3D_PTR vdest, VECTOR3D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y; vdest->z = vsrc->z;}
inline void VECTOR4D_COPY(VECTOR4D_PTR vdest, VECTOR4D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y; 
     vdest->z = vsrc->z; vdest->w = vsrc->w;}       
     
inline void POINT2D_INIT(POINT2D_PTR vdest, POINT2D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y;}
inline void POINT3D_INIT(POINT3D_PTR vdest, POINT3D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y; vdest->z = vsrc->z;}
inline void POINT4D_INIT(POINT4D_PTR vdest, POINT4D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y; 
  vdest->z = vsrc->z; vdest->w = vsrc->w;}

inline void POINT2D_COPY(POINT2D_PTR vdest, POINT2D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y;}
inline void POINT3D_COPY(POINT3D_PTR vdest, POINT3D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y; vdest->z = vsrc->z;}
inline void POINT4D_COPY(POINT4D_PTR vdest, POINT4D_PTR vsrc) { vdest->x = vsrc->x; vdest->y = vsrc->y; 
  vdest->z = vsrc->z; vdest->w = vsrc->w;}    
  
inline void MAT_TRANSPOSE_3X3(MATRIX3X3_PTR m) {
    MATRIX3X3_T mt;
    mt.M00 = m->M00; mt.M01 = m->M10; mt.M02 = m->M20;
    mt.M10 = m->M01; mt.M11 = m->M11; mt.M12 = m->M21;
    mt.M20 = m->M02; mt.M21 = m->M12; mt.M22 = m->M22;
    memcpy((void*)m, (void*)&mt, sizeof(MATRIX3X3_T));
}
inline void MAT_TRANSPOSE_4X4(MATRIX4X4_PTR m) {
    MATRIX4X4_T mt;
    mt.M00 = m->M00; mt.M01 = m->M10; mt.M02 = m->M20; mt.M03 = m->M30;
    mt.M10 = m->M01; mt.M11 = m->M11; mt.M12 = m->M21; mt.M13 = m->M31;
    mt.M20 = m->M02; mt.M21 = m->M12; mt.M22 = m->M22; mt.M23 = m->M32;
    mt.M30 = m->M03; mt.M31 = m->M13; mt.M32 = m->M23; mt.M33 = m->M33;
    memcpy((void*)m, (void*)&mt, sizeof(MATRIX4X4_T));
}

inline void MAT_COLUMN_SWAP_4x4(MATRIX4X4_PTR m, int c, MATRIX1X4_PTR v) {
    m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1];
    m->M[2][c] = v->M[2]; m->M[3][c] = v->M[3];
}

inline void MAT_COLUMN_SWAP_3x3(MATRIX3X3_PTR m, int c, MATRIX1X3_PTR v) 
{ m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2];}

inline void QUAT_ZERO(QUAT_PTR q) { q->qv.x = q->qv.y =  q->qv.z = q->z = 0.0f;}
inline void QUAT_INITXYZW(QUAT_PTR q, float x, float y, float z, float w) { q->x = x; q->y = y; q->z = z; q->w = w;}
inline void QUAT_INIT_VECTOR3D(QUAT_PTR q, VECTOR3D_PTR v) { q->w = 0; q->x = v->x; q->y = v->y; q->z = v->z;}
inline void QUAT_INIT(QUAT_PTR qdest, QUAT_PTR qsrc) { qdest->x = qsrc->x; qdest->y = qsrc->y; 
             qdest->z = qsrc->z; qdest->w = qsrc->w; }
inline void QUAT_COPY(QUAT_PTR qdest, QUAT_PTR qsrc) { qdest->x = qsrc->x; qdest->y = qsrc->y; 
             qdest->z = qsrc->z; qdest->w = qsrc->w; }

float cos_look[361]; // 1 extra element so we can store 0-360 inclusive
float sin_look[361]; // 1 extra element so we can store 0-360 inclusive

void Build_Sin_Cos_Tables(void) {
    for (int ang = 0; ang <= 360; ang++) {
        float theta = (float)ang*PI/(float)180;
        cos_look[ang] = cos(theta);
        sin_look[ang] = sin(theta);
    } 
}

float Fast_Sin(float theta) {

    theta = fmodf(theta,360);

    if (theta < 0) theta+=360.0;

    int theta_int    = (int)theta;
    float theta_frac = theta - theta_int;

    return(sin_look[theta_int] + 
        theta_frac*(sin_look[theta_int+1] - sin_look[theta_int]));

}

float Fast_Cos(float theta) {

    theta = fmodf(theta,360);

    if (theta < 0) theta+=360.0;

    int theta_int    = (int)theta;
    float theta_frac = theta - theta_int;

    return(cos_look[theta_int] + 
        theta_frac*(cos_look[theta_int+1] - cos_look[theta_int]));

} 

void POLAR2D_To_POINT2D(POLAR2D_PTR polar, POINT2D_PTR rect) {
    rect->x = polar->r*cosf(polar->theta);
    rect->y = polar->r*sinf(polar->theta);
}
void POLAR2D_To_RectXY(POLAR2D_PTR polar, float *x, float *y) {
    *x = polar->r*cosf(polar->theta);
    *y = polar->r*sinf(polar->theta);
}
void POINT2D_To_POLAR2D(POINT2D_PTR rect, POLAR2D_PTR polar) {
    polar->r     = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
    polar->theta = atanf(rect->y/rect->x);
}
void POINT2D_To_PolarRTh(POINT2D_PTR rect, float *r, float *theta){
    *r=sqrtf((rect->x * rect->x) + (rect->y * rect->y));
    *theta = atanf(rect->y/rect->x);
}
void SPHERICAL3D_To_POINT3D(SPHERICAL3D_PTR sph, POINT3D_PTR rect){
    
    rect->z = sph->p*cosf(sph->phi);

    float r = sph->p*sinf(sph->phi);
    rect->x = r*cosf(sph->theta);
    rect->y = r*sinf(sph->theta);
}
void SPHERICAL3D_To_RectXYZ(SPHERICAL3D_PTR sph, float *x, float *y, float *z){
    *z = sph->p*cosf(sph->phi);

    float r = sph->p*sinf(sph->phi);
    *x = r*cosf(sph->theta);
    *y = r*sinf(sph->theta);
}
void POINT3D_To_SPHERICAL3D(POINT3D_PTR rect, SPHERICAL3D_PTR sph){
    sph->p     = sqrtf((rect->x*rect->x)+(rect->y*rect->y)+(rect->z*rect->z));
    sph->theta = atanf(rect->y/rect->x);
    
    float r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
    sph->phi    = asinf(r / (sph->p));
}
void POINT3D_To_SphericalPThPh(POINT3D_PTR rect, float *p, float *theta, float *phi){

    *p     = sqrtf((rect->x*rect->x)+(rect->y*rect->y)+(rect->z*rect->z));
    *theta = atanf(rect->y/rect->x);

    float r = sqrtf((rect->x * rect->x) + (rect->y * rect->y));
    *phi    = asinf(r / (*p));
}


VECTOR2D_T VECTOR2D_Add(VECTOR2D_PTR va, VECTOR2D_PTR vb) {
    VECTOR2D_T vsum;
    vsum.x = va->x + vb->x;
    vsum.y = va->y + vb->y;
    return(vsum);
} 

VECTOR2D_T VECTOR2D_Sub(VECTOR2D_PTR va, VECTOR2D_PTR vb) {
    VECTOR2D_T vdiff;
    vdiff.x = va->x - vb->x;
    vdiff.y = va->y - vb->y;
    return(vdiff);  
} 

VECTOR2D_T VECTOR2D_Scale(float k, VECTOR2D_PTR va) {
    VECTOR2D_T vscaled;
    vscaled.x = k*va->x;
    vscaled.y = k*va->y;
    return(vscaled);
} 

float VECTOR2D_Dot(VECTOR2D_PTR va, VECTOR2D_PTR vb) {
    return( (va->x * vb->x) + (va->y * vb->y) );
} 

float VECTOR2D_Length(VECTOR2D_PTR va) {
    return(sqrtf(va->x*va->x + va->y*va->y));
} 

static int Fast_Distance_2D(int x, int y) {
    x = abs(x);
    y = abs(y);

    int mn = MIN(x,y);

    return(x+y-(mn>>1)-(mn>>2)+(mn>>4));
} 

float VECTOR2D_Length_Fast(VECTOR2D_PTR va) {
    return( (float)Fast_Distance_2D(va->x, va->y) );
}

void VECTOR2D_Normalize(VECTOR2D_PTR va, VECTOR2D_PTR vn) {
    VECTOR2D_ZERO(vn);
    float length = (float)sqrtf(va->x*va->x + va->y*va->y );

    if (length < EPSILON_E5) 
    return;

    float length_inv = 1/length;

    vn->x = va->x*length_inv;
    vn->y = va->y*length_inv;
} 

void VECTOR2D_Build(VECTOR2D_PTR init, VECTOR2D_PTR term, VECTOR2D_PTR result) {

    result->x = term->x - init->x;
    result->y = term->y - init->y;
} 

float VECTOR2D_CosTh(VECTOR2D_PTR va, VECTOR2D_PTR vb){
    return(VECTOR2D_Dot(va,vb)/(VECTOR2D_Length(va)*VECTOR2D_Length(vb)));
}

VECTOR3D_T VECTOR3D_Add(VECTOR3D_PTR va, VECTOR3D_PTR vb) {
    VECTOR3D_T vsum;

    vsum.x = va->x + vb->x;
    vsum.y = va->y + vb->y;
    vsum.z = va->z + vb->z;
    return(vsum);
} 

VECTOR3D_T VECTOR3D_Sub(VECTOR3D_PTR va, VECTOR3D_PTR vb) {
    VECTOR3D_T vdiff;

    vdiff.x = va->x - vb->x;
    vdiff.y = va->y - vb->y;
    vdiff.z = va->z - vb->z;

    return(vdiff);
} 

VECTOR3D_T VECTOR3D_Scale(float k, VECTOR3D_PTR va) {
    VECTOR3D_T vscaled;
    vscaled.x = k*va->x;
    vscaled.y = k*va->y;
    vscaled.z = k*va->z;
    return(vscaled);
} 

float VECTOR3D_Dot(VECTOR3D_PTR va, VECTOR3D_PTR vb) {
    return( (va->x * vb->x) + (va->y * vb->y) + (va->z * vb->z) );
}

VECTOR3D_T VECTOR3D_Cross(VECTOR3D_PTR va, VECTOR3D_PTR vb) {
    VECTOR3D_T vn;

    vn.x =  ( (va->y * vb->z) - (va->z * vb->y) );
    vn.y = -( (va->x * vb->z) - (va->z * vb->x) );
    vn.z =  ( (va->x * vb->y) - (va->y * vb->x) ); 

    return(vn);
} 

float VECTOR3D_Length(VECTOR3D_PTR va) {
    return( (float)sqrtf(va->x*va->x + va->y*va->y + va->z*va->z) );
} 

static float Fast_Distance_3D(float fx, float fy, float fz) {

    int temp; 
    int x,y,z; 

    x = fabs(fx) * 1024;
    y = fabs(fy) * 1024;
    z = fabs(fz) * 1024;

    if (y < x) SWAP(x,y,temp)

    if (z < y) SWAP(y,z,temp)

    if (y < x) SWAP(x,y,temp)

    int dist = (z + 11 * (y >> 5) + (x >> 2) );

    return((float)(dist >> 10));
} 

float VECTOR3D_Length_Fast(VECTOR3D_PTR va) {
    return( Fast_Distance_3D(va->x, va->y, va->z) );
} 

void VECTOR3D_Normalize(VECTOR3D_PTR va, VECTOR3D_PTR vn) {

    VECTOR3D_ZERO(vn);
    float length = VECTOR3D_Length(va);

    if (length < EPSILON_E5) 
    return;

    float length_inv = 1.0/length;

    vn->x = va->x*length_inv;
    vn->y = va->y*length_inv;
    vn->z = va->z*length_inv;

}

void VECTOR3D_Build(VECTOR3D_PTR init,VECTOR3D_PTR term,VECTOR3D_PTR result) {
    result->x = term->x - init->x;
    result->y = term->y - init->y;
    result->z = term->z - init->z;
} 

float VECTOR3D_CosTh(VECTOR3D_PTR va, VECTOR3D_PTR vb) {
    return(VECTOR3D_Dot(va,vb)/(VECTOR3D_Length(va)*VECTOR3D_Length(vb)));
}


void VECTOR4D_Build(VECTOR4D_PTR init, VECTOR4D_PTR term, VECTOR4D_PTR result) {
    result->x = term->x - init->x;
    result->y = term->y - init->y;
    result->z = term->z - init->z;
    result->w = 1;
} 


VECTOR4D_T VECTOR4D_Add(VECTOR4D_PTR va, VECTOR4D_PTR vb) {
    VECTOR4D_T vsum;
    vsum.x = va->x + vb->x;
    vsum.y = va->y + vb->y;
    vsum.z = va->z + vb->z;
    vsum.w = 1;
    return(vsum);
}

VECTOR4D_T VECTOR4D_Sub(VECTOR4D_PTR va, VECTOR4D_PTR vb) {
    VECTOR4D_T vdiff;
    vdiff.x = va->x - vb->x;
    vdiff.y = va->y - vb->y;
    vdiff.z = va->z - vb->z;
    vdiff.w = 1;
    return(vdiff);  
} 

VECTOR4D_T VECTOR4D_Scale(float k, VECTOR4D_PTR va) {

    VECTOR4D_T vscaled;
    vscaled.x = k*va->x;
    vscaled.y = k*va->y;
    vscaled.z = k*va->z;
    vscaled.w = 1;
} 

float VECTOR4D_Dot(VECTOR4D_PTR va, VECTOR4D_PTR vb) {
    return( (va->x * vb->x) + (va->y * vb->y) + (va->z * vb->z) );
}

VECTOR4D_T VECTOR4D_Cross(VECTOR4D_PTR va, VECTOR4D_PTR vb) {
    VECTOR4D_T vn;

    vn.x =  ( (va->y * vb->z) - (va->z * vb->y) );
    vn.y = -( (va->x * vb->z) - (va->z * vb->x) );
    vn.z =  ( (va->x * vb->y) - (va->y * vb->x) ); 
    vn.w = 1;
    return(vn);
} 

float VECTOR4D_Length(VECTOR4D_PTR va) {
    return(sqrtf(va->x*va->x + va->y*va->y + va->z*va->z) );
} 

float VECTOR4D_Length_Fast(VECTOR4D_PTR va) {
    return( Fast_Distance_3D(va->x, va->y, va->z) );
}

void VECTOR4D_Normalize(VECTOR4D_PTR va, VECTOR4D_PTR vn) {

    VECTOR4D_ZERO(vn);

    float length = sqrt(va->x*va->x + va->y*va->y + va->z*va->z);

    if (length < EPSILON_E5) 
    return;

    float length_inv = 1.0/length;

    vn->x = va->x*length_inv;
    vn->y = va->y*length_inv;
    vn->z = va->z*length_inv;
    vn->w = 1;
}

float VECTOR4D_CosTh(VECTOR4D_PTR va, VECTOR4D_PTR vb) {
    return(VECTOR4D_Dot(va,vb)/(VECTOR4D_Length(va)*VECTOR4D_Length(vb)));
}

void Mat_Add_3X3(MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR msum) {
    for (int row=0; row<3; row++) {
        for (int col=0; col<3; col++) {
           msum->M[row][col] = ma->M[row][col] + mb->M[row][col];
        } 
    }
}
void Mat_Mul_VECTOR3D_3X3(VECTOR3D_PTR va, MATRIX3X3_PTR mb,VECTOR3D_PTR vprod) {
        for (int col=0; col < 3; col++) {
            float sum = 0;
            for (int row=0; row<3; row++) {
                sum+=(va->M[row]*mb->M[row][col]);
             } 
            vprod->M[col] = sum;
        } 
}
int Mat_Inverse_3X3(MATRIX3X3_PTR m, MATRIX3X3_PTR mi) {
    float det = m->M00*(m->M11*m->M22 - m->M21*m->M12) - 
            m->M01*(m->M10*m->M22 - m->M20*m->M12) + 
            m->M02*(m->M10*m->M21 - m->M20*m->M11);

    if (fabs(det) < EPSILON_E5)
    return(0);

    float det_inv = 1.0/det;

    mi->M00 =  det_inv*(m->M11*m->M22 - m->M21*m->M12);
    mi->M10 = -det_inv*(m->M10*m->M22 - m->M20*m->M12);
    mi->M20 =  det_inv*(m->M10*m->M21 - m->M20*m->M11);

    mi->M01 = -det_inv*(m->M01*m->M22 - m->M21*m->M02);
    mi->M11 =  det_inv*(m->M00*m->M22 - m->M20*m->M02);
    mi->M21 = -det_inv*(m->M00*m->M21 - m->M20*m->M01);

    mi->M02 =  det_inv*(m->M01*m->M12 - m->M11*m->M02);
    mi->M12 = -det_inv*(m->M00*m->M12 - m->M10*m->M02);
    mi->M22 =  det_inv*(m->M00*m->M11 - m->M10*m->M01);

    return(1);
}
void Mat_Init_3X3(MATRIX3X3_PTR ma, float m00, float m01, float m02,
                                   float m10, float m11, float m12,
                                   float m20, float m21, float m22) {
    ma->M00 = m00; ma->M01 = m01; ma->M02 = m02;
    ma->M10 = m10; ma->M11 = m11; ma->M12 = m12;
    ma->M20 = m20; ma->M21 = m21; ma->M22 = m22;
}
float Mat_Det_3X3(MATRIX3X3_PTR m) {
    return(m->M00*(m->M11*m->M22 - m->M21*m->M12) - 
       m->M01*(m->M10*m->M22 - m->M20*m->M12) + 
       m->M02*(m->M10*m->M21 - m->M20*m->M11) );
}
int Mat_Mul_3X3(MATRIX3X3_PTR ma, MATRIX3X3_PTR mb, MATRIX3X3_PTR mprod) {
    for (int row=0; row<3; row++) {
        for (int col=0; col<3; col++) {
            float sum = 0; 
            for (int index=0; index<3; index++) {
                sum+=(ma->M[row][index]*mb->M[index][col]);
            }
            mprod->M[row][col] = sum;
        } 
    } 
    return(1);
}
void Mat_Add_4X4(MATRIX4X4_PTR ma, MATRIX4X4_PTR mb, MATRIX4X4_PTR msum) {
    for (int row=0; row<4; row++) {
        for (int col=0; col<4; col++) {
           msum->M[row][col] = ma->M[row][col] + mb->M[row][col];
        } 
    }
}
void Mat_Mul_4X4(MATRIX4X4_PTR ma, MATRIX4X4_PTR mb, MATRIX4X4_PTR mprod) {
    for (int row=0; row<4; row++) {
        for (int col=0; col<4; col++) {
            float sum = 0; 
            for (int index=0; index<4; index++) {
                sum+=(ma->M[row][index]*mb->M[index][col]);
            }
            mprod->M[row][col] = sum;
        } 
    } 
}
void Mat_Mul_1X4_4X4(MATRIX1X4_PTR ma, MATRIX4X4_PTR mb, MATRIX1X4_PTR mprod) {
    for (int col=0; col<4; col++) {
        float sum = 0; 
        for (int row=0; row<4; row++) {
            sum+=(ma->M[row] * mb->M[row][col]);
        }
        mprod->M[col] = sum;
    }
}
void Mat_Mul_VECTOR3D_4X4(VECTOR3D_PTR  va, MATRIX4X4_PTR mb, VECTOR3D_PTR vprod) {
    for (int col=0; col < 3; col++) {
        float sum = 0; 
        int row = 0;
        for (row=0; row<3; row++) {
             sum+=(va->M[row]*mb->M[row][col]);
        } 
        sum+=mb->M[row][col];    
        vprod->M[col] = sum;
    }
}
void Mat_Mul_VECTOR4D_4X4(VECTOR4D_PTR  va, MATRIX4X4_PTR mb, VECTOR4D_PTR vprod) {
    for (int col=0; col < 4; col++) {
        float sum = 0; 
        int row = 0;
        for (row=0; row<4; row++) {
            sum+=(va->M[row]*mb->M[row][col]);
        }    
        vprod->M[col] = sum;
    }
}
int Mat_Inverse_4X4(MATRIX4X4_PTR m, MATRIX4X4_PTR mi) {
    float det =  ( m->M00 * ( m->M11 * m->M22 - m->M12 * m->M21 ) -
                   m->M01 * ( m->M10 * m->M22 - m->M12 * m->M20 ) +
                   m->M02 * ( m->M10 * m->M21 - m->M11 * m->M20 ) );

    if (fabs(det) < EPSILON_E5)
    return(0);

    float det_inv  = 1.0f / det;

    mi->M00 =  det_inv * ( m->M11 * m->M22 - m->M12 * m->M21 );
    mi->M01 = -det_inv * ( m->M01 * m->M22 - m->M02 * m->M21 );
    mi->M02 =  det_inv * ( m->M01 * m->M12 - m->M02 * m->M11 );
    mi->M03 = 0.0f;

    mi->M10 = -det_inv * ( m->M10 * m->M22 - m->M12 * m->M20 );
    mi->M11 =  det_inv * ( m->M00 * m->M22 - m->M02 * m->M20 );
    mi->M12 = -det_inv * ( m->M00 * m->M12 - m->M02 * m->M10 );
    mi->M13 = 0.0f; 

    mi->M20 =  det_inv * ( m->M10 * m->M21 - m->M11 * m->M20 );
    mi->M21 = -det_inv * ( m->M00 * m->M21 - m->M01 * m->M20 );
    mi->M22 =  det_inv * ( m->M00 * m->M11 - m->M01 * m->M10 );
    mi->M23 = 0.0f; 

    mi->M30 = -( m->M30 * mi->M00 + m->M31 * mi->M10 + m->M32 * mi->M20 );
    mi->M31 = -( m->M30 * mi->M01 + m->M31 * mi->M11 + m->M32 * mi->M21 );
    mi->M32 = -( m->M30 * mi->M02 + m->M31 * mi->M12 + m->M32 * mi->M22 );
    mi->M33 = 1.0f;

    return(1);
}
void Mat_Init_4X4(MATRIX4X4_PTR ma, float m00, float m01, float m02, float m03,
                                   float m10, float m11, float m12, float m13,
                                   float m20, float m21, float m22, float m23,
                                   float m30, float m31, float m32, float m33) {
    ma->M00 = m00; ma->M01 = m01; ma->M02 = m02; ma->M03 = m03;
    ma->M10 = m10; ma->M11 = m11; ma->M12 = m12; ma->M13 = m13;
    ma->M20 = m20; ma->M21 = m21; ma->M22 = m22; ma->M23 = m23;
    ma->M30 = m30; ma->M31 = m31; ma->M32 = m32; ma->M33 = m33;
}

void Init_Parm_Line2D(POINT2D_PTR p_init, POINT2D_PTR p_term, PARAMLINE2D_PTR p) {

    VECTOR2D_INIT(&(p->p0), p_init);

    VECTOR2D_INIT(&(p->p1), p_term);

    VECTOR2D_Build(p_init, p_term, &(p->v));
}

void Init_Parm_Line3D(POINT3D_PTR p_init, POINT3D_PTR p_term, PARAMLINE3D_PTR p) {
    VECTOR3D_INIT(&(p->p0), p_init);

    VECTOR3D_INIT(&(p->p1),p_term);

    VECTOR3D_Build(p_init, p_term, &(p->v));
}

void Compute_Parm_Line2D(PARAMLINE2D_PTR p, float t, POINT2D_PTR pt) {
    pt->x = p->p0.x + p->v.x*t;
    pt->y = p->p0.y + p->v.y*t;
} 

void Compute_Parm_Line3D(PARAMLINE3D_PTR p, float t, POINT3D_PTR pt) {
    pt->x = p->p0.x + p->v.x*t;
    pt->y = p->p0.y + p->v.y*t;
    pt->z = p->p0.z + p->v.z*t;
} 

int Intersect_Parm_Lines2D(PARAMLINE2D_PTR p1, PARAMLINE2D_PTR p2, POINT2D_PTR pt) {
    float t1, t2, det_p1p2 = (p1->v.x*p2->v.y - p1->v.y*p2->v.x);

    if (fabs(det_p1p2) <= EPSILON_E5) {
        return(PARM_LINE_NO_INTERSECT);
    }

    t1 = (p2->v.x*(p1->p0.y - p2->p0.y) - p2->v.y*(p1->p0.x - p2->p0.x)) /det_p1p2;

    t2 = (p1->v.x*(p1->p0.y - p2->p0.y) - p1->v.y*(p1->p0.x - p2->p0.x)) /det_p1p2;

    pt->x = p1->p0.x + p1->v.x*t1;
    pt->y = p1->p0.y + p1->v.y*t1;

    if ((t1>=0) && (t1<=1) && (t2>=0) && (t2<=1))
        return(PARM_LINE_INTERSECT_IN_SEGMENT);
    else
        return(PARM_LINE_INTERSECT_OUT_SEGMENT);

}

void PLANE3D_Init(PLANE3D_PTR plane, POINT3D_PTR p0, VECTOR3D_PTR normal, int normalize) {   
    POINT3D_COPY(&plane->p0, p0);

    if (!normalize)
        VECTOR3D_COPY(&plane->n, normal);
    else {
        VECTOR3D_Normalize(normal,&plane->n);
    }
} 

float Compute_Point_In_Plane3D(POINT3D_PTR pt, PLANE3D_PTR plane) {
    float hs = plane->n.x*(pt->x - plane->p0.x) + 
               plane->n.y*(pt->y - plane->p0.y) +
               plane->n.z*(pt->z - plane->p0.z); 
    return(hs);
} 

int Intersect_Parm_Line3D_Plane3D(PARAMLINE3D_PTR pline, PLANE3D_PTR plane, float *t, POINT3D_PTR pt) {
    float plane_dot_line = VECTOR3D_Dot(&pline->v, &plane->n);

    if (fabs(plane_dot_line) <= EPSILON_E5) {
        if (fabs(Compute_Point_In_Plane3D(&pline->p0, plane)) <= EPSILON_E5)
            return(PARM_LINE_INTERSECT_EVERYWHERE);
        else
            return(PARM_LINE_NO_INTERSECT);
   }

    *t = -(plane->n.x*pline->p0.x + 
           plane->n.y*pline->p0.y + 
           plane->n.z*pline->p0.z -
           plane->n.x*plane->p0.x - 
           plane->n.y*plane->p0.y - 
           plane->n.z*plane->p0.z) / (plane_dot_line);
   
    pt->x = pline->p0.x + pline->v.x*(*t);
    pt->y = pline->p0.y + pline->v.y*(*t);
    pt->z = pline->p0.z + pline->v.z*(*t);

    if (*t>=0.0 && *t<=1.0)
    return(PARM_LINE_INTERSECT_IN_SEGMENT );
    else
    return(PARM_LINE_INTERSECT_OUT_SEGMENT);

}

void QUAT_Add(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qsum) {
    qsum->x = q1->x + q2->x;
    qsum->y = q1->y + q2->y;
    qsum->z = q1->z + q2->z;
    qsum->w = q1->w + q2->w;
} 

void QUAT_Sub(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qdiff) {
    qdiff->x = q1->x - q2->x;
    qdiff->y = q1->y - q2->y;
    qdiff->z = q1->z - q2->z;
    qdiff->w = q1->w - q2->w;
}

void QUAT_Conjugate(QUAT_PTR q, QUAT_PTR qconj) {
    qconj->x = -q->x;
    qconj->y = -q->y;
    qconj->z = -q->z;
    qconj->w = q->w;
}

void QUAT_Scale(QUAT_PTR q, float scale, QUAT_PTR qs) {
    qs->x = scale*q->x;
    qs->y = scale*q->y;
    qs->z = scale*q->z;
    qs->w = scale*q->w;

}

float QUAT_Norm(QUAT_PTR q) {
    return(sqrtf(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z));
} 

float QUAT_Norm2(QUAT_PTR q) {
    return(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);
}

void QUAT_Normalize(QUAT_PTR q, QUAT_PTR qn) {
    float qlength_inv = 1.0/(sqrtf(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z));

    qn->w=q->w*qlength_inv;
    qn->x=q->x*qlength_inv;
    qn->y=q->y*qlength_inv;
    qn->z=q->z*qlength_inv;
}

void QUAT_Unit_Inverse(QUAT_PTR q, QUAT_PTR qi) {
    qi->w =  q->w;
    qi->x = -q->x;
    qi->y = -q->y;
    qi->z = -q->z;
}

void QUAT_Inverse(QUAT_PTR q, QUAT_PTR qi) {
    float norm2_inv = 1.0/(q->w*q->w + q->x*q->x + q->y*q->y + q->z*q->z);

    qi->w =  q->w*norm2_inv;
    qi->x = -q->x*norm2_inv;
    qi->y = -q->y*norm2_inv;
    qi->z = -q->z*norm2_inv;
}

void QUAT_Mul(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR qprod) {

    float prd_0 = (q1->z - q1->y) * (q2->y - q2->z);
    float prd_1 = (q1->w + q1->x) * (q2->w + q2->x);
    float prd_2 = (q1->w - q1->x) * (q2->y + q2->z);
    float prd_3 = (q1->y + q1->z) * (q2->w - q2->x);
    float prd_4 = (q1->z - q1->x) * (q2->x - q2->y);
    float prd_5 = (q1->z + q1->x) * (q2->x + q2->y);
    float prd_6 = (q1->w + q1->y) * (q2->w - q2->z);
    float prd_7 = (q1->w - q1->y) * (q2->w + q2->z);

    float prd_8 = prd_5 + prd_6 + prd_7;
    float prd_9 = 0.5 * (prd_4 + prd_8);

    qprod->w = prd_0 + prd_9 - prd_5;
    qprod->x = prd_1 + prd_9 - prd_8;
    qprod->y = prd_2 + prd_9 - prd_7;
    qprod->z = prd_3 + prd_9 - prd_6;
}

void QUAT_Triple_Product(QUAT_PTR q1, QUAT_PTR q2, QUAT_PTR q3, QUAT_PTR qprod) {
    QUAT_T qtmp;
    QUAT_Mul(q1,q2,&qtmp);
    QUAT_Mul(&qtmp, q3, qprod);
} 

void VECTOR3D_Theta_To_QUAT(QUAT_PTR q, VECTOR3D_PTR v, float theta) {

    float theta_div_2 = (0.5)*theta; 
    float sinf_theta = sinf(theta_div_2);

    q->x = sinf_theta * v->x;
    q->y = sinf_theta * v->y;
    q->z = sinf_theta * v->z;
    q->w = cosf( theta_div_2 );
} 

 void VECTOR4D_Theta_To_QUAT(QUAT_PTR q, VECTOR4D_PTR v, float theta) {
    float theta_div_2 = (0.5)*theta; 

    float sinf_theta = sinf(theta_div_2);

    q->x = sinf_theta * v->x;
    q->y = sinf_theta * v->y;
    q->z = sinf_theta * v->z;
    q->w = cosf( theta_div_2 );
} 

void EulerZYX_To_QUAT(QUAT_PTR q, float theta_z, float theta_y, float theta_x) {

    float cos_z_2 = 0.5*cosf(theta_z);
    float cos_y_2 = 0.5*cosf(theta_y);
    float cos_x_2 = 0.5*cosf(theta_x);

    float sin_z_2 = 0.5*sinf(theta_z);
    float sin_y_2 = 0.5*sinf(theta_y);
    float sin_x_2 = 0.5*sinf(theta_x);

    q->w = cos_z_2*cos_y_2*cos_x_2 + sin_z_2*sin_y_2*sin_x_2;
    q->x = cos_z_2*cos_y_2*sin_x_2 - sin_z_2*sin_y_2*cos_x_2;
    q->y = cos_z_2*sin_y_2*cos_x_2 + sin_z_2*cos_y_2*sin_x_2;
    q->z = sin_z_2*cos_y_2*cos_x_2 - cos_z_2*sin_y_2*sin_x_2;

}

void QUAT_To_VECTOR3D_Theta(QUAT_PTR q, VECTOR3D_PTR v, float *theta) {

    *theta = acosf(q->w);

    float sinf_theta_inv = 1.0/sinf(*theta);

    v->x    = q->x*sinf_theta_inv;
    v->y    = q->y*sinf_theta_inv;
    v->z    = q->z*sinf_theta_inv;

    *theta*=2;
}