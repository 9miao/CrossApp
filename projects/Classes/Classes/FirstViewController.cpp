#pragma execution_character_set("utf-8")
// 本文件为utf-8 编码格式

#include "FirstViewController.h"

FirstViewController::FirstViewController()
:clipNode(NULL)
{

}

FirstViewController::~FirstViewController()
{

}

void FirstViewController::viewDidLoad()
{
	clipNode = CAImageView::createWithFrame(this->getView()->getBounds());
	clipNode->setImage(CAImage::create("HelloWorld.png"));
	this->getView()->addSubview(clipNode);

	CALabel *lable = CALabel::createWithFrame(CCRect(10, 500, 300, 200));
	lable->setFontSize(34);
	lable->setText("sdlkweldkfsdljlsdfkwe");
	clipNode->addSubview(lable);

	CAView *view = CAView::createWithFrame(CCRect(0, 400, 400, 400), ccc4(0, 0, 0, 128));
	clipNode->addSubview(view);

	CAButton *jietuBtn = CAButton::createWithFrame(CCRect(200, 200, 100, 40), CAButtonTypeRoundedRect);
	jietuBtn->setTitleForState(CAControlStateAll, "截图");
	jietuBtn->setTitleColorForState(CAControlStateAll,CAColor_black);
	jietuBtn->addTarget(this, CAControl_selector(FirstViewController::onJietuBtnclick), CAControlEventTouchUpInSide);
	this->getView()->addSubview(jietuBtn);

	CAButton *lvjingBtn = CAButton::createWithFrame(CCRect(400, 200, 100, 40), CAButtonTypeRoundedRect);
	lvjingBtn->setTitleForState(CAControlStateAll, "滤镜");
	lvjingBtn->addTarget(this, CAControl_selector(FirstViewController::onLvjingBtnclick), CAControlEventTouchUpInSide);
	lvjingBtn->setTitleColorForState(CAControlStateAll, CAColor_black);
	this->getView()->addSubview(lvjingBtn);
}

void FirstViewController::viewDidUnload()
{
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

void FirstViewController::onLvjingBtnclick(CAControl *control, CCPoint point)
{
	CAImage *newImage = getBlackFilterEffect(clipNode->getImage());

	std::string imagePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "image/";
	CCFileUtils::sharedFileUtils()->createDirectory(imagePath.c_str());
	imagePath = imagePath + "lvjing.png";

	newImage->saveToFile(imagePath);
}

void FirstViewController::onJietuBtnclick(CAControl *control, CCPoint point)
{
	CARenderImage* render = CARenderImage::create(400, 400, CAImage::PixelFormat_RGB565);
	render->printscreenWithView(clipNode, CCPoint(0, 400), CAColor_clear);
	//render->setVisible(false);


	CAImageView *imageView = CAImageView::createWithImage(render->getImageView()->getImage());
	imageView->setFrame(CCRect(100, 100, 400, 400));
	//this->getView()->addSubview(imageView);

	std::string imagePath = CCFileUtils::sharedFileUtils()->getWritablePath() + "image/";
	CCFileUtils::sharedFileUtils()->createDirectory(imagePath.c_str());
	imagePath = imagePath + "jietu.jpg";

	imageView->getImage()->saveToFile(imagePath);

	CCLog("%f", CAApplication::getApplication()->getWinSize().width);
}


CAImage* FirstViewController::getBlackFilterEffect(CAImage *sender)
{
    unsigned char* _image =  static_cast<unsigned char*>(malloc(sender->getDataLenght() * sizeof(unsigned char)));
    memcpy(_image, sender->getData(), sender->getDataLenght());

	CAImage::PixelFormat pixelFormat = sender->getPixelFormat();

	if (pixelFormat == CAImage::PixelFormat_RGBA8888 || pixelFormat == CAImage::PixelFormat_RGBA4444)
	{
		for (int j = 0; j < sender->getPixelsHigh(); j++)
		for (int i = 0; i < sender->getPixelsWide(); i++)
		{
			unsigned char r = _image[(j * sender->getPixelsWide() + i) * 4 + 0];
			unsigned char g = _image[(j * sender->getPixelsWide() + i) * 4 + 1];
			unsigned char b = _image[(j * sender->getPixelsWide() + i) * 4 + 2];
			unsigned char newColor = 0.3*r + 0.59*g + 0.11*b;
			_image[(j * sender->getPixelsWide() + i) * 4 + 0] = newColor;
			_image[(j * sender->getPixelsWide() + i) * 4 + 1] = newColor;
			_image[(j * sender->getPixelsWide() + i) * 4 + 2] = newColor;
			_image[(j * sender->getPixelsWide() + i) * 4 + 3] = 255;
		}
	}
	else
	{
		for (int j = 0; j < sender->getPixelsHigh(); j++)
		for (int i = 0; i < sender->getPixelsWide(); i++)
		{
			unsigned char r = _image[(j * sender->getPixelsWide() + i) * 3 + 0];
			unsigned char g = _image[(j * sender->getPixelsWide() + i) * 3 + 1];
			unsigned char b = _image[(j * sender->getPixelsWide() + i) * 3 + 2];
			unsigned char newColor = 0.3*r + 0.59*g + 0.11*b;
			_image[(j * sender->getPixelsWide() + i) * 3 + 0] = newColor;
			_image[(j * sender->getPixelsWide() + i) * 3 + 1] = newColor;
			_image[(j * sender->getPixelsWide() + i) * 3 + 2] = newColor;
		}
	}
    CAImage* m_pBlackEffect = new CAImage();
	m_pBlackEffect->initWithRawData(_image, sender->getPixelFormat(), sender->getContentSize().width, sender->getContentSize().height);
    m_pBlackEffect->autorelease();
    clipNode->setImage(m_pBlackEffect);
    delete[] _image;

	return m_pBlackEffect;
}