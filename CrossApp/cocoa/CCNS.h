
#ifndef __PLATFOMR_CCNS_H__
#define __PLATFOMR_CCNS_H__

#include "basics/CAPoint.h"
#include "basics/CASize.h"
#include "basics/CARect.h"

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 */

/**
@brief Returns a Core Graphics rectangle structure corresponding to the data in a given string.
@param pszContent   A string object whose contents are of the form "{{x,y},{w, h}}",
                    where x is the x coordinate, y is the y coordinate, w is the width, and h is the height.
                    These components can represent integer or float values.
                    An example of a valid string is "{{3,2},{4,5}}".
                    The string is not localized, so items are always separated with a comma.
@return A Core Graphics structure that represents a rectangle.
        If the string is not well-formed, the function returns DRectZero.
*/
DRect CC_DLL DRectFromString(const char* pszContent);

/**
@brief Returns a Core Graphics point structure corresponding to the data in a given string.
@param pszContent   A string object whose contents are of the form "{x,y}",
                    where x is the x coordinate and y is the y coordinate.
                    The x and y values can represent integer or float values.
                    An example of a valid string is "{3.0,2.5}".
                    The string is not localized, so items are always separated with a comma.
@return A Core Graphics structure that represents a point.
        If the string is not well-formed, the function returns DPointZero.
*/
DPoint CC_DLL DPointFromString(const char* pszContent);

/**
@brief Returns a Core Graphics size structure corresponding to the data in a given string.
@param pszContent   A string object whose contents are of the form "{w, h}",
                    where w is the width and h is the height.
                    The w and h values can be integer or float values.
                    An example of a valid string is "{3.0,2.5}".
                    The string is not localized, so items are always separated with a comma.
@return A Core Graphics structure that represents a size.
        If the string is not well-formed, the function returns DSizeZero.
*/
DSize CC_DLL DSizeFromString(const char* pszContent);

// end of data_structure group
/// @}

NS_CC_END

#endif // __PLATFOMR_CCNS_H__


