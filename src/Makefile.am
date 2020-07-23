# Our Binary is produced here
bin_PROGRAMS = $(top_srcdir)/bin/simppru

# Provide Cflags to point to path of header files
AM_CFLAGS = -I$(top_srcdir)/include/ -I$(top_srcdir)/third-party/
AM_YFLAGS = -d

noinst_HEADERS = $(top_srcdir)/include/*.h $(top_srcdir)/third-party/cJSON/cJSON.h $(top_srcdir)/third-party/vec/vec.h $(top_srcdir)/third-party/klib/*.h
__top_srcdir__bin_simppru_SOURCES = ast.c code_printer.c main.c pin_config.c semantic.c symbol_table.c parser.y lexer.l

# third-party static libraries
__top_srcdir__bin_simppru_DEPENDENCIES = $(top_builddir)/third-party/cJSON/libcJSON.a $(top_builddir)/third-party/vec/libvec.a
__top_srcdir__bin_simppru_LDADD = $(top_builddir)/third-party/cJSON/libcJSON.a $(top_builddir)/third-party/vec/libvec.a
