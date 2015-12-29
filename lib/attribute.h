#ifndef __SEED_ATTRIBUTE_H
#define __SEED_ATTRIBUTE_H

#ifdef __GNUC__
#define __Attribute__(X) __attribute__(X)
#else
#define __Attribute__(X)
#endif

#endif
