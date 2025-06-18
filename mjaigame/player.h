#ifndef mjaiC___player_h
#define mjaiC___player_h

#include "picojson.h"
#include "yuukouhai.h"

enum DeagariType { NOT_AVALI = 0, FURITEN, KATA_AGARI, AVAIL };

class Deagari {
public:
    vector<Pai> pais;
    vector<MJScore> mjscores;
    DeagariType type;
    
    Deagari() {
        pais.clear();
        mjscores.clear();
        type = NOT_AVALI;
    }
    
    Deagari(vector<Pai> _pais, DeagariType _type) {
        pais = _pais;
        type = _type;
        mjscores.clear();
    }
};

class PlayerData {
public:
    int identifier;
    int score = 25000;
    std::vector<Pai> tehais;
    std::vector<Furo> furos;
    std::vector<Pai> sutehais;
    
    Pai lastTsumohai = Pai("?");
    
    std::vector<Pai> extraAnpais; // リーチ後に通った牌
    bool reachState;
    int reachSutehaiIndex;
    
    std::vector<int> tedashi; // 1 = tedashi 0 = tsumogiri
    std::vector<int> furoSutehai;
    
    std::string toString() {
        char ch[10240];
        
        std::string tehais_s = "";
        for (Pai p : tehais) {
            tehais_s += p.toString() + " ";
        }
        std::string furos_s = "";
        for (Furo f : furos) {
            furos_s += f.type + "." + f.taken.toString() + " ";
        }
        std::string sutehais_s = "";
        for (Pai p : sutehais) {
            sutehais_s += p.toString() + " ";
        }
        
        std::string extraAnpais_s = "";
        for (Pai p : extraAnpais) {
            extraAnpais_s += p.toString() + " ";
        }
        
        std::string tedashi_s = "";
        for (int t : tedashi) {
            char tary[10];
            sprintf(tary, "%d ", t);
            tedashi_s += tary;
        }
        
        std::string furosutehais_s = "";
        for (int t : furoSutehai) {
            char tary[10];
            sprintf(tary, "%d ", t);
            furosutehais_s += tary;
        }
        
        sprintf(ch, "score = %d | sutehai = %s | tehai = %s | furo = %s | "
                "lastTsumohai = %s | reachState = %d | reachSutehaiIndex = %d "
                "| extraAnpais_s = %s | tedashi = %s | furoSutehai = %s",
                score, sutehais_s.c_str(), tehais_s.c_str(), furos_s.c_str(),
                lastTsumohai.toString().c_str(), reachState, reachSutehaiIndex,
                extraAnpais_s.c_str(), tedashi_s.c_str(), furosutehais_s.c_str());
        return ch;
    }
    
    void copy(PlayerData p) {
        identifier = p.identifier;
        score = p.score;
        reachState = p.reachState;
        reachSutehaiIndex = p.reachSutehaiIndex;
        
        tehais.clear();
        for (int i = 0; i < p.tehais.size(); i++) {
            tehais.push_back(p.tehais[i]);
        }
        
        furos.clear();
        for (int i = 0; i < p.furos.size(); i++) {
            furos.push_back(p.furos[i]);
        }
        
        sutehais.clear();
        for (int i = 0; i < p.sutehais.size(); i++) {
            sutehais.push_back(p.sutehais[i]);
        }
    }
    
    picojson::value toJson() {
        picojson::object obj;
        obj["identifier"] = (picojson::value)(PlayerData::IntToString(identifier));
        obj["score"] = (picojson::value)(PlayerData::IntToString(score));
        obj["reachState"] = (picojson::value)(PlayerData::IntToString(reachState));
        obj["reachSutehaiIndex"] =
        (picojson::value)(PlayerData::IntToString(reachSutehaiIndex));
        {
            picojson::array v;
            for (Pai p : tehais) {
                v.push_back(PlayerData::toJsonPai(p));
            }
            obj.insert(make_pair("tehais", picojson::value(v)));
        }
        {
            picojson::array v;
            for (Furo p : furos) {
                v.push_back(PlayerData::toJsonFuro(p));
            }
            obj.insert(make_pair("furos", picojson::value(v)));
        }
        {
            picojson::array v;
            for (Pai p : sutehais) {
                v.push_back(PlayerData::toJsonPai(p));
            }
            obj.insert(make_pair("sutehais", picojson::value(v)));
        }
        {
            picojson::array v;
            for (int p : furoSutehai) {
                v.push_back(PlayerData::toJsonPai(p));
            }
            obj.insert(make_pair("furo_sute", picojson::value(v)));
        }
        {
            picojson::array v;
            for (int p : tedashi) {
                v.push_back((picojson::value)(PlayerData::IntToString(p)));
            }
            obj.insert(make_pair("tedashi", picojson::value(v)));
        }
        
        picojson::value val(obj);
        // cout << val.serialize() << endl;
        
        return val;
    }
    
    void fromJson(std::string json) {}
    
    static string IntToString(int number) {
        std::ostringstream ss;
        ss << number;
        return ss.str();
    }
    static picojson::value toJsonFuro(Furo p) {
        picojson::object obj;
        
        obj["pai"] = (picojson::value)(p.taken.toString());
        obj["target"] = (picojson::value)(PlayerData::IntToString(p.target));
        obj["type"] = (picojson::value)(p.type);
        
        picojson::array a;
        for (Pai __p : p.consumed) {
            a.push_back(PlayerData::toJsonPai(__p));
        }
        obj.insert(make_pair("consume", picojson::value(a)));
        
        picojson::value val(obj);
        
        return val;
    }
    static picojson::value toJsonPai(Pai p) {
        picojson::object obj;
        
        obj["pai"] = (picojson::value)(p.toString());
        obj["pai_id"] = (picojson::value)(PlayerData::IntToString(p.identifier));
        obj["red"] = (picojson::value)(PlayerData::IntToString(p.red));
        // obj["tsumogiri"] = (picojson::value)(IntToString(p.tsumogiri));
        
        picojson::value val(obj);
        
        return val;
    }
    
    PlayerData() { zeroInit(); }
    PlayerData(int _identifire, int _reachSutehaiIndex = 0) {
        zeroInit();
        identifier = _identifire;
        reachSutehaiIndex = _reachSutehaiIndex;
    }
    void zeroInit() {
        identifier = score = reachState = reachSutehaiIndex = 0;
        tehais.clear();
        furos.clear();
        sutehais.clear();
        extraAnpais.clear();
        furoSutehai.clear();
        furoSutehai.push_back(-1);
        furoSutehai.push_back(-1);
        furoSutehai.push_back(-1);
        furoSutehai.push_back(-1);
        tedashi.clear();
    }
    bool equal(PlayerData player) {
        if (player.identifier == identifier) {
            return true;
        }
        return false;
    }
    bool empty() { return identifier == 0 && score == 0; }
    
    static vector<Pai> getMachi(vector<Pai> tehais, vector<Furo> furos) {
        
        int t[38] = {0};
        for (Pai p : tehais) {
            int _t = Util::convertMJAIPainumToMjscore(p.identifier);
            t[_t]++;
        }
        
        Yuukouhai yuu;
        yuu.set_tehai(t);
        yuu.set_fuurosuu((int)furos.size());
        vector<int> work;
        if (yuu.NormalSyanten() == 0) {
            yuu.NormalYuukou(&work);
        } else if (yuu.TiitoituSyanten() == 0) {
            yuu.TiitoiYuukou(&work);
        } else if (yuu.KokusiSyanten() == 0) {
            yuu.KokusiYuukou(&work);
        }
        
        vector<Pai> ret;
        for (int f : work) {
            ret.push_back(Pai(Util::convertMjscorePainumToMjAI(f)));
        }
        return ret;
    }
    
    bool isFuriten(vector<Pai> mati) {
        
        for (int m = 0; m < sutehais.size(); m++) {
            for (int k = 0; k < mati.size(); k++) {
                if (sutehais[m].equal(mati[k])) {
                    return true;
                }
            }
        }
        return false;
    }
    
    Deagari canDeagari(Pai p, Pai jikaze, Pai bakaze, bool doReach = false) {
        
        if (tehais.size() % 3 != 1) {
            throw "tehai count must be mod 3 equeal 1 (canDeagari)";
        }
        
        Deagari ret = Deagari();
        ret.type = DeagariType::NOT_AVALI;
        
        // フリテン
        
        cout << "furiten check. furosuu = " << furos.size() << endl;
        
        vector<Pai> mati = PlayerData::getMachi(tehais, furos);
        
        cout << "mati = " << endl;
        for (Pai p : mati) {
            cout << p.toString() << endl;
        }
        
        if (isFuriten(mati)) {
            cout << "Furiten" << endl;
            ret.type = DeagariType::FURITEN;
            return ret;
        } else {
            cout << "NOT Furiten" << endl;
        }
        
        // 役あり
        if (reachState || doReach) {
            cout << "Deagari OK due to REACH" << endl;
            ret.type = DeagariType::AVAIL;
        } else {
            int t[41] = {0};
            for (Pai __p : tehais) {
                int _t = Util::convertMJAIPainumToMjscore(__p.identifier);
                t[_t]++;
            }
            t[Util::convertMJAIPainumToMjscore(p.identifier)]++;
            
            int f[20] = {0};
            PlayerData::convertMJAIFuroToMJScoreFuro(f, furos);
            
            MJScore sc =
            getMJScore(t, f, Util::convertMJAIPainumToMjscore(p.identifier),
                       Util::convertMJAIPainumToMjscore(bakaze.identifier),
                       Util::convertMJAIPainumToMjscore(jikaze.identifier));
            
            sc.Run();
            
            ret.mjscores.push_back(sc);
            ret.pais.push_back(p);
            
            int err = sc.Get_ErrorCode();
            
            cout << "Deagari Error Code = " << sc.Get_ErrorCodeString() << endl;
            
            if (err == MJScore::ERRORCODE::SEIJYOU) {
                cout << "Deagari OK" << endl;
                ret.type = DeagariType::AVAIL;
            } else {
                // if(err!=0) { throw "Deagari Error"; }
                cout << "Deagari NG" << endl;
            }
        }
        return ret;
    }
    
    MJScore getMJScore(int tehai[], int furo[], int agarihai, int bakaze,
                       int jikaze) {
        MJScore score;
        //ルール入力
        score.Avail_Akahai(true);           //赤牌有り
        score.Avail_Kuitan(true);           //クイタン有り
        score.Avail_ManganKiriage(true);    //満貫切り上げ有り
        score.Avail_Ba1500(false);          //場千五有り
        score.Avail_DoubleKokushi13(false); //国士13面待ちダブル役満有り
        score.Avail_DoubleTyuuren9(false); //九連宝燈9面待ちダブル役満有り
        score.Avail_DoubleDaisuusii(false); //大四喜ダブル役満有り
        score.Avail_DoubleSuttan(false);    //四暗刻単騎ダブル役満有り
        
        //特殊役成立フラグ入力
        score.Is_Riichi(reachState);       //リーチ
        score.Is_Tenhou(false);            //天和
        score.Is_Tiihou(false);            //地和
        score.Is_DoubleRiichi(reachState); //ダブルリーチ
        score.Is_Ippatu(false);            //一発
        score.Is_Tyankan(false);           //チャンカン
        score.Is_Rinsyan(false);           //リンシャン
        score.Is_NagashiMangan(false);     //流し満貫
        score.Is_Haitei(false);            //ハイテイ
        
        //必須情報入力
        score.Set_Tehai(tehai);       //手牌
        score.Set_Agarihai(agarihai); //あがり牌
        score.Set_Bakaze(bakaze);     //場風
        score.Set_Jikaze(jikaze);     //自風
        
        //追加情報入力
        score.Set_Tumoagari(false); //ツモあがりかどうか
        score.Set_Fuuro(furo);      //フーロ牌入力
        
        return score;
    }
    
    static void convertMJAIFuroToMJScoreFuro(int f[], vector<Furo> furos) {
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
        for (int i = 0; i < 20; i++) {
            f[i] = furos4MJscore[i];
        }
    }
    
    vector<bool> getTedashiAsVecBool() {
        vector<bool> r;
        for (int i : tedashi) {
            r.push_back(i == 1);
        }
        return r;
    }
};
#endif
