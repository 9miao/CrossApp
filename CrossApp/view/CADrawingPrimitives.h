
#ifndef __CCDRAWING_PRIMITIVES__
#define __CCDRAWING_PRIMITIVES__
    

#include "ccTypes.h"
#include "ccMacros.h"
#include "basics/CAPoint.h"
#include <vector>


NS_CC_BEGIN

/**
 * @addtogroup global
 * @{
 */


/** Initializes the drawing primitives */
void CC_DLL ccDrawInit();

/** Frees allocated resources by the drawing primitives */
void CC_DLL ccDrawFree();

/** draws a point given x and y coordinate measured in points */
void CC_DLL ccDrawPoint( const DPoint& point );

/** draws an array of points.
 @since v0.7.2
 */
void CC_DLL ccDrawPoints( const DPoint *points, unsigned int numberOfPoints );

/** draws a line given the origin and destination point measured in points */
void CC_DLL ccDrawLine( const DPoint& origin, const DPoint& destination );

/** draws a rectangle given the origin and destination point measured in points. */
void CC_DLL ccDrawRect( DPoint origin, DPoint destination );

/** draws a solid rectangle given the origin and destination point measured in points.
    @since 1.1
 */
void CC_DLL ccDrawSolidRect( DPoint origin, DPoint destination, CAColor4F color );

/** draws a polygon given a pointer to DPoint coordinates and the number of vertices measured in points.
The polygon can be closed or open
*/
void CC_DLL ccDrawPoly( const DPoint *vertices, unsigned int numOfVertices, bool closePolygon );

/** draws a solid polygon given a pointer to CGPoint coordinates, the number of vertices measured in points, and a color.
 */
void CC_DLL ccDrawSolidPoly( const DPoint *poli, unsigned int numberOfPoints, CAColor4F color );

/** draws a circle given the center, radius and number of segments. */
void CC_DLL ccDrawCircle( const DPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY);
void CC_DLL ccDrawCircle( const DPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter);

/** draws a quad bezier path
 @warning This function could be pretty slow. Use it only for debugging purposes.
 @since v0.8
 */
void CC_DLL ccDrawQuadBezier(const DPoint& origin, const DPoint& control, const DPoint& destination, unsigned int segments);

/** draws a cubic bezier path
 @warning This function could be pretty slow. Use it only for debugging purposes.
 @since v0.8
 */
void CC_DLL ccDrawCubicBezier(const DPoint& origin, const DPoint& control1, const DPoint& control2, const DPoint& destination, unsigned int segments);

/** draws a Catmull Rom path.
 @warning This function could be pretty slow. Use it only for debugging purposes.
 @since v2.0
 */
void CC_DLL ccDrawCatmullRom(const std::vector<DPoint>& arrayOfControlPoints, unsigned int segments );

/** draws a Cardinal Spline path.
 @warning This function could be pretty slow. Use it only for debugging purposes.
 @since v2.0
 */
void CC_DLL ccDrawCardinalSpline(const std::vector<DPoint>& config, float tension,  unsigned int segments );

extern CC_DLL DPoint ccCardinalSplineAt(DPoint &p0, DPoint &p1, DPoint &p2, DPoint &p3, float tension, float t);

/** set the drawing color with 4 unsigned bytes
 @since v2.0
 */
void CC_DLL ccDrawColor4B( GLubyte r, GLubyte g, GLubyte b, GLubyte a );

/** set the drawing color with 4 floats
 @since v2.0
 */
void CC_DLL ccDrawColor4F( GLfloat r, GLfloat g, GLfloat b, GLfloat a );

/** set the point size in points. Default 1.
 @since v2.0
 */
void CC_DLL DPointSize( GLfloat pointSize );

// end of global group
/// @}

NS_CC_END

#endif // __CCDRAWING_PRIMITIVES__
