

#ifndef __CADrawViewS_CCDRAW_NODE_H__
#define __CADrawViewS_CCDRAW_NODE_H__

#include "view/CAView.h"
#include "ccTypes.h"
#include "renderer/CCCustomCommand.h"
#include "math/CAMath.h"

NS_CC_BEGIN

static const int DEFAULT_LINE_WIDTH = 2;

class PointArray;

class CC_DLL CADrawView : public CAView
{
public:
    /** creates and initialize a CADrawView node.
     *
     * @return Return an autorelease object.
     */
    static CADrawView* create(int defaultLineWidth = DEFAULT_LINE_WIDTH);
    
    /** Draw a point.
     *
     * @param point A Vec2 used to point.
     * @param pointSize The point size.
     * @param color The point color.
     * @js NA
     */
    void drawPoint(const Vec2& point, const float pointSize, const CAColor4B &color);
    
    /** Draw a group point.
     *
     * @param position A Vec2 pointer.
     * @param numberOfPoints The number of points.
     * @param color The point color.
     * @js NA
     */
    void drawPoints(const Vec2 *position, unsigned int numberOfPoints, const CAColor4B &color);
    
    /** Draw a group point.
     *
     * @param position A Vec2 pointer.
     * @param numberOfPoints The number of points.
     * @param pointSize The point size.
     * @param color The point color.
     * @js NA
     */
    void drawPoints(const Vec2 *position, unsigned int numberOfPoints, const float pointSize, const CAColor4B &color);
    
    /** Draw an line from origin to destination with color.
     *
     * @param origin The line origin.
     * @param destination The line destination.
     * @param color The line color.
     * @js NA
     */
    void drawLine(const Vec2 &origin, const Vec2 &destination, const CAColor4B &color);
    
    /** Draws a rectangle given the origin and destination point measured in points.
     * The origin and the destination can not have the same x and y coordinate.
     *
     * @param origin The rectangle origin.
     * @param destination The rectangle destination.
     * @param color The rectangle color.
     */
    void drawRect(const Vec2 &origin, const Vec2 &destination, const CAColor4B &color);
    
    /** Draws a polygon given a pointer to point coordinates and the number of vertices measured in points.
     * The polygon can be closed or open.
     *
     * @param poli A pointer to point coordinates.
     * @param numberOfPoints The number of vertices measured in points.
     * @param closePolygon The polygon can be closed or open.
     * @param color The polygon color.
     */
    void drawPoly(const Vec2 *poli, unsigned int numberOfPoints, bool closePolygon, const CAColor4B &color);
    
    /** Draws a circle given the center, radius and number of segments.
     *
     * @param center The circle center point.
     * @param radius The circle rotate of radius.
     * @param angle  The circle angle.
     * @param segments The number of segments.
     * @param drawLineToCenter Whether or not draw the line from the origin to center.
     * @param scaleX The scale value in x.
     * @param scaleY The scale value in y.
     * @param color Set the circle color.
     */
    void drawCircle( const Vec2& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY, const CAColor4B &color);
    
    /** Draws a circle given the center, radius and number of segments.
     *
     * @param center The circle center point.
     * @param radius The circle rotate of radius.
     * @param angle  The circle angle.
     * @param segments The number of segments.
     * @param drawLineToCenter Whether or not draw the line from the origin to center.
     * @param color Set the circle color.
     */
    void drawCircle(const Vec2 &center, float radius, float angle, unsigned int segments, bool drawLineToCenter, const CAColor4B &color);
    
    /** Draws a quad bezier path.
     *
     * @param origin The origin of the bezier path.
     * @param control The control of the bezier path.
     * @param destination The destination of the bezier path.
     * @param segments The number of segments.
     * @param color Set the quad bezier color.
     */
    void drawQuadBezier(const Vec2 &origin, const Vec2 &control, const Vec2 &destination, unsigned int segments, const CAColor4B &color);
    
    /** Draw a cubic bezier curve with color and number of segments
     *
     * @param origin The origin of the bezier path.
     * @param control1 The first control of the bezier path.
     * @param control2 The second control of the bezier path.
     * @param destination The destination of the bezier path.
     * @param segments The number of segments.
     * @param color Set the cubic bezier color.
     */
    void drawCubicBezier(const Vec2 &origin, const Vec2 &control1, const Vec2 &control2, const Vec2 &destination, unsigned int segments, const CAColor4B &color);
    
    /** Draws a Cardinal Spline path.
     *
     * @param config A array point.
     * @param tension The tension of the spline.
     * @param segments The number of segments.
     * @param color Set the Spline color.
     */
    void drawCardinalSpline(PointArray *config, float tension,  unsigned int segments, const CAColor4B &color);
    
    /** Draws a Catmull Rom path.
     *
     * @param points A point array  of control point.
     * @param segments The number of segments.
     * @param color The Catmull Rom color.
     */
    void drawCatmullRom(PointArray *points, unsigned int segments, const CAColor4B &color);
    
    /** draw a dot at a position, with a given radius and color.
     *
     * @param pos The dot center.
     * @param radius The dot radius.
     * @param color The dot color.
     */
    void drawDot(const Vec2 &pos, float radius, const CAColor4B &color);
    
    /** Draws a rectangle with 4 points.
     *
     * @param p1 The rectangle vertex point.
     * @param p2 The rectangle vertex point.
     * @param p3 The rectangle vertex point.
     * @param p4 The rectangle vertex point.
     * @param color The rectangle color.
     */
    void drawRect(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const Vec2& p4, const CAColor4B &color);
    
    /** Draws a solid rectangle given the origin and destination point measured in points.
     * The origin and the destination can not have the same x and y coordinate.
     *
     * @param origin The rectangle origin.
     * @param destination The rectangle destination.
     * @param color The rectangle color.
     * @js NA
     */
    void drawSolidRect(const Vec2 &origin, const Vec2 &destination, const CAColor4B &color);
    
    /** Draws a solid polygon given a pointer to CGPoint coordinates, the number of vertices measured in points, and a color.
     *
     * @param poli A solid polygon given a pointer to CGPoint coordinates.
     * @param numberOfPoints The number of vertices measured in points.
     * @param color The solid polygon color.
     * @js NA
     */
    void drawSolidPoly(const Vec2 *poli, unsigned int numberOfPoints, const CAColor4B &color);
    
    /** Draws a solid circle given the center, radius and number of segments.
     * @param center The circle center point.
     * @param radius The circle rotate of radius.
     * @param angle  The circle angle.
     * @param segments The number of segments.
     * @param scaleX The scale value in x.
     * @param scaleY The scale value in y.
     * @param color The solid circle color.
     * @js NA
     */
    void drawSolidCircle(const Vec2& center, float radius, float angle, unsigned int segments, float scaleX, float scaleY, const CAColor4B &color);
    
    /** Draws a solid circle given the center, radius and number of segments.
     * @param center The circle center point.
     * @param radius The circle rotate of radius.
     * @param angle  The circle angle.
     * @param segments The number of segments.
     * @param color The solid circle color.
     * @js NA
     */
    void drawSolidCircle(const Vec2& center, float radius, float angle, unsigned int segments, const CAColor4B& color);
    
    /** draw a segment with a radius and color.
     *
     * @param from The segment origin.
     * @param to The segment destination.
     * @param radius The segment radius.
     * @param color The segment color.
     */
    void drawSegment(const Vec2 &from, const Vec2 &to, float radius, const CAColor4B &color);
    
    /** draw a polygon with a fill color and line color
     * @code
     * When this function bound into js or lua,the parameter will be changed
     * In js: var drawPolygon(var Arrayofpoints, var fillColor, var width, var borderColor)
     * In lua:local drawPolygon(local pointTable,local tableCount,local fillColor,local width,local borderColor)
     * @endcode
     * @param verts A pointer to point coordinates.
     * @param count The number of verts measured in points.
     * @param fillColor The color will fill in polygon.
     * @param borderWidth The border of line width.
     * @param borderColor The border of line color.
     * @js NA
     */
    void drawPolygon(const Vec2 *verts, int count, const CAColor4B &fillColor, float borderWidth, const CAColor4B &borderColor);
    
    /** draw a triangle with color.
     *
     * @param p1 The triangle vertex point.
     * @param p2 The triangle vertex point.
     * @param p3 The triangle vertex point.
     * @param color The triangle color.
     * @js NA
     */
    void drawTriangle(const Vec2 &p1, const Vec2 &p2, const Vec2 &p3, const CAColor4B &color);
    
    
    /** Clear the geometry in the node's buffer. */
    void clear();
    /** Get the color mixed mode.
     * @lua NA
     */
    const BlendFunc& getBlendFunc() const;
    /** Set the color mixed mode.
     * @code
     * When this function bound into js or lua,the parameter will be changed
     * In js: var setBlendFunc(var src, var dst)
     * @endcode
     * @lua NA
     */
    void setBlendFunc(const BlendFunc &blendFunc);
    
    /**
     * @js NA
     */
    virtual void onDraw(const Mat4 &transform, uint32_t flags);
    /**
     * @js NA
     */
    virtual void onDrawGLLine(const Mat4 &transform, uint32_t flags);
    /**
     * @js NA
     */
    virtual void onDrawGLPoint(const Mat4 &transform, uint32_t flags);
    
    // Overrides
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    
    void setLineWidth(int lineWidth);
    
    // Get CocosStudio guide lines width.
    float getLineWidth();
    
    CADrawView(int lineWidth = DEFAULT_LINE_WIDTH);
    virtual ~CADrawView();
    virtual bool init() override;
    
protected:
    void ensureCapacity(int count);
    void ensureCapacityGLPoint(int count);
    void ensureCapacityGLLine(int count);
    
    GLuint      _vao;
    GLuint      _vbo;
    GLuint      _vaoGLPoint;
    GLuint      _vboGLPoint;
    GLuint      _vaoGLLine;
    GLuint      _vboGLLine;
    
    int         _bufferCapacity;
    GLsizei     _bufferCount;
    ccV2F_C4B_T2F *_buffer;
    
    int         _bufferCapacityGLPoint;
    GLsizei     _bufferCountGLPoint;
    ccV2F_C4B_T2F *_bufferGLPoint;
    CAColor4B     _pointColor;
    int         _pointSize;
    
    int         _bufferCapacityGLLine;
    GLsizei     _bufferCountGLLine;
    ccV2F_C4B_T2F *_bufferGLLine;
    
    BlendFunc   _blendFunc;
    CustomCommand _customCommand;
    CustomCommand _customCommandGLPoint;
    CustomCommand _customCommandGLLine;
    
    bool        _dirty;
    bool        _dirtyGLPoint;
    bool        _dirtyGLLine;
    
    int         _lineWidth;
    
    int  _defaultLineWidth;};

CC_DEPRECATED_ATTRIBUTE typedef CADrawView CCCADrawView;

NS_CC_END

#endif // __CADrawViewS_CCDRAW_NODE_H__
