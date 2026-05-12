#include "cachehead.h"


int cacheSet :: tagMatch(unsigned int inTag) {
    if (line0.valid == true && line0.tag == inTag) { LRU = 1; return 0; }
    if (line1.valid == true && line1.tag == inTag) { LRU = 0; return 1;}
    return -1;
}

uint16_t cacheSet :: addressCompile( unsigned int index, line& l){
    uint16_t tagP = (l.tag) << 9;
    uint16_t indexP =  index<< 4;

    return tagP | indexP; //this is already block aligned
}


void cacheSet::insert(unsigned int newTag, array<uint8_t,16> newdata){
    if (!line0.valid){
        line0 = {1, newTag, 0, newdata};
        LRU = 1;
    }
    else if (!line1.valid){
        line1 = {1, newTag, 0, newdata};
        LRU = 0;
    }
}

void cacheSet :: evict(uint8_t index){
    if (line0.valid && line1.valid){
        if (LRU == 0){
            if (line0.dirty){
                //write back to ram for now will update later
                uint16_t addr = addressCompile(index, line0);
                ram.writeback(addr, line0.data);
            }
            line0 = {};
        }
        else{
            if (line1.dirty){
                uint16_t addr = addressCompile(index, line1);
                ram.writeback(addr, line1.data);
            }
                //write back to ram for now will update later
            line1 = {};
        }
            
    }
}



int Cache :: access(uint16_t address){
    uint8_t tag = address >> 9 ; //top 7 bits
    uint8_t index = (address>>4) & 0x01F; //5 bits between tag and offset
    uint8_t offset = address & 0x0F; //offset of 4bits

    cacheSet& foundSet = setArray[index]; //since it will be valid and index in 0-31

    int result = foundSet.tagMatch(tag); //check for tag in set
    line loc;
   

    //currently assuming cpu is word addressed, will modular
    if (result == 0){ //found at line0
        loc = foundSet.line0;
        assert(offset <= 12);
        return (loc.data[offset] << 24) |
                ((uint32_t)loc.data[offset+1] << 16)|
                ((uint32_t)loc.data[offset+2] << 8) |
                ((uint32_t)loc.data[offset+3]);
    }

    else  if (result == 1){ //found at line0
        loc = foundSet.line1;
        assert(offset <= 12);
        return (loc.data[offset] << 24) |
                ((uint32_t)loc.data[offset+1] << 16)|
                ((uint32_t)loc.data[offset+2] << 8) |
                ((uint32_t)loc.data[offset+3]);
    }

    else if(result == -1){
        array<uint8_t, 16> newData = ram.fetchFromRAM(address);
        foundSet.evict(index);
        foundSet.insert(tag, newData);
    } 
}

array<uint8_t, 16> RAM :: fetchFromRAM(uint16_t address){
    uint16_t alignedaddress = address & 0xFFF0; //zero out offset since we want entirely 16bytes block
    array<uint8_t, 16> dataBlock;

    for(int i = 0; i<16; i++){
        dataBlock[i] = storage[alignedaddress + i];
    }
    return dataBlock;
}


void RAM :: writeback(uint16_t addr, array<uint8_t,16> data){
    for(int i = 0; i<16; i++){
        storage[addr + i] = data[i];
    }

}