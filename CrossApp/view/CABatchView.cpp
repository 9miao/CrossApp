
#include "CABatchView.h"
#include "ccConfig.h"
#include "CAView.h"
#include "draw_nodes/CCDrawingPrimitives.h"
#include "images/CAImageCache.h"
#include "support/CCPointExtension.h"
#include "shaders/CAShaderCache.h"
#include "shaders/CAGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "basics/CAApplication.h"
#include "support/TransformUtils.h"
#include "support/CCProfiling.h"
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN

CABatchView* CABatchView::createWithImage(CAImage* tex, unsigned int capacity/* = kDefaultSpriteBatchCapacity*/)
{
    CABatchView *batchNode = new CABatchView();
    batchNode->initWithImage(tex, capacity);
    batchNode->autorelease();

    return batchNode;
}

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

    m_pSubviews = new CCArray();
    m_pSubviews->initWithCapacity(capacity);

    m_pobDescendants = new CCArray();
    m_pobDescendants->initWithCapacity(capacity);

    setShaderProgram(CAShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionTextureColor));
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

void CABatchView::visit(void)
{
    CC_PROFILER_START_CATEGORY(kCCProfilerCategoryBatchSprite, "CABatchView - visit");

    CC_RETURN_IF(!m_bVisible);
    
    kmGLPushMatrix();
    sortAllSubview();
    transform();

    draw();

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

void CABatchView::reorderSubview(CAView * subview, int zOrder)
{
    if (zOrder == subview->getZOrder())
    {
        return;
    }

    CAView::reorderSubview(subview, zOrder);
}

void CABatchView::removeSubview(CAView* subview)
{
    CAView *pSprite = (CAView*)(subview);

    if (pSprite == NULL)
    {
        return;
    }

    removeSpriteFromAtlas(pSprite);

    CAView::removeSubview(pSprite);
}

void CABatchView::removeSubviewAtIndex(unsigned int uIndex)
{
    removeSubview((CAView*)(m_pSubviews->objectAtIndex(uIndex)));
}

void CABatchView::removeAllSubviews()
{
    CAView::removeAllSubviews();

    m_pobImageAtlas->removeAllQuads();
}

void CABatchView::sortAllSubview()
{
    if (m_bReorderChildDirty)
    {
        int i = 0,j = 0,length = m_pSubviews->data->num;
        CAView ** x = (CAView**)m_pSubviews->data->arr;
        CAView *tempItem = NULL;

        for(i=1; i<length; i++)
        {
            tempItem = x[i];
            j = i-1;


            while(j>=0 && ( tempItem->getZOrder() < x[j]->getZOrder() || ( tempItem->getZOrder() == x[j]->getZOrder() && tempItem->getOrderOfArrival() < x[j]->getOrderOfArrival() ) ) )
            {
                x[j+1] = x[j];
                j--;
            }

            x[j+1] = tempItem;
        }

        if (m_pSubviews->count() > 0)
        {
            arrayMakeObjectsPerformSelector(m_pSubviews, sortAllSubviews, CAView*);

            int index=0;

            CAObject* pObj = NULL;
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

        CAObject* pObj = NULL;
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
        {
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
    CAObject** x = m_pobDescendants->data->arr;
    ccV3F_C4B_T2F_Quad* quads = m_pobImageAtlas->getQuads();

    CAObject* tempItem = x[oldIndex];
    ccV3F_C4B_T2F_Quad tempItemQuad=quads[oldIndex];

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

void CABatchView::draw(void)
{
    CC_PROFILER_START("CABatchView - draw");

    CC_RETURN_IF(!m_pobImageAtlas);
    CC_RETURN_IF(m_pobImageAtlas->getTotalQuads() == 0);
    
    CC_NODE_DRAW_SETUP();

    arrayMakeObjectsPerformSelector(m_pSubviews, updateTransform, CAView*);

    ccGLBlendFunc( m_blendFunc.src, m_blendFunc.dst );

    m_pobImageAtlas->drawQuads();

    CC_PROFILER_STOP("CABatchView - draw");
}

void CABatchView::increaseAtlasCapacity(void)
{
    unsigned int quantity = (m_pobImageAtlas->getCapacity() + 1) * 4 / 3;

    CCLOG("CrossApp: CABatchView: resizing TextureAtlas capacity from [%lu] to [%lu].",
        (long)m_pobImageAtlas->getCapacity(),
        (long)quantity);

    if (! m_pobImageAtlas->resizeCapacity(quantity))
    {
        CCLOGWARN("CrossApp: WARNING: Not enough memory to resize the atlas");
        CCAssert(false, "Not enough memory to resize the atlas");
    }
}

unsigned int CABatchView::rebuildIndexInOrder(CAView *pobParent, unsigned int uIndex)
{
    CCArray *psubview = pobParent->getSubviews();

    if (psubview && psubview->count() > 0)
    {
        CAObject* pObject = NULL;
        CCARRAY_FOREACH(psubview, pObject)
        {
            CAView* pChild = (CAView*) pObject;
            if (pChild && (pChild->getZOrder() < 0))
            {
                uIndex = rebuildIndexInOrder(pChild, uIndex);
            }
        }
    }    

    if (! pobParent->isEqual(this))
    {
        pobParent->setAtlasIndex(uIndex);
        uIndex++;
    }

    if (psubview && psubview->count() > 0)
    {
        CAObject* pObject = NULL;
        CCARRAY_FOREACH(psubview, pObject)
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
    CCArray *psubview = pSprite->getSubviews();

    if (! psubview || psubview->count() == 0)
    {
        return pSprite->getAtlasIndex();
    }
    else
    {
        return highestAtlasIndexInChild((CAView*)(psubview->lastObject()));
    }
}

unsigned int CABatchView::lowestAtlasIndexInChild(CAView *pSprite)
{
    CCArray *psubview = pSprite->getSubviews();

    if (! psubview || psubview->count() == 0)
    {
        return pSprite->getAtlasIndex();
    }
    else
    {
        return lowestAtlasIndexInChild((CAView*)(psubview->objectAtIndex(0)));
    }
}

unsigned int CABatchView::atlasIndexForChild(CAView *pobSprite, int nZ)
{
    CCArray *pBrothers = pobSprite->getSuperview()->getSubviews();
    unsigned int uChildIndex = pBrothers->indexOfObject(pobSprite);

    bool bIgnoreParent = (CABatchView*)(pobSprite->getSuperview()) == this;
    CAView *pPrevious = NULL;
    if (uChildIndex > 0 &&
        uChildIndex < UINT_MAX)
    {
        pPrevious = (CAView*)(pBrothers->objectAtIndex(uChildIndex - 1));
    }

    if (bIgnoreParent)
    {
        if (uChildIndex == 0)
        {
            return 0;
        }

        return highestAtlasIndexInChild(pPrevious) + 1;
    }

    if (uChildIndex == 0)
    {
        CAView *p = (CAView*)(pobSprite->getSuperview());

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
        if ((pPrevious->getZOrder() < 0 && nZ < 0) || (pPrevious->getZOrder() >= 0 && nZ >= 0))
        {
            return highestAtlasIndexInChild(pPrevious) + 1;
        }

        CAView *p = (CAView*)(pobSprite->getSuperview());
        return p->getAtlasIndex() + 1;
    }

    CCAssert(0, "should not run here");
    return 0;
}

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

    unsigned int i = uIndex+1;
    
    CAView* pChild = NULL;
    for(; i<descendantsData->num; i++){
        pChild = (CAView*)descendantsData->arr[i];
        pChild->setAtlasIndex(pChild->getAtlasIndex() + 1);
    }

    CAObject* pObj = NULL;
    CCARRAY_FOREACH(pSprite->getSubviews(), pObj)
    {
        pChild = (CAView*)pObj;
        unsigned int idx = atlasIndexForChild(pChild, pChild->getZOrder());
        insertChild(pChild, idx);
    }
}

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
    
    CAObject* pObj = NULL;
    CCARRAY_FOREACH(sprite->getSubviews(), pObj)
    {
        CAView* child = (CAView*)pObj;
        appendChild(child);
    }
}

void CABatchView::removeSpriteFromAtlas(CAView *pobSprite)
{
    m_pobImageAtlas->removeQuadAtIndex(pobSprite->getAtlasIndex());

    pobSprite->setBatch(NULL);

    unsigned int uIndex = m_pobDescendants->indexOfObject(pobSprite);
    if (uIndex != UINT_MAX)
    {
        m_pobDescendants->removeObjectAtIndex(uIndex);

        unsigned int count = m_pobDescendants->count();
        
        for(; uIndex < count; ++uIndex)
        {
            CAView* s = (CAView*)(m_pobDescendants->objectAtIndex(uIndex));
            s->setAtlasIndex( s->getAtlasIndex() - 1 );
        }
    }

    CCArray *psubview = pobSprite->getSubviews();
    if (psubview && psubview->count() > 0)
    {
        CAObject* pObject = NULL;
        CCARRAY_FOREACH(psubview, pObject)
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

void CABatchView::insertQuadFromSprite(CAView *sprite, unsigned int index)
{
    CCAssert( sprite != NULL, "Argument must be non-NULL");
    CCAssert( dynamic_cast<CAView*>(sprite), "CABatchView only supports CAViews as subview");

    while(index >= m_pobImageAtlas->getCapacity() || m_pobImageAtlas->getCapacity() == m_pobImageAtlas->getTotalQuads())
    {
        this->increaseAtlasCapacity();
    }
    sprite->setBatch(this);
    sprite->setAtlasIndex(index);

    ccV3F_C4B_T2F_Quad quad = sprite->getQuad();
    m_pobImageAtlas->insertQuad(&quad, index);

    sprite->setDirty(true);
    sprite->updateTransform();
}

void CABatchView::updateQuadFromSprite(CAView *sprite, unsigned int index)
{
    CCAssert(sprite != NULL, "Argument must be non-nil");
    CCAssert(dynamic_cast<CAView*>(sprite) != NULL, "CABatchView only supports CAViews as subview");
    
	while (index >= m_pobImageAtlas->getCapacity() || m_pobImageAtlas->getCapacity() == m_pobImageAtlas->getTotalQuads())
    {
		this->increaseAtlasCapacity();
    }

	sprite->setBatch(this);
    sprite->setAtlasIndex(index);
	sprite->setDirty(true);
    sprite->updateTransform();
}

CABatchView * CABatchView::addSpriteWithoutQuad(CAView*child, unsigned int z, int aTag)
{
    CCAssert( child != NULL, "Argument must be non-NULL");
    CCAssert( dynamic_cast<CAView*>(child), "CABatchView only supports CAViews as subview");

    child->setAtlasIndex(z);

    int i=0;
 
    CAObject* pObject = NULL;
    CCARRAY_FOREACH(m_pobDescendants, pObject)
    {
        CAView* pChild = (CAView*) pObject;
        if (pChild && (pChild->getAtlasIndex() >= z))
        {
            ++i;
        }
    }
    
    m_pobDescendants->insertObject(child, i);

    CAView::insertSubview(child, z);
    child->setTag(aTag);
    reorderBatch(false);

    return this;
}

NS_CC_END
