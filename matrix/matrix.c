#include "matrix.h"
#include "../utils/utils.h"

matrix4x4_t matrix4x4_identity() {
    matrix4x4_t m = {0};
    m.M[0][0] = 1.0f;
    m.M[1][1] = 1.0f;
    m.M[2][2] = 1.0f;
    m.M[3][3] = 1.0f;
    return m;
}

matrix4x4_t matrix4x4_zero() {
    matrix4x4_t m = {0};
    return m;
}

matrix4x4_t matrix4x4_mult_4x4(matrix4x4_t ma, matrix4x4_t mb) {
    matrix4x4_t result = {0};
    for (int row=0; row<4; row++) {
        for (int col=0; col<4; col++) {
            float sum = 0; 
            for (int index=0; index<4; index++) {
                sum+=(ma.M[row][index]*mb.M[index][col]);
            }
            result.M[row][col] = sum;
        } 
    } 
    return result;
}

vector4d_t matrix4x4_mult_4x1(matrix4x4_t ma, vector4d_t mb) {
    vector4d_t result = {0};
    for (int row=0; row<4; row++) {
        float sum = 0; 
        for (int col=0; col<4; col++) {
            sum += ma.M[row][col] * mb.M[col];
        }
        result.M[row] = sum;
    }
    return result;
}

/******************************************************************************
 * camera is defined in the right-hand coordinate system, 
 * and camera is always looking at the -z axis. thus 
 * z-axis = -normalize(target - eye);
 * x-axis = normalize(-z-axis x up);
 * y-axis = normalize(z-axis x x-axis);
 * which `x` is cross product and `-` is vector subtraction, and
 * x-axis、y-axis and z-axis should be normalize.
 * so look-at matrix as follows: 
 * x_axis.x  x_axis.y  x_axis.z  -dot(x_axis,eye)
 * y_axis.x  y_axis.y  y_axis.z  -dot(y_axis,eye)
 * z_axis.x  z_axis.y  z_axis.z  -dot(z_axis,eye)
 *    0         0         0                 1
 * 
 * for more details, refer to
 * https://www.bilibili.com/video/BV1BP411Q7ej/
 * https://www.bilibili.com/video/BV1X7411F744?p=4
 * https://www.cnblogs.com/Gr-blogs/p/17741819.html#%E8%A7%86%E5%9B%BE%E5%8F%98%E6%8D%A2view-transformation
 * 
 * eye: the position of the eye point
 * target: the position of the target point
 * up: the position of the up vector
******************************************************************************/
matrix4x4_t matrix4x4_lookAt(vector3d_t eye, vector3d_t target, vector3d_t up) {

    vector3d_t lookAt = vector3d_Sub(target, eye);
    vector3d_t Negative_z_axis = vector3d_Normalize(lookAt);
    vector3d_t z_axis = { -Negative_z_axis.x, -Negative_z_axis.y, -Negative_z_axis.z };
    vector3d_t x_axis = vector3d_Normalize(vector3d_Cross(lookAt, up));
    vector3d_t y_axis = vector3d_Cross(z_axis, x_axis);
    matrix4x4_t rotate = matrix4x4_identity();
    matrix4x4_t translate = matrix4x4_identity();

    rotate.M[0][0] = x_axis.x;
    rotate.M[0][1] = x_axis.y;
    rotate.M[0][2] = x_axis.z;

    rotate.M[1][0] = y_axis.x;
    rotate.M[1][1] = y_axis.y;
    rotate.M[1][2] = y_axis.z;

    rotate.M[2][0] = z_axis.x;
    rotate.M[2][1] = z_axis.y;
    rotate.M[2][2] = z_axis.z;

    translate.M[0][3] = -eye.x;
    translate.M[1][3] = -eye.y;
    translate.M[2][3] = -eye.z;

    return matrix4x4_mult_4x4(rotate, translate);
}

matrix4x4_t matrix4x4_translate(float x, float y, float z) {
    matrix4x4_t m= matrix4x4_identity();
    m.M[0][3] = x;
    m.M[1][3] = y;
    m.M[2][3] = z;
    return m;
}

matrix4x4_t matrix4x4_scale(float x, float y, float z) {
    matrix4x4_t m = matrix4x4_identity();
    m.M[0][0] = x;
    m.M[1][1] = y;
    m.M[2][2] = z;
    return m;
}

/******************************************************************************
 * orthographic projection matrix as follows and 
 * defined in the right-hand coordinate system:
 *  2/(r-l)      0         0        -(r+l)/(r-l)
 *    0       2/(t-b)      0        -(t+b)/(t-b)
 *    0          0       2/(n-f)    -(n+f)/(n-f)
 *    0          0         0              1
 * notes: n and f are negative values.
 * 
 * for more details, refer to
 * https://www.bilibili.com/video/BV1X7411F744?p=4
 * https://www.bilibili.com/video/BV1UX4y1k7CT/
 * https://www.cnblogs.com/Gr-blogs/p/17741819.html#%E6%9C%89%E6%95%B0%E7%BB%84%E7%9A%84%E6%8A%A5%E5%91%8A
 * 
 * left: the negative x-axis coordinate point of near clipping plane.
 * right: the positive x-axis coordinate point of near clipping plane.
 * bottom: the negative y-axis coordinate point of near clipping plane.
 * top: the positive y-axis coordinate point of near clipping plane.
 * near: the negative z-axis coordinate point of near clipping plane.
 * far: the negative z-axis coordinate point of far clipping plane.
******************************************************************************/
matrix4x4_t matrix4x4_ortho(float left, float right, float bottom, float top, float near, float far) {
    
    float translateX = - (right + left) / 2;
    float translateY = - (top + bottom) / 2;
    float translateZ = - (near + far) / 2;
    matrix4x4_t translate = matrix4x4_translate(translateX, translateY, translateZ);
    
    float scaleX = 2 / (right - left);
    float scaleY = 2 / (top - bottom);
    float scaleZ = 2 / (near - far);
    matrix4x4_t scale = matrix4x4_scale(scaleX, scaleY, scaleZ);

    matrix4x4_t m = matrix4x4_mult_4x4(scale, translate);

    return m;
}

/**
 * perspective projection matrix as follows and
 * defined in the right-hand coordinate system:
 * 2n/(r-l)         0            -(r+l)/(r-l)           0
 * 0             2n/(t-b)        -(t+b)/(t-b)           0
 * 0              0               (n+f)/(n-f)      -(2nf)/(n-f)
 * 0              0                    1                0
 *
 * for more details, refer to
 * https://www.bilibili.com/video/BV1LS4y1b7xZ/?p=1&t=0&spm_id_from=..0.0&vd_source=9f9eb54cdac68c9a0eb1a034b61df5d7
 * https://www.bilibili.com/video/BV1X7411F744?p=4
 * https://www.bilibili.com/video/BV1UX4y1k7CT/
 * https://www.cnblogs.com/Gr-blogs/p/17741819.html#%E6%9C%89%E6%95%B0%E7%BB%84%E7%9A%84%E6%8A%A5%E5%91%8A
 * 
 * fovY: the camera's vertical field of view(unit in degress, conversion to radians internally).
 * aspect: The ratio of image width to image height(width/height).
 * near: the distance of near clipping plane, is positive value.
 * far: the distance of far clipping plane, is positive value.
 */
matrix4x4_t matrix4x4_perspective(float fovY, float aspect, float near, float far) {
    
    float radians = degrees_2_radians(fovY);
    float top = (float)tan(radians / 2) * fabs(near);
    float bottom = -top;
    float right = top * aspect;
    float left = -right;

    float negative_near = near < 0 ? near : -near;
    float negative_far = far < 0 ? far : -far;

    matrix4x4_t Persp2Ortho = matrix4x4_identity();

    Persp2Ortho.M[0][0] = negative_near;
    Persp2Ortho.M[1][1] = negative_near;
    Persp2Ortho.M[2][2] = negative_near + negative_far;
    Persp2Ortho.M[2][3] = - (negative_near * negative_far);
    Persp2Ortho.M[3][2] = 1;
    Persp2Ortho.M[3][3] = 0;

    matrix4x4_t ortho = matrix4x4_ortho(left, right, bottom, top, negative_near, negative_far);

    matrix4x4_t m = matrix4x4_mult_4x4(ortho, Persp2Ortho);

    return m;
}

matrix4x4_t matrix4x4_viewport(int width, int height) {
    matrix4x4_t m = matrix4x4_identity();
    m.M[0][0] = 0.5f * width;
    m.M[1][1] = 0.5f * height;
    m.M[0][3] = 0.5f * width;
    m.M[1][3] = 0.5f * height;
    return m;
}