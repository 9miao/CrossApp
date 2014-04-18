angleproject
============

WinRT (Windows 8.0 and 8.1) and Windows Phone 8.0 contributions by Dale Stammen (https://github.com/stammen) and Steven Chith (https://github.com/steven-chith)

Angle Project (https://code.google.com/p/angleproject/) with support for Windows Store Apps (WinRT) and Windows Phone Apps (WP8). This version supports
developing Windows Store Apps using Visual Studio 2013 and Windows 8.1. It is also possbile to develop a Windows Store App
using Visual Studio 2012 and Windows 8.0 but this version will not be accepted in the Windows App Store.

This version is based on ANGLE v1.2.0.2446

This version adds support for Windows Phone 8.0 and Windows 8.0 WinRT Store Apps using precompiled shaders. For an example, see samples/RotatingCube/RotatingCube_phone.sln.

ANGLE
=====
The goal of ANGLE is to allow Windows users to seamlessly run WebGL and other OpenGL ES 2.0 content by translating OpenGL ES 2.0 API calls to DirectX 9 or DirectX 11 API calls.

ANGLE is a conformant implementation of the OpenGL ES 2.0 specification that is hardware‐accelerated via Direct3D. ANGLE v1.0.772 was certified compliant by passing the ES 2.0.3 conformance tests in October 2011. ANGLE also provides an implementation of the EGL 1.4 specification.

ANGLE is used as the default WebGL backend for both Google Chrome and Mozilla Firefox on Windows platforms. Chrome uses ANGLE for all graphics rendering on Windows, including the accelerated Canvas2D implementation and the Native Client sandbox environment.

Portions of the ANGLE shader compiler are used as a shader validator and translator by WebGL imp38lementations across multiple platforms. It is used on Mac OS X, Linux, and in mobile variants of the browsers. Having one shader validator helps to ensure that a consistent set of GLSL ES shaders are accepted across browsers and platforms. The shader translator can be used to translate shaders to other shading languages, and to optionally apply shader modifications to work around bugs or quirks in the native graphics drivers. The translator targets Desktop GLSL, Direct3D 
