#ifndef mjaiC___furo_h
#define mjaiC___furo_h

#include <vector>
#include <string>
#include <algorithm>
#include "pai.h"

class Furo
{
public:
    std::string type;
    int target;
    Pai taken;
    std::vector<Pai> consumed;
    Furo(std::string _type, Pai _taken, int _target, std::vector<Pai> _consumed)
    {
        taken = _taken;
        type = _type;
        target = _target;
        consumed = _consumed;
    }
    std::vector<Pai> pais()
    {
        std::vector<Pai> ret = consumed;
        ret.push_back(taken);
        return ret;
    }
    int hasRed()
    {
        int ret = 0;
        if (taken.red) {
            ret++;
        }
        std::vector<Pai> hais = consumed;
        for (int i = 0; i < hais.size(); i++) {
            if (hais[i].red) {
                ret++;
            }
        }
        return ret;
    }
    
    Pai smallestPai() {
        Pai ret = taken;
        for (int i = 0; i < consumed.size(); i++) {
            if (ret.identifier > consumed[i].identifier) {
                ret = consumed[i];
            }
        }
        return ret;
    }
    
    std::string toString() {
        return type + "." + taken.toString() + " ";
    }
};

#endif
