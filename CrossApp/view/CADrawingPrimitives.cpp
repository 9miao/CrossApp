
#include "CADrawingPrimitives.h"
#include "ccTypes.h"
#include "ccMacros.h"
#include "CCGL.h"
#include "basics/CAApplication.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCGLProgram.h"
#include "renderer/CCRenderer.h"
#include "basics/CAPointExtension.h"
#include <string.h>
#include <cmath>

NS_CC_BEGIN

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (push)
#pragma warning (disable: 4996)
#endif

#ifndef M_PI
    #define M_PI       3.14159265358979323846
#endif

static bool s_bInitialized = false;
static GLProgram* s_shader = nullptr;
static int s_nColorLocation = -1;
static CAColor4F s_tColor = {1.0f,1.0f,1.0f,1.0f};
static int s_nPointSizeLocation = -1;
static GLfloat s_fPointSize = 1.0f;

static void lazy_init( void )
{
    if( ! s_bInitialized ) {

        //
        // Position and 1 color passed as a uniform (to simulate glColor4ub )
        //
        s_shader = GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_U_COLOR);
        s_shader->retain();
        
        s_nColorLocation = s_shader->getUniformLocation("u_color");
    CHECK_GL_ERROR_DEBUG();
        s_nPointSizeLocation = s_shader->getUniformLocation("u_pointSize");
    CHECK_GL_ERROR_DEBUG();

        s_bInitialized = true;
    }
}

// When switching from backround to foreground on android, we want the params to be initialized again
void ccDrawInit()
{
    lazy_init();
}

void ccDrawFree()
{
	CC_SAFE_RELEASE_NULL(s_shader);
	s_bInitialized = false;
}

void ccDrawPoint( const DPoint& point )
{
    lazy_init();

    DPoint p;
    p.x = point.x;
    p.y = point.y;

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );
    s_shader->use();
    s_shader->setUniformsForBuiltins();

    s_shader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &s_tColor.r, 1);
    s_shader->setUniformLocationWith1f(s_nPointSizeLocation, s_fPointSize);

    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, &p);
    glDrawArrays(GL_POINTS, 0, 1);
    
    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,1);
}

void ccDrawPoints( const DPoint *points, unsigned int numberOfPoints )
{
    lazy_init();

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );
    s_shader->use();
    s_shader->setUniformsForBuiltins();
    s_shader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &s_tColor.r, 1);
    s_shader->setUniformLocationWith1f(s_nPointSizeLocation, s_fPointSize);

    // XXX: Mac OpenGL error. arrays can't go out of scope before draw is executed
    DPoint* newPoints = new DPoint[numberOfPoints];

    // iPhone and 32-bit machines optimization
    if( sizeof(DPoint) == sizeof(DPoint) )
    {
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, points);

    }
    else
    {
        // Mac on 64-bit
        for( unsigned int i=0; i<numberOfPoints;i++) {
            newPoints[i].x = points[i].x;
            newPoints[i].y = points[i].y;
        }
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, newPoints);

    }

    glDrawArrays(GL_POINTS, 0, (GLsizei) numberOfPoints);

    CC_SAFE_DELETE_ARRAY(newPoints);
    
    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, numberOfPoints);
}


void ccDrawLine( const DPoint& origin, const DPoint& destination )
{
    lazy_init();

    DPoint vertices[2] = {
        {origin.x, origin.y},
        {destination.x, destination.y}
    };

    s_shader->use();
    s_shader->setUniformsForBuiltins();
    s_shader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &s_tColor.r, 1);

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_LINES, 0, 2);
    
    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,2);
}

void ccDrawRect( DPoint origin, DPoint destination )
{
    ccDrawLine(DPoint(origin.x, origin.y), DPoint(destination.x, origin.y));
    ccDrawLine(DPoint(destination.x, origin.y), DPoint(destination.x, destination.y));
    ccDrawLine(DPoint(destination.x, destination.y), DPoint(origin.x, destination.y));
    ccDrawLine(DPoint(origin.x, destination.y), DPoint(origin.x, origin.y));
}

void ccDrawSolidRect( DPoint origin, DPoint destination, CAColor4F color )
{
    DPoint vertices[] = {
        origin,
        DPoint(destination.x, origin.y),
        destination,
        DPoint(origin.x, destination.y)
    };

    ccDrawSolidPoly(vertices, 4, color );
}

void ccDrawPoly( const DPoint *poli, unsigned int numberOfPoints, bool closePolygon )
{
    lazy_init();

    s_shader->use();
    s_shader->setUniformsForBuiltins();
    s_shader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &s_tColor.r, 1);

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );

    // iPhone and 32-bit machines optimization
    if( sizeof(DPoint) == sizeof(DPoint) )
    {
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, poli);


        if( closePolygon )
            glDrawArrays(GL_LINE_LOOP, 0, (GLsizei) numberOfPoints);
        else
            glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) numberOfPoints);
    }
    else
    {
        // Mac on 64-bit
        // XXX: Mac OpenGL error. arrays can't go out of scope before draw is executed
        DPoint* newPoli = new DPoint[numberOfPoints];
        for( unsigned int i=0; i<numberOfPoints;i++) {
            newPoli[i].x = poli[i].x;
            newPoli[i].y = poli[i].y;
        }
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, newPoli);

        if( closePolygon )
            glDrawArrays(GL_LINE_LOOP, 0, (GLsizei) numberOfPoints);
        else
            glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) numberOfPoints);

        CC_SAFE_DELETE_ARRAY(newPoli);
    }
    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, numberOfPoints);
}

void ccDrawSolidPoly( const DPoint *poli, unsigned int numberOfPoints, CAColor4F color )
{
    lazy_init();

    s_shader->use();
    s_shader->setUniformsForBuiltins();
    s_shader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &color.r, 1);

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );

    // XXX: Mac OpenGL error. arrays can't go out of scope before draw is executed
    DPoint* newPoli = new DPoint[numberOfPoints];

    // iPhone and 32-bit machines optimization
    if( sizeof(DPoint) == sizeof(DPoint) )
    {
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, poli);
    }
    else
    {
        // Mac on 64-bit
        for( unsigned int i=0; i<numberOfPoints;i++)
        {
            newPoli[i] = DPoint( poli[i].x, poli[i].y );
        }
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, newPoli);
    }

    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei) numberOfPoints);

    CC_SAFE_DELETE_ARRAY(newPoli);
    
    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, numberOfPoints);
}

void ccDrawCircle( const DPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter, float scaleX, float scaleY)
{
    lazy_init();

    int additionalSegment = 1;
    if (drawLineToCenter)
        additionalSegment++;

    const float coef = 2.0f * (float)M_PI/segments;

    GLfloat *vertices = (GLfloat*)calloc( sizeof(GLfloat)*2*(segments+2), 1);
    if( ! vertices )
        return;

    for(unsigned int i = 0;i <= segments; i++) {
        float rads = i*coef;
        GLfloat j = radius * cosf(rads + angle) * scaleX + center.x;
        GLfloat k = radius * sinf(rads + angle) * scaleY + center.y;

        vertices[i*2] = j;
        vertices[i*2+1] = k;
    }
    vertices[(segments+1)*2] = center.x;
    vertices[(segments+1)*2+1] = center.y;

    s_shader->use();
    s_shader->setUniformsForBuiltins();
    s_shader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &s_tColor.r, 1);

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );

    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) segments+additionalSegment);

    free( vertices );
}

void CC_DLL ccDrawCircle( const DPoint& center, float radius, float angle, unsigned int segments, bool drawLineToCenter)
{
    ccDrawCircle(center, radius, angle, segments, drawLineToCenter, 1.0f, 1.0f);
}

void ccDrawQuadBezier(const DPoint& origin, const DPoint& control, const DPoint& destination, unsigned int segments)
{
    lazy_init();

    DPoint* vertices = new DPoint[segments + 1];

    float t = 0.0f;
    for(unsigned int i = 0; i < segments; i++)
    {
        vertices[i].x = powf(1 - t, 2) * origin.x + 2.0f * (1 - t) * t * control.x + t * t * destination.x;
        vertices[i].y = powf(1 - t, 2) * origin.y + 2.0f * (1 - t) * t * control.y + t * t * destination.y;
        t += 1.0f / segments;
    }
    vertices[segments].x = destination.x;
    vertices[segments].y = destination.y;

    s_shader->use();
    s_shader->setUniformsForBuiltins();
    s_shader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &s_tColor.r, 1);

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );

    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) segments + 1);
    CC_SAFE_DELETE_ARRAY(vertices);
    
    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,segments+1);
}

void ccDrawCatmullRom(const std::vector<DPoint>& points, unsigned int segments )
{
    ccDrawCardinalSpline( points, 0.5f, segments );
}

void ccDrawCardinalSpline(const std::vector<DPoint>& config, float tension,  unsigned int segments )
{
    lazy_init();

    DPoint* vertices = new DPoint[segments + 1];

    unsigned int p;
    float lt;
    float deltaT = 1.0f / config.size();

    for( unsigned int i=0; i < segments+1;i++) {

        float dt = (float)i / segments;

        // border
        if( dt == 1 ) {
            p = (unsigned int)config.size() - 1;
            lt = 1;
        } else {
            p = dt / deltaT;
            lt = (dt - deltaT * (float)p) / deltaT;
        }

        // Interpolate
        DPoint pp0 = config.at(p-1);
        DPoint pp1 = config.at(p+0);
        DPoint pp2 = config.at(p+1);
        DPoint pp3 = config.at(p+2);

        DPoint newPos = ccCardinalSplineAt( pp0, pp1, pp2, pp3, tension, lt);
        vertices[i].x = newPos.x;
        vertices[i].y = newPos.y;
    }

    s_shader->use();
    s_shader->setUniformsForBuiltins();
    s_shader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*)&s_tColor.r, 1);

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );

    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) segments + 1);

    CC_SAFE_DELETE_ARRAY(vertices);
    
    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,segments+1);
}

// CatmullRom Spline formula:
DPoint ccCardinalSplineAt(DPoint &p0, DPoint &p1, DPoint &p2, DPoint &p3, float tension, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;
    
	/*
	 * Formula: s(-ttt + 2tt - t)P1 + s(-ttt + tt)P2 + (2ttt - 3tt + 1)P2 + s(ttt - 2tt + t)P3 + (-2ttt + 3tt)P3 + s(ttt - tt)P4
	 */
    float s = (1 - tension) / 2;
	
    float b1 = s * ((-t3 + (2 * t2)) - t);                      // s(-t3 + 2 t2 - t)P1
    float b2 = s * (-t3 + t2) + (2 * t3 - 3 * t2 + 1);          // s(-t3 + t2)P2 + (2 t3 - 3 t2 + 1)P2
    float b3 = s * (t3 - 2 * t2 + t) + (-2 * t3 + 3 * t2);      // s(t3 - 2 t2 + t)P3 + (-2 t3 + 3 t2)P3
    float b4 = s * (t3 - t2);                                   // s(t3 - t2)P4
    
    float x = (p0.x*b1 + p1.x*b2 + p2.x*b3 + p3.x*b4);
    float y = (p0.y*b1 + p1.y*b2 + p2.y*b3 + p3.y*b4);
	
	return DPoint(x,y);
}

void ccDrawCubicBezier(const DPoint& origin, const DPoint& control1, const DPoint& control2, const DPoint& destination, unsigned int segments)
{
    lazy_init();

    DPoint* vertices = new DPoint[segments + 1];

    float t = 0;
    for(unsigned int i = 0; i < segments; i++)
    {
        vertices[i].x = powf(1 - t, 3) * origin.x + 3.0f * powf(1 - t, 2) * t * control1.x + 3.0f * (1 - t) * t * t * control2.x + t * t * t * destination.x;
        vertices[i].y = powf(1 - t, 3) * origin.y + 3.0f * powf(1 - t, 2) * t * control1.y + 3.0f * (1 - t) * t * t * control2.y + t * t * t * destination.y;
        t += 1.0f / segments;
    }
    vertices[segments].x = destination.x;
    vertices[segments].y = destination.y;

    s_shader->use();
    s_shader->setUniformsForBuiltins();
    s_shader->setUniformLocationWith4fv(s_nColorLocation, (GLfloat*) &s_tColor.r, 1);

    GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );

    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glDrawArrays(GL_LINE_STRIP, 0, (GLsizei) segments + 1);
    CC_SAFE_DELETE_ARRAY(vertices);
    
    INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,segments+1);
}

void ccDrawColor4F( GLfloat r, GLfloat g, GLfloat b, GLfloat a )
{
    s_tColor.r = r;
    s_tColor.g = g;
    s_tColor.b = b;
    s_tColor.a = a;
}

void DPointSize( GLfloat pointSize )
{
    s_fPointSize = pointSize;

    //TODO :glPointSize( pointSize );

}

void ccDrawColor4B( GLubyte r, GLubyte g, GLubyte b, GLubyte a )
{
    s_tColor.r = r/255.0f;
    s_tColor.g = g/255.0f;
    s_tColor.b = b/255.0f;
    s_tColor.a = a/255.0f;
}

NS_CC_END
