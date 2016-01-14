seedincdir = $(includedir)/seed

lib_LTLIBRARIES = libseed.la
libseed_la_SOURCES = attribute.h \
		     aggregator.c aggregator.h \
		     buf.c buf.h \
		     concurrent.c concurrent.h \
		     fmpool.c fmpool.h \
		     list.c list.h \
		     net.c net.h \
		     tcp.c tcp.h \
		     threadlocal.h \
		     time.c time.h 
libseed_CLAGS = -std=c99 -Wall -pedantic -pthread
seedinc_HEADERS = attribute.h buf.h concurrent.h fmpool.h list.h net.h tcp.h threadlocal.h time.h
