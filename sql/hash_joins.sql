SET enable_bitmapscan = OFF;
SET enable_mergejoin = OFF;
SET enable_nestloop = OFF;
SET enable_sort = OFF;
SET enable_tidscan = OFF;

-- uint1 hash joins
CREATE TABLE t1_uint1 (f1 uint1 primary key);
INSERT INTO t1_uint1 SELECT generate_series(1, 255);
ANALYZE t1_uint1;
--EXPLAIN SELECT * FROM t1_uint1, generate_series(1, 10) AS foo WHERE t1_uint1.f1 = foo::uint1;
SELECT * FROM t1_uint1, generate_series(1, 10) AS foo WHERE t1_uint1.f1 = foo::uint1;

--EXPLAIN SELECT * FROM t1_uint1, generate_series(1, 10) AS foo WHERE foo::uint1 = t1_uint1.f1;
SELECT * FROM t1_uint1, generate_series(1, 10) AS foo WHERE foo::uint1 = t1_uint1.f1;


-- uint2 hash joins
CREATE TABLE t1_uint2 (f1 uint2 primary key);
INSERT INTO t1_uint2 SELECT generate_series(1, 255);
ANALYZE t1_uint2;
--EXPLAIN SELECT * FROM t1_uint2, generate_series(1, 10) AS foo WHERE t1_uint2.f1 = foo::uint2;
SELECT * FROM t1_uint2, generate_series(1, 10) AS foo WHERE t1_uint2.f1 = foo::uint2;

--EXPLAIN SELECT * FROM t1_uint2, generate_series(1, 10) AS foo WHERE foo::uint2 = t1_uint2.f1;
SELECT * FROM t1_uint2, generate_series(1, 10) AS foo WHERE foo::uint2 = t1_uint2.f1;


-- uint4 hash joins
CREATE TABLE t1_uint4 (f1 uint4 primary key);
INSERT INTO t1_uint4 SELECT generate_series(1, 255);
ANALYZE t1_uint4;
--EXPLAIN SELECT * FROM t1_uint4, generate_series(1, 10) AS foo WHERE t1_uint4.f1 = foo;
SELECT * FROM t1_uint4, generate_series(1, 10) AS foo WHERE t1_uint4.f1 = foo;

--EXPLAIN SELECT * FROM t1_uint4, generate_series(1, 10) AS foo WHERE foo = t1_uint4.f1;
SELECT * FROM t1_uint4, generate_series(1, 10) AS foo WHERE foo = t1_uint4.f1;
