#ifndef __DEFS_H__
#define __DEFS_H__

#ifdef __GNUC__
#define __Attribute__(X) __attribute__(X)
#else
#define __Attribute__(X)
#endif

#ifdef __GNUC__
#define tls __thread
#endif

#endif /* __DEFS_H__ */
