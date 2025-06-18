#ifndef mjaiC___pai_set_h
#define mjaiC___pai_set_h

#include "./pai.h"
#include <string>
#include <vector>

class PaiSet {
public:
    int array[NUM_IDS];
    PaiSet() {
        for (int i = 0; i < NUM_IDS; i++) {
            array[i] = 0;
        }
    }
    PaiSet(int pais[]) {
        for (int i = 0; i < NUM_IDS; i++) {
            array[i] = pais[i];
        }
    }
    PaiSet(std::vector<Pai> pais) {
        for (int i = 0; i < NUM_IDS; i++) {
            array[i] = 0;
        }
        if (pais.size() > 0) {
            addPais(pais);
        }
    }
    int length() {
        int len = 0;
        for (int i = 0; i < NUM_IDS; i++) {
            len += array[i];
        }
        return len;
    }
    std::vector<Pai> toPais() {
        std::vector<Pai> ret;
        for (int i = 0; i < NUM_IDS; i++) {
            for (int j = 0; j < array[i]; j++) {
                ret.push_back(Pai(i));
            }
        }
        return ret;
    }
    int count(Pai pai) { return array[pai.identifier]; }
    bool has(Pai n) { return array[n.identifier] > 0; }
    void addPai(Pai pai, int n) { array[pai.identifier] += n; }
    void addPais(std::vector<Pai> pais) {
        for (int i = 0; i < pais.size(); i++) {
            array[pais[i].identifier]++;
        }
    }
    void removePaiset(PaiSet paiset) {
        int otherArray[NUM_IDS];
        memcpy(otherArray, paiset.array, sizeof(otherArray));
        for (int i = 0; i < NUM_IDS; i++) {
            array[i] -= otherArray[i];
            if (array[i] < 0) {
                array[i] = 0;
            }
        }
    }
    
    std::string toString() {
        std::vector<Pai> _topais = toPais();
        std::string ret = "";
        for (int i = 0; i < _topais.size(); i++) {
            ret += _topais[i].toString() + " ";
        }
        return ret;
    }
    static PaiSet getAll() {
        int tmpAry[NUM_IDS];
        for (int i = 0; i < NUM_IDS; i++) {
            tmpAry[i] = 4;
        }
        return PaiSet(tmpAry);
    }
    void clear() { memset(array, 0, sizeof(array)); }
    int getIsolatedPai() {
        int isolate = 0;
        for (int i = 0; i < NUM_IDS; i++) {
            if (array[i] && isIsolated(i)) {
                isolate++;
            }
        }
        return isolate;
    }
    bool exsistIsolatedPai() {
        for (int i = 0; i < NUM_IDS; i++) {
            if (array[i] && isIsolated(i)) {
                return true;
            }
        }
        return false;
    }
    bool isIsolated(int identifier) {
        if (array[identifier] != 1) {
            return false;
        }
        if (identifier >= Pai("E").identifier) {
            return true;
        }
        if (identifier == Pai("1m").identifier ||
            identifier == Pai("1p").identifier ||
            identifier == Pai("1s").identifier) {
            if (!array[identifier + 1] && !array[identifier + 2]) {
                return true;
            }
        }
        if (identifier == Pai("9m").identifier ||
            identifier == Pai("9p").identifier ||
            identifier == Pai("9s").identifier) {
            if (!array[identifier - 1] && !array[identifier - 2]) {
                return true;
            }
        }
        if (identifier == Pai("2m").identifier ||
            identifier == Pai("2p").identifier ||
            identifier == Pai("2s").identifier) {
            if (!array[identifier - 1] && !array[identifier + 1] &&
                !array[identifier + 2]) {
                return true;
            }
        }
        if (identifier == Pai("8m").identifier ||
            identifier == Pai("8p").identifier ||
            identifier == Pai("8s").identifier) {
            if (!array[identifier + 1] && !array[identifier - 1] &&
                !array[identifier - 2]) {
                return true;
            }
        }
        if (!array[identifier - 1] && !array[identifier + 1] &&
            !array[identifier - 2] && !array[identifier + 2]) {
            return true;
        }
        return false;
    }
    std::vector<Pai> enumlateIsolatedPais() {
        std::vector<Pai> ret;
        for (int i = 0; i < NUM_IDS; i++) {
            if (array[i] && isIsolated(i)) {
                ret.push_back(i);
            }
        }
        return ret;
    }
};

#endif
