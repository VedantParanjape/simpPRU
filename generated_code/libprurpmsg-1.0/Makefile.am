# Specify inst library, i.e these will be installed
lib_LIBRARIES = $(top_srcdir)/lib/libprurpmsg0.a $(top_srcdir)/lib/libprurpmsg1.a

__top_srcdir__lib_libprurpmsg0_a_CFLAGS = -I$(top_srcdir)/include/pru -I$(top_srcdir)/include/pru/am335x/ -mmcu=am335x.pru0
__top_srcdir__lib_libprurpmsg1_a_CFLAGS = -I$(top_srcdir)/include/pru -I$(top_srcdir)/include/pru/am335x/ -mmcu=am335x.pru1
nobase_include_HEADERS = $(top_srcdir)/include/pru/* $(top_srcdir)/include/pru/am335x/* 

__top_srcdir__lib_libprurpmsg0_a_SOURCES = pru_rpmsg.c pru_virtqueue.c
__top_srcdir__lib_libprurpmsg1_a_SOURCES = pru_rpmsg.c pru_virtqueue.c
