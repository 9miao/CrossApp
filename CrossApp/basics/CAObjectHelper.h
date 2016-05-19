//
//  CASyncQueue.h
//  CrossApp
//
//  Created by Zhujian on 16-05-19.
//  Copyright (c) 2014 http://www.9miao.com All rights reserved.
//

#ifndef __CAObjectHelper_H__
#define __CAObjectHelper_H__

#include "CAObject.h"
#include "CASyncQueue.h"


NS_CC_BEGIN

typedef struct DelayTimerElement
{
	DelayTimerElement()
		: pOwnerObj(NULL)
		, func1(NULL)
		, func2(NULL)
		, pObj(NULL)
		, fInterval(0)
		, fCurrentTime(0)
	{

	}
	CAObject* pOwnerObj;
	SEL_CallFunc func1;
	SEL_CallFuncO func2;
	CAObject* pObj;
	float fInterval;
	float fCurrentTime;
}
tDelayTimerElement;


class CAObjectHelper : public CAObject
{
public:
    
	CAObjectHelper(void);

	virtual ~CAObjectHelper(void);

	static CAObjectHelper* getInstance();
    
	void performSelector(CAObject* pOwnerObj, SEL_CallFunc callFunc, float afterDelay);

	void performSelector(CAObject* pOwnerObj, SEL_CallFuncO callFunc, CAObject* objParam, float afterDelay);

protected:

	void updateDelayTimers(float dt);
	void releaseAllDelays();

private:
	CASyncQueue<tDelayTimerElement> m_vDelayTimerVect;
};

NS_CC_END

#endif // __CAObjectHelper_H__
