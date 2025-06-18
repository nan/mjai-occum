#ifndef MJAIGame_hpp
#define MJAIGame_hpp

//--------------------------------------------------------
#include "../AverageRankCalculator/AverageRankCalculator.hpp"
#include "../HoraEstimator/MJHoraEstimator.hpp"
#include "../OpponentHoraEstimator/OpponentHoraEstimator.hpp"
#include "../TenpaiProbEstimator/TenpaiProbEstimator.hpp"
//-----------------------**---------------------------------

#include "./furo.h"
#include "./mjscore.h"
#include "./pai.h"
#include "./picojson.h"
#include "./util.h"
#include "prob_dist.h"

#include <map>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>

#include <regex>

#include "./player.h"

#define NUM_INITIAL_PIPAIS 70
#define FINAL_TURN 17.5
#define READ_FILE_BUFFER_SIZE 10000
#define DEFAULT_POINT 25000

#ifdef TENHOUPORT
//#define MY_PLAYER_NAME "NoName"
//#define MY_PLAYER_NAME "ihsimoy"
//#define MY_PLAYER_NAME "ihsoyim"
#define MY_PLAYER_NAME "occam"
#elif RELEASE_BUILD == 1
#define MY_PLAYER_NAME "Occam"
#else
#define MY_PLAYER_NAME "Occam"
#endif

struct TEMPORARY_ACTION {
    int actor;
    Pai pai;
    string type;
    bool tsumogiri;
};

class MJAIGame {
public:
    string key_start_hello = "hello";
    string key_start_game = "start_game";
    string key_end_game = "end_game";
    string key_start_kyoku = "start_kyoku";
    string key_end_kyoku = "end_kyoku";
    string key_tsumo = "tsumo";
    string key_dahai = "dahai";
    string key_reach = "reach";
    string key_reach_accepted = "reach_accepted";
    string key_chi = "chi";
    string key_pon = "pon";
    string key_daiminkan = "daiminkan";
    string key_ankan = "ankan";
    string key_kakan = "kakan";
    string key_hora = "hora";
    string key_ryukyoku = "ryukyoku";
    string key_dora = "dora";
    string key_error = "error";
    
    OpponentHoraEstimator _OpponentHoraEstimator;
    MJHoraEstimator _HoraEstimator;
    
    BKUC1681 bkuc; // 爆打本体
    BKUC1681MajhanUtil bkucUtil;
    TenpaiProbEstimator _TenpaiProbEstimator;
    AverageRankCalculator calculator;
    
    // リーチ後切る牌とか鳴き後切る牌とかに使用する
    string reserved_action = "";
    
public:
    int kyokuNum;
    int honba;
    int richiKyoutaku;
    int numPipais; // nokori tsumo maisuu
    PlayerData players[4];
    Pai bakaze;
    std::vector<Pai> doraMarkers;
    TEMPORARY_ACTION currentAction;
    TEMPORARY_ACTION previousAction;
    int previousActor = -1;
    int chichaNum = 0;
    int me = -1;
    
    int countOfFileNum = 0;
    bool tonpuusen = false;
    
    // for log
    double totalKyokusuu = 0;
    double totalHora = 0;
    double totalHoujyuu = 0;
    double totalFuro = 0;
    double totalRichiWhenHora = 0;
    double totalMenzenHora = 0;
    double totalGame = 0;
    
    double tmpTotalKyokusuu = 0;
    double tmpTotalHora = 0;
    double tmpTotalHoujyuu = 0;
    double tmpTotalFuro = 0;
    double tmpTotalRichiWhenHora = 0;
    double tmpTotalMenzenHora = 0;
    double tmpTotalGame = 0;
    
    double sumOfRank = 0;
    double rankArray[5] = {0};
    
    bool checkFuroSuteForKan = false;
    
    MJAIGame() {
        doraMarkers.clear();
        numPipais = kyokuNum = honba = richiKyoutaku = 0;
        
        bkuc.init_engine();
    }
    
    std::string toString() {
        
#ifdef RELEASE_BUILD
        return "";
#endif
        
        char ch[10240];
        
        std::string dora = "";
        for (Pai p : doraMarkers) {
            dora += p.toString() + " ";
        }
        
        sprintf(ch, "me = %d : kyokuNum = %d : honba = %d : richiKyokutaku = %d : "
                "bakaze = %s : chichaNum = %d : dora = %s \n player0 -> %s \n "
                "player1 -> %s \n player2 -> %s \n player3 -> %s",
                me, kyokuNum, honba, richiKyoutaku, bakaze.toString().c_str(),
                chichaNum, dora.c_str(), players[0].toString().c_str(),
                players[1].toString().c_str(), players[2].toString().c_str(),
                players[3].toString().c_str());
        return ch;
    }
    
    std::string toJson() {
        
#ifdef RELEASE_BUILD
        return "";
#endif
        
        return "";
        
        picojson::object obj;
        
        obj["me"] = (picojson::value)(PlayerData::IntToString(me));
        obj["kyokuNum"] = (picojson::value)(PlayerData::IntToString(kyokuNum));
        obj["honba"] = (picojson::value)(PlayerData::IntToString(honba));
        obj["richiKyoutaku"] =
        (picojson::value)(PlayerData::IntToString(richiKyoutaku));
        obj["chichaNum"] = (picojson::value)(PlayerData::IntToString(chichaNum));
        obj["numPiPais"] = (picojson::value)(PlayerData::IntToString(numPipais));
        
        obj["bakaze"] = PlayerData::toJsonPai(bakaze);
        
        picojson::array v;
        for (Pai p : doraMarkers) {
            v.push_back(PlayerData::toJsonPai(p));
        }
        obj.insert(make_pair("dora", picojson::value(v)));
        
        picojson::array p;
        p.push_back(players[0].toJson());
        p.push_back(players[1].toJson());
        p.push_back(players[2].toJson());
        p.push_back(players[3].toJson());
        
        //        obj["identifier"] =
        //        (picojson::value)(PlayerData::IntToString(++countOfFileNum));
        
        //        picojson::array k;
        //        vector<Pai> machi =
        //        PlayerData::getMachi(players[2].tehais,players[2].furos);
        //        for(Pai p : machi) {
        //            k.push_back(PlayerData::toJsonPai(p));
        //        }
        //        obj.insert(make_pair("kotae", picojson::value(k)));
        
        obj.insert(make_pair("players", picojson::value(p)));
        
        picojson::value val(obj);
        // cout << val.serialize() << endl;
        
        std::string ret_str = val.serialize();
        
        ret_str = std::regex_replace(ret_str, std::regex("\""), "\\\"");
        ret_str = myreplace(ret_str, "{", "\\{");
        ret_str = myreplace(ret_str, "}", "\\}");
        ret_str = myreplace(ret_str, "[", "\\[");
        ret_str = myreplace(ret_str, "]", "\\]");
        //        ret_str = std::regex_replace(ret_str, std::regex("\""), "_A_");
        //        ret_str = myreplace(ret_str,"{","_B_");
        //        ret_str = myreplace(ret_str,"}","_C_");
        //        ret_str = myreplace(ret_str,"[","_D_");
        //        ret_str = myreplace(ret_str,"]","_E_");
        
        cout << ret_str << endl;
        
        return ret_str.c_str();
    }
    
    void fromJson(std::string json) {
        
        init();
        
        picojson::value v;
        std::string err;
        picojson::parse(v, json.c_str(), json.c_str() + strlen(json.c_str()), &err);
        picojson::object obj = v.get<picojson::object>();
        
        numPipais = bkucUtil.toInt(obj["numPiPais"].get<string>());
        me = bkucUtil.toInt(obj["me"].get<string>());
        kyokuNum = bkucUtil.toInt(obj["kyokuNum"].get<string>());
        honba = bkucUtil.toInt(obj["honba"].get<string>());
        richiKyoutaku = bkucUtil.toInt(obj["richiKyoutaku"].get<string>());
        chichaNum = bkucUtil.toInt(obj["chichaNum"].get<string>());
        
        picojson::object obj2 = obj["bakaze"].get<picojson::object>();
        Pai _p = Pai(bkucUtil.toInt(obj2["pai_id"].get<std::string>()));
        bakaze = _p;
        
        picojson::array &doraList = obj["dora"].get<picojson::array>();
        for (picojson::array::iterator it = doraList.begin(); it != doraList.end();
             it++) {
            picojson::object obj2 = (it->get<picojson::object>());
            Pai _p = Pai(bkucUtil.toInt(obj2["pai_id"].get<std::string>()));
            doraMarkers.push_back(_p);
        }
        
        picojson::array &playersList = obj["players"].get<picojson::array>();
        int idx = 0;
        for (picojson::array::iterator it = playersList.begin();
             it != playersList.end(); it++) {
            
            picojson::object obj2 = (it->get<picojson::object>());
            
            players[idx].reachState =
            bkucUtil.toInt(obj2["reachState"].get<string>());
            players[idx].reachSutehaiIndex =
            bkucUtil.toInt(obj2["reachSutehaiIndex"].get<string>());
            players[idx].score = bkucUtil.toInt(obj2["score"].get<string>());
            
            picojson::array &furo_sute = obj2["furo_sute"].get<picojson::array>();
            players[idx].furoSutehai.clear();
            for (picojson::array::iterator it2 = furo_sute.begin();
                 it2 != furo_sute.end(); it2++) {
                picojson::object obj3 = (it2->get<picojson::object>());
                Pai _p = Pai(bkucUtil.toInt(obj3["pai_id"].get<std::string>()));
                players[idx].furoSutehai.push_back(_p.identifier);
            }
            
            picojson::array &sutehais = obj2["sutehais"].get<picojson::array>();
            for (picojson::array::iterator it2 = sutehais.begin();
                 it2 != sutehais.end(); it2++) {
                picojson::object obj3 = (it2->get<picojson::object>());
                Pai _p = Pai(bkucUtil.toInt(obj3["pai_id"].get<std::string>()));
                players[idx].sutehais.push_back(_p);
            }
            
            picojson::array &furos = obj2["furos"].get<picojson::array>();
            for (picojson::array::iterator it2 = furos.begin(); it2 != furos.end();
                 it2++) {
                picojson::object obj3 = (it2->get<picojson::object>());
                
                std::vector<Pai> c;
                picojson::array &consume = obj3["consume"].get<picojson::array>();
                for (picojson::array::iterator it3 = consume.begin();
                     it3 != consume.end(); it3++) {
                    picojson::object obj4 = (it3->get<picojson::object>());
                    Pai _p = Pai(bkucUtil.toInt(obj4["pai_id"].get<std::string>()));
                    c.push_back(_p);
                }
                
                players[idx].furos.push_back(Furo(obj3["type"].get<std::string>(),
                                                  Pai(obj3["pai"].get<std::string>()),
                                                  3, c));
            }
            
            picojson::array &tedashi = obj2["tedashi"].get<picojson::array>();
            for (picojson::array::iterator it2 = tedashi.begin();
                 it2 != tedashi.end(); it2++) {
                players[idx].tedashi.push_back(bkucUtil.toInt(it2->get<string>()));
            }
            if (idx == me) {
                picojson::array &tehais = obj2["tehais"].get<picojson::array>();
                for (picojson::array::iterator it2 = tehais.begin();
                     it2 != tehais.end(); it2++) {
                    picojson::object obj3 = (it2->get<picojson::object>());
                    Pai _p = Pai(bkucUtil.toInt(obj3["pai_id"].get<std::string>()));
                    _p.red = obj3["red"].get<std::string>() == "1" ? true : false;
                    players[idx].tehais.push_back(_p);
                }
                sortTehai(idx);
            }
            idx++;
        }
    }
    
    std::string myreplace(std::string subject, const std::string &search,
                          const std::string &replace) {
        size_t pos = 0;
        while ((pos = subject.find(search, pos)) != std::string::npos) {
            subject.replace(pos, search.length(), replace);
            pos += replace.length();
        }
        return subject;
    }
    
    void sortTehai(int num) {
        int i, j;
        Pai temp;
        for (i = 0; i < players[num].tehais.size() - 1; i++) {
            for (j = (unsigned)players[num].tehais.size() - 1; j > i; j--) {
                if (players[num].tehais[j - 1].identifier >
                    players[num].tehais[j].identifier) {
                    temp = players[num].tehais[j];
                    players[num].tehais[j] = players[num].tehais[j - 1];
                    players[num].tehais[j - 1] = temp;
                }
            }
        }
    }
    
    //  void setTemporaryExtraAnpais() {
    //    // TODO
    //    // more accurate
    //    for (int i = 0; i < 4; i++) {
    //      players[i].extraAnpais.clear();
    //      if (players[i].reachState) {
    //        for (int k = 0; k < 4; k++) {
    //          if (i == k) {
    //            continue;
    //          }
    //          for (int j = players[i].reachSutehaiIndex + 1;
    //               j < players[k].sutehais.size(); j++) {
    //            players[i].extraAnpais.push_back(players[k].sutehais[j]);
    //          }
    //        }
    //      }
    //    }
    //  }
    
    std::vector<Pai> anpais(PlayerData p) {
        std::vector<Pai> ret = p.sutehais;
        for (int i = 0; i < p.extraAnpais.size(); i++) {
            ret.push_back(p.extraAnpais[i]);
        }
        return ret;
    }
    
    int visibleAkaPaisNum(PlayerData p) {
        int num = 0;
        
        for (int i = 0; i < doraMarkers.size(); i++) {
            if (doraMarkers[i].red)
                num++;
        }
        for (int i = 0; i < p.tehais.size(); i++) {
            if (p.tehais[i].red)
                num++;
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < players[i].sutehais.size(); j++) {
                if (players[i].sutehais[j].red)
                    num++;
            }
            
            for (int j = 0; j < players[i].furos.size(); j++) {
                if (players[i].furos[j].hasRed())
                    num++;
            }
        }
        return num;
    }
    
    std::vector<Pai> visiblePais(PlayerData p) {
        std::vector<Pai> ret;
        
        for (int i = 0; i < doraMarkers.size(); i++) {
            ret.push_back(doraMarkers[i]);
        }
        for (int i = 0; i < p.tehais.size(); i++) {
            ret.push_back(p.tehais[i]);
        }
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < players[i].sutehais.size(); j++) {
                ret.push_back(players[i].sutehais[j]);
            }
            
            for (int j = 0; j < players[i].furos.size(); j++) {
                std::vector<Pai> pais = players[i].furos[j].consumed;
                for (int k = 0; k < pais.size(); k++) {
                    ret.push_back(pais[k]);
                }
            }
        }
        return ret;
    }
    
    PlayerData oya() { return players[(chichaNum + kyokuNum - 1) % 4]; }
    
    PlayerData chicha() { return players[chichaNum]; }
    
    std::vector<Pai> doras() {
        std::vector<Pai> ret;
        if (doraMarkers.size() > 0) {
            for (int i = 0; i < doraMarkers.size(); i++) {
                ret.push_back(doraMarkers[i].nextForDora());
            }
            return ret;
        }
        // throw "doraMarkers is empty";
        return ret;
    }
    Pai jikaze(PlayerData p) {
        // cout<<"oya().identifier = "<<oya().identifier<<endl;
        return Pai("t", 1 + (4 + p.identifier - oya().identifier) % 4);
    }
    
    int yakuhaifan(Pai pai, PlayerData player) {
        int fan = 0;
        if (pai.type == "t" && pai.number >= 5 && pai.number <= 7)
            ++fan;
        if (pai.hasSameSymbol(bakaze))
            ++fan;
        if (pai.hasSameSymbol(jikaze(player)))
            ++fan;
        return fan;
    }
    int turn() { return (NUM_INITIAL_PIPAIS - numPipais) / 4; }
    
    bool isTenpai(PlayerData p) {
        ShantenAnalysis syan(p.tehais, 0, 0);
        return syan.shanten <= 0;
    }
    
    void deleteTehai(int idx, Pai pai) {
        bool deleted = false;
        int paiIndex = 0;
        for (int i = 0; players[idx].tehais.size(); i++) {
            if (players[idx].tehais[i].equal(pai) ||
                players[idx].tehais[i].equal(Pai("?"))) {
                deleted = true;
                paiIndex = i;
                break;
            }
        }
        
        if (!deleted) {
            for (int i = 0; players[idx].tehais.size(); i++) {
                if (players[idx].tehais[i].hasSameSymbol(pai)) {
                    deleted = true;
                    paiIndex = i;
                    break;
                }
            }
        }
        
        if (!deleted) {
            throw "trying to delete which is not in tehais";
        }
        players[idx].tehais.erase(players[idx].tehais.begin() + paiIndex);
    }
    
    int getDistance(PlayerData p1, PlayerData p2) {
        return (4 + p1.identifier - p2.identifier) % 4;
    }
    
    int rankedPlayers(PlayerData p1, PlayerData p2) {
        if (p1.score > p2.score) {
            return -1;
        } else if (p1.score < p2.score) {
            return 1;
        } else
            return getDistance(p1, chicha()) - getDistance(p2, chicha());
    }
    
    MJAIGame getNextKyoku(int kyokuNum, Pai bakaze, Pai *retBakaze) {
        if (kyokuNum == 4 && bakaze.equal(Pai("S"))) {
            return *this;
        }
        
        MJAIGame ret = *this;
        if (kyokuNum == 4) {
            ret.bakaze = bakaze.nextForDora();
            return ret;
        }
        ret.kyokuNum = kyokuNum + 1;
        return ret;
    }
    
    void initScore() {
        for (int i = 0; i < 4; i++) {
            players[i].score = DEFAULT_POINT;
        }
    }
    
    void init() {
        for (int i = 0; i < 4; i++) {
            players[i].identifier = i;
            players[i].reachState = 0;
            players[i].reachSutehaiIndex = 0;
            for (int j = 0; j < 13; j++) {
                players[i].tehais.clear();
                // players[i].score = DEFAULT_POINT;
                players[i].furos.clear();
                players[i].sutehais.clear();
                players[i].tedashi.clear();
                players[i].extraAnpais.clear();
            }
        }
        kyokuNum = 0;
        honba = 0;
        richiKyoutaku = 0;
        numPipais = NUM_INITIAL_PIPAIS;
    }
    
    static double probDistToAverageRank(ProbDist dist,
                                        AverageRankCalculator calculator,
                                        int basyo, int nokoriKyoku,
                                        std::vector<double> point) {
        
        double avg = 0;
        std::map<std::vector<double>, double> mp = dist.dist;
        
        for (auto itr = mp.begin(); itr != mp.end(); ++itr) {
            std::vector<double> deltas = itr->first;
            double prob = itr->second;
            avg += calculator.calc(basyo, point[0] + deltas[0], point[1] + deltas[1],
                                   point[2] + deltas[2], point[3] + deltas[3],
                                   nokoriKyoku) *
            prob;
        }
        
        // cout << "avg = " << avg << endl;
        
        return avg;
    }
    
    std::string updateStateAndRespondToAction(string lineString,
                                              bool reachDecleared) {
        picojson::value v;
        std::string err;
        picojson::parse(v, lineString.c_str(),
                        lineString.c_str() + strlen(lineString.c_str()), &err);
        picojson::object obj = v.get<picojson::object>();
        bool tsumogiri = false;
        
        std::string ret = "{\"type\":\"none\"}\n";
        int actor = -1;
        Pai p;
        string type = obj["type"].get<std::string>();
        
        cout << "type = " << type << " me = " << me << endl;
        
        currentAction.type = type;
        if (obj["actor"].is<double>())
            currentAction.actor = obj["actor"].get<double>();
        if (obj["pai"].is<std::string>())
            currentAction.pai = Pai(obj["pai"].get<std::string>());
        
        if (reserved_action != "" && type != key_reach_accepted &&
            currentAction.actor == me) {
            cout << "reserved_action exist" << endl;
            string ret_tmp = reserved_action + "\n";
            reserved_action = "";
            ret = ret_tmp;
        }
        
        if (type == key_start_hello) {
            
            ret = string_sprintf(
                                 "{\"type\":\"join\",\"name\":\"%s\",\"room\":\"default\"}\n",
                                 MY_PLAYER_NAME);
            
        } else if (type == key_start_game) {
            if (obj["id"].is<double>()) {
                me = obj["id"].get<double>();
            } else {
                picojson::array &nameList = obj["names"].get<picojson::array>();
                int idx = 0;
                for (picojson::array::iterator it = nameList.begin();
                     it != nameList.end(); it++) {
                    std::string s = it->get<std::string>();
                    if (s == MY_PLAYER_NAME) {
                        me = idx;
                        break;
                    }
                    idx++;
                }
            }
            
            if (obj["gametype"].is<string>()) {
                string t = obj["gametype"].get<string>();
                if (t == "tonpu") {
                    tonpuusen = true;
                    printf("tonpu/n");
                } else {
                    tonpuusen = false;
                    printf("tonnan/n");
                }
            } else {
                tonpuusen = false;
                printf("gametype not found/n");
            }
            
            if (me == -1)
                throw "my number not found";
            
            bakaze = Pai("E");
            
            cout << "ME = " << me << endl;
            
            tmpTotalKyokusuu = 0;
            tmpTotalHora = 0;
            tmpTotalHoujyuu = 0;
            tmpTotalFuro = 0;
            tmpTotalRichiWhenHora = 0;
            tmpTotalMenzenHora = 0;
            tmpTotalGame = 1;
            
            for (int i = 0; i < 4; i++) {
                players[i].score = DEFAULT_POINT;
            }
            
        } else if (type == key_start_kyoku) {
            
            init();
            
            if (obj["tonpuusen"].is<double>()) {
                int t = obj["tonpuusen"].get<double>();
                if (t == 1) {
                    tonpuusen = true;
                }
            }
            
            // ここでもmeは設定できるようにしてみた JSONにあればね
            if (obj["me"].is<double>()) {
                me = obj["me"].get<double>();
            }
            
            bakaze = Pai(obj["bakaze"].get<std::string>());
            kyokuNum = obj["kyoku"].get<double>();
            if (kyokuNum == 1) {
                chichaNum = obj["oya"].get<double>();
            }
            honba = obj["honba"].get<double>();
            richiKyoutaku = obj["kyotaku"].get<double>();
            
            doraMarkers.clear();
            doraMarkers.push_back(Pai(obj["dora_marker"].get<std::string>()));
            
            picojson::array &tehaiList = obj["tehais"].get<picojson::array>();
            int index = 0;
            for (picojson::array::iterator it = tehaiList.begin();
                 it != tehaiList.end(); it++) {
                picojson::array &tehaiList2 = it->get<picojson::array>();
                for (picojson::array::iterator it2 = tehaiList2.begin();
                     it2 != tehaiList2.end(); it2++) {
                    Pai p = Pai(it2->get<std::string>());
                    players[index].tehais.push_back(p);
                }
                index++;
            }
            for (int i = 0; i < 4; i++) {
                players[i].sutehais.clear();
                for (int j = 0; j < 4; j++) {
                    players[i].furoSutehai[j] = -1;
                }
                sortTehai(i);
            }
            
            if (kyokuNum == 0 && bakaze.equal(Pai("E")) && honba == 0) {
                initScore();
            }
            
            tmpTotalKyokusuu++;
            
        } else if (type == key_tsumo) {
            actor = obj["actor"].get<double>();
            p = Pai(obj["pai"].get<std::string>());
            
            numPipais--;
            
            cout << "key_tsumo" << endl;
            // cout << endl << toString() << endl;
            cout << endl
            << actor << "pai:" << p.toString() << " actor:" << actor << endl;
            
            if (actor == me) {
                players[actor].tehais.push_back(p);
                players[actor].lastTsumohai = p;
                sortTehai(actor);
                ret = respondToAction(me, actor, p);
                cout << endl << toString() << endl;
            }
            
        } else if (type == key_dahai) {
            actor = obj["actor"].get<double>();
            p = Pai(obj["pai"].get<std::string>());
            tsumogiri = obj["tsumogiri"].get<bool>();
            
            players[actor].sutehais.push_back(p);
            players[actor].tedashi.push_back(!tsumogiri);
            
            for (int pnum = 0; pnum < 4; pnum++) {
                //        players[pnum].extraAnpais.push_back(p);
                //        if (pnum == actor && players[actor].reachState == 0 &&
                //        !tsumogiri) {
                //          // 鳴きplayer考慮
                //          players[actor].extraAnpais.clear();
                //        }
                
                if (players[pnum].reachState > 0) {
                    players[pnum].extraAnpais.push_back(p);
                } else {
                    // 鳴きplayer考慮
                    if (pnum == actor && !tsumogiri) {
                        // 手出しでクリア
                        players[pnum].extraAnpais.clear();
                    } else {
                        players[pnum].extraAnpais.push_back(p);
                    }
                }
            }
            
            cout << "key_dahai" << endl;
            if (actor == me)
                cout << endl << toString() << endl;
            cout << endl
            << actor << "pai:" << p.toString() << " actor:" << actor << endl;
            
            if (actor == me)
                deleteTehai(actor, p);
            
            if (previousAction.type == key_pon || previousAction.type == key_chi ||
                checkFuroSuteForKan) {
                for (int i = 0; i < 4; i++) {
                    if (players[actor].furoSutehai[i] == -1) {
                        players[actor].furoSutehai[i] = players[actor].sutehais.size() - 1;
                        break;
                    }
                }
                checkFuroSuteForKan = false;
            }
            
            cout << endl << toString() << endl;
            
            if (!(actor == me)) {
                if (obj["possible_actions"].is<picojson::array>() &&
                    obj["possible_actions"].get<picojson::array>().size() > 0)
                    cout << "possible action exist" << endl;
                // sortTehai(actor);
                ret = respondToAction(me, actor, p);
            }
            
        } else if (type == key_pon || type == key_chi || type == key_ankan ||
                   type == key_daiminkan) {
            actor = obj["actor"].get<double>();
            int target = 0;
            if (type != key_ankan)
                target = obj["target"].get<double>();
            
            picojson::array &tehaiList = obj["consumed"].get<picojson::array>();
            vector<Pai> consume;
            for (picojson::array::iterator it = tehaiList.begin();
                 it != tehaiList.end(); it++) {
                Pai _p = Pai(it->get<std::string>());
                consume.push_back(_p);
                if (actor == me)
                    deleteTehai(actor, _p);
                
                p = _p;
            }
            
            if (obj["pai"].is<std::string>()) {
                p = Pai(obj["pai"].get<std::string>());
            }
            players[actor].furos.push_back(Furo(type, p, target, consume));
            
            if (type != key_ankan) {
                players[actor].extraAnpais.clear();
            }
            
        } else if (type == key_kakan) {
            actor = obj["actor"].get<double>();
            p = Pai(obj["pai"].get<std::string>());
            for (int i = 0; i < players[actor].furos.size(); i++) {
                if (players[actor].furos[i].type == "pon" &&
                    players[actor].furos[i].taken.equal(p)) {
                    players[actor].furos[i] = Furo("kakan", p, 0, {p, p, p});
                    if (actor == me)
                        deleteTehai(actor, p);
                    break;
                }
            }
        } else if (type == key_reach) {
        } else if (type == key_end_kyoku) {
            
            // For フーロ数
            if (players[me].furos.size() > 0) {
                tmpTotalFuro++;
            }
            
        } else if (type == key_reach_accepted) {
            
            actor = obj["actor"].get<double>();
            players[actor].reachState = true;
            players[actor].reachSutehaiIndex = players[actor].sutehais.size();
            
            players[actor].extraAnpais.clear();
            
            richiKyoutaku++;
            
            if (obj["deltas"].is<picojson::array>()) {
                picojson::array &deltaList = obj["deltas"].get<picojson::array>();
                int index = 0;
                for (picojson::array::iterator it = deltaList.begin();
                     it != deltaList.end(); it++) {
                    players[index].score += it->get<double>();
                    index++;
                }
            }
            
        } else if (type == key_dora) {
            p = Pai(obj["dora_marker"].get<std::string>());
            doraMarkers.push_back(p);
        } else if (type == key_hora) {
            picojson::array &pointArray = obj["scores"].get<picojson::array>();
            int idx = 0;
            for (picojson::array::iterator it = pointArray.begin();
                 it != pointArray.end(); it++) {
                players[idx++].score = it->get<double>();
            }
            
            actor = obj["actor"].get<double>();
            int target = obj["target"].get<double>();
            if (actor == me) {
                tmpTotalHora++;
                if (players[me].reachState && players[me].reachState > 0) {
                    tmpTotalRichiWhenHora++;
                }
                if (players[me].furos.size() == 0) {
                    tmpTotalMenzenHora++;
                }
            } else if (actor != me && target == me) {
                tmpTotalHoujyuu++;
            }
            
        } else if (type == key_ryukyoku) {
            picojson::array &pointArray = obj["scores"].get<picojson::array>();
            int idx = 0;
            for (picojson::array::iterator it = pointArray.begin();
                 it != pointArray.end(); it++) {
                players[idx++].score = it->get<double>();
            }
        } else if (type == key_end_game) {
            int r = getRank(me);
            
            rankArray[r]++;
            sumOfRank += r;
            
            totalKyokusuu += tmpTotalKyokusuu;
            totalHora += tmpTotalHora;
            totalHoujyuu += tmpTotalHoujyuu;
            totalFuro += tmpTotalFuro;
            totalRichiWhenHora += tmpTotalRichiWhenHora;
            totalMenzenHora += tmpTotalMenzenHora;
            totalGame += tmpTotalGame;
            
            tmpTotalKyokusuu = 0;
            tmpTotalHora = 0;
            tmpTotalHoujyuu = 0;
            tmpTotalFuro = 0;
            tmpTotalRichiWhenHora = 0;
            tmpTotalMenzenHora = 0;
            
            printf("順位 = %f\n", (double)r);
            printf("sumOfRank = %f\n", (double)sumOfRank);
            printf("totalGame = %f\n", (double)totalGame);
            printf("平均順位 = %f\n", (double)sumOfRank / totalGame);
            printf("あがり率 = %.2f%%\n", (totalHora / totalKyokusuu) * 100);
            printf("放銃率 = %.2f%%\n", (totalHoujyuu / totalKyokusuu) * 100);
            printf("フーロ率 = %.2f%%\n", (totalFuro / totalKyokusuu) * 100);
            printf("あがり時リーチ率 = %.2f%%\n",
                   (totalRichiWhenHora / totalMenzenHora) * 100);
            printf("1位率 = %.2f%%\n", ((double)rankArray[1] / totalGame) * 100);
            printf("2位率 = %.2f%%\n", ((double)rankArray[2] / totalGame) * 100);
            printf("3位率 = %.2f%%\n", ((double)rankArray[3] / totalGame) * 100);
            printf("4位率 = %.2f%%\n", ((double)rankArray[4] / totalGame) * 100);
#ifndef RELEASE_BUILD
            FILE *fp = fopen("rank.txt", "a");
            fprintf(fp, "%d\n", r);
            fclose(fp);
            
            FILE *fp_write = fopen("stats.txt", "w");
            fprintf(fp_write, "順位 = %f\n", (double)r);
            fprintf(fp_write, "sumOfRank = %f\n", (double)sumOfRank);
            fprintf(fp_write, "totalGame = %f\n", (double)totalGame);
            fprintf(fp_write, "平均順位 = %f\n", (double)sumOfRank / totalGame);
            fprintf(fp_write, "あがり率 = %.2f%%\n",
                    (totalHora / totalKyokusuu) * 100);
            fprintf(fp_write, "放銃率 = %.2f%%\n",
                    (totalHoujyuu / totalKyokusuu) * 100);
            fprintf(fp_write, "フーロ率 = %.2f%%\n",
                    (totalFuro / totalKyokusuu) * 100);
            fprintf(fp_write, "あがり時リーチ率 = %.2f%%\n",
                    (totalRichiWhenHora / totalMenzenHora) * 100);
            fprintf(fp_write, "1位率 = %.2f%%\n",
                    ((double)rankArray[1] / totalGame) * 100);
            fprintf(fp_write, "2位率 = %.2f%%\n",
                    ((double)rankArray[2] / totalGame) * 100);
            fprintf(fp_write, "3位率 = %.2f%%\n",
                    ((double)rankArray[3] / totalGame) * 100);
            fprintf(fp_write, "4位率 = %.2f%%\n",
                    ((double)rankArray[4] / totalGame) * 100);
            fclose(fp_write);
#endif
        }
        
        previousAction.type = type;
        previousAction.actor = actor;
        previousAction.pai = p;
        previousAction.tsumogiri = tsumogiri;
        
        return ret;
    }
    
    std::string respondToAction(int num, int target, Pai p) {
        
        std::string ret = "";
        
        // 九種九牌判定
        if (num == target && canKyusyuKyuhai()) {
            cout << "kyusyukyuhai" << endl;
            Metrics m;
            m.type = RESPONSE_TYPE_MJPIR_NAGASHI;
            return getJsonWithMetrics(m, "", false);
        }
        
        // ・終局パターン
        //    *自分ツモアガリ
        //    自分ロンアガリ　上家から
        //    自分ロンアガリ　対面から
        //    自分ロンアガリ　下家から
        //    下家ツモアガリ
        //    *下家ロンアガリ　自分から
        //    下家ロンアガリ　対面から
        //    下家ロンアガリ　上家から
        //    対面ツモアガリ
        //    *対面ロンアガリ　自分から
        //    対面ロンアガリ　上家から
        //    対面ロンアガリ　下家から
        //    上家ツモアガリ
        //    *上家ロンアガリ　自分から
        //    上家ロンアガリ　対面から
        //    上家ロンアガリ　下家から
        //    流局
        //
        // 以上の点棒移動と確率をうまいこといい感じで求め、平均順位の平均が評価値
        // 横移動ってどうやって求めるんすかね・・ただこれを求めるのがかなり重要っぽい
        
        //------------------------------------------------------------
        
        // テンパイ率推定（爆打のやつ）
        // 放銃率と放銃点
        getOpponentHoraEstimator(me, &_OpponentHoraEstimator);
        calcTenpaiProbEstimator();
        
        //        _HoraEstimator.existDengerausPlayer = false;
        //        for(int k = 0; k < 3; k++) {
        //            double tenpaiProb =  _TenpaiProbEstimator.tenpai_pro[k];
        //            if(tenpaiProb >= 0.7) {
        //                _HoraEstimator.existDengerausPlayer = true;
        //                break;
        //            }
        //        }
        //------------------------------------------------------------
        
        // あがり率
        std::vector<Metrics> dist =
        target == me ? getHoraEstimator() : getHoraEstimatorWhenFuro(p, target);
        
        std::string html = toJson();
        
        if (dist.size() == 1) {
            cout << "not exist possible furo" << endl;
            return getJsonWithMetrics(dist[0], html, false);
        }
        
        std::vector<Metrics> sortedDist = getSortedMetricsVectorWithCalculate(dist);
        
        // cout << "getSortedMetricsVectorWithCalculate" << endl;
        
        // html = "";
        for (int j = 0; j < sortedDist.size(); j++) {
            html += sortedDist[j].toHtmlString();
        }
        
        html += toJson();
        
        return getJsonWithMetrics(sortedDist[0], html, true);
    }
    
    /*
     *******************************************************************************************
     *
     *
     *                   評価関数 eval
     *
     *
     *******************************************************************************************
     */
    std::vector<Metrics>
    getSortedMetricsVectorWithCalculate(std::vector<Metrics> vec) {
        std::vector<Metrics> ret;
        
        // int nsyan =
        // _HoraEstimator.getNormalShanten(players[me].tehais,players[me].furos);
        // int tsyan =
        // _HoraEstimator.getChitoituShanten(players[me].tehais,players[me].furos);
#ifndef RELEASE_BUILD
        cout << "0 tenpaiProb = " << _TenpaiProbEstimator.tenpai_pro[0] << endl;
        cout << "1 tenpaiProb = " << _TenpaiProbEstimator.tenpai_pro[1] << endl;
        cout << "2 tenpaiProb = " << _TenpaiProbEstimator.tenpai_pro[2] << endl;
#endif
        
        int reachDecleardNum = 0;
        int overThreshouldPlayer = 0;
        
        for (int i = 0; i < vec.size(); i++) {
            
            ProbDist dist;
            
            double totalProb = 1;
            
            vec[i].hojuProb = 0;
            
            for (int k = 0; k < 3; k++) {
                
                double tenpaiProb = _TenpaiProbEstimator.tenpai_pro[k]; // テンパイ率
                double hojuProb =
                _OpponentHoraEstimator.hojuProb[k][(
                                                    vec[i].pai.identifier)]; // 放銃率
                double hojuTen =
                _OpponentHoraEstimator.hojuTen[k][(
                                                   vec[i].pai.identifier)];
                
#ifdef SOCKET
                cout << "HOJU stats " << vec[i].pai.toString() << " = " << hojuProb << " - " << hojuTen
                          << endl;
#endif
                // テンパイ率0.7以上なら
                if ((tenpaiProb >= 0.75 && hojuTen >= 2000) ||
                    (tenpaiProb >= 0.4 && hojuTen >= 6000)) {

                    // hojuTen += richiKyoutaku * 1000;
                    
                    if (tenpaiProb >= 0.95) {
                        // hojuTen *= 1.5;
                        reachDecleardNum++;
                    }
                    
                    // TODO TEST
                    if (vec[i].shanten == 0 || vec[i].chitoitu_shanten == 0) {
                        hojuProb /= 2.55;
                    }
                    
                    if (totalProb - tenpaiProb * hojuProb > 0) {
                        vector<double> hoju = {-hojuTen, k == 0 ? hojuTen : 0,
                            k == 1 ? hojuTen : 0, k == 2 ? hojuTen : 0};
                        dist.upsert(irekaeVector(me, hoju), tenpaiProb * hojuProb);
                        totalProb -= tenpaiProb * hojuProb;
                    }
                    
                    vec[i].hojuProb += tenpaiProb * hojuProb;
                    vec[i].averageHojuPoints = hojuTen > 0 ? hojuTen : 0;
                    
                    overThreshouldPlayer++;
                }
            }
            
            double totalProbExceptHoju = totalProb;
            
            double horaProbHosei = 0.05;
            
            double horaProb =
            (vec[i].horaProb) * (totalProbExceptHoju * horaProbHosei);
            
            /*
             // TODO
             // ベタオリ?
             bool betaoriFlag = (nsyan >= 3 && tsyan >= 2 && reachDecleardNum >= 1);
             if(betaoriFlag) {
             horaProb = 0;
             }
             */
            
            if ((vec[i].shanten == 0 || vec[i].chitoitu_shanten == 0) &&
                (vec[i].type == RESPONSE_TYPE_MJPIR_REACH ||
                 vec[i].type == RESPONSE_TYPE_MJPIR_SUTEHAI)) {
                    
                    // であがり不可ならあがり率減らす
                    if (vec[i].type == RESPONSE_TYPE_MJPIR_REACH ||
                        vec[i].type == RESPONSE_TYPE_MJPIR_SUTEHAI) {
                        
                        deleteTehai(me, vec[i].pai);
                    }
                    
                    vector<Pai> machi =
                    PlayerData::getMachi(players[me].tehais, players[me].furos);
                    
                    cout << "mati hosei = " << endl;
                    for (Pai p : machi) {
                        cout << p.toString() << endl;
                    }
                    
                    bool chk = false;
                    bool chk_avail = false;
                    bool chk_notavail = false;
                    for (Pai p : machi) {
                        Deagari d = players[me].canDeagari(
                                                           p, bakaze, jikaze(me), vec[i].type == RESPONSE_TYPE_MJPIR_REACH);
                        
                        if (d.type == DeagariType::FURITEN) {
                            
                            cout << "フリテン" << endl;
                            
                            chk = true;
                            horaProb /= 5;
                            break;
                        }
                        if (d.type == DeagariType::AVAIL)
                            chk_avail = true;
                        if (d.type == DeagariType::NOT_AVALI)
                            chk_notavail = true;
                    }
                    
                    if (chk_notavail && chk_avail) {
                        cout << "かた上がり" << endl;
                        horaProb /= 2;
                    } else if (chk_notavail && !chk_avail) {
                        cout << "であがり不可あり2" << endl;
                        horaProb /= 5;
                    }
                    
                    // 待ち牌数による補正
                    if (!chk) {
                        cout << "であがり不可なし" << endl;
                        
                        int totalMachimaisuu = 0;
                        
                        PaiSet visiblePaiSet = PaiSet(visiblePais(players[me]));
                        
                        bool jihaiFUkumu = false;
                        for (Pai p : machi) {
                            totalMachimaisuu += 4 - visiblePaiSet.array[p.identifier];
                            
                            cout << "totalMachimaisuu" << totalMachimaisuu << endl;
                            
                            if (p.type == "t" && 4 - visiblePaiSet.array[p.identifier] > 0)
                                jihaiFUkumu = true;
                        }
                        
                        if (jihaiFUkumu && totalMachimaisuu > 0) {
                            cout << "待ちに字牌がある" << endl;
                            horaProb *= 1.3;
                            
//                            if (vec[i].type == RESPONSE_TYPE_MJPIR_REACH) {
//                                cout << "立直だしさらに少しふやす" << endl;
//                                horaProb *= 1.15;
//                            }
                        } else if (totalMachimaisuu >= 5) {
                            
                            cout << "あがり5枚以上あり" << endl;
                            
                            horaProb *= 1.5;
                            
//                            if (vec[i].type == RESPONSE_TYPE_MJPIR_REACH) {
//                                cout << "立直だしさらに少しふやす" << endl;
//                                horaProb *= 1.05;
//                            }
                        } else if (players[me].furos.size() == 0 && totalMachimaisuu < 4 &&
                                   machi.size() == 1) {
                            // 待ち枚数３以下単騎待ちとか
                            cout << "待ち枚数３以下単騎待ちとか totalMachimaisuu ="
                            << totalMachimaisuu << endl;
                            if (totalMachimaisuu > 0 && machi[0].type == "t") {
                                cout << "じはい" << endl;
                            } else {
                                cout << "数牌" << endl;
                                horaProb /= 1.25; //(double)7.0 -
                                
                                if (machi.size() == 1 && vec[i].pai.type != "t" &&
                                    (vec[i].pai.number == 4 || vec[i].pai.number == 5 ||
                                     vec[i].pai.number == 6)) {
                                        cout << "4 5 6単騎とかしね1" << endl;
                                        horaProb /= 2;
                                        
                                        if (vec[i].type == RESPONSE_TYPE_MJPIR_REACH) {
                                            cout << "立直だしさらに少し減らす" << endl;
                                            horaProb /= 1.1;
                                        }
                                    }
                                
                                //(double)totalMachimaisuu;
                                
                                if (vec[i].type == RESPONSE_TYPE_MJPIR_REACH) {
                                    cout << "立直だしさらに少し減らす" << endl;
                                    horaProb /= 1.05;
                                }
                            }
                        } else if (players[me].furos.size() == 0 && machi.size() == 1) {
                            cout << "1種類待ちだし少し減らす" << endl;
                            horaProb /= 1.05;
                            
                            if (machi.size() == 1 && vec[i].pai.type != "t" &&
                                (vec[i].pai.number == 4 || vec[i].pai.number == 5 ||
                                 vec[i].pai.number == 6)) {
                                    cout << "4 5 6単騎とかしね2" << endl;
                                    horaProb /= 2;
                                    
                                    if (vec[i].type == RESPONSE_TYPE_MJPIR_REACH) {
                                        cout << "立直だしさらに少し減らす" << endl;
                                        horaProb /= 1.1;
                                    }
                                }
                            
                            if (vec[i].type == RESPONSE_TYPE_MJPIR_REACH) {
                                cout << "立直だしさらに少し減らす" << endl;
                                horaProb /= 1.05;
                            }
                            
                        } else {
                            cout << "test" << endl;
                        }
                    }
                    
                    if (vec[i].type == RESPONSE_TYPE_MJPIR_REACH ||
                        vec[i].type == RESPONSE_TYPE_MJPIR_SUTEHAI) {
                        players[me].tehais.push_back(vec[i].pai);
                        
                        sortTehai(me);
                    }
                }
            
            if (vec[i].pai.red || _HoraEstimator.isDora(doraMarkers, vec[i].pai)) {
                horaProb /= 1.2;
            }
            
            if (horaProb > 1)
                horaProb = 1;
            
            //    *自分ツモアガリ
            double horaPoint = vec[i].averageHoraPoints;
            
            int horapointWaru3 = horaPoint / 3;
            horapointWaru3 =
            ((int)horapointWaru3 % 100 > 0)
            ? (int)horapointWaru3 + 100 - (int)horapointWaru3 % 100
            : (int)horapointWaru3;
            
            double horaPointWithKyoutaku = horaPoint;
            if (horaPoint > 0) {
                horaPointWithKyoutaku = horaPoint + 1000 * richiKyoutaku;
            }
            
            dist.upsert(irekaeVector(me, {horaPointWithKyoutaku,
                static_cast<double>(-horapointWaru3),
                static_cast<double>(-horapointWaru3),
                static_cast<double>(-horapointWaru3)}),
                        horaProb);
            
            //      // 自分ロンアガリ
            //      dist.upsert(
            //          irekaeVector(me,
            //                       {horaPointWithKyoutaku,
            //                       static_cast<double>(-horaPoint),
            //                        static_cast<double>(-0),
            //                        static_cast<double>(-0)}),
            //          horaProb / 4);
            //
            //      dist.upsert(
            //          irekaeVector(me, {horaPointWithKyoutaku,
            //          static_cast<double>(-0),
            //                            static_cast<double>(-horaPoint),
            //                            static_cast<double>(-0)}),
            //          horaProb / 4);
            //
            //      dist.upsert(
            //          irekaeVector(me, {horaPointWithKyoutaku,
            //          static_cast<double>(-0),
            //                            static_cast<double>(-0),
            //                            static_cast<double>(-horaPoint)}),
            //          horaProb / 4);
            
            totalProb -= horaProb;
            
            // テンパイ　TODO
            double tenpaiProbTmp =
            vec[i].tenpaiProb * (totalProbExceptHoju * horaProbHosei);
            if (numPipais < 15 && totalProb - tenpaiProbTmp > 0) {
                
                vector<double> tenapiRyou = {2000, 0, 0, 0};
                /*
                 if (_TenpaiProbEstimator.tenpai_pro[0] > 0.75) {
                 }
                 if (_TenpaiProbEstimator.tenpai_pro[1] > 0.75) {
                 }
                 if (_TenpaiProbEstimator.tenpai_pro[2] > 0.75) {
                 }
                 vector<vector<double>> tenapiRyouTable = {
                 {3000, -1000, -1000, -1000},
                 {1500, 1500, -1500, -1500},
                 {1500, -1500, 1500, -1500},
                 {1500, -1500, -1500, 1500},
                 {1000, -1000, 1000, 1000},
                 {1000, -1000, 1000, 1000},
                 {1000, 1000, -1000, 1000},
                 {1000, 1000, 1000, -1000},
                 {0, 0, 0, 0},
                 };
                 */
                dist.upsert(irekaeVector(me, tenapiRyou), tenpaiProbTmp);
                totalProb -= (tenpaiProbTmp);
            }
            
            if (totalProb > 0) {
                // 横移動 TODO てきとう
                
                //        double riibou = 0;
                //        if (vec[i].type == RESPONSE_TYPE_MJPIR_REACH)
                //          riibou = 1000;
                
                dist.upsert(irekaeVector(me, {0, 0, 0, 0}), totalProb);
            }
            
            int totalKyokusuuForAvg = 7;
            
            if (tonpuusen) {
                totalKyokusuuForAvg = 3;
            }
            
            int keikaKyoku = kyokuNum;
            if (bakaze.equal(Pai("S")))
                keikaKyoku += 4;
            if (bakaze.equal(Pai("W")))
                keikaKyoku += 4;
            if (bakaze.equal(Pai("N")))
                keikaKyoku += 4;
            
            if (totalProb < 0) {
                cout << totalProb << endl;
                cout << horaProb << endl;
                // throw "totalprob error";
                cout << "totalprob error : " << totalProb << " horaProb : " << horaProb
                << " tenpaiProbTmp: " << tenpaiProbTmp
                << " sum of hojuProb(todo): " << totalProbExceptHoju << endl;
            }
            int nokoriKyokuSuu = totalKyokusuuForAvg - keikaKyoku;
            //if (nokoriKyokuSuu < 0) nokoriKyokuSuu = 0;
            
            vec[i].averageRank = MJAIGame::probDistToAverageRank(
                                                                 dist, calculator, me, nokoriKyokuSuu,
                                                                 {static_cast<double>(players[0].score),
                                                                     static_cast<double>(players[1].score),
                                                                     static_cast<double>(players[2].score),
                                                                     static_cast<double>(players[3].score)});
            //      相手もあがらず流局もせず自分がツモる確率 == 1 - 上記の確率
            
            //      // ProbDist Log
            //      //#ifdef SOCKET
            //      cout << vec[i].pai.toString() << endl;
            //      cout << dist.toString() << endl;
            //      //#endif
            
            // TODO
            if (vec[i].averageHoraPoints == 0 && vec[i].horaProb == 0 &&
                (MJHoraEstimator::isFuroType(vec[i].type) ||
                 vec[i].type == RESPONSE_TYPE_MJPIR_REACH)) {
                    vec[i].averageRank += 0.1;
                }
            
            // 孤立牌
            // このヒューリスティックなくすならモンテツモの回数あげるしかない
            if (vec[i].shanten > 2 && vec[i].chitoitu_shanten > 2 &&
                _HoraEstimator.isKoritsuHai(players[me].tehais, vec[i].pai) &&
                vec[i].hojuProb <= 0.00001 &&
                (vec[i].type == RESPONSE_TYPE_MJPIR_REACH ||
                 vec[i].type == RESPONSE_TYPE_MJPIR_SUTEHAI)) {
                    // cout << "koritu" << vec[i].pai.toString() << endl;
                    //if (!_HoraEstimator.isDora(doraMarkers, vec[i].pai)) {
                    vec[i].averageRank = vec[i].averageHoraPoints = vec[i].horaProb =
                    vec[i].tenpaiProb = 0;
                    //}
                }
            
            ret.push_back(vec[i]);
        }
        
        //        for(int j=0;j<ret.size();j++) {
        //            cout << ret[j].toString() << endl;
        //        }
        
        //(int paiIdx,int tehai[],int bakaze,int jikaze)
        int tempt[38] = {0};
        for (int i = 0; i < players[me].tehais.size(); i++) {
            int b =
            Util::convertMJAIPainumToMjscore(players[me].tehais[i].identifier);
            tempt[b]++;
        }
        for (int j = 0; j < ret.size(); j++) {
            int paiidx = Util::convertMJAIPainumToMjscore(ret[j].pai.identifier);
            int ba = Util::convertMJAIPainumToMjscore(bakaze.identifier);
            int ji = Util::convertMJAIPainumToMjscore(jikaze(players[me]).identifier);
            if (ret[j].pai.equal(Pai("?"))) {
                ret[j].koritsuPaiPoint = -1;
            } else {
                ret[j].koritsuPaiPoint =
                EvalKoritsuhai(paiidx, tempt, ba, ji, doraMarkers);
                
                // そもそも孤立牌じゃない
                if (!_HoraEstimator.isKoritsuHai(players[me].tehais, ret[j].pai)) {
                    ret[j].koritsuPaiPoint += 100;
                }
            }
            
            //      // 3時間数近似テスト
            //      // ウマ、オカ
            //      // y = -3.3333x^3 + 30x^2 - 106.67x + 170
            //      ret[j].averageRank = -((-3.3333 * ret[j].averageRank *
            //                              ret[j].averageRank * ret[j].averageRank) +
            //                             (30 * ret[j].averageRank *
            //                             ret[j].averageRank) -
            //                             (106.67 * ret[j].averageRank) + 170);
        }
        
        // マジで糞な自作ソート
        
        if (reachDecleardNum > 0 && (MJHoraEstimator::isFuroType(ret[0].type)) &&
            ret[0].shanten > 1) {
            
            cout << "立直がいるので2シャンテン以上ならなかない1" << endl;
            ret[0].averageRank += 999;
        }
        
        vector<Metrics> sortedRet = vector<Metrics>{ret[0]};
        for (int j = 1; j < ret.size(); j++) {
            bool ins = false;
            
            if (reachDecleardNum > 0 && (MJHoraEstimator::isFuroType(ret[j].type))) {
                if (ret[j].shanten > 1) {
                    cout << "立直がいるので2シャンテン以上ならなかない2" << endl;
                    continue;
                } else {
                    cout << "立直がいるけど1シャンテン以下" << endl;
                }
            }
            
            for (int k = 0; k < sortedRet.size(); k++) {
                if (Metrics::compareMyDataPredicate(ret[j], sortedRet[k])) {
                    
                    sortedRet.insert(sortedRet.begin() + k, ret[j]);
                    
                    //          cout << "sortedRet.size()" << sortedRet.size() << endl;
                    //          for (int j = 0; j < sortedRet.size(); j++) {
                    //            cout << sortedRet[j].toString() << endl;
                    //          }
                    
                    ins = true;
                    
                    break;
                }
            }
            
            if (!ins)
                sortedRet.push_back(ret[j]);
        }
        
        // std::sort(ret.begin(), ret.end(), Metrics::compareMyDataPredicate);
        
        // cout << "--------------------------------" << endl;
#ifndef RELEASE_BUILD
        for (int j = 0; j < sortedRet.size(); j++) {
            cout << sortedRet[j].toString() << endl;
        }
#endif
        return sortedRet;
    }
    
    // from : vector<double> 0:me 1 2 3
    // to     : vector<double>
    vector<double> irekaeVector(int me, vector<double> from) {
        
        //        cout << "IREKAE : ";
        //        for(int j=0;j<from.size();j++) {
        //            cout << from[j] << " " ;
        //        }
        //        cout << endl;
        
        vector<double> ret = {0, 0, 0, 0};
        
        int _me = me;
        ret[_me] = from[0];
        _me++;
        if (_me > 3)
            _me = 0;
        ret[_me] = from[1];
        _me++;
        if (_me > 3)
            _me = 0;
        ret[_me] = from[2];
        _me++;
        if (_me > 3)
            _me = 0;
        ret[_me] = from[3];
        
        //        cout << "IREKAE AFTER : ";
        //        for(int j=0;j<ret.size();j++) {
        //            cout << ret[j] << " ";
        //        }
        //        cout << endl;
        
        return ret;
    }
    
    std::string getSutehaiJsonWithMetrics(Metrics m, std::string html) {
        std::string ret = "{\"type\":\"dahai\",\"actor\":";
        ret += IntToString(me) + ",\"pai\":\"";
        ret += m.pai.toString() + "\",\"tsumogiri\":";
        ret += players[me].reachState || m.tsumogiri ? "true" : "false";
        ret += ",\"log\":\"";
        ret += html;
        ret += "\"}";
        return ret;
    }
    
    std::string getChiConsumePai(Pai p,int next,bool red) {
        
        Pai tmp = p.next(next);
        
        if(tmp.number != 5 || tmp.type == "t" || red) {
            return tmp.toString();
        }
        
        bool existAka = false;
        for(Pai pt : players[me].tehais) {
            if(pt.hasSameSymbol(tmp) && pt.red) {
                existAka = true;
                break;
            }
        }
        if(existAka) {
            tmp.red = true;
        }
        return tmp.toString();
    }
    
    std::string getPonConsumePai(Pai p,bool red) {
        
        if(p.number != 5 || p.type == "t") {
            return p.toString();
        }
        if(red) {
            return Pai(p.type,p.number,false).toString();
        }
        
        bool existAka = false;
        for(Pai pt : players[me].tehais) {
            if(pt.hasSameSymbol(p) && pt.red) {
                existAka = true;
                break;
            }
        }
        if(existAka) {
            p.red = true;
        }
        return p.toString();
    }

    std::string getJsonWithMetrics(Metrics m, std::string html,
                                   bool cancel = false) {
        std::string ret = "{\"type\":\"none\"";
        ret += ",\"cancel\":";
        ret += cancel ? "true" : "false";
        ret += ",\"log\":\"";
        ret += html;
        ret += "\"}";
        
        char ch[65535];
        char ch_tmp[65535];
        
        char paiChar[10];
        sprintf(paiChar, "%s", m.pai.toString().c_str());
        
        char currentPaiChar[10];
        sprintf(currentPaiChar, "%s", currentAction.pai.toString().c_str());
        
        switch (m.type) {
            case RESPONSE_TYPE_MJPIR_REACH:
                if (!players[me].reachState && numPipais >= 4) {
                    sprintf(ch, "{\"type\":\"reach\",\"actor\":%d}", me);
                    
                    reserved_action = "{\"type\":\"dahai\",\"actor\":";
                    reserved_action += IntToString(me) + ",\"pai\":\"";
                    reserved_action += m.pai.toString() + "\",\"tsumogiri\":";
                    reserved_action +=
                    players[me].reachState || m.tsumogiri ? "true" : "false";
                    reserved_action += ",\"log\":\"";
                    reserved_action += html;
                    
                    reserved_action += "\"}";
                    
                    ret = ch;
                    
                } else {
                    ret = getSutehaiJsonWithMetrics(m, html);
                }
                break;
            case RESPONSE_TYPE_MJPIR_SUTEHAI:
                
                ret = getSutehaiJsonWithMetrics(m, html);
                
                break;
            case RESPONSE_TYPE_MJPIR_NAGASHI:
                
                sprintf(
                        ch,
                        "{\"type\":\"ryukyoku\",\"actor\":%d,\"reason\":\"kyushukyuhai\"}",
                        me);
                ret = ch;
                
                break;
            case RESPONSE_TYPE_MJPIR_PON:
                
                sprintf(ch, "{\"type\":\"pon\",\"actor\":%d,\"target\":%d,\"pai\":\"%s\","
                        "\"consumed\":[\"%s\",\"%s\"]}",
                        me, currentAction.actor, currentPaiChar, getPonConsumePai(currentAction.pai,currentAction.pai.red).c_str(),
                        currentPaiChar);
                ret = ch;
                
                sprintf(ch_tmp, "{\"type\":\"dahai\",\"actor\":%d,\"pai\":\"%s\","
                        "\"tsumogiri\":false,\"log\":\"%s\"}",
                        me, m.pai.toString().c_str(), html.c_str());
                reserved_action = ch_tmp;
                
                break;
            case RESPONSE_TYPE_MJPIR_CHII_LEFT:
                
                sprintf(ch, "{\"type\":\"chi\",\"actor\":%d,\"target\":%d,\"pai\":\"%s\","
                        "\"consumed\":[\"%s\",\"%s\"]}",
                        me, currentAction.actor, currentPaiChar,
                        currentAction.pai.next(1).toString().c_str(),
                        currentAction.pai.next(2).toString().c_str());
                ret = ch;
                
                sprintf(ch_tmp, "{\"type\":\"dahai\",\"actor\":%d,\"pai\":\"%s\","
                        "\"tsumogiri\":false,\"log\":\"%s\"}",
                        me, m.pai.toString().c_str(), html.c_str());
                reserved_action = ch_tmp;
                
                break;
            case RESPONSE_TYPE_MJPIR_CHII_RIGHT:
                
                sprintf(ch, "{\"type\":\"chi\",\"actor\":%d,\"target\":%d,\"pai\":\"%s\","
                        "\"consumed\":[\"%s\",\"%s\"]}",
                        me, currentAction.actor, currentPaiChar,
                        getChiConsumePai(currentAction.pai,-1,currentAction.pai.red).c_str(),
                        getChiConsumePai(currentAction.pai,-2,currentAction.pai.red).c_str());

                
                ret = ch;
                
                sprintf(ch_tmp, "{\"type\":\"dahai\",\"actor\":%d,\"pai\":\"%s\","
                        "\"tsumogiri\":false,\"log\":\"%s\"}",
                        me, m.pai.toString().c_str(), html.c_str());
                reserved_action = ch_tmp;
                
                break;
            case RESPONSE_TYPE_MJPIR_CHII_MIDDLE:
                
                sprintf(ch, "{\"type\":\"chi\",\"actor\":%d,\"target\":%d,\"pai\":\"%s\","
                        "\"consumed\":[\"%s\",\"%s\"]}",
                        me, currentAction.actor, currentPaiChar,
                        getChiConsumePai(currentAction.pai,-1,currentAction.pai.red).c_str(),
                        getChiConsumePai(currentAction.pai,1,currentAction.pai.red).c_str());

                ret = ch;
                
                sprintf(ch_tmp, "{\"type\":\"dahai\",\"actor\":%d,\"pai\":\"%s\","
                        "\"tsumogiri\":false,\"log\":\"%s\"}",
                        me, m.pai.toString().c_str(), html.c_str());
                reserved_action = ch_tmp;
                
                break;
            case RESPONSE_TYPE_MJPIR_MINKAN:
                char minkanPaiChar[10];
                sprintf(minkanPaiChar, "%s", m.taken.toString().c_str());
                char minkanPaiCharRed[10];
                if (m.taken.number == 5 && m.taken.getType() != "t") {
                    sprintf(minkanPaiCharRed, "%s",
                            Pai(m.taken.type, 5, true).toString().c_str());
                } else {
                    sprintf(minkanPaiCharRed, "%s", m.taken.toString().c_str());
                }
                sprintf(ch, "{\"type\":\"daiminkan\",\"actor\":%d,\"target\":%d,\"pai\":"
                        "\"%s\",\"consumed\":[\"%s\",\"%s\",\"%s\"]}",
                        me, currentAction.actor, minkanPaiCharRed, minkanPaiChar,
                        minkanPaiChar, minkanPaiChar);
                ret = ch;
                checkFuroSuteForKan = true;
                break;

            case RESPONSE_TYPE_MJPIR_ANKAN:                char ankanPaiChar[10];
                sprintf(ankanPaiChar, "%s", m.taken.toString().c_str());
                char ankanPaiCharRed[10];
                if (m.taken.number == 5 && m.taken.getType() != "t") {
                    sprintf(ankanPaiCharRed, "%s",
                            Pai(m.taken.type, 5, true).toString().c_str());
                } else {
                    sprintf(ankanPaiCharRed, "%s", m.taken.toString().c_str());
                }
                sprintf(ch, "{\"type\":\"ankan\",\"actor\":%d,\"consumed\":[\"%s\",\"%"
                        "s\",\"%s\",\"%s\"],\"log\":\"%s\"}",
                        me, ankanPaiCharRed, ankanPaiChar, ankanPaiChar, ankanPaiChar,
                        html.c_str());
                ret = ch;
                
                checkFuroSuteForKan = true;
                break;
            case RESPONSE_TYPE_MJPIR_RON: {
                Deagari d = players[me].canDeagari(currentAction.pai, bakaze, jikaze(me));
                if (d.type == DeagariType::AVAIL && !isLastKakuteiAgari("ron")) {
                    sprintf(ch,
                            "{\"type\":\"hora\",\"actor\":%d,\"target\":%d,\"pai\":\"%s\"}",
                            me, currentAction.actor, currentPaiChar);
                    ret = ch;
                } else {
                    ret = "{\"type\":\"none\",\"cancel\":true}";
                }
                break;
            }
            case RESPONSE_TYPE_MJPIR_KAKAN:
                if (numPipais > 0) {
                    
                    char ankanPaiChar[10];
                    sprintf(ankanPaiChar, "%s", m.taken.toString().c_str());
                    char ankanPaiCharRed[10];
                    if (m.taken.number == 5 && m.taken.getType() != "t") {
                        sprintf(ankanPaiCharRed, "%s",
                                Pai(m.taken.type, 5, true).toString().c_str());
                    } else {
                        sprintf(ankanPaiCharRed, "%s", m.taken.toString().c_str());
                    }
                    
                    sprintf(ch, "{\"type\":\"kakan\",\"actor\":%d,\"pai\":\"%s\","
                            "\"consumed\":[\"%s\",\"%s\",\"%s\"]}",
                            me, ankanPaiChar, ankanPaiChar, ankanPaiChar, ankanPaiCharRed);
                    ret = ch;
                    
                    checkFuroSuteForKan = true;
                }
                break;
            case RESPONSE_TYPE_MJPIR_TSUMO:
                if (canTouhai(me)) {
                    // 九種九牌
                    ret = "{\"type\":\"ryukyoku\"}";
                    break;
                }
                
                if (!isLastKakuteiAgari("tsumo")) {
                    sprintf(ch,
                            "{\"type\":\"hora\",\"actor\":%d,\"target\":%d,\"pai\":\"%s\"}",
                            me, me, currentPaiChar);
                } else {
                    // ツモ切り
                    sprintf(ch, "{\"type\":\"dahai\",\"actor\":%d,\"pai\":\"%s\","
                            "\"tsumogiri\":true,\"log\":\"ignore last kakutei tsumo\"}",
                            me, previousAction.pai.toString().c_str());
                }
                
                ret = ch;
                break;
            case RESPONSE_TYPE_MJPIR_NONE:
                if (cancel) {
                    ret = "{\"type\":\"none\",\"cancel\":true}";
                } else {
                    ret = "{\"type\":\"none\",\"cancel\":false}";
                }
                break;
            default:
                break;
        }
        return ret + "\n";
    }
    
    std::vector<std::string> &split(const std::string &s, char delim,
                                    std::vector<std::string> &elems) {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }
    
    std::vector<Metrics> getHoraEstimator() {
        
//        // 対面目線の、自分に向けたベタオリ情報
        OpponentHoraEstimator toimenMesenNoOpponentHoraEstimator;
//        getOpponentHoraEstimator(getIdentifier(me, 2),
//                                 &toimenMesenNoOpponentHoraEstimator,
//                                 players[me].furos.size() == 0);
        
        return _HoraEstimator.getHoraEstimation(
                                                players[me].tehais, // 自分手牌
                                                players[me].furos,  //自分フーロ
                                                doraMarkers,        // ドラ
                                                players[me].sutehais,
                                                numPipais,                //残り牌
                                                richiKyoutaku,            //立直棒
                                                honba,                    //積み棒
                                                bakaze,                   // 場風
                                                jikaze(players[me]),      // 自風
                                                visiblePais(players[me]), // 見えている牌
                                                players[me].lastTsumohai, players[me].reachState,
                                                players[me].score >= 1000 && numPipais > 0,
                                                &toimenMesenNoOpponentHoraEstimator);
    }
    
    std::vector<Metrics> getHoraEstimatorWhenFuro(Pai p, int target) {
        
        //        cout<<"bakaze = "<<bakaze.toString()<<endl;
        //        cout<<"jikaze = "<<jikaze(players[me]).toString()<<endl;
        
//        // 対面目線の、自分に向けたベタオリ情報
        OpponentHoraEstimator toimenMesenNoOpponentHoraEstimator;
//        getOpponentHoraEstimator(getIdentifier(me, 2),
//                                 &toimenMesenNoOpponentHoraEstimator,
//                                 players[me].furos.size() == 0);
        
        return _HoraEstimator.getHoraEstimationWhenFuro(
                                                        p, relationalPosition(target),
                                                        players[me].tehais, // 自分手牌
                                                        players[me].furos,  //自分フーロ
                                                        doraMarkers,        // ドラ
                                                        players[me].sutehais,
                                                        numPipais,                //残り牌
                                                        richiKyoutaku,            //立直棒
                                                        honba,                    //積み棒
                                                        bakaze,                   // 場風
                                                        jikaze(players[me]),      // 自風
                                                        visiblePais(players[me]), // 見えている牌
                                                        players[me].reachState, &toimenMesenNoOpponentHoraEstimator);
    }
    
    int relationalPosition(int target) {
        for (int i = 0; i <= 3; i++) {
            if (getIdentifier(me, i) == target)
                return i;
        }
        return me;
    }
    
    int getIdentifier(int idx, int delta) {
        int a = idx + delta;
        if (a > 3) {
            a -= 4;
        }
        return a;
    }
    
    bool canTouhai(int num) {
        
        if (numPipais < NUM_INITIAL_PIPAIS - 4)
            return false;
        if (players[num].sutehais.size() > 0)
            return false;
        for (int i = 0; i < 4; i++) {
            if (players[num].furos.size() > 0)
                return false;
        }
        
        int yaotyuuCnt = 0;
        for (Pai p : players[num].tehais) {
            if (p.type == "t" || p.number == 9 || p.number == 1) {
                yaotyuuCnt++;
            }
        }
        return yaotyuuCnt >= 9;
    }
    
    void getOpponentHoraEstimator(int kijyun_no_hito,
                                  OpponentHoraEstimator *estimator,
                                  bool toimenRichi = false) {
        
        player _player[4]; // 爆打内部のプレイヤデータ構造体
        field _field;      // 爆打内部の卓データ構造体
        convertMjaiToBKUCPlayer(_player);
        convertMjaiToBKUCField(&_field);
        
        estimator->estimate(
                           &bkuc,
                           _player,
                           &_field);
        
        //        for (int i = 0; i < 3; i++) {
        //            for (int j = 0; j < OpponentHoraEstimator_NUMOF_TEHAI; j++) {
        //                cout << i << ":" << j << " - hojuTen = "
        //                << estimator.hojuTen[i][j] << " |  hojuProb = "
        //                << estimator.hojuProb[i][j] << endl;
        //            }
        //        }
    }
    
    void calcTenpaiProbEstimator() {
        player _player[4]; // 爆打内部のプレイヤデータ構造体
        field _field;      // 爆打内部の卓データ構造体
        convertMjaiToBKUCPlayer(_player);
        convertMjaiToBKUCField(&_field);
        
        // 計算
        _TenpaiProbEstimator.estimate(&bkuc, _player, &_field);
        
        //        // 出力
        //        std::cout <<  "0 : 下家 ";
        //        std::cout <<  "1 : 対面 ";
        //        std::cout <<  "2 : 上家 " << std::endl;
        //        for (auto i = 0; i < 3; i++) {
        //            std::cout
        //            << i + 1
        //            << "のテンパイ率 = "
        //            << tenpaiProbEstimator.tenpai_pro[i]
        //            << std::endl;
        //        }
    }
    
    std::vector<Pai> visibleSutePais() {
        std::vector<Pai> ret;
        
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < players[i].sutehais.size(); j++) {
                ret.push_back(players[i].sutehais[j]);
            }
        }
        return ret;
    }
    
    void convertMjaiToBKUCField(field *game_field) {
        game_field->bafuu = bakaze.identifier;     // 27 = E, 28 = S
        game_field->kyoku = kyokuNum;              // kyoku num
        game_field->game_long = tonpuusen ? 4 : 8; // max of kyoku(8 means tonnnan)
        game_field->ri_ti_bou = richiKyoutaku;     // reach bou
        game_field->honba = honba;                 // honba
        memset(game_field->dora, -1, sizeof(int) * 5);
        // dora, int[5], hainumber(0-33) otherways -1
        for (int i = 0; i < doraMarkers.size(); i++) {
            game_field->dora[i] = doraMarkers[i].identifier;
        }
        game_field->dora_num = -1;           // ? unused
        game_field->haitei = numPipais <= 1; // haitei flag
        game_field->kan_flag =
        0; // 0:none 1:some player act ankan 2:some player act minkan maybe..
        game_field->dora_basho = 0;            // ? unused
        game_field->nokori_maisuu = numPipais; // left of yama
        game_field->mieta_aka_hai =
        visibleAkaPaisNum(players[me]); // visible aka in taku
        
        PaiSet visiblePaiSet = PaiSet(visibleSutePais());
        
        for (int i = 0; i < 37; i++) game_field->sutehai_sarasi[i] = 0;
        
        // visible hai in kawa
        for (int i = 0; i < 37; i++) {
            int idx = Util::convertMJAIPainumToMjscore(i);
            game_field->sutehai_sarasi[idx] = visiblePaiSet.array[i];
        }
    }
    
    void convertMjaiToBKUCPlayer(player tatya[]) {
        // player info // 0 = me 1 = kamitya ...
        
        for (int i = 0; i < 4; i++) {
            
            int getnum = 0;
            if (me == 0) {
                if (i == 0)
                    getnum = 0;
                if (i == 1)
                    getnum = 1;
                if (i == 2)
                    getnum = 2;
                if (i == 3)
                    getnum = 3;
            } else if (me == 1) {
                if (i == 0)
                    getnum = 1;
                if (i == 1)
                    getnum = 2;
                if (i == 2)
                    getnum = 3;
                if (i == 3)
                    getnum = 0;
            } else if (me == 2) {
                if (i == 0)
                    getnum = 2;
                if (i == 1)
                    getnum = 3;
                if (i == 2)
                    getnum = 0;
                if (i == 3)
                    getnum = 1;
            } else if (me == 3) {
                if (i == 0)
                    getnum = 3;
                if (i == 1)
                    getnum = 0;
                if (i == 2)
                    getnum = 1;
                if (i == 3)
                    getnum = 2;
            }
            
            tatya[i].fu_ro_suu = (int)players[getnum].furos.size(); // num of furo
            
            for (int j = 0; j < 4; j++) {
                tatya[i].fu_ro[j] = -1;
            }
            //        副露した牌は以下に対応する
            //        チー　数字の一番小さな値に対応
            //        例:マンズ345のチー→2,ソーズ789のチー→24
            //        ポン　ポンした数字に34を足す   例:東のポン→61
            //        明槓　数字に+68を足す
            //        暗槓　数字に102を足す
            
            // int[4] furo. see readme if you wanna know hainum format
            
            tatya[i].aka_fu_ro = 0; // num of furo whick contain aka
            
            for (int k = 0; k < players[getnum].furos.size(); k++) {
                
                if (players[getnum].furos[k].type == "chi") {
                    tatya[i].fu_ro[k] = players[getnum].furos[k].smallestPai().identifier;
                } else if (players[getnum].furos[k].type == "pon") {
                    tatya[i].fu_ro[k] = players[getnum].furos[k].taken.identifier + 34;
                } else if (players[getnum].furos[k].type == "daiminkan") {
                    tatya[i].fu_ro[k] = players[getnum].furos[k].taken.identifier + 68;
                } else if (players[getnum].furos[k].type == "kakan") {
                    tatya[i].fu_ro[k] = players[getnum].furos[k].taken.identifier + 68;
                } else if (players[getnum].furos[k].type == "ankan") {
                    tatya[i].fu_ro[k] = players[getnum].furos[k].taken.identifier + 102;
                }
                
                if (players[getnum].furos[k].hasRed())
                    tatya[i].aka_fu_ro++;
            }
            
            tatya[i].aka_moti = 0; // 0 none 2 5mr 4 5pr 6 5sr
            tatya[i].ri_ti_flag = players[getnum].reachState; // reach or not
            tatya[i].ri_ti_sute = players[getnum].reachSutehaiIndex > 0
            ? players[getnum].reachSutehaiIndex - 1
            : -1;
            // sutehainum when reach. -1 means not declear reach
            
            // int[4]  sutehai position when berk. -1 means not declear berk
            for (int j = 0; j < 4; j++) {
                tatya[i].fu_ro_sute[j] = players[getnum].furoSutehai[j];
                //tatya[i].fu_ro_sute[j] = -1;
            }
            tatya[i].ippatu = 0;                     // ippatsu flag
            tatya[i].tenho = 0;                      // has tenho privilage
            tatya[i].tensuu = players[getnum].score; // point
            tatya[i].sutehai = vector<int>{};        // vector<int> hainum
            for (int j = 0; j < 34; j++) {
                tatya[i].tegawari[j] = 3;
                tatya[i].genbutu[j] = 0;
                tatya[i].hai[j] = -1;
            } // ? when init set 3 when dahai set 0
            // int[34] if genbutsu, index 1 otherwise 0
            
            vector<int> tedasitmp;
            
            for (int j = 0; j < players[getnum].sutehais.size(); j++) {
                tatya[i].sutehai.push_back(bkuc.util.trance34to37(players[getnum].sutehais[j].identifier));
                tatya[i].tegawari[players[getnum].sutehais[j].identifier] = 0;
                tatya[i].genbutu[players[getnum].sutehais[j].identifier] = 1;
                
                tedasitmp.push_back(players[getnum].tedashi[j]);
            }
            
            tatya[i].tedasi = tedasitmp;
            
            for (int j = 0; j < players[getnum].extraAnpais.size(); j++) {
                tatya[i].tegawari[players[getnum].extraAnpais[j].identifier] = 0;
                tatya[i].genbutu[players[getnum].extraAnpais[j].identifier] = 1;
            }
            // players[i].tedashi; // vector<int> concern with sutehai if
            // tedashi 1 otherwise 0
            
            tatya[i].jifuu = jikaze(players[getnum]).identifier; // jikaze
            tatya[i].fold_flag = 0;                              // use internal
        }
        // pai of me
        for (int j = 0; j < players[me].tehais.size(); j++) {
            tatya[0].hai[players[me].tehais[j].identifier]++;
        }
        // error check
        for (int i = 0; i < 4; i++) {
            if (tatya[i].fu_ro_sute[0] == -1) {
                if (tatya[i].fu_ro_suu != 0) {
                    // throw "errrrrrr";
                    cout << "errrr--" << tatya[i].fu_ro_suu << "--" << i << endl;
                }
            }
        }
    }
    
    bool canKyusyuKyuhai() {
        for (int i = 0; i < 4; i++) {
            if (players[i].furos.size() > 0 || players[i].sutehais.size() > 1) {
                return false;
            }
        }
        if (players[me].sutehais.size() > 0) {
            return false;
        }
        
        vector<int> chk;
        
        int yaotyuuCount = 0;
        for (int j = 0; j < players[me].tehais.size(); j++) {
            if (players[me].tehais[j].isYaochu()) {
                
                vector<int>::iterator cIter =
                find(chk.begin(), chk.end(), players[me].tehais[j].identifier);
                
                if (cIter != chk.end()) {
                    // printf("存在してます");
                } else {
                    yaotyuuCount++;
                    chk.push_back(players[me].tehais[j].identifier);
                }
            }
        }
        if (yaotyuuCount < 9) {
            return false;
        }
        // 1シャンテンだし国士狙う
        if (yaotyuuCount >= 12) {
            return false;
        }
        return true;
    }
    
    std::vector<std::string> split(const std::string &s, char delim) {
        
        // usage : std::vector<std::string> x = split("one:two::three", ':');
        
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }
    
    // 孤立牌の評価関数
    int EvalKoritsuhai(int paiIdx, int tehai[], int bakaze, int jikaze,
                       vector<Pai> doraMarkers) {
        const int MAX = 16;
        
        int point[42] = {MAX + 20, // Pai("?")
            0,        MAX - 2, MAX - 1, MAX,     MAX,     MAX,
            MAX - 1,  MAX - 2, 0,       0,       0,       MAX - 2,
            MAX - 1,  MAX,     MAX,     MAX,     MAX - 1, MAX - 2,
            0,        0,       0,       MAX - 2, MAX - 1, MAX,
            MAX,      MAX,     MAX - 1, MAX - 2, 0,       0,
            0,        0,       0,       0,       10,      10,
            10,       MAX + 1, MAX + 1, MAX + 1, //赤
            -1};
        
        // 1.9牌
        for (int i = 1; i < 30; i += 10) {
            //１間面子
            if (tehai[i + 2] & tehai[i + 3] & tehai[i + 4]) {
                point[i] = MAX - 3;
            }
            //１間ターツ
            else if (tehai[i + 2] & tehai[i + 3]) {
                point[i] = MAX - 4;
            }
            //リャンカン孤立
            else if (tehai[i / 10 + 5]) {
                point[i] = MAX - 7;
            }
            //面子スジ
            else if (tehai[i + 3] & tehai[i + 4] & tehai[i + 5]) {
                point[i] = MAX - 8;
            }
            //ターツスジ
            else if (tehai[i + 3] & tehai[i + 4]) {
                point[i] = MAX - 9;
            }
            //スジ孤立
            else if (tehai[i + 3] == 1 & !tehai[i + 2] & !tehai[i + 4]) {
                point[i] = MAX - 14;
            }
            //連続スジ孤立
            else if (tehai[i + 3] >= 2 & !tehai[i + 2] & !tehai[i + 4]) {
                point[i] = MAX - 15;
            }
            //孤立
            else {
                point[i] = MAX - 10;
            }
        }
        for (int i = 9; i < 30; i += 10) {
            //１間面子
            if (tehai[i - 2] & tehai[i - 3] & tehai[i - 4]) {
                point[i] = MAX - 3;
            }
            //１間ターツ
            else if (tehai[i - 2] & tehai[i - 3]) {
                point[i] = MAX - 4;
            }
            //リャンカン孤立
            else if (tehai[i / 10 + 5]) {
                point[i] = MAX - 7;
            }
            //面子スジ
            else if (tehai[i - 3] & tehai[i - 4] & tehai[i - 5]) {
                point[i] = MAX - 8;
            }
            //ターツスジ
            else if (tehai[i - 3] & tehai[i - 4]) {
                point[i] = MAX - 9;
            }
            //スジ孤立
            else if (tehai[i - 3] == 1 & !tehai[i - 2] & !tehai[i - 4]) {
                point[i] = MAX - 14;
            }
            //連続スジ孤立
            else if (tehai[i - 3] >= 2 & !tehai[i - 2] & !tehai[i - 4]) {
                point[i] = MAX - 15;
            }
            //孤立
            else {
                point[i] = MAX - 10;
            }
        }
        
        //字牌
        for (int i = 31; i <= 37; i++) {
            
            //連風牌
            if (i == bakaze && i == jikaze) {
                point[i] = MAX - 5;
            }
            //役牌
            else if (i == bakaze || i == jikaze || i >= 35) {
                point[i] = MAX - 6;
            }
            //オタ風
            else {
                point[i] = MAX - 11;
            }
        }
        // ドラはMAX
        for (Pai p : doraMarkers) {
            int idx_tmp =
            Util::convertMJAIPainumToMjscore(p.nextForDora().identifier);
            point[idx_tmp] = MAX;
        }
        
        return point[paiIdx];
    }
    
    bool isLastKakuteiAgari(string agariType) {
        if(!isOrasu()) return false;
        if(getRank(me) < 4) return false;
        if(players[me].reachState > 0) return false;
        // TODO
        return false;
    }
    
    bool isOrasu() {
        bool ret = false;
        if(tonpuusen) {
            if((bakaze.identifier > Pai("E").identifier) || kyokuNum == 4) {
                ret = true;
            }
        } else {
            if((bakaze.identifier > Pai("S").identifier) || (bakaze.equal(Pai("S")) && kyokuNum == 4)) {
                ret = true;
            }
        }
        return ret;
    }
    
    int getRank(int target_id) {
        int rank = 4;
        for (int i = 0; i < 4; i++) {
            if (i == target_id)
                continue;
            if (players[i].score <= players[target_id].score)
                rank--;
        }
        return rank;
    }
    
    string IntToString(int number) {
        std::ostringstream ss;
        ss << number;
        return ss.str();
    }
    
    template <typename... Args>
    std::string string_sprintf(const char *format, Args... args) {
        int length = std::snprintf(nullptr, 0, format, args...);
        assert(length >= 0);
        
        char *buf = new char[length + 1];
        std::snprintf(buf, length + 1, format, args...);
        
        std::string str(buf);
        delete[] buf;
        return std::move(str);
    }
    
};
#endif
