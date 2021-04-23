/* lws_config.h  Generated from lws_config.h.in  */

#ifndef NDEBUG
	#ifndef _DEBUG
		#define _DEBUG
	#endif
#endif

#define LWS_INSTALL_DATADIR "/Users/cocos/Github/cocos2d-x-3rd-party-libs-src/contrib/install-android/arm64/share"

/* Define to 1 to use wolfSSL/CyaSSL as a replacement for OpenSSL.
 * LWS_OPENSSL_SUPPORT needs to be set also for this to work. */
/* #undef USE_WOLFSSL */

/* Also define to 1 (in addition to USE_WOLFSSL) when using the
  (older) CyaSSL library */
/* #undef USE_OLD_CYASSL */
/* #undef LWS_WITH_BORINGSSL */

/* #undef LWS_WITH_MBEDTLS */
/* #undef LWS_WITH_POLARSSL */
/* #undef LWS_WITH_ESP8266 */
/* #undef LWS_WITH_ESP32 */

/* #undef LWS_WITH_PLUGINS */
/* #undef LWS_WITH_NO_LOGS */

/* The Libwebsocket version */
#define LWS_LIBRARY_VERSION "2.4.2"

#define LWS_LIBRARY_VERSION_MAJOR 2
#define LWS_LIBRARY_VERSION_MINOR 4
#define LWS_LIBRARY_VERSION_PATCH 2
/* LWS_LIBRARY_VERSION_NUMBER looks like 1005001 for e.g. version 1.5.1 */
#define LWS_LIBRARY_VERSION_NUMBER (LWS_LIBRARY_VERSION_MAJOR*1000000)+(LWS_LIBRARY_VERSION_MINOR*1000)+LWS_LIBRARY_VERSION_PATCH

/* The current git commit hash that we're building from */
#define LWS_BUILD_HASH "cocos@cocoss-MacBook-Pro.local-v3.4-182-g408ba56"

/* Build with OpenSSL support */
#define LWS_OPENSSL_SUPPORT

/* The client should load and trust CA root certs it finds in the OS */
#define LWS_SSL_CLIENT_USE_OS_CA_CERTS

/* Sets the path where the client certs should be installed. */
#define LWS_OPENSSL_CLIENT_CERTS "../share"

/* Turn off websocket extensions */
/* #undef LWS_NO_EXTENSIONS */

/* Enable libev io loop */
/* #undef LWS_WITH_LIBEV */

/* Enable libuv io loop */
#define LWS_WITH_LIBUV

/* Enable libevent io loop */
/* #undef LWS_WITH_LIBEVENT */

/* Build with support for ipv6 */
#define LWS_WITH_IPV6

/* Build with support for UNIX domain socket */
/* #undef LWS_WITH_UNIX_SOCK */

/* Build with support for HTTP2 */
/* #undef LWS_WITH_HTTP2 */

/* Turn on latency measuring code */
/* #undef LWS_LATENCY */

/* Don't build the daemonizeation api */
#define LWS_NO_DAEMONIZE

/* Build without server support */
/* #undef LWS_NO_SERVER */

/* Build without client support */
/* #undef LWS_NO_CLIENT */

/* If we should compile with MinGW support */
/* #undef LWS_MINGW_SUPPORT */

/* Use the BSD getifaddrs that comes with libwebsocket, for uclibc support */
#define LWS_BUILTIN_GETIFADDRS

/* use SHA1() not internal libwebsockets_SHA1 */
/* #undef LWS_SHA1_USE_OPENSSL_NAME */

/* SSL server using ECDH certificate */
/* #undef LWS_SSL_SERVER_WITH_ECDH_CERT */
/* #undef LWS_HAVE_SSL_CTX_set1_param */
/* #undef LWS_HAVE_X509_VERIFY_PARAM_set1_host */
/* #undef LWS_HAVE_RSA_SET0_KEY */

/* #undef LWS_HAVE_UV_VERSION_H */

/* CGI apis */
/* #undef LWS_WITH_CGI */

/* whether the Openssl is recent enough, and / or built with, ecdh */
#define LWS_HAVE_OPENSSL_ECDH_H

/* HTTP Proxy support */
/* #undef LWS_WITH_HTTP_PROXY */

/* HTTP Ranges support */
#define LWS_WITH_RANGES

/* Http access log support */
/* #undef LWS_WITH_ACCESS_LOG */
/* #undef LWS_WITH_SERVER_STATUS */

/* #undef LWS_WITH_STATEFUL_URLDECODE */
/* #undef LWS_WITH_PEER_LIMITS */

/* Maximum supported service threads */
#define LWS_MAX_SMP 1

/* Lightweight JSON Parser */
/* #undef LWS_WITH_LEJP */

/* SMTP */
/* #undef LWS_WITH_SMTP */

/* OPTEE */
/* #undef LWS_PLAT_OPTEE */

/* ZIP FOPS */
#define LWS_WITH_ZIP_FOPS
#define LWS_HAVE_STDINT_H

/* #undef LWS_AVOID_SIGPIPE_IGN */

/* #undef LWS_FALLBACK_GETHOSTBYNAME */

/* #undef LWS_WITH_STATS */
/* #undef LWS_WITH_SOCKS5 */

#define LWS_HAVE_SYS_CAPABILITY_H
/* #undef LWS_HAVE_LIBCAP */

#define LWS_HAVE_ATOLL
/* #undef LWS_HAVE__ATOI64 */
/* #undef LWS_HAVE__STAT32I64 */

/* OpenSSL various APIs */

/* #undef LWS_HAVE_TLS_CLIENT_METHOD */
/* #undef LWS_HAVE_TLSV1_2_CLIENT_METHOD */
/* #undef LWS_HAVE_SSL_SET_INFO_CALLBACK */

#define LWS_HAS_INTPTR_T


