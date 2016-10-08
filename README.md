# postgresql-contrib-uint

This implementation is a branch from http://pgfoundry.org/projects/uint/ by [Ryan Bradetich]( http://pgfoundry.org/users/rbrad/).
It provides unsigned 2 byte and unsigned 4 byte data types to postgres 8.1 and greater.

This implementation of **uint** duplicates https://github.com/maciekgajewski/postgres-uints in a way. But atop of that this implementation provides also **uint1**.

## Major changes made so far

- made compatible with 9.x;
- made compatible with >= 9.3.x;
- added support for **CREATE EXTENSION**;
- retested with PostgreSQL 9.5 and 9.6

## Installation
```
PATH=<path to your PostgreSQL bin with **pg_config**>:$PATH; make && make install
```

## Usage

```
postgres=# CREATE EXTENSION uint;
CREATE EXTENSION

postgres=# CREATE TABLE utable (id uint4, val8bit uint1);
CREATE TABLE
postgres=# INSERT INTO utable VALUES (1, 2), (3, 4), (5, 6);
INSERT 0 3
postgres=# SELECT * FROM utable;
 id | val8bit 
----+---------
 1  | 2
 3  | 4
 5  | 6
(3 rows)

postgres=# \d utable 
    Table "public.utable"
 Column  | Type  | Modifiers 
---------+-------+-----------
 id      | uint4 | 
 val8bit | uint1 | 
```
