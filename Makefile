MODULES = uint
DATA_built = uint.sql sql/copy.sql sql/indexes.sql expected/copy.out expected/indexes.out
DATA = uninstall_uint.sql
REGRESS = init cast comparision_ops bit_ops tables insert copy indexes hash_joins restrict_selops
EXTRA_CLEAN = data/copy_binary.data

ifdef USE_PGXS
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
else
subdir = contrib/uint
top_builddir = ../..
include $(top_builddir)/src/Makefile.global
include $(top_srcdir)/contrib/contrib-global.mk
endif

PWD=$(shell pwd)

sql/copy.sql: sql/copy.sql.in
	sed 's,@abs_builddir@,$(PWD),g' sql/copy.sql.in > sql/copy.sql

expected/copy.out: expected/copy.out.in
	sed 's,@abs_builddir@,$(PWD),g' expected/copy.out.in > expected/copy.out

sql/indexes.sql: sql/indexes.sql.in
	sed 's,@abs_builddir@,$(PWD),g' sql/indexes.sql.in > sql/indexes.sql

expected/indexes.out: expected/indexes.out.in
	sed 's,@abs_builddir@,$(PWD),g' expected/indexes.out.in > expected/indexes.out
