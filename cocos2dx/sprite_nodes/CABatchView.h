
#ifndef __CC_SPRITE_BATCH_NODE_H__
#define __CC_SPRITE_BATCH_NODE_H__

#include "CAView.h"
#include "CCProtocols.h"
#include "images/CAImageAtlas.h"
#include "ccMacros.h"
#include "cocoa/CCArray.h"

NS_CC_BEGIN

#define kDefaultSpriteBatchCapacity   29

class CC_DLL CABatchView : public CAView
{
public:
    /**
     *  @js ctor
     */
    CABatchView();
    /**
     * @js NA
     * @lua NA
     */
    ~CABatchView();

    // property
    
    // retain
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

    /** creates a CABatchView with a texture2d and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    */
    static CABatchView* createWithImage(CAImage* tex, unsigned int capacity);
    static CABatchView* createWithImage(CAImage* tex)
    {
        return CABatchView::createWithImage(tex, kDefaultSpriteBatchCapacity);
    }
    
    /** initializes a CABatchView with a texture2d and capacity of children.
    The capacity will be increased in 33% in runtime if it run out of space.
    */
    bool initWithImage(CAImage *tex, unsigned int capacity);

    bool init();

    void increaseAtlasCapacity();

    /** removes a child given a certain index. It will also cleanup the running actions depending on the cleanup parameter.
    @warning Removing a child from a CABatchView is very slow
    */
    void removeSubviewAtIndex(unsigned int index);

    void insertChild(CAView *child, unsigned int index);
    void appendChild(CAView* sprite);
    void removeSpriteFromAtlas(CAView *sprite);

    unsigned int rebuildIndexInOrder(CAView *parent, unsigned int index);
    unsigned int highestAtlasIndexInChild(CAView *sprite);
    unsigned int lowestAtlasIndexInChild(CAView *sprite);
    unsigned int atlasIndexForChild(CAView *sprite, int z);
    /* Sprites use this to start sortChildren, don't call this manually */
    void reorderBatch(bool reorder);
    // CCTextureProtocol
    virtual CAImage* getImage(void);
    virtual void setImage(CAImage *image);
    virtual void setBlendFunc(ccBlendFunc blendFunc);
    /**
     * @js NA
     */
    virtual ccBlendFunc getBlendFunc(void);

    virtual void visit(void);
    virtual void addSubview(CAView * child);
    virtual void insertSubview(CAView* subview, int z);
    virtual void reorderSubview(CAView * child, int zOrder);
    virtual void removeSubview(CAView* subview);
    virtual void removeAllSubviews();
    virtual void sortAllChildren();
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
