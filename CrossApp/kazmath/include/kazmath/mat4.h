

#ifndef MAT4_H_INCLUDED
#define MAT4_H_INCLUDED

#include "platform/CCPlatformMacros.h"
#include "utility.h"

struct kmVec3;
struct kmMat3;
struct kmQuaternion;
struct kmPlane;

/*
A 4x4 matrix

        | 0   4   8  12 |
mat =   | 1   5   9  13 |
        | 2   6  10  14 |
        | 3   7  11  15 |
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kmMat4 {
    kmScalar mat[16];
} kmMat4;

CC_DLL kmMat4* const kmMat4Fill(kmMat4* pOut, const kmScalar* pMat);


CC_DLL kmMat4* const kmMat4Identity(kmMat4* pOut);

CC_DLL kmMat4* const kmMat4Inverse(kmMat4* pOut, const kmMat4* pM);


CC_DLL const int kmMat4IsIdentity(const kmMat4* pIn);

CC_DLL kmMat4* const kmMat4Transpose(kmMat4* pOut, const kmMat4* pIn);
CC_DLL kmMat4* const kmMat4Multiply(kmMat4* pOut, const kmMat4* pM1, const kmMat4* pM2);

CC_DLL kmMat4* const kmMat4Assign(kmMat4* pOut, const kmMat4* pIn);
CC_DLL const int kmMat4AreEqual(const kmMat4* pM1, const kmMat4* pM2);

CC_DLL kmMat4* const kmMat4RotationX(kmMat4* pOut, const kmScalar radians);
CC_DLL kmMat4* const kmMat4RotationY(kmMat4* pOut, const kmScalar radians);
CC_DLL kmMat4* const kmMat4RotationZ(kmMat4* pOut, const kmScalar radians);
CC_DLL kmMat4* const kmMat4RotationPitchYawRoll(kmMat4* pOut, const kmScalar pitch, const kmScalar yaw, const kmScalar roll);
CC_DLL kmMat4* const kmMat4RotationQuaternion(kmMat4* pOut, const struct kmQuaternion* pQ);
CC_DLL kmMat4* const kmMat4RotationTranslation(kmMat4* pOut, const struct kmMat3* rotation, const struct kmVec3* translation);
CC_DLL kmMat4* const kmMat4Scaling(kmMat4* pOut, const kmScalar x, const kmScalar y, const kmScalar z);
CC_DLL kmMat4* const kmMat4Translation(kmMat4* pOut, const kmScalar x, const kmScalar y, const kmScalar z);

CC_DLL struct kmVec3* const kmMat4GetUpVec3(struct kmVec3* pOut, const kmMat4* pIn);
CC_DLL struct kmVec3* const kmMat4GetRightVec3(struct kmVec3* pOut, const kmMat4* pIn);
CC_DLL struct kmVec3* const kmMat4GetForwardVec3(struct kmVec3* pOut, const kmMat4* pIn);

CC_DLL kmMat4* const kmMat4PerspectiveProjection(kmMat4* pOut, kmScalar fovY, kmScalar aspect, kmScalar zNear, kmScalar zFar);
CC_DLL kmMat4* const kmMat4OrthographicProjection(kmMat4* pOut, kmScalar left, kmScalar right, kmScalar bottom, kmScalar top, kmScalar nearVal, kmScalar farVal);
CC_DLL kmMat4* const kmMat4LookAt(kmMat4* pOut, const struct kmVec3* pEye, const struct kmVec3* pCenter, const struct kmVec3* pUp);

CC_DLL kmMat4* const kmMat4RotationAxisAngle(kmMat4* pOut, const struct kmVec3* axis, kmScalar radians);
CC_DLL struct kmMat3* const kmMat4ExtractRotation(struct kmMat3* pOut, const kmMat4* pIn);
CC_DLL struct kmPlane* const kmMat4ExtractPlane(struct kmPlane* pOut, const kmMat4* pIn, const kmEnum plane);
CC_DLL struct kmVec3* const kmMat4RotationToAxisAngle(struct kmVec3* pAxis, kmScalar* radians, const kmMat4* pIn);
#ifdef __cplusplus
}
#endif
#endif /* MAT4_H_INCLUDED */
