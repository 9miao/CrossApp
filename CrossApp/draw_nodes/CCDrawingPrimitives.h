
#ifndef __CCDRAWING_PRIMITIVES__
#define __CCDRAWING_PRIMITIVES__
    

#include "ccTypes.h"
#include "ccMacros.h"
#include "basics/CAGeometry.h"    // for CCPoint
#include <vector>


NS_CC_BEGIN

/**
 * @addtogroup global
 * @{
 */

class CCPointArray;

/** Initializes the drawing primitives */
void CC_DLL ccDrawInit();

/** Frees allocated resources by the drawing primitives */
void CC_DLL ccDrawFree();

/** draws a point given x and y coordinate measured in points */
void CC_DLL ccDrawPoint( const CCPoint& point );

/** draws an array of points.
 @since v0.7.2
 */
void CC_DLL ccDrawPoints( const CCPoint *points, unsigned int numberOfPoints );

/** draws a line given the origin and destination point measured in points */
void CC_DLL ccDrawLine( const CCPoint& origin, const CCPoint& destination );

/** draws a rectangle given the origin and destination point measured in points. */
void CC_DLL ccDrawRect( CCPoint origin, CCPoint destination );

/** draws a solid rectangle given the origin and destination point measured in points.
    @since 1.1
 */
void CC_DLL ccDrawSolidRect( CCPoint origin, CCPoint destination, CAColor4F color );

/** draws a polygon given a pointer to CCPoint coordinates and the number of vertices measured in points.
The polygon can be closed or open
*/
void CC_DLL ccDrawPoly( const CCPoint *vertices, unsigned int numOfVertices, bool closePolygon );

/** draws a solid polygon given a pointer to CGPoint coordinates, the number of vertices measured in points, and a color.
 */
void CC_DLL ccDrawSolidPoly( const CCPoint *poli, unsigned int numberOfPoints, CAColor4F color );

/** draws a circle given the center, radius and number of segments. */
void CC_DLL ccDrawCircle( const CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY);
void CC_DLL ccDrawCircle( const CCPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter);

/** draws a quad bezier path
 @warning This function could be pretty slow. Use it only for debugging purposes.
 @since v0.8
 */
void CC_DLL ccDrawQuadBezier(const CCPoint& origin, const CCPoint& control, const CCPoint& destination, unsigned int segments);

/** draws a cubic bezier path
 @warning This function could be pretty slow. Use it only for debugging purposes.
 @since v0.8
 */
void CC_DLL ccDrawCubicBezier(const CCPoint& origin, const CCPoint& control1, const CCPoint& control2, const CCPoint& destination, unsigned int segments);

/** draws a Catmull Rom path.
 @warning This function could be pretty slow. Use it only for debugging purposes.
 @since v2.0
 */
void CC_DLL ccDrawCatmullRom(const std::vector<CCPoint>& arrayOfControlPoints, unsigned int segments );

/** draws a Cardinal Spline path.
 @warning This function could be pretty slow. Use it only for debugging purposes.
 @since v2.0
 */
void CC_DLL ccDrawCardinalSpline(const std::vector<CCPoint>& config, float tension,  unsigned int segments );

extern CC_DLL CCPoint ccCardinalSplineAt(CCPoint &p0, CCPoint &p1, CCPoint &p2, CCPoint &p3, float tension, float t);

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
void CC_DLL ccPointSize( GLfloat pointSize );

// end of global group
/// @}

NS_CC_END

#endif // __CCDRAWING_PRIMITIVES__
