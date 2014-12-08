-- Create table.
CREATE TABLE uint_insert_test (
   col1 uint4 NOT NULL,
   col2 uint2 NOT NULL,
   col3 uint1 NOT NULL
) WITHOUT OIDS;

-- Load data
INSERT INTO uint_insert_test VALUES (4294967295, 65535, 255);
INSERT INTO uint_insert_test VALUES (4294967295::uint4, 65535::uint2, 255::uint1);
INSERT INTO uint_insert_test VALUES (2147483648, 32768, 128);
INSERT INTO uint_insert_test VALUES (2147483648::uint4, 32768::uint2, 128::uint1);
INSERT INTO uint_insert_test VALUES (2147483647, 32767, 127);
INSERT INTO uint_insert_test VALUES (2147483647::uint4, 32767::uint2, 127::uint1);
INSERT INTO uint_insert_test VALUES (65536, 256, 64);
INSERT INTO uint_insert_test VALUES (65536::uint4, 256::uint2, 64::uint1);
INSERT INTO uint_insert_test VALUES (65535, 255, 63);
INSERT INTO uint_insert_test VALUES (65535::uint4, 255::uint2, 63::uint1);
INSERT INTO uint_insert_test VALUES (32768, 128, 32);
INSERT INTO uint_insert_test VALUES (32768::uint4, 128::uint2, 32::uint1);
INSERT INTO uint_insert_test VALUES (32767, 127, 31);
INSERT INTO uint_insert_test VALUES (32767::uint4, 127::uint2, 31::uint1);
INSERT INTO uint_insert_test VALUES (256, 64, 16);
INSERT INTO uint_insert_test VALUES (256::uint4, 64::uint2, 16::uint1);
INSERT INTO uint_insert_test VALUES (255, 63, 15);
INSERT INTO uint_insert_test VALUES (255::uint4, 63::uint2, 15::uint1);
INSERT INTO uint_insert_test VALUES (128, 32, 8);
INSERT INTO uint_insert_test VALUES (128::uint4, 32::uint2, 8::uint1);
INSERT INTO uint_insert_test VALUES (127, 31, 7);
INSERT INTO uint_insert_test VALUES (127::uint4, 31::uint2, 7::uint1);
INSERT INTO uint_insert_test VALUES (64, 16, 4);
INSERT INTO uint_insert_test VALUES (64::uint4, 16::uint2, 4::uint1);
INSERT INTO uint_insert_test VALUES (63, 15, 3);
INSERT INTO uint_insert_test VALUES (63::uint4, 15::uint2, 3::uint1);
INSERT INTO uint_insert_test VALUES (32, 8, 2);
INSERT INTO uint_insert_test VALUES (32::uint4, 8::uint2, 2::uint1);
INSERT INTO uint_insert_test VALUES (31, 7, 1);
INSERT INTO uint_insert_test VALUES (31::uint4, 7::uint2, 1::uint1);
INSERT INTO uint_insert_test VALUES (16, 4, 0);
INSERT INTO uint_insert_test VALUES (16::uint4, 4::uint2, 0::uint1);
INSERT INTO uint_insert_test VALUES (15, 3, 0);
INSERT INTO uint_insert_test VALUES (15::uint4, 3::uint2, 0::uint1);
INSERT INTO uint_insert_test VALUES (8, 2, 0);
INSERT INTO uint_insert_test VALUES (8::uint4, 2::uint2, 0::uint1);
INSERT INTO uint_insert_test VALUES (7, 1, 0);
INSERT INTO uint_insert_test VALUES (7::uint4, 1::uint2, 0::uint1);
INSERT INTO uint_insert_test VALUES (4, 0, 0);
INSERT INTO uint_insert_test VALUES (4::uint4, 0::uint2, 0::uint1);
INSERT INTO uint_insert_test VALUES (3, 0, 0);
INSERT INTO uint_insert_test VALUES (3::uint4, 0::uint2, 0::uint1);
INSERT INTO uint_insert_test VALUES (2, 0, 0);
INSERT INTO uint_insert_test VALUES (2::uint4, 0::uint2, 0::uint1);
INSERT INTO uint_insert_test VALUES (1, 0, 0);
INSERT INTO uint_insert_test VALUES (1::uint4, 0::uint2, 0::uint1);
INSERT INTO uint_insert_test VALUES (0, 0, 0);
INSERT INTO uint_insert_test VALUES (0::uint4, 0::uint2, 0::uint1);

-- Verify data.
SELECT col3, col2, col1
FROM   uint_insert_test
ORDER BY col1;
