
#include "CGProgressTimer.h"
#include "ccMacros.h"
#include "images/CAImageCache.h"
#include "basics/CAPointExtension.h"
#include "basics/CAApplication.h"
#include "math/TransformUtils.h"
#include "view/CADrawingPrimitives.h"
#include "renderer/ccGLStateCache.h"
#include "renderer/CCRenderer.h"
#include "renderer/CCGLProgramState.h"
NS_CC_BEGIN

#define kProgressTextureCoordsCount 4
//  kProgressTextureCoords holds points {0,1} {0,0} {1,0} {1,1} we can represent it as bits
const char kCCProgressTextureCoords = 0x4b;


CGProgressTimer::CGProgressTimer()
:m_eType(Type::RADIAL)
,m_fPercentage(0.0f)
,m_pSprite(nullptr)
,m_nVertexDataCount(0)
,m_pVertexData(nullptr)
,m_tMidpoint(0,0)
,m_tBarChangeRate(0,0)
,m_bReverseDirection(false)
{}

CGProgressTimer* CGProgressTimer::create(CGSprite* sp)
{
    CGProgressTimer *pProgressTimer = new CGProgressTimer();
    if (pProgressTimer->initWithSprite(sp))
    {
        pProgressTimer->autorelease();
    }
    else
    {
        delete pProgressTimer;
        pProgressTimer = NULL;
    }
    
    return pProgressTimer;
}

bool CGProgressTimer::initWithSprite(CGSprite* sp)
{
    setPercentage(0.0f);
    m_pVertexData = NULL;
    m_nVertexDataCount = 0;
    
    setAnchorPoint(DPoint(0.5f,0.5f));
    m_eType = Type::RADIAL;
    m_bReverseDirection = false;
    setMidpoint(DPoint(0.5f, 0.5f));
    setBarChangeRate(DPoint(1,1));
    setSprite(sp);
    // shader program
    setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
    return true;
}

CGProgressTimer::~CGProgressTimer(void)
{
    CC_SAFE_FREE(m_pVertexData);
    CC_SAFE_RELEASE(m_pSprite);
}

void CGProgressTimer::setPercentage(float fPercentage)
{
    if (m_fPercentage != fPercentage)
    {
        m_fPercentage = clampf(fPercentage, 0, 100);
        updateProgress();
    }
}

void CGProgressTimer::setSprite(CGSprite* pSprite)
{
    if (m_pSprite != pSprite)
    {
        CC_SAFE_RETAIN(pSprite);
        CC_SAFE_RELEASE(m_pSprite);
        m_pSprite = pSprite;
        setContentSize(m_pSprite->getContentSize());
        
        //    Every time we set a new sprite, we free the current vertex data
        if (m_pVertexData)
        {
            CC_SAFE_FREE(m_pVertexData);
            m_nVertexDataCount = 0;
        }
    }
}

void CGProgressTimer::setType(Type type)
{
    if (type != m_eType)
    {
        //    release all previous information
        if (m_pVertexData)
        {
            CC_SAFE_FREE(m_pVertexData);
            m_pVertexData = NULL;
            m_nVertexDataCount = 0;
        }
        
        m_eType = type;
    }
}

void CGProgressTimer::setReverseProgress(bool reverse)
{
    if( m_bReverseDirection != reverse ) {
        m_bReverseDirection = reverse;
        
        //    release all previous information
        CC_SAFE_FREE(m_pVertexData);
        m_nVertexDataCount = 0;
    }
}

void CGProgressTimer::setColor(const CAColor4B& color)
{
    m_pSprite->setColor(color);
    updateColor();
}

const CAColor4B& CGProgressTimer::getColor() const
{
    return m_pSprite->getColor();
}

void CGProgressTimer::setAlpha(float alpha)
{
    m_pSprite->setAlpha(alpha);
    updateColor();
}

float CGProgressTimer::getAlpha() const
{
    return m_pSprite->getAlpha();
}

// Interval

///
//    @returns the vertex position from the texture coordinate
///
ccTex2F CGProgressTimer::textureCoordFromAlphaPoint(DPoint alpha)
{
    ccTex2F ret = {0.0f, 0.0f};
    if (!m_pSprite) {
        return ret;
    }
    ccV3F_C4B_T2F_Quad quad = m_pSprite->getQuad();
    DPoint min = DPoint(quad.bl.texCoords.u,quad.bl.texCoords.v);
    DPoint max = DPoint(quad.tr.texCoords.u,quad.tr.texCoords.v);
    //  Fix bug #1303 so that progress timer handles sprite frame texture rotation
    if (m_pSprite->isImageRectRotated()) {
        CC_SWAP(alpha.x, alpha.y, float);
    }
    return tex2(min.x * (1.f - alpha.x) + max.x * alpha.x, min.y * (1.f - alpha.y) + max.y * alpha.y);
}

DPoint CGProgressTimer::vertexFromAlphaPoint(DPoint alpha)
{
    DPoint ret = {0.0f, 0.0f};
    if (!m_pSprite) {
        return ret;
    }
    ccV3F_C4B_T2F_Quad quad = m_pSprite->getQuad();
    DPoint min = DPoint(quad.bl.vertices.x,quad.bl.vertices.y);
    DPoint max = DPoint(quad.tr.vertices.x,quad.tr.vertices.y);
    ret.x = min.x * (1.f - alpha.x) + max.x * alpha.x;
    ret.y = min.y * (1.f - alpha.y) + max.y * alpha.y;
    return ret;
}

void CGProgressTimer::updateColor(void)
{
    if (!m_pSprite) {
        return;
    }
    
    if (m_pVertexData)
    {
        CAColor4B sc = m_pSprite->getQuad().tl.colors;
        for (int i = 0; i < m_nVertexDataCount; ++i)
        {
            m_pVertexData[i].colors = sc;
        }
    }
}

void CGProgressTimer::updateProgress(void)
{
    switch (m_eType)
    {
        case Type::RADIAL:
            updateRadial();
            break;
        case Type::BAR:
            updateBar();
            break;
        default:
            break;
    }
}

void CGProgressTimer::setAnchorPoint(DPoint anchorPoint)
{
    CAView::setAnchorPoint(anchorPoint);
}

DPoint CGProgressTimer::getMidpoint(void)
{
    return m_tMidpoint;
}

void CGProgressTimer::setMidpoint(DPoint midPoint)
{
    m_tMidpoint = ccpClamp(midPoint, DPointZero, DPoint(1,1));
}

///
//    Update does the work of mapping the texture onto the triangles
//    It now doesn't occur the cost of free/alloc data every update cycle.
//    It also only changes the percentage point but no other points if they have not
//    been modified.
//
//    It now deals with flipped texture. If you run into this problem, just use the
//    sprite property and enable the methods flipX, flipY.
///
void CGProgressTimer::updateRadial(void)
{
    if (!m_pSprite) {
        return;
    }
    float alpha = m_fPercentage / 100.f;
    
    float angle = 2.f*((float)M_PI) * ( m_bReverseDirection ? alpha : 1.0f - alpha);
    
    //    We find the vector to do a hit detection based on the percentage
    //    We know the first vector is the one @ 12 o'clock (top,mid) so we rotate
    //    from that by the progress angle around the m_tMidpoint pivot
    DPoint topMid = DPoint(m_tMidpoint.x, 1.f);
    DPoint percentagePt = ccpRotateByAngle(topMid, m_tMidpoint, angle);
    
    
    int index = 0;
    DPoint hit = DPointZero;
    
    if (alpha == 0.f) {
        //    More efficient since we don't always need to check intersection
        //    If the alpha is zero then the hit point is top mid and the index is 0.
        hit = topMid;
        index = 0;
    } else if (alpha == 1.f) {
        //    More efficient since we don't always need to check intersection
        //    If the alpha is one then the hit point is top mid and the index is 4.
        hit = topMid;
        index = 4;
    } else {
        //    We run a for loop checking the edges of the texture to find the
        //    intersection point
        //    We loop through five points since the top is split in half
        
        float min_t = FLT_MAX;
        
        for (int i = 0; i <= kProgressTextureCoordsCount; ++i) {
            int pIndex = (i + (kProgressTextureCoordsCount - 1))%kProgressTextureCoordsCount;
            
            DPoint edgePtA = boundaryTexCoord(i % kProgressTextureCoordsCount);
            DPoint edgePtB = boundaryTexCoord(pIndex);
            
            //    Remember that the top edge is split in half for the 12 o'clock position
            //    Let's deal with that here by finding the correct endpoints
            if(i == 0){
                edgePtB = ccpLerp(edgePtA, edgePtB, 1-m_tMidpoint.x);
            } else if(i == 4){
                edgePtA = ccpLerp(edgePtA, edgePtB, 1-m_tMidpoint.x);
            }
            
            //    s and t are returned by ccpLineIntersect
            float s = 0, t = 0;
            if(ccpLineIntersect(edgePtA, edgePtB, m_tMidpoint, percentagePt, &s, &t))
            {
                
                //    Since our hit test is on rays we have to deal with the top edge
                //    being in split in half so we have to test as a segment
                if ((i == 0 || i == 4)) {
                    //    s represents the point between edgePtA--edgePtB
                    if (!(0.f <= s && s <= 1.f)) {
                        continue;
                    }
                }
                //    As long as our t isn't negative we are at least finding a
                //    correct hitpoint from m_tMidpoint to percentagePt.
                if (t >= 0.f) {
                    //    Because the percentage line and all the texture edges are
                    //    rays we should only account for the shortest intersection
                    if (t < min_t) {
                        min_t = t;
                        index = i;
                    }
                }
            }
        }
        
        //    Now that we have the minimum magnitude we can use that to find our intersection
        hit = ccpAdd(m_tMidpoint, ccpMult(ccpSub(percentagePt, m_tMidpoint),min_t));
        
    }
    
    
    //    The size of the vertex data is the index from the hitpoint
    //    the 3 is for the m_tMidpoint, 12 o'clock point and hitpoint position.
    
    bool sameIndexCount = true;
    if(m_nVertexDataCount != index + 3){
        sameIndexCount = false;
        CC_SAFE_FREE(m_pVertexData);
        m_nVertexDataCount = 0;
    }
    
    
    if(!m_pVertexData) {
        m_nVertexDataCount = index + 3;
        m_pVertexData = (ccV2F_C4B_T2F*)malloc(m_nVertexDataCount * sizeof(ccV2F_C4B_T2F));
        CCAssert( m_pVertexData, "CGProgressTimer. Not enough memory");
    }
    updateColor();
    
    if (!sameIndexCount) {
        
        //    First we populate the array with the m_tMidpoint, then all
        //    vertices/texcoords/colors of the 12 'o clock start and edges and the hitpoint
        m_pVertexData[0].texCoords = textureCoordFromAlphaPoint(m_tMidpoint);
        m_pVertexData[0].vertices = vertexFromAlphaPoint(m_tMidpoint);
        
        m_pVertexData[1].texCoords = textureCoordFromAlphaPoint(topMid);
        m_pVertexData[1].vertices = vertexFromAlphaPoint(topMid);
        
        for(int i = 0; i < index; ++i){
            DPoint alphaPoint = boundaryTexCoord(i);
            m_pVertexData[i+2].texCoords = textureCoordFromAlphaPoint(alphaPoint);
            m_pVertexData[i+2].vertices = vertexFromAlphaPoint(alphaPoint);
        }
    }
    
    //    hitpoint will go last
    m_pVertexData[m_nVertexDataCount - 1].texCoords = textureCoordFromAlphaPoint(hit);
    m_pVertexData[m_nVertexDataCount - 1].vertices = vertexFromAlphaPoint(hit);
    
}

///
//    Update does the work of mapping the texture onto the triangles for the bar
//    It now doesn't occur the cost of free/alloc data every update cycle.
//    It also only changes the percentage point but no other points if they have not
//    been modified.
//
//    It now deals with flipped texture. If you run into this problem, just use the
//    sprite property and enable the methods flipX, flipY.
///
void CGProgressTimer::updateBar(void)
{
    if (!m_pSprite) {
        return;
    }
    float alpha = m_fPercentage / 100.0f;
    DPoint alphaOffset = ccpMult(DPoint(1.0f * (1.0f - m_tBarChangeRate.x) + alpha * m_tBarChangeRate.x, 1.0f * (1.0f - m_tBarChangeRate.y) + alpha * m_tBarChangeRate.y), 0.5f);
    DPoint min = ccpSub(m_tMidpoint, alphaOffset);
    DPoint max = ccpAdd(m_tMidpoint, alphaOffset);
    
    if (min.x < 0.f) {
        max.x += -min.x;
        min.x = 0.f;
    }
    
    if (max.x > 1.f) {
        min.x -= max.x - 1.f;
        max.x = 1.f;
    }
    
    if (min.y < 0.f) {
        max.y += -min.y;
        min.y = 0.f;
    }
    
    if (max.y > 1.f) {
        min.y -= max.y - 1.f;
        max.y = 1.f;
    }
    
    
    if (!m_bReverseDirection) {
        if(!m_pVertexData) {
            m_nVertexDataCount = 4;
            m_pVertexData = (ccV2F_C4B_T2F*)malloc(m_nVertexDataCount * sizeof(ccV2F_C4B_T2F));
            CCAssert( m_pVertexData, "CGProgressTimer. Not enough memory");
        }
        //    TOPLEFT
        m_pVertexData[0].texCoords = textureCoordFromAlphaPoint(DPoint(min.x,max.y));
        m_pVertexData[0].vertices = vertexFromAlphaPoint(DPoint(min.x,max.y));
        
        //    BOTLEFT
        m_pVertexData[1].texCoords = textureCoordFromAlphaPoint(DPoint(min.x,min.y));
        m_pVertexData[1].vertices = vertexFromAlphaPoint(DPoint(min.x,min.y));
        
        //    TOPRIGHT
        m_pVertexData[2].texCoords = textureCoordFromAlphaPoint(DPoint(max.x,max.y));
        m_pVertexData[2].vertices = vertexFromAlphaPoint(DPoint(max.x,max.y));
        
        //    BOTRIGHT
        m_pVertexData[3].texCoords = textureCoordFromAlphaPoint(DPoint(max.x,min.y));
        m_pVertexData[3].vertices = vertexFromAlphaPoint(DPoint(max.x,min.y));
    } else {
        if(!m_pVertexData) {
            m_nVertexDataCount = 8;
            m_pVertexData = (ccV2F_C4B_T2F*)malloc(m_nVertexDataCount * sizeof(ccV2F_C4B_T2F));
            CCAssert( m_pVertexData, "CGProgressTimer. Not enough memory");
            //    TOPLEFT 1
            m_pVertexData[0].texCoords = textureCoordFromAlphaPoint(DPoint(0,1));
            m_pVertexData[0].vertices = vertexFromAlphaPoint(DPoint(0,1));
            
            //    BOTLEFT 1
            m_pVertexData[1].texCoords = textureCoordFromAlphaPoint(DPoint(0,0));
            m_pVertexData[1].vertices = vertexFromAlphaPoint(DPoint(0,0));
            
            //    TOPRIGHT 2
            m_pVertexData[6].texCoords = textureCoordFromAlphaPoint(DPoint(1,1));
            m_pVertexData[6].vertices = vertexFromAlphaPoint(DPoint(1,1));
            
            //    BOTRIGHT 2
            m_pVertexData[7].texCoords = textureCoordFromAlphaPoint(DPoint(1,0));
            m_pVertexData[7].vertices = vertexFromAlphaPoint(DPoint(1,0));
        }
        
        //    TOPRIGHT 1
        m_pVertexData[2].texCoords = textureCoordFromAlphaPoint(DPoint(min.x,max.y));
        m_pVertexData[2].vertices = vertexFromAlphaPoint(DPoint(min.x,max.y));
        
        //    BOTRIGHT 1
        m_pVertexData[3].texCoords = textureCoordFromAlphaPoint(DPoint(min.x,min.y));
        m_pVertexData[3].vertices = vertexFromAlphaPoint(DPoint(min.x,min.y));
        
        //    TOPLEFT 2
        m_pVertexData[4].texCoords = textureCoordFromAlphaPoint(DPoint(max.x,max.y));
        m_pVertexData[4].vertices = vertexFromAlphaPoint(DPoint(max.x,max.y));
        
        //    BOTLEFT 2
        m_pVertexData[5].texCoords = textureCoordFromAlphaPoint(DPoint(max.x,min.y));
        m_pVertexData[5].vertices = vertexFromAlphaPoint(DPoint(max.x,min.y));
    }
    updateColor();
}

DPoint CGProgressTimer::boundaryTexCoord(char index)
{
    if (index < kProgressTextureCoordsCount) {
        if (m_bReverseDirection) {
            return DPoint((kCCProgressTextureCoords>>(7-(index<<1)))&1,(kCCProgressTextureCoords>>(7-((index<<1)+1)))&1);
        } else {
            return DPoint((kCCProgressTextureCoords>>((index<<1)+1))&1,(kCCProgressTextureCoords>>(index<<1))&1);
        }
    }
    return DPointZero;
}

void CGProgressTimer::onDraw(const Mat4 &transform, uint32_t flags)
{
    getShaderProgram()->use();
    getShaderProgram()->setUniformsForBuiltins(transform);
    
    GL::blendFunc( m_pSprite->getBlendFunc().src, m_pSprite->getBlendFunc().dst );
    
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX );
    
    GL::bindTexture2D( m_pSprite->getImage()->getName() );
    
    glVertexAttribPointer( GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(m_pVertexData[0]) , &m_pVertexData[0].vertices);
    glVertexAttribPointer( GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(m_pVertexData[0]), &m_pVertexData[0].texCoords);
    glVertexAttribPointer( GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(m_pVertexData[0]), &m_pVertexData[0].colors);
    
    if(m_eType == Type::RADIAL)
    {
        glDrawArrays(GL_TRIANGLE_FAN, 0, m_nVertexDataCount);
        
        do {
            auto __renderer__ = CAApplication::getApplication()->getRenderer();
            __renderer__->addDrawnBatches(1);
            __renderer__->addDrawnVertices(m_nVertexDataCount);
        } while(0);
        
    }
    else if (m_eType == Type::BAR)
    {
        if (!m_bReverseDirection)
        {
            glDrawArrays(GL_TRIANGLE_STRIP, 0, m_nVertexDataCount);
            
            do {
                auto __renderer__ = CAApplication::getApplication()->getRenderer();
                __renderer__->addDrawnBatches(1);
                __renderer__->addDrawnVertices(m_nVertexDataCount);
            } while(0);
        }
        else
        {
            glDrawArrays(GL_TRIANGLE_STRIP, 0, m_nVertexDataCount/2);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, m_nVertexDataCount/2);
            // 2 draw calls
            do {
                auto __renderer__ = CAApplication::getApplication()->getRenderer();
                __renderer__->addDrawnBatches(1);
                __renderer__->addDrawnVertices(m_nVertexDataCount);
            } while(0);
        }
    }
}

void CGProgressTimer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if( ! m_pVertexData || ! m_pSprite)
        return;
    
    m_tCustomCommand.init(m_nZOrder, transform, flags);
    m_tCustomCommand.func = std::bind(&CGProgressTimer::onDraw, this, transform, flags);
    renderer->addCommand(&m_tCustomCommand);
}
NS_CC_END
