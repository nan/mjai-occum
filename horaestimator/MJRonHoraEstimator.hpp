#ifndef MJRONHoraEstimator_hpp
#define MJRONHoraEstimator_hpp

#include "../OpponentHoraEstimator/OpponentHoraEstimator.hpp"
#include "MJHoraEstimator.hpp"
#include "pai.h"

// ロンあがり率等計算クラス（一人麻雀）
// モンテカルロ法のツモをロンあがりに対応させるクラス
// TODO
class MJRonHoraEstimator {
    
public:
    static std::vector<Pai>
    randomeTsumoYamaIfBetaori(std::vector<Pai> invisiblePaiSetVec,
                              OpponentHoraEstimator estimator) {
        std::vector<Pai> ret;
/*
        std::vector<Pai> _invisiblePaiSetVec = invisiblePaiSetVec;
        // 残り牌牌列
        int nokoriPaiVec[38] = {0};
        for (Pai p : _invisiblePaiSetVec) {
            nokoriPaiVec[Util::convertMJAIPainumToMjscore(p.identifier)]++;
        }
        
        //        // TEST
        //        for(int i=0;i<30;i++) {
        //            if(i%10==0) continue;
        //            if(nokoriPaiVec[i] >1) nokoriPaiVec[i]--;
        //        }
        
        // estimatorは対面からみた値が入っている
        // なので1が自分の番号になっている前提
        // 放銃率が重みとなる
        int distribution[38] = {0};
        int max = 0;
        double max_element_hojuProb = -1;
        
        for (double d : estimator.hojuProb[1]) {
            if (d > max_element_hojuProb)
                max_element_hojuProb = d;
        }
        
        for (int j = 0; j < OpponentHoraEstimator_NUMOF_TEHAI; j++) {
            
            if (j % 10 == 0)
                continue;
            
            // 少数2桁の整数にする
            // 例:0.12なら12
            double prob = max_element_hojuProb - estimator.hojuProb[1][j];
            prob *= 100;
            int count = (int)prob;
            distribution[j] = count;
            if (max < count) {
                max = count;
            }
        }
        
        for (int i = 0; i < OpponentHoraEstimator_NUMOF_TEHAI; i++) {
            if (i % 10 == 0)
                continue;
            if (nokoriPaiVec[i] == 0)
                continue;
            else if (nokoriPaiVec[i] == 1)
                ret.push_back(Pai(Util::convertMjscorePainumToMjAI(i)));
            else if (nokoriPaiVec[i] >= 2 && max > 0) {
                int cnt = std::max((int)((double)nokoriPaiVec[i] *
                                         ((double)distribution[i] / (double)max)),
                                   1);
                for (int j = 0; j < cnt; j++) {
                    ret.push_back(Pai(Util::convertMjscorePainumToMjAI(i)));
                }
            } else {
                int cnt = (double)nokoriPaiVec[i];
                for (int j = 0; j < cnt; j++) {
                    ret.push_back(Pai(Util::convertMjscorePainumToMjAI(i)));
                }
            }
        }
        
        //        for(Pai p : ret) {
        //            cout << p.toString() << " ";
        //        }
        //        cout << endl;
*/
        return ret;
    }
};

#endif
