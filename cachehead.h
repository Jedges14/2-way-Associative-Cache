/**
 * Cache of 1kb size 
 * block size 16bytes
 * 
 * @author Jedges Gyasi
 * @date 5/7/2026
 */
#ifndef MYLINE_H
#define MYLINE_H
#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <assert.h>
using namespace std;

/**
 * Cache line for the cache 
 * @param valid Determines if line is empty or filled with data
 * @param tag Unique bit match for the data block
 * @param dirty If the block has been modified and not stored in RAM 
 * @param data Actual 16byte block size
 */
struct line{
    bool valid = 0;
    unsigned int tag;
    bool dirty = 0;
    array<uint8_t,16> data = {};
};

/**
 * 2 way associative set containing two lines
 * 
 */
struct cacheSet {
    line line0; //first line of cache set
    line line1; // second line of cache set
    /**
     * checks if incoming tag is present in either of the lines
     * @param inTag incoming tag from the cpu decoded in the address put on bus
     */
    int tagMatch(unsigned int inTag){};
    uint16_t addressCompile( unsigned int index, line& l){};
    bool LRU = 0; // LRU=1 line1 is least recent and line0 is most recent and vice versa
    /**
     * Verifies for hits or misses and either fetch from memory to overwrite valid line and write to ram if modified or not
     * @param newTag tag of address line fetched from memory
     * @param newdata block fetched from memory
     */
    void evict(uint8_t index){};
   
    void insert(unsigned int newTag, array<uint8_t,16> newdata){};
    RAM& ram;
};


struct Cache{
    vector<cacheSet, int> setArray;
    int access(uint16_t address){};
    RAM& ram;
};


struct RAM{
    uint8_t storage[65536]; //2^16 memory address locations
    array<uint8_t, 16> fetchFromRAM(uint16_t address){};
    void writeback(uint16_t addr, array<uint8_t,16> data){};
};
#endif


