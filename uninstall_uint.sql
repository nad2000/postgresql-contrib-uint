BEGIN WORK;

-- Adjust this setting to control where the objects get dropped.
SET search_path = public;

DROP OPERATOR FAMILY unsigned_integer_ops USING btree CASCADE;

DROP TYPE uint1 CASCADE;
DROP TYPE uint2 CASCADE;
DROP TYPE uint4 CASCADE;

COMMIT;
