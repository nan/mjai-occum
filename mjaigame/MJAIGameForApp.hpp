#ifndef MJAIGameForApp_hpp
#define MJAIGameForApp_hpp


class MJAIGameForApp : public MJAIGame {
public:
    
    bool ignoreOutlogForApp = false;
    
    std::string updateStateAndRespondToAction(string lineString, bool reachDecleared) {
        picojson::value v;
        std::string err;
        picojson::parse(v, lineString.c_str(), lineString.c_str() + strlen(lineString.c_str()), &err);
        picojson::object obj = v.get<picojson::object>();
        
        std::string ret = "{\"type\":\"none\"}\n";
        int actor = -1;
        Pai p;
        string type = obj["type"].get<std::string>();
        
        if(type == key_start_hello) {
            
            ret = string_sprintf("{\"type\":\"join\",\"name\":\"%s\",\"room\":\"default\"}\n",MY_PLAYER_NAME);
            
        }else if(type == key_start_game) {
            
            initScore();
            //me = obj["id"].get<double>();
            
        }else if(type == key_start_kyoku) {
            
            init();
            ignoreOutlogForApp = false;
            
            bakaze = obj["bakaze"].get<std::string>();
            kyokuNum = obj["kyoku"].get<double>();
            if(kyokuNum  == 1) {
                chichaNum = obj["oya"].get<double>();
            }
            honba = obj["honba"].get<double>();
            
            doraMarkers.clear();
            doraMarkers.push_back(Pai(obj["dora_marker"].get<std::string>()));
            
            picojson::array& tehaiList = obj["tehais"].get<picojson::array>();
            int index = 0;
            for (picojson::array::iterator it = tehaiList.begin(); it != tehaiList.end(); it++) {
                picojson::array& tehaiList2 =  it->get<picojson::array>();
                for (picojson::array::iterator it2 = tehaiList2.begin(); it2 != tehaiList2.end(); it2++) {
                    Pai p = Pai(it2->get<std::string>());
                    players[index].tehais.push_back(p);
                }
                index++;
            }
            for(int i=0;i<4;i++)sortTehai(i);
            
        } else if (type == key_tsumo) {
            actor = obj["actor"].get<double>();
            p = Pai(obj["pai"].get<std::string>());
            
            numPipais--;
            
            cout << "key_tsumo" << endl;
            cout << endl << toString() << endl;
            cout << endl << actor << "pai:" << p.toString() << " actor:" << actor <<endl;
            
            //if(actor == me) {
            players[actor].tehais.push_back(p);
            players[actor].lastTsumohai = p;
            sortTehai(actor);
            //ret = respondToAction(me,actor,p);
            //}
            
            cout << endl << toString() << endl;
            
        } else if (type == key_dahai) {
            actor = obj["actor"].get<double>();
            p = Pai(obj["pai"].get<std::string>());
            bool tsumogiri = obj["tsumogiri"].get<bool>();
            
            players[actor].sutehais.push_back(p);
            players[actor].tedashi.push_back(!tsumogiri);
            if(players[actor].reachState == false) players[actor].extraAnpais.clear();
            else  players[actor].extraAnpais.push_back(p);
            
            cout << "key_dahai" << endl;
            cout << endl << toString() << endl;
            cout << endl << actor << "pai:" << p.toString() << " actor:" << actor <<endl;
            
            //if(actor == me)
            deleteTehai(actor,p);
            
            cout << endl << toString() << endl;
            
            
            //            if(!(actor == me) && obj["possible_actions"].is<picojson::array>() && obj["possible_actions"].get<picojson::array>().size() > 0) {
            //                cout << "possible action exist" << endl;
            //                //sortTehai(actor);
            //                ret = respondToAction(me,actor,p);
            //            }
            //            if (previousAction.type  == key_pon || previousAction.type  == key_chi || previousAction.type  == key_ankan || previousAction.type  == key_daiminkan ) {
            //                for(int i=0;i<4;i++) {
            //                    if(players[actor].furoSutehai[i]==-1) {
            //                        players[actor].furoSutehai[i]=p.identifier;
            //                        break;
            //                    }
            //                }
            //            }
            
        } else if (type == key_pon || type == key_chi || type == key_ankan || type == key_daiminkan ) {
            
            if(type==key_ankan || type == key_daiminkan) ignoreOutlogForApp = true;
            
            actor = obj["actor"].get<double>();
            int target = 0;
            if(type!=key_ankan)target = obj["target"].get<double>();
            p = Pai(obj["pai"].get<std::string>());
            
            picojson::array& tehaiList = obj["consumed"].get<picojson::array>();
            vector<Pai> consume;
            for (picojson::array::iterator it = tehaiList.begin(); it != tehaiList.end(); it++) {
                Pai _p = Pai(it->get<std::string>());
                consume.push_back(_p);
                deleteTehai(actor,_p);
            }
            players[actor].furos.push_back(
                                           Furo(type, p, target, consume)
                                           );
            
        } else if (type == key_kakan) {
            
            ignoreOutlogForApp = true;
            
            actor = obj["actor"].get<double>();
            p = Pai(obj["pai"].get<std::string>());
            for(int i =0;i<players[actor].furos.size();i++){
                if(players[actor].furos[i].type == "pon" && players[actor].furos[i].taken.equal(p)) {
                    players[actor].furos[i] = Furo("kakan", p, 0, {p,p,p});
                    break;
                }
            }
        } else if (type == key_reach) {
            
        } else if (type == key_reach_accepted) {
            
            actor = obj["actor"].get<double>();
            players[actor].reachState = true;
            players[actor].reachSutehaiIndex = players[actor].sutehais.size();
            
            if( ignoreOutlogForApp == false && players[actor].sutehais.size() > 9) {
                
                PlayerData tmpActor = players[actor];
                PlayerData tmpToimen = players[2];
                if(actor!=2) {
                    players[actor].copy(tmpToimen);
                    players[2].copy(tmpActor);
                    players[actor].identifier = 2;
                    players[2].identifier = actor;
                }
                
                std::string s = toJson();
                std::ofstream out(PlayerData::IntToString(countOfFileNum)+".json");
                out << s;
                out.close();
                ignoreOutlogForApp = true;
                
                if(actor!=2) {
                    players[actor].copy(tmpActor);
                    players[2].copy(tmpToimen);
                    players[actor].identifier = actor;
                    players[2].identifier = 2;
                }
            }
            
        } else if (type == key_dora) {
            p = Pai(obj["dora_marker"].get<std::string>());
            doraMarkers.push_back(p);
        } else if (type == key_hora) {
            picojson::array& pointArray = obj["scores"].get<picojson::array>();
            int idx=0;
            for (picojson::array::iterator it = pointArray.begin(); it != pointArray.end(); it++) {
                players[idx++].score = it->get<double>();
                cout<<"key_hora score"<<idx-1<<" "<<players[idx-1].score<<endl;
            }
        } else if (type == key_ryukyoku) {
            picojson::array& pointArray = obj["scores"].get<picojson::array>();
            int idx=0;
            for (picojson::array::iterator it = pointArray.begin(); it != pointArray.end(); it++) {
                players[idx++].score = it->get<double>();
                cout<<"key_ryukyoku score"<<idx-1<<" "<<players[idx-1].score<<endl;
            }
        } else if (type == key_end_kyoku) {
            
        }
        
        previousAction.type = type;
        previousAction.actor = actor;
        previousAction.pai = p;
        
        return ret;
        
    }
    
};
#endif
