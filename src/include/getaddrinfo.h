/*-------------------------------------------------------------------------
 *
 * getaddrinfo.h
 *	  Support getaddrinfo() on platforms that don't have it.
 *
 * Note: we use our own routines on platforms that don't HAVE_STRUCT_ADDRINFO,
 * whether or not the library routine getaddrinfo() can be found.  This
 * policy is needed because on some platforms a manually installed libbind.a
 * may provide getaddrinfo(), yet the system headers may not provide the
 * struct definitions needed to call it.  To avoid conflict with the libbind
 * definition in such cases, we rename our routines to pg_xxx() via macros.
 *
 * This code will also work on platforms where struct addrinfo is defined
 * in the system headers but no getaddrinfo() can be located.
 *
 * Copyright (c) 2003-2009, PostgreSQL Global Development Group
 *
 * $PostgreSQL: pgsql/src/include/getaddrinfo.h,v 1.26 2009/01/01 17:23:55 momjian Exp $
 *
 *-------------------------------------------------------------------------
 */
#ifndef GETADDRINFO_H
#define GETADDRINFO_H

#include <sys/socket.h>
#include <netdb.h>


/* Various macros that ought to be in <netdb.h>, but might not be */

#ifndef EAI_FAIL
#ifndef WIN32
#define EAI_BADFLAGS	(-1)
#define EAI_NONAME		(-2)
#define EAI_AGAIN		(-3)
#define EAI_FAIL		(-4)
#define EAI_FAMILY		(-6)
#define EAI_SOCKTYPE	(-7)
#define EAI_SERVICE		(-8)
#define EAI_MEMORY		(-10)
#define EAI_SYSTEM		(-11)
#else							/* WIN32 */
#ifdef WIN32_ONLY_COMPILER
#ifndef WSA_NOT_ENOUGH_MEMORY
#define WSA_NOT_ENOUGH_MEMORY	(WSAENOBUFS)
#endif
#ifndef __BORLANDC__
#define WSATYPE_NOT_FOUND		(WSABASEERR+109)
#endif
#endif
#define EAI_AGAIN		WSATRY_AGAIN
#define EAI_BADFLAGS	WSAEINVAL
#define EAI_FAIL		WSANO_RECOVERY
#define EAI_FAMILY		WSAEAFNOSUPPORT
#define EAI_MEMORY		WSA_NOT_ENOUGH_MEMORY
#define EAI_NODATA		WSANO_DATA
#define EAI_NONAME		WSAHOST_NOT_FOUND
#define EAI_SERVICE		WSATYPE_NOT_FOUND
#define EAI_SOCKTYPE	WSAESOCKTNOSUPPORT
#endif   /* !WIN32 */
#endif   /* !EAI_FAIL */

#ifndef AI_PASSIVE
#define AI_PASSIVE		0x0001
#endif

#ifndef AI_NUMERICHOST
/*
 * some platforms don't support AI_NUMERICHOST; define as zero if using
 * the system version of getaddrinfo...
 */
#if defined(HAVE_STRUCT_ADDRINFO) && defined(HAVE_GETADDRINFO)
#define AI_NUMERICHOST	0
#else
#define AI_NUMERICHOST	0x0004
#endif
#endif

#ifndef NI_NUMERICHOST
#define NI_NUMERICHOST	1
#endif
#ifndef NI_NUMERICSERV
#define NI_NUMERICSERV	2
#endif

#ifndef NI_MAXHOST
#define NI_MAXHOST	1025
#endif
#ifndef NI_MAXSERV
#define NI_MAXSERV	32
#endif


#ifndef HAVE_STRUCT_ADDRINFO

#ifndef WIN32
struct addrinfo
{
	int			ai_flags;		// AI_PASSIVE, AI_CANONNAME
	int			ai_family;		// 地址族协议
	int			ai_socktype;	// 套接字类型
	int			ai_protocol;	// 0 或者 IPv4、IPv6 选项
	size_t		ai_addrlen;		// 套接口地址结构长度
	struct sockaddr *ai_addr;	// 套接口地址指针
	char	   *ai_canonname;	// 主机规范名
	struct addrinfo *ai_next;	// 指向下一个 struct addrinfo 的指针
};
#else
/*
 *	The order of the structure elements on Win32 doesn't match the
 *	order specified in the standard, but we have to match it for
 *	IPv6 to work.
 */
struct addrinfo
{
	int			ai_flags;
	int			ai_family;
	int			ai_socktype;
	int			ai_protocol;
	size_t		ai_addrlen;
	char	   *ai_canonname;
	struct sockaddr *ai_addr;
	struct addrinfo *ai_next;
};
#endif
#endif   /* HAVE_STRUCT_ADDRINFO */


#ifndef HAVE_GETADDRINFO

/* Rename private copies per comments above */
#ifdef getaddrinfo
#undef getaddrinfo
#endif
#define getaddrinfo pg_getaddrinfo

#ifdef freeaddrinfo
#undef freeaddrinfo
#endif
#define freeaddrinfo pg_freeaddrinfo

#ifdef gai_strerror
#undef gai_strerror
#endif
#define gai_strerror pg_gai_strerror

#ifdef getnameinfo
#undef getnameinfo
#endif
#define getnameinfo pg_getnameinfo

extern int getaddrinfo(const char *node, const char *service,
			const struct addrinfo * hints, struct addrinfo ** res);
extern void freeaddrinfo(struct addrinfo * res);
extern const char *gai_strerror(int errcode);
extern int getnameinfo(const struct sockaddr * sa, int salen,
			char *node, int nodelen,
			char *service, int servicelen, int flags);
#endif   /* HAVE_GETADDRINFO */

#endif   /* GETADDRINFO_H */
