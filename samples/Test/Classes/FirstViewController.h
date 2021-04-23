#ifndef __Test__FirstViewController__
#define __Test__FirstViewController__

#include "RootWindow.h"

class FirstViewController : public CAViewController
{
    
public:
    
    FirstViewController();
    
    virtual ~FirstViewController();
    
    CREATE_FUNC(FirstViewController);
    
protected:
    
    void viewDidLoad();
    
    void viewDidUnload();

    void collectionViewDidSelectCellAtIndexPath(unsigned int section, unsigned int item);
        
    CACollectionViewCell* collectionCellAtIndex(const DSize& cellSize, unsigned int section, unsigned int item);
    
    DSize collectionViewCellSizeAtIndexPath(unsigned int section, unsigned int item);
    
    unsigned int numberOfItemsInSection(unsigned int section);

private:
    
    CAAutoCollectionView* m_pAutoCollection;
    
    std::vector<std::string> m_vTitles;
    
};
#endif /* defined(__Test__CDUIShowCollectionView__) */
