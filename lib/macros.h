#ifndef __SEED_MACROS_H
#define __SEED_MACROS_H

#define _BSD_SOURCE

#if defined(__linux__)
#define _GNU_SOURCE
#define _DEFAULT_SOURCE
#endif

#if defined(__linux__) || defined(_OpenBSD_)
#define _XOPEN_SOURCE 700
#endif

#endif
