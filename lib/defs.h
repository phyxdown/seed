#ifndef __SEED_DEFS_H
#define __SEED_DEFS_H

#ifdef __GNUC__
#define __Attribute__(X) __attribute__(X)
#else
#define __Attribute__(X)
#endif

#ifdef __GNUC__
#define tls __thread
#endif

#endif
