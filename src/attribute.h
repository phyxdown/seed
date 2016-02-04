#ifndef __SEED_ATTRIBUTE_H
#define __SEED_ATTRIBUTE_H

#ifdef __GNUC__
#define __seed_attribute__(X) __attribute__(X)
#else
#define __seed_attribute__(X)
#endif

#define __seed_unused__(x) seed_unused_ ## x __seed_attribute__((__unused__))

#endif
