
#ifndef OpponentHoraEstimator_HPP
#define OpponentHoraEstimator_HPP

#include "bkuc/engine.h"

#include "util.h"

using namespace std;

// 相手の放銃点と放銃率の見積もりクラス
class OpponentHoraEstimator {
    // --------------------------------------------------------------
public:
    
    // 相手の牌ごとの放銃点
    double hojuTen[3][34];
    // 相手の牌ごとの放銃率
    double hojuProb[3][34];
    
    // インターフェイス
    void estimate(BKUC1681 *bkuc,
                  player tatya[],
                  field *game_field) {
        
        // init
        memset(hojuTen,0,sizeof(hojuTen));
        memset(hojuProb,0,sizeof(hojuProb));
        
        int sutehai[34] = {0};
        for (int i = 0; i < 37; i++) {
            if(i%10==0)continue;
            sutehai[bkuc->util.trance37to34(i)] += game_field->sutehai_sarasi[i];
        }
        int mieteruhai[34] = {0};
        bkuc->make_mieteruhai(0, tatya, mieteruhai, sutehai);
        
        for (int i = 0; i < 34; i++) {
            //if(i%10==0)continue;
            if(mieteruhai[i]>4) {
                mieteruhai[i]=0;
            }
        }
        
        field game_field_tmp;
        memcpy(&game_field_tmp,game_field,sizeof(field));
        
        // hoju prob
        boost::unordered_map<string, double> matiyomi_pro = bkuc->cal_matiyomi_pro(bkuc->matiyomi_wei,
                                                                                   0,
                                                                                   tatya,
                                                                                   game_field,
                                                                                   mieteruhai);
        
        for (boost::unordered_map<string, double>::iterator it =
             matiyomi_pro.begin();
             it != matiyomi_pro.end(); ++it) {
            
            //std::cout << " " << it->first << ":" << it->second;
            
            std::string s = it->first;
            
            std::vector<std::string> words;
            boost::split(words, it->first, boost::is_any_of("_"),
                         boost::token_compress_on);
            
            //            std::cout << words[0] << "****" << words[1] << std::endl;
            //            std::cout << std::stoi(words[0]) << "~|~|" << std::endl;
            
            hojuProb[std::stoi(words[0])-1][std::stoi(words[1])] = it->second;
        }
        
        // hoju point
        boost::unordered_map<string, double> tokuten;
        bkuc->cal_tokuten_pro(tokuten, bkuc->tokuten_wei, 0, tatya,
                              &game_field_tmp, mieteruhai);
        
        for (auto it = tokuten.begin(); it != tokuten.end();
             ++it) {
            
            //std::cout << " " << it->first << ":" << it->second << " ";
            
            std::vector<std::string> words;
            boost::split(words, it->first, boost::is_any_of("_"),
                         boost::token_compress_on);
            
            if (std::stoi(words[1]) < 0) {
                continue;
            }
            
            // std::cout << words[0] << "****" << words[1] << std::endl;
            // std::cout << std::stoi(words[0]) - 1 << "~|~|" << std::endl;
            
            hojuTen[std::stoi(words[0]) - 1][std::stoi(words[1])] = it->second * 100;
            hojuTen[std::stoi(words[0]) - 1][std::stoi(words[1])] = (int)hojuTen[std::stoi(words[0]) - 1][std::stoi(words[1])] - (int)hojuTen[std::stoi(words[0]) - 1][std::stoi(words[1])] % 1000;
        }
    }
private:
    
};
#endif
