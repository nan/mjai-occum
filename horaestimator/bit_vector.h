#ifndef mjaiC___bit_vector_h
#define mjaiC___bit_vector_h
#include <cmath>

class BitVector
{
public:
    bool data[5][34];
    BitVector()
    {
        memset(data, false, sizeof(data));
    }
    BitVector(int array[])
    {
        memset(data, false, sizeof(data));
        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < 34; i++) {
                data[j][i] = array[i] > j;
            }
        }
    }
    
    bool isSubsetOf(BitVector other)
    {
        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < 34; i++) {
                if ((data[j][i] & other.data[j][i]) != data[j][i]) {
                    return false;
                }
            }
        }
        return true;
    }
    
    bool checkTenpai(BitVector other)
    {
        bool oneMiss = false;
        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < 34; i++) {
                if ((data[j][i] & other.data[j][i]) != data[j][i]) {
                    if(oneMiss) {
                        return false;
                    }
                    oneMiss = true;
                }
            }
        }
        return true;
    }
    
    bool hasIntersectionWith(BitVector other)
    {
        for (int j = 0; j < 5; j++) {
            for (int i = 0; i < 34; i++) {
                if ((data[j][i] & other.data[j][i]) != 0) {
                    return true;
                }
            }
        }
        return false;
    }
};

#endif
