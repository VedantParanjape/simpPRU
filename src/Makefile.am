# Our Binary is produced here
bin_PROGRAMS = $(srcdir)/../bin/simppru

# Provide Cflags to point to path of header files
AM_CFLAGS = -I$(srcdir)/../include/ -I$(srcdir)/../third-party/
AM_YFLAGS = -d
__srcdir_____bin_simppru_SOURCES = ast.c code_printer.c main.c pin_config.c semantic.c symbol_table.c parser.y lexer.l

# third-party static libraries
__srcdir_____bin_simppru_DEPENDENCIES = ../third-party/cJSON/libcJSON.a ../third-party/vec/libvec.a
__srcdir_____bin_simppru_LDADD = ../third-party/cJSON/libcJSON.a ../third-party/vec/libvec.a
