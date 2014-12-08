-- Valid uint1 casts.
SELECT 0::uint1;
SELECT 1::uint1;
SELECT 127::uint1;
SELECT 128::uint1;
SELECT 254::uint1;
SELECT 255::uint1;

-- Invalid uint1 casts (negative numbers)
SELECT -1::uint1;
SELECT -127::uint1;
SELECT -128::uint1;
SELECT -255::uint1;
SELECT -32767::uint1;
SELECT -32768::uint1;
SELECT -65535::uint1;
SELECT -65536::uint1;
SELECT -2147483647::uint1;
SELECT -2147483648::uint1;
SELECT -4294967295::uint1;
SELECT -4294967296::uint1;

-- Invalid uint1 casts (out of range)
SELECT 256::uint1;
SELECT 32767::uint1;
SELECT 32768::uint1;
SELECT 65535::uint1;
SELECT 65536::uint1;
SELECT 2147483647::uint1;
SELECT 2147483648::uint1;
SELECT 4294967295::uint1;
SELECT 4294967296::uint1;


-- Valid uint2 casts.
SELECT 0::uint2;
SELECT 1::uint2;
SELECT 127::uint2;
SELECT 128::uint2;
SELECT 254::uint2;
SELECT 255::uint2;
SELECT 32767::uint2;
SELECT 32768::uint2;
SELECT 65535::uint2;

-- Invalid uint2 casts (negative numbers)
SELECT -1::uint2;
SELECT -127::uint2;
SELECT -128::uint2;
SELECT -255::uint2;
SELECT -32767::uint2;
SELECT -32768::uint2;
SELECT -65535::uint2;
SELECT -65536::uint2;
SELECT -2147483647::uint2;
SELECT -2147483648::uint2;
SELECT -4294967295::uint2;
SELECT -4294967296::uint2;

-- Invalid uint2 casts (out of range)
SELECT 2147483647::uint2;
SELECT 2147483648::uint2;
SELECT 4294967295::uint2;
SELECT 4294967296::uint2;


-- Valid uint4 casts.
SELECT 0::uint4;
SELECT 1::uint4;
SELECT 127::uint4;
SELECT 128::uint4;
SELECT 254::uint4;
SELECT 255::uint4;
SELECT 32767::uint4;
SELECT 32768::uint4;
SELECT 65535::uint4;
SELECT 65536::uint4;
SELECT 2147483647::uint4;
SELECT 2147483648::uint4;
SELECT 4294967295::uint4;

-- Invalid uint4 casts (negative numbers)
SELECT -1::uint4;
SELECT -127::uint4;
SELECT -128::uint4;
SELECT -255::uint4;
SELECT -32767::uint4;
SELECT -32768::uint4;
SELECT -65535::uint4;
SELECT -65536::uint4;
SELECT -2147483647::uint4;
SELECT -2147483648::uint4;
SELECT -4294967295::uint4;
SELECT -4294967296::uint4;

-- Invalid uint4 casts (out of range)
SELECT 4294967296::uint4;
