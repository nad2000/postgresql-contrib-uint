# postgresql-contrib-uint [![Build Status](https://travis-ci.org/nad2000/postgresql-contrib-uint.svg?branch=master)](https://travis-ci.org/nad2000/postgresql-contrib-uint)

This implementation is a branch from http://pgfoundry.org/projects/uint/ by [Ryan Bradetich]( http://pgfoundry.org/users/rbrad/).
It provides unsigned 2 byte and unsigned 4 byte data types to postgres 8.1 and greater.

This implementation of **uint** is in a way similar to  https://github.com/maciekgajewski/postgres-uints. But atop of that this implementation provides also **uint1**.

## Major changes made so far

- *2022-11-07*: made compatible with 13.x, 14.x, 15.x;
- *2020-03-08*: made compatible with 9.x, 10.x, 11.x, 12.x;
- added automated testing and CI (*TravisCI*): https://travis-ci.org/nad2000/postgresql-contrib-uint.
- dockerized and published at https://cloud.docker.com/u/nad2000/repository/docker/nad2000/postgres-uint;
- retested with PostgreSQL 9.5 and 9.6
- added support for **CREATE EXTENSION**;
- made compatible with >= 9.3.x;
- made compatible with 9.x;

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
