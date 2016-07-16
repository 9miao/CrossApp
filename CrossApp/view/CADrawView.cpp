

#include "CADrawView.h"
#include "support/CAPointExtension.h"
#include "shaders/CAShaderCache.h"
#include "CCGL.h"
#include "support/CANotificationCenter.h"
#include "ccTypes.h"

NS_CC_BEGIN

// DPoint == CGPoint in 32-bits, but not in 64-bits (OS X)
// that's why the "v2f" functions are needed
static DPoint v2fzero = {0.0f,0.0f};

static inline DPoint v2f(float x, float y)
{
    DPoint ret = {x, y};
	return ret;
}

static inline DPoint v2fadd(const DPoint &v0, const DPoint &v1)
{
	return v2f(v0.x+v1.x, v0.y+v1.y);
}

static inline DPoint v2fsub(const DPoint &v0, const DPoint &v1)
{
	return v2f(v0.x-v1.x, v0.y-v1.y);
}

static inline DPoint v2fmult(const DPoint &v, float s)
{
	return v2f(v.x * s, v.y * s);
}

static inline DPoint v2fperp(const DPoint &p0)
{
	return v2f(-p0.y, p0.x);
}

static inline DPoint v2fneg(const DPoint &p0)
{
	return v2f(-p0.x, - p0.y);
}

static inline float v2fdot(const DPoint &p0, const DPoint &p1)
{
	return  p0.x * p1.x + p0.y * p1.y;
}

static inline DPoint v2fforangle(float _a_)
{
	return v2f(cosf(_a_), sinf(_a_));
}

static inline DPoint v2fnormalize(const DPoint &p)
{
	DPoint r = ccpNormalize(DPoint(p.x, p.y));
	return v2f(r.x, r.y);
}

static inline DPoint __v2f(const DPoint &v)
{
//#ifdef __LP64__
	return v2f(v.x, v.y);
// #else
// 	return * ((DPoint*) &v);
// #endif
}

static inline ccTex2F __t(const DPoint &v)
{
	return *(ccTex2F*)&v;
}

// implementation of CADrawView

CADrawView::CADrawView()
: m_uVao(0)
, m_uVbo(0)
, m_uBufferCapacity(0)
, m_nBufferCount(0)
, m_pBuffer(NULL)
, m_bDirty(false)
{
    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;
}

CADrawView::~CADrawView()
{
    free(m_pBuffer);
    m_pBuffer = NULL;
    
    glDeleteBuffers(1, &m_uVbo);
    m_uVbo = 0;
    
#if CC_TEXTURE_ATLAS_USE_VAO      
    glDeleteVertexArrays(1, &m_uVao);
    ccGLBindVAO(0);
    m_uVao = 0;
#endif

}

CADrawView* CADrawView::create()
{
    CADrawView* pRet = new CADrawView();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    
    return pRet;
}

void CADrawView::ensureCapacity(unsigned int count)
{
    if(m_nBufferCount + count > m_uBufferCapacity)
    {
		m_uBufferCapacity += MAX(m_uBufferCapacity, count);
		m_pBuffer = (ccV2F_C4B_T2F*)realloc(m_pBuffer, m_uBufferCapacity*sizeof(ccV2F_C4B_T2F));
	}
}

bool CADrawView::init()
{
    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;

    setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionLengthTexureColor));
    
    ensureCapacity(512);
    
#if CC_TEXTURE_ATLAS_USE_VAO    
    glGenVertexArrays(1, &m_uVao);
    ccGLBindVAO(m_uVao);
#endif
    
    glGenBuffers(1, &m_uVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)* m_uBufferCapacity, m_pBuffer, GL_STREAM_DRAW);
    
    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
    
    glEnableVertexAttribArray(kCCVertexAttrib_Color);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
    
    glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
#if CC_TEXTURE_ATLAS_USE_VAO 
    ccGLBindVAO(0);
#endif
    
    CHECK_GL_ERROR_DEBUG();
    
    m_bDirty = true;

    return true;
}

void CADrawView::render()
{
    if (m_bDirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*m_uBufferCapacity, m_pBuffer, GL_STREAM_DRAW);
        m_bDirty = false;
    }
#if CC_TEXTURE_ATLAS_USE_VAO     
    ccGLBindVAO(m_uVao);
#else
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
    glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
    // vertex
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
    
    // color
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
    
    // texcood
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));
#endif

    glDrawArrays(GL_TRIANGLES, 0, m_nBufferCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    CHECK_GL_ERROR_DEBUG();
}

void CADrawView::draw()
{
    CC_NODE_DRAW_SETUP();
    ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
    
    render();
}

void CADrawView::drawDot(const DPoint &pos, float radius, const CAColor4F &color)
{
    unsigned int vertex_count = 2*3;
    ensureCapacity(vertex_count);
	
	ccV2F_C4B_T2F a = {{pos.x - radius, pos.y - radius}, ccc4BFromccc4F(color), {-1.0, -1.0} };
	ccV2F_C4B_T2F b = {{pos.x - radius, pos.y + radius}, ccc4BFromccc4F(color), {-1.0,  1.0} };
	ccV2F_C4B_T2F c = {{pos.x + radius, pos.y + radius}, ccc4BFromccc4F(color), { 1.0,  1.0} };
	ccV2F_C4B_T2F d = {{pos.x + radius, pos.y - radius}, ccc4BFromccc4F(color), { 1.0, -1.0} };
	
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(m_pBuffer + m_nBufferCount);
    ccV2F_C4B_T2F_Triangle triangle0 = {a, b, c};
    ccV2F_C4B_T2F_Triangle triangle1 = {a, c, d};
	triangles[0] = triangle0;
	triangles[1] = triangle1;
	
	m_nBufferCount += vertex_count;
	
	m_bDirty = true;
}

void CADrawView::drawSegment(const DPoint &from, const DPoint &to, float radius, const CAColor4F &color)
{
    unsigned int vertex_count = 6*3;
    ensureCapacity(vertex_count);
	
	DPoint a = __v2f(from);
	DPoint b = __v2f(to);
	
	
	DPoint n = v2fnormalize(v2fperp(v2fsub(b, a)));
	DPoint t = v2fperp(n);
	
	DPoint nw = v2fmult(n, radius);
	DPoint tw = v2fmult(t, radius);
	DPoint v0 = v2fsub(b, v2fadd(nw, tw));
	DPoint v1 = v2fadd(b, v2fsub(nw, tw));
	DPoint v2 = v2fsub(b, nw);
	DPoint v3 = v2fadd(b, nw);
	DPoint v4 = v2fsub(a, nw);
	DPoint v5 = v2fadd(a, nw);
	DPoint v6 = v2fsub(a, v2fsub(nw, tw));
	DPoint v7 = v2fadd(a, v2fadd(nw, tw));
	
	
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(m_pBuffer + m_nBufferCount);
	
    ccV2F_C4B_T2F_Triangle triangles0 = {
        {v0, ccc4BFromccc4F(color), __t(v2fneg(v2fadd(n, t)))},
        {v1, ccc4BFromccc4F(color), __t(v2fsub(n, t))},
        {v2, ccc4BFromccc4F(color), __t(v2fneg(n))},
    };
	triangles[0] = triangles0;
	
    ccV2F_C4B_T2F_Triangle triangles1 = {
        {v3, ccc4BFromccc4F(color), __t(n)},
        {v1, ccc4BFromccc4F(color), __t(v2fsub(n, t))},
        {v2, ccc4BFromccc4F(color), __t(v2fneg(n))},
    };
	triangles[1] = triangles1;
	
    ccV2F_C4B_T2F_Triangle triangles2 = {
        {v3, ccc4BFromccc4F(color), __t(n)},
        {v4, ccc4BFromccc4F(color), __t(v2fneg(n))},
        {v2, ccc4BFromccc4F(color), __t(v2fneg(n))},
    };
	triangles[2] = triangles2;

    ccV2F_C4B_T2F_Triangle triangles3 = {
        {v3, ccc4BFromccc4F(color), __t(n)},
        {v4, ccc4BFromccc4F(color), __t(v2fneg(n))},
        {v5, ccc4BFromccc4F(color), __t(n) },
    };
    triangles[3] = triangles3;

    ccV2F_C4B_T2F_Triangle triangles4 = {
        {v6, ccc4BFromccc4F(color), __t(v2fsub(t, n))},
        {v4, ccc4BFromccc4F(color), __t(v2fneg(n)) },
        {v5, ccc4BFromccc4F(color), __t(n)},
    };
	triangles[4] = triangles4;

    ccV2F_C4B_T2F_Triangle triangles5 = {
        {v6, ccc4BFromccc4F(color), __t(v2fsub(t, n))},
        {v7, ccc4BFromccc4F(color), __t(v2fadd(n, t))},
        {v5, ccc4BFromccc4F(color), __t(n)},
    };
	triangles[5] = triangles5;
	
	m_nBufferCount += vertex_count;
	
	m_bDirty = true;
}

void CADrawView::drawPolygon(DPoint *verts, unsigned int count, const CAColor4F &fillColor, float borderWidth, const CAColor4F &borderColor)
{
    struct ExtrudeVerts {DPoint offset, n;};
	struct ExtrudeVerts* extrude = (struct ExtrudeVerts*)malloc(sizeof(struct ExtrudeVerts)*count);
	memset(extrude, 0, sizeof(struct ExtrudeVerts)*count);
	
	for(unsigned int i = 0; i < count; i++)
    {
		DPoint v0 = __v2f(verts[(i-1+count)%count]);
		DPoint v1 = __v2f(verts[i]);
		DPoint v2 = __v2f(verts[(i+1)%count]);
        
		DPoint n1 = v2fnormalize(v2fperp(v2fsub(v1, v0)));
		DPoint n2 = v2fnormalize(v2fperp(v2fsub(v2, v1)));
		
		DPoint offset = v2fmult(v2fadd(n1, n2), 1.0/(v2fdot(n1, n2) + 1.0));
        struct ExtrudeVerts tmp = {offset, n2};
		extrude[i] = tmp;
	}
	
	bool outline = (borderColor.a > 0.0 && borderWidth > 0.0);
	
	unsigned int triangle_count = 3*count - 2;
	unsigned int vertex_count = 3*triangle_count;
    ensureCapacity(vertex_count);
	
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(m_pBuffer + m_nBufferCount);
	ccV2F_C4B_T2F_Triangle *cursor = triangles;
	
	float inset = (outline == 0.0 ? 0.5 : 0.0);
	for(unsigned int i = 0; i < count-2; i++)
    {
		DPoint v0 = v2fsub(__v2f(verts[0  ]), v2fmult(extrude[0  ].offset, inset));
		DPoint v1 = v2fsub(__v2f(verts[i+1]), v2fmult(extrude[i+1].offset, inset));
		DPoint v2 = v2fsub(__v2f(verts[i+2]), v2fmult(extrude[i+2].offset, inset));
		
        ccV2F_C4B_T2F_Triangle tmp = {
            {v0, ccc4BFromccc4F(fillColor), __t(v2fzero)},
            {v1, ccc4BFromccc4F(fillColor), __t(v2fzero)},
            {v2, ccc4BFromccc4F(fillColor), __t(v2fzero)},
        };

		*cursor++ = tmp;
	}
	
	for(unsigned int i = 0; i < count; i++)
    {
		int j = (i+1)%count;
		DPoint v0 = __v2f(verts[i]);
		DPoint v1 = __v2f(verts[j]);
		
		DPoint n0 = extrude[i].n;
		
		DPoint offset0 = extrude[i].offset;
		DPoint offset1 = extrude[j].offset;
		
		if(outline)
        {
			DPoint inner0 = v2fsub(v0, v2fmult(offset0, borderWidth));
			DPoint inner1 = v2fsub(v1, v2fmult(offset1, borderWidth));
			DPoint outer0 = v2fadd(v0, v2fmult(offset0, borderWidth));
			DPoint outer1 = v2fadd(v1, v2fmult(offset1, borderWidth));
			
            ccV2F_C4B_T2F_Triangle tmp1 = {
                {inner0, ccc4BFromccc4F(borderColor), __t(v2fneg(n0))},
                {inner1, ccc4BFromccc4F(borderColor), __t(v2fneg(n0))},
                {outer1, ccc4BFromccc4F(borderColor), __t(n0)}
            };
			*cursor++ = tmp1;

            ccV2F_C4B_T2F_Triangle tmp2 = {
                {inner0, ccc4BFromccc4F(borderColor), __t(v2fneg(n0))},
                {outer0, ccc4BFromccc4F(borderColor), __t(n0)},
                {outer1, ccc4BFromccc4F(borderColor), __t(n0)}
            };
			*cursor++ = tmp2;
		}
        else {
			DPoint inner0 = v2fsub(v0, v2fmult(offset0, 0.5));
			DPoint inner1 = v2fsub(v1, v2fmult(offset1, 0.5));
			DPoint outer0 = v2fadd(v0, v2fmult(offset0, 0.5));
			DPoint outer1 = v2fadd(v1, v2fmult(offset1, 0.5));
			
            ccV2F_C4B_T2F_Triangle tmp1 = {
                {inner0, ccc4BFromccc4F(fillColor), __t(v2fzero)},
                {inner1, ccc4BFromccc4F(fillColor), __t(v2fzero)},
                {outer1, ccc4BFromccc4F(fillColor), __t(n0)}
            };
			*cursor++ = tmp1;

            ccV2F_C4B_T2F_Triangle tmp2 = {
                {inner0, ccc4BFromccc4F(fillColor), __t(v2fzero)},
                {outer0, ccc4BFromccc4F(fillColor), __t(n0)},
                {outer1, ccc4BFromccc4F(fillColor), __t(n0)}
            };
			*cursor++ = tmp2;
		}
	}
	
	m_nBufferCount += vertex_count;
	
	m_bDirty = true;

    free(extrude);
}

void CADrawView::clear()
{
    m_nBufferCount = 0;
    m_bDirty = true;
}

BlendFunc CADrawView::getBlendFunc() const
{
    return m_sBlendFunc;
}

void CADrawView::setBlendFunc(const BlendFunc &blendFunc)
{
    m_sBlendFunc = blendFunc;
}

/** listen the event that coming to foreground on Android
 */
void CADrawView::listenBackToForeground(CAObject *obj)
{
    init();
}

void CADrawView::setContentSize(const DSize& contentSize)
{
    CAView::setContentSize(DSize(0, 0));
}

NS_CC_END
