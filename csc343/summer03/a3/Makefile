# A makefile to compile embeded SQL C programs
#
# input: a3.sqc 
# output: a3  (executable codes)
#

DB2DIR = /usr/IBMdb2/V7.1
Cflags = -I $(DB2DIR)/include \
         -L $(DB2DIR)/lib \
         -ldb2

all: a3

# The first thing is to connect to the database and precompile the programs.
# This generates C programs where all SQL commands (except dynamic SQL
# queries inside dyn-select.sqc) are replaced by subroutine calls.
# 

a3.c: a3.sqc
	db2 connect to ${DB2DATABASE}
	db2 prep a3.sqc
	db2 connect reset

# The next step is to compile the C programs and
# generate the executable codes.


a3: a3.c
	gcc -Wall $(Cflags) -o a3 a3.c







