
#ifndef __SUPPORT_CC_UTILS_H__
#define __SUPPORT_CC_UTILS_H__

/** @file ccUtils.h
Misc free functions
*/

namespace CrossApp {
/*
ccNextPOT function is licensed under the same license that is used in CAImage.m.
*/

/** returns the Next Power of Two value.

Examples:
- If "value" is 15, it will return 16.
- If "value" is 16, it will return 16.
- If "value" is 17, it will return 32.

@since v0.99.5
*/

unsigned long ccNextPOT( unsigned long value );

}

#endif // __SUPPORT_CC_UTILS_H__
