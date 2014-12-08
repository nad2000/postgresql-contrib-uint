-- uint1 comparision operations
SELECT btuint1cmp(10::uint1, 11::uint1);
SELECT btuint1cmp(10::uint1, 10::uint1);
SELECT btuint1cmp(11::uint1, 10::uint1);

SELECT 10::uint1 = 10::uint1;
SELECT 10::uint1 = 100::uint1;

SELECT 70::uint1 <> 80::uint1;
SELECT 50::uint1 <> 50::uint1;

SELECT 70::uint1 < 80::uint1;
SELECT 70::uint1 < 70::uint1;
SELECT 70::uint1 < 60::uint1;

SELECT 70::uint1 <= 80::uint1;
SELECT 70::uint1 <= 70::uint1;
SELECT 70::uint1 <= 60::uint1;

SELECT 80::uint1 > 70::uint1;
SELECT 70::uint1 > 70::uint1;
SELECT 60::uint1 > 70::uint1;

SELECT 80::uint1 >= 70::uint1;
SELECT 70::uint1 >= 70::uint1;
SELECT 60::uint1 >= 70::uint1;


-- uint2 comparision operations
SELECT btuint2cmp(10::uint2, 11::uint2);
SELECT btuint2cmp(10::uint2, 10::uint2);
SELECT btuint2cmp(11::uint2, 10::uint2);

SELECT 10::uint2 = 10::uint2;
SELECT 10::uint2 = 100::uint2;

SELECT 70::uint2 <> 80::uint2;
SELECT 50::uint2 <> 50::uint2;

SELECT 70::uint2 < 80::uint2;
SELECT 70::uint2 < 70::uint2;
SELECT 80::uint2 < 60::uint2;

SELECT 70::uint2 <= 80::uint2;
SELECT 70::uint2 <= 70::uint2;
SELECT 70::uint2 <= 60::uint2;

SELECT 80::uint2 > 70::uint2;
SELECT 70::uint2 > 70::uint2;
SELECT 60::uint2 > 70::uint2;

SELECT 80::uint2 >= 70::uint2;
SELECT 70::uint2 >= 70::uint2;
SELECT 60::uint2 >= 70::uint2;


-- uint4 comparision operations
SELECT btuint2cmp(10::uint4, 11::uint4);
SELECT btuint2cmp(10::uint4, 10::uint4);
SELECT btuint2cmp(11::uint4, 10::uint4);

SELECT 10::uint4 = 10::uint4;
SELECT 10::uint4 = 100::uint4;

SELECT 70::uint4 <> 80::uint4;
SELECT 50::uint4 <> 50::uint4;

SELECT 70::uint4 < 80::uint4;
SELECT 70::uint4 < 70::uint4;
SELECT 80::uint4 < 60::uint4;

SELECT 70::uint4 <= 80::uint4;
SELECT 70::uint4 <= 70::uint4;
SELECT 70::uint4 <= 60::uint4;

SELECT 80::uint4 > 70::uint4;
SELECT 70::uint4 > 70::uint4;
SELECT 60::uint4 > 70::uint4;

SELECT 80::uint4 >= 70::uint4;
SELECT 70::uint4 >= 70::uint4;
SELECT 60::uint4 >= 70::uint4;


-- int4 - uint4 comparision operations
SELECT 10::int4 = 10::uint4;
SELECT 10::int4 = 100::uint4;
SELECT -1::int4 = 100::uint4;

SELECT 70::int4 <> 80::uint4;
SELECT 50::int4 <> 50::uint4;
SELECT -1::int4 <> 50::uint4;

SELECT 70::int4 < 80::uint4;
SELECT 70::int4 < 70::uint4;
SELECT 70::int4 < 60::uint4;
SELECT -1::int4 < 50::uint4;

SELECT 70::int4 <= 80::uint4;
SELECT 70::int4 <= 70::uint4;
SELECT 70::int4 <= 60::uint4;
SELECT -1::int4 <= 50::uint4;

SELECT 80::uint4 > 70::uint4;
SELECT 70::uint4 > 70::uint4;
SELECT 60::uint4 > 70::uint4;
SELECT -1::uint4 > 70::uint4;

SELECT 80::uint4 >= 70::uint4;
SELECT 70::uint4 >= 70::uint4;
SELECT 60::uint4 >= 70::uint4;
SELECT -1::uint4 >= 70::uint4;


-- uint4 - int4 comparision operations
SELECT 10::uint4 = 10::int4;
SELECT 10::uint4 = 100::int4;
SELECT 100::uint4 = -1::int4;

SELECT 70::uint4 <> 80::int4;
SELECT 50::uint4 <> 50::int4;
SELECT 50::uint4 <> -1::int4;

SELECT 70::uint4 < 80::int4;
SELECT 70::uint4 < 70::int4;
SELECT 70::uint4 < 60::int4;
SELECT 60::uint4 < -1::int4;

SELECT 70::uint4 <= 80::int4;
SELECT 70::uint4 <= 70::int4;
SELECT 70::uint4 <= 60::int4;
SELECT 60::uint4 <= -1::int4;

SELECT 80::uint4 > 70::int4;
SELECT 70::uint4 > 70::int4;
SELECT 60::uint4 > 70::int4;
SELECT 50::uint4 > -1::int4;

SELECT 80::uint4 >= 70::int4;
SELECT 70::uint4 >= 70::int4;
SELECT 60::uint4 >= 70::int4;
SELECT 50::uint4 >= -1::int4;
