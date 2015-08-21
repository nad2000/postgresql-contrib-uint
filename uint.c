#include "postgres.h"
#include "fmgr.h"
#include "libpq/pqformat.h"
#include "access/hash.h"

#include "utils/selfuncs.h"
#include "utils/syscache.h"
#include "utils/lsyscache.h"
#include "catalog/pg_statistic.h"

#if PG_VERSION_NUM >= 90300
  #include "access/htup_details.h"
#endif

#include <stdlib.h>
#include <limits.h>
#include <math.h>

#define PG_GETARG_UINT8(n) DatumGetUInt8(PG_GETARG_DATUM(n))
#define PG_RETURN_UINT8(x) return UInt8GetDatum(x)
#define PG_RETURN_UINT16(x) return UInt16GetDatum(x)

#if defined(__GNUC__) && (__GNUC__ > 2 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 95))
#   define likely(x) __builtin_expect((x),1)
#   define unlikely(x) __builtin_expect((x),0)
#else
#   define likely(x) (x)
#   define unlikely(x) (x)
#endif

#ifdef PG_MODULE_MAGIC
PG_MODULE_MAGIC;
#endif

/*
 *  ==================
 *  UINT1 DECLARATIONS
 *  ==================
 */
Datum uint1in(PG_FUNCTION_ARGS);
Datum uint1out(PG_FUNCTION_ARGS);
Datum uint1recv(PG_FUNCTION_ARGS);
Datum uint1send(PG_FUNCTION_ARGS);
Datum btuint1cmp(PG_FUNCTION_ARGS);
Datum uint1eq(PG_FUNCTION_ARGS);
Datum uint1ne(PG_FUNCTION_ARGS);
Datum uint1lt(PG_FUNCTION_ARGS);
Datum uint1le(PG_FUNCTION_ARGS);
Datum uint1gt(PG_FUNCTION_ARGS);
Datum uint1ge(PG_FUNCTION_ARGS);
Datum int4uint1eq(PG_FUNCTION_ARGS);
Datum uint1and(PG_FUNCTION_ARGS);
Datum uint1or(PG_FUNCTION_ARGS);
Datum uint1xor(PG_FUNCTION_ARGS);
Datum uint1not(PG_FUNCTION_ARGS);
Datum uint1shl(PG_FUNCTION_ARGS);
Datum uint1shr(PG_FUNCTION_ARGS);
Datum hashuint1(PG_FUNCTION_ARGS);
Datum int4touint1(PG_FUNCTION_ARGS);
Datum uint1toint4(PG_FUNCTION_ARGS);


/*
 *  =====================
 *  UINT1 PUBLIC ROUTINES
 *  =====================
 */
PG_FUNCTION_INFO_V1(uint1in);
PG_FUNCTION_INFO_V1(uint1out);
PG_FUNCTION_INFO_V1(uint1recv);
PG_FUNCTION_INFO_V1(uint1send);

/**
 * Convert a c-string into a value suitable for the uint1 datatype.
 *
 * @param  s The c-string representation.
 * @return   The Datum containing the converted uint1 value.
 */
Datum
uint1in(PG_FUNCTION_ARGS)
{
   char *badp;
   unsigned long ul;
   char *s = PG_GETARG_CSTRING(0);

   /* Check for NULL pointer. */
   if(unlikely(s == NULL)) {
      elog(ERROR, "NULL pointer");
   }

   /*
    * Some versions of strtoul treat the empty string as an error, but some
    * seem not to.  Make an explicit test to be sure we catch it.
    */
   if(unlikely(*s == 0)) {
      ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
              errmsg("invalid input syntax for unsigned integer: \"%s\"", s)));
   }

   errno = 0;
   ul = strtoul(s, &badp, 10);

   /* We made no progress parsing the string, so bail out */
   if(unlikely(s == badp)) {
      ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
              errmsg("invalid input syntax for unsigned integer: \"%s\"", s)));
   }

   /* Verify value is valid in the uint1 datatype range. */
   if(unlikely(errno == ERANGE || ul < 0 || ul > UCHAR_MAX)) {
      ereport(ERROR, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
              errmsg("value \"%s\" is out of range for unsigned 8-bit integer", s)));
   }

   /*
    * Skip any trailing whitespace; if anything but whitespace remains before
    * the terminating character, bail out
    */
   while(*badp && isspace((unsigned char)*badp)) {
      badp++;
   }

   /* Verify the c-string is empty. */
   if(unlikely(*badp)) {
      ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
              errmsg("invalid input syntax for unsigned integer: \"%s\"", s)));
   }

   PG_RETURN_UINT8((unsigned char)ul);
}

/**
 * Convert the uint1 value into a c-string.
 *
 * @param  num The uint1 value.
 * @return     The Datum containing the converted uint1 value.
 */
Datum
uint1out(PG_FUNCTION_ARGS)
{
   uint8 num = PG_GETARG_UINT8(0);
   char *str = (char *) palloc(5);

   snprintf(str, 5, "%u", num);
   PG_RETURN_CSTRING(str);
}

/**
 * Convert the binary represented value	into a value suitable for
 * the uint1 datatype.
 *
 * @param  buf The uint1 value.
 * @return     The Datum containing the converted uint1 value.
 */
Datum
uint1recv(PG_FUNCTION_ARGS)
{
   StringInfo buf = (StringInfo) PG_GETARG_POINTER(0);
   PG_RETURN_UINT8((uint8)pq_getmsgint(buf, sizeof(uint8)));
}

/**
 * Convert the uint1 datatype into its binary representation.
 *
 * @param  num The uint1 value.
 * @return     The Datum containing the converted uint1 value.
 */
Datum
uint1send(PG_FUNCTION_ARGS)
{
   uint8 num = PG_GETARG_UINT8(0);
   StringInfoData buf;

   pq_begintypsend(&buf);
   pq_sendint(&buf, num, sizeof(uint8));
   PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}

/*
 *  ==================================
 *  UINT1 COMPARISON OPERATOR ROUTINES
 *  ==================================
 */

PG_FUNCTION_INFO_V1(btuint1cmp);

/**
 * Comparison function used by the B-tree index for sorting purposes.
 *
 * It is safe to directly subtract the values in this function because
 * the return value can not overflow a signed 32-bit integer.
 *
 * @param  a The first uint1 value.
 * @param  b The second uint1 value.
 * @return The Datum containing the comparison value.
 *         The comparison value is:
 *         < 0 if a < b
 *           0 if a == b
 *         > 0 if a > b
 */
Datum
btuint1cmp(PG_FUNCTION_ARGS)
{
   uint8 a = PG_GETARG_UINT8(0);
   uint8 b = PG_GETARG_UINT8(1);

   PG_RETURN_INT32((int32)a - (int32)b);
}

PG_FUNCTION_INFO_V1(uint1eq);
PG_FUNCTION_INFO_V1(uint1ne);
PG_FUNCTION_INFO_V1(uint1lt);
PG_FUNCTION_INFO_V1(uint1le);
PG_FUNCTION_INFO_V1(uint1gt);
PG_FUNCTION_INFO_V1(uint1ge);
PG_FUNCTION_INFO_V1(uint1int4eq);

/**
 * This function implements the "equal" (=) operator for the uint1 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint1 datatypes.
 *
 * @param  arg1 The first uint1 value.
 * @param  arg2 The second uint1 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 == arg2
 *         false if arg1 != arg2
 */
Datum
uint1eq(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);
   uint8 arg2 = PG_GETARG_UINT8(1);

   PG_RETURN_BOOL(arg1 == arg2);
}

/**
 * This function implements the "not equal" (<>) operator for the uint1 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint1 datatypes.
 *
 * @param  arg1 The first uint1 value.
 * @param  arg2 The second uint1 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 != arg2
 *         false if arg1 == arg2
 */
Datum
uint1ne(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);
   uint8 arg2 = PG_GETARG_UINT8(1);

   PG_RETURN_BOOL(arg1 != arg2);
}

/**
 * This function implements the "less than" (<) operator for the uint1 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint1 datatypes.
 *
 * @param  arg1 The first uint1 value.
 * @param  arg2 The second uint1 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 < arg2
 *         false if arg1 >= arg2
 */
Datum
uint1lt(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);
   uint8 arg2 = PG_GETARG_UINT8(1);

   PG_RETURN_BOOL(arg1 < arg2);
}

/**
 * This function implements the "less than or equal" (<=) operator for the uint1 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint1 datatypes.
 *
 * @param  arg1 The first uint1 value.
 * @param  arg2 The second uint1 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 <= arg2
 *         false if arg1 > arg2
 */
Datum
uint1le(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);
   uint8 arg2 = PG_GETARG_UINT8(1);

   PG_RETURN_BOOL(arg1 <= arg2);
}

/**
 * This function implements the "greater than" (>) operator for the uint1 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint1 datatypes.
 *
 * @param  arg1 The first uint1 value.
 * @param  arg2 The second uint1 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 > arg2
 *         false if arg1 <= arg2
 */
Datum
uint1gt(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);
   uint8 arg2 = PG_GETARG_UINT8(1);

   PG_RETURN_BOOL(arg1 > arg2);
}

/**
 * This function implements the "greater than or equal" (>=) operator for the uint1 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint1 datatypes.
 *
 * @param  arg1 The first uint1 value.
 * @param  arg2 The second uint1 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 >= arg2
 *         false if arg1 < arg2
 */
Datum
uint1ge(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);
   uint8 arg2 = PG_GETARG_UINT8(1);

   PG_RETURN_BOOL(arg1 >= arg2);
}

/**
 * This function implements the "equal" (=) operator between the int4
 * and uint1 data types.
 *
 * This function is required to support hash joins for the uint1 data
 * type when the one argument is an int4 data type.
 *
 * Example query:
 *   SELECT t.u1 FROM test t, generate_series(1, 10) gs WHERE t.u1 = gs;
 *
 * XXX
 * RBRAD TODO: test and make sure this works.
 * XXX
 *
 * @param  arg1 The int32 value.
 * @param  arg2 The uint1 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 == arg2
 *         false if arg1 != arg2
 */
Datum
int4uint1eq(PG_FUNCTION_ARGS)
{
   int32 arg1 = PG_GETARG_INT32(0);
   uint8 arg2 = PG_GETARG_UINT8(1);

   PG_RETURN_BOOL(arg1 == (int32)arg2);
}


/*
 *  ============================
 *  UINT1 BIT OPERATION ROUTINES
 *  ============================
 */
PG_FUNCTION_INFO_V1(uint1and);
PG_FUNCTION_INFO_V1(uint1or);
PG_FUNCTION_INFO_V1(uint1xor);
PG_FUNCTION_INFO_V1(uint1not);
PG_FUNCTION_INFO_V1(uint1shl);
PG_FUNCTION_INFO_V1(uint1shr);

/**
 * This function implements the bit-wise AND (&) operator for the uint1 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint1 datatypes.
 *
 * @param  arg1 The first uint1 value.
 * @param  arg2 The second uint1 value.
 * @return The Datum containing the new value.
 */
Datum
uint1and(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);
   uint8 arg2 = PG_GETARG_UINT8(1);

   PG_RETURN_UINT8(arg1 & arg2);
}

/**
 * This function implements the bit-wise OR (|) operator for the uint1 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint1 datatypes.
 *
 * @param  arg1 The first uint1 value.
 * @param  arg2 The second uint1 value.
 * @return The Datum containing the new value.
 */
Datum
uint1or(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);
   uint8 arg2 = PG_GETARG_UINT8(1);

   PG_RETURN_UINT8(arg1 | arg2);
}

/**
 * This function implements the bit-wise XOR (#) operator for the uint1 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint1 datatypes.
 *
 * @param  arg1 The first uint1 value.
 * @param  arg2 The second uint1 value.
 * @return The Datum containing the new value.
 */
Datum
uint1xor(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);
   uint8 arg2 = PG_GETARG_UINT8(1);

   PG_RETURN_UINT8(arg1 ^ arg2);
}

/**
 * This function implements the compliment (~) operator for the uint1 datatype.
 *
 * @param  arg1 The uint1 value.
 * @return The Datum containing the new value.
 */
Datum
uint1not(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);

   PG_RETURN_UINT8(~arg1);
}

/**
 * This function implements the "shift left" (<<) operator for the uint1 datatype.
 *
 * @param  arg1 The uint1 value.
 * @param  arg2 The number of bits to shift.
 * @return The Datum containing the new value.
 */
Datum
uint1shl(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);
   int32 arg2 = PG_GETARG_INT32(1);

   PG_RETURN_UINT8(arg1 << arg2);
}

/**
 * This function implements the "shift right" (>>) operator for the uint1 datatype.
 *
 * @param  arg1 The uint1 value.
 * @param  arg2 The number of bits to shift.
 * @return The Datum containing the new value.
 */
Datum
uint1shr(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);
   int32 arg2 = PG_GETARG_INT32(1);

   PG_RETURN_UINT8(arg1 >> arg2);
}


/*
 *  ============================
 *  UINT1 HASH OPERATOR ROUTINES
 *  ============================
 */
PG_FUNCTION_INFO_V1(hashuint1);

/**
 * This function returns a hash value suitable for the hash index.
 *
 * @param  arg1 The uint1 value.
 * @return      The Datum containing the hash value.
 */
Datum
hashuint1(PG_FUNCTION_ARGS)
{
   uint8 arg1 = PG_GETARG_UINT8(0);

   return hash_uint32((uint32) arg1);
}


/*
 *  =========================
 *  UINT1 CONVERSION ROUTINES
 *  =========================
 */
PG_FUNCTION_INFO_V1(int4touint1);
PG_FUNCTION_INFO_V1(uint1toint4);

/**
 * Cast an int4 value into a uint1 value.
 * This function throws an error if the int4 value is out-of-range
 * for the uint1 value.
 *
 * @param  num The int4 value.
 * @return     The Datum containing the uint1 value.
 */
Datum
int4touint1(PG_FUNCTION_ARGS)
{
   int32 num = PG_GETARG_INT32(0);

   if(unlikely(num < 0 || num > UCHAR_MAX)) {
      ereport(ERROR, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
                      errmsg("uint1 out of range")));
   }

   PG_RETURN_UINT8((uint8) num);
}

/**
 * Cast a uint1 value into an int4 value.
 *
 * @param  num The uint1 value.
 * @return     The Datum containing the int4 value.
 */
Datum
uint1toint4(PG_FUNCTION_ARGS)
{
   uint8 num = PG_GETARG_UINT8(0);

   PG_RETURN_INT32((int32) num);
}


/*
 *  ==================
 *  UINT2 DECLARATIONS
 *  ==================
 */
Datum uint2in(PG_FUNCTION_ARGS);
Datum uint2out(PG_FUNCTION_ARGS);
Datum uint2recv(PG_FUNCTION_ARGS);
Datum uint2send(PG_FUNCTION_ARGS);
Datum btuint2cmp(PG_FUNCTION_ARGS);
Datum uint2eq(PG_FUNCTION_ARGS);
Datum uint2ne(PG_FUNCTION_ARGS);
Datum uint2lt(PG_FUNCTION_ARGS);
Datum uint2le(PG_FUNCTION_ARGS);
Datum uint2gt(PG_FUNCTION_ARGS);
Datum uint2ge(PG_FUNCTION_ARGS);
Datum int4uint2eq(PG_FUNCTION_ARGS);
Datum uint2and(PG_FUNCTION_ARGS);
Datum uint2or(PG_FUNCTION_ARGS);
Datum uint2xor(PG_FUNCTION_ARGS);
Datum uint2not(PG_FUNCTION_ARGS);
Datum uint2shl(PG_FUNCTION_ARGS);
Datum uint2shr(PG_FUNCTION_ARGS);
Datum hashuint2(PG_FUNCTION_ARGS);
Datum int4touint2(PG_FUNCTION_ARGS);
Datum uint2toint4(PG_FUNCTION_ARGS);


/*
 *  =====================
 *  UINT2 PUBLIC ROUTINES
 *  =====================
 */
PG_FUNCTION_INFO_V1(uint2in);
PG_FUNCTION_INFO_V1(uint2out);
PG_FUNCTION_INFO_V1(uint2recv);
PG_FUNCTION_INFO_V1(uint2send);

/**
 * Convert a c-string into a value suitable for the uint2 datatype.
 *
 * @param  s The c-string representation.
 * @return   The Datum containing the converted uint2 value.
 */
Datum
uint2in(PG_FUNCTION_ARGS)
{
   char *badp;
   unsigned long ul;
   char *s = PG_GETARG_CSTRING(0);

   /* Check for NULL pointer. */
   if(unlikely(s == NULL)) {
      elog(ERROR, "NULL pointer");
   }

   /*
    * Some versions of strtoul treat the empty string as an error, but some
    * seem not to.  Make an explicit test to be sure we catch it.
    */
   if(unlikely(*s == 0)) {
      ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
              errmsg("invalid input syntax for unsigned integer: \"%s\"", s)));
   }

   errno = 0;
   ul = strtoul(s, &badp, 10);

   /* We made no progress parsing the string, so bail out */
   if(unlikely(s == badp)) {
      ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
              errmsg("invalid input syntax for unsigned integer: \"%s\"", s)));
   }

   /* Verify value is valid in the uint2 datatype range. */
   if(unlikely(errno == ERANGE || ul < 0 || ul > USHRT_MAX)) {
      ereport(ERROR, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
              errmsg("value \"%s\" is out of range for type unsigned smallint", s)));
   }

   /*
    * Skip any trailing whitespace; if anything but whitespace remains before
    * the terminating character, bail out
    */
   while(*badp && isspace((unsigned char)*badp)) {
      badp++;
   }

   /* Verify the c-string is empty. */
   if(unlikely(*badp)) {
      ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
              errmsg("invalid input syntax for unsigned integer: \"%s\"", s)));
   }

   PG_RETURN_UINT16((uint16)ul);
}

/**
 * Convert the uint2 value into a c-string.
 *
 * @param  num The uint2 value.
 * @return     The Datum containing the converted uint2 value.
 */
Datum
uint2out(PG_FUNCTION_ARGS)
{
   uint16 num = PG_GETARG_UINT16(0);
   char *str = (char *) palloc(7);

   snprintf(str, 7, "%u", num);
   PG_RETURN_CSTRING(str);
}

/**
 * Convert the binary represented value into a value suitable for
 * the uint2 datatype.
 *
 * @param  buf The uint2 value.
 * @return     The Datum containing the converted uint2 value.
 */
Datum
uint2recv(PG_FUNCTION_ARGS)
{
   StringInfo buf = (StringInfo) PG_GETARG_POINTER(0);
   PG_RETURN_UINT16((uint16) pq_getmsgint(buf, sizeof(uint16)));
}

/**
 * Convert the uint2 datatype into its binary representation.
 *
 * @param  num The uint2 value.
 * @return     The Datum containing the converted uint2 value.
 */
Datum
uint2send(PG_FUNCTION_ARGS)
{
   uint16 num = PG_GETARG_UINT16(0);
   StringInfoData buf;

   pq_begintypsend(&buf);
   pq_sendint(&buf, num, sizeof(uint16));
   PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}


/*
 *  ==================================
 *  UINT2 COMPARISON OPERATOR ROUTINES
 *  ==================================
 */

PG_FUNCTION_INFO_V1(btuint2cmp);

/**
 * Comparison function used by the B-tree index for sorting purposes.
 *
 * It is safe to directly subtract the values in this function because
 * the return value can not overflow a signed 32-bit integer.
 *
 * @param  a The first uint2 value.
 * @param  b The second uint2 value.
 * @return The Datum containing the comparison value.
 *         The comparison value is:
 *         < 0 if a < b
 *           0 if a == b
 *         > 0 if a > b
 */
Datum
btuint2cmp(PG_FUNCTION_ARGS)
{
   uint16 a = PG_GETARG_UINT16(0);
   uint16 b = PG_GETARG_UINT16(1);

   PG_RETURN_INT32((int32)a - (int32)b);
}

PG_FUNCTION_INFO_V1(uint2eq);
PG_FUNCTION_INFO_V1(uint2ne);
PG_FUNCTION_INFO_V1(uint2lt);
PG_FUNCTION_INFO_V1(uint2le);
PG_FUNCTION_INFO_V1(uint2gt);
PG_FUNCTION_INFO_V1(uint2ge);
PG_FUNCTION_INFO_V1(uint2int4eq);

/**
 * This function implements the "equal" (=) operator for the uint2 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint2 datatypes.
 *
 * @param  arg1 The first uint2 value.
 * @param  arg2 The second uint2 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 == arg2
 *         false if arg1 != arg2
 */
Datum
uint2eq(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);
   uint16 arg2 = PG_GETARG_UINT16(1);

   PG_RETURN_BOOL(arg1 == arg2);
}

/**
 * This function implements the "not equal" (<>) operator for the uint2 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint2 datatypes.
 *
 * @param  arg1 The first uint2 value.
 * @param  arg2 The second uint2 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 != arg2
 *         false if arg1 == arg2
 */
Datum
uint2ne(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);
   uint16 arg2 = PG_GETARG_UINT16(1);

   PG_RETURN_BOOL(arg1 != arg2);
}

/**
 * This function implements the "less than" (<) operator for the uint2 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint2 datatypes.
 *
 * @param  arg1 The first uint2 value.
 * @param  arg2 The second uint2 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 < arg2
 *         false if arg1 >= arg2
 */
Datum
uint2lt(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);
   uint16 arg2 = PG_GETARG_UINT16(1);

   PG_RETURN_BOOL(arg1 < arg2);
}

/**
 * This function implements the "less than or equal" (<=) operator for the uint2 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint2 datatypes.
 *
 * @param  arg1 The first uint2 value.
 * @param  arg2 The second uint2 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 <= arg2
 *         false if arg1 > arg2
 */
Datum
uint2le(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);
   uint16 arg2 = PG_GETARG_UINT16(1);

   PG_RETURN_BOOL(arg1 <= arg2);
}

/**
 * This function implements the "greater than" (>) operator for the uint2 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint2 datatypes.
 *
 * @param  arg1 The first uint2 value.
 * @param  arg2 The second uint2 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 > arg2
 *         false if arg1 <= arg2
 */
Datum
uint2gt(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);
   uint16 arg2 = PG_GETARG_UINT16(1);

   PG_RETURN_BOOL(arg1 > arg2);
}

/**
 * This function implements the "greater than or equal" (>=) operator for the uint2 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint2 datatypes.
 *
 * @param  arg1 The first uint2 value.
 * @param  arg2 The second uint2 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 >= arg2
 *         false if arg1 < arg2
 */
Datum
uint2ge(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);
   uint16 arg2 = PG_GETARG_UINT16(1);

   PG_RETURN_BOOL(arg1 >= arg2);
}

/**
 * This function implements the "equal" (=) operator between the int4
 * and uint2 data types.
 *
 * This function is required to support hash joins for the uint2 data
 * type when the one argument is an int2 data type.
 *
 * Example query:
 *   SELECT t.u2 FROM test t, generate_series(1, 10) gs WHERE t.u2 = gs;
 *
 * XXX
 * RBRAD TODO: test and make sure this works.
 * XXX
 *
 * @param  arg1 The int32 value.
 * @param  arg2 The uint2 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 == arg2
 *         false if arg1 != arg2
 */
Datum
int4uint2eq(PG_FUNCTION_ARGS)
{
   int32 arg1 = PG_GETARG_INT32(0);
   uint16 arg2 = PG_GETARG_UINT16(1);

   PG_RETURN_BOOL(arg1 == (int32)arg2);
}


/*
 *  ============================
 *  UINT2 BIT OPERATION ROUTINES
 *  ============================
 */
PG_FUNCTION_INFO_V1(uint2and);
PG_FUNCTION_INFO_V1(uint2or);
PG_FUNCTION_INFO_V1(uint2xor);
PG_FUNCTION_INFO_V1(uint2not);
PG_FUNCTION_INFO_V1(uint2shl);
PG_FUNCTION_INFO_V1(uint2shr);

/**
 * This function implements the bit-wise AND (&) operator for the uint2 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint2 datatypes.
 *
 * @param  arg1 The first uint2 value.
 * @param  arg2 The second uint2 value.
 * @return The Datum containing the new value.
 */
Datum
uint2and(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);
   uint16 arg2 = PG_GETARG_UINT16(1);

   PG_RETURN_UINT16(arg1 & arg2);
}

/**
 * This function implements the bit-wise OR (|) operator for the uint2 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint2 datatypes.
 *
 * @param  arg1 The first uint2 value.
 * @param  arg2 The second uint2 value.
 * @return The Datum containing the new value.
 */
Datum
uint2or(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);
   uint16 arg2 = PG_GETARG_UINT16(1);

   PG_RETURN_UINT16(arg1 | arg2);
}

/**
 * This function implements the bit-wise XOR (#) operator for the uint2 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint2 datatypes.
 *
 * @param  arg1 The first uint2 value.
 * @param  arg2 The second uint2 value.
 * @return The Datum containing the new value.
 */
Datum
uint2xor(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);
   uint16 arg2 = PG_GETARG_UINT16(1);

   PG_RETURN_UINT16(arg1 ^ arg2);
}

/**
 * This function implements the compliment (~) operator for the uint2 datatype.
 *
 * @param  arg1 The uint2 value.
 * @return The Datum containing the new value.
 */
Datum
uint2not(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);

   PG_RETURN_UINT16(~arg1);
}

/**
 * This function implements the "shift left" (<<) operator for the uint2 datatype.
 *
 * @param  arg1 The uint2 value.
 * @param  arg2 The number of bits to shift.
 * @return The Datum containing the new value.
 */
Datum
uint2shl(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);
   int32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_UINT16(arg1 << arg2);
}

/**
 * This function implements the "shift right" (>>) operator for the uint2 datatype.
 *
 * @param  arg1 The uint2 value.
 * @param  arg2 The number of bits to shift.
 * @return The Datum containing the new value.
 */
Datum
uint2shr(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);
   int32 arg2 = PG_GETARG_INT32(1);

   PG_RETURN_UINT16(arg1 >> arg2);
}


/*
 *  ============================
 *  UINT2 HASH OPERATOR ROUTINES
 *  ============================
 */
PG_FUNCTION_INFO_V1(hashuint2);

/**
 * This function returns a hash value suitable for the hash index.
 *
 * @param  arg1 The uint2 value.
 * @return      The Datum containing the hash value.
 */
Datum
hashuint2(PG_FUNCTION_ARGS)
{
   uint16 arg1 = PG_GETARG_UINT16(0);

   return hash_uint32((uint32) arg1);
}


/*
 *  =========================
 *  UINT2 CONVERSION ROUTINES
 *  =========================
 */
PG_FUNCTION_INFO_V1(int4touint2);
PG_FUNCTION_INFO_V1(uint2toint4);

/**
 * Cast an int4 value into a uint2 value.
 * This function throws an error if the int4 value is out-of-range
 * for the uint2 value.
 *
 * @param  num The int4 value.
 * @return     The Datum containing the uint2 value.
 */
Datum
int4touint2(PG_FUNCTION_ARGS)
{
   int32 num = PG_GETARG_INT32(0);

   if(unlikely(num < 0 || num > USHRT_MAX)) {
      ereport(ERROR, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
                      errmsg("unsigned smallint out of range")));
   }

   PG_RETURN_UINT16((uint16) num);
}

/**
 * Cast a uint2 value into an int4 value.
 *
 * @param  num The uint2 value.
 * @return     The Datum containing the int4 value.
 */
Datum
uint2toint4(PG_FUNCTION_ARGS)
{
   uint16 num = PG_GETARG_UINT16(0);

   PG_RETURN_INT32((int32) num);
}



/*
 *  ==================
 *  UINT4 DECLARATIONS
 *  ==================
 */
Datum uint4in(PG_FUNCTION_ARGS);
Datum uint4out(PG_FUNCTION_ARGS);
Datum uint4recv(PG_FUNCTION_ARGS);
Datum uint4send(PG_FUNCTION_ARGS);
Datum btuint4cmp(PG_FUNCTION_ARGS);
Datum uint4eq(PG_FUNCTION_ARGS);
Datum uint4ne(PG_FUNCTION_ARGS);
Datum uint4lt(PG_FUNCTION_ARGS);
Datum uint4le(PG_FUNCTION_ARGS);
Datum uint4gt(PG_FUNCTION_ARGS);
Datum uint4ge(PG_FUNCTION_ARGS);
Datum int4uint4eq(PG_FUNCTION_ARGS);
Datum int4uint4ne(PG_FUNCTION_ARGS);
Datum int4uint4lt(PG_FUNCTION_ARGS);
Datum int4uint4le(PG_FUNCTION_ARGS);
Datum int4uint4gt(PG_FUNCTION_ARGS);
Datum int4uint4ge(PG_FUNCTION_ARGS);
Datum uint4int4eq(PG_FUNCTION_ARGS);
Datum uint4int4ne(PG_FUNCTION_ARGS);
Datum uint4int4lt(PG_FUNCTION_ARGS);
Datum uint4int4le(PG_FUNCTION_ARGS);
Datum uint4int4gt(PG_FUNCTION_ARGS);
Datum uint4int4ge(PG_FUNCTION_ARGS);
Datum uint4and(PG_FUNCTION_ARGS);
Datum uint4or(PG_FUNCTION_ARGS);
Datum uint4xor(PG_FUNCTION_ARGS);
Datum uint4not(PG_FUNCTION_ARGS);
Datum uint4shl(PG_FUNCTION_ARGS);
Datum uint4shr(PG_FUNCTION_ARGS);
Datum hashuint4(PG_FUNCTION_ARGS);
Datum hashuint4_from_int4(PG_FUNCTION_ARGS);
Datum uint4toint4(PG_FUNCTION_ARGS);
Datum int4touint4(PG_FUNCTION_ARGS);
Datum uint4toint8(PG_FUNCTION_ARGS);
Datum int8touint4(PG_FUNCTION_ARGS);


/*
 *  =====================
 *  UINT4 PUBLIC ROUTINES
 *  =====================
 */

PG_FUNCTION_INFO_V1(uint4in);
PG_FUNCTION_INFO_V1(uint4out);
PG_FUNCTION_INFO_V1(uint4recv);
PG_FUNCTION_INFO_V1(uint4send);

/**
 * Convert a c-string into a value suitable for the uint4 datatype.
 *
 * @param  s The c-string representation.
 * @return   The Datum containing the converted uint4 value.
 */
Datum
uint4in(PG_FUNCTION_ARGS)
{
   char *badp;
   unsigned long ul;
   char *s = PG_GETARG_CSTRING(0);

   /* Check for NULL pointer. */
   if(unlikely(s == NULL)) {
      elog(ERROR, "NULL pointer");
   }

   /*
    * Some versions of strtoul treat the empty string as an error, but some
    * seem not to.  Make an explicit test to be sure we catch it.
    */
   if(unlikely(*s == 0)) {
      ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
              errmsg("invalid input syntax for unsigned integer: \"%s\"", s)));
   }

   errno = 0;
   ul = strtoul(s, &badp, 10);

   /* We made no progress parsing the string, so bail out */
   if(unlikely(s == badp)) {
      ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
              errmsg("invalid input syntax for unsigned integer: \"%s\"", s)));
   }

   /* Verify value is valid in the uint4 datatype range. */
   if(unlikely(errno == ERANGE || ul < 0UL || ul > UINT_MAX)) {
      ereport(ERROR, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
              errmsg("value \"%s\" is out of range for type unsigned integer", s)));
   }

   /*
    * Skip any trailing whitespace; if anything but whitespace remains before
    * the terminating character, bail out
    */
   while(*badp && isspace((unsigned char)*badp)) {
      badp++;
   }

   /* Verify the c-string is empty. */
   if(unlikely(*badp)) {
      ereport(ERROR, (errcode(ERRCODE_INVALID_TEXT_REPRESENTATION),
              errmsg("invalid input syntax for unsigned integer: \"%s\"", s)));
   }

   PG_RETURN_UINT32((uint32)ul);
}

/**
 * Convert the uint4 value into a c-string.
 *
 * @param  num The uint4 value.
 * @return     The Datum containing the converted uint4 value.
 */
Datum
uint4out(PG_FUNCTION_ARGS)
{
   uint32 num = PG_GETARG_UINT32(0);
   char *str = (char *) palloc(12);

   snprintf(str, 12, "%u", num);
   PG_RETURN_CSTRING(str);
}

/**
 * Convert the binary represented value into a value suitable for
 * the uint4 datatype.
 *
 * @param  buf The uint4 value.
 * @return     The Datum containing the converted uint4 value.
 */
Datum
uint4recv(PG_FUNCTION_ARGS)
{
   StringInfo buf = (StringInfo) PG_GETARG_POINTER(0);
   PG_RETURN_UINT32((uint32) pq_getmsgint(buf, sizeof(uint32)));
}

/**
 * Convert the uint4 datatype into its binary representation.
 *
 * @param  num The uint4 value.
 * @return     The Datum containing the converted uint4 value.
 */
Datum
uint4send(PG_FUNCTION_ARGS)
{
   uint32 num = PG_GETARG_UINT32(0);
   StringInfoData buf;

   pq_begintypsend(&buf);
   pq_sendint(&buf, num, sizeof(uint32));
   PG_RETURN_BYTEA_P(pq_endtypsend(&buf));
}


/*
 *   ==================================
 *   UINT4 COMPARISON OPERATOR ROUTINES
 *   ==================================
 */

PG_FUNCTION_INFO_V1(btuint4cmp);

/**
 * Comparison function used by the B-tree index for sorting purposes.
 *
 * It is NOT safe to directly subtract the values in this function
 * because the return value could overflow a signed 32-bit integer.
 *
 * @param  a The first uint1 value.
 * @param  b The second uint1 value.
 * @return The Datum containing the comparison value.
 *         The comparison value is:
 *         -1 if a < b
 *          0 if a == b
 *          1 if a > b
 */
Datum
btuint4cmp(PG_FUNCTION_ARGS)
{
   uint32 a = PG_GETARG_UINT32(0);
   uint32 b = PG_GETARG_UINT32(1);

   if(a > b) {
      PG_RETURN_INT32(1);
   }

   if(a == b) {
      PG_RETURN_INT32(0);
   }

   PG_RETURN_INT32(-1);
}

PG_FUNCTION_INFO_V1(uint4eq);
PG_FUNCTION_INFO_V1(uint4ne);
PG_FUNCTION_INFO_V1(uint4lt);
PG_FUNCTION_INFO_V1(uint4le);
PG_FUNCTION_INFO_V1(uint4gt);
PG_FUNCTION_INFO_V1(uint4ge);

/**
 * This function implements the "equal" (=) operator for the uint4 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint4 datatypes.
 *
 * @param  arg1 The first uint4 value.
 * @param  arg2 The second uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 == arg2
 *         false if arg1 != arg2
 */
Datum
uint4eq(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(arg1 == arg2);
}

/**
 * This function implements the "not equal" (<>) operator for the uint4 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint4 datatypes.
 *
 * @param  arg1 The first uint4 value.
 * @param  arg2 The second uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 != arg2
 *         false if arg1 == arg2
 */
Datum
uint4ne(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(arg1 != arg2);
}

/**
 * This function implements the "less than" (<) operator for the uint4 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint4 datatypes.
 *
 * @param  arg1 The first uint4 value.
 * @param  arg2 The second uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 < arg2
 *         false if arg1 >= arg2
 */
Datum
uint4lt(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(arg1 < arg2);
}

/**
 * This function implements the "less than or equal" (<=) operator for the uint4 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint4 datatypes.
 *
 * @param  arg1 The first uint4 value.
 * @param  arg2 The second uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 <= arg2
 *         false if arg1 > arg2
 */
Datum
uint4le(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(arg1 <= arg2);
}

/**
 * This function implements the "greater than" (>) operator for the uint4 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint4 datatypes.
 *
 * @param  arg1 The first uint4 value.
 * @param  arg2 The second uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 > arg2
 *         false if arg1 <= arg2
 */
Datum
uint4gt(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(arg1 > arg2);
}

/**
 * This function implements the "greater than or equal" (>=) operator for the uint4 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint4 datatypes.
 *
 * @param  arg1 The first uint4 value.
 * @param  arg2 The second uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 >= arg2
 *         false if arg1 < arg2
 */
Datum
uint4ge(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(arg1 >= arg2);
}

PG_FUNCTION_INFO_V1(int4uint4eq);
PG_FUNCTION_INFO_V1(int4uint4ne);
PG_FUNCTION_INFO_V1(int4uint4lt);
PG_FUNCTION_INFO_V1(int4uint4le);
PG_FUNCTION_INFO_V1(int4uint4gt);
PG_FUNCTION_INFO_V1(int4uint4ge);


/**
 * Internal function used by the int4uint4 operators.
 *
 * It is NOT safe to directly subtract the values in this function because
 * the return value can overflow a signed 32-bit integer.
 *
 * @param  a The int4 value.
 * @param  b The uint4 value.
 * @return The Datum containing the comparison value.
 *         The comparison value is:
 *         -1 if a < b
 *          0 if a == b
 *          1 if a > b
 */
static int
int4uint4cmp(int32 a, uint32 b)
{
   if(unlikely(a < 0)) {
      return -1;
   }

   if((uint32)a > b) {
      return 1;
   }

   if((uint32)a < b) {
      return -1;
   }

   return 0;
}

/**
 * This function implements the "equal" (=) operator for the uint4 datatype
 * where the the left argument is an int4 and the right argument is a uint4.
 *
 * @param  arg1 The int4 value.
 * @param  arg2 The uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 == arg2
 *         false if arg1 != arg2
 */
Datum
int4uint4eq(PG_FUNCTION_ARGS)
{
   int32 arg1 = PG_GETARG_INT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(int4uint4cmp(arg1, arg2) == 0);
}

/**
 * This function implements the "not equal" (<>) operator for the uint4 datatype
 * where the the left argument is an int4 and the right argument is a uint4.
 *
 * @param  arg1 The int4 value.
 * @param  arg2 The uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 != arg2
 *         false if arg1 == arg2
 */
Datum
int4uint4ne(PG_FUNCTION_ARGS)
{
   int32 arg1 = PG_GETARG_INT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(int4uint4cmp(arg1, arg2) != 0);
}

/**
 * This function implements the "less than" (<) operator for the uint4 datatype
 * where the the left argument is an int4 and the right argument is a uint4.
 *
 * @param  arg1 The int4 value.
 * @param  arg2 The uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 < arg2
 *         false if arg1 >= arg2
 */
Datum
int4uint4lt(PG_FUNCTION_ARGS)
{
   int32 arg1 = PG_GETARG_INT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(int4uint4cmp(arg1, arg2) < 0);
}

/**
 * This function implements the "less than or equal" (<=) operator for the uint4 datatype
 * where the the left argument is an int4 and the right argument is a uint4.
 *
 * @param  arg1 The int4 value.
 * @param  arg2 The uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 <= arg2
 *         false if arg1 > arg2
 */
Datum
int4uint4le(PG_FUNCTION_ARGS)
{
   int32 arg1 = PG_GETARG_INT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(int4uint4cmp(arg1, arg2) <= 0);
}

/**
 * This function implements the "greater than" (>) operator for the uint4 datatype
 * where the the left argument is an int4 and the right argument is a uint4.
 *
 * @param  arg1 The int4 value.
 * @param  arg2 The uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 > arg2
 *         false if arg1 <= arg2
 */
Datum
int4uint4gt(PG_FUNCTION_ARGS)
{
   int32 arg1 = PG_GETARG_INT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(int4uint4cmp(arg1, arg2) > 0);
}

/**
 * This function implements the "greater than or equal" (>=) operator for the uint4 datatype
 * where the the left argument is an int4 and the right argument is a uint4.
 *
 * @param  arg1 The int4 value.
 * @param  arg2 The uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 >= arg2
 *         false if arg1 < arg2
 */
Datum
int4uint4ge(PG_FUNCTION_ARGS)
{
   int32 arg1 = PG_GETARG_INT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_BOOL(int4uint4cmp(arg1, arg2) >= 0);
}

PG_FUNCTION_INFO_V1(uint4int4eq);
PG_FUNCTION_INFO_V1(uint4int4ne);
PG_FUNCTION_INFO_V1(uint4int4lt);
PG_FUNCTION_INFO_V1(uint4int4le);
PG_FUNCTION_INFO_V1(uint4int4gt);
PG_FUNCTION_INFO_V1(uint4int4ge);

/**
 * Internal function used by the uint4int4 operators.
 *
 * It is NOT safe to directly subtract the values in this function because
 * the return value can overflow a signed 32-bit integer.
 *
 * @param  a The uint4 value.
 * @param  b The int4 value.
 * @return The Datum containing the comparison value.
 *         The comparison value is:
 *         -1 if a < b
 *          0 if a == b
 *          1 if a > b
 */
static int
uint4int4cmp(uint32 a, int32 b)
{
   if(unlikely(b < 0)) {
      return 1;
   }

   if((uint32)a > b) {
      return 1;
   }

   if((uint32)a < b) {
      return -1;
   }

   return 0;
}

/**
 * This function implements the "equal" (=) operator for the uint4 datatype
 * where the the left argument is a uint4 and the right argument is an int4.
 *
 * @param  arg1 The uint4 value.
 * @param  arg2 The int4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 == arg2
 *         false if arg1 != arg2
 */
Datum
uint4int4eq(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   int32 arg2 = PG_GETARG_INT32(1);

   PG_RETURN_BOOL(uint4int4cmp(arg1, arg2) == 0);
}

/**
 * This function implements the "not equal" (<>) operator for the uint4 datatype
 * where the the left argument is a uint4 and the right argument is an int4.
 *
 * @param  arg1 The uint4 value.
 * @param  arg2 The int4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 != arg2
 *         false if arg1 == arg2
 */
Datum
uint4int4ne(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   int32 arg2 = PG_GETARG_INT32(1);

   PG_RETURN_BOOL(uint4int4cmp(arg1, arg2) != 0);
}

/**
 * This function implements the "less than" (<) operator for the uint4 datatype
 * where the the left argument is a uint4 and the right argument is an int4.
 *
 * @param  arg1 The uint4 value.
 * @param  arg2 The int4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 < arg2
 *         false if arg1 >= arg2
 */
Datum
uint4int4lt(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   int32 arg2 = PG_GETARG_INT32(1);

   PG_RETURN_BOOL(uint4int4cmp(arg1, arg2) < 0);
}

/**
 * This function implements the "less than or equal" (<=) operator for the uint4 datatype
 * where the the left argument is a uint4 and the right argument is an int4.
 *
 * @param  arg1 The uint4 value.
 * @param  arg2 The int4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 <= arg2
 *         false if arg1 > arg2
 */
Datum
uint4int4le(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   int32 arg2 = PG_GETARG_INT32(1);

   PG_RETURN_BOOL(uint4int4cmp(arg1, arg2) <= 0);
}

/**
 * This function implements the "greater than" (>) operator for the uint4 datatype
 * where the the left argument is a uint4 and the right argument is an int4.
 *
 * @param  arg1 The uint4 value.
 * @param  arg2 The int4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 > arg2
 *         false if arg1 <= arg2
 */
Datum
uint4int4gt(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   int32 arg2 = PG_GETARG_INT32(1);

   PG_RETURN_BOOL(uint4int4cmp(arg1, arg2) > 0);
}

/**
 * This function implements the "greater than or equal" (>=) operator for the uint4 datatype
 * where the the left argument is a uint4 and the right argument is an int4.
 *
 * @param  arg1 The int4 value.
 * @param  arg2 The uint4 value.
 * @return The Datum containing the boolean operator value.
 *         The operator value is:
 *         true if arg1 >= arg2
 *         false if arg1 < arg2
 */
Datum
uint4int4ge(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   int32 arg2 = PG_GETARG_INT32(1);

   PG_RETURN_BOOL(uint4int4cmp(arg1, arg2) >= 0);
}


/*
 *  ============================
 *  UINT4 BIT OPERATION ROUTINES
 *  ============================
 */
PG_FUNCTION_INFO_V1(uint4and);
PG_FUNCTION_INFO_V1(uint4or);
PG_FUNCTION_INFO_V1(uint4xor);
PG_FUNCTION_INFO_V1(uint4not);
PG_FUNCTION_INFO_V1(uint4shl);
PG_FUNCTION_INFO_V1(uint4shr);

/**
 * This function implements the bit-wise AND (&) operator for the uint4 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint4 datatypes.
 *
 * @param  arg1 The first uint4 value.
 * @param  arg2 The second uint4 value.
 * @return The Datum containing the new value.
 */
Datum
uint4and(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_UINT32(arg1 & arg2);
}

/**
 * This function implements the bit-wise OR (|) operator for the uint4 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint4 datatypes.
 *
 * @param  arg1 The first uint4 value.
 * @param  arg2 The second uint4 value.
 * @return The Datum containing the new value.
 */
Datum
uint4or(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_UINT32(arg1 | arg2);
}

/**
 * This function implements the bit-wise XOR (#) operator for the uint4 datatype.
 *
 * This function only supports the operator function when both arguments
 * are uint4 datatypes.
 *
 * @param  arg1 The first uint4 value.
 * @param  arg2 The second uint4 value.
 * @return The Datum containing the new value.
 */
Datum
uint4xor(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   uint32 arg2 = PG_GETARG_UINT32(1);

   PG_RETURN_UINT32(arg1 ^ arg2);
}

/**
 * This function implements the compliment (~) operator for the uint4 datatype.
 *
 * @param  arg1 The uint4 value.
 * @return The Datum containing the new value.
 */
Datum
uint4not(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);

   PG_RETURN_UINT32(~arg1);
}

/**
 * This function implements the "shift left" (<<) operator for the uint4 datatype.
 *
 * @param  arg1 The uint4 value.
 * @param  arg2 The number of bits to shift.
 * @return The Datum containing the new value.
 */
Datum
uint4shl(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   int32 arg2 = PG_GETARG_INT32(1);

   PG_RETURN_UINT32(arg1 << arg2);
}

/**
 * This function implements the "shift right" (>>) operator for the uint4 datatype.
 *
 * @param  arg1 The uint4 value.
 * @param  arg2 The number of bits to shift.
 * @return The Datum containing the new value.
 */
Datum
uint4shr(PG_FUNCTION_ARGS)
{
   uint32 arg1 = PG_GETARG_UINT32(0);
   int32 arg2 = PG_GETARG_INT32(1);

   PG_RETURN_UINT32(arg1 >> arg2);
}


/*
 *  ============================
 *  UINT4 HASH OPERATOR ROUTINES
 *  ============================
 */
PG_FUNCTION_INFO_V1(hashuint4);
PG_FUNCTION_INFO_V1(hashuint4_from_int4);

/**
 * This function returns a hash value suitable for the hash index.
 *
 * @param  arg1 The uint4 value.
 * @return      The Datum containing the hash value.
 */
Datum
hashuint4(PG_FUNCTION_ARGS)
{
   return hash_uint32(PG_GETARG_UINT32(0));
}

/**
 * This function returns a hash value suitable for the hash index.
 *
 * @param  arg1 The int4 value.
 * @return      The Datum containing the hash value.
 */
Datum
hashuint4_from_int4(PG_FUNCTION_ARGS)
{
   return hash_uint32((uint32)PG_GETARG_INT32(0));
}


/*
 *   =========================
 *   UINT4 CONVERSION ROUTINES
 *   =========================
 */
PG_FUNCTION_INFO_V1(uint4toint4);
PG_FUNCTION_INFO_V1(int4touint4);
PG_FUNCTION_INFO_V1(uint4toint8);
PG_FUNCTION_INFO_V1(int8touint4);

/**
 * Cast a uint4 value into an int1 value.
 * This function throws an error if the uint4 value is out-of-range
 * for the int4 value.
 *
 * @param  num The int4 value.
 * @return     The Datum containing the uint1 value.
 */
Datum
uint4toint4(PG_FUNCTION_ARGS)
{
   uint32 num = PG_GETARG_UINT32(0);

   if(unlikely(num > INT_MAX)) {
      ereport(ERROR, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
                      errmsg("integer out of range")));
   }

   PG_RETURN_INT32((uint32) num);
}

/**
 * Cast an int4 value into a uint4 value.
 * This function throws an error if the int4 value is out-of-range
 * for the uint4 value.
 *
 * @param  num The int4 value.
 * @return     The Datum containing the uint4 value.
 */
Datum
int4touint4(PG_FUNCTION_ARGS)
{
   int32 num = PG_GETARG_INT32(0);

   if(unlikely(num < 0)) {
      ereport(ERROR, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
                      errmsg("unsigned integer out of range")));
   }

   PG_RETURN_UINT32((uint32) num);
}

/**
 * Cast a uint4 value into a int8 value.
 *
 * @param  num The int4 value.
 * @return     The Datum containing the int8 value.
 */
Datum
uint4toint8(PG_FUNCTION_ARGS)
{
   uint32 num = PG_GETARG_UINT32(0);

   PG_RETURN_INT64((int64) num);
}

/**
 * Cast an int8 value into a uint4 value.
 * This function throws an error if the int8 value is out-of-range
 * for the uint4 value.
 *
 * @param  num The int4 value.
 * @return     The Datum containing the uint4 value.
 */
Datum
int8touint4(PG_FUNCTION_ARGS)
{
   int64 num = PG_GETARG_INT64(0);

   if(unlikely(num < 0 || num > UINT_MAX)) {
      ereport(ERROR, (errcode(ERRCODE_NUMERIC_VALUE_OUT_OF_RANGE),
                      errmsg("unsigned integer out of range")));
   }

   PG_RETURN_UINT32((uint32) num);
}


/*
 *   ===============================
 *   SELECTIVITY ESTIMATION ROUTINES
 *   ===============================
 */

Datum int4ltsel(PG_FUNCTION_ARGS);
Datum int4gtsel(PG_FUNCTION_ARGS);

PG_FUNCTION_INFO_V1(int4ltsel);
PG_FUNCTION_INFO_V1(int4gtsel);

static double
convert_int4_to_double(Datum value)
{
   return (double)DatumGetInt32(value);
}

static double
convert_uint4_to_double(Datum value)
{
   return (double)DatumGetUInt32(value);
}

/*
 * uint_histogram_selectivity  - Examine the histogram for scalarineqsel
 *
 * Determine the fraction of the variable's histogram population that
 * satisfies the inequality condition, ie, VAR < CONST or VAR > CONST.
 *
 * Returns zero if there is no histogram (valid results will always be
 * greater than zero).
 *
 * Note that the result disregards both the most-common-values (if any) and
 * null entries.  The caller is expected to combine this result with
 * statistics for those portions of the column population.
 */
static double
uint_histogram_selectivity(VariableStatData *vardata, FmgrInfo *opproc,
                           bool isgt, Datum constval,
                           double (*convert_value)(Datum),
                           double (*convert_stats)(Datum))
{
   double hist_selec;
   Datum *values;
   int nvalues;

   hist_selec = 0.0;

   /*
    * Someday, ANALYZE might store more than one histogram per rel/att,
    * corresponding to more than one possible sort ordering defined for the
    * column type.  However, to make that work we will need to figure out
    * which staop to search for --- it's not necessarily the one we have at
    * hand!  (For example, we might have a '<=' operator rather than the '<'
    * operator that will appear in staop.)  For now, assume that whatever
    * appears in pg_statistic is sorted the same way our operator sorts, or
    * the reverse way if isgt is TRUE.
    */
   if(HeapTupleIsValid(vardata->statsTuple) &&
      get_attstatsslot(vardata->statsTuple, vardata->atttype,
                       vardata->atttypmod, STATISTIC_KIND_HISTOGRAM,
                       InvalidOid,
                       NULL,  /* Added for postgresql 9 compatibality */
                       &values, &nvalues, NULL, NULL))
   {
      if(nvalues > 1) {
         /*
          * Use binary search to find proper location, ie, the first slot
          * at which the comparison fails.  (If the given operator isn't
          * actually sort-compatible with the histogram, you'll get garbage
          * results ... but probably not any more garbage-y than you would
          * from the old linear search.)
          */
         double histfrac;
         int lobound = 0;        /* first possible slot to search */
         int hibound = nvalues;  /* last+1 slot to search */

         while(lobound < hibound) {
            int probe = (lobound + hibound) / 2;
            bool ltcmp;

            ltcmp = DatumGetBool(FunctionCall2(opproc, values[probe], constval));
            if(isgt)
               ltcmp = !ltcmp;

            if(ltcmp)
               lobound = probe + 1;
            else
               hibound = probe;
         }

         if(lobound >= nvalues) {
            /* Constant is above upper histogram boundary. */
            histfrac = 1.0;
         }
         else
         {
            int i = lobound;
            double val, high, low;
            double binfrac;

            /*
             * We have values[i-1] < constant < values[i].
             *
             * Convert the constant and the two nearest bin boundary
             * values to a uniform comparison scale, and do a linear
             * interpolation within this bin.
             */

            val = (*convert_value)(constval);
            low = (*convert_stats)(values[i - 1]);
            high = (*convert_stats)(values[i]);

            if(high <= low)
               binfrac = 0.5; /* cope if bin boundaries appear identical */
            else if (val <= low)
               binfrac = 0.0;
            else if (val >= high)
               binfrac = 1.0;
            else {
               binfrac = (val - low) / (high - low);

              /*
               * Watch out for the possibility that we got a NaN or
               * Infinity from the division.  This can happen
               * despite the previous checks, if for example "low"
               * is -Infinity.
               */
              if(isnan(binfrac) || binfrac < 0.0 || binfrac > 1.0)
                 binfrac = 0.5;
            }

            /*
             * Now, compute the overall selectivity across the values
             * represented by the histogram.  We have i-1 full bins and
             * binfrac partial bin below the constant.
             */
            histfrac = (double) (i - 1) + binfrac;
            histfrac /= (double) (nvalues - 1);
         }

         /*
          * Now histfrac = fraction of histogram entries below the
          * constant.
          *
          * Account for "<" vs ">"
          */
         hist_selec = isgt ? (1.0 - histfrac) : histfrac;

         /*
          * The histogram boundaries are only approximate to begin with,
          * and may well be out of date anyway.  Therefore, don't believe
          * extremely small or large selectivity estimates.
          */
         if(hist_selec < 0.0001)
            hist_selec = 0.0001;
         else if (hist_selec > 0.9999)
            hist_selec = 0.9999;
      }
      free_attstatsslot(vardata->atttype, values, nvalues, NULL, 0);
   }
   return hist_selec;
}

static double
uintrestrictsel(VariableStatData *vardata, FmgrInfo opproc, Datum constval,
                bool isgt, double (*convert_value)(Datum), double (*convert_stats)(Datum))
{
   Form_pg_statistic stats;
   double selec, mcv_selec, sumcommon, hist_selec;

   /*
    * If we have most-common-values info, add up the fractions of the MCV
    * entries that satisfy MCV OP CONST.  These fractions contribute directly
    * to the result selectivity.  Also add up the total fraction represented
    * by MCV entries.
    */
   mcv_selec = mcv_selectivity(vardata, &opproc, constval, true, &sumcommon);
   hist_selec = uint_histogram_selectivity(vardata, &opproc, isgt, constval,
                                           convert_value, convert_stats);

   stats = (Form_pg_statistic) GETSTRUCT(vardata->statsTuple);

   /*
    * Now merge the results from the MCV and histogram calculations,
    * realizing that the histogram covers only the non-null values that are
    * not listed in MCV.
    */
   selec = 1.0 - stats->stanullfrac - sumcommon;

   if(hist_selec > 0.0)
      selec *= hist_selec;
   else
   {
      /*
       * If no histogram but there are values not accounted for by MCV,
       * arbitrarily assume half of them will match.
       */
      selec *= 0.5;
   }

   selec += mcv_selec;

   /* result should be in range, but make sure... */
   CLAMP_PROBABILITY(selec);
   return selec;
}

Datum
int4ltsel(PG_FUNCTION_ARGS)
{
   PlannerInfo *root = (PlannerInfo *)PG_GETARG_POINTER(0);
   Oid operator = PG_GETARG_OID(1);
   List *args = (List *) PG_GETARG_POINTER(2);
   int varRelid = PG_GETARG_INT32(3);
   VariableStatData vardata;
   Node *other;
   bool varonleft;
   bool isgt = false;
   FmgrInfo opproc;
   Datum constval;
   double selec;

   /*
    * If expression is not variable op something or something op variable,
    * then punt and return a default estimate.
    */
   if(!get_restriction_variable(root, args, varRelid, &vardata, &other, &varonleft)) {
      PG_RETURN_FLOAT8(DEFAULT_INEQ_SEL);
   }

   /*
    * Can't do anything useful if the something is not a constant, either.
    */
   if(!IsA(other, Const)) {
      ReleaseVariableStats(vardata);
      PG_RETURN_FLOAT8(DEFAULT_INEQ_SEL);
   }

   /* Can't do anything useful if stats are not available. */
   if(unlikely(!HeapTupleIsValid(vardata.statsTuple))) {
      ReleaseVariableStats(vardata);
      PG_RETURN_FLOAT8(DEFAULT_INEQ_SEL);
   }

   /*
    * If the constant is NULL, assume operator is strict and return zero.
    * ie. operator will never return TRUE.
    */
   if(((Const *)other)->constisnull) {
       ReleaseVariableStats(vardata);
       PG_RETURN_FLOAT8(0.0);
   }

   /* Force the var to be on the left. */
   if(!varonleft) {
      /* we have other < var, commute to make var > other */
      operator = get_commutator(operator);
      Assert(operator != InvalidOid);
      isgt = true;
   }

   constval = ((Const *)other)->constvalue;
   fmgr_info(get_opcode(operator), &opproc);

   selec = uintrestrictsel(&vardata, opproc, isgt, constval,
                           &convert_uint4_to_double,
                           &convert_int4_to_double);

   ReleaseVariableStats(vardata);
   PG_RETURN_FLOAT8((float8)selec);
}

Datum
int4gtsel(PG_FUNCTION_ARGS)
{
   PlannerInfo *root = (PlannerInfo *)PG_GETARG_POINTER(0);
   Oid operator = PG_GETARG_OID(1);
   List *args = (List *) PG_GETARG_POINTER(2);
   int varRelid = PG_GETARG_INT32(3);
   VariableStatData vardata;
   Node *other;
   bool varonleft;
   bool isgt = true;
   FmgrInfo opproc;
   Datum constval;
   double selec;

   /*
    * If expression is not variable op something or something op variable,
    * then punt and return a default estimate.
    */
   if(!get_restriction_variable(root, args, varRelid, &vardata, &other, &varonleft)) {
      PG_RETURN_FLOAT8(DEFAULT_INEQ_SEL);
   }

   /*
    * Can't do anything useful if the something is not a constant, either.
    */
   if(!IsA(other, Const)) {
      ReleaseVariableStats(vardata);
      PG_RETURN_FLOAT8(DEFAULT_INEQ_SEL);
   }

   /* Can't do anything useful if stats are not available. */
   if(unlikely(!HeapTupleIsValid(vardata.statsTuple))) {
      ReleaseVariableStats(vardata);
      PG_RETURN_FLOAT8(DEFAULT_INEQ_SEL);
   }

   /*
    * If the constant is NULL, assume operator is strict and return zero.
    * ie. operator will never return TRUE.
    */
   if(((Const *)other)->constisnull) {
       ReleaseVariableStats(vardata);
       PG_RETURN_FLOAT8(0.0);
   }

   /* Force the var to be on the left.  */
   if(!varonleft) {
      /* we have other < var, commute to make var > other */
      operator = get_commutator(operator);
      Assert(operator != InvalidOid);
      isgt = false;
   }

   constval = ((Const *)other)->constvalue;
   fmgr_info(get_opcode(operator), &opproc);

   selec = uintrestrictsel(&vardata, opproc, isgt, constval,
                           &convert_uint4_to_double,
                           &convert_int4_to_double);

   ReleaseVariableStats(vardata);
   PG_RETURN_FLOAT8((float8)selec);
}
