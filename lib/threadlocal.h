#ifndef __SEED_THREADLOCAL_H
#define __SEED_THREADLOCAL_H

#ifdef __GNUC__
#define __seed_tls __thread
#else
#define __seed_tls
#endif

#endif
