MODULES = uint

EXTENSION  = uint
EXTVERSION = $(shell grep default_version $(EXTENSION).control | \
			   sed -e "s/default_version[[:space:]]*=[[:space:]]*'\([^']*\)'/\1/")
NUMVERSION = $(shell echo $(EXTVERSION) | sed -e 's/\([[:digit:]]*[.][[:digit:]]*\).*/\1/')


DATA_built = $(EXTENSION)--$(EXTVERSION).sql sql/copy.sql sql/indexes.sql expected/copy.out expected/indexes.out
DATA = uninstall_uint.sql
REGRESS = init cast comparision_ops bit_ops tables insert copy indexes hash_joins restrict_selops
EXTRA_CLEAN = data/copy_binary.data

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)

PWD=$(shell pwd)

$(EXTENSION)--$(EXTVERSION).sql: $(EXTENSION).sql
	cp $(EXTENSION).sql $(EXTENSION)--$(EXTVERSION).sql

sql/copy.sql: sql/copy.sql.in
	sed 's,@abs_builddir@,$(PWD),g' sql/copy.sql.in > sql/copy.sql

expected/copy.out: expected/copy.out.in
	sed 's,@abs_builddir@,$(PWD),g' expected/copy.out.in > expected/copy.out

sql/indexes.sql: sql/indexes.sql.in
	sed 's,@abs_builddir@,$(PWD),g' sql/indexes.sql.in > sql/indexes.sql

expected/indexes.out: expected/indexes.out.in
	sed 's,@abs_builddir@,$(PWD),g' expected/indexes.out.in > expected/indexes.out
