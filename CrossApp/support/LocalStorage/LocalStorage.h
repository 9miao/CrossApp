

#ifndef __JSB_LOCALSTORAGE_H
#define __JSB_LOCALSTORAGE_H

#include <stdio.h>
#include <stdlib.h>
NS_CC_BEGIN

/** Initializes the database. If path is null, it will create an in-memory DB */
CC_DLL void localStorageInit(const char *fullpath);

/** Frees the allocated resources */
CC_DLL void localStorageFree();

/** sets an item in the LS */
CC_DLL void localStorageSetItem(const char *key, const char *value);

/** gets an item from the LS */
CC_DLL const char* localStorageGetItem(const char *key);

/** removes an item from the LS */
CC_DLL void localStorageRemoveItem(const char *key);

#endif // __JSB_LOCALSTORAGE_H
NS_CC_END
