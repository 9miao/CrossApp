//
//  CDNewsAboutController.cpp
//  Test
//
//  Created by renhongguang on 15/4/17.
//
//

#include "CDNewsAboutController.h"

#ifndef WIN32
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#define ___FILE_PATH___ std::string(CCFileUtils::sharedFileUtils()->getWritablePath() + "image")

void getAllFilePaths(std::vector<std::string>& filePaths, std::vector<std::string>& dirPaths, const std::string& path)
{
#ifndef WIN32
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
#endif // WIN32
}

unsigned long getFilePathSize(const std::string& path)
{
    unsigned long lCurFileSize = 0;
    
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
    
    return lCurFileSize;
}

void removeFilePath(const std::string& path)
{
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

void CDNewsAboutTableCell::normalTableViewCell()
{
    this->setBackgroundView(CAView::createWithColor(CAColor_white));
}

void CDNewsAboutTableCell::initWithCell()
{
    DSize m_size = this->getFrame().size;
    
    CALabel* cellText = CALabel::createWithCenter(DRect(m_size.width/2+40, m_size.height/2, m_size.width, 40));
    cellText->setTag(100);
    cellText->setFontSize(_px(30));
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
:_filesize(0)
{

}

CDNewsAboutController::~CDNewsAboutController()
{
    
}

void CDNewsAboutController::viewDidLoad()
{
    winSize = this->getView()->getBounds().size;
    this->getView()->setColor(ccc4(0,0,0,0));
    if (p_TableView!=NULL)
    {
        this->getView()->removeSubview(p_TableView);
        p_TableView = NULL;
    }
    
//    CAView* bg = CAView::createWithColor(ccc4(255,255,255,0));
//    bg->setFrame(this->getView()->getBounds());
//    this->getView()->addSubview(bg);
    
    bg = CAView::createWithColor(CAColor_white);
    bg->setFrame(DRect(-500,0,500,winSize.height));
    this->getView()->addSubview(bg);
    
    p_TableView= CATableView::createWithFrame(DRect(50, winSize.height/4, 400, winSize.height));
    p_TableView->setTableViewDataSource(this);
    p_TableView->setTableViewDelegate(this);
    p_TableView->setAllowsSelection(true);
    p_TableView->setAllowsMultipleSelection(false);
    p_TableView->setBackgroundColor(CAColor_white);
    p_TableView->setAlwaysTopSectionHeader(false);
    p_TableView->setSeparatorColor(CAColor_gray);
    p_TableView->setScrollEnabled(false);
    p_TableView->setShowsScrollIndicators(false);
    bg->addSubview(p_TableView);
    
    CAButton* btn_back = CAButton::create(CAButtonTypeSquareRect);
    btn_back->setCenter(DRect(250, winSize.height-200, 40, 40));
    btn_back->setTitleColorForState(CAControlStateNormal,CAColor_white);
    btn_back->setBackGroundViewForState(CAControlStateNormal, CAImageView::createWithImage(CAImage::create("image/news_close.png")));
    btn_back->setBackGroundViewForState(CAControlStateHighlighted, CAImageView::createWithImage(CAImage::create("image/news_close.png")));
    btn_back->addTarget(this, CAControl_selector(CDNewsAboutController::btn_callback), CAControlEventTouchDown);
    bg->addSubview(btn_back);
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0.3f);
    //CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CDNewsAboutController::endAction));
    this->getView()->setColor(ccc4(0,0,0,200));
    bg->setFrameOrigin(DPoint(0, 0));
    CAViewAnimation::commitAnimations();
}

void CDNewsAboutController::viewDidUnload()
{

}

void CDNewsAboutController::endAction()
{
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0.3f);
    bg->setFrameOrigin(CCPointMake(0, 0));
    CAViewAnimation::commitAnimations();
}

void CDNewsAboutController::worker()
{
    _filesize = getFilePathSize(___FILE_PATH___);
    _tempfilesize = _filesize;
    CCLog("_filesize = = %f",_filesize);
    removeFilePath(___FILE_PATH___);
    _filesize = 0;
    CCLog("_filesize111111 = = %f",_filesize);
}
void* CDNewsAboutController::toGetFilePathSize()
{
	return NULL;
}

void CDNewsAboutController::deleteCallBack(float dt)
{
    if (_filesize<=0) {
        this->getView()->removeSubview(_waitview);
        _waitview = NULL;
        CAScheduler::unschedule(schedule_selector(CDNewsAboutController::deleteCallBack), this);
        char temstr[200];
        sprintf(temstr, UTF8("本次共清理%0.1f M缓存！"),_tempfilesize/1048576.0f);
		CAAlertView* alertView = CAAlertView::createWithText(UTF8("提示"), temstr, UTF8("关闭"), NULL);
        alertView->show();
        _tempfilesize = 0;
    }
}

CAView* CDNewsAboutController::tableViewSectionViewForHeaderInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(CAColor_clear);
    
    return view;
}

CAView* CDNewsAboutController::tableViewSectionViewForFooterInSection(CATableView* table, const CCSize& viewSize, unsigned int section)
{
    CAView* view = CAView::createWithColor(CAColor_gray);
    
    return view;
}

void CDNewsAboutController::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    CCLog("section==%d,row==%d",section,row);
    if (section==0 && row == 0) {
        _waitview = CAView::createWithColor(ccc4(0,0,0,128));
        _waitview->setFrame(DRect(0,0,winSize.width,winSize.height));
        CAActivityIndicatorView* p_pLoading = CAActivityIndicatorView::createWithCenter(DRect( winSize.width/2, winSize.height/2,50,50));
        p_pLoading->setStyle(CAActivityIndicatorViewStyleWhiteLarge);
        p_pLoading->startAnimating();
        CALabel* label = CALabel::createWithCenter(DRect(winSize.width/2,winSize.height/2+100,200,50));
        label->setText("正在清理...");
        label->setColor(CAColor_white);
        label->setFontSize(18);
        
        _waitview->addSubview(p_pLoading);
        _waitview->addSubview(label);
        this->getView()->insertSubview(_waitview, CAWindowZOderTop);
        this->start();
        CAScheduler::schedule(schedule_selector(CDNewsAboutController::deleteCallBack), this, 0.01f);
    }
}

void CDNewsAboutController::tableViewDidDeselectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    
}

CATableViewCell* CDNewsAboutController::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
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
    cellText->setText(unicode_to_utf8(aboutMenuTag[row]));
    return cell;
    
}

unsigned int CDNewsAboutController::numberOfRowsInSection(CATableView *table, unsigned int section)
{
    return 5;
}

unsigned int CDNewsAboutController::numberOfSections(CATableView *table)
{
    return 1;
}
unsigned int CDNewsAboutController::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
    return _px(100);
}

unsigned int CDNewsAboutController::tableViewHeightForHeaderInSection(CATableView* table, unsigned int section)
{
    int h = 40;
    if (section==0) {
        h=30;
    }
    return _px(h);
}

unsigned int CDNewsAboutController::tableViewHeightForFooterInSection(CATableView* table, unsigned int section)
{
    return 1;
}

void CDNewsAboutController::btn_callback(CrossApp::CAControl *btn, CrossApp::CCPoint point)
{
    CAViewAnimation::beginAnimations("", NULL);
    CAViewAnimation::setAnimationDuration(0.3f);
    CAViewAnimation::setAnimationDidStopSelector(this, CAViewAnimation0_selector(CDNewsAboutController::dissModel));
    bg->setFrameOrigin(CCPointMake(-500, 0));
    this->getView()->setColor(ccc4(0,0,0,0));
    CAViewAnimation::commitAnimations();
    
}

void CDNewsAboutController::dissModel()
{
    RootWindow::getInstance()->getSplitNavigationController()->dismissModalViewController(false);
}
