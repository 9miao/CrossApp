#ifndef GIFCONFIG_H
#define GIFCONFIG_H

#include <stdint.h>
#include <stdio.h>

#define MulS16(x, y)  ((int16_t)(x) * (y))
#define AlphaMul(value, alpha256)     (MulS16(value, alpha256) >> 8)

template <typename T> inline void TSwap(T& a, T& b) {
	     T c(a);
	     a = b;
	     b = c;
}


#define CREATE_WITH_PARAM(__TYPE__,_PARAM_) \
	static __TYPE__* create(_PARAM_ _param_) \
{ \
	__TYPE__ *pRet = new __TYPE__(); \
	if (pRet && pRet->init(_param_)) \
{ \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
} \
}

#define CREATE_CCOBJ_WITH_PARAM(__TYPE__,_PARAM_) \
	static __TYPE__* create(_PARAM_ _param_) \
{ \
	__TYPE__ *pRet = new __TYPE__(); \
	if (pRet && pRet->init(_param_)) \
{ \
	pRet->autorelease(); \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
} \
}

#define CREATE_CCOBJ_WITH_PARAMS(__TYPE__, _PARAM_1_, _PARAM_2_) \
	static __TYPE__* create(_PARAM_1_ _param_1_, _PARAM_2_ _param_2_) \
{ \
	__TYPE__ *pRet = new __TYPE__(); \
	if (pRet && pRet->init(_param_1_, _param_2_)) \
{ \
	pRet->autorelease(); \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
} \
}

#endif //GIFCONFIG_H

