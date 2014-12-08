-- uint1 bit operations
SELECT 0::uint1 & 15::uint1;
SELECT 85::uint1 & 15::uint1;
SELECT 170::uint1 & 15::uint1;
SELECT 255::uint1 & 15::uint1;

SELECT 0::uint1 | 85::uint1;
SELECT 85::uint1 | 170::uint1;
SELECT 0::uint1 | 255::uint1;

SELECT 0::uint1 | 85::uint1;
SELECT 85::uint1 | 170::uint1;
SELECT 0::uint1 | 255::uint1;

SELECT 0::uint1 # 85::uint1;
SELECT 85::uint1 # 170::uint1;
SELECT 0::uint1 # 255::uint1;

SELECT ~0::uint1;
SELECT ~170::uint1;
SELECT ~255::uint1;

SELECT 0::uint1 >> 4;
SELECT 170::uint1 >> 4;
SELECT 255::uint1 >> 4;

SELECT 0::uint1 << 4;
SELECT 170::uint1 << 4;
SELECT 255::uint1 << 4;



-- uint2 comparision operations
SELECT 0::uint2 & 255::uint2;
SELECT 21845::uint2 & 255::uint2;
SELECT 43690::uint2 & 255::uint2;
SELECT 65535::uint2 & 255::uint2;

SELECT 0::uint2 | 21845::uint2;
SELECT 21845::uint2 | 43690::uint2;
SELECT 0::uint2 | 65535::uint2;

SELECT 0::uint2 # 21845::uint2;
SELECT 21845::uint2 # 43690::uint2;
SELECT 0::uint2 # 65535::uint2;

SELECT ~0::uint2;
SELECT ~43690::uint2;
SELECT ~65535::uint2;

SELECT 0::uint2 >> 8;
SELECT 43690::uint2 >> 8;
SELECT 65535::uint2 >> 8;

SELECT 0::uint2 << 8;
SELECT 43690::uint2 << 8;
SELECT 65535::uint2 << 8;



-- uint4 comparision operations
SELECT 0::uint4 & 65535::uint4;
SELECT 1431655767::uint4 & 65535::uint4;
SELECT 2863311530::uint4 & 65535::uint4;
SELECT 4294967295::uint4 & 65535::uint4;

SELECT 0::uint4 | 1431655767::uint4;
SELECT 1431655767::uint4 | 2863311530::uint4;
SELECT 0::uint4 | 4294967295::uint4;

SELECT 0::uint4 # 1431655767::uint4;
SELECT 1431655767::uint4 # 2863311530::uint4;
SELECT 0::uint4 # 4294967295::uint4;

SELECT ~0::uint4;
SELECT ~2863311530::uint4;
SELECT ~4294967295::uint4;

SELECT 0::uint4 >> 16;
SELECT 2863311530::uint4 >> 16;
SELECT 4294967295::uint4 >> 16;

SELECT 0::uint4 << 16;
SELECT 2863311530::uint4 << 16;
SELECT 4294967295::uint4 << 16;
