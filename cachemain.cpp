#include "cachehead.h"


int cacheSet :: tagMatch(unsigned int inTag) {
    if (line0.valid == true && line0.tag == inTag) { LRU = 1; return 0; }
    if (line1.valid == true && line1.tag == inTag) { LRU = 0; return 1;}
    return -1;
}

void cacheSet :: evict(unsigned int newTag, array<uint8_t,16> newdata){
    if (line0.valid && line1.valid){
        if (LRU == 0){
            line0 = {1, newTag, 0, newdata};
            LRU = 1;
        }
        else if (LRU == 1){
            line1 = {1, newTag, 0, newdata};
            LRU = 0;
        }
    }
    else if (!line0.valid){
        line0 = {1, newTag, 0, newdata};
        LRU = 1;
    }
    else if (!line1.valid){
        line1 = {1, newTag, 0, newdata};
        LRU = 0;
    }
}



int Cache :: access(uint16_t address){
    uint8_t tag = address >> 9 ; //top 7 bits
    uint8_t index = (address>>4) & 0x01F; //5 bits between tag and offset
    uint8_t offset = address & 0x0F; //offset of 4bits

    cacheSet foundSet = setArray[index]; //since it will be valid and index in 0-31

    int result = foundSet.tagMatch(tag); //check for tag in set
    line loc;
    uint32_t word = 0;

    //currently assuming cpu is word addressed
    if (result == 0){ //found at line0
        loc = foundSet.line0;
        word |= (loc.data[offset] << 24);
        word |= ((loc.data[offset]+1) << 16);
        word |= ((loc.data[offset]+2) << 8);
        word |= ((loc.data[offset]+3) );

        return word;

    }
    else  if (result == 1){ //found at line0
        loc = foundSet.line1;
        word |= (loc.data[offset] << 24);
        word |= ((loc.data[offset]+1) << 16);
        word |= ((loc.data[offset]+2) << 8);
        word |= ((loc.data[offset]+3) );

        return word;

    }
    else{
        foundSet.evict()
    }
    
}