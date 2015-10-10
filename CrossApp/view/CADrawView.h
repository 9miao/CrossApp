

#ifndef __CADrawViewS_CCDRAW_NODE_H__
#define __CADrawViewS_CCDRAW_NODE_H__

#include "view/CAView.h"
#include "ccTypes.h"

NS_CC_BEGIN

/** CADrawView
 Node that draws dots, segments and polygons.
 Faster than the "drawing primitives" since they it draws everything in one single batch.
 
 @since v2.1
 @lua NA
 */
class CC_DLL CADrawView : public CAView
{
protected:
    GLuint      m_uVao;
    GLuint      m_uVbo;
    
    unsigned int    m_uBufferCapacity;
    GLsizei         m_nBufferCount;
    ccV2F_C4B_T2F   *m_pBuffer;
    
    ccBlendFunc     m_sBlendFunc;
    
    bool            m_bDirty;
    
public:
    static CADrawView* create();
    virtual ~CADrawView();
    
    virtual bool init();
    virtual void draw();
    
    /** draw a dot at a position, with a given radius and color */
    void drawDot(const DPoint &pos, float radius, const CAColor4F &color);
    
    /** draw a segment with a radius and color */
    void drawSegment(const DPoint &from, const DPoint &to, float radius, const CAColor4F &color);
    
    /** draw a polygon with a fill color and line color 
     * @code
     * when this funciton bound to js,the input params are changed
     * js:var drawPolygon(var verts, var fillColor,var borderWidth,var borderColor)
     * @endcode
     */
    void drawPolygon(DPoint *verts, unsigned int count, const CAColor4F &fillColor, float borderWidth, const CAColor4F &borderColor);
    
    /** Clear the geometry in the node's buffer. */
    void clear();
    /**
     * @js NA
     */
    ccBlendFunc getBlendFunc() const;
    /**
     * @code
     * when this function bound to js ,the input param is change
     * js:var setBlendFunc(var src,var dst)
     * @endcode
     */
    void setBlendFunc(const ccBlendFunc &blendFunc);
    
    CADrawView();

    /** listen the event that coming to foreground on Android  
     * @js NA
     */
    void listenBackToForeground(CAObject *obj);
private:
    virtual void setContentSize(const DSize& contentSize);
    void ensureCapacity(unsigned int count);
    void render();
};

CC_DEPRECATED_ATTRIBUTE typedef CADrawView CCDrawNode;

NS_CC_END

#endif // __CADrawViewS_CCDRAW_NODE_H__
