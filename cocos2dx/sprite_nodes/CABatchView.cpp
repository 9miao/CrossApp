
#include "CABatchView.h"
#include "ccConfig.h"
#include "CAView.h"
#include "effects/CCGrid.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "images/CAImageCache.h"
#include "support/CCPointExtension.h"
#include "shaders/CCShaderCache.h"
#include "shaders/CCGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "CCDirector.h"
#include "support/TransformUtils.h"
#include "support/CCProfiling.h"
// external
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN

/*
* creation with CAImage
*/

CABatchView* CABatchView::createWithImage(CAImage* tex, unsigned int capacity/* = kDefaultSpriteBatchCapacity*/)
{
    CABatchView *batchNode = new CABatchView();
    batchNode->initWithImage(tex, capacity);
    batchNode->autorelease();

    return batchNode;
}



/*
* init with CAImage
*/
bool CABatchView::initWithImage(CAImage *tex, unsigned int capacity)
{
    m_blendFunc.src = CC_BLEND_SRC;
    m_blendFunc.dst = CC_BLEND_DST;
    m_pobImageAtlas = new CAImageAtlas();

    if (0 == capacity)
    {
        capacity = kDefaultSpriteBatchCapacity;
    }
    
    m_pobImageAtlas->initWithImage(tex, capacity);

    updateBlendFunc();

    // no lazy alloc in this node
    m_pSubviews = new CCArray();
    m_pSubviews->initWithCapacity(capacity);

    m_pobDescendants = new CCArray();
    m_pobDescendants->initWithCapacity(capacity);

    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
    return true;
}

bool CABatchView::init()
{
    CAImage * texture = new CAImage();
    texture->autorelease();
    return this->initWithImage(texture, 0);
}

CABatchView::CABatchView()
: m_pobImageAtlas(NULL)
, m_pobDescendants(NULL)
{
}

CABatchView::~CABatchView()
{
    CC_SAFE_RELEASE(m_pobImageAtlas);
    CC_SAFE_RELEASE(m_pobDescendants);
}

// override visit
// don't call visit on it's children
void CABatchView::visit(void)
{
    CC_PROFILER_START_CATEGORY(kCCProfilerCategoryBatchSprite, "CABatchView - visit");

    // CAREFUL:
    // This visit is almost identical to CocosNode#visit
    // with the exception that it doesn't call visit on it's children
    //
    // The alternative is to have a void CAView#visit, but
    // although this is less maintainable, is faster
    //
    if (! m_bVisible)
    {
        return;
    }

    kmGLPushMatrix();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->beforeDraw();
        transformAncestors();
    }

    sortAllChildren();
    transform();

    draw();

    if (m_pGrid && m_pGrid->isActive())
    {
        m_pGrid->afterDraw(this);
    }

    kmGLPopMatrix();
    setOrderOfArrival(0);

    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategoryBatchSprite, "CABatchView - visit");

}

void CABatchView::insertSubview(CAView *subview, int zOrder)
{

    CAView::insertSubview(subview, zOrder);
    appendChild(dynamic_cast<CAView*>(subview));
}

void CABatchView::addSubview(CAView *subview)
{
    CAView::addSubview(subview);
}

// override reorderChild
void CABatchView::reorderSubview(CAView * subview, int zOrder)
{
    if (zOrder == subview->getZOrder())
    {
        return;
    }

    //set the z-order and sort later
    CAView::reorderSubview(subview, zOrder);
}

// override remove child
void CABatchView::removeSubview(CAView* subview)
{
    CAView *pSprite = (CAView*)(subview);

    // explicit null handling
    if (pSprite == NULL)
    {
        return;
    }


    // cleanup before removing
    removeSpriteFromAtlas(pSprite);

    CAView::removeSubview(pSprite);
}

void CABatchView::removeSubviewAtIndex(unsigned int uIndex)
{
    removeSubview((CAView*)(m_pSubviews->objectAtIndex(uIndex)));
}

void CABatchView::removeAllSubviews()
{
    // Invalidate atlas index. issue #569
    // useSelfRender should be performed on all descendants. issue #1216
    //arrayMakeObjectsPerformSelectorWithObject(m_pobDescendants, setBatch, NULL, CAView*);

    CAView::removeAllSubviews();

    //m_pobDescendants->removeAllObjects();
    m_pobImageAtlas->removeAllQuads();
}

//override sortAllChildren
void CABatchView::sortAllChildren()
{
    if (m_bReorderChildDirty)
    {
        int i = 0,j = 0,length = m_pSubviews->data->num;
        CAView ** x = (CAView**)m_pSubviews->data->arr;
        CAView *tempItem = NULL;

        //insertion sort
        for(i=1; i<length; i++)
        {
            tempItem = x[i];
            j = i-1;

            //continue moving element downwards while zOrder is smaller or when zOrder is the same but orderOfArrival is smaller
            while(j>=0 && ( tempItem->getZOrder() < x[j]->getZOrder() || ( tempItem->getZOrder() == x[j]->getZOrder() && tempItem->getOrderOfArrival() < x[j]->getOrderOfArrival() ) ) )
            {
                x[j+1] = x[j];
                j--;
            }

            x[j+1] = tempItem;
        }

        //sorted now check all children
        if (m_pSubviews->count() > 0)
        {
            //first sort all children recursively based on zOrder
            arrayMakeObjectsPerformSelector(m_pSubviews, sortAllSubviews, CAView*);

            int index=0;

            CCObject* pObj = NULL;
            //fast dispatch, give every child a new atlasIndex based on their relative zOrder (keep parent -> child relations intact)
            // and at the same time reorder descendants and the quads to the right index
            CCARRAY_FOREACH(m_pSubviews, pObj)
            {
                CAView* pChild = (CAView*)pObj;
                updateAtlasIndex(pChild, &index);
            }
        }

        m_bReorderChildDirty=false;
    }
}

void CABatchView::updateAtlasIndex(CAView* sprite, int* curIndex)
{
    unsigned int count = 0;
    CCArray* pArray = sprite->getSubviews();
    if (pArray != NULL)
    {
        count = pArray->count();
    }
    
    int oldIndex = 0;

    if( count == 0 )
    {
        oldIndex = sprite->getAtlasIndex();
        sprite->setAtlasIndex(*curIndex);
        sprite->setOrderOfArrival(0);
        if (oldIndex != *curIndex){
            swap(oldIndex, *curIndex);
        }
        (*curIndex)++;
    }
    else
    {
        bool needNewIndex=true;

        if (((CAView*) (pArray->data->arr[0]))->getZOrder() >= 0)
        {
            //all children are in front of the parent
            oldIndex = sprite->getAtlasIndex();
            sprite->setAtlasIndex(*curIndex);
            sprite->setOrderOfArrival(0);
            if (oldIndex != *curIndex)
            {
                swap(oldIndex, *curIndex);
            }
            (*curIndex)++;

            needNewIndex = false;
        }

        CCObject* pObj = NULL;
        CCARRAY_FOREACH(pArray,pObj)
        {
            CAView* child = (CAView*)pObj;
            if (needNewIndex && child->getZOrder() >= 0)
            {
                oldIndex = sprite->getAtlasIndex();
                sprite->setAtlasIndex(*curIndex);
                sprite->setOrderOfArrival(0);
                if (oldIndex != *curIndex) {
                    this->swap(oldIndex, *curIndex);
                }
                (*curIndex)++;
                needNewIndex = false;

            }

            updateAtlasIndex(child, curIndex);
        }

        if (needNewIndex)
        {//all children have a zOrder < 0)
            oldIndex=sprite->getAtlasIndex();
            sprite->setAtlasIndex(*curIndex);
            sprite->setOrderOfArrival(0);
            if (oldIndex!=*curIndex) {
                swap(oldIndex, *curIndex);
            }
            (*curIndex)++;
        }
    }
}

void CABatchView::swap(int oldIndex, int newIndex)
{
    CCObject** x = m_pobDescendants->data->arr;
    ccV3F_C4B_T2F_Quad* quads = m_pobImageAtlas->getQuads();

    CCObject* tempItem = x[oldIndex];
    ccV3F_C4B_T2F_Quad tempItemQuad=quads[oldIndex];

    //update the index of other swapped item
    ((CAView*) x[newIndex])->setAtlasIndex(oldIndex);

    x[oldIndex]=x[newIndex];
    quads[oldIndex]=quads[newIndex];
    x[newIndex]=tempItem;
    quads[newIndex]=tempItemQuad;
}

void CABatchView::reorderBatch(bool reorder)
{
    m_bReorderChildDirty=reorder;
}

// draw
void CABatchView::draw(void)
{
    CC_PROFILER_START("CABatchView - draw");

    // Optimization: Fast Dispatch
    if( m_pobImageAtlas->getTotalQuads() == 0 )
    {
        return;
    }

    CC_NODE_DRAW_SETUP();

    arrayMakeObjectsPerformSelector(m_pSubviews, updateTransform, CAView*);

    ccGLBlendFunc( m_blendFunc.src, m_blendFunc.dst );

    m_pobImageAtlas->drawQuads();

    CC_PROFILER_STOP("CABatchView - draw");
}

void CABatchView::increaseAtlasCapacity(void)
{
    // if we're going beyond the current TextureAtlas's capacity,
    // all the previously initialized sprites will need to redo their texture coords
    // this is likely computationally expensive
    unsigned int quantity = (m_pobImageAtlas->getCapacity() + 1) * 4 / 3;

    CCLOG("cocos2d: CABatchView: resizing TextureAtlas capacity from [%lu] to [%lu].",
        (long)m_pobImageAtlas->getCapacity(),
        (long)quantity);

    if (! m_pobImageAtlas->resizeCapacity(quantity))
    {
        // serious problems
        CCLOGWARN("cocos2d: WARNING: Not enough memory to resize the atlas");
        CCAssert(false, "Not enough memory to resize the atlas");
    }
}

unsigned int CABatchView::rebuildIndexInOrder(CAView *pobParent, unsigned int uIndex)
{
    CCArray *pChildren = pobParent->getSubviews();

    if (pChildren && pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(pChildren, pObject)
        {
            CAView* pChild = (CAView*) pObject;
            if (pChild && (pChild->getZOrder() < 0))
            {
                uIndex = rebuildIndexInOrder(pChild, uIndex);
            }
        }
    }    

    // ignore self (batch node)
    if (! pobParent->isEqual(this))
    {
        pobParent->setAtlasIndex(uIndex);
        uIndex++;
    }

    if (pChildren && pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(pChildren, pObject)
        {
            CAView* pChild = (CAView*) pObject;
            if (pChild && (pChild->getZOrder() >= 0))
            {
                uIndex = rebuildIndexInOrder(pChild, uIndex);
            }
        }
    }

    return uIndex;
}

unsigned int CABatchView::highestAtlasIndexInChild(CAView *pSprite)
{
    CCArray *pChildren = pSprite->getSubviews();

    if (! pChildren || pChildren->count() == 0)
    {
        return pSprite->getAtlasIndex();
    }
    else
    {
        return highestAtlasIndexInChild((CAView*)(pChildren->lastObject()));
    }
}

unsigned int CABatchView::lowestAtlasIndexInChild(CAView *pSprite)
{
    CCArray *pChildren = pSprite->getSubviews();

    if (! pChildren || pChildren->count() == 0)
    {
        return pSprite->getAtlasIndex();
    }
    else
    {
        return lowestAtlasIndexInChild((CAView*)(pChildren->objectAtIndex(0)));
    }
}

unsigned int CABatchView::atlasIndexForChild(CAView *pobSprite, int nZ)
{
    CCArray *pBrothers = pobSprite->getSuperview()->getSubviews();
    unsigned int uChildIndex = pBrothers->indexOfObject(pobSprite);

    // ignore parent Z if parent is spriteSheet
    bool bIgnoreParent = (CABatchView*)(pobSprite->getSuperview()) == this;
    CAView *pPrevious = NULL;
    if (uChildIndex > 0 &&
        uChildIndex < UINT_MAX)
    {
        pPrevious = (CAView*)(pBrothers->objectAtIndex(uChildIndex - 1));
    }

    // first child of the sprite sheet
    if (bIgnoreParent)
    {
        if (uChildIndex == 0)
        {
            return 0;
        }

        return highestAtlasIndexInChild(pPrevious) + 1;
    }

    // parent is a CAView, so, it must be taken into account

    // first child of an CAView ?
    if (uChildIndex == 0)
    {
        CAView *p = (CAView*)(pobSprite->getSuperview());

        // less than parent and brothers
        if (nZ < 0)
        {
            return p->getAtlasIndex();
        }
        else
        {
            return p->getAtlasIndex() + 1;
        }
    }
    else
    {
        // previous & sprite belong to the same branch
        if ((pPrevious->getZOrder() < 0 && nZ < 0) || (pPrevious->getZOrder() >= 0 && nZ >= 0))
        {
            return highestAtlasIndexInChild(pPrevious) + 1;
        }

        // else (previous < 0 and sprite >= 0 )
        CAView *p = (CAView*)(pobSprite->getSuperview());
        return p->getAtlasIndex() + 1;
    }

    // Should not happen. Error calculating Z on SpriteSheet
    CCAssert(0, "should not run here");
    return 0;
}

// add child helper

void CABatchView::insertChild(CAView *pSprite, unsigned int uIndex)
{
    pSprite->setBatch(this);
    pSprite->setAtlasIndex(uIndex);
    pSprite->setDirty(true);

    if(m_pobImageAtlas->getTotalQuads() == m_pobImageAtlas->getCapacity())
    {
        increaseAtlasCapacity();
    }

    ccV3F_C4B_T2F_Quad quad = pSprite->getQuad();
    m_pobImageAtlas->insertQuad(&quad, uIndex);

    ccArray *descendantsData = m_pobDescendants->data;

    ccArrayInsertObjectAtIndex(descendantsData, pSprite, uIndex);

    // update indices
    unsigned int i = uIndex+1;
    
    CAView* pChild = NULL;
    for(; i<descendantsData->num; i++){
        pChild = (CAView*)descendantsData->arr[i];
        pChild->setAtlasIndex(pChild->getAtlasIndex() + 1);
    }

    // add children recursively
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(pSprite->getSubviews(), pObj)
    {
        pChild = (CAView*)pObj;
        unsigned int idx = atlasIndexForChild(pChild, pChild->getZOrder());
        insertChild(pChild, idx);
    }
}

// addChild helper, faster than insertChild
void CABatchView::appendChild(CAView* sprite)
{
    m_bReorderChildDirty=true;
    sprite->setBatch(this);
    sprite->setDirty(true);

    if(m_pobImageAtlas->getTotalQuads() == m_pobImageAtlas->getCapacity()) {
        increaseAtlasCapacity();
    }

    ccArray *descendantsData = m_pobDescendants->data;

    ccArrayAppendObjectWithResize(descendantsData, sprite);

    unsigned int index=descendantsData->num-1;

    sprite->setAtlasIndex(index);

    ccV3F_C4B_T2F_Quad quad = sprite->getQuad();
    m_pobImageAtlas->insertQuad(&quad, index);

    // add children recursively
    
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(sprite->getSubviews(), pObj)
    {
        CAView* child = (CAView*)pObj;
        appendChild(child);
    }
}

void CABatchView::removeSpriteFromAtlas(CAView *pobSprite)
{
    // remove from TextureAtlas
    m_pobImageAtlas->removeQuadAtIndex(pobSprite->getAtlasIndex());

    // Cleanup sprite. It might be reused (issue #569)
    pobSprite->setBatch(NULL);

    unsigned int uIndex = m_pobDescendants->indexOfObject(pobSprite);
    if (uIndex != UINT_MAX)
    {
        m_pobDescendants->removeObjectAtIndex(uIndex);

        // update all sprites beyond this one
        unsigned int count = m_pobDescendants->count();
        
        for(; uIndex < count; ++uIndex)
        {
            CAView* s = (CAView*)(m_pobDescendants->objectAtIndex(uIndex));
            s->setAtlasIndex( s->getAtlasIndex() - 1 );
        }
    }

    // remove children recursively
    CCArray *pChildren = pobSprite->getSubviews();
    if (pChildren && pChildren->count() > 0)
    {
        CCObject* pObject = NULL;
        CCARRAY_FOREACH(pChildren, pObject)
        {
            CAView* pChild = (CAView*) pObject;
            if (pChild)
            {
                removeSpriteFromAtlas(pChild);
            }
        }
    }
}

void CABatchView::updateBlendFunc(void)
{
    if (! m_pobImageAtlas->getImage()->hasPremultipliedAlpha())
    {
        m_blendFunc.src = GL_SRC_ALPHA;
        m_blendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    }
}

// CocosNodeTexture protocol
void CABatchView::setBlendFunc(ccBlendFunc blendFunc)
{
    m_blendFunc = blendFunc;
}

ccBlendFunc CABatchView::getBlendFunc(void)
{
    return m_blendFunc;
}

CAImage* CABatchView::getImage(void)
{
    return m_pobImageAtlas->getImage();
}

void CABatchView::setImage(CAImage *image)
{
    m_pobImageAtlas->setImage(image);
    updateBlendFunc();
}


// CAViewSheet Extension
//implementation CAViewSheet (TMXTiledMapExtension)

void CABatchView::insertQuadFromSprite(CAView *sprite, unsigned int index)
{
    CCAssert( sprite != NULL, "Argument must be non-NULL");
    CCAssert( dynamic_cast<CAView*>(sprite), "CABatchView only supports CAViews as children");

    // make needed room
    while(index >= m_pobImageAtlas->getCapacity() || m_pobImageAtlas->getCapacity() == m_pobImageAtlas->getTotalQuads())
    {
        this->increaseAtlasCapacity();
    }
    //
    // update the quad directly. Don't add the sprite to the scene graph
    //
    sprite->setBatch(this);
    sprite->setAtlasIndex(index);

    ccV3F_C4B_T2F_Quad quad = sprite->getQuad();
    m_pobImageAtlas->insertQuad(&quad, index);

    // XXX: updateTransform will update the textureAtlas too, using updateQuad.
    // XXX: so, it should be AFTER the insertQuad
    sprite->setDirty(true);
    sprite->updateTransform();
}

void CABatchView::updateQuadFromSprite(CAView *sprite, unsigned int index)
{
    CCAssert(sprite != NULL, "Argument must be non-nil");
    CCAssert(dynamic_cast<CAView*>(sprite) != NULL, "CABatchView only supports CAViews as children");
    
	// make needed room
	while (index >= m_pobImageAtlas->getCapacity() || m_pobImageAtlas->getCapacity() == m_pobImageAtlas->getTotalQuads())
    {
		this->increaseAtlasCapacity();
    }
    
	//
	// update the quad directly. Don't add the sprite to the scene graph
	//
	sprite->setBatch(this);
    sprite->setAtlasIndex(index);
    
	sprite->setDirty(true);
	
	// UpdateTransform updates the textureAtlas quad
	sprite->updateTransform();
}

CABatchView * CABatchView::addSpriteWithoutQuad(CAView*child, unsigned int z, int aTag)
{
    CCAssert( child != NULL, "Argument must be non-NULL");
    CCAssert( dynamic_cast<CAView*>(child), "CABatchView only supports CAViews as children");

    // quad index is Z
    child->setAtlasIndex(z);

    // XXX: optimize with a binary search
    int i=0;
 
    CCObject* pObject = NULL;
    CCARRAY_FOREACH(m_pobDescendants, pObject)
    {
        CAView* pChild = (CAView*) pObject;
        if (pChild && (pChild->getAtlasIndex() >= z))
        {
            ++i;
        }
    }
    
    m_pobDescendants->insertObject(child, i);

    // IMPORTANT: Call super, and not self. Avoid adding it to the texture atlas array
    CAView::insertSubview(child, z);
    child->setTag(aTag);
    //#issue 1262 don't use lazy sorting, tiles are added as quads not as sprites, so sprites need to be added in order
    reorderBatch(false);

    return this;
}

NS_CC_END
