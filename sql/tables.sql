--
-- uint1 table tests
--

-- Single uint1 column, NULLS permitted, with OIDS.
CREATE TABLE uint1_table_test (
   col1 uint1
) WITH OIDS;
DROP TABLE uint1_table_test;

-- Single uint1 column, NULLS permitted, without OIDS
CREATE TABLE uint1_table_test (
   col1 uint1
) WITHOUT OIDS;
DROP TABLE uint1_table_test;

-- Single uint1 column, NULLS prohibited, without OIDS
CREATE TABLE uint1_table_test (
   col1 uint1 NOT NULL
) WITHOUT OIDS;
DROP TABLE uint1_table_test;

-- Multiple uint1 column, NULLS prohibited, without OIDS
CREATE TABLE uint1_table_test (
   col1 uint1 NOT NULL, 
   col2 uint1 NOT NULL,
   col3 uint1 NOT NULL,
   col4 uint1 NOT NULL,
   col5 uint1 NOT NULL
) WITHOUT OIDS;
DROP TABLE uint1_table_test;

-- Multiple uint1 column, mixed NULLS, without OIDS
CREATE TABLE uint1_table_test (
   col1 uint1, 
   col2 uint1 NOT NULL,
   col3 uint1,
   col4 uint1 NOT NULL,
   col5 uint1,
   col6 uint1 NOT NULL
) WITHOUT OIDS;
DROP TABLE uint1_table_test;

-- Mixed columns, mixed NULLS, without OIDS
CREATE TABLE uint1_table_test (
   col1 "char" NOT NULL, 
   col2 uint1 NOT NULL,
   col3 int2,
   col4 uint1 NOT NULL,
   col5 text
) WITHOUT OIDS;
DROP TABLE uint1_table_test;


--
-- uint2 table tests
--

-- Single uint2 column, NULLS permitted, with OIDS.
CREATE TABLE uint2_table_test (
   col1 uint2
) WITH OIDS;
DROP TABLE uint2_table_test;

-- Single uint2 column, NULLS permitted, without OIDS
CREATE TABLE uint2_table_test (
   col1 uint2
) WITHOUT OIDS;
DROP TABLE uint2_table_test;

-- Single uint2 column, NULLS prohibited, without OIDS
CREATE TABLE uint2_table_test (
   col1 uint2 NOT NULL
) WITHOUT OIDS;
DROP TABLE uint2_table_test;

-- Multiple uint2 column, NULLS prohibited, without OIDS
CREATE TABLE uint2_table_test (
   col1 uint2 NOT NULL, 
   col2 uint2 NOT NULL,
   col3 uint2 NOT NULL,
   col4 uint2 NOT NULL,
   col5 uint2 NOT NULL
) WITHOUT OIDS;
DROP TABLE uint2_table_test;

-- Multiple uint2 column, mixed NULLS, without OIDS
CREATE TABLE uint2_table_test (
   col1 uint2, 
   col2 uint2 NOT NULL,
   col3 uint2,
   col4 uint2 NOT NULL,
   col5 uint2,
   col6 uint2 NOT NULL
) WITHOUT OIDS;
DROP TABLE uint2_table_test;

-- Mixed columns, mixed NULLS, without OIDS
CREATE TABLE uint2_table_test (
   col1 int2 NOT NULL, 
   col2 uint2 NOT NULL,
   col3 int4,
   col4 uint2 NOT NULL,
   col5 text
) WITHOUT OIDS;
DROP TABLE uint2_table_test;


--
-- uint4 table tests
--

-- Single uint4 column, NULLS permitted, with OIDS.
CREATE TABLE uint4_table_test (
   col1 uint4
) WITH OIDS;
DROP TABLE uint4_table_test;

-- Single uint4 column, NULLS permitted, without OIDS
CREATE TABLE uint4_table_test (
   col1 uint4
) WITHOUT OIDS;
DROP TABLE uint4_table_test;

-- Single uint4 column, NULLS prohibited, without OIDS
CREATE TABLE uint4_table_test (
   col1 uint4 NOT NULL
) WITHOUT OIDS;
DROP TABLE uint4_table_test;

-- Multiple uint4 column, NULLS prohibited, without OIDS
CREATE TABLE uint4_table_test (
   col1 uint4 NOT NULL, 
   col2 uint4 NOT NULL,
   col3 uint4 NOT NULL,
   col4 uint4 NOT NULL,
   col5 uint4 NOT NULL
) WITHOUT OIDS;
DROP TABLE uint4_table_test;

-- Multiple uint4 column, mixed NULLS, without OIDS
CREATE TABLE uint4_table_test (
   col1 uint4, 
   col2 uint4 NOT NULL,
   col3 uint4,
   col4 uint4 NOT NULL,
   col5 uint4,
   col6 uint4 NOT NULL
) WITHOUT OIDS;
DROP TABLE uint4_table_test;

-- Mixed columns, mixed NULLS, without OIDS
CREATE TABLE uint4_table_test (
   col1 int4 NOT NULL, 
   col2 uint4 NOT NULL,
   col3 int2,
   col4 uint4 NOT NULL,
   col5 text
) WITHOUT OIDS;
DROP TABLE uint4_table_test;
