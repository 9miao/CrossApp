
#ifndef __CC_SPRITE_BATCH_NODE_H__
#define __CC_SPRITE_BATCH_NODE_H__

#include "CAView.h"
#include "images/CAImageCache.h"
#include "ccMacros.h"

NS_CC_BEGIN

#define kDefaultSpriteBatchCapacity   29

class CC_DLL CABatchView : public CAView
{
public:

    CABatchView();

    virtual ~CABatchView();

    inline CAImageAtlas* getImageAtlas(void) { return m_pobImageAtlas; }
    
    inline void setImageAtlas(CAImageAtlas* imageAtlas)
    { 
        if (imageAtlas != m_pobImageAtlas)
        {
            CC_SAFE_RETAIN(imageAtlas);
            CC_SAFE_RELEASE(m_pobImageAtlas);
            m_pobImageAtlas = imageAtlas;
        }
    }

    inline const CAVector<CAView*>& getDescendants(void) { return m_obDescendants; }

    static CABatchView* createWithImage(CAImage* image, unsigned int capacity);
    
    static CABatchView* createWithImage(CAImage* image)
    {
        return CABatchView::createWithImage(image, kDefaultSpriteBatchCapacity);
    }

    bool initWithImage(CAImage *image, unsigned int capacity);

    bool init();

    void increaseAtlasCapacity();

    void removeSubviewAtIndex(unsigned int index);

    void insertSubview(CAView *subview, unsigned int index);
    
    void appendSubview(CAView* subview);
    
    void removeViewFromAtlas(CAView *view);
    
    unsigned int rebuildIndexInOrder(CAView *superview, unsigned int index);
    
    unsigned int highestAtlasIndexInSubview(CAView *view);
    
    unsigned int lowestAtlasIndexInSubview(CAView *view);
    
    unsigned int atlasIndexForSubview(CAView *view, int z);

    void reorderBatch(bool reorder);

    virtual CAImage* getImage(void);
    
    virtual void setImage(CAImage *image);
    
    virtual void setBlendFunc(ccBlendFunc blendFunc);

    virtual ccBlendFunc getBlendFunc(void);

    virtual void visit(void);
    
    virtual void addSubview(CAView * child);
    
    virtual void insertSubview(CAView* subview, int z);
    
    virtual void reorderSubview(CAView * child, int zOrder);
    
    virtual void removeSubview(CAView* subview);
    
    virtual void removeAllSubviews();
    
    virtual void sortAllSubview();
    
    virtual void draw(void);

protected:

    void insertQuadFromView(CAView *view, unsigned int index);

    void updateQuadFromView(CAView *view, unsigned int index);

    CABatchView * addViewWithoutQuad(CAView* view, unsigned int z, int aTag);

private:
    
    void updateAtlasIndex(CAView* view, int* curIndex);
    
    void swap(int oldIndex, int newIndex);
    
    void updateBlendFunc();

protected:
    
    CAImageAtlas *m_pobImageAtlas;
    
    ccBlendFunc m_blendFunc;

    CAVector<CAView*> m_obDescendants;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CC_SPRITE_BATCH_NODE_H__
