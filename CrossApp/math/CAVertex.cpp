

#include "CAVertex.h"
#include "basics/CAPointExtension.h"
#include "ccMacros.h"

NS_CC_BEGIN

void ccVertexLineToPolygon(DPoint *points, float stroke, DPoint *vertices, unsigned int offset, unsigned int nuPoints)
{
    nuPoints += offset;
    if(nuPoints<=1) return;
    
    stroke *= 0.5f;
    
    unsigned int idx;
    unsigned int nuPointsMinus = nuPoints-1;
    
    for(unsigned int i = offset; i<nuPoints; i++)
    {
        idx = i*2;
        DPoint p1 = points[i];
        DPoint perpVector;
        
        if(i == 0)
            perpVector = ccpPerp(ccpNormalize(ccpSub(p1, points[i+1])));
        else if(i == nuPointsMinus)
            perpVector = ccpPerp(ccpNormalize(ccpSub(points[i-1], p1)));
        else
        {
            DPoint p2 = points[i+1];
            DPoint p0 = points[i-1];
            
            DPoint p2p1 = ccpNormalize(ccpSub(p2, p1));
            DPoint p0p1 = ccpNormalize(ccpSub(p0, p1));
            
            // Calculate angle between vectors
            float angle = acosf(ccpDot(p2p1, p0p1));
            
            if(angle < CC_DEGREES_TO_RADIANS(70))
                perpVector = ccpPerp(ccpNormalize(ccpMidpoint(p2p1, p0p1)));
            else if(angle < CC_DEGREES_TO_RADIANS(170))
                perpVector = ccpNormalize(ccpMidpoint(p2p1, p0p1));
            else
                perpVector = ccpPerp(ccpNormalize(ccpSub(p2, p0)));
        }
        perpVector = ccpMult(perpVector, stroke);
        
        vertices[idx] = DPoint(p1.x+perpVector.x, p1.y+perpVector.y);
        vertices[idx+1] = DPoint(p1.x-perpVector.x, p1.y-perpVector.y);
        
    }
    
    // Validate vertexes
    offset = (offset==0) ? 0 : offset-1;
    for(unsigned int i = offset; i<nuPointsMinus; i++)
    {
        idx = i*2;
        const unsigned int idx1 = idx+2;
        
        DPoint p1 = vertices[idx];
        DPoint p2 = vertices[idx+1];
        DPoint p3 = vertices[idx1];
        DPoint p4 = vertices[idx1+1];
        
        float s;
        //BOOL fixVertex = !ccpLineIntersect(DPoint(p1.x, p1.y), DPoint(p4.x, p4.y), DPoint(p2.x, p2.y), DPoint(p3.x, p3.y), &s, &t);
        bool fixVertex = !ccVertexLineIntersect(p1.x, p1.y, p4.x, p4.y, p2.x, p2.y, p3.x, p3.y, &s);
        if(!fixVertex)
            if (s<0.0f || s>1.0f)
                fixVertex = true;
        
        if(fixVertex)
        {
            vertices[idx1] = p4;
            vertices[idx1+1] = p3;
        }
    }
}

bool ccVertexLineIntersect(float Ax, float Ay, float Bx, float By, float Cx, float Cy, float Dx, float Dy, float *T)
{
    float  distAB, theCos, theSin, newX;
    
    // FAIL: Line undefined
    if ((Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy)) return false;
    
    //  Translate system to make A the origin
    Bx-=Ax; By-=Ay;
    Cx-=Ax; Cy-=Ay;
    Dx-=Ax; Dy-=Ay;
    
    // Length of segment AB
    distAB = sqrtf(Bx*Bx+By*By);
    
    // Rotate the system so that point B is on the positive X axis.
    theCos = Bx/distAB;
    theSin = By/distAB;
    newX = Cx*theCos+Cy*theSin;
    Cy  = Cy*theCos-Cx*theSin; Cx = newX;
    newX = Dx*theCos+Dy*theSin;
    Dy  = Dy*theCos-Dx*theSin; Dx = newX;
    
    // FAIL: Lines are parallel.
    if (Cy == Dy) return false;
    
    // Discover the relative position of the intersection in the line AB
    *T = (Dx+(Cx-Dx)*Dy/(Dy-Cy))/distAB;
    
    // Success.
    return true;
}

NS_CC_END
