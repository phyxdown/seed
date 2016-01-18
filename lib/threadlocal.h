#ifndef __SEED_THREADLOCAL_H
#define __SEED_THREADLOCAL_H

#ifdef __GNUC__
#define seed_tls __thread
#else
#define seed_tls
#endif

#endif
