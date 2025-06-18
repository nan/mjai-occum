#ifndef mjaiC___shanten_analysis_h
#define mjaiC___shanten_analysis_h

#include "./bit_vector.h"
#include "./furo.h"
#include "./pai.h"
#include "./pai_set.h"
#include "./util.h"
#include <algorithm>
#include <string>
#include <vector>

#define INF 9999
#define CHOWS_LEN 21

#define SELF_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define SELF_MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef struct {
    std::string type;
    std::vector<Pai> pids;
} Mentu;

class GOALS {
public:
    int fu;
    int fan;
    int points;
    int shanten;
    int chitoitu_shanten;
    int kokushi_shanten;
    int countVector[NUM_IDS];
    std::vector<Mentu> mentsus;
    int requiredVector[NUM_IDS];
    int throwableVector[NUM_IDS]; //打牌可能な牌
    std::vector<Furo> furos;
    std::string yaku[20];
    //牌のmulti setを表すデータ構造の1つ。bitVectors[i][pai.id] = (count(pai)/>i)
    //となるようなBitVectorの配列。
    BitVector requiredBitVectors;
    
    GOALS() {
        memset(countVector, 0, sizeof(countVector));
        memset(requiredVector, 0, sizeof(requiredVector));
        memset(throwableVector, 0, sizeof(throwableVector));
        points = shanten = 0;
        chitoitu_shanten = kokushi_shanten = INF;
    }
    
    int minimumShanten() {
        return min(min(shanten, chitoitu_shanten), kokushi_shanten);
    }
    
    bool equal(GOALS g) {
        for (int i = 0; i < NUM_IDS; i++) {
            if (countVector[i] != g.countVector[i]) {
                return false;
            }
        }
        return true;
    }
    
    std::string toStringMentsuOnly() {
        char s[1024];
        
        std::string mentsus_s = "";
        for (Mentu m : mentsus) {
            std::string pais_s = "";
            for (Pai p : m.pids) {
                pais_s += p.toString() + " ";
            }
            mentsus_s += m.type + ", pai:" + pais_s + ":";
        }
        
        sprintf(s, " mentsu = %s ", mentsus_s.c_str());
        return s;
    }
    
    std::string toString() {
        char s[1024];
        
        std::string mentsus_s = "";
        for (Mentu m : mentsus) {
            std::string pais_s = "";
            for (Pai p : m.pids) {
                pais_s += p.toString() + " ";
            }
            mentsus_s += m.type + ", pai:" + pais_s + ":";
        }
        
        std::string furos_s = "";
        for (Furo f : furos) {
            furos_s += f.type + "" + f.taken.toString();
        }
        
        std::string yaku_s = "";
        for (std::string y : yaku) {
            if (y != "") {
                yaku_s += y;
            }
        }
        
        std::string countVector_s = "";
        std::string requiredVector_s = "";
        std::string throwableVector_s = "";
        for (int i = 0; i < NUM_IDS; i++) {
            char s2[10];
            sprintf(s2, "%d:%d ", i, countVector[i]);
            countVector_s += s2;
            sprintf(s2, "%d:%d ", i, requiredVector[i]);
            requiredVector_s += s2;
            sprintf(s2, "%d:%d ", i, throwableVector[i]);
            throwableVector_s += s2;
        }
        
        sprintf(s, "----------\n fu = %d \n fan = %d \n points = %d \n shanten = "
                "%d \n chitoitu_shanten = %d \n mentsu = %s \n furos_s = %s \n "
                "yaku = %s \n contVector      = %s \n requiredVector  = %s \n "
                "throwableVector = %s \n",
                fu, fan, points, shanten, chitoitu_shanten, mentsus_s.c_str(),
                furos_s.c_str(), yaku_s.c_str(), countVector_s.c_str(),
                requiredVector_s.c_str(), throwableVector_s.c_str());
        return s;
    }
};

class ShantenAnalysis {
public:
    int allowedExtraPais;
    int thisUpperbound;
    int currentVector[NUM_IDS];
    int targetVector[NUM_IDS];
    int shantenVector[NUM_IDS];
    int chitoiShantenVector[NUM_IDS];
    int shanten;
    int chitoitu_shanten;
    int kokushi_shanten;
    int numMentsus;
    int CHOWS[CHOWS_LEN];
    
    std::vector<GOALS> goals;
    
    ShantenAnalysis() {}
    ShantenAnalysis(std::vector<Pai> pids, int _allowedExtraPais = 0,
                    int upperbound = INF + 1) {
        
        kokushi_shanten = chitoitu_shanten = 999;
        
        int _CHOWS[CHOWS_LEN] = {0,  1,  2,  3,  4,  5,  6,  9,  10, 11, 12,
            13, 14, 15, 18, 19, 20, 21, 22, 23, 24};
        for (int i = 0; i < CHOWS_LEN; i++) {
            CHOWS[i] = _CHOWS[i];
        }
        
        allowedExtraPais = _allowedExtraPais;
        thisUpperbound = INF;
        pidToCountVector(pids);
        memset(targetVector, 0, sizeof(targetVector));
        memset(shantenVector, 999, sizeof(shantenVector));
        memset(chitoiShantenVector, 999, sizeof(chitoiShantenVector));
        numMentsus = (int)pids.size() / 3;
        goals.clear();
        std::vector<GOALS> localGoals;
        std::vector<Mentu> localMentsus;
        
        shanten = calculateShantensuInternal(currentVector, targetVector, -1,
                                             numMentsus, 0, thisUpperbound,
                                             localMentsus, &localGoals);
        
        thisUpperbound = upperbound != INF + 1
        ? upperbound
        : SELF_MIN(shanten + allowedExtraPais, thisUpperbound);
        for (int i = 0; i < localGoals.size(); i++) {
            if (localGoals[i].shanten <= thisUpperbound
                //&& !alreadyExistGoalInGoals(localGoals[i])
                ) {
                
                for (int pid = 0; pid < NUM_IDS; pid++) {
                    
                    localGoals[i].requiredVector[pid] =
                    SELF_MAX(localGoals[i].countVector[pid] - currentVector[pid], 0);
                    localGoals[i].throwableVector[pid] =
                    SELF_MAX(currentVector[pid] - localGoals[i].countVector[pid], 0);
                }
                
                goals.push_back(localGoals[i]);
            }
        }
        
        shanten = goals.size() == 0 ? INF : shanten;
        
        goalsLog();
    }
    
    void goalsLog() {
        //    for (int i = 0; i < goals.size(); i++) {
        //      std::cout << goals[i].toStringMentsuOnly() << std::endl;
        //      // std::cout << goals[i].toString() << std::endl;
        //    }
    }
    
    bool alreadyExistGoalInGoals(GOALS g) {
        // 同じやつはチェック
        for (int i = 0; i < goals.size(); i++) {
            if (goals[i].equal(g))
                return true;
        }
        return false;
    }
    
    void pidToCountVector(std::vector<Pai> pids) {
        memset(currentVector, 0, sizeof(currentVector));
        for (int i = 0; i < pids.size(); i++) {
            currentVector[pids[i].identifier]++;
        }
    }
    int calculateShantensuInternal(int vector0[], int vector1[], int current,
                                   int numMeldsLeft, int minMeldId,
                                   int upperbound, std::vector<Mentu> mentsus,
                                   std::vector<GOALS> *goals) {
        if (numMeldsLeft == 0) {
            // int minDelta = 2;
            int newShanten = 0;
            for (int i = 0; i < NUM_IDS; i++) {
                
                int delta = SELF_MAX((vector1[i] + 2) - vector0[i], 0);
                
                if (delta >= 2)
                    continue;
                newShanten = current + delta;
                
                if (newShanten <= upperbound + allowedExtraPais) {
                    int goalVector[NUM_IDS];
                    memcpy(goalVector, vector1, sizeof(goalVector));
                    goalVector[i] += 2;
                    GOALS willAddGoal;
                    Mentu m;
                    m.pids.clear();
                    m.pids.push_back(Pai(i));
                    m.pids.push_back(Pai(i));
                    m.type = "toitsu";
                    willAddGoal.mentsus = mentsus;
                    willAddGoal.mentsus.push_back(m);
                    willAddGoal.shanten = newShanten;
                    memcpy(willAddGoal.countVector, goalVector,
                           sizeof(willAddGoal.countVector));
                    
                    goals->push_back(willAddGoal);
                    if (newShanten < upperbound)
                        upperbound = newShanten;
                    shantenVector[i] = newShanten;
                }
            }
            return upperbound;
        } else {
            if (minMeldId < NUM_IDS) {
                for (int i = minMeldId; i < NUM_IDS; i++) {
                    int current1 = current;
                 
                    if (vector1[i] >= 2) {
                        continue;
                    }
                    if (vector0[i] <= vector1[i]) {
                        current1 = current + 3;
                    } else if (vector0[i] < vector1[i] + 3) {
                        current1 = current + (vector1[i] + 3) - vector0[i];
                    } else {
                        current1 = current;
                    }
                    if (current1 < current + 3 &&
                        current1 <= upperbound + allowedExtraPais
                        && vector0[i] >= 2//手持ちが1枚しかないのにアンコは評価しない
                        ) {
                        vector1[i] += 3;
                        std::vector<Mentu> mentsusForArgs = mentsus;
                        Mentu willAddMentu;
                        willAddMentu.type = "kotsu";
                        willAddMentu.pids.push_back(Pai(i));
                        willAddMentu.pids.push_back(Pai(i));
                        willAddMentu.pids.push_back(Pai(i));
                        mentsusForArgs.push_back(willAddMentu);
                        upperbound = calculateShantensuInternal(
                                                                vector0, vector1, current1, numMeldsLeft - 1, i, upperbound,
                                                                mentsusForArgs, goals);
                        vector1[i] -= 3;
                    }
                }
            }
            
            int startChowId = minMeldId < NUM_IDS ? 0 : minMeldId - NUM_IDS;
            
            for (int chowid = startChowId; chowid < CHOWS_LEN; chowid++) {
                int i = CHOWS[chowid];
                if (vector1[i] >= 4 || vector1[i + 1] >= 4 || vector1[i + 2] >= 4) {
                    continue;
                }
                
                int current1 = current;
                if (vector0[i] <= vector1[i]) {
                    ++current1;
                }
                if (vector0[i + 1] <= vector1[i + 1]) {
                    ++current1;
                }
                if (vector0[i + 2] <= vector1[i + 2]) {
                    ++current1;
                }
                //手持ちが1枚しかないのにシュンツは評価しない
                int chkKoritsu = vector0[i] + vector0[i+1] + vector0[i+2];
                
                if (current1 < current + 3 &&
                    current1 <= upperbound + allowedExtraPais &&
                    chkKoritsu >= 2) {
                    ++vector1[i];
                    ++vector1[i + 1];
                    ++vector1[i + 2];
                    std::vector<Mentu> mentsusForArgs = mentsus;
                    Mentu willAddMentu;
                    willAddMentu.type = "shuntsu";
                    willAddMentu.pids.push_back(Pai(i));
                    willAddMentu.pids.push_back(Pai(i + 1));
                    willAddMentu.pids.push_back(Pai(i + 2));
                    mentsusForArgs.push_back(willAddMentu);
                    upperbound = calculateShantensuInternal(
                                                            vector0, vector1, current1, numMeldsLeft - 1, chowid + NUM_IDS,
                                                            upperbound, mentsusForArgs, goals);
                    --vector1[i];
                    --vector1[i + 1];
                    --vector1[i + 2];
                }
            }
            return upperbound;
        }
    }
    
    int countVec1() {
        int ret = 0;
        for (int i = 0; i < NUM_IDS; i++) {
            ret += targetVector[i];
        }
        return ret;
    }
};

#endif
