#include "cachehead.h"


void cacheSet :: tagMatch(unsigned int inTag) {
    if (line0.valid == true && line0.tag == inTag) {foundline == 0; LRU = 1;}
    if (line1.valid == true && line1.tag == inTag) {foundline == 1; LRU = 0;}
    else{foundline = -1;}

}

void cacheSet :: evict(unsigned int newTag, uint8_t newdata[16]){
    if (line0.valid && line1.valid){
        if (LRU == 0){
            line0 = {0, newTag, 0, newdata[16]};
            LRU = 1;
        }
        else if (LRU == 1){
            line1 = {0, newTag, 0, newdata[16]};
            LRU = 0;
        }
    }
    else if (!line0.valid){
        line0 = {0, newTag, 0, newdata[16]};
        LRU = 1;
    }
    else if (!line1.valid){
        line1 = {0, newTag, 0, newdata[16]};
        LRU = 0;
    }
}