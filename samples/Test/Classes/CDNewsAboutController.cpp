//
//  CDNewsAboutController.cpp
//  Test
//
//  Created by renhongguang on 15/4/17.
//
//

#include "CDNewsAboutController.h"
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)

void getAllFilePaths(std::vector<std::string>& filePaths, std::vector<std::string>& dirPaths, const std::string& path)
{
    std::string p = path.at(path.length() - 1) == '/' ? path : path + "/";
    
    struct dirent *ptr;
    DIR* dir;
    dir = opendir(p.c_str());
    CC_RETURN_IF(dir == 0);
    while ((ptr = readdir(dir)) != NULL)
    {
        std::string file_path = p + ptr->d_name;
        
        if (ptr->d_type == DT_DIR && ptr->d_name[0] != '.')
        {
            dirPaths.push_back(file_path);
            getAllFilePaths(filePaths, dirPaths, file_path);
        }
        else
        {
            filePaths.push_back(file_path);
        }
    }
    
    closedir(dir);
}

#endif

unsigned long getFilePathSize(const std::string& path)
{

    unsigned long lCurFileSize = 0;

#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    std::vector<std::string> filePaths;
    std::vector<std::string> dirPaths;
    getAllFilePaths(filePaths, dirPaths, path);
    for (int i=0; i<filePaths.size(); i++)
    {
        FILE* fp = fopen(filePaths.at(i).c_str(), "rb");
        if (fp != NULL)
        {
            fseek(fp, 0L, SEEK_END);
            lCurFileSize += ftell(fp);
            fclose(fp);
        }
    }

#endif
    return lCurFileSize;
}

void removeFilePath(const std::string& path)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    std::vector<std::string> filePaths;
    std::vector<std::string> dirPaths;
    getAllFilePaths(filePaths, dirPaths, path);
    
    for (std::vector<std::string>::iterator itr=filePaths.begin();
         itr!=filePaths.end();
         itr++)
    {
        remove(itr->c_str());
    }
    
    for (std::vector<std::string>::iterator itr=dirPaths.begin();
         itr!=dirPaths.end();
         itr++)
    {
        remove(itr->c_str());
    }
#endif
}

CDNewsAboutTableCell::CDNewsAboutTableCell()
{
    this->setAllowsSelected(false);
}

CDNewsAboutTableCell::~CDNewsAboutTableCell()
{
    
}

CDNewsAboutTableCell* CDNewsAboutTableCell::create(const std::string& identifier, const DRect& _rect)
{
    CDNewsAboutTableCell* tableViewCell = new CDNewsAboutTableCell();
    if(tableViewCell&&tableViewCell->initWithReuseIdentifier(identifier))
    {
        tableViewCell->setFrame(_rect);
        tableViewCell->autorelease();
        return tableViewCell;
    }
    CC_SAFE_DELETE(tableViewCell);
    return NULL;
}

void CDNewsAboutTableCell::highlightedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 64)));
}

void CDNewsAboutTableCell::selectedTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(ccc4(0, 0, 0, 64)));
}

void CDNewsAboutTableCell::initWithCell()
{
    DSize m_size = this->getFrame().size;
    
    CALabel* cellText = CALabel::createWithCenter(DRect(m_size.width/2+40, m_size.height/2, m_size.width, 40));
    cellText->setTag(100);
    cellText->setFontSize(30);
    cellText->setTextAlignment(CATextAlignmentLeft);
    cellText->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
    this->addSubview(cellText);
    
    CAScale9ImageView* iv = CAScale9ImageView::createWithCenter(DRect(m_size.width-50,m_size.height/2,50,50));
    iv->setImage(CAImage::create("source_material/cell_btn_right.png"));
    iv->setTag(101);
    this->addSubview(iv);
    
    CASwitch* sw = CASwitch::createWithCenter(DRect(m_size.width-90, m_size.height/2, 100, 50));
    sw->setTag(103);
    sw->setVisible(false);
    this->addSubview(sw);
}

CDNewsAboutController::CDNewsAboutController()
{

}

CDNewsAboutController::~CDNewsAboutController()
{
    
}

void CDNewsAboutController::viewDidLoad()
{
    temp_Path = CCFileUtils::sharedFileUtils()->getWritablePath() + "image";
    if (p_TableView!=NULL)
    {
        this->getView()->removeSubview(p_TableView);
        p_TableView = NULL;
    }
    p_TableView= CATableView::createWithLayout(DLayout(DHorizontalLayoutFill, DVerticalLayout_T_B(-150, 0)));
    p_TableView->setTableViewDataSource(this);
    p_TableView->setTableViewDelegate(this);
    p_TableView->setAllowsSelection(true);
    p_TableView->setAllowsMultipleSelection(false);
    p_TableView->setAlwaysTopSectionHeader(false);
    this->getView()->addSubview(p_TableView);
    p_TableView->setTableHeaderHeight(602);
    CAView* view = CAView::createWithColor(CAColor_clear);
    view->setLayout(DLayoutFill);
    
    CAImageView* head_bg = CAImageView::createWithLayout(DLayoutFill);
    head_bg->setImage(CAImage::create("image/about_head_bg.png"));
    view->addSubview(head_bg);
    
    CAImageView* head = CAImageView::createWithLayout(DLayoutFill);
    head->setImage(CAImage::create("image/avatar_bg_70.png"));
    view->addSubview(head);
    
//    CAButton* btn1 = CAButton::create(CAButtonTypeSquareRect);
//    btn1->setCenter(DRect(winSize.width/6, 600-(winSize.width/3)/9*5/2, winSize.width/3-1, (winSize.width/3)/9*5));
//    btn1->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn1_up.png")));
//    btn1->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn1_down.png")));
//    view->addSubview(btn1);
//    
//    CAButton* btn2 = CAButton::create(CAButtonTypeSquareRect);
//    btn2->setCenter(DRect(winSize.width/2, 600-(winSize.width/3)/9*5/2, winSize.width/3-1, (winSize.width/3)/9*5));
//    btn2->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn2_up.png")));
//    btn2->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn2_down.png")));
//    view->addSubview(btn2);
//    
//    CAButton* btn3 = CAButton::create(CAButtonTypeSquareRect);
//    btn3->setTag(100);
//    btn3->setCenter(DRect(winSize.width/6*5, 600-(winSize.width/3)/9*5/2, winSize.width/3-1, (winSize.width/3)/9*5));
//    btn3->setBackgroundViewForState(CAControlStateNormal, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn3_up.png")));
//    btn3->setBackgroundViewForState(CAControlStateHighlighted, CAScale9ImageView::createWithImage(CAImage::create("image/about_btn3_down.png")));
//    view->addSubview(btn3);
    
    p_TableView->setTableHeaderView(view);
}

void CDNewsAboutController::viewDidUnload()
{
    
}

CAView* CDNewsAboutController::tableViewSectionViewForHeaderInSection(CATableView* table, const DSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(ccc4(240,240,240,255));
    
    return view;
}

CAView* CDNewsAboutController::tableViewSectionViewForFooterInSection(CATableView* table, const DSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(CAColor_gray);
    
    return view;
}

void CDNewsAboutController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    if (section==1 && row == 5)
    {
        _waitview = CAView::createWithColor(ccc4(0,0,0,128));
        _waitview->setLayout(DLayoutFill);
        this->getView()->insertSubview(_waitview, CAWindowZOderTop);
        
        CAActivityIndicatorView* p_pLoading = CAActivityIndicatorView::createWithLayout(DLayoutFill);
        p_pLoading->setStyle(CAActivityIndicatorViewStyleWhiteLarge);
        p_pLoading->startAnimating();
        _waitview->addSubview(p_pLoading);
        
        DSize size = this->getView()->getBounds().size;
        
        CALabel* label = CALabel::createWithCenter(DRect(size.width/2, size.height/2+100, 200, 50));
        label->setText("正在清理...");
        label->setColor(CAColor_white);
        label->setFontSize(18);
        _waitview->addSubview(label);
        
        this->start();
        CAScheduler::schedule(schedule_selector(CDNewsAboutController::deleteCallBack), this, 0.01f);
    }
}

void CDNewsAboutController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* CDNewsAboutController::tableCellAtIndex(CATableView* table, const DSize& cellSize, unsigned int section, unsigned int row)
{
    DSize _size = cellSize;
    CDNewsAboutTableCell* cell = dynamic_cast<CDNewsAboutTableCell*>(table->dequeueReusableCellWithIdentifier("CrossApp"));
    if (cell == NULL)
    {
        cell = CDNewsAboutTableCell::create("CrossApp", DRect(0, 0, _size.width, _size.height));
        cell->initWithCell();
    }
    
    CALabel* cellText = (CALabel*)cell->getSubviewByTag(100);
    CASwitch* cellBtn = (CASwitch*)cell->getSubviewByTag(103);
    cellBtn->setVisible(false);
    switch (section) {
        case 0:
			cellText->setText(unicode_to_utf8(aboutMenuTag[row]));
            break;
        case 1:
			cellText->setText(unicode_to_utf8(aboutMenuTag[row + 2]));
            if (row==2||row==3||row==4) {
                cellBtn->setVisible(true);
            };
            break;
        case 2:
			cellText->setText(unicode_to_utf8(aboutMenuTag[row + 8]));
            break;
    }
    return cell;
    
}

unsigned int CDNewsAboutController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    int cellnum = 0;
    if (section==0) {
        cellnum = 2;
    }else if (section==1){
        cellnum = 6;
    }
    else if (section==2){
        cellnum = 4;
    }
    return cellnum;
}

unsigned int CDNewsAboutController::numberOfSections(CATableView *table)
{
    return 3;
}
unsigned int CDNewsAboutController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return 100;
}

unsigned int CDNewsAboutController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    int h = 40;
    if (section==0) {
        h=30;
    }
    return h;
}

void CDNewsAboutController::worker()
{
    _filesize = getFilePathSize(temp_Path);
    _tempfilesize = _filesize;
    CCLog("_filesize = = %f",_filesize);
    removeFilePath(temp_Path);
    _filesize = 0;
    CCLog("_filesize111111 = = %f",_filesize);
}

unsigned int CDNewsAboutController::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
    return 1;
}

void CDNewsAboutController::deleteCallBack(float dt)
{
    if (_filesize<=0) {
        this->getView()->removeSubview(_waitview);
        _waitview = NULL;
        CAScheduler::unschedule(schedule_selector(CDNewsAboutController::deleteCallBack), this);
        char temstr[200];
        sprintf(temstr, UTF8("本次共清理%0.1fM缓存!"),_tempfilesize/1048576.0f);
		CAAlertView* alertView = CAAlertView::createWithText(UTF8("提示"), temstr, UTF8("关闭"), NULL);
        alertView->show();
        _tempfilesize = 0;
    }
}
