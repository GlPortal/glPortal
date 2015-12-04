/*
 *  Determine platform features, select feature selection defines
 *  (e.g. _XOPEN_SOURCE), include system headers, and define DUK_USE_xxx
 *  defines which are (only) checked in Duktape internal code for
 *  activated features.  Duktape feature selection is based on automatic
 *  feature detection, user supplied DUK_OPT_xxx defines, and optionally
 *  a "duk_custom.h" user header (if DUK_OPT_HAVE_CUSTOM_H is defined).
 *
 *  When compiling Duktape, DUK_COMPILING_DUKTAPE is set, and this file
 *  is included before any system headers are included.  Feature selection
 *  defines (e.g. _XOPEN_SOURCE) are defined here before any system headers
 *  are included (which is a requirement for system headers to work correctly).
 *  This file is responsible for including all system headers and contains
 *  all platform dependent cruft in general.  When compiling user code,
 *  DUK_COMPILING_DUKTAPE is not defined, and we must avoid e.g. defining
 *  unnecessary feature selection defines.
 *
 *  The general order of handling:
 *    - Compiler feature detection (require no includes)
 *    - Intermediate platform detection (-> easier platform defines)
 *    - Platform detection, system includes, byte order detection, etc
 *    - ANSI C wrappers (e.g. DUK_MEMCMP), wrappers for constants, etc
 *    - DUK_USE_xxx defines are resolved based on input defines
 *    - Duktape Date provider settings
 *    - Final sanity checks
 *
 *  DUK_F_xxx are internal feature detection macros which should not be
 *  used outside this header.
 *
 *  Useful resources:
 *
 *    http://sourceforge.net/p/predef/wiki/Home/
 *    http://sourceforge.net/p/predef/wiki/Architectures/
 *    http://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor
 *    http://en.wikipedia.org/wiki/C_data_types#Fixed-width_integer_types
 *
 *  Preprocessor defines available in a particular GCC:
 *
 *    gcc -dM -E - </dev/null   # http://www.brain-dump.org/blog/entry/107
 */

#ifndef DUK_CONFIG_H_INCLUDED
#define DUK_CONFIG_H_INCLUDED

/*
 *  Compiler features
 */

#undef DUK_F_C99
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
#define DUK_F_C99
#endif

#undef DUK_F_CPP
#if defined(__cplusplus)
#define DUK_F_CPP
#endif

#undef DUK_F_CPP11
#if defined(__cplusplus) && (__cplusplus >= 201103L)
#define DUK_F_CPP11
#endif

/*
 *  Provides the duk_rdtsc() inline function (if available), limited to
 *  GCC C99.
 *
 *  See: http://www.mcs.anl.gov/~kazutomo/rdtsc.html
 */

/* XXX: more accurate detection of what gcc versions work; more inline
 * asm versions for other compilers.
 */
#if defined(__GNUC__) && defined(__i386__) && defined(DUK_F_C99) && \
    !defined(__cplusplus) /* unsigned long long not standard */
static __inline__ unsigned long long duk_rdtsc(void) {
	unsigned long long int x;
	__asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
	return x;
}
#define DUK_USE_RDTSC()  duk_rdtsc()
#elif defined(__GNUC__) && defined(__x86_64__) && defined(DUK_F_C99) && \
    !defined(__cplusplus) /* unsigned long long not standard */
static __inline__ unsigned long long duk_rdtsc(void) {
	unsigned hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ((unsigned long long) lo) | (((unsigned long long) hi) << 32);
}
#define DUK_USE_RDTSC()  duk_rdtsc()
#else
/* not available */
#undef DUK_USE_RDTSC
#endif

/*
 *  Intermediate platform, architecture, and compiler detection.  These are
 *  hopelessly intertwined - e.g. architecture defines depend on compiler etc.
 *
 *  Provide easier defines for platforms and compilers which are often tricky
 *  or verbose to detect.  The intent is not to provide intermediate defines for
 *  all features; only if existing feature defines are inconvenient.
 */

/* Intel x86 (32-bit) */
#if defined(i386) || defined(__i386) || defined(__i386__) || \
    defined(__i486__) || defined(__i586__) || defined(__i686__) || \
    defined(__IA32__) || defined(_M_IX86) || defined(__X86__) || \
    defined(_X86_) || defined(__THW_INTEL__) || defined(__I86__)
#define DUK_F_X86
#endif

/* AMD64 (64-bit) */
#if defined(__amd64__) || defined(__amd64) || \
    defined(__x86_64__) || defined(__x86_64) || \
    defined(_M_X64) || defined(_M_AMD64)
#define DUK_F_X64
#endif

/* X32: 64-bit with 32-bit pointers (allows packed tvals).  X32 support is
 * not very mature yet.
 *
 * https://sites.google.com/site/x32abi/
 */
#if defined(DUK_F_X64) && \
    (defined(_ILP32) || defined(__ILP32__))
#define DUK_F_X32
/* define only one of: DUK_F_X86, DUK_F_X32, or DUK_F_X64 */
#undef DUK_F_X64
#undef DUK_F_X86
#endif

/* ARM */
#if defined(__arm__) || defined(__thumb__) || defined(_ARM) || defined(_M_ARM)
#define DUK_F_ARM
#endif

/* MIPS */
/* Related defines: __MIPSEB__, __MIPSEL__, __mips_isa_rev, __LP64__ */
#if defined(__mips__) || defined(mips) || defined(_MIPS_ISA) || \
    defined(_R3000) || defined(_R4000) || defined(_R5900) || \
    defined(_MIPS_ISA_MIPS1) || defined(_MIPS_ISA_MIPS2) || \
    defined(_MIPS_ISA_MIPS3) || defined(_MIPS_ISA_MIPS4) || \
    defined(__mips) || defined(__MIPS__)
#define DUK_F_MIPS
#if defined(__LP64__) || defined(__mips64) || defined(__mips64__) || \
    defined(__mips_n64)
#define DUK_F_MIPS64
#else
#define DUK_F_MIPS32
#endif
#endif

/* SuperH */
#if defined(__sh__) || \
    defined(__sh1__) || defined(__SH1__) || \
    defined(__sh2__) || defined(__SH2__) || \
    defined(__sh3__) || defined(__SH3__) || \
    defined(__sh4__) || defined(__SH4__) || \
    defined(__sh5__) || defined(__SH5__)
#define DUK_F_SUPERH
#endif

/* Motorola 68K.  Not defined by VBCC, so user must define one of these
 * manually when using VBCC.
 */
#if defined(__m68k__) || defined(M68000) || defined(__MC68K__)
#define DUK_F_M68K
#endif

/* PowerPC */
#if defined(__powerpc) || defined(__powerpc__) || defined(__PPC__)
#define DUK_F_PPC
#if defined(__PPC64__)
#define DUK_F_PPC64
#else
#define DUK_F_PPC32
#endif
#endif

/* Linux */
#if defined(__linux) || defined(__linux__) || defined(linux)
#define DUK_F_LINUX
#endif

/* FreeBSD */
#if defined(__FreeBSD__) || defined(__FreeBSD)
#define DUK_F_FREEBSD
#endif

/* NetBSD */
#if defined(__NetBSD__) || defined(__NetBSD)
#define DUK_F_NETBSD
#endif

/* OpenBSD */
#if defined(__OpenBSD__) || defined(__OpenBSD)
#define DUK_F_OPENBSD
#endif

/* BSD variant */
#if defined(DUK_F_FREEBSD) || defined(DUK_F_NETBSD) || defined(DUK_F_OPENBSD) || \
    defined(__bsdi__) || defined(__DragonFly__)
#define DUK_F_BSD
#endif

/* Generic Unix (includes Cygwin) */
#if defined(__unix) || defined(__unix__) || defined(unix) || \
    defined(DUK_F_LINUX) || defined(DUK_F_BSD)
#define DUK_F_UNIX
#endif

/* Cygwin */
#if defined(__CYGWIN__)
#define DUK_F_CYGWIN
#endif

/* Windows (32-bit or above) */
#if defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || \
    defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#define DUK_F_WINDOWS
#endif

#if defined(__APPLE__)
#define DUK_F_APPLE
#endif

/* Atari ST TOS. __TOS__ defined by PureC (which doesn't work as a target now
 * because int is 16-bit, to be fixed).  No platform define in VBCC apparently,
 * so to use with VBCC, user must define '__TOS__' manually.
  */
#if defined(__TOS__)
#define DUK_F_TOS
#endif

/* AmigaOS.  Neither AMIGA nor __amigaos__ is defined on VBCC, so user must
 * define 'AMIGA' manually.
 */
#if defined(AMIGA) || defined(__amigaos__)
#define DUK_F_AMIGAOS
#endif

/* Flash player (e.g. Crossbridge) */
#if defined(__FLASHPLAYER__)
#define DUK_F_FLASHPLAYER
#endif

/* Emscripten (provided explicitly by user), improve if possible */
#if defined(EMSCRIPTEN)
#define DUK_F_EMSCRIPTEN
#endif

/* QNX */
#if defined(__QNX__)
#define DUK_F_QNX
#endif

/* TI-Nspire (using Ndless) */
#if defined(_TINSPIRE)
#define DUK_F_TINSPIRE
#endif

/* GCC and GCC version convenience define. */
#if defined(__GNUC__)
#define DUK_F_GCC
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && defined(__GNUC_PATCHLEVEL__)
/* Convenience, e.g. gcc 4.5.1 == 40501; http://stackoverflow.com/questions/6031819/emulating-gccs-builtin-unreachable */
#define DUK_F_GCC_VERSION  (__GNUC__ * 10000L + __GNUC_MINOR__ * 100L + __GNUC_PATCHLEVEL__)
#else
#error cannot figure out gcc version
#endif
#endif

/* Clang */
#if defined(__clang__)
#define DUK_F_CLANG
/* It seems clang also defines __GNUC__, so undo the GCC detection. */
#if defined(DUK_F_GCC)
#undef DUK_F_GCC
#endif
#if defined(DUK_F_GCC_VERSION)
#undef DUK_F_GCC_VERSION
#endif
#endif

/* MSVC */
#if defined(_MSC_VER)
/* MSVC preprocessor defines: http://msdn.microsoft.com/en-us/library/b0084kay.aspx
 * _MSC_FULL_VER includes the build number, but it has at least two formats, see e.g.
 * BOOST_MSVC_FULL_VER in http://www.boost.org/doc/libs/1_52_0/boost/config/compiler/visualc.hpp
 */
#define DUK_F_MSVC
#if defined(_MSC_FULL_VER)
#if (_MSC_FULL_VER > 100000000)
#define DUK_F_MSVC_FULL_VER _MSC_FULL_VER
#else
#define DUK_F_MSCV_FULL_VER (_MSC_FULL_VER * 10)
#endif
#endif
#endif  /* _MSC_VER */

/* MinGW */
#if defined(__MINGW32__) || defined(__MINGW64__)
/* NOTE: Also GCC flags are detected (DUK_F_GCC etc). */
#define DUK_F_MINGW
#endif

/* BCC (Bruce's C compiler): this is a "torture target" for compilation */
#if defined(__BCC__) || defined(__BCC_VERSION__)
#define DUK_F_BCC
#endif

#if defined(__VBCC__)
#define DUK_F_VBCC
#endif

/*
 *  Platform detection, system includes, Date provider selection.
 *
 *  Feature selection (e.g. _XOPEN_SOURCE) must happen before any system
 *  headers are included.  This header should avoid providing any feature
 *  selection defines when compiling user code (only when compiling Duktape
 *  itself).  If a feature selection option is required for user code to
 *  compile correctly (e.g. it is needed for type detection), it should
 *  probably be -checked- here, not defined here.
 *
 *  Date provider selection seems a bit out-of-place here, but since
 *  the date headers and provider functions are heavily platform
 *  specific, there's little point in duplicating the platform if-else
 *  ladder.  All platform specific Date provider functions are in
 *  duk_bi_date.c; here we provide appropriate #defines to enable them,
 *  and include all the necessary system headers so that duk_bi_date.c
 *  compiles.  Date "providers" are:
 *
 *    NOW = getting current time (required)
 *    TZO = getting local time offset (required)
 *    PRS = parse datetime (optional)
 *    FMT = format datetime (optional)
 *
 *  There's a lot of duplication here, unfortunately, because many
 *  platforms have similar (but not identical) headers, Date providers,
 *  etc.  The duplication could be removed by more complicated nested
 *  #ifdefs, but it would then be more difficult to make fixes which
 *  affect only a specific platform.
 *
 *  XXX: add a way to provide custom functions to provide the critical
 *  primitives; this would be convenient when porting to unknown platforms
 *  (rather than muck with Duktape internals).
 */

#if defined(DUK_COMPILING_DUKTAPE) && \
 (defined(DUK_F_LINUX) || defined(DUK_F_EMSCRIPTEN))
/* A more recent Emscripten (2014-05) seems to lack "linux" environment
 * defines, so check for Emscripten explicitly.
 */
#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE  200809L
#endif
#ifndef _GNU_SOURCE
#define _GNU_SOURCE      /* e.g. getdate_r */
#endif
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE    /* e.g. strptime */
#endif
#endif

#if defined(DUK_F_QNX) && defined(DUK_COMPILING_DUKTAPE)
/* See: /opt/qnx650/target/qnx6/usr/include/sys/platform.h */
#define _XOPEN_SOURCE    600
#define _POSIX_C_SOURCE  200112L
#endif

#undef DUK_F_MSVC_CRT_SECURE
#if defined(DUK_F_WINDOWS) && defined(_MSC_VER)
/* http://msdn.microsoft.com/en-us/library/8ef0s5kh.aspx
 * http://msdn.microsoft.com/en-us/library/wd3wzwts.aspx
 * Seem to be available since VS2005.
 */
#if (_MSC_VER >= 1400)
/* VS2005+, secure CRT functions are preferred.  Windows Store applications
 * (and probably others) should use these.
 */
#define DUK_F_MSVC_CRT_SECURE
#endif
#if (_MSC_VER < 1700)
/* VS2012+ has stdint.h, < VS2012 does not (but it's available for download). */
#define DUK_F_NO_STDINT_H
#endif
/* Initial fix: disable secure CRT related warnings when compiling Duktape
 * itself (must be defined before including Windows headers).  Don't define
 * for user code including duktape.h.
 */
#if defined(DUK_COMPILING_DUKTAPE) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif  /* DUK_F_WINDOWS && _MSC_VER */

#if defined(DUK_F_TOS) || defined(DUK_F_BCC)
#define DUK_F_NO_STDINT_H
#endif

/* Workaround for older C++ compilers before including <inttypes.h>,
 * see e.g.: https://sourceware.org/bugzilla/show_bug.cgi?id=15366
 */
#if defined(__cplusplus) && !defined(__STDC_LIMIT_MACROS)
#define __STDC_LIMIT_MACROS
#endif
#if defined(__cplusplus) && !defined(__STDC_CONSTANT_MACROS)
#define __STDC_CONSTANT_MACROS
#endif

#if defined(__APPLE__)
/* Mac OSX, iPhone, Darwin */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <TargetConditionals.h>
#include <architecture/byte_order.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_OPENBSD)
/* http://www.monkey.org/openbsd/archive/ports/0401/msg00089.html */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <sys/endian.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_BSD)
/* other BSD */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <sys/endian.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_TOS)
/* Atari ST TOS */
#define DUK_USE_DATE_NOW_TIME
#define DUK_USE_DATE_TZO_GMTIME
/* no parsing (not an error) */
#define DUK_USE_DATE_FMT_STRFTIME
#include <limits.h>
#include <time.h>
#elif defined(DUK_F_AMIGAOS)
#if defined(DUK_F_M68K)
/* AmigaOS on M68k */
#define DUK_USE_DATE_NOW_TIME
#define DUK_USE_DATE_TZO_GMTIME
/* no parsing (not an error) */
#define DUK_USE_DATE_FMT_STRFTIME
#include <limits.h>
#include <time.h>
#elif defined(DUK_F_PPC)
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <limits.h>
#include <time.h>
#ifndef UINTPTR_MAX
#define UINTPTR_MAX UINT_MAX
#endif
#else
#error AmigaOS but not M68K/PPC, not supported now
#endif
#elif defined(DUK_F_WINDOWS)
/* Windows 32-bit and 64-bit are currently the same. */
/* MSVC does not have sys/param.h */
#define DUK_USE_DATE_NOW_WINDOWS
#define DUK_USE_DATE_TZO_WINDOWS
/* Note: PRS and FMT are intentionally left undefined for now.  This means
 * there is no platform specific date parsing/formatting but there is still
 * the ISO 8601 standard format.
 */
#if defined(DUK_COMPILING_DUKTAPE)
#include <windows.h>
#endif
#include <limits.h>
#elif defined(DUK_F_FLASHPLAYER)
/* Crossbridge */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <endian.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_QNX)
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_TINSPIRE)
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_LINUX)
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#if defined(DUK_F_BCC)
/* no endian.h */
#else
#include <endian.h>
#endif  /* DUK_F_BCC */
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(__posix)
/* POSIX */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <endian.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#elif defined(DUK_F_CYGWIN)
/* Cygwin -- don't use strptime() for now */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#include <endian.h>
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#else
/* Other UNIX, hopefully others */
#define DUK_USE_DATE_NOW_GETTIMEOFDAY
#define DUK_USE_DATE_TZO_GMTIME_R
#define DUK_USE_DATE_PRS_STRPTIME
#define DUK_USE_DATE_FMT_STRFTIME
#include <sys/types.h>
#if defined(DUK_F_BCC)
/* no endian.h */
#else
#include <endian.h>
#endif  /* DUK_F_BCC */
#include <limits.h>
#include <sys/param.h>
#include <sys/time.h>
#include <time.h>
#endif

/* Shared includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>  /* varargs */
#include <setjmp.h>
#include <stddef.h>  /* e.g. ptrdiff_t */
#if defined(DUK_F_NO_STDINT_H)
/* stdint.h not available */
#else
/* Technically C99 (C++11) but found in many systems.  Note the workaround
 * above for some C++ compilers (__STDC_LIMIT_MACROS etc).
 */
#include <stdint.h>
#endif
#include <math.h>

#if (defined(DUK_F_C99) || defined(DUK_F_CPP11)) && \
    !defined(DUK_F_BCC)
/* ULL / LL preprocessor constants should be avoided because they're not
 * always available.  With suitable options, some compilers will support
 * 64-bit integer types but won't support ULL / LL preprocessor constants.
 * Assume C99/C++11 environments have these.  However, BCC is nominally
 * C99 but doesn't support these constants.
 */
#define DUK_F_ULL_CONSTS
#endif

/*
 *  Detection for specific libc variants (like uclibc) and other libc specific
 *  features.  Potentially depends on the #includes above.
 */

#if defined(__UCLIBC__)
#define DUK_F_UCLIBC
#endif

/*
 *  Wrapper typedefs and constants for integer types, also sanity check types.
 *
 *  C99 typedefs are quite good but not always available, and we want to avoid
 *  forcibly redefining the C99 typedefs.  So, there are Duktape wrappers for
 *  all C99 typedefs and Duktape code should only use these typedefs.  Type
 *  detection when C99 is not supported is best effort and may end up detecting
 *  some types incorrectly.
 *
 *  Pointer sizes are a portability problem: pointers to different types may
 *  have a different size and function pointers are very difficult to manage
 *  portably.
 *
 *  http://en.wikipedia.org/wiki/C_data_types#Fixed-width_integer_types
 *
 *  Note: there's an interesting corner case when trying to define minimum
 *  signed integer value constants which leads to the current workaround of
 *  defining e.g. -0x80000000 as (-0x7fffffffL - 1L).  See doc/code-issues.txt
 *  for a longer discussion.
 *
 *  Note: avoid typecasts and computations in macro integer constants as they
 *  can then no longer be used in macro relational expressions (such as
 *  #if DUK_SIZE_MAX < 0xffffffffUL).  There is internal code which relies on
 *  being able to compare DUK_SIZE_MAX against a limit.
 */

/* XXX: add feature options to force basic types from outside? */

#if !defined(INT_MAX)
#error INT_MAX not defined
#endif

/* Check that architecture is two's complement, standard C allows e.g.
 * INT_MIN to be -2**31+1 (instead of -2**31).
 */
#if defined(INT_MAX) && defined(INT_MIN)
#if INT_MAX != -(INT_MIN + 1)
#error platform does not seem complement of two
#endif
#else
#error cannot check complement of two
#endif

/* Pointer size determination based on architecture.
 * XXX: unsure about BCC correctness.
 */
#if defined(DUK_F_X86) || defined(DUK_F_X32) || \
    defined(DUK_F_BCC) || \
    (defined(__WORDSIZE) && (__WORDSIZE == 32))
#define DUK_F_32BIT_PTRS
#elif defined(DUK_F_X64) || \
      (defined(__WORDSIZE) && (__WORDSIZE == 64))
#define DUK_F_64BIT_PTRS
#else
/* not sure, not needed with C99 anyway */
#endif

/* Intermediate define for 'have inttypes.h' */
#undef DUK_F_HAVE_INTTYPES
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) && \
    !(defined(DUK_F_AMIGAOS) && defined(DUK_F_VBCC))
/* vbcc + AmigaOS has C99 but no inttypes.h */
#define DUK_F_HAVE_INTTYPES
#elif defined(__cplusplus) && (__cplusplus >= 201103L)
/* C++11 apparently ratified stdint.h */
#define DUK_F_HAVE_INTTYPES
#endif

/* Basic integer typedefs and limits, preferably from inttypes.h, otherwise
 * through automatic detection.
 */
#if defined(DUK_F_HAVE_INTTYPES)
/* C99 or compatible */

#define DUK_F_HAVE_64BIT
#include <inttypes.h>

typedef uint8_t duk_uint8_t;
typedef int8_t duk_int8_t;
typedef uint16_t duk_uint16_t;
typedef int16_t duk_int16_t;
typedef uint32_t duk_uint32_t;
typedef int32_t duk_int32_t;
typedef uint64_t duk_uint64_t;
typedef int64_t duk_int64_t;
typedef uint_least8_t duk_uint_least8_t;
typedef int_least8_t duk_int_least8_t;
typedef uint_least16_t duk_uint_least16_t;
typedef int_least16_t duk_int_least16_t;
typedef uint_least32_t duk_uint_least32_t;
typedef int_least32_t duk_int_least32_t;
typedef uint_least64_t duk_uint_least64_t;
typedef int_least64_t duk_int_least64_t;
typedef uint_fast8_t duk_uint_fast8_t;
typedef int_fast8_t duk_int_fast8_t;
typedef uint_fast16_t duk_uint_fast16_t;
typedef int_fast16_t duk_int_fast16_t;
typedef uint_fast32_t duk_uint_fast32_t;
typedef int_fast32_t duk_int_fast32_t;
typedef uint_fast64_t duk_uint_fast64_t;
typedef int_fast64_t duk_int_fast64_t;
typedef uintptr_t duk_uintptr_t;
typedef intptr_t duk_intptr_t;
typedef uintmax_t duk_uintmax_t;
typedef intmax_t duk_intmax_t;

#define DUK_UINT8_MIN         0
#define DUK_UINT8_MAX         UINT8_MAX
#define DUK_INT8_MIN          INT8_MIN
#define DUK_INT8_MAX          INT8_MAX
#define DUK_UINT_LEAST8_MIN   0
#define DUK_UINT_LEAST8_MAX   UINT_LEAST8_MAX
#define DUK_INT_LEAST8_MIN    INT_LEAST8_MIN
#define DUK_INT_LEAST8_MAX    INT_LEAST8_MAX
#define DUK_UINT_FAST8_MIN    0
#define DUK_UINT_FAST8_MAX    UINT_FAST8_MAX
#define DUK_INT_FAST8_MIN     INT_FAST8_MIN
#define DUK_INT_FAST8_MAX     INT_FAST8_MAX
#define DUK_UINT16_MIN        0
#define DUK_UINT16_MAX        UINT16_MAX
#define DUK_INT16_MIN         INT16_MIN
#define DUK_INT16_MAX         INT16_MAX
#define DUK_UINT_LEAST16_MIN  0
#define DUK_UINT_LEAST16_MAX  UINT_LEAST16_MAX
#define DUK_INT_LEAST16_MIN   INT_LEAST16_MIN
#define DUK_INT_LEAST16_MAX   INT_LEAST16_MAX
#define DUK_UINT_FAST16_MIN   0
#define DUK_UINT_FAST16_MAX   UINT_FAST16_MAX
#define DUK_INT_FAST16_MIN    INT_FAST16_MIN
#define DUK_INT_FAST16_MAX    INT_FAST16_MAX
#define DUK_UINT32_MIN        0
#define DUK_UINT32_MAX        UINT32_MAX
#define DUK_INT32_MIN         INT32_MIN
#define DUK_INT32_MAX         INT32_MAX
#define DUK_UINT_LEAST32_MIN  0
#define DUK_UINT_LEAST32_MAX  UINT_LEAST32_MAX
#define DUK_INT_LEAST32_MIN   INT_LEAST32_MIN
#define DUK_INT_LEAST32_MAX   INT_LEAST32_MAX
#define DUK_UINT_FAST32_MIN   0
#define DUK_UINT_FAST32_MAX   UINT_FAST32_MAX
#define DUK_INT_FAST32_MIN    INT_FAST32_MIN
#define DUK_INT_FAST32_MAX    INT_FAST32_MAX
#define DUK_UINT64_MIN        0
#define DUK_UINT64_MAX        UINT64_MAX
#define DUK_INT64_MIN         INT64_MIN
#define DUK_INT64_MAX         INT64_MAX
#define DUK_UINT_LEAST64_MIN  0
#define DUK_UINT_LEAST64_MAX  UINT_LEAST64_MAX
#define DUK_INT_LEAST64_MIN   INT_LEAST64_MIN
#define DUK_INT_LEAST64_MAX   INT_LEAST64_MAX
#define DUK_UINT_FAST64_MIN   0
#define DUK_UINT_FAST64_MAX   UINT_FAST64_MAX
#define DUK_INT_FAST64_MIN    INT_FAST64_MIN
#define DUK_INT_FAST64_MAX    INT_FAST64_MAX

#define DUK_UINTPTR_MIN       0
#define DUK_UINTPTR_MAX       UINTPTR_MAX
#define DUK_INTPTR_MIN        INTPTR_MIN
#define DUK_INTPTR_MAX        INTPTR_MAX

#define DUK_UINTMAX_MIN       0
#define DUK_UINTMAX_MAX       UINTMAX_MAX
#define DUK_INTMAX_MIN        INTMAX_MIN
#define DUK_INTMAX_MAX        INTMAX_MAX

#define DUK_SIZE_MIN          0
#define DUK_SIZE_MAX          SIZE_MAX
#undef DUK_SIZE_MAX_COMPUTED

#else  /* C99 types */

/* When C99 types are not available, we use heuristic detection to get
 * the basic 8, 16, 32, and (possibly) 64 bit types.  The fast/least
 * types are then assumed to be exactly the same for now: these could
 * be improved per platform but C99 types are very often now available.
 * 64-bit types are not available on all platforms; this is OK at least
 * on 32-bit platforms.
 *
 * This detection code is necessarily a bit hacky and can provide typedefs
 * and defines that won't work correctly on some exotic platform.
 */

#if (defined(CHAR_BIT) && (CHAR_BIT == 8)) || \
    (defined(UCHAR_MAX) && (UCHAR_MAX == 255))
typedef unsigned char duk_uint8_t;
typedef signed char duk_int8_t;
#else
#error cannot detect 8-bit type
#endif

#if defined(USHRT_MAX) && (USHRT_MAX == 65535UL)
typedef unsigned short duk_uint16_t;
typedef signed short duk_int16_t;
#elif defined(UINT_MAX) && (UINT_MAX == 65535UL)
/* On some platforms int is 16-bit but long is 32-bit (e.g. PureC) */
typedef unsigned int duk_uint16_t;
typedef signed int duk_int16_t;
#else
#error cannot detect 16-bit type
#endif

#if defined(UINT_MAX) && (UINT_MAX == 4294967295UL)
typedef unsigned int duk_uint32_t;
typedef signed int duk_int32_t;
#elif defined(ULONG_MAX) && (ULONG_MAX == 4294967295UL)
/* On some platforms int is 16-bit but long is 32-bit (e.g. PureC) */
typedef unsigned long duk_uint32_t;
typedef signed long duk_int32_t;
#else
#error cannot detect 32-bit type
#endif

/* 64-bit type detection is a bit tricky.
 *
 * ULLONG_MAX is a standard define.  __LONG_LONG_MAX__ and __ULONG_LONG_MAX__
 * are used by at least GCC (even if system headers don't provide ULLONG_MAX).
 * Some GCC variants may provide __LONG_LONG_MAX__ but not __ULONG_LONG_MAX__.
 *
 * ULL / LL constants are rejected / warned about by some compilers, even if
 * the compiler has a 64-bit type and the compiler/system headers provide an
 * unsupported constant (ULL/LL)!  Try to avoid using ULL / LL constants.
 * As a side effect we can only check that e.g. ULONG_MAX is larger than 32
 * bits but can't be sure it is exactly 64 bits.  Self tests will catch such
 * cases.
 */
#undef DUK_F_HAVE_64BIT
#if !defined(DUK_F_HAVE_64BIT) && defined(ULONG_MAX)
#if (ULONG_MAX > 4294967295UL)
#define DUK_F_HAVE_64BIT
typedef unsigned long duk_uint64_t;
typedef signed long duk_int64_t;
#endif
#endif
#if !defined(DUK_F_HAVE_64BIT) && defined(ULLONG_MAX)
#if (ULLONG_MAX > 4294967295UL)
#define DUK_F_HAVE_64BIT
typedef unsigned long long duk_uint64_t;
typedef signed long long duk_int64_t;
#endif
#endif
#if !defined(DUK_F_HAVE_64BIT) && defined(__ULONG_LONG_MAX__)
#if (__ULONG_LONG_MAX__ > 4294967295UL)
#define DUK_F_HAVE_64BIT
typedef unsigned long long duk_uint64_t;
typedef signed long long duk_int64_t;
#endif
#endif
#if !defined(DUK_F_HAVE_64BIT) && defined(__LONG_LONG_MAX__)
#if (__LONG_LONG_MAX__ > 2147483647L)
#define DUK_F_HAVE_64BIT
typedef unsigned long long duk_uint64_t;
typedef signed long long duk_int64_t;
#endif
#endif
#if !defined(DUK_F_HAVE_64BIT) && \
    (defined(DUK_F_MINGW) || defined(DUK_F_MSVC))
/* Both MinGW and MSVC have a 64-bit type. */
#define DUK_F_HAVE_64BIT
typedef unsigned long duk_uint64_t;
typedef signed long duk_int64_t;
#endif
#if !defined(DUK_F_HAVE_64BIT)
/* cannot detect 64-bit type, not always needed so don't error */
#endif

typedef duk_uint8_t duk_uint_least8_t;
typedef duk_int8_t duk_int_least8_t;
typedef duk_uint16_t duk_uint_least16_t;
typedef duk_int16_t duk_int_least16_t;
typedef duk_uint32_t duk_uint_least32_t;
typedef duk_int32_t duk_int_least32_t;
typedef duk_uint8_t duk_uint_fast8_t;
typedef duk_int8_t duk_int_fast8_t;
typedef duk_uint16_t duk_uint_fast16_t;
typedef duk_int16_t duk_int_fast16_t;
typedef duk_uint32_t duk_uint_fast32_t;
typedef duk_int32_t duk_int_fast32_t;
#if defined(DUK_F_HAVE_64BIT)
typedef duk_uint64_t duk_uint_least64_t;
typedef duk_int64_t duk_int_least64_t;
typedef duk_uint64_t duk_uint_fast64_t;
typedef duk_int64_t duk_int_fast64_t;
#endif
#if defined(DUK_F_HAVE_64BIT)
typedef duk_uint64_t duk_uintmax_t;
typedef duk_int64_t duk_intmax_t;
#else
typedef duk_uint32_t duk_uintmax_t;
typedef duk_int32_t duk_intmax_t;
#endif

/* Note: the funny looking computations for signed minimum 16-bit, 32-bit, and
 * 64-bit values are intentional as the obvious forms (e.g. -0x80000000L) are
 * -not- portable.  See code-issues.txt for a detailed discussion.
 */
#define DUK_UINT8_MIN         0UL
#define DUK_UINT8_MAX         0xffUL
#define DUK_INT8_MIN          (-0x80L)
#define DUK_INT8_MAX          0x7fL
#define DUK_UINT_LEAST8_MIN   0UL
#define DUK_UINT_LEAST8_MAX   0xffUL
#define DUK_INT_LEAST8_MIN    (-0x80L)
#define DUK_INT_LEAST8_MAX    0x7fL
#define DUK_UINT_FAST8_MIN    0UL
#define DUK_UINT_FAST8_MAX    0xffUL
#define DUK_INT_FAST8_MIN     (-0x80L)
#define DUK_INT_FAST8_MAX     0x7fL
#define DUK_UINT16_MIN        0UL
#define DUK_UINT16_MAX        0xffffUL
#define DUK_INT16_MIN         (-0x7fffL - 1L)
#define DUK_INT16_MAX         0x7fffL
#define DUK_UINT_LEAST16_MIN  0UL
#define DUK_UINT_LEAST16_MAX  0xffffUL
#define DUK_INT_LEAST16_MIN   (-0x7fffL - 1L)
#define DUK_INT_LEAST16_MAX   0x7fffL
#define DUK_UINT_FAST16_MIN   0UL
#define DUK_UINT_FAST16_MAX   0xffffUL
#define DUK_INT_FAST16_MIN    (-0x7fffL - 1L)
#define DUK_INT_FAST16_MAX    0x7fffL
#define DUK_UINT32_MIN        0UL
#define DUK_UINT32_MAX        0xffffffffUL
#define DUK_INT32_MIN         (-0x7fffffffL - 1L)
#define DUK_INT32_MAX         0x7fffffffL
#define DUK_UINT_LEAST32_MIN  0UL
#define DUK_UINT_LEAST32_MAX  0xffffffffUL
#define DUK_INT_LEAST32_MIN   (-0x7fffffffL - 1L)
#define DUK_INT_LEAST32_MAX   0x7fffffffL
#define DUK_UINT_FAST32_MIN   0UL
#define DUK_UINT_FAST32_MAX   0xffffffffUL
#define DUK_INT_FAST32_MIN    (-0x7fffffffL - 1L)
#define DUK_INT_FAST32_MAX    0x7fffffffL

/* 64-bit constants.  Since LL / ULL constants are not always available,
 * use computed values.  These values can't be used in preprocessor
 * comparisons; flag them as such.
 */
#if defined(DUK_F_HAVE_64BIT)
#define DUK_UINT64_MIN        ((duk_uint64_t) 0)
#define DUK_UINT64_MAX        ((duk_uint64_t) -1)
#define DUK_INT64_MIN         ((duk_int64_t) (~(DUK_UINT64_MAX >> 1)))
#define DUK_INT64_MAX         ((duk_int64_t) (DUK_UINT64_MAX >> 1))
#define DUK_UINT_LEAST64_MIN  DUK_UINT64_MIN
#define DUK_UINT_LEAST64_MAX  DUK_UINT64_MAX
#define DUK_INT_LEAST64_MIN   DUK_INT64_MIN
#define DUK_INT_LEAST64_MAX   DUK_INT64_MAX
#define DUK_UINT_FAST64_MIN   DUK_UINT64_MIN
#define DUK_UINT_FAST64_MAX   DUK_UINT64_MAX
#define DUK_INT_FAST64_MIN    DUK_INT64_MIN
#define DUK_INT_FAST64_MAX    DUK_INT64_MAX
#define DUK_UINT64_MIN_COMPUTED
#define DUK_UINT64_MAX_COMPUTED
#define DUK_INT64_MIN_COMPUTED
#define DUK_INT64_MAX_COMPUTED
#define DUK_UINT_LEAST64_MIN_COMPUTED
#define DUK_UINT_LEAST64_MAX_COMPUTED
#define DUK_INT_LEAST64_MIN_COMPUTED
#define DUK_INT_LEAST64_MAX_COMPUTED
#define DUK_UINT_FAST64_MIN_COMPUTED
#define DUK_UINT_FAST64_MAX_COMPUTED
#define DUK_INT_FAST64_MIN_COMPUTED
#define DUK_INT_FAST64_MAX_COMPUTED
#endif

#if defined(DUK_F_HAVE_64BIT)
#define DUK_UINTMAX_MIN       DUK_UINT64_MIN
#define DUK_UINTMAX_MAX       DUK_UINT64_MAX
#define DUK_INTMAX_MIN        DUK_INT64_MIN
#define DUK_INTMAX_MAX        DUK_INT64_MAX
#define DUK_UINTMAX_MIN_COMPUTED
#define DUK_UINTMAX_MAX_COMPUTED
#define DUK_INTMAX_MIN_COMPUTED
#define DUK_INTMAX_MAX_COMPUTED
#else
#define DUK_UINTMAX_MIN       0UL
#define DUK_UINTMAX_MAX       0xffffffffUL
#define DUK_INTMAX_MIN        (-0x7fffffffL - 1L)
#define DUK_INTMAX_MAX        0x7fffffffL
#endif

/* This detection is not very reliable. */
#if defined(DUK_F_32BIT_PTRS)
typedef duk_int32_t duk_intptr_t;
typedef duk_uint32_t duk_uintptr_t;
#define DUK_UINTPTR_MIN       DUK_UINT32_MIN
#define DUK_UINTPTR_MAX       DUK_UINT32_MAX
#define DUK_INTPTR_MIN        DUK_INT32_MIN
#define DUK_INTPTR_MAX        DUK_INT32_MAX
#elif defined(DUK_F_64BIT_PTRS) && defined(DUK_F_HAVE_64BIT)
typedef duk_int64_t duk_intptr_t;
typedef duk_uint64_t duk_uintptr_t;
#define DUK_UINTPTR_MIN       DUK_UINT64_MIN
#define DUK_UINTPTR_MAX       DUK_UINT64_MAX
#define DUK_INTPTR_MIN        DUK_INT64_MIN
#define DUK_INTPTR_MAX        DUK_INT64_MAX
#define DUK_UINTPTR_MIN_COMPUTED
#define DUK_UINTPTR_MAX_COMPUTED
#define DUK_INTPTR_MIN_COMPUTED
#define DUK_INTPTR_MAX_COMPUTED
#else
#error cannot determine intptr type
#endif

/* SIZE_MAX may be missing so use an approximate value for it. */
#undef DUK_SIZE_MAX_COMPUTED
#if !defined(SIZE_MAX)
#define DUK_SIZE_MAX_COMPUTED
#define SIZE_MAX              ((size_t) (-1))
#endif
#define DUK_SIZE_MIN          0
#define DUK_SIZE_MAX          SIZE_MAX

#endif  /* C99 types */

/* A few types are assumed to always exist. */
typedef size_t duk_size_t;
typedef ptrdiff_t duk_ptrdiff_t;

/* The best type for an "all around int" in Duktape internals is "at least
 * 32 bit signed integer" which is most convenient.  Same for unsigned type.
 * Prefer 'int' when large enough, as it is almost always a convenient type.
 */
#if defined(UINT_MAX) && (UINT_MAX >= 0xffffffffUL)
typedef int duk_int_t;
typedef unsigned int duk_uint_t;
#define DUK_INT_MIN           INT_MIN
#define DUK_INT_MAX           INT_MAX
#define DUK_UINT_MIN          0
#define DUK_UINT_MAX          UINT_MAX
#else
typedef duk_int_fast32_t duk_int_t;
typedef duk_uint_fast32_t duk_uint_t;
#define DUK_INT_MIN           DUK_INT_FAST32_MIN
#define DUK_INT_MAX           DUK_INT_FAST32_MAX
#define DUK_UINT_MIN          DUK_UINT_FAST32_MIN
#define DUK_UINT_MAX          DUK_UINT_FAST32_MAX
#endif

/* Same as 'duk_int_t' but guaranteed to be a 'fast' variant if this
 * distinction matters for the CPU.  These types are used mainly in the
 * executor where it might really matter.
 */
typedef duk_int_fast32_t duk_int_fast_t;
typedef duk_uint_fast32_t duk_uint_fast_t;
#define DUK_INT_FAST_MIN      DUK_INT_FAST32_MIN
#define DUK_INT_FAST_MAX      DUK_INT_FAST32_MAX
#define DUK_UINT_FAST_MIN     DUK_UINT_FAST32_MIN
#define DUK_UINT_FAST_MAX     DUK_UINT_FAST32_MAX

/* Small integers (16 bits or more) can fall back to the 'int' type, but
 * have a typedef so they are marked "small" explicitly.
 */
typedef int duk_small_int_t;
typedef unsigned int duk_small_uint_t;
#define DUK_SMALL_INT_MIN     INT_MIN
#define DUK_SMALL_INT_MAX     INT_MAX
#define DUK_SMALL_UINT_MIN    0
#define DUK_SMALL_UINT_MAX    UINT_MAX

/* Fast variants of small integers, again for really fast paths like the
 * executor.
 */
typedef duk_int_fast16_t duk_small_int_fast_t;
typedef duk_uint_fast16_t duk_small_uint_fast_t;
#define DUK_SMALL_INT_FAST_MIN    DUK_INT_FAST16_MIN
#define DUK_SMALL_INT_FAST_MAX    DUK_INT_FAST16_MAX
#define DUK_SMALL_UINT_FAST_MIN   DUK_UINT_FAST16_MIN
#define DUK_SMALL_UINT_FAST_MAX   DUK_UINT_FAST16_MAX

/* Boolean values are represented with the platform 'int'. */
typedef duk_small_int_t duk_bool_t;
#define DUK_BOOL_MIN              DUK_SMALL_INT_MIN
#define DUK_BOOL_MAX              DUK_SMALL_INT_MAX

/* Index values must have at least 32-bit signed range. */
typedef duk_int_t duk_idx_t;
#define DUK_IDX_MIN               DUK_INT_MIN
#define DUK_IDX_MAX               DUK_INT_MAX

/* Array index values, could be exact 32 bits.
 * Currently no need for signed duk_arridx_t.
 */
typedef duk_uint_t duk_uarridx_t;
#define DUK_UARRIDX_MIN           DUK_UINT_MIN
#define DUK_UARRIDX_MAX           DUK_UINT_MAX

/* Duktape/C function return value, platform int is enough for now to
 * represent 0, 1, or negative error code.  Must be compatible with
 * assigning truth values (e.g. duk_ret_t rc = (foo == bar);).
 */
typedef duk_small_int_t duk_ret_t;
#define DUK_RET_MIN               DUK_SMALL_INT_MIN
#define DUK_RET_MAX               DUK_SMALL_INT_MAX

/* Error codes are represented with platform int.  High bits are used
 * for flags and such, so 32 bits are needed.
 */
typedef duk_int_t duk_errcode_t;
#define DUK_ERRCODE_MIN           DUK_INT_MIN
#define DUK_ERRCODE_MAX           DUK_INT_MAX

/* Codepoint type.  Must be 32 bits or more because it is used also for
 * internal codepoints.  The type is signed because negative codepoints
 * are used as internal markers (e.g. to mark EOF or missing argument).
 * (X)UTF-8/CESU-8 encode/decode take and return an unsigned variant to
 * ensure duk_uint32_t casts back and forth nicely.  Almost everything
 * else uses the signed one.
 */
typedef duk_int_t duk_codepoint_t;
typedef duk_uint_t duk_ucodepoint_t;
#define DUK_CODEPOINT_MIN         DUK_INT_MIN
#define DUK_CODEPOINT_MAX         DUK_INT_MAX
#define DUK_UCODEPOINT_MIN        DUK_UINT_MIN
#define DUK_UCODEPOINT_MAX        DUK_UINT_MAX

/* IEEE float/double typedef. */
typedef float duk_float_t;
typedef double duk_double_t;

/* We're generally assuming that we're working on a platform with a 32-bit
 * address space.  If DUK_SIZE_MAX is a typecast value (which is necessary
 * if SIZE_MAX is missing), the check must be avoided because the
 * preprocessor can't do a comparison.
 */
#if !defined(DUK_SIZE_MAX)
#error DUK_SIZE_MAX is undefined, probably missing SIZE_MAX
#elif !defined(DUK_SIZE_MAX_COMPUTED)
#if DUK_SIZE_MAX < 0xffffffffUL
/* On some systems SIZE_MAX can be smaller than max unsigned 32-bit value
 * which seems incorrect if size_t is (at least) an unsigned 32-bit type.
 * However, it doesn't seem useful to error out compilation if this is the
 * case.
 */
#endif
#endif

/* Type for public API calls. */
typedef struct duk_hthread duk_context;

/*
 *  Check whether we should use 64-bit integers
 */

/* Quite incomplete now.  Use 64-bit types if detected (C99 or other detection)
 * unless they are known to be unreliable.  For instance, 64-bit types are
 * available on VBCC but seem to misbehave.
 */
#if defined(DUK_F_HAVE_64BIT) && !defined(DUK_F_VBCC)
#define DUK_USE_64BIT_OPS
#else
#undef DUK_USE_64BIT_OPS
#endif

/*
 *  Alignment requirement and support for unaligned accesses
 *
 *  Assume unaligned accesses are not supported unless specifically allowed
 *  in the target platform.  Some platforms may support unaligned accesses
 *  but alignment to 4 or 8 may still be desirable.
 */

#undef DUK_USE_UNALIGNED_ACCESSES_POSSIBLE
#undef DUK_USE_ALIGN_BY

#if defined(DUK_F_EMSCRIPTEN)
/* Required on at least some targets, so use whenever Emscripten used,
 * regardless of compilation target.
 */
#define DUK_USE_ALIGN_BY 8
#elif defined(DUK_F_ARM)
#define DUK_USE_ALIGN_BY 4
#elif defined(DUK_F_MIPS32)
/* Based on 'make checkalign' there are no alignment requirements on
 * Linux MIPS except for doubles, which need align by 4.  Alignment
 * requirements vary based on target though.
 */
#define DUK_USE_ALIGN_BY 4
#elif defined(DUK_F_MIPS64)
/* Good default is a bit arbitrary because alignment requirements
 * depend on target.  See https://github.com/svaarala/duktape/issues/102.
 */
#define DUK_USE_ALIGN_BY 8
#elif defined(DUK_F_SUPERH)
/* Based on 'make checkalign' there are no alignment requirements on
 * Linux SH4, but align by 4 is probably a good basic default.
 */
#define DUK_USE_ALIGN_BY 4
#elif defined(DUK_F_X86) || defined(DUK_F_X32) || defined(DUK_F_X64) || \
      defined(DUK_F_BCC)
/* XXX: This is technically not guaranteed because it's possible to configure
 * an x86 to require aligned accesses with Alignment Check (AC) flag.
 */
#define DUK_USE_ALIGN_BY 1
#define DUK_USE_UNALIGNED_ACCESSES_POSSIBLE
#else
/* Unknown, use safe default */
#define DUK_USE_ALIGN_BY 8
#endif

/* User forced alignment to 4 or 8. */
#if defined(DUK_OPT_FORCE_ALIGN)
#undef DUK_USE_ALIGN_BY
#undef DUK_USE_UNALIGNED_ACCESSES_POSSIBLE
#if (DUK_OPT_FORCE_ALIGN == 4)
#define DUK_USE_ALIGN_BY 4
#elif (DUK_OPT_FORCE_ALIGN == 8)
#define DUK_USE_ALIGN_BY 8
#else
#error invalid DUK_OPT_FORCE_ALIGN value
#endif
#endif

/* Compiler specific hackery needed to force struct size to match aligment,
 * see e.g. duk_hbuffer.h.
 *
 * http://stackoverflow.com/questions/11130109/c-struct-size-alignment
 * http://stackoverflow.com/questions/10951039/specifying-64-bit-alignment
 */
#if defined(DUK_F_MSVC)
#define DUK_USE_PACK_MSVC_PRAGMA
#elif defined(DUK_F_GCC)
#define DUK_USE_PACK_GCC_ATTR
#elif defined(DUK_F_CLANG)
#define DUK_USE_PACK_CLANG_ATTR
#else
#define DUK_USE_PACK_DUMMY_MEMBER
#endif

#ifdef DUK_USE_UNALIGNED_ACCESSES_POSSIBLE
#define DUK_USE_HASHBYTES_UNALIGNED_U32_ACCESS
#else
#undef DUK_USE_HASHBYTES_UNALIGNED_U32_ACCESS
#endif

/* Object property allocation layout has implications for memory and code
 * footprint and generated code size/speed.  The best layout also depends
 * on whether the platform has alignment requirements or benefits from
 * having mostly aligned accesses.
 */
#undef DUK_USE_HOBJECT_LAYOUT_1
#undef DUK_USE_HOBJECT_LAYOUT_2
#undef DUK_USE_HOBJECT_LAYOUT_3
#if defined(DUK_USE_UNALIGNED_ACCESSES_POSSIBLE) && (DUK_USE_ALIGN_BY == 1)
/* On platforms without any alignment issues, layout 1 is preferable
 * because it compiles to slightly less code and provides direct access
 * to property keys.
 */
#define DUK_USE_HOBJECT_LAYOUT_1
#else
/* On other platforms use layout 2, which requires some padding but
 * is a bit more natural than layout 3 in ordering the entries.  Layout
 * 3 is currently not used.
 */
#define DUK_USE_HOBJECT_LAYOUT_2
#endif

/*
 *  Byte order and double memory layout detection
 *
 *  Endianness detection is a major portability hassle because the macros
 *  and headers are not standardized.  There's even variance across UNIX
 *  platforms.  Even with "standard" headers, details like underscore count
 *  varies between platforms, e.g. both __BYTE_ORDER and _BYTE_ORDER are used
 *  (Crossbridge has a single underscore, for instance).
 *
 *  The checks below are structured with this in mind: several approaches are
 *  used, and at the end we check if any of them worked.  This allows generic
 *  approaches to be tried first, and platform/compiler specific hacks tried
 *  last.  As a last resort, the user can force a specific endianness, as it's
 *  not likely that automatic detection will work on the most exotic platforms.
 *
 *  Duktape supports little and big endian machines.  There's also support
 *  for a hybrid used by some ARM machines where integers are little endian
 *  but IEEE double values use a mixed order (12345678 -> 43218765).  This
 *  byte order for doubles is referred to as "mixed endian".
 */

#undef DUK_F_BYTEORDER
#undef DUK_USE_BYTEORDER_FORCED

/* DUK_F_BYTEORDER is set as an intermediate value when detection
 * succeeds, to one of:
 *   1 = little endian
 *   2 = mixed (arm hybrid) endian
 *   3 = big endian
 */

/* For custom platforms allow user to define byteorder explicitly.
 * Since endianness headers are not standardized, this is a useful
 * workaround for custom platforms for which endianness detection
 * is not directly supported.  Perhaps custom hardware is used and
 * user cannot submit upstream patches.
 */
#if defined(DUK_OPT_FORCE_BYTEORDER)
#if (DUK_OPT_FORCE_BYTEORDER == 1)
#define DUK_F_BYTEORDER 1
#elif (DUK_OPT_FORCE_BYTEORDER == 2)
#define DUK_F_BYTEORDER 2
#elif (DUK_OPT_FORCE_BYTEORDER == 3)
#define DUK_F_BYTEORDER 3
#else
#error invalid DUK_OPT_FORCE_BYTEORDER value
#endif
#define DUK_USE_BYTEORDER_FORCED
#endif  /* DUK_OPT_FORCE_BYTEORDER */

/* More or less standard endianness predefines provided by header files.
 * The ARM hybrid case is detected by assuming that __FLOAT_WORD_ORDER
 * will be big endian, see: http://lists.mysql.com/internals/443.
 */
#if !defined(DUK_F_BYTEORDER)
#if defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && (__BYTE_ORDER == __LITTLE_ENDIAN) || \
    defined(_BYTE_ORDER) && defined(_LITTLE_ENDIAN) && (_BYTE_ORDER == _LITTLE_ENDIAN) || \
    defined(__LITTLE_ENDIAN__)
/* Integer little endian */
#if defined(__FLOAT_WORD_ORDER) && defined(__LITTLE_ENDIAN) && (__FLOAT_WORD_ORDER == __LITTLE_ENDIAN) || \
    defined(_FLOAT_WORD_ORDER) && defined(_LITTLE_ENDIAN) && (_FLOAT_WORD_ORDER == _LITTLE_ENDIAN)
#define DUK_F_BYTEORDER 1
#elif defined(__FLOAT_WORD_ORDER) && defined(__BIG_ENDIAN) && (__FLOAT_WORD_ORDER == __BIG_ENDIAN) || \
      defined(_FLOAT_WORD_ORDER) && defined(_BIG_ENDIAN) && (_FLOAT_WORD_ORDER == _BIG_ENDIAN)
#define DUK_F_BYTEORDER 2
#elif !defined(__FLOAT_WORD_ORDER) && !defined(_FLOAT_WORD_ORDER)
/* Float word order not known, assume not a hybrid. */
#define DUK_F_BYTEORDER 1
#else
/* byte order is little endian but cannot determine IEEE double word order */
#endif  /* float word order */
#elif defined(__BYTE_ORDER) && defined(__BIG_ENDIAN) && (__BYTE_ORDER == __BIG_ENDIAN) || \
      defined(_BYTE_ORDER) && defined(_BIG_ENDIAN) && (_BYTE_ORDER == _BIG_ENDIAN) || \
      defined(__BIG_ENDIAN__)
/* Integer big endian */
#if defined(__FLOAT_WORD_ORDER) && defined(__BIG_ENDIAN) && (__FLOAT_WORD_ORDER == __BIG_ENDIAN) || \
    defined(_FLOAT_WORD_ORDER) && defined(_BIG_ENDIAN) && (_FLOAT_WORD_ORDER == _BIG_ENDIAN)
#define DUK_F_BYTEORDER 3
#elif !defined(__FLOAT_WORD_ORDER) && !defined(_FLOAT_WORD_ORDER)
/* Float word order not known, assume not a hybrid. */
#define DUK_F_BYTEORDER 3
#else
/* byte order is big endian but cannot determine IEEE double word order */
#endif  /* float word order */
#else
/* cannot determine byte order */
#endif  /* integer byte order */
#endif  /* !defined(DUK_F_BYTEORDER) */

/* GCC and Clang provide endianness defines as built-in predefines, with
 * leading and trailing double underscores (e.g. __BYTE_ORDER__).  See
 * output of "make gccpredefs" and "make clangpredefs".  Clang doesn't
 * seem to provide __FLOAT_WORD_ORDER__.
 * http://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
 */
#if !defined(DUK_F_BYTEORDER) && defined(__BYTE_ORDER__)
#if defined(__ORDER_LITTLE_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
/* Integer little endian */
#if defined(__FLOAT_WORD_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && \
    (__FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define DUK_F_BYTEORDER 1
#elif defined(__FLOAT_WORD_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && \
      (__FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__)
#define DUK_F_BYTEORDER 2
#elif !defined(__FLOAT_WORD_ORDER__)
/* Float word order not known, assume not a hybrid. */
#define DUK_F_BYTEORDER 1
#else
/* byte order is little endian but cannot determine IEEE double word order */
#endif  /* float word order */
#elif defined(__ORDER_BIG_ENDIAN__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
/* Integer big endian */
#if defined(__FLOAT_WORD_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && \
    (__FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__)
#define DUK_F_BYTEORDER 3
#elif !defined(__FLOAT_WORD_ORDER__)
/* Float word order not known, assume not a hybrid. */
#define DUK_F_BYTEORDER 3
#else
/* byte order is big endian but cannot determine IEEE double word order */
#endif  /* float word order */
#else
/* cannot determine byte order; __ORDER_PDP_ENDIAN__ is related to 32-bit
 * integer ordering and is not relevant
 */
#endif  /* integer byte order */
#endif  /* !defined(DUK_F_BYTEORDER) && defined(__BYTE_ORDER__) */

/* Atari ST TOS */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_TOS)
#define DUK_F_BYTEORDER 3
#endif

/* AmigaOS on M68K or PPC */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_AMIGAOS)
#if defined(DUK_F_M68K) || defined(DUK_F_PPC)
#define DUK_F_BYTEORDER 3
#endif
#endif

/* On Windows, assume we're little endian.  Even Itanium which has a
 * configurable endianness runs little endian in Windows.
 */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_WINDOWS)
/* XXX: verify that Windows on ARM is little endian for floating point
 * values too.
 */
#define DUK_F_BYTEORDER 1
#endif  /* Windows */

/* Crossbridge should work with the standard byteorder #ifdefs.  It doesn't
 * provide _FLOAT_WORD_ORDER but the standard approach now covers that case
 * too.  This has been left here just in case.
 */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_FLASHPLAYER)
#define DUK_F_BYTEORDER 1
#endif

/* QNX gcc cross compiler seems to define e.g. __LITTLEENDIAN__ or __BIGENDIAN__:
 *  $ /opt/qnx650/host/linux/x86/usr/bin/i486-pc-nto-qnx6.5.0-gcc -dM -E - </dev/null | grep -ni endian
 *  67:#define __LITTLEENDIAN__ 1
 *  $ /opt/qnx650/host/linux/x86/usr/bin/mips-unknown-nto-qnx6.5.0-gcc -dM -E - </dev/null | grep -ni endian
 *  81:#define __BIGENDIAN__ 1
 *  $ /opt/qnx650/host/linux/x86/usr/bin/arm-unknown-nto-qnx6.5.0-gcc -dM -E - </dev/null | grep -ni endian
 *  70:#define __LITTLEENDIAN__ 1
 */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_QNX)
/* XXX: ARM hybrid? */
#if defined(__LITTLEENDIAN__)
#define DUK_F_BYTEORDER 1
#elif defined(__BIGENDIAN__)
#define DUK_F_BYTEORDER 3
#endif
#endif

/* Bruce's C Compiler (BCC), assume we're on x86. */
#if !defined(DUK_F_BYTEORDER) && defined(DUK_F_BCC)
#define DUK_F_BYTEORDER 1
#endif

/* Check whether or not byte order detection worked based on the intermediate
 * define, and define final values.  If detection failed, #error out.
 */
#if defined(DUK_F_BYTEORDER)
#if (DUK_F_BYTEORDER == 1)
#define DUK_USE_INTEGER_LE
#define DUK_USE_DOUBLE_LE
#elif (DUK_F_BYTEORDER == 2)
#define DUK_USE_INTEGER_LE  /* integer endianness is little on purpose */
#define DUK_USE_DOUBLE_ME
#elif (DUK_F_BYTEORDER == 3)
#define DUK_USE_INTEGER_BE
#define DUK_USE_DOUBLE_BE
#else
#error unsupported: byte order detection failed (internal error, should not happen)
#endif  /* byte order */
#else
#error unsupported: byte order detection failed
#endif  /* defined(DUK_F_BYTEORDER) */

/*
 *  Check whether or not a packed duk_tval representation is possible.
 *  What's basically required is that pointers are 32-bit values
 *  (sizeof(void *) == 4).  Best effort check, not always accurate.
 *  If guess goes wrong, crashes may result; self tests also verify
 *  the guess.
 */

#undef DUK_USE_PACKED_TVAL_POSSIBLE

/* Strict C99 case: DUK_UINTPTR_MAX (= UINTPTR_MAX) should be very reliable */
#if !defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_F_HAVE_INTTYPES) && defined(DUK_UINTPTR_MAX)
#if (DUK_UINTPTR_MAX <= 0xffffffffUL)
#define DUK_USE_PACKED_TVAL_POSSIBLE
#endif
#endif

/* Non-C99 case, still relying on DUK_UINTPTR_MAX, as long as it is not a computed value */
#if !defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_UINTPTR_MAX) && !defined(DUK_UINTPTR_MAX_COMPUTED)
#if (DUK_UINTPTR_MAX <= 0xffffffffUL)
#define DUK_USE_PACKED_TVAL_POSSIBLE
#endif
#endif

/* DUK_SIZE_MAX (= SIZE_MAX) is often reliable */
#if !defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_SIZE_MAX) && !defined(DUK_SIZE_MAX_COMPUTED)
#if DUK_SIZE_MAX <= 0xffffffffUL
#define DUK_USE_PACKED_TVAL_POSSIBLE
#endif
#endif

/* M68K: packed always possible */
#if !defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_F_M68K)
#define DUK_USE_PACKED_TVAL_POSSIBLE
#endif

/* PPC32: packed always possible */
#if !defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_F_PPC32)
#define DUK_USE_PACKED_TVAL_POSSIBLE
#endif

/* With Emscripten, force unpacked duk_tval just to be safe, as it seems to
 * break at least on Firefox (probably IEEE double arithmetic is not 100%
 * supported, especially for NaNs).
 */
#if defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_F_EMSCRIPTEN)
#undef DUK_USE_PACKED_TVAL_POSSIBLE
#endif

/* Microsoft Visual Studio 2010 on x64 fails the above rules and tries to
 * use a packed type.  Force unpacked on x64 in general.
 */
#if defined(DUK_USE_PACKED_TVAL_POSSIBLE) && defined(DUK_F_X64)
#undef DUK_USE_PACKED_TVAL_POSSIBLE
#endif

/* GCC/clang inaccurate math would break compliance and probably duk_tval,
 * so refuse to compile.  Relax this if -ffast-math is tested to work.
 */
#if defined(__FAST_MATH__)
#error __FAST_MATH__ defined, refusing to compile
#endif

/*
 *  Detection of double constants and math related functions.  Availability
 *  of constants and math functions is a significant porting concern.
 *
 *  INFINITY/HUGE_VAL is problematic on GCC-3.3: it causes an overflow warning
 *  and there is no pragma in GCC-3.3 to disable it.  Using __builtin_inf()
 *  avoids this problem for some reason.
 */

#define DUK_DOUBLE_2TO32     4294967296.0
#define DUK_DOUBLE_2TO31     2147483648.0

#undef DUK_USE_COMPUTED_INFINITY
#if defined(DUK_F_GCC_VERSION) && (DUK_F_GCC_VERSION < 40600)
/* GCC older than 4.6: avoid overflow warnings related to using INFINITY */
#define DUK_DOUBLE_INFINITY  (__builtin_inf())
#elif defined(INFINITY)
#define DUK_DOUBLE_INFINITY  ((double) INFINITY)
#elif !defined(DUK_F_VBCC) && !defined(DUK_F_MSVC) && !defined(DUK_F_BCC)
#define DUK_DOUBLE_INFINITY  (1.0 / 0.0)
#else
/* In VBCC (1.0 / 0.0) results in a warning and 0.0 instead of infinity.
 * Use a computed infinity (initialized when a heap is created at the
 * latest).
 */
extern double duk_computed_infinity;
#define DUK_USE_COMPUTED_INFINITY
#define DUK_DOUBLE_INFINITY  duk_computed_infinity
#endif

#undef DUK_USE_COMPUTED_NAN
#if defined(NAN)
#define DUK_DOUBLE_NAN       NAN
#elif !defined(DUK_F_VBCC) && !defined(DUK_F_MSVC) && !defined(DUK_F_BCC)
#define DUK_DOUBLE_NAN       (0.0 / 0.0)
#else
/* In VBCC (0.0 / 0.0) results in a warning and 0.0 instead of NaN.
 * In MSVC (VS2010 Express) (0.0 / 0.0) results in a compile error.
 * Use a computed NaN (initialized when a heap is created at the
 * latest).
 */
extern double duk_computed_nan;
#define DUK_USE_COMPUTED_NAN
#define DUK_DOUBLE_NAN       duk_computed_nan
#endif

/* Many platforms are missing fpclassify() and friends, so use replacements
 * if necessary.  The replacement constants (FP_NAN etc) can be anything but
 * match Linux constants now.
 */
#undef DUK_USE_REPL_FPCLASSIFY
#undef DUK_USE_REPL_SIGNBIT
#undef DUK_USE_REPL_ISFINITE
#undef DUK_USE_REPL_ISNAN
#undef DUK_USE_REPL_ISINF

/* Complex condition broken into separate parts. */
#undef DUK_F_USE_REPL_ALL
#if !(defined(FP_NAN) && defined(FP_INFINITE) && defined(FP_ZERO) && \
      defined(FP_SUBNORMAL) && defined(FP_NORMAL))
/* Missing some obvious constants. */
#define DUK_F_USE_REPL_ALL
#elif defined(DUK_F_AMIGAOS) && defined(DUK_F_VBCC)
/* VBCC is missing the built-ins even in C99 mode (perhaps a header issue) */
#define DUK_F_USE_REPL_ALL
#elif defined(DUK_F_FREEBSD) && defined(DUK_F_CLANG)
/* Placeholder fix for (detection is wider than necessary):
 * http://llvm.org/bugs/show_bug.cgi?id=17788
 */
#define DUK_F_USE_REPL_ALL
#elif defined(DUK_F_UCLIBC)
/* At least some uclibc versions have broken floating point math.  For
 * example, fpclassify() can incorrectly classify certain NaN formats.
 * To be safe, use replacements.
 */
#define DUK_F_USE_REPL_ALL
#endif

#if defined(DUK_F_USE_REPL_ALL)
#define DUK_USE_REPL_FPCLASSIFY
#define DUK_USE_REPL_SIGNBIT
#define DUK_USE_REPL_ISFINITE
#define DUK_USE_REPL_ISNAN
#define DUK_USE_REPL_ISINF
#define DUK_FPCLASSIFY       duk_repl_fpclassify
#define DUK_SIGNBIT          duk_repl_signbit
#define DUK_ISFINITE         duk_repl_isfinite
#define DUK_ISNAN            duk_repl_isnan
#define DUK_ISINF            duk_repl_isinf
#define DUK_FP_NAN           0
#define DUK_FP_INFINITE      1
#define DUK_FP_ZERO          2
#define DUK_FP_SUBNORMAL     3
#define DUK_FP_NORMAL        4
#else
#define DUK_FPCLASSIFY       fpclassify
#define DUK_SIGNBIT          signbit
#define DUK_ISFINITE         isfinite
#define DUK_ISNAN            isnan
#define DUK_ISINF            isinf
#define DUK_FP_NAN           FP_NAN
#define DUK_FP_INFINITE      FP_INFINITE
#define DUK_FP_ZERO          FP_ZERO
#define DUK_FP_SUBNORMAL     FP_SUBNORMAL
#define DUK_FP_NORMAL        FP_NORMAL
#endif

#if defined(DUK_F_USE_REPL_ALL)
#undef DUK_F_USE_REPL_ALL
#endif

/* Some math functions are C99 only.  This is also an issue with some
 * embedded environments using uclibc where uclibc has been configured
 * not to provide some functions.  For now, use replacements whenever
 * using uclibc.
 */
#undef DUK_USE_MATH_FMIN
#undef DUK_USE_MATH_FMAX
#undef DUK_USE_MATH_ROUND
#if defined(DUK_F_UCLIBC)
/* uclibc may be missing these */
#elif defined(DUK_F_AMIGAOS) && defined(DUK_F_VBCC)
/* vbcc + AmigaOS may be missing these */
#elif !defined(DUK_F_C99) && !defined(DUK_F_CPP11)
/* build is not C99 or C++11, play it safe */
#else
/* C99 or C++11, no known issues */
#define DUK_USE_MATH_FMIN
#define DUK_USE_MATH_FMAX
#define DUK_USE_MATH_ROUND
#endif

/* These functions don't currently need replacement but are wrapped for
 * completeness.  Because these are used as function pointers, they need
 * to be defined as concrete C functions (not macros).
 */
#define DUK_FABS             fabs
#define DUK_FMIN             fmin
#define DUK_FMAX             fmax
#define DUK_FLOOR            floor
#define DUK_CEIL             ceil
#define DUK_FMOD             fmod
#define DUK_POW              pow
#define DUK_ACOS             acos
#define DUK_ASIN             asin
#define DUK_ATAN             atan
#define DUK_ATAN2            atan2
#define DUK_SIN              sin
#define DUK_COS              cos
#define DUK_TAN              tan
#define DUK_EXP              exp
#define DUK_LOG              log
#define DUK_SQRT             sqrt

/* NetBSD 6.0 x86 (at least) has a few problems with pow() semantics,
 * see test-bug-netbsd-math-pow.js.  Use NetBSD specific workaround.
 * (This might be a wider problem; if so, generalize the define name.)
 */
#undef DUK_USE_POW_NETBSD_WORKAROUND
#if defined(DUK_F_NETBSD)
#define DUK_USE_POW_NETBSD_WORKAROUND
#endif

/* Rely as little as possible on compiler behavior for NaN comparison,
 * signed zero handling, etc.  Currently never activated but may be needed
 * for broken compilers.
 */
#undef DUK_USE_PARANOID_MATH

/* There was a curious bug where test-bi-date-canceling.js would fail e.g.
 * on 64-bit Ubuntu, gcc-4.8.1, -m32, and no -std=c99.  Some date computations
 * using doubles would be optimized which then broke some corner case tests.
 * The problem goes away by adding 'volatile' to the datetime computations.
 * Not sure what the actual triggering conditions are, but using this on
 * non-C99 systems solves the known issues and has relatively little cost
 * on other platforms.  See bugs/issue-2e9d9c2d761dabaf8136c0897b91a270d1a47147.yaml.
 */
#undef DUK_USE_PARANOID_DATE_COMPUTATION
#if !defined(DUK_F_C99)
#define DUK_USE_PARANOID_DATE_COMPUTATION
#endif

/*
 *  ANSI C string/memory function wrapper defines to allow easier workarounds.
 *  Also convenience macros like DUK_MEMZERO which may be mapped to existing
 *  platform function to zero memory (like the deprecated bzero).
 *
 *  For instance, some platforms don't support zero-size memcpy correctly,
 *  some arcane uclibc versions have a buggy memcpy (but working memmove)
 *  and so on.  Such broken platforms can be dealt with here.
 *
 *  NOTE: ANSI C (various versions) and some implementations require that the
 *  pointer arguments to memset(), memcpy(), and memmove() be valid values
 *  even when byte size is 0 (even a NULL pointer is considered invalid in
 *  this context).  Zero-size operations as such are allowed, as long as their
 *  pointer arguments point to a valid memory area.  The DUK_MEMSET(),
 *  DUK_MEMCPY(), and DUK_MEMMOVE() macros require this same behavior, i.e.:
 *  (1) pointers must be valid and non-NULL, (2) zero size must otherwise be
 *  allowed.  If these are not fulfilled, a macro wrapper is needed.
 *
 *    http://stackoverflow.com/questions/5243012/is-it-guaranteed-to-be-safe-to-perform-memcpy0-0-0
 *    http://lists.cs.uiuc.edu/pipermail/llvmdev/2007-October/011065.html
 *
 *  Not sure what's the required behavior when a pointer points just past the
 *  end of a buffer, which often happens in practice (e.g. zero size memmoves).
 *  For example, if allocation size is 3, the following pointer would not
 *  technically point to a valid memory byte:
 *
 *    <-- alloc -->
 *    | 0 | 1 | 2 | .....
 *                  ^-- p=3, points after last valid byte (2)
 *
 *  If this is a practical issue, wrappers are again needed.
 */

typedef FILE duk_file;
#define DUK_STDIN       stdin
#define DUK_STDOUT      stdout
#define DUK_STDERR      stderr

/* Special naming to avoid conflict with e.g. DUK_FREE() in duk_heap.h
 * (which is unfortunately named).
 */
#define DUK_ANSI_MALLOC      malloc
#define DUK_ANSI_REALLOC     realloc
#define DUK_ANSI_CALLOC      calloc
#define DUK_ANSI_FREE        free

/* Old uclibcs have a broken memcpy so use memmove instead (this is overly
 * wide now on purpose):
 * http://lists.uclibc.org/pipermail/uclibc-cvs/2008-October/025511.html
 */
#if defined(DUK_F_UCLIBC)
#define DUK_MEMCPY       memmove
#else
#define DUK_MEMCPY       memcpy
#endif

#define DUK_MEMMOVE      memmove
#define DUK_MEMCMP       memcmp
#define DUK_MEMSET       memset
#define DUK_STRLEN       strlen
#define DUK_STRCMP       strcmp
#define DUK_STRNCMP      strncmp
#define DUK_PRINTF       printf
#define DUK_FPRINTF      fprintf
#define DUK_SPRINTF      sprintf

#if defined(DUK_F_MSVC)
/* _snprintf() does NOT NUL terminate on truncation, but Duktape code never
 * assumes that.
 * http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
 */
#define DUK_SNPRINTF     _snprintf
#else
#define DUK_SNPRINTF     snprintf
#endif

#define DUK_VSPRINTF     vsprintf

#if defined(DUK_F_MSVC)
#if (_MSC_VER < 1600)
/* Older MSVC version are missing vsnprintf() but have _vsnprintf(). */
#define DUK_VSNPRINTF    _vsnprintf
#else
#define DUK_VSNPRINTF    vsnprintf
#endif
#else
#define DUK_VSNPRINTF    vsnprintf
#endif  /* DUK_F_MSVC */

#define DUK_SSCANF       sscanf
#define DUK_VSSCANF      vsscanf
#define DUK_FOPEN        fopen
#define DUK_FCLOSE       fclose
#define DUK_FREAD        fread
#define DUK_FWRITE       fwrite
#define DUK_FSEEK        fseek
#define DUK_FTELL        ftell
#define DUK_FFLUSH       fflush
#define DUK_FPUTC        fputc

#define DUK_MEMZERO(p,n) \
	DUK_MEMSET((p), 0, (n))

/*
 *  Vararg macro wrappers.  We need va_copy() which is defined in C99 / C++11,
 *  so an awkward replacement is needed for pre-C99 / pre-C++11 environments.
 *  This will quite likely need portability hacks for some non-C99 environments.
 */

#if defined(DUK_F_C99) || defined(DUK_F_CPP11)
/* C99 / C++11 and above: rely on va_copy() which is required.
 * Omit parenthesis on macro right side on purpose to minimize differences
 * to direct use.
 */
#define DUK_VA_COPY(dest,src) va_copy(dest,src)
#elif defined(DUK_F_GCC) || defined(DUK_F_CLANG)
/* GCC: assume we have __va_copy() in non-C99 mode, which should be correct
 * for even quite old GCC versions.  Clang matches GCC behavior.
 */
#define DUK_VA_COPY(dest,src) __va_copy(dest,src)
#else
/* Pre-C99: va_list type is implementation dependent.  This replacement
 * assumes it is a plain value so that a simple assignment will work.
 * This is not the case on all platforms (it may be a single-array element,
 * for instance).
 */
#define DUK_VA_COPY(dest,src) do { (dest) = (src); } while (0)
#endif

/*
 *  Miscellaneous ANSI C or other platform wrappers.
 */

#define DUK_ABORT        abort
#define DUK_EXIT         exit

/*
 *  Avoiding platform function pointers.
 *
 *  On some platforms built-in functions may be implemented as macros or
 *  inline functions, so they can't be necessarily addressed by function
 *  pointers.  This is certainly the case with some platform "polyfills"
 *  which provide missing C99/C++11 functions through macros, and may be
 *  the case with VS2013 (see GH-17).
 */

/* This is now the default: the cost in footprint is negligible. */
#define DUK_USE_AVOID_PLATFORM_FUNCPTRS

/*
 *  Macro hackery to convert e.g. __LINE__ to a string without formatting,
 *  see: http://stackoverflow.com/questions/240353/convert-a-preprocessor-token-to-a-string
 */

#define DUK_F_STRINGIFY_HELPER(x)  #x
#define DUK_MACRO_STRINGIFY(x)  DUK_F_STRINGIFY_HELPER(x)

/*
 *  Cause segfault macro.
 *
 *  This is optionally used by panic handling to cause the program to segfault
 *  (instead of e.g. abort()) on panic.  Valgrind will then indicate the C
 *  call stack leading to the panic.
 */

#define DUK_CAUSE_SEGFAULT()  do { \
		*((volatile duk_uint32_t *) NULL) = (duk_uint32_t) 0xdeadbeefUL; \
	} while (0)

/*
 *  Macro for suppressing warnings for potentially unreferenced variables.
 *  The variables can be actually unreferenced or unreferenced in some
 *  specific cases only; for instance, if a variable is only debug printed,
 *  it is unreferenced when debug printing is disabled.
 *
 *  (Introduced here because it's potentially compiler specific.)
 */

#define DUK_UNREF(x)  do { \
		(void) (x); \
	} while (0)

/*
 *  DUK_NORETURN: macro for declaring a 'noreturn' function.
 *  Unfortunately the noreturn declaration may appear in various
 *  places of a function declaration, so the solution is to wrap
 *  the entire declaration inside the macro.  Compiler support
 *  for using a noreturn declaration on function pointers varies;
 *  this macro must only be used for actual function declarations.
 *
 *  http://gcc.gnu.org/onlinedocs/gcc-4.3.2//gcc/Function-Attributes.html
 *  http://clang.llvm.org/docs/LanguageExtensions.html
 */

#if defined(DUK_F_GCC_VERSION) && (DUK_F_GCC_VERSION >= 20500L)
/* since gcc-2.5 */
#define DUK_NORETURN(decl)  decl __attribute__((noreturn))
#elif defined(__clang__)
/* syntax same as gcc */
#define DUK_NORETURN(decl)  decl __attribute__((noreturn))
#elif defined(DUK_F_MSVC)
/* http://msdn.microsoft.com/en-us/library/aa235362(VS.60).aspx */
#define DUK_NORETURN(decl)  __declspec(noreturn) decl
#else
/* Don't know how to declare a noreturn function, so don't do it; this
 * may cause some spurious compilation warnings (e.g. "variable used
 * uninitialized").
 */
#define DUK_NORETURN(decl)  decl
#endif

/*
 *  Macro for stating that a certain line cannot be reached.
 *
 *  http://gcc.gnu.org/onlinedocs/gcc-4.5.0/gcc/Other-Builtins.html#Other-Builtins
 *  http://clang.llvm.org/docs/LanguageExtensions.html
 */

#if defined(DUK_F_GCC_VERSION) && (DUK_F_GCC_VERSION >= 40500L)
/* since gcc-4.5 */
#define DUK_UNREACHABLE()  do { __builtin_unreachable(); } while(0)
#elif defined(__clang__) && defined(__has_builtin)
#if __has_builtin(__builtin_unreachable)
/* same as gcc */
#define DUK_UNREACHABLE()  do { __builtin_unreachable(); } while(0)
#endif
#else
/* unknown */
#endif

#if !defined(DUK_UNREACHABLE)
/* Don't know how to declare unreachable point, so don't do it; this
 * may cause some spurious compilation warnings (e.g. "variable used
 * uninitialized").
 */
#define DUK_UNREACHABLE()  /* unreachable */
#endif

/*
 *  Likely and unlikely branches.  Using these is not at all a clear cut case,
 *  so the selection is a two-step process: (1) DUK_USE_BRANCH_HINTS is set
 *  if the architecture, compiler etc make it useful to use the hints, and (2)
 *  a separate check determines how to do them.
 *
 *  These macros expect the argument to be a relational expression with an
 *  integer value.  If used with pointers, you should use an explicit check
 *  like:
 *
 *    if (DUK_LIKELY(ptr != NULL)) { ... }
 *
 *  instead of:
 *
 *    if (DUK_LIKELY(ptr)) { ... }
 *
 *  http://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html  (__builtin_expect)
 */

/* pretty much a placeholder now */
#if defined(DUK_F_GCC)
#define DUK_USE_BRANCH_HINTS
#elif defined(DUK_F_CLANG)
#define DUK_USE_BRANCH_HINTS
#else
#undef DUK_USE_BRANCH_HINTS
#endif

#if defined(DUK_USE_BRANCH_HINTS)
#if defined(DUK_F_GCC_VERSION) && (DUK_F_GCC_VERSION >= 40500L)
/* GCC: test not very accurate; enable only in relatively recent builds
 * because of bugs in gcc-4.4 (http://lists.debian.org/debian-gcc/2010/04/msg00000.html)
 */
#define DUK_LIKELY(x)    __builtin_expect((x), 1)
#define DUK_UNLIKELY(x)  __builtin_expect((x), 0)
#elif defined(DUK_F_CLANG)
#define DUK_LIKELY(x)    __builtin_expect((x), 1)
#define DUK_UNLIKELY(x)  __builtin_expect((x), 0)
#endif
#endif  /* DUK_USE_BRANCH_HINTS */

#if !defined(DUK_LIKELY)
#define DUK_LIKELY(x)    (x)
#endif
#if !defined(DUK_UNLIKELY)
#define DUK_UNLIKELY(x)  (x)
#endif

/*
 *  Function inlining control
 *
 *  DUK_NOINLINE: avoid inlining a function.
 *  DUK_INLINE: suggest inlining a function.
 *  DUK_ALWAYS_INLINE: force inlining for critical functions.
 *
 *  Apply to function definition only (not declaration).
 */

#if defined(DUK_F_CLANG) && (defined(DUK_F_C99) || defined(DUK_F_CPP11))
#define DUK_NOINLINE        __attribute__((noinline))
#define DUK_INLINE          inline
#define DUK_ALWAYS_INLINE   inline __attribute__((always_inline))
#elif defined(DUK_F_GCC) && defined(DUK_F_GCC_VERSION) && (defined(DUK_F_C99) || defined(DUK_F_CPP11))
#if (DUK_F_GCC_VERSION >= 30101)
#define DUK_NOINLINE        __attribute__((noinline))
#define DUK_INLINE          inline
#define DUK_ALWAYS_INLINE   inline __attribute__((always_inline))
#endif
#endif

#if !defined(DUK_NOINLINE)
#define DUK_NOINLINE       /*nop*/
#define DUK_INLINE         /*nop*/
#define DUK_ALWAYS_INLINE  /*nop*/
#endif

/* Temporary workaround for GH-323: avoid inlining control when
 * compiling from multiple sources, as it causes compiler trouble.
 */
#if !defined(DUK_SINGLE_FILE)
#undef DUK_NOINLINE
#undef DUK_INLINE
#undef DUK_ALWAYS_INLINE
#define DUK_NOINLINE       /*nop*/
#define DUK_INLINE         /*nop*/
#define DUK_ALWAYS_INLINE  /*nop*/
#endif

/*
 *  Symbol visibility macros
 *
 *  To avoid C++ declaration issues (see GH-63):
 *
 *    - Don't use DUK_LOCAL_DECL for local -data symbols- so that you don't
 *      end up with both a "static" declaration and a definition.
 *
 *    - Wrap any DUK_INTERNAL_DECL with a '#if !defined(DUK_SINGLE_FILE)'
 *      so that the internal declarations (which would map to "static" in
 *      a single file distribution) get dropped.
 */

/* XXX: user override for these? user override for just using the default visibility macros? */
/* XXX: separate macros for function and data may be necessary at some point. */

#if defined(DUK_F_GCC_VERSION)
#if (DUK_F_GCC_VERSION >= 40000) && !(defined(DUK_F_MINGW) || defined(DUK_F_CYGWIN))
/* Might work on earlier versions too but limit to GCC 4+.
 * MinGW should use Windows specific __declspec or no visibility attributes at all,
 * otherwise: "warning: visibility attribute not supported in this configuration; ignored".
 * Same applies to Cygwin GCC.
 */
#define DUK_F_GCC_SYMBOL_VISIBILITY
#endif
#endif
#if defined(DUK_F_CLANG) && !defined(DUK_F_GCC_SYMBOL_VISIBILITY)
#define DUK_F_GCC_SYMBOL_VISIBILITY
#endif
#if defined(DUK_OPT_DLL_BUILD) && defined(_WIN32) && (defined(_MSC_VER) || defined(__GNUC__))
/* __declspec(dllexport) and __declspec(dllimport) only for Windows DLL build.
 * MSVC: any minimum version?
 * MinGW: no minimum version, even gcc-2.95.3 supported dllimport/dllexport.
*/
#define DUK_F_MSVC_DLL_SYMBOL_VISIBILITY
#endif

#if defined(DUK_F_GCC_SYMBOL_VISIBILITY)
/* GCC 4+ visibility attributes. */
#define DUK_EXTERNAL_DECL  __attribute__ ((visibility("default"))) extern
#define DUK_EXTERNAL       __attribute__ ((visibility("default")))
#if defined(DUK_SINGLE_FILE)
#define DUK_INTERNAL_DECL  static
#define DUK_INTERNAL       static
#else
#define DUK_INTERNAL_DECL  __attribute__ ((visibility("hidden"))) extern
#define DUK_INTERNAL       __attribute__ ((visibility("hidden")))
#endif
#elif defined(DUK_F_MSVC_DLL_SYMBOL_VISIBILITY)
/* MSVC dllexport/dllimport: appropriate __declspec depends on whether we're
 * compiling Duktape or the application.
 */
#if defined(DUK_COMPILING_DUKTAPE)
#define DUK_EXTERNAL_DECL  extern __declspec(dllexport)
#define DUK_EXTERNAL       __declspec(dllexport)
#else
#define DUK_EXTERNAL_DECL  extern __declspec(dllimport)
#define DUK_EXTERNAL       should_not_happen
#endif
#if defined(DUK_SINGLE_FILE)
#define DUK_INTERNAL_DECL  static
#define DUK_INTERNAL       static
#else
#define DUK_INTERNAL_DECL  extern
#define DUK_INTERNAL       /*empty*/
#endif
#else
/* Default visibility. */
#define DUK_EXTERNAL_DECL  extern
#define DUK_EXTERNAL       /*empty*/
#if defined(DUK_SINGLE_FILE)
#define DUK_INTERNAL_DECL  static
#define DUK_INTERNAL       static
#else  /* DUK_SINGLE_FILE */
#define DUK_INTERNAL_DECL  extern
#define DUK_INTERNAL       /*empty*/
#endif
#endif

/* For now, these are shared. */
#define DUK_LOCAL_DECL     static
#define DUK_LOCAL          static

/*
 *  __FILE__, __LINE__, __func__ are wrapped.  Especially __func__ is a
 *  problem because it is not available even in some compilers which try
 *  to be C99 compatible (e.g. VBCC with -c99 option).
 */

#define DUK_FILE_MACRO  __FILE__

#define DUK_LINE_MACRO  __LINE__

#if !defined(DUK_F_VBCC) && !defined(DUK_F_MSVC)
#define DUK_FUNC_MACRO  __func__
#else
#define DUK_FUNC_MACRO  "unknown"
#endif

/*
 *  Byteswap macros
 *
 *  These are here so that inline assembly or other platform functions can be
 *  used if available.
 */

#define DUK_BSWAP32(x) \
	((((duk_uint32_t) (x)) >> 24) | \
	 ((((duk_uint32_t) (x)) >> 8) & 0xff00UL) | \
	 ((((duk_uint32_t) (x)) << 8) & 0xff0000UL) | \
	 (((duk_uint32_t) (x)) << 24))

#define DUK_BSWAP16(x) \
	((duk_uint16_t) (x) >> 8) | \
	((duk_uint16_t) (x) << 8)

/*
 *  Architecture string, human readable value exposed in Duktape.env
 */

#if defined(DUK_F_X86)
#define DUK_USE_ARCH_STRING "x86"
#elif defined(DUK_F_X32)
#define DUK_USE_ARCH_STRING "x32"
#elif defined(DUK_F_X64)
#define DUK_USE_ARCH_STRING "x64"
#elif defined(DUK_F_ARM)
#define DUK_USE_ARCH_STRING "arm"
#elif defined(DUK_F_MIPS32)
#define DUK_USE_ARCH_STRING "mips32"
#elif defined(DUK_F_MIPS64)
#define DUK_USE_ARCH_STRING "mips64"
#elif defined(DUK_F_SUPERH)
#define DUK_USE_ARCH_STRING "sh"
#elif defined(DUK_F_PPC)
#define DUK_USE_ARCH_STRING "ppc"
#elif defined(DUK_F_M68K)
#define DUK_USE_ARCH_STRING "m68k"
#elif defined(DUK_F_FLASHPLAYER)
#define DUK_USE_ARCH_STRING "flashplayer"
#elif defined(DUK_F_EMSCRIPTEN)
#define DUK_USE_ARCH_STRING "emscripten"
#else
#define DUK_USE_ARCH_STRING "unknown"
#endif

/*
 *  OS string, human readable value exposed in Duktape.env
 */

#if defined(DUK_F_LINUX)
#define DUK_USE_OS_STRING "linux"
#elif defined(__APPLE__)
/* http://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor */
#if TARGET_IPHONE_SIMULATOR
#define DUK_USE_OS_STRING "iphone-sim"
#elif TARGET_OS_IPHONE
#define DUK_USE_OS_STRING "iphone"
#elif TARGET_OS_MAC
#define DUK_USE_OS_STRING "ios"
#else
#define DUK_USE_OS_STRING "ios-unknown"
#endif
#elif defined(DUK_F_FREEBSD)
#define DUK_USE_OS_STRING "freebsd"
#elif defined(DUK_F_OPENBSD)
#define DUK_USE_OS_STRING "openbsd"
#elif defined(DUK_F_NETBSD)
#define DUK_USE_OS_STRING "netbsd"
#elif defined(DUK_F_BSD)
#define DUK_USE_OS_STRING "bsd"
#elif defined(DUK_F_UNIX)
#define DUK_USE_OS_STRING "unix"
#elif defined(DUK_F_WINDOWS)
#define DUK_USE_OS_STRING "windows"
#elif defined(DUK_F_TOS)
#define DUK_USE_OS_STRING "tos"
#elif defined(DUK_F_AMIGAOS)
#define DUK_USE_OS_STRING "amigaos"
#elif defined(DUK_F_QNX)
#define DUK_USE_OS_STRING "qnx"
#elif defined(DUK_F_TINSPIRE)
#define DUK_USE_OS_STRING "tinspire"
#else
#define DUK_USE_OS_STRING "unknown"
#endif

/*
 *  Compiler string, human readable value exposed in Duktape.env
 */

#if defined(DUK_F_MINGW)
#define DUK_USE_COMPILER_STRING "mingw"
#elif defined(DUK_F_GCC)
#if defined(DUK_F_CPP)
#define DUK_USE_COMPILER_STRING "g++"
#else
#define DUK_USE_COMPILER_STRING "gcc"
#endif
#elif defined(DUK_F_CLANG)
#define DUK_USE_COMPILER_STRING "clang"
#elif defined(DUK_F_MSVC)
#define DUK_USE_COMPILER_STRING "msvc"
#elif defined(DUK_F_VBCC)
#define DUK_USE_COMPILER_STRING "vbcc"
#else
#define DUK_USE_COMPILER_STRING "unknown"
#endif

/*
 *  Target info string
 */

#if defined(DUK_OPT_TARGET_INFO)
#define DUK_USE_TARGET_INFO DUK_OPT_TARGET_INFO
#else
#define DUK_USE_TARGET_INFO "unknown"
#endif

/*
 *  Long control transfer, setjmp/longjmp or alternatives
 *
 *  Signal mask is not saved (when that can be communicated to the platform)
 */

/* dummy non-zero value to be used as an argument for longjmp(), see man longjmp */
#define DUK_LONGJMP_DUMMY_VALUE  1

#if defined(DUK_OPT_SETJMP)
#define DUK_USE_SETJMP
#elif defined(DUK_OPT_UNDERSCORE_SETJMP)
#define DUK_USE_UNDERSCORE_SETJMP
#elif defined(DUK_OPT_SIGSETJMP)
#define DUK_USE_SIGSETJMP
#elif defined(__APPLE__)
/* Use _setjmp() on Apple by default, see GH-55. */
#define DUK_USE_UNDERSCORE_SETJMP
#else
/* The most portable default is setjmp(). */
#define DUK_USE_SETJMP
#endif

#if defined(DUK_USE_UNDERSCORE_SETJMP)
#define DUK_SETJMP(jb)        _setjmp((jb))
#define DUK_LONGJMP(jb)       _longjmp((jb), DUK_LONGJMP_DUMMY_VALUE)
#elif defined(DUK_USE_SIGSETJMP)
#define DUK_SETJMP(jb)        sigsetjmp((jb), 0 /*savesigs*/)
#define DUK_LONGJMP(jb)       siglongjmp((jb), DUK_LONGJMP_DUMMY_VALUE)
#elif defined(DUK_USE_SETJMP)
#define DUK_SETJMP(jb)        setjmp((jb))
#define DUK_LONGJMP(jb)       longjmp((jb), DUK_LONGJMP_DUMMY_VALUE)
#else
#error internal error
#endif

/*
 *  Speed/size and other performance options
 */

/* Use fast ("inline") refcount operations instead of calling out to helpers
 * by default.  The difference in binary size is small (~1kB on x64).
 */
#define DUK_USE_FAST_REFCOUNT_DEFAULT

/* Assert for valstack space but don't check for it in non-assert build.
 * Valstack overruns (writing beyond checked space) is memory unsafe and
 * potentially a segfault.  Produces a smaller and faster binary.
 * (In practice the speed difference is small with -O3 so default to
 * safer behavior for now.)
 */
#undef DUK_USE_VALSTACK_UNSAFE

/* Catch-all flag which can be used to choose between variant algorithms
 * where a speed-size tradeoff exists (e.g. lookup tables).  When it really
 * matters, specific use flags may be appropriate.
 */
#define DUK_USE_PREFER_SIZE

/* Use a sliding window for lexer; slightly larger footprint, slightly faster. */
#define DUK_USE_LEXER_SLIDING_WINDOW

/* Transparent JSON.stringify() fastpath. */
#undef DUK_USE_JSON_STRINGIFY_FASTPATH
#if defined(DUK_OPT_JSON_STRINGIFY_FASTPATH)
#define DUK_USE_JSON_STRINGIFY_FASTPATH
#endif

/*
 *  Tagged type representation (duk_tval)
 */

#undef DUK_USE_PACKED_TVAL
#undef DUK_USE_FULL_TVAL

#if defined(DUK_USE_PACKED_TVAL_POSSIBLE) && !defined(DUK_OPT_NO_PACKED_TVAL)
#define DUK_USE_PACKED_TVAL
#endif

/* Support for 48-bit signed integer duk_tval with transparent semantics. */
#undef DUK_USE_FASTINT
#if defined(DUK_OPT_FASTINT)
#if !defined(DUK_F_HAVE_64BIT)
#error DUK_OPT_FASTINT requires 64-bit integer type support at the moment
#endif
#define DUK_USE_FASTINT
#endif

/*
 *  Memory management options
 */

#define DUK_USE_REFERENCE_COUNTING
#define DUK_USE_DOUBLE_LINKED_HEAP
#define DUK_USE_MARK_AND_SWEEP
#define DUK_USE_MS_STRINGTABLE_RESIZE

#if defined(DUK_OPT_NO_REFERENCE_COUNTING)
#undef DUK_USE_REFERENCE_COUNTING
#undef DUK_USE_DOUBLE_LINKED_HEAP
/* XXX: undef DUK_USE_MS_STRINGTABLE_RESIZE as it is more expensive
 * with more frequent mark-and-sweeps?
 */
#endif

#if defined(DUK_OPT_NO_MARK_AND_SWEEP)
#undef DUK_USE_MARK_AND_SWEEP
#endif

#if defined(DUK_USE_MARK_AND_SWEEP)
#define DUK_USE_VOLUNTARY_GC
#if defined(DUK_OPT_NO_VOLUNTARY_GC)
#undef DUK_USE_VOLUNTARY_GC
#endif
#endif

#if !defined(DUK_USE_MARK_AND_SWEEP) && !defined(DUK_USE_REFERENCE_COUNTING)
#error must have either mark-and-sweep or reference counting enabled
#endif

#if defined(DUK_OPT_NO_MS_STRINGTABLE_RESIZE)
#undef DUK_USE_MS_STRINGTABLE_RESIZE
#endif

#undef DUK_USE_GC_TORTURE
#if defined(DUK_OPT_GC_TORTURE)
#define DUK_USE_GC_TORTURE
#endif

/*
 *  String table options
 */

#if defined(DUK_OPT_STRTAB_CHAIN) && defined(DUK_OPT_STRTAB_CHAIN_SIZE)
/* Low memory algorithm: separate chaining using arrays, fixed size hash */
#define DUK_USE_STRTAB_CHAIN
#define DUK_USE_STRTAB_CHAIN_SIZE  DUK_OPT_STRTAB_CHAIN_SIZE
#else
/* Default algorithm: open addressing (probing) */
#define DUK_USE_STRTAB_PROBE
#endif

/*
 *  Error handling options
 */

#define DUK_USE_AUGMENT_ERROR_CREATE
#define DUK_USE_AUGMENT_ERROR_THROW
#define DUK_USE_TRACEBACKS
#define DUK_USE_ERRCREATE
#define DUK_USE_ERRTHROW

#define DUK_USE_VERBOSE_ERRORS

#if defined(DUK_OPT_NO_AUGMENT_ERRORS)
#undef DUK_USE_AUGMENT_ERROR_CREATE
#undef DUK_USE_AUGMENT_ERROR_THROW
#undef DUK_USE_TRACEBACKS
#undef DUK_USE_ERRCREATE
#undef DUK_USE_ERRTHROW
#elif defined(DUK_OPT_NO_TRACEBACKS)
#undef DUK_USE_TRACEBACKS
#endif

#if defined(DUK_OPT_NO_VERBOSE_ERRORS)
#undef DUK_USE_VERBOSE_ERRORS
#endif

#if defined(DUK_USE_TRACEBACKS)
#if defined(DUK_OPT_TRACEBACK_DEPTH)
#define DUK_USE_TRACEBACK_DEPTH  DUK_OPT_TRACEBACK_DEPTH
#else
#define DUK_USE_TRACEBACK_DEPTH  10
#endif
#endif

/* Include messages in executor internal errors. */
#define DUK_USE_VERBOSE_EXECUTOR_ERRORS

/*
 *  Execution and debugger options
 */

#undef DUK_USE_INTERRUPT_COUNTER
#if defined(DUK_OPT_INTERRUPT_COUNTER)
#define DUK_USE_INTERRUPT_COUNTER
#endif

#undef DUK_USE_EXEC_TIMEOUT_CHECK
#if defined(DUK_OPT_EXEC_TIMEOUT_CHECK)
#define DUK_USE_EXEC_TIMEOUT_CHECK(udata)  DUK_OPT_EXEC_TIMEOUT_CHECK((udata))
#endif

#undef DUK_USE_DEBUGGER_SUPPORT
#if defined(DUK_OPT_DEBUGGER_SUPPORT)
#define DUK_USE_DEBUGGER_SUPPORT
#endif

#undef DUK_USE_DEBUGGER_FWD_PRINTALERT
#if defined(DUK_OPT_DEBUGGER_SUPPORT) && defined(DUK_OPT_DEBUGGER_FWD_PRINTALERT)
#define DUK_USE_DEBUGGER_FWD_PRINTALERT
#endif

#undef DUK_USE_DEBUGGER_FWD_LOGGING
#if defined(DUK_OPT_DEBUGGER_SUPPORT) && defined(DUK_OPT_DEBUGGER_FWD_LOGGING)
#define DUK_USE_DEBUGGER_FWD_LOGGING
#endif

/* DumpHeap is optional because it's not always needed and has a relatively
 * large footprint.
 */
#undef DUK_USE_DEBUGGER_DUMPHEAP
#if defined(DUK_OPT_DEBUGGER_DUMPHEAP)
#define DUK_USE_DEBUGGER_DUMPHEAP
#endif

/* Debugger transport read/write torture. */
#undef DUK_USE_DEBUGGER_TRANSPORT_TORTURE
#if defined(DUK_OPT_DEBUGGER_TRANSPORT_TORTURE)
#define DUK_USE_DEBUGGER_TRANSPORT_TORTURE
#endif

/* For opcodes with indirect indices, check final index against stack size.
 * This should not be necessary because the compiler is trusted, and we don't
 * bound check non-indirect indices either.
 */
#undef DUK_USE_EXEC_INDIRECT_BOUND_CHECK
#if defined(DUK_OPT_DEBUG) || defined(DUK_OPT_ASSERTIONS)
/* Enabled with debug/assertions just so that any issues can be caught. */
#define DUK_USE_EXEC_INDIRECT_BOUND_CHECK
#endif

/*
 *  Debug printing and assertion options
 */

#undef DUK_USE_DEBUG
#undef DUK_USE_DPRINT
#undef DUK_USE_DDPRINT
#undef DUK_USE_DDDPRINT
#undef DUK_USE_DPRINT_RDTSC
#undef DUK_USE_ASSERTIONS

/* Global debug enable.  Compile must be clean on C99 regardless of whether or
 * not debugging is enabled.  On non-C99 platforms compile should be clean with
 * debugging disabled but may produce warnings with debugging enabled (related
 * to debug macro hackery and such).
 */
#if defined(DUK_OPT_DEBUG)
#define DUK_USE_DEBUG
#endif

#if defined(DUK_OPT_DEBUG) && defined(DUK_OPT_DPRINT)
#define DUK_USE_DPRINT
#endif
#if defined(DUK_OPT_DEBUG) && defined(DUK_OPT_DDPRINT)
#define DUK_USE_DDPRINT
#endif
#if defined(DUK_OPT_DEBUG) && defined(DUK_OPT_DDDPRINT)
#define DUK_USE_DDDPRINT
#endif

#undef DUK_USE_DPRINT_COLORS
#if defined(DUK_OPT_DPRINT_COLORS)
#define DUK_USE_DPRINT_COLORS
#endif

#if defined(DUK_USE_RDTSC) && defined(DUK_OPT_DPRINT_RDTSC)
#define DUK_USE_DPRINT_RDTSC
#else
#undef DUK_USE_DPRINT_RDTSC
#endif

#if defined(DUK_OPT_ASSERTIONS)
#define DUK_USE_ASSERTIONS
#endif

/* The static buffer for debug printing is quite large by default, so there
 * is an option to shrink it manually for constrained builds.
 */
#if defined(DUK_OPT_DEBUG_BUFSIZE)
#define DUK_USE_DEBUG_BUFSIZE  DUK_OPT_DEBUG_BUFSIZE
#else
#define DUK_USE_DEBUG_BUFSIZE  65536L
#endif

/*
 *  Ecmascript features / compliance options
 */

#if defined(DUK_F_BCC)
/* Math built-in is stubbed out on BCC to allow compiler torture testing. */
#else
#define DUK_USE_MATH_BUILTIN
#endif

#define DUK_USE_STRICT_DECL
#if defined(DUK_OPT_NO_STRICT_DECL)
#undef DUK_USE_STRICT_DECL
#endif

#define DUK_USE_REGEXP_SUPPORT
#if defined(DUK_OPT_NO_REGEXP_SUPPORT)
#undef DUK_USE_REGEXP_SUPPORT
#endif

#undef DUK_USE_STRICT_UTF8_SOURCE
#if defined(DUK_OPT_STRICT_UTF8_SOURCE)
#define DUK_USE_STRICT_UTF8_SOURCE
#endif

#define DUK_USE_OCTAL_SUPPORT
#if defined(DUK_OPT_NO_OCTAL_SUPPORT)
#undef DUK_USE_OCTAL_SUPPORT
#endif

#define DUK_USE_SOURCE_NONBMP
#if defined(DUK_OPT_NO_SOURCE_NONBMP)
#undef DUK_USE_SOURCE_NONBMP
#endif

#define DUK_USE_BROWSER_LIKE
#if defined(DUK_OPT_NO_BROWSER_LIKE)
#undef DUK_USE_BROWSER_LIKE
#endif

/* E5/E5.1 Section B features. */
#define DUK_USE_SECTION_B
#if defined(DUK_OPT_NO_SECTION_B)
#undef DUK_USE_SECTION_B
#endif

/* Non-standard regexp parsing features. */
#define DUK_USE_NONSTD_REGEXP_DOLLAR_ESCAPE

/* Treat function statements (function declarations outside top level of
 * Program or FunctionBody) same as normal function declarations.  This is
 * also V8 behavior.  See test-dev-func-decl-outside-top.js.
 */
#define DUK_USE_NONSTD_FUNC_STMT
#if defined(DUK_OPT_NO_NONSTD_FUNC_STMT)
#undef DUK_USE_NONSTD_FUNC_STMT
#endif

/* Array.prototype.splice() non-standard but real world compatible behavior
 * when deleteCount is omitted.
 */
#define DUK_USE_NONSTD_ARRAY_SPLICE_DELCOUNT
#if defined(DUK_OPT_NO_NONSTD_ARRAY_SPLICE_DELCOUNT)
#undef DUK_USE_NONSTD_ARRAY_SPLICE_DELCOUNT
#endif

/* Array.prototype.concat() non-standard but real world compatible behavior
 * for non-existent trailing elements.
 */
#define DUK_USE_NONSTD_ARRAY_CONCAT_TRAILER
#if defined(DUK_OPT_NO_NONSTD_ARRAY_CONCAT_TRAILER)
#undef DUK_USE_NONSTD_ARRAY_CONCAT_TRAILER
#endif

/* Array.prototype.map() non-standard but real world compatible behavior
 * for non-existent trailing elements.
 */
#define DUK_USE_NONSTD_ARRAY_MAP_TRAILER
#if defined(DUK_OPT_NO_NONSTD_ARRAY_MAP_TRAILER)
#undef DUK_USE_NONSTD_ARRAY_MAP_TRAILER
#endif

/* Non-standard 'caller' property for function instances, see
 * test-bi-function-nonstd-caller-prop.js.
 */
#undef DUK_USE_NONSTD_FUNC_CALLER_PROPERTY
#if defined(DUK_OPT_NONSTD_FUNC_CALLER_PROPERTY)
#define DUK_USE_NONSTD_FUNC_CALLER_PROPERTY
#endif

/* Non-standard Object.prototype.__proto__ (ES6), see
 * test-bi-object-proto-__proto__.js.
 */
#define DUK_USE_ES6_OBJECT_PROTO_PROPERTY
#if defined(DUK_OPT_NO_ES6_OBJECT_PROTO_PROPERTY)
#undef DUK_USE_ES6_OBJECT_PROTO_PROPERTY
#endif

/* Non-standard Object.setPrototypeOf (ES6), see
 * test-bi-object-setprototypeof.js.
 */
#define DUK_USE_ES6_OBJECT_SETPROTOTYPEOF
#if defined(DUK_OPT_NO_ES6_OBJECT_SETPROTOTYPEOF)
#undef DUK_USE_ES6_OBJECT_SETPROTOTYPEOF
#endif

/* ES6 Proxy object (subset for now). */
#define DUK_USE_ES6_PROXY
#if defined(DUK_OPT_NO_ES6_PROXY)
#undef DUK_USE_ES6_PROXY
#endif

/* Record pc-to-line information. */
#define DUK_USE_PC2LINE
#if defined(DUK_OPT_NO_PC2LINE)
#undef DUK_USE_PC2LINE
#endif

/* Non-standard function 'source' property. */
#undef DUK_USE_NONSTD_FUNC_SOURCE_PROPERTY
#if defined(DUK_OPT_NONSTD_FUNC_SOURCE_PROPERTY)
#define DUK_USE_NONSTD_FUNC_SOURCE_PROPERTY
#endif

/* CommonJS modules */
#define DUK_USE_COMMONJS_MODULES
#if defined(DUK_OPT_NO_COMMONJS_MODULES)
#undef DUK_USE_COMMONJS_MODULES
#endif

/* Additional key argument to setter/getter calls when triggered by property
 * accesses.
 */

#define DUK_USE_NONSTD_GETTER_KEY_ARGUMENT
#define DUK_USE_NONSTD_SETTER_KEY_ARGUMENT
#if defined(DUK_OPT_NO_NONSTD_ACCESSOR_KEY_ARGUMENT)
#undef DUK_USE_NONSTD_GETTER_KEY_ARGUMENT
#undef DUK_USE_NONSTD_SETTER_KEY_ARGUMENT
#endif

/* JSON escaping of U+2028 and U+2029.
 */

#define DUK_USE_NONSTD_JSON_ESC_U2028_U2029
#if defined(DUK_OPT_NO_NONSTD_JSON_ESC_U2028_U2029)
#undef DUK_USE_NONSTD_JSON_ESC_U2028_U2029
#endif

/* Allow 32-bit codepoints in String.fromCharCode. */
#define DUK_USE_NONSTD_STRING_FROMCHARCODE_32BIT
#if defined(DUK_OPT_NO_NONSTD_STRING_FROMCHARCODE_32BIT)
#undef DUK_USE_NONSTD_STRING_FROMCHARCODE_32BIT
#endif

/* Non-standard array fast path write behavior: when writing to numeric
 * indexes of an Array instance, assume Array.prototype doesn't have
 * conflicting properties (e.g. a non-writable property "7").
 */
#define DUK_USE_NONSTD_ARRAY_WRITE
#if defined(DUK_OPT_NO_NONSTD_ARRAY_WRITE)
#undef DUK_USE_NONSTD_ARRAY_WRITE
#endif

/* Node.js Buffer and Khronos/ES6 typed array support. */
#define DUK_USE_BUFFEROBJECT_SUPPORT
#if defined(DUK_OPT_NO_BUFFEROBJECT_SUPPORT)
#undef DUK_USE_BUFFEROBJECT_SUPPORT
#endif

/*
 *  Optional C API options
 */

#define DUK_USE_BYTECODE_DUMP_SUPPORT
#if defined(DUK_OPT_NO_BYTECODE_DUMP_SUPPORT)
#undef DUK_USE_BYTECODE_DUMP_SUPPORT
#endif

/*
 *  Tailcalls
 */

/* Tailcalls are enabled by default.  The non-standard function 'caller'
 * property feature conflicts with tailcalls quite severely so tailcalls
 * are disabled if the 'caller' property is enabled.
 */
#define DUK_USE_TAILCALL
#if defined(DUK_USE_NONSTD_FUNC_CALLER_PROPERTY)
#undef DUK_USE_TAILCALL
#endif

/*
 *  Ecmascript compiler
 */

/* Ensure final bytecode never exceeds a certain byte size and never uses
 * line numbers above a certain limit.  This ensures that there is no need
 * to deal with unbounded ranges in e.g. pc2line data structures.  For now,
 * limits are set so that signed 32-bit values can represent line number
 * and byte offset with room to spare.
 */
#define DUK_USE_ESBC_LIMITS
#define DUK_USE_ESBC_MAX_LINENUMBER  0x7fff0000L
#define DUK_USE_ESBC_MAX_BYTES       0x7fff0000L

#undef DUK_USE_SHUFFLE_TORTURE
#if defined(DUK_OPT_SHUFFLE_TORTURE)
#define DUK_USE_SHUFFLE_TORTURE
#endif

/*
 *  User panic handler, panic exit behavior for default panic handler
 */

#undef DUK_USE_PANIC_HANDLER
#if defined(DUK_OPT_PANIC_HANDLER)
#define DUK_USE_PANIC_HANDLER(code,msg) DUK_OPT_PANIC_HANDLER((code),(msg))
#endif

#undef DUK_USE_PANIC_ABORT
#undef DUK_USE_PANIC_EXIT
#undef DUK_USE_PANIC_SEGFAULT

#if defined(DUK_OPT_SEGFAULT_ON_PANIC)
#define DUK_USE_PANIC_SEGFAULT
#else
#define DUK_USE_PANIC_ABORT
#endif

/*
 *  File I/O support.  This is now used in a few API calls to e.g. push
 *  a string from file contents or eval a file.  For portability it must
 *  be possible to disable I/O altogether.
 */

#undef DUK_USE_FILE_IO
#if !defined(DUK_OPT_NO_FILE_IO)
#define DUK_USE_FILE_IO
#endif

/*
 *  Optional run-time self tests executed when a heap is created.  Some
 *  platform/compiler issues cannot be determined at compile time.  One
 *  particular example is the bug described in misc/clang_aliasing.c.
 */

#undef DUK_USE_SELF_TESTS
#if defined(DUK_OPT_SELF_TESTS)
#define DUK_USE_SELF_TESTS
#endif

/* Double aliasing testcase fails when Emscripten-generated code is run
 * on Firefox.  This is not fatal because it only affects packed duk_tval
 * which we avoid with Emscripten.
 */
#undef DUK_USE_NO_DOUBLE_ALIASING_SELFTEST
#if defined(DUK_F_EMSCRIPTEN)
#define DUK_USE_NO_DOUBLE_ALIASING_SELFTEST
#endif

/*
 *  Codecs
 */

#define DUK_USE_JX
#if defined(DUK_OPT_NO_JX)
#undef DUK_USE_JX
#endif

#define DUK_USE_JC
#if defined(DUK_OPT_NO_JC)
#undef DUK_USE_JC
#endif

/*
 *  InitJS code
 */

/* Always use the built-in InitJS code for now. */
#define DUK_USE_BUILTIN_INITJS

/* User provided InitJS. */
#undef DUK_USE_USER_INITJS
#if defined(DUK_OPT_USER_INITJS)
#define DUK_USE_USER_INITJS (DUK_OPT_USER_INITJS)
#endif

/*
 *  External string data support
 *
 *  Allow duk_hstrings to store data also behind an external pointer (see
 *  duk_hstring_external).  This increases code size slightly but is useful
 *  in low memory environments where memory is more limited than flash.
 */

#undef DUK_USE_HSTRING_EXTDATA
#if defined(DUK_OPT_EXTERNAL_STRINGS)
#define DUK_USE_HSTRING_EXTDATA
#endif

#undef DUK_USE_EXTSTR_INTERN_CHECK
#if defined(DUK_OPT_EXTERNAL_STRINGS) && defined(DUK_OPT_EXTSTR_INTERN_CHECK)
#define DUK_USE_EXTSTR_INTERN_CHECK(udata,ptr,len) DUK_OPT_EXTSTR_INTERN_CHECK((udata), (ptr), (len))
#endif

#undef DUK_USE_EXTSTR_FREE
#if defined(DUK_OPT_EXTERNAL_STRINGS) && defined(DUK_OPT_EXTSTR_FREE)
#define DUK_USE_EXTSTR_FREE(udata,ptr) DUK_OPT_EXTSTR_FREE((udata), (ptr))
#endif

/*
 *  Lightweight functions
 */

/* Force built-ins to use lightfunc function pointers when possible.  This
 * makes the built-in functions non-compliant with respect to their property
 * values and such, but is very useful in low memory environments (can save
 * around 14kB of initial RAM footprint).
 */
#undef DUK_USE_LIGHTFUNC_BUILTINS
#if defined(DUK_OPT_LIGHTFUNC_BUILTINS)
#define DUK_USE_LIGHTFUNC_BUILTINS
#endif

/*
 *  Pointer compression and 16-bit header fields for low memory environments
 */

#undef DUK_USE_HEAPPTR16
#undef DUK_USE_HEAPPTR_ENC16
#undef DUK_USE_HEAPPTR_DEC16
#if defined(DUK_OPT_HEAPPTR16) && defined(DUK_OPT_HEAPPTR_ENC16) && defined(DUK_OPT_HEAPPTR_DEC16)
#define DUK_USE_HEAPPTR16
#define DUK_USE_HEAPPTR_ENC16(udata,ptr) DUK_OPT_HEAPPTR_ENC16((udata),(ptr))
#define DUK_USE_HEAPPTR_DEC16(udata,ptr) DUK_OPT_HEAPPTR_DEC16((udata),(ptr))
#endif

#undef DUK_USE_DATAPTR16
#undef DUK_USE_DATAPTR_ENC16
#undef DUK_USE_DATAPTR_DEC16
#if defined(DUK_OPT_DATAPTR16) && defined(DUK_OPT_DATAPTR_ENC16) && defined(DUK_OPT_DATAPTR_DEC16)
#define DUK_USE_DATAPTR16
#define DUK_USE_DATAPTR_ENC16(udata,ptr) DUK_OPT_DATAPTR_ENC16((udata),(ptr))
#define DUK_USE_DATAPTR_DEC16(udata,ptr) DUK_OPT_DATAPTR_DEC16((udata),(ptr))
#endif

#undef DUK_USE_FUNCPTR16
#undef DUK_USE_FUNCPTR_ENC16
#undef DUK_USE_FUNCPTR_DEC16
#if defined(DUK_OPT_FUNCPTR16) && defined(DUK_OPT_FUNCPTR_ENC16) && defined(DUK_OPT_FUNCPTR_DEC16)
#define DUK_USE_FUNCPTR16
#define DUK_USE_FUNCPTR_ENC16(udata,ptr) DUK_OPT_FUNCPTR_ENC16((udata),(ptr))
#define DUK_USE_FUNCPTR_DEC16(udata,ptr) DUK_OPT_FUNCPTR_DEC16((udata),(ptr))
#endif

#undef DUK_USE_REFCOUNT16
#if defined(DUK_OPT_REFCOUNT16)
#define DUK_USE_REFCOUNT16
#endif

#undef DUK_USE_STRHASH16
#if defined(DUK_OPT_STRHASH16)
#define DUK_USE_STRHASH16
#endif

#undef DUK_USE_STRLEN16
#if defined(DUK_OPT_STRLEN16)
#define DUK_USE_STRLEN16
#endif

#undef DUK_USE_BUFLEN16
#if defined(DUK_OPT_BUFLEN16)
#define DUK_USE_BUFLEN16
#endif

#undef DUK_USE_OBJSIZES16
#if defined(DUK_OPT_OBJSIZES16)
#define DUK_USE_OBJSIZES16
#endif

/* For now, hash part is dropped if and only if 16-bit object fields are used. */
#define DUK_USE_HOBJECT_HASH_PART
#if defined(DUK_USE_OBJSIZES16)
#undef DUK_USE_HOBJECT_HASH_PART
#endif

/*
 *  Miscellaneous
 */

/* Convenience define: 32-bit pointers.  32-bit platforms are an important
 * footprint optimization target, and this define allows e.g. struct sizes
 * to be organized for compactness.
 */
#undef DUK_USE_32BIT_PTRS
#if defined(DUK_UINTPTR_MAX) && !defined(DUK_UINTPTR_MAX_COMPUTED)
#if DUK_UINTPTR_MAX <= 0xffffffffUL
#define DUK_USE_32BIT_PTRS
#endif
#endif

#define DUK_USE_PROVIDE_DEFAULT_ALLOC_FUNCTIONS
#undef DUK_USE_EXPLICIT_NULL_INIT

#define DUK_USE_ZERO_BUFFER_DATA
#if defined(DUK_OPT_NO_ZERO_BUFFER_DATA)
#undef DUK_USE_ZERO_BUFFER_DATA
#endif

#undef DUK_USE_VARIADIC_MACROS
#if defined(DUK_F_C99) || (defined(DUK_F_CPP11) && defined(__GNUC__))
#define DUK_USE_VARIADIC_MACROS
#endif
#if defined(_MSC_VER) && !defined(DUK_USE_VARIADIC_MACROS)
#if (_MSC_VER >= 1400)
/* VS2005+ should have variadic macros even when they're not C99. */
#define DUK_USE_VARIADIC_MACROS
#endif
#endif

/*
 *  Variable size array initialization.
 *
 *  Variable size array at the end of a structure is nonportable.
 *  There are three alternatives:
 *
 *    1) C99 (flexible array member): char buf[]
 *    2) Compiler specific (e.g. GCC): char buf[0]
 *    3) Portable but wastes memory / complicates allocation: char buf[1]
 */

/* XXX: Currently unused, only hbuffer.h needed this at some point. */
#undef DUK_USE_FLEX_C99
#undef DUK_USE_FLEX_ZEROSIZE
#undef DUK_USE_FLEX_ONESIZE
#if defined(DUK_F_C99)
#define DUK_USE_FLEX_C99
#elif defined(__GNUC__)
#define DUK_USE_FLEX_ZEROSIZE
#else
#define DUK_USE_FLEX_ONESIZE
#endif

/*
 *  GCC pragmas
 */

/* XXX: GCC pragma inside a function fails in some earlier GCC versions (e.g. gcc 4.5).
 * This is very approximate but allows clean builds for development right now.
 */
/* http://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html */
#if defined(__GNUC__) && defined(__GNUC_MINOR__) && (__GNUC__ == 4) && (__GNUC_MINOR__ >= 6)
#define DUK_USE_GCC_PRAGMAS
#else
#undef DUK_USE_GCC_PRAGMAS
#endif

/*
 *  User declarations
 */

#if defined(DUK_OPT_DECLARE)
#define DUK_USE_USER_DECLARE() DUK_OPT_DECLARE
#else
#define DUK_USE_USER_DECLARE() /* no user declarations */
#endif

/*
 *  Autogenerated defaults
 */

#define DUK_USE_COMPILER_RECLIMIT 2500
#undef DUK_USE_DATE_FORMAT_STRING
#undef DUK_USE_DATE_GET_LOCAL_TZOFFSET
#undef DUK_USE_DATE_GET_NOW
#undef DUK_USE_DATE_PARSE_STRING
#undef DUK_USE_DATE_PRS_GETDATE
#undef DUK_USE_INTEGER_ME
#define DUK_USE_JSON_DECNUMBER_FASTPATH
#define DUK_USE_JSON_DECSTRING_FASTPATH
#define DUK_USE_JSON_DEC_RECLIMIT 1000
#define DUK_USE_JSON_EATWHITE_FASTPATH
#define DUK_USE_JSON_ENC_RECLIMIT 1000
#define DUK_USE_JSON_QUOTESTRING_FASTPATH
#undef DUK_USE_MARKANDSWEEP_FINALIZER_TORTURE
#define DUK_USE_MARK_AND_SWEEP_RECLIMIT 256
#define DUK_USE_NATIVE_CALL_RECLIMIT 1000
#undef DUK_USE_REFZERO_FINALIZER_TORTURE
#define DUK_USE_REGEXP_COMPILER_RECLIMIT 10000
#define DUK_USE_REGEXP_EXECUTOR_RECLIMIT 10000

/*
 *  Alternative customization header
 *
 *  If you want to modify the final DUK_USE_xxx flags directly (without
 *  using the available DUK_OPT_xxx flags), define DUK_OPT_HAVE_CUSTOM_H
 *  and tweak the final flags there.
 */

#if defined(DUK_OPT_HAVE_CUSTOM_H)
#include "duk_custom.h"
#endif

/*
 *  You may add overriding #define/#undef directives below for
 *  customization.  You of course cannot un-#include or un-typedef
 *  anything; these require direct changes above.
 */

/* __OVERRIDE_DEFINES__ */

/*
 *  Date provider selection
 *
 *  User may define DUK_USE_DATE_GET_NOW() etc directly, in which case we'll
 *  rely on an external provider.  If this is not done, revert to previous
 *  behavior and use Unix/Windows built-in provider.
 */

#if defined(DUK_COMPILING_DUKTAPE)

#if defined(DUK_USE_DATE_GET_NOW)
/* External provider already defined. */
#elif defined(DUK_USE_DATE_NOW_GETTIMEOFDAY)
DUK_INTERNAL_DECL duk_double_t duk_bi_date_get_now_gettimeofday(duk_context *ctx);
#define DUK_USE_DATE_GET_NOW(ctx)            duk_bi_date_get_now_gettimeofday((ctx))
#elif defined(DUK_USE_DATE_NOW_TIME)
DUK_INTERNAL_DECL duk_double_t duk_bi_date_get_now_time(duk_context *ctx);
#define DUK_USE_DATE_GET_NOW(ctx)            duk_bi_date_get_now_time((ctx))
#elif defined(DUK_USE_DATE_NOW_WINDOWS)
DUK_INTERNAL_DECL duk_double_t duk_bi_date_get_now_windows(duk_context *ctx);
#define DUK_USE_DATE_GET_NOW(ctx)            duk_bi_date_get_now_windows((ctx))
#else
#error no provider for DUK_USE_DATE_GET_NOW()
#endif

#if defined(DUK_USE_DATE_GET_LOCAL_TZOFFSET)
/* External provider already defined. */
#elif defined(DUK_USE_DATE_TZO_GMTIME_R) || defined(DUK_USE_DATE_TZO_GMTIME)
DUK_INTERNAL_DECL duk_int_t duk_bi_date_get_local_tzoffset_gmtime(duk_double_t d);
#define DUK_USE_DATE_GET_LOCAL_TZOFFSET(d)   duk_bi_date_get_local_tzoffset_gmtime((d))
#elif defined(DUK_USE_DATE_TZO_WINDOWS)
DUK_INTERNAL_DECL duk_int_t duk_bi_date_get_local_tzoffset_windows(duk_double_t d);
#define DUK_USE_DATE_GET_LOCAL_TZOFFSET(d)   duk_bi_date_get_local_tzoffset_windows((d))
#else
#error no provider for DUK_USE_DATE_GET_LOCAL_TZOFFSET()
#endif

#if defined(DUK_USE_DATE_PARSE_STRING)
/* External provider already defined. */
#elif defined(DUK_USE_DATE_PRS_STRPTIME)
DUK_INTERNAL_DECL duk_bool_t duk_bi_date_parse_string_strptime(duk_context *ctx, const char *str);
#define DUK_USE_DATE_PARSE_STRING(ctx,str)   duk_bi_date_parse_string_strptime((ctx), (str))
#elif defined(DUK_USE_DATE_PRS_GETDATE)
DUK_INTERNAL_DECL duk_bool_t duk_bi_date_parse_string_getdate(duk_context *ctx, const char *str);
#define DUK_USE_DATE_PARSE_STRING(ctx,str)   duk_bi_date_parse_string_getdate((ctx), (str))
#else
/* No provider for DUK_USE_DATE_PARSE_STRING(), fall back to ISO 8601 only. */
#endif

#if defined(DUK_USE_DATE_FORMAT_STRING)
/* External provider already defined. */
#elif defined(DUK_USE_DATE_FMT_STRFTIME)
DUK_INTERNAL_DECL duk_bool_t duk_bi_date_format_parts_strftime(duk_context *ctx, duk_int_t *parts, duk_int_t tzoffset, duk_small_uint_t flags);
#define DUK_USE_DATE_FORMAT_STRING(ctx,parts,tzoffset,flags) \
	duk_bi_date_format_parts_strftime((ctx), (parts), (tzoffset), (flags))
#else
/* No provider for DUK_USE_DATE_FORMAT_STRING(), fall back to ISO 8601 only. */
#endif

#endif  /* DUK_COMPILING_DUKTAPE */

/*
 *  Sanity check for the final effective internal defines.  Also
 *  double checks user tweaks made by an optional duk_custom.h header.
 */

/*
 *  Deprecated feature options.
 *
 *  Catch so that user more easily notices and updates build.
 */

#if defined(DUK_OPT_NO_FUNC_STMT)
#error DUK_OPT_NO_FUNC_STMT is deprecated, use DUK_OPT_NO_NONSTD_FUNC_STMT
#endif

#if defined(DUK_OPT_FUNC_NONSTD_CALLER_PROPERTY)
#error DUK_OPT_FUNC_NONSTD_CALLER_PROPERTY is deprecated, use DUK_OPT_NONSTD_FUNC_CALLER_PROPERTY
#endif

#if defined(DUK_OPT_FUNC_NONSTD_SOURCE_PROPERTY)
#error DUK_OPT_FUNC_NONSTD_SOURCE_PROPERTY is deprecated, use DUK_OPT_NONSTD_FUNC_SOURCE_PROPERTY
#endif

#if defined(DUK_OPT_NO_ARRAY_SPLICE_NONSTD_DELCOUNT)
#error DUK_OPT_NO_ARRAY_SPLICE_NONSTD_DELCOUNT is deprecated, use DUK_OPT_NO_NONSTD_ARRAY_SPLICE_DELCOUNT
#endif

#if defined(DUK_OPT_NO_OBJECT_ES6_PROTO_PROPERTY)
#error DUK_OPT_NO_OBJECT_ES6_PROTO_PROPERTY is deprecated, use DUK_OPT_NO_ES6_OBJECT_PROTO_PROPERTY
#endif

#if defined(DUK_OPT_NO_OBJECT_ES6_SETPROTOTYPEOF)
#error DUK_OPT_NO_OBJECT_ES6_SETPROTOTYPEOF is deprecated, use DUK_OPT_NO_ES6_OBJECT_SETPROTOTYPEOF
#endif

#if defined(DUK_OPT_NO_JSONX)
#error DUK_OPT_NO_JSONX is deprecated, use DUK_OPT_NO_JX
#endif

#if defined(DUK_OPT_NO_JSONC)
#error DUK_OPT_NO_JSONC is deprecated, use DUK_OPT_NO_JC
#endif

/*
 *  Debug print consistency
 */

#if defined(DUK_USE_DPRINT) && !defined(DUK_USE_DEBUG)
#error DUK_USE_DPRINT without DUK_USE_DEBUG
#endif

#if defined(DUK_USE_DDPRINT) && !defined(DUK_USE_DEBUG)
#error DUK_USE_DDPRINT without DUK_USE_DEBUG
#endif

#if defined(DUK_USE_DDDPRINT) && !defined(DUK_USE_DEBUG)
#error DUK_USE_DDDPRINT without DUK_USE_DEBUG
#endif

#if defined(DUK_USE_HEAPPTR16) && defined(DUK_USE_DEBUG)
/* Debug code doesn't have access to 'heap' so it cannot decode pointers. */
#error debug printing cannot currently be used with heap pointer compression
#endif

/*
 *  Debugger consistency
 */

#if defined(DUK_USE_DEBUGGER_SUPPORT)
#if !defined(DUK_USE_INTERRUPT_COUNTER)
#error DUK_USE_INTERRUPT_COUNTER is needed when debugger support is enabled
#endif
#if !defined(DUK_USE_PC2LINE)
#error DUK_USE_PC2LINE is needed when debugger support is enabled
#endif
#endif

/*
 *  Garbage collection consistency
 */

#if defined(DUK_USE_REFERENCE_COUNTING) && !defined(DUK_USE_DOUBLE_LINKED_HEAP)
#error DUK_USE_REFERENCE_COUNTING defined without DUK_USE_DOUBLE_LINKED_HEAP
#endif

#if defined(DUK_USE_GC_TORTURE) && !defined(DUK_USE_MARK_AND_SWEEP)
#error DUK_USE_GC_TORTURE defined without DUK_USE_MARK_AND_SWEEP
#endif

/*
 *  Low memory feature consistency
 */

#if defined(DUK_USE_OBJSIZES16)
#if defined(DUK_USE_HOBJECT_HASH_PART)
#error DUK_USE_OBJSIZES16 assumes DUK_USE_HOBJECT_HASH_PART is not defined
#endif
#endif

#if defined(DUK_USE_STRTAB_CHAIN) && defined(DUK_USE_STRTAB_PROBE)
#error both DUK_USE_STRTAB_CHAIN and DUK_USE_STRTAB_PROBE defined
#endif
#if !defined(DUK_USE_STRTAB_CHAIN) && !defined(DUK_USE_STRTAB_PROBE)
#error neither DUK_USE_STRTAB_CHAIN nor DUK_USE_STRTAB_PROBE is defined
#endif

#endif  /* DUK_CONFIG_H_INCLUDED */
