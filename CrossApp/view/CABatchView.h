
#ifndef __CC_SPRITE_BATCH_NODE_H__
#define __CC_SPRITE_BATCH_NODE_H__

#include "CAView.h"
#include "images/CAImageCache.h"
#include "ccMacros.h"
#include "cocoa/CCArray.h"

NS_CC_BEGIN

#define kDefaultSpriteBatchCapacity   29

class CC_DLL CABatchView : public CAView
{
public:

    CABatchView();

    ~CABatchView();

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

    inline CCArray* getDescendants(void) { return m_pobDescendants; }

    static CABatchView* createWithImage(CAImage* tex, unsigned int capacity);
    
    static CABatchView* createWithImage(CAImage* tex)
    {
        return CABatchView::createWithImage(tex, kDefaultSpriteBatchCapacity);
    }

    bool initWithImage(CAImage *tex, unsigned int capacity);

    bool init();

    void increaseAtlasCapacity();

    void removeSubviewAtIndex(unsigned int index);

    void insertChild(CAView *child, unsigned int index);
    
    void appendChild(CAView* sprite);
    
    void removeSpriteFromAtlas(CAView *sprite);
    
    unsigned int rebuildIndexInOrder(CAView *parent, unsigned int index);
    
    unsigned int highestAtlasIndexInChild(CAView *sprite);
    
    unsigned int lowestAtlasIndexInChild(CAView *sprite);
    
    unsigned int atlasIndexForChild(CAView *sprite, int z);

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

    void insertQuadFromSprite(CAView *sprite, unsigned int index);

    void updateQuadFromSprite(CAView *sprite, unsigned int index);

    CABatchView * addSpriteWithoutQuad(CAView*child, unsigned int z, int aTag);

private:
    
    void updateAtlasIndex(CAView* sprite, int* curIndex);
    
    void swap(int oldIndex, int newIndex);
    
    void updateBlendFunc();

protected:
    
    CAImageAtlas *m_pobImageAtlas;
    
    ccBlendFunc m_blendFunc;

    CCArray* m_pobDescendants;
};

// end of sprite_nodes group
/// @}

NS_CC_END

#endif // __CC_SPRITE_BATCH_NODE_H__
