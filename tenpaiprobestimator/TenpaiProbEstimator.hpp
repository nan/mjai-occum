#ifndef TenpaiProbEstimator_hpp
#define TenpaiProbEstimator_hpp

#include <iostream>
#include <vector>
#include "bkuc/engine.h"

class TenpaiProbEstimator {
public:
    std::vector<double> tenpai_pro;
    
    double estimate(BKUC1681 *bkuc,
                    player tatya[],
                    field *game_field) {
        
        tenpai_pro.clear();
        tenpai_pro.push_back(0);
        tenpai_pro.push_back(0);
        tenpai_pro.push_back(0);
        
        int sutehai[34] = {0};
        for (int i = 0; i < 37; i++) {
            sutehai[bkuc->util.trance37to34(i)] += game_field->sutehai_sarasi[i];
        }
        
        tenpai_pro = bkuc->cal_tenpai_pro(tenpai_pro,
                                          bkuc->tenpai_wei,
                                          0,
                                          tatya,
                                          game_field,
                                          sutehai);
        
        return 0;
    }
};

#endif
