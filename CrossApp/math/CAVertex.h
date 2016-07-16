
#ifndef __CCVERTEX_H__
#define __CCVERTEX_H__

#include "ccTypes.h"

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

/** @file CCVertex.h */

/** converts a line to a polygon */
void CC_DLL ccVertexLineToPolygon(DPoint *points, float stroke, DPoint *vertices, unsigned int offset, unsigned int nuPoints);

/** returns whether or not the line intersects */
bool CC_DLL ccVertexLineIntersect(float Ax, float Ay,
                                  float Bx, float By,
                                  float Cx, float Cy,
                                  float Dx, float Dy, float *T);

// end of data_structures group
/// @}

NS_CC_END

#endif /* __CCVERTEX_H__ */

