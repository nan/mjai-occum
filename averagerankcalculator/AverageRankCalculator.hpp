#ifndef AverageRankCalculator_hpp
#define AverageRankCalculator_hpp

#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>

#define LOG_FOR_AverageRankCalculator 0

// 平均順位計算クラス
class AverageRankCalculator {
private:
public:
    // 平均順位計算
    // @param 席(0起家〜3ラス親)
    // @param 起家点数
    // @param 上家点数
    // @param 対面点数
    // @param ラス親点数
    // @param 残り局数 7:東1局 0:南4局 -1:ゲーム終了
    // @return 平均順位 1 〜 4
    double calc(int seki, int chichaPoint, int kamityaPoint, int toimenPoint,
                int rasuoyaPoint, int nokoriKyokuSuu) {
        
        if (nokoriKyokuSuu < 0)
            nokoriKyokuSuu = -1;
        
        double ret = 4;
        
        std::vector<int> taiSyou;
        std::vector<int> workPointVector;
        
        workPointVector.push_back(chichaPoint);
        workPointVector.push_back(kamityaPoint);
        workPointVector.push_back(toimenPoint);
        workPointVector.push_back(rasuoyaPoint);
        
        for (int i = 0; i < 4; i++) {
            if (i == seki) {
                continue;
            }
            taiSyou.push_back(i);
        }
#if LOG_FOR_AverageRankCalculator
        std::cout << "---AverageRankCalc---" << std::endl;
        std::cout << "seki = " << seki << std::endl;
        std::cout << "jibun no tensuu = " << workPointVector[seki] << std::endl;
        for (int i = 0; i < 3; i++) {
            std::cout << "taiSyou[" << i << "] = " << taiSyou[i] << std::endl;
            std::cout << "tensuu = " << workPointVector[taiSyou[i]] << std::endl;
            std::cout << "workPointVector[" << seki << "] -
            workPointVector[taiSyou["
                                    << i << "]] = "
                                    << workPointVector[seki] - workPointVector[taiSyou[i]]
                                    << std::endl;
                                    }
#endif
                                    for (int i = 0; i < 3; i++) {
                                    double winProb =
                                    1 * getWinProb(seki, taiSyou[0], nokoriKyokuSuu,
                                                   workPointVector[seki] - workPointVector[taiSyou[i]]);
                                    
#if LOG_FOR_AverageRankCalculator
                                    std::cout << "winProb" << i << " = " << winProb << std::endl;
#endif
                                    
                                    ret -= winProb;
                                    }
                                    
#if LOG_FOR_AverageRankCalculator
                                    std::cout << "ret = " << ret << std::endl;
#endif
                                    
                                    if (ret < 1 || ret > 4) {
                                    throw "error";
                                    }
                                    
                                    return ret;
                                    }
                                    
                                    // myPosとotherPosの点差から勝つ確率を求める
                                    double getWinProb(int myPos, int otherPos, int nokori, int diff) {
                                    
                                    if (nokori < 0) {
                                    // ゲーム終了時順位
                                    return getWinProbFromRelativeScoreIfOurasu(diff, myPos, otherPos);
                                    }
                                    
                                    char key1s[20];
                                    
                                    int kyokuNumAry[8] = {4, 3, 2, 1, 4, 3, 2, 1};
                                    
                                    // JSONキー
                                    if (nokori >= 4) {
                                    if (nokori > 7)
                                    nokori = 7;
                                    sprintf(key1s, "E%d,%d,%d", kyokuNumAry[nokori], myPos, otherPos);
                                    } else {
                                    sprintf(key1s, "S%d,%d,%d", kyokuNumAry[nokori], myPos, otherPos);
                                    }
                                    
                                    double winProb = getWinProbFromRelativeScore(diff, myPos, otherPos, key1s);
                                    
                                    return winProb;
                                    }
                                    
                                    // 点差と席から相手に勝つ確率を返す
                                    double getWinProbFromRelativeScore(int relativeScore, int myPos, int otherPos,
                                                                       std::string key1s) {
                                    
                                    // 100以下はだめ
                                    relativeScore = relativeScore - relativeScore % 100;
                                    
                                    int point_diff = (int)relativeScore / 100;
                                    
                                    double ret = -1;
                                    
                                    if (key1s == "E1,0,1") {
                                    ret = (1 / (1 + exp(-0.008511 * (point_diff - 2.486363))));
                                    } else if (key1s == "E1,0,2") {
                                    ret = (1 / (1 + exp(-0.008533 * (point_diff - 2.228121))));
                                    } else if (key1s == "E1,0,3") {
                                    ret = (1 / (1 + exp(-0.008603 * (point_diff - 2.384106))));
                                    } else if (key1s == "E1,1,0") {
                                    ret = (1 / (1 + exp(-0.008511 * (point_diff + 2.486362))));
                                    } else if (key1s == "E1,1,2") {
                                    ret = (1 / (1 + exp(-0.008470 * (point_diff - 0.240220))));
                                    } else if (key1s == "E1,1,3") {
                                    ret = (1 / (1 + exp(-0.008539 * (point_diff - 0.386048))));
                                    } else if (key1s == "E1,2,0") {
                                    ret = (1 / (1 + exp(-0.008533 * (point_diff + 2.228121))));
                                    } else if (key1s == "E1,2,1") {
                                    ret = (1 / (1 + exp(-0.008470 * (point_diff + 0.240220))));
                                    } else if (key1s == "E1,2,3") {
                                    ret = (1 / (1 + exp(-0.008560 * (point_diff - 0.647130))));
                                    } else if (key1s == "E1,3,0") {
                                    ret = (1 / (1 + exp(-0.008603 * (point_diff + 2.384107))));
                                    } else if (key1s == "E1,3,1") {
                                    ret = (1 / (1 + exp(-0.008539 * (point_diff + 0.386048))));
                                    } else if (key1s == "E1,3,2") {
                                    ret = (1 / (1 + exp(-0.008560 * (point_diff + 0.647130))));
                                    } else if (key1s == "E2,0,1") {
                                    ret = (1 / (1 + exp(-0.009335 * (point_diff - 6.594324))));
                                    } else if (key1s == "E2,0,2") {
                                    ret = (1 / (1 + exp(-0.009272 * (point_diff - 8.853030))));
                                    } else if (key1s == "E2,0,3") {
                                    ret = (1 / (1 + exp(-0.009353 * (point_diff - 10.227729))));
                                    } else if (key1s == "E2,1,0") {
                                    ret = (1 / (1 + exp(-0.009335 * (point_diff + 6.594324))));
                                    } else if (key1s == "E2,1,2") {
                                    ret = (1 / (1 + exp(-0.009113 * (point_diff - 2.743206))));
                                    } else if (key1s == "E2,1,3") {
                                    ret = (1 / (1 + exp(-0.009190 * (point_diff - 4.106530))));
                                    } else if (key1s == "E2,2,0") {
                                    ret = (1 / (1 + exp(-0.009272 * (point_diff + 8.853030))));
                                    } else if (key1s == "E2,2,1") {
                                    ret = (1 / (1 + exp(-0.009113 * (point_diff + 2.743206))));
                                    } else if (key1s == "E2,2,3") {
                                    ret = (1 / (1 + exp(-0.009132 * (point_diff - 1.853322))));
                                    } else if (key1s == "E2,3,0") {
                                    ret = (1 / (1 + exp(-0.009353 * (point_diff + 10.227729))));
                                    } else if (key1s == "E2,3,1") {
                                    ret = (1 / (1 + exp(-0.009190 * (point_diff + 4.10653))));
                                    } else if (key1s == "E2,3,2") {
                                    ret = (1 / (1 + exp(-0.009132 * (point_diff + 1.853323))));
                                    } else if (key1s == "E3,0,1") {
                                    ret = (1 / (1 + exp(-0.010260 * (point_diff + 1.40400))));
                                    } else if (key1s == "E3,0,2") {
                                    ret = (1 / (1 + exp(-0.010010 * (point_diff - 6.27093))));
                                    } else if (key1s == "E3,0,3") {
                                    ret = (1 / (1 + exp(-0.010010 * (point_diff - 9.83810))));
                                    } else if (key1s == "E3,1,0") {
                                    ret = (1 / (1 + exp(-0.010260 * (point_diff - 1.40400))));
                                    } else if (key1s == "E3,1,2") {
                                    ret = (1 / (1 + exp(-0.010090 * (point_diff - 8.15818))));
                                    } else if (key1s == "E3,1,3") {
                                    ret = (1 / (1 + exp(-0.010080 * (point_diff - 11.73491))));
                                    } else if (key1s == "E3,2,0") {
                                    ret = (1 / (1 + exp(-0.010010 * (point_diff + 6.27093))));
                                    } else if (key1s == "E3,2,1") {
                                    ret = (1 / (1 + exp(-0.010090 * (point_diff + 8.15818))));
                                    } else if (key1s == "E3,2,3") {
                                    ret = (1 / (1 + exp(-0.009849 * (point_diff - 4.038221))));
                                    } else if (key1s == "E3,3,0") {
                                    ret = (1 / (1 + exp(-0.010010 * (point_diff + 9.83810))));
                                    } else if (key1s == "E3,3,1") {
                                    ret = (1 / (1 + exp(-0.010080 * (point_diff + 11.73491))));
                                    } else if (key1s == "E3,3,2") {
                                    ret = (1 / (1 + exp(-0.009849 * (point_diff + 4.038221))));
                                    } else if (key1s == "E4,0,1") {
                                    ret = (1 / (1 + exp(-0.011170 * (point_diff + 0.20649))));
                                    } else if (key1s == "E4,0,2") {
                                    ret = (1 / (1 + exp(-0.011240 * (point_diff + 1.46132))));
                                    } else if (key1s == "E4,0,3") {
                                    ret = (1 / (1 + exp(-0.010980 * (point_diff - 6.65083))));
                                    } else if (key1s == "E4,1,0") {
                                    ret = (1 / (1 + exp(-0.011170 * (point_diff - 0.20650))));
                                    } else if (key1s == "E4,1,2") {
                                    ret = (1 / (1 + exp(-0.011300 * (point_diff + 0.75720))));
                                    } else if (key1s == "E4,1,3") {
                                    ret = (1 / (1 + exp(-0.011040 * (point_diff - 7.35665))));
                                    } else if (key1s == "E4,2,0") {
                                    ret = (1 / (1 + exp(-0.011240 * (point_diff - 1.46132))));
                                    } else if (key1s == "E4,2,1") {
                                    ret = (1 / (1 + exp(-0.011300 * (point_diff - 0.75720))));
                                    } else if (key1s == "E4,2,3") {
                                    ret = (1 / (1 + exp(-0.011100 * (point_diff - 8.60280))));
                                    } else if (key1s == "E4,3,0") {
                                    ret = (1 / (1 + exp(-0.010980 * (point_diff + 6.65083))));
                                    } else if (key1s == "E4,3,1") {
                                    ret = (1 / (1 + exp(-0.011040 * (point_diff + 7.35665))));
                                    } else if (key1s == "E4,3,2") {
                                    ret = (1 / (1 + exp(-0.011100 * (point_diff + 8.60280))));
                                    } else if (key1s == "S1,0,1") {
                                    ret = (1 / (1 + exp(-0.012440 * (point_diff - 1.27409))));
                                    } else if (key1s == "S1,0,2") {
                                    ret = (1 / (1 + exp(-0.012500 * (point_diff - 0.82720))));
                                    } else if (key1s == "S1,0,3") {
                                    ret = (1 / (1 + exp(-0.012610 * (point_diff - 2.07150))));
                                    } else if (key1s == "S1,1,0") {
                                    ret = (1 / (1 + exp(-0.012440 * (point_diff + 1.27409))));
                                    } else if (key1s == "S1,1,2") {
                                    ret = (1 / (1 + exp(-0.012470 * (point_diff - 0.05185))));
                                    } else if (key1s == "S1,1,3") {
                                    ret = (1 / (1 + exp(-0.012580 * (point_diff - 1.29156))));
                                    } else if (key1s == "S1,2,0") {
                                    ret = (1 / (1 + exp(-0.012500 * (point_diff + 0.82720))));
                                    } else if (key1s == "S1,2,1") {
                                    ret = (1 / (1 + exp(-0.012470 * (point_diff + 0.05185))));
                                    } else if (key1s == "S1,2,3") {
                                    ret = (1 / (1 + exp(-0.012640 * (point_diff - 1.74580))));
                                    } else if (key1s == "S1,3,0") {
                                    ret = (1 / (1 + exp(-0.012610 * (point_diff + 2.07150))));
                                    } else if (key1s == "S1,3,1") {
                                    ret = (1 / (1 + exp(-0.012580 * (point_diff + 1.29156))));
                                    } else if (key1s == "S1,3,2") {
                                    ret = (1 / (1 + exp(-0.012640 * (point_diff + 1.74580))));
                                    } else if (key1s == "S2,0,1") {
                                    ret = (1 / (1 + exp(-0.015070 * (point_diff - 5.97097))));
                                    } else if (key1s == "S2,0,2") {
                                    ret = (1 / (1 + exp(-0.014970 * (point_diff - 7.94181))));
                                    } else if (key1s == "S2,0,3") {
                                    ret = (1 / (1 + exp(-0.015070 * (point_diff - 9.89596))));
                                    } else if (key1s == "S2,1,0") {
                                    ret = (1 / (1 + exp(-0.015070 * (point_diff + 5.97097))));
                                    } else if (key1s == "S2,1,2") {
                                    ret = (1 / (1 + exp(-0.014360 * (point_diff - 2.44154))));
                                    } else if (key1s == "S2,1,3") {
                                    ret = (1 / (1 + exp(-0.014440 * (point_diff - 4.33849))));
                                    } else if (key1s == "S2,2,0") {
                                    ret = (1 / (1 + exp(-0.014970 * (point_diff + 7.94181))));
                                    } else if (key1s == "S2,2,1") {
                                    ret = (1 / (1 + exp(-0.014360 * (point_diff + 2.44154))));
                                    } else if (key1s == "S2,2,3") {
                                    ret = (1 / (1 + exp(-0.014350 * (point_diff - 2.38628))));
                                    } else if (key1s == "S2,3,0") {
                                    ret = (1 / (1 + exp(-0.015070 * (point_diff + 9.89596))));
                                    } else if (key1s == "S2,3,1") {
                                    ret = (1 / (1 + exp(-0.014440 * (point_diff + 4.33849))));
                                    } else if (key1s == "S2,3,2") {
                                    ret = (1 / (1 + exp(-0.014350 * (point_diff + 2.38628))));
                                    } else if (key1s == "S3,0,1") {
                                    ret = (1 / (1 + exp(-0.019880 * (point_diff + 1.21993))));
                                    } else if (key1s == "S3,0,2") {
                                    ret = (1 / (1 + exp(-0.018550 * (point_diff - 4.25702))));
                                    } else if (key1s == "S3,0,3") {
                                    ret = (1 / (1 + exp(-0.018350 * (point_diff - 7.92111))));
                                    } else if (key1s == "S3,1,0") {
                                    ret = (1 / (1 + exp(-0.019880 * (point_diff - 1.21993))));
                                    } else if (key1s == "S3,1,2") {
                                    ret = (1 / (1 + exp(-0.018740 * (point_diff - 5.91014))));
                                    } else if (key1s == "S3,1,3") {
                                    ret = (1 / (1 + exp(-0.018530 * (point_diff - 9.59927))));
                                    } else if (key1s == "S3,2,0") {
                                    ret = (1 / (1 + exp(-0.018550 * (point_diff + 4.25702))));
                                    } else if (key1s == "S3,2,1") {
                                    ret = (1 / (1 + exp(-0.018740 * (point_diff + 5.91014))));
                                    } else if (key1s == "S3,2,3") {
                                    ret = (1 / (1 + exp(-0.017470 * (point_diff - 3.97031))));
                                    } else if (key1s == "S3,3,0") {
                                    ret = (1 / (1 + exp(-0.018350 * (point_diff + 7.92111))));
                                    } else if (key1s == "S3,3,1") {
                                    ret = (1 / (1 + exp(-0.018530 * (point_diff + 9.59927))));
                                    } else if (key1s == "S3,3,2") {
                                    ret = (1 / (1 + exp(-0.017470 * (point_diff + 3.97031))));
                                    } else if (key1s == "S4,0,1") {
                                    ret = (1 / (1 + exp(-0.031000 * (point_diff + 0.30090))));
                                    } else if (key1s == "S4,0,2") {
                                    ret = (1 / (1 + exp(-0.031570 * (point_diff + 1.06490))));
                                    } else if (key1s == "S4,0,3") {
                                    ret = (1 / (1 + exp(-0.027260 * (point_diff - 4.51279))));
                                    } else if (key1s == "S4,1,0") {
                                    ret = (1 / (1 + exp(-0.031000 * (point_diff - 0.30090))));
                                    } else if (key1s == "S4,1,2") {
                                    ret = (1 / (1 + exp(-0.031920 * (point_diff + 0.26936))));
                                    } else if (key1s == "S4,1,3") {
                                    ret = (1 / (1 + exp(-0.027490 * (point_diff - 5.28167))));
                                    } else if (key1s == "S4,2,0") {
                                    ret = (1 / (1 + exp(-0.031570 * (point_diff - 1.06490))));
                                    } else if (key1s == "S4,2,1") {
                                    ret = (1 / (1 + exp(-0.031920 * (point_diff - 0.26936))));
                                    } else if (key1s == "S4,2,3") {
                                    ret = (1 / (1 + exp(-0.027950 * (point_diff - 5.93742))));
                                    } else if (key1s == "S4,3,0") {
                                    ret = (1 / (1 + exp(-0.027260 * (point_diff + 4.51279))));
                                    } else if (key1s == "S4,3,1") {
                                    ret = (1 / (1 + exp(-0.027490 * (point_diff + 5.28167))));
                                    } else if (key1s == "S4,3,2") {
                                    ret = (1 / (1 + exp(-0.027950 * (point_diff + 5.93742))));
                                    }
                                    
                                    // 点差が大きすぎるかゲーム終了
                                    if (ret == -1) {
                                    return relativeScore >= 0 ? 1 : 0;
                                    }
                                    
                                    return ret;
                                    }
                                    
                                    // ゲーム終了時順位
                                    double getWinProbFromRelativeScoreIfOurasu(int relativeScore, int myPos,
                                                                               int otherPos) {
                                    if (myPos < otherPos) {
                                    return relativeScore >= 0 ? 1 : 0;
                                    } else {
                                    return relativeScore > 0 ? 1 : 0;
            }
            }
            
            };
            
            
#endif
