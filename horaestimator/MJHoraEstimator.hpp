#ifndef MJHoraEstimator_hpp
#define MJHoraEstimator_hpp

#include "MJRonHoraEstimator.hpp"
#include "mjscore.h"
#include "shanten_analysis.h"
#include "syanten4hora_estimator.h"
#include "util.h"
//#include <algorithm>
#include <iostream>
#include <map>
#include <random>
#include <thread>
#include <vector>

#include "../OpponentHoraEstimator/OpponentHoraEstimator.hpp"

#define THREAD_NUM 10
#define MONTE_SIZE (THREAD_NUM * 900)
#define SHANTEN_UNDEFINE 999

#define RESPONSE_TYPE_MJPIR_SUTEHAI 0x00000100
#define RESPONSE_TYPE_MJPIR_REACH 0x00000200
#define RESPONSE_TYPE_MJPIR_KAN 0x00000400
#define RESPONSE_TYPE_MJPIR_TSUMO 0x00000800
#define RESPONSE_TYPE_MJPIR_NAGASHI 0x00001000
#define RESPONSE_TYPE_MJPIR_PON 0x00002000
#define RESPONSE_TYPE_MJPIR_CHII_LEFT 0x00004000   // left chi
#define RESPONSE_TYPE_MJPIR_CHII_RIGHT 0x00010000  // right chi
#define RESPONSE_TYPE_MJPIR_CHII_MIDDLE 0x00008000 // middle chi
#define RESPONSE_TYPE_MJPIR_MINKAN 0x00020000
#define RESPONSE_TYPE_MJPIR_ANKAN 0x00040000
#define RESPONSE_TYPE_MJPIR_RON 0x00080000
#define RESPONSE_TYPE_MJPIR_NONE 0x00100000
#define RESPONSE_TYPE_MJPIR_KAKAN 0x00200000

class HoraProbDistStruct {
public:
    int points;
    int pid;
    int freq;
    HoraProbDistStruct() { points = pid = freq = 0; }
};

class MJHoraEstimator;

class Metrics {
public:
    Pai pai = Pai("?");
    double horaProb = 0;
    double tenpaiProb = 0;
    double expectedHoraPoints = 0;
    int averageHoraPoints = 0;
    int shanten = 999;
    int chitoitu_shanten = 999;
    int kokushi_shanten = 999;
    double expectedPoints = 0;
    bool red = false;
    double averageRank = 0;
    double hojuProb = 0;
    double averageHojuPoints = 0;
    Pai taken = Pai("?");
    int type = RESPONSE_TYPE_MJPIR_NONE;
    int response_type = RESPONSE_TYPE_MJPIR_NONE;
    bool tsumogiri = false;
    int koritsuPaiPoint = 0;
    
    int ryanmenSuu = 0;
    
    Metrics() { shanten = SHANTEN_UNDEFINE; }
    Metrics(Pai p) { taken = p; }
    
    std::string toString() {
#ifdef RELEASE_BUILD
        return "";
#endif
        char s[512];
        sprintf(
                s, "pai = %3s | shanten = %1d | chitoi = %1d | kokushi = %1d | "
                "horaProb = %.6f | "
                "averageHoraPoints = %d | hojuProb = %.3f | averageHojuPoint = %d | "
                "tenpaiProb = %.3f | "
                "type = %s | taken = %3s | koritsu = %2d | averageRank = %.10f\n",
                pai.toString().c_str(), shanten, chitoitu_shanten, kokushi_shanten,
                horaProb, averageHoraPoints, hojuProb, (int)averageHojuPoints,
                tenpaiProb, Metrics::typeToString(type).c_str(),
                taken.toString().c_str(), koritsuPaiPoint, averageRank);
        
        return std::string(s);
    }
    
    std::string toHtmlString() {
#ifdef RELEASE_BUILD
        return "";
#endif
        char s[512];
        sprintf(
                s, "pai = %3s | shanten = %1d | chitoi = %1d | kokushi = %1d | "
                "horaProb = %.6f | "
                "averageHoraPoints = %d | hojuProb = %.3f | averageHojuPoint = %d | "
                "tenpaiProb = %.3f | "
                "type = %s | taken = %3s | koritsu = %2d | averageRank = "
                "%.10f\\n",
                pai.toString().c_str(), shanten, chitoitu_shanten, kokushi_shanten,
                horaProb, averageHoraPoints, hojuProb, (int)averageHojuPoints,
                tenpaiProb, Metrics::typeToString(type).c_str(),
                taken.toString().c_str(), koritsuPaiPoint, averageRank);
        
        return std::string(s);
    }
    
    static std::string typeToString(int t) {
        
        if (RESPONSE_TYPE_MJPIR_SUTEHAI == t)
            return "sutehai";
        if (RESPONSE_TYPE_MJPIR_REACH == t)
            return "reach";
        if (RESPONSE_TYPE_MJPIR_KAN == t)
            return "kan";
        if (RESPONSE_TYPE_MJPIR_TSUMO == t)
            return "tsumo hora";
        if (RESPONSE_TYPE_MJPIR_NAGASHI == t)
            return "nagasi";
        if (RESPONSE_TYPE_MJPIR_PON == t)
            return "pon";
        if (RESPONSE_TYPE_MJPIR_CHII_LEFT == t)
            return "chi left";
        if (RESPONSE_TYPE_MJPIR_CHII_RIGHT == t)
            return "chi right";
        if (RESPONSE_TYPE_MJPIR_CHII_MIDDLE == t)
            return "chi middle";
        if (RESPONSE_TYPE_MJPIR_MINKAN == t)
            return "daiminkan";
        if (RESPONSE_TYPE_MJPIR_ANKAN == t)
            return "ankan";
        if (RESPONSE_TYPE_MJPIR_RON == t)
            return "ron";
        if (RESPONSE_TYPE_MJPIR_NONE == t)
            return "none";
        if (RESPONSE_TYPE_MJPIR_KAKAN == t)
            return "kakan";
        return "?";
    }
    
    static bool compareMyDataPredicate(Metrics lhs, Metrics rhs) {
        // trueなら左の方が価値高い
        
        double lhsAvgDbl = (double)((lhs.averageRank));
        double rhsAvgDbl = (double)((rhs.averageRank));
        
        float lhsAvgFlt = (float)((lhs.averageRank));
        float rhsAvgFlt = (float)((rhs.averageRank));
        
        // double lhsAvgpt = std::min((double)2000, (double)lhs.averageHoraPoints);
        // double rhsAvgpt = std::min((double)2000, (double)rhs.averageHoraPoints);
        
        double lhsAvgpt = (double)lhs.averageHoraPoints;
        double rhsAvgpt = (double)rhs.averageHoraPoints;
        
        if (lhsAvgFlt != rhsAvgFlt && lhsAvgDbl != rhsAvgDbl) {
            return (lhsAvgDbl < rhsAvgDbl);
        } else if (lhs.horaProb * lhsAvgpt -
                   lhs.hojuProb * (double)lhs.averageHojuPoints !=
                   rhs.horaProb * rhsAvgpt -
                   rhs.hojuProb * (double)rhs.averageHojuPoints) {
            
            return (lhs.horaProb * lhsAvgpt -
                    lhs.hojuProb * (double)lhs.averageHojuPoints >
                    rhs.horaProb * rhsAvgpt -
                    rhs.hojuProb * (double)rhs.averageHojuPoints);
        } else if (lhs.shanten != rhs.shanten) {
            return (lhs.shanten < rhs.shanten);
        } else if (lhs.horaProb != rhs.horaProb) {
            return (lhs.horaProb > rhs.horaProb);
        } else if (lhs.hojuProb != rhs.hojuProb) {
            return (lhs.hojuProb < rhs.hojuProb);
        } else if (lhs.tenpaiProb != rhs.tenpaiProb) {
            return (lhs.tenpaiProb > rhs.tenpaiProb);
        } else if (lhs.averageHoraPoints != rhs.averageHoraPoints) {
            return (lhs.averageHoraPoints > rhs.averageHoraPoints);
        } else if (lhs.chitoitu_shanten != rhs.chitoitu_shanten) {
            return (lhs.chitoitu_shanten < rhs.chitoitu_shanten);
        } else if (lhs.koritsuPaiPoint != rhs.koritsuPaiPoint) {
            return (lhs.koritsuPaiPoint < rhs.koritsuPaiPoint);
        } else if (lhs.pai.red != rhs.pai.red) {
            return lhs.pai.red < rhs.pai.red;
        }
        return 0;
    }
};

// あがり率等計算クラス（一人麻雀）
// mjai/manueのパクリ
class MJHoraEstimator {
public:
    bool oya;
    Pai bakaze;
    Pai jikaze;
    
    OpponentHoraEstimator estimator;
    
    bool existDengerausPlayer = false;
    
    // 入り口（ツモ時）
    std::vector<Metrics>
    getHoraEstimation(std::vector<Pai> tehais, std::vector<Furo> furos,
                      std::vector<Pai> doraMarker, std::vector<Pai> jibunNoKawa,
                      int numRemainPai, int richiBou, int honba, Pai _bakaze,
                      Pai _jikaze, std::vector<Pai> visiblePais, Pai tsumohai,
                      bool isReach, bool canReach,
                      OpponentHoraEstimator *_estimator) {
        
        estimator = *_estimator;
        
        int allowExtra = 1;
        
        //    if (furos.size() > 1 || numRemainPai < 10) {
        //      allowExtra = 0;
        //    }
        
        ShantenAnalysis analysis = ShantenAnalysis(tehais, allowExtra);
        
        int chitoi_shanten = getChitoituShanten(tehais, furos);
        
        cout << "shanten = " << analysis.shanten << endl;
        
        // tsumoHora
        if (analysis.shanten < 0 || chitoi_shanten < 0) {
            bool chk = true;
            if (!isReach) {
                // 役なしチェック
                if (isYakunashiTsumoHora(tehais, furos, tsumohai, bakaze, jikaze)) {
                    chk = false;
                }
            }
            if (chk) {
                Metrics ret;
                ret.pai = tsumohai;
                ret.type = RESPONSE_TYPE_MJPIR_TSUMO;
                return {ret};
            }
        }
        
        // reach {
        if (isReach) {
            Metrics ret;
            ret.pai = tsumohai;
            ret.type = RESPONSE_TYPE_MJPIR_SUTEHAI;
            return {ret};
        }
        
        bakaze = _bakaze;
        jikaze = _jikaze;
        oya = bakaze.equal(jikaze);
        
        std::vector<Metrics> ret = getHoraEstimationInternal(
                                                             analysis, tehais, furos, doraMarker, jibunNoKawa,
                                                             false, // none
                                                             std::max(0, numRemainPai / 4), richiBou, honba, visiblePais,
                                                             RESPONSE_TYPE_MJPIR_SUTEHAI, tsumohai);
        // リーチ
        if (canReach && furoSizeWithoutAnkan(furos) < 1 && !isReach) {
            
            ShantenAnalysis analysis_noExtra = ShantenAnalysis(tehais, 0);
            
            std::vector<Metrics> retRichi = getHoraEstimationInternal(
                                                                      analysis_noExtra, tehais, furos, doraMarker, jibunNoKawa,
                                                                      false, // none
                                                                      std::max(0, numRemainPai / 4), richiBou, honba, visiblePais,
                                                                      RESPONSE_TYPE_MJPIR_REACH, tsumohai);
            
            std::copy(retRichi.begin(), retRichi.end(), std::back_inserter(ret));
        }
        // アンカン
        if (numRemainPai >= 1) {
            std::vector<Pai> _ankanPais = ankanPais(tehais);
            for (int i = 0; i < _ankanPais.size(); i++) {
                
                std::vector<Pai> _tehais = tehais;
                std::vector<Furo> _furos = furos;
                
                _tehais = removePaiVector(_ankanPais[i], _tehais);
                _tehais = removePaiVector(_ankanPais[i], _tehais);
                _tehais = removePaiVector(_ankanPais[i], _tehais);
                _tehais = removePaiVector(_ankanPais[i], _tehais);
                std::vector<Pai> tmp_consumed = {_ankanPais[i], _ankanPais[i],
                    _ankanPais[i]};
                _furos.push_back(Furo("ankan", _ankanPais[i], 0, tmp_consumed));
                
                ShantenAnalysis _analysis = ShantenAnalysis(_tehais, 1);
                
                if (analysis.shanten == _analysis.shanten) {
                    
                    std::vector<Metrics> retAnkan = getHoraEstimationInternal(
                                                                              _analysis, _tehais, _furos, doraMarker, jibunNoKawa,
                                                                              true, // none
                                                                              std::max(0, numRemainPai / 4), richiBou, honba, visiblePais,
                                                                              RESPONSE_TYPE_MJPIR_ANKAN);
                    for (int j = 0; j < retAnkan.size(); j++) {
                        retAnkan[j].taken = _ankanPais[i];
                        
                        bool ankanIgnoreFlag = false;
                        for (int k = 0; k < ret.size(); k++) {
                            // add furo
                            if (retAnkan[j].horaProb < ret[k].horaProb ||
                                retAnkan[j].shanten > 3) {
                                cout << "ignore ankan" << endl;
                                ankanIgnoreFlag = true;
                                break;
                            }
                        }
                        if (!ankanIgnoreFlag) {
                            ret.push_back(retAnkan[j]);
                        }
                    }
                    // std::copy(retAnkan.begin(), retAnkan.end(),
                    // std::back_inserter(ret));
                }
            }
        }
        // カカン
        std::vector<Pai> _kakanPais = kakanPais(tehais, furos);
        for (int i = 0; i < _kakanPais.size(); i++) {
            
            std::vector<Pai> _tehais = tehais;
            std::vector<Furo> _furos = furos;
            
            for (int j = 0; j < _furos.size(); j++) {
                if (_furos[j].type == "pon") {
                    _tehais = removePaiVector(_kakanPais[i], _tehais);
                    _furos[j] = Furo("kakan", _kakanPais[i], 0,
                                     {_kakanPais[i], _kakanPais[i], _kakanPais[i]});
                    break;
                }
            }
            
            ShantenAnalysis _analysis = ShantenAnalysis(_tehais, 1);
            if (analysis.shanten == _analysis.shanten) {
                
                std::vector<Metrics> retKakan = getHoraEstimationInternal(
                                                                          _analysis, _tehais, _furos, doraMarker, jibunNoKawa,
                                                                          true, // none
                                                                          std::max(0, numRemainPai / 4), richiBou, honba, visiblePais,
                                                                          RESPONSE_TYPE_MJPIR_KAKAN);
                
                for (int j = 0; j < retKakan.size(); j++) {
                    retKakan[j].taken = _kakanPais[i];
                    
                    bool ankanIgnoreFlag = false;
                    for (int k = 0; k < ret.size(); k++) {
                        // add furo
                        if (retKakan[j].horaProb < ret[k].horaProb ||
                            retKakan[j].shanten > 3) {
                            cout << "ignore kakan" << endl;
                            ankanIgnoreFlag = true;
                            break;
                        }
                    }
                    if (!ankanIgnoreFlag) {
                        ret.push_back(retKakan[j]);
                    }
                }
                
                //        for (int j = 0; j < retKakan.size(); j++) {
                //          retKakan[j].taken = _kakanPais[i];
                //        }
                //        std::copy(retKakan.begin(), retKakan.end(),
                //        std::back_inserter(ret));
            }
        }
        return ret;
    }
    
    //  static void myshuffle(vector<Pai> &array, std::mt19937 mt, int size) {
    //    for (int i = 0; i < size; i++) {
    //      size_t j = mt() % size;
    //      //      Pai t = *(array[j]);
    //      //      *(array[j]) = *(array[i]);
    //      //      *(array[i]) = t;
    //    }
    //  }
    
    // 入り口（フーロ時）
    std::vector<Metrics>
    getHoraEstimationWhenFuro(Pai pai, int target // 1:下家 2:対面 3:上家
                              ,
                              std::vector<Pai> tehais, std::vector<Furo> furos,
                              std::vector<Pai> doraMarker,
                              std::vector<Pai> jibunNoKawa, int numRemainPai,
                              int richiBou, int honba, Pai _bakaze, Pai _jikaze,
                              std::vector<Pai> visiblePais, bool isRIchi,
                              OpponentHoraEstimator *_estimator) {
        
        estimator = *_estimator;
        
        // srand((unsigned)time(NULL));
        
        bakaze = _bakaze;
        jikaze = _jikaze;
        oya = bakaze.equal(jikaze);
        
        std::vector<Metrics> metrics;
        
        if (canRon(pai, tehais, furos)) {
            Metrics ret;
            ret.type = RESPONSE_TYPE_MJPIR_RON;
            return {ret};
        }
        
        if (isRIchi) {
            Metrics ret;
            ret.type = RESPONSE_TYPE_MJPIR_NONE;
            return {ret};
        }
        
        bool existPossibleFuro = false;
        
        // 鳴かないシャンテン数
        
        // ないて無いなら手変わり等ありえるが
        // ないてないならそのまま真っすぐ的な意味
        //      int extra = 0;
        //      if(furos.size() == 0) {
        //          extra = 1;
        //      }
        //      ShantenAnalysis nakanai_analysis = ShantenAnalysis(tehais, extra);
        
        ShantenAnalysis nakanai_analysis =
        ShantenAnalysis(tehais, 0);
        
        if (canLeftChi(pai, tehais, target)) {
            existPossibleFuro = true;
            
            std::vector<Pai> _tehais = tehais;
            
            _tehais = removePaiVector(pai.next(1), _tehais);
            _tehais = removePaiVector(pai.next(2), _tehais);
            std::vector<Pai> tmp_consumed = {pai.next(1), pai.next(2)};
            furos.push_back(Furo("chi", pai, target, tmp_consumed));
            
            ShantenAnalysis _analysis = ShantenAnalysis(_tehais, 0);
            
            vector<Metrics> tmp_metrics = getHoraEstimationInternal(
                                                                    _analysis, _tehais, furos, doraMarker, jibunNoKawa,
                                                                    false, // none
                                                                    std::max(0, numRemainPai / 4), richiBou, honba, visiblePais,
                                                                    RESPONSE_TYPE_MJPIR_CHII_LEFT);
            
            for (int j = 0; j < tmp_metrics.size(); j++) {
                if ( // nakanai_analysis.shanten > tmp_metrics[j].shanten &&
                    tmp_metrics[j].tenpaiProb > 0) {
                    
                    if (!isKuikaeChi(tmp_metrics[j].pai, pai, 0)) {
                        metrics.push_back(tmp_metrics[j]);
                    }
                }
            }
            furos.pop_back();
        }
        
        if (canMiddleChi(pai, tehais, target)) {
            existPossibleFuro = true;
            
            std::vector<Pai> _tehais = tehais;
            
            _tehais = removePaiVector(pai.next(-1), _tehais);
            _tehais = removePaiVector(pai.next(1), _tehais);
            std::vector<Pai> tmp_consumed = {pai.next(-1), pai.next(1)};
            furos.push_back(Furo("chi", pai, target, tmp_consumed));
            
            ShantenAnalysis _analysis = ShantenAnalysis(_tehais, 0);
            
            vector<Metrics> tmp_metrics = getHoraEstimationInternal(
                                                                    _analysis, _tehais, furos, doraMarker, jibunNoKawa,
                                                                    false, // none
                                                                    std::max(0, numRemainPai / 4), richiBou, honba, visiblePais,
                                                                    RESPONSE_TYPE_MJPIR_CHII_MIDDLE);
            
            for (int j = 0; j < tmp_metrics.size(); j++) {
                if ( // nakanai_analysis.shanten > tmp_metrics[j].shanten &&
                    tmp_metrics[j].tenpaiProb > 0) {
                    
                    if (!isKuikaeChi(tmp_metrics[j].pai, pai, 1)) {
                        metrics.push_back(tmp_metrics[j]);
                    }
                }
            }
            furos.pop_back();
        }
        
        if (canRightChi(pai, tehais, target)) {
            
            existPossibleFuro = true;
            
            std::vector<Pai> _tehais = tehais;
            _tehais = removePaiVector(pai.next(-1), _tehais);
            _tehais = removePaiVector(pai.next(-2), _tehais);
            std::vector<Pai> tmp_consumed = {pai.next(-2), pai.next(-1)};
            furos.push_back(Furo("chi", pai, target, tmp_consumed));
            
            ShantenAnalysis _analysis = ShantenAnalysis(_tehais, 0);
            
            vector<Metrics> tmp_metrics = getHoraEstimationInternal(
                                                                    _analysis, _tehais, furos, doraMarker, jibunNoKawa,
                                                                    false, // none
                                                                    std::max(0, numRemainPai / 4), richiBou, honba, visiblePais,
                                                                    RESPONSE_TYPE_MJPIR_CHII_RIGHT);
            
            for (int j = 0; j < tmp_metrics.size(); j++) {
                if ( // nakanai_analysis.shanten > tmp_metrics[j].shanten &&
                    tmp_metrics[j].tenpaiProb > 0) {
                    
                    if (!isKuikaeChi(tmp_metrics[j].pai, pai, 2)) {
                        metrics.push_back(tmp_metrics[j]);
                    }
                }
            }
            furos.pop_back();
        }
        
        if (canPon(pai, tehais)) {
            
            existPossibleFuro = true;
            
            std::vector<Pai> _tehais = tehais;
            _tehais = removePaiVector(pai, _tehais);
            _tehais = removePaiVector(pai, _tehais);
            std::vector<Pai> tmp_consumed = {pai, pai};
            furos.push_back(Furo("pon", pai, target, tmp_consumed));
            
            ShantenAnalysis _analysis = ShantenAnalysis(_tehais, 0);
            
            vector<Metrics> tmp_metrics = getHoraEstimationInternal(
                                                                    _analysis, _tehais, furos, doraMarker, jibunNoKawa,
                                                                    false, // none
                                                                    std::max(0, numRemainPai / 4), richiBou, honba, visiblePais,
                                                                    RESPONSE_TYPE_MJPIR_PON);
            
            for (int j = 0; j < tmp_metrics.size(); j++) {
                
                if (!pai.hasSameSymbol(tmp_metrics[j].pai) && tmp_metrics[j].tenpaiProb > 0) {
                    metrics.push_back(tmp_metrics[j]);
                }
            }
            furos.pop_back();
        }
        
        if (canDaiminkan(pai, tehais)) {
            
            existPossibleFuro = true;
            
            std::vector<Pai> _tehais = tehais;
            _tehais = removePaiVector(pai, _tehais);
            _tehais = removePaiVector(pai, _tehais);
            _tehais = removePaiVector(pai, _tehais);
            std::vector<Pai> tmp_consumed = {pai, pai};
            furos.push_back(Furo("daiminkan", pai, target, tmp_consumed));
            
            ShantenAnalysis _analysis = ShantenAnalysis(_tehais, 0);
            
            vector<Metrics> tmp_metrics = getHoraEstimationInternal(
                                                                    _analysis, _tehais, furos, doraMarker, jibunNoKawa,
                                                                    false, // none
                                                                    std::max(0, numRemainPai / 4), richiBou, honba, visiblePais,
                                                                    RESPONSE_TYPE_MJPIR_MINKAN);
            
            for (int j = 0; j < tmp_metrics.size(); j++) {
                if ( // nakanai_analysis.shanten > tmp_metrics[j].shanten &&
                    tmp_metrics[j].tenpaiProb > 0) {
                    
                    metrics.push_back(tmp_metrics[j]);
                }
            }
            furos.pop_back();
        }
        
        // no furo metrics
        
        // 可能なアクションがない
        if (metrics.size() == 0 || numRemainPai < 1) {
            if (existPossibleFuro)
                return {Metrics(Pai("?")), Metrics(Pai("?"))};
            else
                return {Metrics(Pai("?"))};
        }
        
        for (int j = 0; j < metrics.size(); j++) {
            metrics[j].taken = pai;
        }
        
        // ShantenAnalysis analysis = ShantenAnalysis(tehais, 1);
        
        vector<Metrics> tmp_metrics = getHoraEstimationInternal(
                                                                nakanai_analysis, tehais, furos, doraMarker, jibunNoKawa,
                                                                true, // none
                                                                std::max(0, numRemainPai / 4), richiBou, honba, visiblePais,
                                                                RESPONSE_TYPE_MJPIR_NONE);
        
        vector<Metrics> ret_metrics;
        for (int j = 0; j < metrics.size(); j++) {
            // add furo
            if (metrics[j].shanten >= tmp_metrics[0].shanten
                && metrics[j].averageHoraPoints <= tmp_metrics[0].averageHoraPoints) {
                metrics[j].horaProb /= 4;
            }
            //      else {
            //          metrics[j].horaProb *= 1.6;
            //          if(metrics[j].horaProb > 1) metrics[j].horaProb = 1;
            //      }
            
            //      if (metrics[j].horaProb > tmp_metrics[0].horaProb &&
            //          metrics[j].shanten <= 4) {
            //
            //        ret_metrics.push_back(metrics[j]);
            //      } else {
            //        cout << "ignore furo" << metrics[j].horaProb << " - "
            //             << tmp_metrics[0].horaProb << endl;
            //      }
            
            ret_metrics.push_back(metrics[j]);
            
            // cout << metrics[j].horaProb << " - " << tmp_metrics[0].horaProb <<
            // endl;
            //
            //      if (metrics[j].horaProb > tmp_metrics[0].horaProb &&
            //          metrics[j].shanten <= 4 &&
            //          tmp_metrics[0].shanten > metrics[j].shanten) {
            //        ret_metrics.push_back(metrics[j]);
            //      } else {
            //        if (tmp_metrics[0].shanten <= metrics[j].shanten &&
            //            metrics[j].horaProb / 2 > tmp_metrics[0].horaProb &&
            //            metrics[j].averageHoraPoints >=
            //            tmp_metrics[0].averageHoraPoints &&
            //            metrics[j].shanten <= 4) {
            //          ret_metrics.push_back(metrics[j]);
            //        }
            //      }
        }
        // no furo
        ret_metrics.push_back(tmp_metrics[0]);
        
        // margin
        if (ret_metrics.size() == 1 && existPossibleFuro) {
            ret_metrics.push_back(Metrics(Pai("?")));
        }
        
        return ret_metrics;
    }
    
    std::vector<Pai> randomeTsumoYama(std::vector<Pai> invisiblePaiSetVec) {
        return invisiblePaiSetVec;
    }
    
    std::vector<Metrics> getHoraEstimationInternal(
                                                   ShantenAnalysis &analysis, std::vector<Pai> tehais,
                                                   std::vector<Furo> furos, std::vector<Pai> doraMarker,
                                                   std::vector<Pai> jibunNoKawa, bool none, int numRemainTurn, int richiBou,
                                                   int honba, std::vector<Pai> visiblePais,
                                                   int type = RESPONSE_TYPE_MJPIR_SUTEHAI, Pai tsumohai = Pai("?")) {
        
        std::vector<Metrics> ret;
        
        // add Chitoitu and Kokushi goals if each shanten under 3
        if (analysis.shanten >= 2) {
            addChitoituGoals(tehais, furos, &analysis.goals);
            addKokushiMusouGoals(tehais, furos, &analysis.goals);
        }
        
        bool existZeroPtFuro[NUM_IDS] = {false};
        
        std::vector<GOALS> goals;
        for (int i = 0; i < analysis.goals.size(); i++) {
            if (type == RESPONSE_TYPE_MJPIR_REACH &&
                analysis.goals[i].minimumShanten()) {
                continue;
            }
            // If shanten > 3, including goals with extra pais is too slow.
            if (analysis.goals[i].minimumShanten() > 2 && analysis.shanten > 3 &&
                analysis.goals[i].shanten > analysis.shanten) {
                continue;
            }
            
            analysis.goals[i].requiredBitVectors =
            BitVector(analysis.goals[i].requiredVector);
            analysis.goals[i].furos = furos;
            
            calculateFan(&analysis.goals[i], tehais, furos,
                         type == RESPONSE_TYPE_MJPIR_REACH ? "now" : "never",
                         doraMarker, none, richiBou, honba);
            if (analysis.goals[i].points > 0.0) {
                goals.push_back(analysis.goals[i]);
            } else if (MJHoraEstimator::isFuroType(type)) {
                for (int k = 0; k < NUM_IDS; k++) {
                    existZeroPtFuro[k] =
                    existZeroPtFuro[k] | (analysis.goals[i].throwableVector[k] > 0);
                }
            }
        }
#ifndef RELEASE_BUILD
        printf("goals size = %lu\n", goals.size());
#endif
        PaiSet visiblePaiSet = PaiSet(visiblePais);
        PaiSet invisiblePaiSet = PaiSet::getAll();
        invisiblePaiSet.removePaiset(visiblePaiSet);
        
        std::vector<Pai> invisiblePaiSetVec;
        std::vector<Pai> tmpInvisiblePaiSetVec =
        randomeTsumoYama(invisiblePaiSet.toPais());
        
        // フリテン防ぐため自分が捨てた牌は除く
        // TODO 待ち考慮
        for (int n = 0; n < tmpInvisiblePaiSetVec.size(); n++) {
            bool exist = false;
            for (int m = 0; m < jibunNoKawa.size(); m++) {
                if (jibunNoKawa[m].equal(tmpInvisiblePaiSetVec[n])) {
                    exist = true;
                    break;
                }
            }
            if (!exist)
                invisiblePaiSetVec.push_back(tmpInvisiblePaiSetVec[n]);
        }
        
        int numTsumos = numRemainTurn;
        
        if (numTsumos < 2) {
            numTsumos = 2;
        }
        
        int numTries; // = MONTE_SIZE; // min((int)goals.size() * 10, MONTE_SIZE);
        std::atomic<int> totalHoraVector[NUM_IDS + 1];
        std::atomic<int> totalHoraJyunmeVector[NUM_IDS + 1];
        std::vector<std::vector<HoraProbDistStruct>> totalPointsFreqsVector(
                                                                            NUM_IDS + 1);
        std::atomic<long long> totalPointsVector[NUM_IDS + 1];
        std::atomic<int> totalYakuToFanVector[NUM_IDS + 1];
        memset(totalHoraVector, 0, sizeof(totalHoraVector));
        memset(totalHoraJyunmeVector, 0, sizeof(totalHoraJyunmeVector));
        memset(totalPointsVector, 0, sizeof(totalPointsVector));
        memset(totalYakuToFanVector, 0, sizeof(totalYakuToFanVector));
        
        //// 再現性を持たせるため乱数固定
        // srand(1);
        
        // シャンテン計算
        int shantenVector[NUM_IDS + 1];
        int chitoiShantenVector[NUM_IDS + 1];
        int kokushiShantenVector[NUM_IDS + 1];
        for (int i = 0; i < NUM_IDS + 1; i++) {
            shantenVector[i] = INF;
            chitoiShantenVector[i] = INF;
            kokushiShantenVector[i] = INF;
        }
        shantenVector[NUM_IDS] = analysis.shanten;
        chitoiShantenVector[NUM_IDS] = analysis.chitoitu_shanten;
        for (int i = 0; i < analysis.goals.size(); i++) {
            for (int pid = 0; pid < NUM_IDS; pid++) {
                if (analysis.goals[i].throwableVector[pid] > 0 &&
                    analysis.goals[i].shanten < shantenVector[pid]) {
                    shantenVector[pid] = analysis.goals[i].shanten;
                }
                if (analysis.goals[i].throwableVector[pid] > 0 &&
                    analysis.goals[i].chitoitu_shanten < chitoiShantenVector[pid]) {
                    chitoiShantenVector[pid] = analysis.goals[i].chitoitu_shanten;
                }
                if (analysis.goals[i].throwableVector[pid] > 0 &&
                    analysis.goals[i].kokushi_shanten < kokushiShantenVector[pid]) {
                    kokushiShantenVector[pid] = analysis.goals[i].kokushi_shanten;
                }
            }
        }
        
//        // ロンアガリ
//        // 相手が自分にベタ降りしたと仮定した時の牌
//        std::vector<Pai> randomTsumosIfOpponentBetaoriVec =
//        MJRonHoraEstimator::randomeTsumoYamaIfBetaori(invisiblePaiSetVec,
//                                                      estimator);
//        
//        int randomTsumosIfOpponentBetaoriVecSize =
//        (int)randomTsumosIfOpponentBetaoriVec.size();
        
        // random
        std::random_device rd;
        std::mt19937 mt(rd());
        
        //---------------------------------------------------------------------------------
        // thread start
        //---------------------------------------------------------------------------------
        
        // グローバル領域にmtxを用意
        static std::mutex mtx;
        
        std::vector<std::thread> threads;
        
        // スレッド数
        numTries = MONTE_SIZE / THREAD_NUM;
        
        auto start = std::chrono::system_clock::now(); // 計測スタート時刻を保存
        
        for (int threadSuu = 0; threadSuu < THREAD_NUM; threadSuu++) {
            
            threads.push_back(
                              std::thread([invisiblePaiSetVec, &mt, numTries, numTsumos, goals,
                                           furos, &totalHoraVector,
                                           &totalPointsVector, &totalPointsFreqsVector, start,
                                           &totalHoraJyunmeVector, type] {
                
                for (int j = 0; j < numTries; j++) {
                    std::vector<Pai> randomTsumos;
                    std::vector<Pai> randomTsumosBetaori;
                    
                    // std::random_shuffle(invisiblePaiSetVec.begin(),
                    // invisiblePaiSetVec.end());
                    
                    //              std::shuffle(std::begin(invisiblePaiSetVec),
                    //                           std::end(invisiblePaiSetVec),
                    //                           mt); // Vectorをシャッフル
                    
                    int invisiblePaiSetVecSize = (int)invisiblePaiSetVec.size();
                    for (int i = 0; i < numTsumos; i++) {
                        randomTsumos.push_back(
                                               invisiblePaiSetVec[mt() % invisiblePaiSetVecSize]);
                    }
                    
                    // PaiSet tsumoVector = PaiSet(randomTsumos);
                    // BitVector tsumoBitVectors = BitVector(tsumoVector.array);
                    
                    int horaVector[NUM_IDS + 1];
                    int horaJyunmeVector[NUM_IDS + 1];
                    int pointsVector[NUM_IDS + 1];
                    int yakuToFanVector[NUM_IDS + 1];
                    memset(horaVector, 0, sizeof(horaVector));
                    memset(horaJyunmeVector, 0, sizeof(horaJyunmeVector));
                    memset(pointsVector, 0, sizeof(pointsVector));
                    memset(yakuToFanVector, 0, sizeof(yakuToFanVector));
                    
                    for (int k = 0; k < goals.size(); k++) {
                        
                        bool achieved = false;
                        //                bool tenpaiAchieve = false;
                        int total = 0;
                        
                        // if (goals[k].requiredBitVectors.isSubsetOf(tsumoBitVectors))
                        // {
                        
                        int tmpRequiredVector[NUM_IDS];
                        memcpy(tmpRequiredVector, goals[k].requiredVector,
                               sizeof(tmpRequiredVector));
                        
                        for (int pid = 0; pid < NUM_IDS; pid++)
                            total += tmpRequiredVector[pid];
                        
                        int n = 0;
                        for (; n < numTsumos; n++) {
                            
//                            if (total > 1 || (j < numTries / 4) // 1/4がツモあがり評価
//                                //|| furos.size() > 0
//                                )
                            {
                                
                                if (tmpRequiredVector[randomTsumos[n].identifier] > 0) {
                                    tmpRequiredVector[randomTsumos[n].identifier]--;
                                    total--;
                                    n++;
                                }
                            }
//                            else {
//                                
//                                // tenpai
//                                // ロンアガリ
//                                Pai p = randomTsumosIfOpponentBetaoriVec
//                                [mt() % randomTsumosIfOpponentBetaoriVecSize];
//                                if (tmpRequiredVector[p.identifier] > 0) {
//                                    tmpRequiredVector[p.identifier]--;
//                                    total--;
//                                    n++;
//                                }
//                            }

                            if (total == 0) {
                                // hora
                                achieved = true;
                                break;
                            }
                        }
                        if (achieved) {
                            for (int pid = 0; pid < NUM_IDS + 1; pid++) {
                                
                                if (pid == NUM_IDS || goals[k].throwableVector[pid] > 0) {
                                    // あがったかどうかじゃなくて
                                    // 残り何牌でにあがったか
                                    horaVector[pid] = 1;
                                    horaJyunmeVector[pid] = numTries - n + 1;
                                    
                                    if (goals[k].points > pointsVector[pid]) {
                                        pointsVector[pid] = goals[k].points;
                                        yakuToFanVector[pid] = 0;
                                    }
                                }
                            }
                        }
                    }
                    
                    mtx.lock();
                    
                    for (int pid = 0; pid < NUM_IDS + 1; pid++) {
                        //                if (tenpaiVector[pid] == 1) {
                        //                  ++totalTenpaiVector[pid];
                        //                }
                        if (horaVector[pid] == 1) {
                            ++totalHoraVector[pid];
                            totalHoraJyunmeVector[pid] += horaJyunmeVector[pid];
                            
                            int points = pointsVector[pid];
                            totalPointsVector[pid] += points;
                            
                            bool exist = false;
                            for (int m = 0; m < totalPointsFreqsVector[pid].size(); m++) {
                                if (totalPointsFreqsVector[pid][m].points == points) {
                                    totalPointsFreqsVector[pid][m].freq++;
                                    exist = true;
                                    break;
                                }
                            }
                            if (!exist) {
                                
                                HoraProbDistStruct tmp;
                                tmp.points = points;
                                tmp.freq = 1;
                                tmp.pid = pid;
                                totalPointsFreqsVector[pid].push_back(tmp);
                            }
                        }
                    }
                    
                    mtx.unlock();
                    
                    auto end = std::chrono::system_clock::now(); // 計測終了時刻を保存
                    auto dur = end - start; // 要した時間を計算
                    auto msec =
                    std::chrono::duration_cast<std::chrono::milliseconds>(dur)
                    .count();
                    // 最大2秒
                    if (msec > 2000) {
                        break;
                    }
                }
            }));
        }
        
        for (std::thread &th : threads) {
            th.join();
        }
        
        //---------------------------------------------------------------------------------
        // thread end
        //---------------------------------------------------------------------------------
        
        vector<int> ryanmenWork;
        
        for (int i = 0; i < tehais.size(); i++) {
            
            if (type == RESPONSE_TYPE_MJPIR_REACH &&
                (shantenVector[tehais[i].identifier] > 0 &&
                 chitoiShantenVector[tehais[i].identifier] > 0 &&
                 kokushiShantenVector[tehais[i].identifier] > 0)) {
                    continue;
                }
            
            if (i < tehais.size() - 1 && tehais[i].equal(tehais[i + 1]))
                continue;
            
            int pid;
            std::string key;
            if (!none) {
                pid = tehais[i].identifier;
                key = tehais[i].toString();
            } else {
                pid = NUM_IDS;
                key = "?";
            }
            Metrics met;
            met.pai = Pai(key);
            met.horaProb =
            //(double)totalHoraVector[pid] / max((double)1, (double)MONTE_SIZE);
            (double)totalHoraJyunmeVector[pid] /
            max((double)1, (MONTE_SIZE * (double)numTries));
            
            
//            // ryanmen
//            int ryanmen = countRyanmen(tehais,met.pai);
//            //cout << ryanmen << endl;
//            met.ryanmenSuu = ryanmen;
//            ryanmenWork.push_back(ryanmen);
            
            //      if (isKoritsuHai(tehais, met.pai) && !isDora(doraMarker,
            //      met.pai))
            //      {
            //        met.horaProb *= 2;
            //      }
            
            //      if (chitoiShantenVector[pid] == 0 && type ==
            //      RESPONSE_TYPE_MJPIR_REACH) {
            //        met.horaProb /= 1.25;
            //      }
            
            //      met.tenpaiProb = (double)totalTenpaiVector[pid] /
            //      (double)MONTE_SIZE;
            met.averageHoraPoints =
            totalHoraVector[pid] > 0
            ? (double)totalPointsVector[pid] / (double)totalHoraVector[pid]
            : 0;
            std::map<std::vector<double>, double> d;
            
            // 四捨五入テスト
            met.averageHoraPoints = (met.averageHoraPoints % 1000 > 0)
            ? met.averageHoraPoints + 500 -
            (met.averageHoraPoints + 500) % 1000
            : met.averageHoraPoints;
            
            for (int m = 0; m < totalPointsFreqsVector[pid].size(); m++) {
                vector<double> tmpPVec = {
                    (double)(totalPointsFreqsVector[pid][m].points)};
                double tmpProb =
                totalHoraVector[pid] ? //
                (double)((double)totalPointsFreqsVector[pid][m].freq /
                         (double)totalHoraVector[pid])
                : 0;
                std::map<std::vector<double>, double> tmpd;
                d[tmpPVec] = tmpProb;
            }
            met.expectedHoraPoints =
            (double)totalPointsVector[pid] / (double)MONTE_SIZE;
            met.shanten = shantenVector[pid];
            met.chitoitu_shanten = chitoiShantenVector[pid];
            met.kokushi_shanten = kokushiShantenVector[pid];
            met.type = type;
            met.tsumogiri = tsumohai.equal(met.pai);
            
            // std::cout << met.horaProb << " | " << met.shanten<< std::endl;
            
            // met.horaProb = met.horaProb / ((double)(met.shanten + 1) *
            // (double)(met.shanten + 1));
            
            //こういう適当なのは良くないと思う
            if (!MJHoraEstimator::isFuroType(type) &&
                type != RESPONSE_TYPE_MJPIR_NONE) {
                
                int herasuRyou = met.shanten;
                
                if (met.kokushi_shanten <= 2) {
                    herasuRyou = met.kokushi_shanten;
                } else if (analysis.shanten - 1 > met.chitoitu_shanten) {
                    herasuRyou = met.chitoitu_shanten;
                }
                
                for (int herasu = 0; herasu < herasuRyou; herasu++) {
                    met.horaProb /= 1.5;
                }
                if (met.horaProb > 1)
                    met.horaProb = 1;
            }
            
            //      if (MJHoraEstimator::isFuroType(type) &&
            //          !MJHoraEstimator::isAnkanType(type)) {
            //        met.horaProb *= 1.2;
            //        if (met.horaProb > 1)
            //          met.horaProb = 1;
            //      }
            
            // if(type == RESPONSE_TYPE_MJPIR_REACH ) met.horaProb /= 1.5;
            
            // std::cout << met.horaProb << std::endl;
            
            if (existZeroPtFuro[met.pai.identifier]) {
                std::cout << "existZeroPtFuro " << met.pai.toString() << std::endl;
                if (met.shanten <= 1) {
                    met.horaProb /= 1.3;
                } else {
                    met.horaProb /= 3;
                }
            }
            
            ret.push_back(met);
            if (none) {
                break;
            }
        }
        
        ret = getTenpaiProbWithCaluculatedMetrics(ret, tehais, furos, jibunNoKawa,
                                                  numRemainTurn, visiblePais, type,
                                                  tsumohai);
//        if(!MJHoraEstimator::isFuroType(type) &&ryanmenWork.size() > 0) {
//            vector<Metrics> tmpForRyanmen;
//            int maxRyanmen = *std::max_element(ryanmenWork.begin(), ryanmenWork.end());
//            for(Metrics r : ret) {
//                if(maxRyanmen > 0 && r.ryanmenSuu == maxRyanmen) {
//                    r.horaProb *= 1.1;
//                }
//                tmpForRyanmen.push_back(r);
//            }
//            ret = tmpForRyanmen;
//        }
        
        return ret;
    }
    
    //
    int countRyanmen(vector<Pai> tehai, Pai p) {
        int ret = 0;
        vector<Pai> work;
        bool skip = false;
        for(auto t : tehai) {
            if(!skip & t.hasSameSymbol(p)) {
                skip = true;
                continue;
            }
            work.push_back(t);
        }
        for(auto w : work) {
            if(w.type != "t" && w.number >= 2 && w.number <= 7) {
                for(auto w2 : work) {
                    if(w2.next(1).hasSameSymbol(w)) {
                        ret++;
                        break;
                    }
                }
            }
        }
        return ret;
    }
    
    //テンパイ率のみ計算
    std::vector<Metrics> getTenpaiProbWithCaluculatedMetrics(
                                                             std::vector<Metrics> met, std::vector<Pai> tehais,
                                                             std::vector<Furo> furos, std::vector<Pai> jibunNoKawa, int numRemainTurn,
                                                             std::vector<Pai> visiblePais, int type = RESPONSE_TYPE_MJPIR_SUTEHAI,
                                                             Pai tsumohai = Pai("?")) {
        
        ShantenAnalysis analysis = ShantenAnalysis(tehais, 0);
        
        int mote_size_for_tenpai = 100;
        
        // add Chitoitu and Kokushi goals if each shanten under 3
        addChitoituGoals(tehais, furos, &analysis.goals);
        addKokushiMusouGoals(tehais, furos, &analysis.goals);
        
        std::vector<GOALS> goals;
        for (int i = 0; i < analysis.goals.size(); i++) {
            analysis.goals[i].requiredBitVectors =
            BitVector(analysis.goals[i].requiredVector);
            analysis.goals[i].furos = furos;
            
            // 形テンは0点でもいい
            goals.push_back(analysis.goals[i]);
        }
        
        PaiSet visiblePaiSet = PaiSet(visiblePais);
        PaiSet invisiblePaiSet = PaiSet::getAll();
        invisiblePaiSet.removePaiset(visiblePaiSet);
        
        std::vector<Pai> invisiblePaiSetVec =
        randomeTsumoYama(invisiblePaiSet.toPais());
        
        int numTsumos = numRemainTurn;
        int numTries = mote_size_for_tenpai;
        
        if (numTsumos < 2)
            numTsumos = 2;
        
        std::atomic<int> totalTenpaiVector[NUM_IDS + 1];
        
        memset(totalTenpaiVector, 0, sizeof(totalTenpaiVector));
        
        // シャンテン計算
        int shantenVector[NUM_IDS + 1];
        int chitoiShantenVector[NUM_IDS + 1];
        for (int i = 0; i < NUM_IDS + 1; i++) {
            shantenVector[i] = INF;
            chitoiShantenVector[i] = INF;
        }
        shantenVector[NUM_IDS] = analysis.shanten;
        chitoiShantenVector[NUM_IDS] = analysis.chitoitu_shanten;
        
        for (int i = 0; i < analysis.goals.size(); i++) {
            for (int pid = 0; pid < NUM_IDS; pid++) {
                if (analysis.goals[i].throwableVector[pid] > 0 &&
                    analysis.goals[i].shanten < shantenVector[pid]) {
                    shantenVector[pid] = analysis.goals[i].shanten;
                }
                if (analysis.goals[i].throwableVector[pid] > 0 &&
                    analysis.goals[i].chitoitu_shanten < chitoiShantenVector[pid]) {
                    chitoiShantenVector[pid] = analysis.goals[i].chitoitu_shanten;
                }
            }
        }
        
        // random
        std::random_device rd;
        std::mt19937 mt(rd());
        
        for (int j = 0; j < numTries; j++) {
            std::vector<Pai> randomTsumos;
            
            int invisiblePaiSetVecSize = (int)invisiblePaiSetVec.size();
            for (int i = 0; i < numTsumos; i++) {
                randomTsumos.push_back(
                                       invisiblePaiSetVec[mt() % invisiblePaiSetVecSize]);
            }
            
            int tenpaiVector[NUM_IDS + 1];
            memset(tenpaiVector, 0, sizeof(tenpaiVector));
            
            for (int k = 0; k < goals.size(); k++) {
                
                bool tenpaiAchieve = false;
                int total = 0;
                
                int tmpRequiredVector[NUM_IDS];
                memcpy(tmpRequiredVector, goals[k].requiredVector,
                       sizeof(tmpRequiredVector));
                
                for (int pid = 0; pid < NUM_IDS; pid++)
                    total += tmpRequiredVector[pid];
                
                for (int n = 0; n < numTsumos; n++) {
                    
                    if (tmpRequiredVector[randomTsumos[n].identifier] > 0) {
                        tmpRequiredVector[randomTsumos[n].identifier]--;
                        total--;
                    }
                    
                    if (total <= 1) {
                        tenpaiAchieve = true;
                        break;
                    }
                }
                if (tenpaiAchieve) {
                    for (int pid = 0; pid < NUM_IDS + 1; pid++) {
                        if (pid == NUM_IDS || goals[k].throwableVector[pid] > 0) {
                            tenpaiVector[pid] = 1;
                        }
                    }
                }
            }
            for (int pid = 0; pid < NUM_IDS + 1; pid++) {
                if (tenpaiVector[pid] == 1) {
                    ++totalTenpaiVector[pid];
                }
            }
        }
        
        // result
        for (int i = 0; i < met.size(); i++) {
            int pid = met[i].pai.identifier;
            if (pid > -1) {
                met[i].tenpaiProb =
                (double)totalTenpaiVector[pid] / (double)mote_size_for_tenpai;
            }
        }
        return met;
    }
    
    bool isDora(vector<Pai> doraMarkers, Pai p) {
        
        if (p.red)
            return true;
        
        for (Pai d : doraMarkers) {
            if (d.nextForDora().equal(p))
                return true;
        }
        return false;
    }
    
    bool isKoritsuHai(vector<Pai> tehais, Pai p) {
        int t[NUM_IDS] = {0};
        for (int i = 0; i < tehais.size(); i++) {
            t[tehais[i].identifier]++;
        }
        
        //        cout << "isKoritsuHai" << endl;
        //        cout << p.toString() << endl;
        //        for(int i = 0; i < NUM_IDS; i++) {
        //            cout << t[i];
        //        }
        
        int i = p.identifier;
        if (i >= 27) {
            if (t[i] == 1)
                return true;
        } else {
            if (p.number == 1) {
                if (t[i] == 1 && t[i + 1] == 0 && t[i + 2] == 0) {
                    return true;
                }
            } else if (p.number == 2) {
                if (t[i] == 1 && t[i - 1] == 0 && t[i + 1] == 0 && t[i + 2] == 0) {
                    return true;
                }
            } else if (p.number == 8) {
                if (t[i] == 1 && t[i + 1] == 0 && t[i - 1] == 0 && t[i - 2] == 0) {
                    return true;
                }
            } else if (p.number == 9) {
                if (t[i] == 1 && t[i - 1] == 0 && t[i - 2] == 0) {
                    return true;
                }
            } else {
                if (t[i] == 1 && t[i + 2] == 0 && t[i + 1] == 0 && t[i - 1] == 0 &&
                    t[i - 2] == 0) {
                    return true;
                }
            }
        }
        
        // cout << "return false";
        return false;
    }
    
    void addChitoituGoals(std::vector<Pai> tehais, std::vector<Furo> furos,
                          std::vector<GOALS> g[]) {
        
        int chitoituShanten = getChitoituShanten(tehais, furos);
        // 3シャンテン以上は無視
        if (chitoituShanten >= 3) {
            return;
        }
        int normalShanten = INF; // getNormalShanten(tehais, furos);
        
        // setup
        std::vector<std::vector<int>> workVec;
        int tehaiVec[NUM_IDS];
        for (int i = 0; i < NUM_IDS; i++) {
            tehaiVec[i] = 0;
        }
        for (int i = 0; i < tehais.size(); i++)
            tehaiVec[tehais[i].identifier]++;
        
        // create hora pattern
        std::vector<int> fixVec;
        std::vector<int> candidateVec;
        for (int i = 0; i < NUM_IDS; i++) {
            if (tehaiVec[i] > 0) {
                if (tehaiVec[i] >= 2) {
                    fixVec.push_back(i);
                } else if (tehaiVec[i] == 1) {
                    candidateVec.push_back(i);
                }
            }
        }
        // if hai kind under 7 then not eval
        if (candidateVec.size() + fixVec.size() < 7) {
            return;
        }
        std::vector<int> baseHoraPattern;
        for (int i = 0; i < NUM_IDS; i++) {
            baseHoraPattern.push_back(0);
        }
        for (int i = 0; i < fixVec.size(); i++) {
            baseHoraPattern[fixVec[i]] = 2;
        }
        for (int i = 0; i < candidateVec.size(); i++) {
            int added = 0;
            std::vector<int> tmp = baseHoraPattern;
            for (int j = i; j < candidateVec.size(); j++) {
                tmp[candidateVec[j]] = 2;
                added++;
                if (fixVec.size() + added == 7) {
                    workVec.push_back(tmp);
                    break;
                }
            }
        }
        
        // for all hora pattern
        for (int idx = 0; idx < workVec.size(); idx++) {
            // set GOALS
            GOALS tmp;
            for (int i = 0; i < NUM_IDS; i++) {
                tmp.countVector[i] = workVec[idx][i];
            }
            int requiredVector[NUM_IDS];
            int throwableVector[NUM_IDS];
            for (int i = 0; i < NUM_IDS; i++) {
                requiredVector[i] = workVec[idx][i] - tehaiVec[i];
                throwableVector[i] = tehaiVec[i] - workVec[idx][i];
            }
            for (int i = 0; i < NUM_IDS; i++) {
                tmp.requiredVector[i] = requiredVector[i];
                tmp.throwableVector[i] = throwableVector[i];
                if (tmp.requiredVector[i] < 0)
                    tmp.requiredVector[i] = 0;
                if (tmp.throwableVector[i] < 0)
                    tmp.throwableVector[i] = 0;
            }
            tmp.shanten = normalShanten;
            tmp.chitoitu_shanten = chitoituShanten;
            
            g->push_back(tmp);
        }
    }
    void addKokushiMusouGoals(std::vector<Pai> tehais, std::vector<Furo> furos,
                              std::vector<GOALS> g[]) {
        // if kokushiShanten more than 3 then not eval
        int kokushiShanten = getKokushiMusouShanten(tehais, furos);
        if (kokushiShanten > 3) {
            return;
        }
        // setup
        int yaotyuu[] = {0, 8, 9, 17, 18, 26, 27, 28, 29, 30, 31, 32, 33};
        int workVec[NUM_IDS];
        int tehaiVec[NUM_IDS];
        for (int i = 0; i < NUM_IDS; i++) {
            workVec[i] = 0;
            tehaiVec[i] = 0;
        }
        for (int i = 0; i < tehais.size(); i++)
            tehaiVec[tehais[i].identifier]++;
        
        for (int i = 0; i < 13; i++)
            workVec[yaotyuu[i]]++;
        
        // for all hora pattern
        for (int i = 0; i < 13; i++) {
            workVec[yaotyuu[i]]++;
            
            // set GOALS
            GOALS tmp;
            for (int i = 0; i < 13; i++) {
                tmp.countVector[yaotyuu[i]] = workVec[yaotyuu[i]];
            }
            int requiredVector[NUM_IDS];
            int throwableVector[NUM_IDS];
            for (int i = 0; i < NUM_IDS; i++) {
                requiredVector[i] = workVec[i] - tehaiVec[i];
                throwableVector[i] = tehaiVec[i] - workVec[i];
            }
            for (int i = 0; i < NUM_IDS; i++) {
                tmp.requiredVector[i] = requiredVector[i];
                tmp.throwableVector[i] = throwableVector[i];
                if (tmp.requiredVector[i] < 0)
                    tmp.requiredVector[i] = 0;
                if (tmp.throwableVector[i] < 0)
                    tmp.throwableVector[i] = 0;
            }
            tmp.shanten = INF;
            tmp.chitoitu_shanten = INF;
            tmp.kokushi_shanten = kokushiShanten;
            g->push_back(tmp);
            
            workVec[yaotyuu[i]]--;
        }
    }
    
    void calculateFan(GOALS *goal, std::vector<Pai> tehais,
                      std::vector<Furo> furos, std::string reachMode,
                      std::vector<Pai> doraMarkers, bool none, int richiBou,
                      int honba) {
        
        int tehai4MJscore[41];
        memset(tehai4MJscore, 0, sizeof(tehai4MJscore));
        
        int furos4MJscore[20];
        memset(furos4MJscore, 0, sizeof(furos4MJscore));
        
        for (int i = 0; i < NUM_IDS; i++) {
            tehai4MJscore[Util::convertMJAIPainumToMjscore(i)] = goal->countVector[i];
        }
        
        for (int i = 0; i < tehais.size(); i++) {
            if (tehais[i].red && goal->countVector[tehais[i].identifier]) {
                if (tehais[i].type == "m") {
                    tehai4MJscore[5]--;
                    tehai4MJscore[38]++;
                } else if (tehais[i].type == "p") {
                    tehai4MJscore[15]--;
                    tehai4MJscore[39]++;
                    
                } else if (tehais[i].type == "s") {
                    tehai4MJscore[25]--;
                    tehai4MJscore[40]++;
                }
            }
        }
        
        if (goal->chitoitu_shanten < 3) {
            // cout << goal->chitoitu_shanten << endl;
        } else {
            
            for (int num = 0; num < furos.size(); num++) {
                int i = num * 4;
                int nakiNum =
                Util::convertMJAIPainumToMjscore(furos[num].consumed[0].identifier);
                if (furos[num].type == "chi") {
                    
                    nakiNum = Util::convertMJAIPainumToMjscore(
                                                               furos[num].smallestPai().identifier);
                    
                    furos4MJscore[i] = MJScore::CHII;
                    if (furos[num].hasRed() > 0) {
                        furos4MJscore[i] = MJScore::AKA_CHII;
                    }
                    furos4MJscore[i + 1] = furos4MJscore[i + 2] = nakiNum;
                } else if (furos[num].type == "pon") {
                    furos4MJscore[i] = MJScore::PON;
                    if (furos[num].hasRed() == 1) {
                        furos4MJscore[i] = MJScore::AKA_PON1;
                    } else if (furos[num].hasRed() == 2) {
                        furos4MJscore[i] = MJScore::AKA_PON2;
                    }
                    furos4MJscore[i + 1] = furos4MJscore[i + 2] = nakiNum;
                } else if (furos[num].type == "daiminkan") {
                    furos4MJscore[i] = MJScore::MINKAN;
                    furos4MJscore[i + 1] = furos4MJscore[i + 2] = nakiNum;
                } else if (furos[num].type == "kakan") {
                    furos4MJscore[i] = MJScore::KAKAN;
                    furos4MJscore[i + 1] = furos4MJscore[i + 2] = nakiNum;
                } else if (furos[num].type == "ankan") {
                    furos4MJscore[i] = MJScore::ANKAN;
                    furos4MJscore[i + 1] = furos4MJscore[i + 2] = nakiNum;
                }
            }
        }
        
        int doras4MJscore[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        int doracnt = 0;
        for (int i = 0; i < doraMarkers.size(); i++) {
            doras4MJscore[i + 1] =
            Util::convertMJAIPainumToMjscore(doraMarkers[i].identifier);
            doracnt++;
        }
        doras4MJscore[0] = doracnt;
        
        int agariPaiIdx = 0;
        for (int i = 0; i < NUM_IDS; i++) {
            if (goal->requiredVector[i] > 0) {
                agariPaiIdx = Util::convertMJAIPainumToMjscore(i);
                break;
            }
        }
        
        MJScore score;
        score.Set_Tehai(tehai4MJscore);
        score.Set_Agarihai(agariPaiIdx);
        score.Set_Bakaze(Util::convertMJAIPainumToMjscore(bakaze.identifier));
        score.Set_Jikaze(Util::convertMJAIPainumToMjscore(jikaze.identifier));
        // TODO
        // 本当か？true？
        score.Set_Tumoagari(false);
        score.Set_Honba(honba);
        score.Set_KyoutakuRIichi(richiBou);
        score.Set_Fuuro(furos4MJscore);
        score.Set_Dora(doras4MJscore);
        score.Avail_Akahai(true);
        
        // リーチ
        bool isRichi = reachMode != "never";
        if (furos.size() == 0)
            isRichi = true;
        if (furos.size() > 0)
            isRichi = false;
        
        score.Is_Riichi(isRichi);
        
        score.Avail_DoubleKokushi13(false);
        score.Avail_DoubleTyuuren9(false);
        score.Avail_DoubleDaisuusii(false);
        score.Avail_DoubleSuttan(false);

        
        // TEST
        score.mushiTest = true;
        
        score.Run();
        int err = score.Get_ErrorCode();
        if (err == MJScore::SEIJYOU || err == MJScore::YAKUNASI
            //|| err == MJScore::HAIGA14MAIJYANAI
            ) {
            goal->fu = score.Get_Fu();
            goal->fan = score.Get_Fan();
            
            // ロンアガリの点数で考える
            if (goal->fan > 1 && furos.size() == 0 && reachMode == "never" && !none) {
                score.Decision_Score(goal->fan - 1, goal->fu);
            }
            
            //      // 三色はがちでくそ
            //      bool chkYaku[60];
            //      score.Get_Yakutable(chkYaku);
            //      if (score.Get_Fan() > 1 && chkYaku[MJScore::SANSYOKUDOUJYUN]) {
            //        score.Decision_Score(goal->fan - 1, goal->fu);
            //      }
            
            if (!oya) {
                goal->points = score.Get_TokutenKo();
                
                // goal->points = score.Get_Fan() * 1000;
            } else {
                goal->points = score.Get_TokutenOya();
                
                // goal->points = score.Get_Fan() * 1000 * 2;
            }
            // TEST
            // 四捨五入？
            if (goal->points > 0) {
                
                
                if (reachMode != "never") {
                    // リーチ一発とかツモとか裏ドラとか考慮してハンプラス
                    score.Set_Dorasuu(1);
                    //goal->points += 1000;
                }
                
                // goal->points = (int)((goal->points+500) / 1000) * 1000;
                // goal->points = (int)((goal->points) / 1000) * 1000;
                
                //                // TODO
                //                // 一発とか裏ドラ考えると立直の価値高い
                //                if(reachMode != "never"){
                //                    goal->points *= 2;
                //                }
                
//                        if (goal->points >= 3900)
//                          goal->points *= 1.5;
                

                
                score.Get_Yakuname(goal->yaku);
                
                //                for(std::string s : goal->yaku) {
                //                    if(s!="") {
                //                        cout << s << endl;
                //                    }
                //                }
                //                for (int i = 0; i < NUM_IDS; i++) {
                //                    for (int j = 0; j < goal->countVector[i]; j++) {
                //                        cout<<Pai(i).toString()<<" ";
                //                    }
                //                }
                //                cout << endl;
                //                cout << "agari = " << agariPaiIdx << endl;
            }
        } else {
#ifndef RELEASE_BUILD
            //      cout << "calc tokuten err" << score.Get_ErrorCodeString() << endl;
            //      for (int i = 0; i < NUM_IDS; i++) {
            //        for (int j = 0; j < goal->countVector[i]; j++) {
            //          cout << Pai(i).toString() << " ";
            //        }
            //      }
            //      cout << endl;
            //      for (int num = 0; num < furos.size(); num++) {
            //        cout << furos[num].toString() << " ";
            //      }
            //      cout << "agari = " << agariPaiIdx << endl;
#endif
        }
    }
    
    bool isYakunashiTsumoHora(vector<Pai> tehais, vector<Furo> furos,
                              Pai tsumohai, Pai bakaze, Pai jikaze) {
        
        if (furos.size() == 0)
            return false;
        
        int tehai4MJscore[41];
        int agariPaiIdx = Util::convertMJAIPainumToMjscore(tsumohai.identifier);
        memset(tehai4MJscore, 0, sizeof(tehai4MJscore));
        
        for (int i = 0; i < tehais.size(); i++) {
            if (tehais[i].red) {
                if (tehais[i].type == "m") {
                    tehai4MJscore[38]++;
                } else if (tehais[i].type == "p") {
                    tehai4MJscore[39]++;
                    
                } else if (tehais[i].type == "s") {
                    tehai4MJscore[40]++;
                }
            } else {
                tehai4MJscore[Util::convertMJAIPainumToMjscore(tehais[i].identifier)]++;
            }
        }
        
        int furos4MJscore[20];
        memset(furos4MJscore, 0, sizeof(furos4MJscore));
        for (int num = 0; num < furos.size(); num++) {
            int i = num * 4;
            int nakiNum =
            Util::convertMJAIPainumToMjscore(furos[num].consumed[0].identifier);
            if (furos[num].type == "chi") {
                
                nakiNum = Util::convertMJAIPainumToMjscore(
                                                           furos[num].smallestPai().identifier);
                
                furos4MJscore[i] = MJScore::CHII;
                if (furos[num].hasRed() > 0) {
                    furos4MJscore[i] = MJScore::AKA_CHII;
                }
                furos4MJscore[i + 1] = furos4MJscore[i + 2] = nakiNum;
            } else if (furos[num].type == "pon") {
                furos4MJscore[i] = MJScore::PON;
                if (furos[num].hasRed() == 1) {
                    furos4MJscore[i] = MJScore::AKA_PON1;
                } else if (furos[num].hasRed() == 2) {
                    furos4MJscore[i] = MJScore::AKA_PON2;
                }
                furos4MJscore[i + 1] = furos4MJscore[i + 2] = nakiNum;
            } else if (furos[num].type == "daiminkan") {
                furos4MJscore[i] = MJScore::MINKAN;
                furos4MJscore[i + 1] = furos4MJscore[i + 2] = nakiNum;
            } else if (furos[num].type == "kakan") {
                furos4MJscore[i] = MJScore::KAKAN;
                furos4MJscore[i + 1] = furos4MJscore[i + 2] = nakiNum;
            } else if (furos[num].type == "ankan") {
                furos4MJscore[i] = MJScore::ANKAN;
                furos4MJscore[i + 1] = furos4MJscore[i + 2] = nakiNum;
            }
        }
        
        MJScore score;
        score.Set_Tehai(tehai4MJscore);
        score.Set_Agarihai(agariPaiIdx);
        score.Set_Bakaze(Util::convertMJAIPainumToMjscore(bakaze.identifier));
        score.Set_Jikaze(Util::convertMJAIPainumToMjscore(jikaze.identifier));
        score.Set_Tumoagari(true);
        score.Set_Fuuro(furos4MJscore);
        score.Avail_Akahai(true);
        score.Is_Riichi(false);
        score.Run();
        int err = score.Get_ErrorCode();
        
        if (err == MJScore::SEIJYOU && score.Get_TokutenOya() > 0) {
            cout << "Tsumo Hora OK.Yaku Ari" << endl;
        } else {
            cout << "Yakunashi Tsumo Hora. MJScoreError = " << err << endl;
        }
        return err != MJScore::SEIJYOU;
    }
    
    int getChitoituShanten(std::vector<Pai> tehais, std::vector<Furo> furos) {
        if (furos.size() > 0) {
            return 999;
        }
        int tmpTehai[38];
        for (int i = 0; i < 38; i++)
            tmpTehai[i] = 0;
        for (int i = 0; i < tehais.size(); i++) {
            tmpTehai[Util::convertMJAIPainumToMjscore(tehais[i].identifier)]++;
        }
        Syanten4HoraEstimator s;
        s.set_tehai(tmpTehai);
        int t = s.TiitoituSyanten();
        return std::min(999, t);
    }
    
    int getKokushiMusouShanten(std::vector<Pai> tehais, std::vector<Furo> furos) {
        if (furos.size() > 0) {
            return 999;
        }
        int tmpTehai[38];
        memset(tmpTehai, 0, sizeof(tmpTehai));
        for (int i = 0; i < tehais.size(); i++) {
            tmpTehai[Util::convertMJAIPainumToMjscore(tehais[i].identifier)]++;
        }
        Syanten4HoraEstimator s;
        s.set_tehai(tmpTehai);
        int t = s.KokusiSyanten();
        return std::min(999, t);
    }
    
    int getNormalShanten(std::vector<Pai> tehais, std::vector<Furo> furos) {
        Syanten4HoraEstimator shan;
        int normalShanten[38];
        memset(normalShanten, 0, sizeof(normalShanten));
        for (int i = 0; i < tehais.size(); i++) {
            normalShanten[Util::convertMJAIPainumToMjscore(tehais[i].identifier)]++;
        }
        shan.set_tehai(normalShanten);
        shan.set_fuurosuu((int)furos.size());
        int ret = shan.NormalSyanten();
        return ret;
    }
    
    std::vector<Pai> removePaiVector(Pai pai, std::vector<Pai> tehais) {
        bool checked = false;
        std::vector<Pai> tmp;
        for (int i = 0; i < tehais.size(); i++) {
            if (!checked && pai.hasSameSymbol(tehais[i])) {
                checked = true;
                continue;
            }
            tmp.push_back(tehais[i]);
        }
        return tmp;
    }
    
    std::vector<Pai> ankanPais(std::vector<Pai> tehais) {
        int cnt[NUM_IDS] = {0};
        for (int i = 0; i < tehais.size(); i++) {
            cnt[tehais[i].identifier]++;
        }
        std::vector<Pai> tmp;
        for (int i = 0; i < NUM_IDS; i++) {
            if (cnt[i] == 4)
                tmp.push_back(Pai(i));
        }
        return tmp;
    }
    
    std::vector<Pai> kakanPais(std::vector<Pai> tehais, std::vector<Furo> furos) {
        std::vector<Pai> tmp;
        // TODO
        //            for (int i = 0; i < furos.size(); i++) {
        //                if(furos[i].type == "pon") {
        //                    for (int j = 0; j < tehais.size(); j++) {
        //
        //                    if(furos[i].taken.equal(tehais[j])) {
        //                            tmp.push_back(tehais[j]);
        //                            break;
        //                        }
        //                    }
        //                }
        //            }
        return tmp;
    }
    
    bool canRon(Pai pai, vector<Pai> tehais, std::vector<Furo> furos) {
        std::vector<Pai> _tehais = tehais;
        _tehais.push_back(pai);
        ShantenAnalysis _analysis = ShantenAnalysis(_tehais, 0);
        int shanten = _analysis.shanten;
        bool ret = false;
        if (shanten < 0) {
            ret = true;
        } else {
            int chitoi_shanten = getChitoituShanten(_tehais, furos);
            if (chitoi_shanten < 0) {
                ret = true;
            }
        }
        return ret;
    }
    
    bool canLeftChi(Pai pai, vector<Pai> tehais, int target) {
        if (target != 3)
            return false;
        if (pai.identifier >= Pai("E").identifier) {
            return false;
        }
        std::string paiStr = pai.toString();
        
        if (paiStr.find("8") != string::npos || paiStr.find("9") != string::npos) {
            return false;
        }
        
        int chk[NUM_IDS];
        paiVectorToIntVector(tehais, chk);
        return (chk[pai.next(1).identifier] && chk[pai.next(2).identifier]);
    }
    
    bool canMiddleChi(Pai pai, vector<Pai> tehais, int target) {
        if (target != 3)
            return false;
        if (pai.identifier >= Pai("E").identifier) {
            return false;
        }
        std::string paiStr = pai.toString();
        if (paiStr.find("1") != string::npos || paiStr.find("9") != string::npos) {
            return false;
        }
        int chk[NUM_IDS];
        paiVectorToIntVector(tehais, chk);
        return (chk[pai.next(-1).identifier] && chk[pai.next(1).identifier]);
    }
    
    bool canRightChi(Pai pai, vector<Pai> tehais, int target) {
        if (target != 3)
            return false;
        if (pai.identifier >= Pai("E").identifier) {
            return false;
        }
        std::string paiStr = pai.toString();
        if (paiStr.find("1") != string::npos || paiStr.find("2") != string::npos) {
            return false;
        }
        int chk[NUM_IDS];
        paiVectorToIntVector(tehais, chk);
        return (chk[pai.next(-1).identifier] && chk[pai.next(-2).identifier]);
    }
    
    bool canPon(Pai pai, vector<Pai> tehais) {
        int cnt = 0;
        for (int i = 0; i < tehais.size(); i++) {
            if (tehais[i].hasSameSymbol(pai)) {
                cnt++;
            }
        }
        return cnt >= 2;
    }
    
    bool canDaiminkan(Pai pai, vector<Pai> tehais) {
        
        //   // TODO
        //    return false;
        
        int cnt = 0;
        for (int i = 0; i < tehais.size(); i++) {
            if (tehais[i].equal(pai)) {
                cnt++;
            }
        }
        return cnt == 3;
    }
    
    // direction 0:left chi 1:middle chi 2;right chi
    bool isKuikaeChi(Pai p1, Pai p2, int direction) {
        if (direction == 0 && p2.next(3).hasSameSymbol(p1)) {
            return true;
        }
        if (direction == 2 && p2.next(-3).hasSameSymbol(p1)) {
            return true;
        }
        if (Pai(p1.type, p1.number, false).equal(Pai(p2.type, p2.number, false))) {
            return true;
        }
        return false;
    }
    
    void paiVectorToIntVector(vector<Pai> pais, int ret[]) {
        memset(ret, 0, sizeof(int) * NUM_IDS);
        for (int i = 0; i < pais.size(); i++) {
            ret[pais[i].identifier]++;
        }
    }
    
    int furoSizeWithoutAnkan(std::vector<Furo> furos) {
        int ret = 0;
        for (Furo f : furos) {
            if (f.type != "ankan") {
                ret++;
            }
        }
        return ret;
    }
    
    static bool isFuroType(int type) {
        if (type == RESPONSE_TYPE_MJPIR_PON ||
            type == RESPONSE_TYPE_MJPIR_CHII_LEFT ||
            type == RESPONSE_TYPE_MJPIR_CHII_RIGHT ||
            type == RESPONSE_TYPE_MJPIR_CHII_MIDDLE ||
            type == RESPONSE_TYPE_MJPIR_MINKAN ||
            type == RESPONSE_TYPE_MJPIR_KAKAN ||
            type == RESPONSE_TYPE_MJPIR_ANKAN) {
            return true;
        }
        return false;
    }
    
    static bool isAnkanType(int type) {
        if (
            type == RESPONSE_TYPE_MJPIR_KAKAN ||
            type == RESPONSE_TYPE_MJPIR_ANKAN) {
            return true;
        }
        return false;
    }
};

#endif
