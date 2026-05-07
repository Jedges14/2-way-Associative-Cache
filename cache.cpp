/**
 * Cache of 1kb size 
 * block size 16bytes
 * 
 * @author Jedges Gyasi
 * @date 5/7/2026
 */

#include <iostream>
#include <string>

/**
 * Cache line for the cache 
 * @param valid determines if line is empty or filled with data
 * @param tag unique bit match for the data block
 * @param dirty if the block has been modified and not stored in RAM 
 * @param data actual 16byte block size
 */
struct line{
    bool valid = false;
    unsigned int tag;
    bool dirty = false;
    uint8_t data[16] = {0};
};