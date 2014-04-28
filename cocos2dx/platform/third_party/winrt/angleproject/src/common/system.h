//
// Copyright (c) 2002-2012 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//

// system.h: Includes Windows system headers and undefines macros that conflict.

#ifndef COMMON_SYSTEM_H
#define COMMON_SYSTEM_H

#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>

#if defined(WINAPI_FAMILY)
#if !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)
#define ANGLE_PLATFORM_WINRT
#endif
#endif // !WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
#endif // #if defined(WINAPI_FAMILY)

// check if Windows Phone 8
#if defined(WINAPI_FAMILY)
#if defined(WINAPI_PARTITION_PHONE) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_PHONE)
#define ANGLE_PLATFORM_WP8
#ifndef ANGLE_PLATFORM_WINRT
#define ANGLE_PLATFORM_WINRT
#endif
#endif // #if defined(WINAPI_PARTITION_PHONE) && WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_PHONE)
#endif // #if defined(WINAPI_FAMILY)


#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

#endif   // COMMON_SYSTEM_H
