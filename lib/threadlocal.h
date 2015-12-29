#ifndef __SEED_THREADLOCAL_H
#define __SEED_THREADLOCAL_H

#ifdef __GNUC__
#define tls __thread
#endif

#endif
