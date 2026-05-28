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
    unsigned int tag = 0;
    bool dirty = 0;
    array<uint8_t,16> data = {};
};


/**
 * RAM data strucute implemented as 65536bytes (* 8 bits) since there address is 16bits so stroage is (2^16)
 * 
 * 
 */
struct RAM{
    uint8_t storage[65536] = {}; //2^16 memory address locations
    array<uint8_t, 16> fetchFromRAM(uint16_t address);
    void writeFromRAM(uint16_t addr, array<uint8_t,16> data);
};


/**
 * 2 way associative set containing two lines
 * 
 */
struct cacheSet {
    line line0; //first line of cache set
    line line1; // second line of cache set
    RAM& ram;
    bool LRU = 0; // LRU=1 line1 is least recent and line0 is most recent and vice versa

    cacheSet(RAM& r) : ram(r) {}  //cacheset constructor
    /**
     * checks if incoming tag is present in either of the lines
     * @param inTag incoming tag from the cpu decoded in the address put on bus
     */
    int tagMatch(unsigned int inTag){};
    uint16_t addressCompile( unsigned int index, line& l){};
    /**
     * Verifies for hits or misses and either fetch from memory to overwrite valid line and write to ram if modified or not
     * @param newTag tag of address line fetched from memory
     * @param newdata block fetched from memory
     */
    void evict(uint8_t index){};
   
    void insert(unsigned int newTag, array<uint8_t,16> newdata){};    
};

/**
 * 
 * CPU facinge stuct of the acutal cache. has access methods to search and find cache lines if present in cache
 * or fetch from ram if there is a miss
 * 
 */
struct Cache{
    /**
     * vector storage for caches
     */
    vector<cacheSet> setArray; 
    RAM& ram;

    Cache(RAM& r) : ram(r) {
        for (int i = 0; i < 32; i++) {
            setArray.emplace_back(r);  // construct each cacheSet with RAM& bound
        }
    }
    
    int access(uint16_t address){}; 

    void write(uint16_t address, uint16_t value){}; 
};

static void  addsplitter(uint16_t address, uint8_t& tag, uint8_t& index, uint8_t& offset){
    tag = address >> 9 ; //top 7 bits
    index = (address>>4) & 0x01F; //5 bits between tag and offset
    offset = address & 0x0F; //offset of 4bits)
}
#endif


