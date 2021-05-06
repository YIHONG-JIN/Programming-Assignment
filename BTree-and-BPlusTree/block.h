#ifndef BLOCK_H
#define BLOCK_H
#include <cstring>
#include <iostream>
using namespace std;

class Person
{
public:
    Person() {}
    Person(const Person &src) { ID = src.getID(); }
    int setID(string data)
    {
        ID = data;
        return 0;
    }
    string getID(void) const { return ID; }

    // temp function !!!
private:
    string ID;
};

// ---------------------------------
/**
 * @brief The storage unit of block.
 * 
 */
class record
{
private:
    int tombstone;

public:
    Person datum;
    /**
     * @brief Construct a new record object.
     * 
     */
    record() : tombstone{1},
               datum{} {};
    int mark_tombstone(void)
    {
        tombstone = 1;
        return 0;
    }
    int unmark_tombstone(void)
    {
        tombstone = 0;
        return 0;
    }
    int get_tombstone(void) const { return tombstone; }
    string get_key(void) const { return datum.getID(); }
};

/**
 * @brief Use ID as primary key. Assumption: all tuple has unique ID. 
 * 
 */
class block
{
private:
    int mainblock_occupied = 0;
    int overflow_occupied = 0;
    // tombstones_number indicate the number of tombstones in mainblock
    int tombstones_number = 0;
    // fill factor between 1/2 and 2/3. Choose 6/10 after testing.
    // fill_threshold + overflow_size <= mainblock_size
    int const fill_threshold = 7;
    // Choose 1/10 after testing.
    int const merge_threshold = 2;
    // mainblock_size = 10;
    int const mainblock_size = 10;
    record mainblock[10];

    // overflow_size = 3. Choose 1/10 after testing.
    int const overflow_size = 3;
    record overflow[3];

    block *prev = NULL;
    block *next = NULL;

public:
    int clear();
    int sort(void);
    Person *find(string ID);
    block *insert(Person *tuple);
    block *split(void);
    block *remove(string ID);
    block *merge(void);
    block *nextPointer();
    block *prevPointer();
    string maximum(void);
    int display(void);
};
// ---------------------------------

/**
 * @brief clear all the date in the block by marking all tuples' tombstones.
 * Set mainblock_occupied = 0, overflow_occupied = 0, tombstones_number = 0.
 * 
 * @return 1 for indication
 */

#endif