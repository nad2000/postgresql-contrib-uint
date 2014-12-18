postgresql-contrib-uint
=======================

This implementation is a branch from http://pgfoundry.org/projects/uint/ by [Ryan Bradetich]( http://pgfoundry.org/users/rbrad/).
It provides unsigned 2 byte and unsigned 4 byte data types to postgres 8.1 and greater.

This implementation of **uint** duplicates https://github.com/maciekgajewski/postgres-uints in a way. But in addition this implementation provides **uint1** as well.

Major changes made so far:

- made compatible with 9.x;
- made compatible with >= 9.3.x;
- added support for **CREATE EXTENSION**;

