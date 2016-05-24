# MinS-DBMS
Build own Database system. 

# Intro to Min's DBMS
 - Building the SQL database system by C language.
 - Read in `books.txt` and `sellRecord.txt` for the resource file.

# Data Structure
 - Using user-defined 2D-dimensional matrix of linked-list `buckets_Header` to keep the data we retrieve from resource file , and based on hash tables to store.
  - buckets_Header[`hash33(Attributes_Name)`][`hash33(Attributes_value)`]
 - And using 3 user-defined type linked-list to maintain the clause of `SELECT` , `FROM` , `WHERE`

# Function of this DBMS (Currently support)
 - `SELECT * FROM table1`  
  - select out all the attribute from the table
 - `SELECT * FROM table1 WHERE (table1)attri = value` 
  - select out all the attribute from the table , and you can using WHERE to choose which attribute you want.
 - `SELECT * FROM table1 JOIN table2 ON table1_attri = table2_attri`
  - select out the table with the join result.
  - (`SELECT * FROM table2 JOIN table1 ON table2_attri = table1_attri` isn't supported.)
 - `SELECT * FROM table1 JOIN table2 ON table1_attri = table2_attri WHERE xxx=yyy`
  - select out the attribute that fullfill the condition "xxx=yyy".
 - `SELECT DISTINCT attriName FROM attriName's Table`
  - remove the same attributes choosing by user in that tables.


# Reference
Learn Makefile:
###### $@ => get the first string(":"'s left side)
###### $< => get the first string(":"'s right side)
###### $^ => get all left side string
