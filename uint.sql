SET client_min_messages=ERROR;

-- BEGIN WORK;

-- Adjust this setting to control where the objects get created.
SET search_path = public;

CREATE FUNCTION int4ltsel(internal, oid, internal, integer)
   RETURNS float8
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION int4gtsel(internal, oid, internal, integer)
   RETURNS float8
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

--
-- uint1 support
--
CREATE TYPE uint1;

CREATE FUNCTION uint1in(cstring)
   RETURNS uint1
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint1out(uint1) RETURNS cstring
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint1recv(internal)
   RETURNS uint1
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint1send(uint1)
   RETURNS bytea
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE uint1 (
   internallength = 1,
   input = uint1in,
   output = uint1out,
   receive = uint1recv,
   send = uint1send,
   alignment = char,
   storage = plain,
   passedbyvalue
);

CREATE FUNCTION uint1eq(uint1, uint1)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
   LEFTARG = uint1,
   RIGHTARG = uint1,
   PROCEDURE = uint1eq,
   COMMUTATOR = =,
   NEGATOR = <>,
   RESTRICT = eqsel,
   JOIN = eqjoinsel,
   HASHES,
   MERGES
);

CREATE FUNCTION uint1ne(uint1, uint1)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <> (
   LEFTARG = uint1,
   RIGHTARG = uint1,
   PROCEDURE = uint1ne,
   COMMUTATOR = <>,
   NEGATOR = =,
   RESTRICT = neqsel,
   JOIN = neqjoinsel
);

CREATE FUNCTION uint1lt(uint1, uint1)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
   LEFTARG = uint1,
   RIGHTARG = uint1,
   PROCEDURE = uint1lt,
   COMMUTATOR = <,
   NEGATOR = >=,
   RESTRICT = scalarltsel,
   JOIN = scalarltjoinsel
);

CREATE FUNCTION uint1le(uint1, uint1)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <= (
   LEFTARG = uint1,
   RIGHTARG = uint1,
   PROCEDURE = uint1le,
   COMMUTATOR = <=,
   NEGATOR = >,
   RESTRICT = scalarltsel,
   JOIN = scalarltjoinsel
);

CREATE FUNCTION uint1gt(uint1, uint1)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR > (
   LEFTARG = uint1,
   RIGHTARG = uint1,
   PROCEDURE = uint1gt,
   COMMUTATOR = >,
   NEGATOR = <=,
   RESTRICT = scalargtsel,
   JOIN = scalargtjoinsel
);

CREATE FUNCTION uint1ge(uint1, uint1)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >= (
   LEFTARG = uint1,
   RIGHTARG = uint1,
   PROCEDURE = uint1ge,
   COMMUTATOR = >=,
   NEGATOR = <,
   RESTRICT = scalargtsel,
   JOIN = scalargtjoinsel
);

CREATE FUNCTION int4uint1eq(int4, uint1)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
   LEFTARG = int4,
   RIGHTARG = uint1,
   PROCEDURE = int4uint1eq,
   COMMUTATOR = =,
   NEGATOR = <>,
   RESTRICT = eqsel,
   JOIN = eqjoinsel,
   HASHES,
   MERGES
);

CREATE FUNCTION uint1and(uint1, uint1)
   RETURNS uint1
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR & (
   LEFTARG = uint1,
   RIGHTARG = uint1,
   PROCEDURE = uint1and,
   COMMUTATOR = &
);

CREATE FUNCTION uint1or(uint1, uint1)
   RETURNS uint1
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR | (
   LEFTARG = uint1,
   RIGHTARG = uint1,
   PROCEDURE = uint1or,
   COMMUTATOR = |
);

CREATE FUNCTION uint1xor(uint1, uint1)
   RETURNS uint1
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR # (
   LEFTARG = uint1,
   RIGHTARG = uint1,
   PROCEDURE = uint1xor,
   COMMUTATOR = #
);

CREATE FUNCTION uint1not(uint1)
   RETURNS uint1
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR ~ (
   RIGHTARG = uint1,
   PROCEDURE = uint1not
);

CREATE FUNCTION uint1shl(uint1, int4)
   RETURNS uint1
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR << (
   LEFTARG = uint1,
   RIGHTARG = int4,
   PROCEDURE = uint1shl
);

CREATE FUNCTION uint1shr(uint1, int4)
   RETURNS uint1
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >> (
   LEFTARG = uint1,
   RIGHTARG = int4,
   PROCEDURE = uint1shr
);


CREATE OPERATOR FAMILY unsigned_integer_ops USING btree;
CREATE OPERATOR FAMILY unsigned_integer_ops USING hash;

CREATE FUNCTION btuint1cmp(uint1, uint1)
   RETURNS integer
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR CLASS uint1_ops
   DEFAULT FOR TYPE uint1 USING BTREE FAMILY unsigned_integer_ops AS
      OPERATOR 1 <,
      OPERATOR 2 <=,
      OPERATOR 3 =,
      OPERATOR 4 >=,
      OPERATOR 5 >,
      FUNCTION 1 btuint1cmp(uint1, uint1);

CREATE FUNCTION hashuint1(uint1)
   RETURNS integer
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR CLASS uint1_ops
   DEFAULT FOR TYPE uint1 USING HASH FAMILY unsigned_integer_ops AS
      OPERATOR 1 =,
      FUNCTION 1 hashuint1(uint1);

CREATE FUNCTION int4touint1(int4)
   RETURNS uint1
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint1toint4(uint1)
   RETURNS int4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (int4 AS uint1) WITH FUNCTION int4touint1(int4) AS ASSIGNMENT;
CREATE CAST (uint1 AS int4) WITH FUNCTION uint1toint4(uint1) AS IMPLICIT;


--
-- uint2 support
--
CREATE TYPE uint2;

CREATE FUNCTION uint2in(cstring)
   RETURNS uint2
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint2out(uint2) RETURNS cstring
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint2recv(internal)
   RETURNS uint2
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint2send(uint2)
   RETURNS bytea
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE uint2 (
   internallength = 2,
   input = uint2in,
   output = uint2out,
   receive = uint2recv,
   send = uint2send,
   alignment = int2,
   storage = plain,
   passedbyvalue
);

CREATE FUNCTION uint2eq(uint2, uint2)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
   LEFTARG = uint2,
   RIGHTARG = uint2,
   PROCEDURE = uint2eq,
   COMMUTATOR = =,
   NEGATOR = <>,
   RESTRICT = eqsel,
   JOIN = eqjoinsel,
   HASHES,
   MERGES
);

CREATE FUNCTION uint2ne(uint2, uint2)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <> (
   LEFTARG = uint2,
   RIGHTARG = uint2,
   PROCEDURE = uint2ne,
   COMMUTATOR = <>,
   NEGATOR = =,
   RESTRICT = neqsel,
   JOIN = neqjoinsel
);

CREATE FUNCTION uint2lt(uint2, uint2)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
   LEFTARG = uint2,
   RIGHTARG = uint2,
   PROCEDURE = uint2lt,
   COMMUTATOR = <,
   NEGATOR = >=,
   RESTRICT = scalarltsel,
   JOIN = scalarltjoinsel
);

CREATE FUNCTION uint2le(uint2, uint2)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <= (
   LEFTARG = uint2,
   RIGHTARG = uint2,
   PROCEDURE = uint2le,
   COMMUTATOR = <=,
   NEGATOR = >,
   RESTRICT = scalarltsel,
   JOIN = scalarltjoinsel
);

CREATE FUNCTION uint2gt(uint2, uint2)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR > (
   LEFTARG = uint2,
   RIGHTARG = uint2,
   PROCEDURE = uint2gt,
   COMMUTATOR = >,
   NEGATOR = <=,
   RESTRICT = scalargtsel,
   JOIN = scalargtjoinsel
);

CREATE FUNCTION uint2ge(uint2, uint2)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >= (
   LEFTARG = uint2,
   RIGHTARG = uint2,
   PROCEDURE = uint2ge,
   COMMUTATOR = >=,
   NEGATOR = <,
   RESTRICT = scalargtsel,
   JOIN = scalargtjoinsel
);

CREATE FUNCTION int4uint2eq(int4, uint2)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
   LEFTARG = int4,
   RIGHTARG = uint2,
   PROCEDURE = int4uint2eq,
   COMMUTATOR = =,
   NEGATOR = <>,
   RESTRICT = eqsel,
   JOIN = eqjoinsel,
   HASHES,
   MERGES
);

CREATE FUNCTION uint2and(uint2, uint2)
   RETURNS uint2
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR & (
   LEFTARG = uint2,
   RIGHTARG = uint2,
   PROCEDURE = uint2and,
   COMMUTATOR = &
);

CREATE FUNCTION uint2or(uint2, uint2)
   RETURNS uint2
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR | (
   LEFTARG = uint2,
   RIGHTARG = uint2,
   PROCEDURE = uint2or,
   COMMUTATOR = |
);

CREATE FUNCTION uint2xor(uint2, uint2)
   RETURNS uint2
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR # (
   LEFTARG = uint2,
   RIGHTARG = uint2,
   PROCEDURE = uint2xor,
   COMMUTATOR = #
);

CREATE FUNCTION uint2not(uint2)
   RETURNS uint2
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR ~ (
   RIGHTARG = uint2,
   PROCEDURE = uint2not
);

CREATE FUNCTION uint2shl(uint2, int4)
   RETURNS uint2
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR << (
   LEFTARG = uint2,
   RIGHTARG = int4,
   PROCEDURE = uint2shl
);

CREATE FUNCTION uint2shr(uint2, int4)
   RETURNS uint2
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >> (
   LEFTARG = uint2,
   RIGHTARG = int4,
   PROCEDURE = uint2shr
);

CREATE FUNCTION btuint2cmp(uint2, uint2)
   RETURNS integer
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR CLASS uint2_ops
   DEFAULT FOR TYPE uint2 USING BTREE FAMILY unsigned_integer_ops AS
      OPERATOR 1 <,
      OPERATOR 2 <=,
      OPERATOR 3 =,
      OPERATOR 4 >=,
      OPERATOR 5 >,
      FUNCTION 1 btuint2cmp(uint2, uint2);

CREATE FUNCTION hashuint2(uint2)
   RETURNS integer
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR CLASS uint2_ops
   DEFAULT FOR TYPE uint2 USING HASH FAMILY unsigned_integer_ops AS
      OPERATOR 1 =,
      FUNCTION 1 hashuint2(uint2);

CREATE FUNCTION int4touint2(int4)
   RETURNS uint2
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint2toint4(uint2)
   RETURNS int4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (int4 AS uint2) WITH FUNCTION int4touint2(int4) AS ASSIGNMENT;
CREATE CAST (uint2 AS int4) WITH FUNCTION uint2toint4(uint2) AS IMPLICIT;

--
-- uint4 support
-- 
CREATE TYPE uint4;

CREATE FUNCTION uint4in(cstring)
   RETURNS uint4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint4out(uint4) RETURNS cstring
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint4recv(internal)
   RETURNS uint4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint4send(uint4)
   RETURNS bytea
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE TYPE uint4 (
   internallength = 4,
   input = uint4in,
   output = uint4out,
   receive = uint4recv,
   send = uint4send,
   alignment = int4,
   storage = plain,
   passedbyvalue
);

--
-- UINT4 OP UINT4 OPERATORS
--
CREATE FUNCTION uint4eq(uint4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
   LEFTARG = uint4,
   RIGHTARG = uint4,
   PROCEDURE = uint4eq,
   COMMUTATOR = =,
   NEGATOR = <>,
   RESTRICT = eqsel,
   JOIN = eqjoinsel,
   HASHES,
   MERGES
);
    
CREATE FUNCTION uint4ne(uint4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <> (
   LEFTARG = uint4, 
   RIGHTARG = uint4,
   PROCEDURE = uint4ne, 
   COMMUTATOR = <>,
   NEGATOR = =,
   RESTRICT = neqsel,
   JOIN = neqjoinsel
);

CREATE FUNCTION uint4lt(uint4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
   LEFTARG = uint4,
   RIGHTARG = uint4,
   PROCEDURE = uint4lt,
   COMMUTATOR = <,
   NEGATOR = >=,
   RESTRICT = scalarltsel,
   JOIN = scalarltjoinsel
);

CREATE FUNCTION uint4le(uint4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <= (
   LEFTARG = uint4,
   RIGHTARG = uint4,
   PROCEDURE = uint4le,
   COMMUTATOR = <=,
   NEGATOR = >,
   RESTRICT = scalarltsel,
   JOIN = scalarltjoinsel
);

CREATE FUNCTION uint4gt(uint4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR > (
   LEFTARG = uint4,
   RIGHTARG = uint4,
   PROCEDURE = uint4gt,
   COMMUTATOR = >,
   NEGATOR = <=,
   RESTRICT = scalargtsel,
   JOIN = scalargtjoinsel
);

CREATE FUNCTION uint4ge(uint4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >= (
   LEFTARG = uint4,
   RIGHTARG = uint4,
   PROCEDURE = uint4ge,
   COMMUTATOR = >=,
   NEGATOR = <,
   RESTRICT = scalargtsel,
   JOIN = scalargtjoinsel
);

CREATE FUNCTION uint4and(uint4, uint4)
   RETURNS uint4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR & (
   LEFTARG = uint4,
   RIGHTARG = uint4,
   PROCEDURE = uint4and,
   COMMUTATOR = &
);

CREATE FUNCTION uint4or(uint4, uint4)
   RETURNS uint4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR | (
   LEFTARG = uint4,
   RIGHTARG = uint4,
   PROCEDURE = uint4or,
   COMMUTATOR = |
);

CREATE FUNCTION uint4xor(uint4, uint4)
   RETURNS uint4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR # (
   LEFTARG = uint4,
   RIGHTARG = uint4,
   PROCEDURE = uint4xor,
   COMMUTATOR = #
);

CREATE FUNCTION uint4not(uint4)
   RETURNS uint4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR ~ (
   RIGHTARG = uint4,
   PROCEDURE = uint4not
);

CREATE FUNCTION uint4shl(uint4, int4)
   RETURNS uint4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR << (
   LEFTARG = uint4,
   RIGHTARG = int4,
   PROCEDURE = uint4shl
);

CREATE FUNCTION uint4shr(uint4, int4)
   RETURNS uint4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >> (
   LEFTARG = uint4,
   RIGHTARG = int4,
   PROCEDURE = uint4shr
);

CREATE FUNCTION btuint4cmp(uint4, uint4)
   RETURNS integer
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR CLASS uint4_ops
   DEFAULT FOR TYPE uint4 USING BTREE FAMILY unsigned_integer_ops AS
      OPERATOR 1 <,
      OPERATOR 2 <=,
      OPERATOR 3 =,
      OPERATOR 4 >=,
      OPERATOR 5 >,
      FUNCTION 1 btuint4cmp(uint4, uint4);

--
-- INT4 OP UINT OPERATORS
--
CREATE FUNCTION int4uint4eq(int4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
   LEFTARG = int4,
   RIGHTARG = uint4,
   PROCEDURE = int4uint4eq,
   COMMUTATOR = =,
   NEGATOR = <>,
   RESTRICT = eqsel,
   JOIN = eqjoinsel,
   HASHES,
   MERGES
);
    
CREATE FUNCTION int4uint4ne(int4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <> (
   LEFTARG = int4, 
   RIGHTARG = uint4,
   PROCEDURE = int4uint4ne, 
   COMMUTATOR = <>,
   NEGATOR = =,
   RESTRICT = neqsel,
   JOIN = neqjoinsel
);

CREATE FUNCTION int4uint4lt(int4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
   LEFTARG = int4,
   RIGHTARG = uint4,
   PROCEDURE = int4uint4lt,
   COMMUTATOR = <,
   NEGATOR = >=,
   RESTRICT = int4ltsel,
   JOIN = scalarltjoinsel
);

CREATE FUNCTION int4uint4le(int4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <= (
   LEFTARG = int4,
   RIGHTARG = uint4,
   PROCEDURE = int4uint4le,
   COMMUTATOR = <=,
   NEGATOR = >,
   RESTRICT = int4ltsel,
   JOIN = scalarltjoinsel
);

CREATE FUNCTION int4uint4gt(int4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR > (
   LEFTARG = int4,
   RIGHTARG = uint4,
   PROCEDURE = int4uint4gt,
   COMMUTATOR = >,
   NEGATOR = <=,
   RESTRICT = int4gtsel,
   JOIN = scalargtjoinsel
);

CREATE FUNCTION int4uint4ge(int4, uint4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >= (
   LEFTARG = int4,
   RIGHTARG = uint4,
   PROCEDURE = int4uint4ge,
   COMMUTATOR = >=,
   NEGATOR = <,
   RESTRICT = int4gtsel,
   JOIN = scalargtjoinsel
);


--
-- UINT4 OP INT4 OPERATORS
--
CREATE FUNCTION uint4int4eq(uint4, int4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR = (
   LEFTARG = uint4,
   RIGHTARG = int4,
   PROCEDURE = uint4int4eq,
   COMMUTATOR = =,
   NEGATOR = <>,
   RESTRICT = eqsel,
   JOIN = eqjoinsel,
   HASHES,
   MERGES
);
    
CREATE FUNCTION uint4int4ne(uint4, int4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <> (
   LEFTARG = uint4, 
   RIGHTARG = int4,
   PROCEDURE = uint4int4ne, 
   COMMUTATOR = <>,
   NEGATOR = =,
   RESTRICT = neqsel,
   JOIN = neqjoinsel
);

CREATE FUNCTION uint4int4lt(uint4, int4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR < (
   LEFTARG = uint4,
   RIGHTARG = int4,
   PROCEDURE = uint4int4lt,
   COMMUTATOR = <,
   NEGATOR = >=,
   RESTRICT = int4ltsel,
   JOIN = scalarltjoinsel
);

CREATE FUNCTION uint4int4le(uint4, int4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR <= (
   LEFTARG = uint4,
   RIGHTARG = int4,
   PROCEDURE = uint4int4le,
   COMMUTATOR = <=,
   NEGATOR = >,
   RESTRICT = int4ltsel,
   JOIN = scalarltjoinsel
);

CREATE FUNCTION uint4int4gt(uint4, int4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR > (
   LEFTARG = uint4,
   RIGHTARG = int4,
   PROCEDURE = uint4int4gt,
   COMMUTATOR = >,
   NEGATOR = <=,
   RESTRICT = int4gtsel,
   JOIN = scalargtjoinsel
);

CREATE FUNCTION uint4int4ge(uint4, int4)
   RETURNS boolean
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR >= (
   LEFTARG = uint4,
   RIGHTARG = int4,
   PROCEDURE = uint4int4ge,
   COMMUTATOR = >=,
   NEGATOR = <,
   RESTRICT = int4gtsel,
   JOIN = scalargtjoinsel
);

CREATE FUNCTION hashuint4(uint4)
   RETURNS integer
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION hashuint4_from_int4(int4)
   RETURNS integer
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE OPERATOR CLASS uint4_ops
   DEFAULT FOR TYPE uint4 USING HASH FAMILY unsigned_integer_ops AS
      OPERATOR 1 =,
      FUNCTION 1 hashuint4(uint4);

ALTER OPERATOR FAMILY unsigned_integer_ops USING HASH ADD
   OPERATOR 1 = (int4, uint4),
   FUNCTION 1 hashuint4_from_int4(int4);

CREATE FUNCTION uint4toint4(uint4)
   RETURNS int4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION int4touint4(int4)
   RETURNS uint4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION uint4toint8(uint4)
   RETURNS int8
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE FUNCTION int8touint4(int8)
   RETURNS uint4
   AS '$libdir/uint'
   LANGUAGE C IMMUTABLE STRICT;

CREATE CAST (uint4 AS int4) WITH FUNCTION uint4toint4(uint4);
CREATE CAST (int4 AS uint4) WITH FUNCTION int4touint4(int4) AS ASSIGNMENT;
CREATE CAST (uint4 AS int8) WITH FUNCTION uint4toint8(uint4) AS IMPLICIT;
CREATE CAST (int8 AS uint4) WITH FUNCTION int8touint4(int8) AS ASSIGNMENT;

-- COMMIT;
