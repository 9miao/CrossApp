
#include "CABatchView.h"
#include "ccConfig.h"
#include "CAView.h"
#include "view/CADrawingPrimitives.h"
#include "images/CAImageCache.h"
#include "support/CAPointExtension.h"
#include "shaders/CAShaderCache.h"
#include "shaders/CAGLProgram.h"
#include "shaders/ccGLStateCache.h"
#include "basics/CAApplication.h"
#include "support/TransformUtils.h"
#include "support/CAProfiling.h"
#include "kazmath/GL/matrix.h"

NS_CC_BEGIN

CABatchView* CABatchView::createWithImage(CAImage* image, unsigned int capacity/* = kDefaultSpriteBatchCapacity*/)
{
    CABatchView *batchNode = new CABatchView();
    batchNode->initWithImage(image, capacity);
    batchNode->autorelease();

    return batchNode;
}

bool CABatchView::initWithImage(CAImage *image, unsigned int capacity)
{
    m_blendFunc.src = CC_BLEND_SRC;
    m_blendFunc.dst = CC_BLEND_DST;
    m_pobImageAtlas = new CAImageAtlas();

    if (0 == capacity)
    {
        capacity = kDefaultSpriteBatchCapacity;
    }
    
    m_pobImageAtlas->initWithImage(image, capacity);

    updateBlendFunc();

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
{
    
}

CABatchView::~CABatchView()
{
    CC_SAFE_RELEASE(m_pobImageAtlas);
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
    appendSubview(subview);
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

    removeViewFromAtlas(pSprite);

    CAView::removeSubview(pSprite);
}

void CABatchView::removeSubviewAtIndex(unsigned int uIndex)
{
    removeSubview(m_obSubviews.at(uIndex));
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
        std::sort(m_obSubviews.begin(), m_obSubviews.end(), compareSubviewZOrder);
        
        if (!m_obSubviews.empty())
        {
            CAVector<CAView*>::iterator itr;
            
            for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
                if(m_bRunning) (*itr)->sortAllSubviews();
            
            int index=0;
            for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
                updateAtlasIndex(*itr, &index);
        }
        m_bReorderChildDirty=false;
    }
}

void CABatchView::updateAtlasIndex(CAView* view, int* curIndex)
{
    const CAVector<CAView*> pVector = view->getSubviews();
    unsigned int count = (unsigned int)pVector.size();

    int oldIndex = 0;

    if( count == 0 )
    {
        oldIndex = view->getAtlasIndex();
        view->setAtlasIndex(*curIndex);
        view->setOrderOfArrival(0);
        if (oldIndex != *curIndex)
        {
            swap(oldIndex, *curIndex);
        }
        (*curIndex)++;
    }
    else
    {
        bool needNewIndex = true;

        if (pVector.front()->getZOrder() >= 0)
        {
            oldIndex = view->getAtlasIndex();
            view->setAtlasIndex(*curIndex);
            view->setOrderOfArrival(0);
            if (oldIndex != *curIndex)
            {
                swap(oldIndex, *curIndex);
            }
            (*curIndex)++;
            
            needNewIndex = false;
        }
        
        CAVector<CAView*>::const_iterator itr;
        for (itr=pVector.begin(); itr!=pVector.end(); itr++)
        {
            CAView* subview = *itr;
            if (needNewIndex && subview->getZOrder() >= 0)
            {
                oldIndex = view->getAtlasIndex();
                view->setAtlasIndex(*curIndex);
                view->setOrderOfArrival(0);
                if (oldIndex != *curIndex)
                {
                    this->swap(oldIndex, *curIndex);
                }
                (*curIndex)++;
                needNewIndex = false;
                
            }
            
            updateAtlasIndex(subview, curIndex);
        }

        if (needNewIndex)
        {
            oldIndex = view->getAtlasIndex();
            view->setAtlasIndex(*curIndex);
            view->setOrderOfArrival(0);
            if (oldIndex!=*curIndex)
            {
                swap(oldIndex, *curIndex);
            }
            (*curIndex)++;
        }
    }
}

void CABatchView::swap(int oldIndex, int newIndex)
{
    ccV3F_C4B_T2F_Quad* quads = m_pobImageAtlas->getQuads();

    ccV3F_C4B_T2F_Quad tempItemQuad=quads[oldIndex];

    m_obDescendants.at(newIndex)->setAtlasIndex(oldIndex);
    m_obDescendants.swap(oldIndex, newIndex);
    quads[oldIndex]=quads[newIndex];
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

    CAVector<CAView*>::const_iterator itr;
    for (itr=m_obSubviews.begin(); itr!=m_obSubviews.end(); itr++)
        (*itr)->updateTransform();

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
        CCAssert(false, "Not enough memory to resize the atlas");
    }
}

unsigned int CABatchView::rebuildIndexInOrder(CAView *superview, unsigned int uIndex)
{
    const CAVector<CAView*> pVector = superview->getSubviews();
    
    CAVector<CAView*>::const_iterator itr=m_obSubviews.begin();
    while (itr!=m_obSubviews.end())
    {
        CC_BREAK_IF((*itr)->getZOrder() >= 0);
        uIndex = rebuildIndexInOrder((*itr), uIndex);
        itr++;
    }
    
    if (!superview->isEqual(this))
    {
        superview->setAtlasIndex(uIndex);
        uIndex++;
    }

    while (itr!=m_obSubviews.end())
    {
        uIndex = rebuildIndexInOrder((*itr), uIndex);
    }

    return uIndex;
}

unsigned int CABatchView::highestAtlasIndexInSubview(CAView *view)
{
    const CAVector<CAView*>& pSubview = view->getSubviews();

    if (pSubview.empty())
    {
        return view->getAtlasIndex();
    }
    else
    {
        return highestAtlasIndexInSubview(pSubview.back());
    }
}

unsigned int CABatchView::lowestAtlasIndexInSubview(CAView *view)
{
    const CAVector<CAView*>& pSubview = view->getSubviews();
    
    if (pSubview.empty())
    {
        return view->getAtlasIndex();
    }
    else
    {
        return lowestAtlasIndexInSubview(pSubview.front());
    }
}

unsigned int CABatchView::atlasIndexForSubview(CAView *view, int nZ)
{
    const CAVector<CAView*>& pBrothers = view->getSuperview()->getSubviews();
    
    unsigned int uSubviewIndex = (unsigned int)pBrothers.getIndex(view);

    bool bIgnoreParent = (CABatchView*)(view->getSuperview()) == this;
    CAView *pPrevious = NULL;
    if (uSubviewIndex > 0 && uSubviewIndex < UINT_MAX)
    {
        pPrevious = (CAView*)(pBrothers.at(uSubviewIndex - 1));
    }

    if (bIgnoreParent)
    {
        if (uSubviewIndex == 0)
        {
            return 0;
        }

        return highestAtlasIndexInSubview(pPrevious) + 1;
    }

    if (uSubviewIndex == 0)
    {
        CAView *p = (CAView*)(view->getSuperview());

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
            return highestAtlasIndexInSubview(pPrevious) + 1;
        }

        CAView *p = (CAView*)(view->getSuperview());
        return p->getAtlasIndex() + 1;
    }

    CCAssert(0, "should not run here");
    return 0;
}

void CABatchView::insertSubview(CAView *subview, unsigned int uIndex)
{
    subview->setBatch(this);
    subview->setAtlasIndex(uIndex);
    subview->setDirty(true);

    if(m_pobImageAtlas->getTotalQuads() == m_pobImageAtlas->getCapacity())
    {
        increaseAtlasCapacity();
    }

    ccV3F_C4B_T2F_Quad quad = subview->getQuad();
    m_pobImageAtlas->insertQuad(&quad, uIndex);
    m_obDescendants.insert(uIndex, subview);

    
    if (m_obDescendants.size() > uIndex + 1)
    {
        for (CAVector<CAView*>::const_iterator itr=m_obDescendants.begin() + uIndex + 1;
             itr!=m_obDescendants.end();
             itr++)
        {
            (*itr)->setAtlasIndex(subview->getAtlasIndex() + 1);
        }
    }

    for (CAVector<CAView*>::const_iterator itr=subview->getSubviews().begin();
         itr!=subview->getSubviews().end();
         itr++)
    {
        unsigned int idx = atlasIndexForSubview(*itr, (*itr)->getZOrder());
        insertSubview(*itr, idx);
    }
}

void CABatchView::appendSubview(CAView* subview)
{
    m_bReorderChildDirty=true;
    subview->setBatch(this);
    subview->setDirty(true);

    if(m_pobImageAtlas->getTotalQuads() == m_pobImageAtlas->getCapacity()) {
        increaseAtlasCapacity();
    }

    m_obDescendants.pushBack(subview);
    
    unsigned int index = (unsigned int)m_obDescendants.size() - 1;

    subview->setAtlasIndex(index);

    ccV3F_C4B_T2F_Quad quad = subview->getQuad();
    m_pobImageAtlas->insertQuad(&quad, index);
    
    CAVector<CAView*>::const_iterator itr;
    for (itr=subview->getSubviews().begin(); itr!=subview->getSubviews().end(); itr++)
    {
        appendSubview(*itr);
    }
}

void CABatchView::removeViewFromAtlas(CAView *view)
{
    m_pobImageAtlas->removeQuadAtIndex(view->getAtlasIndex());

    view->setBatch(NULL);

    
    
    unsigned int uIndex = (unsigned int)m_obDescendants.getIndex(view);
    if (uIndex != UINT_MAX)
    {
        m_obDescendants.erase(uIndex);
        unsigned int count = (unsigned int)m_obDescendants.size();
        
        for(; uIndex < count; ++uIndex)
        {
            CAView* s = m_obDescendants.at(uIndex);
            s->setAtlasIndex(s->getAtlasIndex() - 1);
        }
    }

    CAVector<CAView*>::const_iterator itr;
    for (itr=view->getSubviews().begin(); itr!=view->getSubviews().end(); itr++)
    {
        removeViewFromAtlas(*itr);
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

void CABatchView::insertQuadFromView(CrossApp::CAView *view, unsigned int index)
{
    CCAssert( view != NULL, "Argument must be non-NULL");

    while(index >= m_pobImageAtlas->getCapacity() || m_pobImageAtlas->getCapacity() == m_pobImageAtlas->getTotalQuads())
    {
        this->increaseAtlasCapacity();
    }
    view->setBatch(this);
    view->setAtlasIndex(index);

    ccV3F_C4B_T2F_Quad quad = view->getQuad();
    m_pobImageAtlas->insertQuad(&quad, index);

    view->setDirty(true);
    view->updateTransform();
}

void CABatchView::updateQuadFromView(CrossApp::CAView *view, unsigned int index)
{
    CCAssert(view != NULL, "Argument must be non-nil");
    
	while (index >= m_pobImageAtlas->getCapacity() || m_pobImageAtlas->getCapacity() == m_pobImageAtlas->getTotalQuads())
    {
		this->increaseAtlasCapacity();
    }

	view->setBatch(this);
    view->setAtlasIndex(index);
	view->setDirty(true);
    view->updateTransform();
}

CABatchView * CABatchView::addViewWithoutQuad(CAView* view, unsigned int z, int aTag)
{
    CCAssert( view != NULL, "Argument must be non-NULL");

    view->setAtlasIndex(z);

    int i=0;
 
    CAVector<CAView*>::iterator itr;
    for (itr=m_obDescendants.begin(); itr!=m_obDescendants.end(); itr++)
    {
        CC_CONTINUE_IF((*itr)->getAtlasIndex() < z);
        ++i;
    }
    m_obDescendants.insert(i, view);

    CAView::insertSubview(view, z);
    view->setTag(aTag);
    reorderBatch(false);

    return this;
}

NS_CC_END
