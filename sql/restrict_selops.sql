-- uint1 op int4 tests
SELECT * FROM t1_uint1 WHERE f1 > 250;
SELECT * FROM t1_uint1 WHERE f1 < 5;

-- int4 op uint1 tests
SELECT * FROM t1_uint1 WHERE 250 < f1;
SELECT * FROM t1_uint1 WHERE 5 > f1;

-- uint2 op int4 tests
SELECT * FROM t1_uint2 WHERE f1 > 250;
SELECT * FROM t1_uint2 WHERE f1 < 5;

-- int4 op uint2 tests
SELECT * FROM t1_uint2 WHERE 250 < f1;
SELECT * FROM t1_uint2 WHERE 5 > f1;

-- uint4 op int4 tests
SELECT * FROM t1_uint4 WHERE f1 > 250;
SELECT * FROM t1_uint4 WHERE f1 < 5;

-- int4 op uint4 tests
SELECT * FROM t1_uint4 WHERE 250 < f1;
SELECT * FROM t1_uint4 WHERE 5 > f1;

-- Test get_restriction_variable
CREATE TABLE t2_uint4 (col1 uint4 PRIMARY KEY, col2 uint4 NOT NULL);
INSERT INTO t2_uint4 SELECT generate_series(1, 10), generate_series(10, 1, -1);
SELECT * FROM t2_uint4 WHERE col1 > col2;
SELECT * FROM t2_uint4 WHERE col1 < col2;

-- Test volatile function
CREATE FUNCTION blah() RETURNS integer AS $$ UPDATE t2_uint4 SET col1 = 5 WHERE col1 = 5; SELECT 5; $$ LANGUAGE SQL VOLATILE;
SELECT * FROM t2_uint4 WHERE col1 <= blah();
SELECT * FROM t2_uint4 WHERE col1 >= blah();

-- Test stable function
DROP FUNCTION blah();
CREATE FUNCTION blah() RETURNS integer AS $$ SELECT col1::int4 FROM t2_uint4 WHERE col1 = 5; $$ LANGUAGE SQL STABLE;
SELECT * FROM t2_uint4 WHERE col1 < blah();
SELECT * FROM t2_uint4 WHERE col1 > blah();

-- Test immutable function
DROP FUNCTION blah();
CREATE FUNCTION blah() RETURNS integer AS $$ SELECT 5; $$ LANGUAGE SQL IMMUTABLE;
SELECT * FROM t2_uint4 WHERE col1 <= blah();
SELECT * FROM t2_uint4 WHERE col1 > blah();
