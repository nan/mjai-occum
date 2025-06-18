#include "easywsclient.hpp"

#include <iostream>

#include "MJAIGame.hpp"
#include "MJAIGameForApp.hpp"

#include "socket.h"

#include <cstdlib>
#include <dirent.h>

void oneUpdateStateAndResponedToAction(MJAIGame *game, string s);

void probDistTest() {
    
    // ProbDist(確率分布)
    //
    // キーはvector<double>のsizeは4
    // 例えば起家が1000ALLをつもったら
    // vector<double>{3000,-1000,-1000,-1000}
    //
    // 値は1以下の少数
    // 確率分布なので値を全部足したら1にならないとDAME
    
    AverageRankCalculator calculator;
    {
        ProbDist dist;
        double agari = 0.11;
        double houjyu = 0.05;
        dist.upsert({0, 0, 0, 0}, 1 - agari - houjyu);
        dist.upsert({18000, -18000, 0, 0}, agari);
        dist.upsert({-3900, 3900, 0, 0}, houjyu);
        cout << MJAIGame::probDistToAverageRank(dist, calculator, 0, 8,
                                                {25000, 25000, 25000, 25000})
        << endl;
    }
    {
        ProbDist dist;
        dist.upsert({0, 0, 0, 0}, 0.9);
        dist.upsert({2000, -2000, 0, 0}, 0.1);
        cout << MJAIGame::probDistToAverageRank(dist, calculator, 0, 8,
                                                {25000, 25000, 25000, 25000})
        << endl;
    }
}

void protocolTest() {
    
    // プロトコルはMJAIプロトコル
    MJAIGame game;
    
    game.me = 2; // 自分の番号
    
    oneUpdateStateAndResponedToAction(
                                      &game,
                                      "{\"type\":\"hello\",\"protocol\":\"mjsonp\",\"protocol_version\":1}");
    oneUpdateStateAndResponedToAction(
                                      &game, "{\"type\":\"start_kyoku\",\"bakaze\":\"E\",\"kyoku\":1,\"honba\":"
                                      "0,\"oya\":0,\"dora_marker\":\"6s\",\"tehais\":[[\"8m\",\"S\","
                                      "\"4m\",\"7s\",\"P\",\"5s\",\"3s\",\"7m\",\"3p\",\"1p\",\"F\","
                                      "\"1s\",\"7m\"],[\"2m\",\"N\",\"6p\",\"1m\",\"7p\",\"C\",\"6s\","
                                      "\"8s\",\"N\",\"9s\",\"F\",\"4p\",\"3s\"],[\"W\",\"4m\",\"9p\","
                                      "\"9s\",\"9m\",\"8s\",\"1p\",\"8s\",\"N\",\"5mr\",\"6m\",\"W\","
                                      "\"9s\"],[\"5m\",\"1s\",\"4m\",\"S\",\"2s\",\"F\",\"P\",\"8p\","
                                      "\"3m\",\"2p\",\"4p\",\"1s\",\"4s\"]]}");
    
    oneUpdateStateAndResponedToAction(
                                      &game, "{\"type\":\"hora\",\"actor\":1,\"target\":0,\"pai\":\"7s\","
                                      "\"uradora_markers\":[\"3s\"],\"hora_tehais\":[\"2m\",\"3m\","
                                      "\"4m\",\"4p\",\"5pr\",\"6p\",\"6p\",\"7p\",\"8p\",\"6s\",\"8s\","
                                      "\"N\",\"N\"],\"yakus\":[[\"akadora\",1],[\"reach\",1]],\"fu\":40,"
                                      "\"fan\":2,\"hora_points\":2600,\"deltas\":[-4400,8400,0,0],"
                                      "\"scores\":[27500,22300,24300,25900]}");
    
    oneUpdateStateAndResponedToAction(
                                      &game, "{\"type\":\"ryukyoku\",\"reason\":\"fanpai\",\"tehais\":[[\"5m\","
                                      "\"5m\",\"5mr\",\"3s\",\"3s\",\"N\",\"N\"],[\"?\",\"?\",\"?\",\"?"
                                      "\",\"?\",\"?\",\"?\",\"?\",\"?\",\"?\",\"?\",\"?\",\"?\"],[\"?\","
                                      "\"?\",\"?\",\"?\",\"?\",\"?\",\"?\",\"?\",\"?\",\"?\",\"?\",\"?"
                                      "\",\"?\"],[\"?\",\"?\",\"?\",\"?\"]],\"tenpais\":[true,false,"
                                      "false,false],\"deltas\":[3000,-1000,-1000,-1000],\"scores\":["
                                      "28000,24000,24000,24000]}");
    
    oneUpdateStateAndResponedToAction(
                                      &game, "{\"type\":\"tsumo\",\"actor\":0,\"pai\":\"2m\"}");
    oneUpdateStateAndResponedToAction(
                                      &game,
                                      "{\"type\":\"dahai\",\"actor\":0,\"pai\":\"1s\",\"tsumogiri\":false}");
    oneUpdateStateAndResponedToAction(
                                      &game, "{\"type\":\"tsumo\",\"actor\":1,\"pai\":\"C\"}");
    oneUpdateStateAndResponedToAction(
                                      &game,
                                      "{\"type\":\"dahai\",\"actor\":1,\"pai\":\"N\",\"tsumogiri\":false}");
    oneUpdateStateAndResponedToAction(
                                      &game, "{\"type\":\"tsumo\",\"actor\":2,\"pai\":\"1p\"}");
    
    game.toJson();
}

void oneUpdateStateAndResponedToActionForApp(MJAIGameForApp *game, string s) {
    cout << " - > " << s << endl;
    cout << " < - " << game->updateStateAndRespondToAction(s, false) << endl;
}

void oneUpdateStateAndResponedToAction(MJAIGame *game, string s) {
    cout << " - > " << s << endl;
    cout << " < - " << game->updateStateAndRespondToAction(s, false) << endl;
}

void readFileAndParseForApp(string filename, MJAIGameForApp *game) {
    
    FILE *fp = fopen(filename.c_str(), "r");
    if (fp == NULL) {
        cout << "can't open : " << filename << endl;
        return;
    }
    
    char row[READ_FILE_BUFFER_SIZE];
    
    while (fgets(row, sizeof(row), fp) != NULL) {
        oneUpdateStateAndResponedToActionForApp(game, row);
    }
    
    fclose(fp);
    
    return;
}

void readFileAndParse(string filename, MJAIGame *game) {
    
    FILE *fp = fopen(filename.c_str(), "r");
    if (fp == NULL) {
        cout << "can't open : " << filename << endl;
        return;
    }
    
    char row[READ_FILE_BUFFER_SIZE];
    
    while (fgets(row, sizeof(row), fp) != NULL) {
        oneUpdateStateAndResponedToAction(game, row);
    }
    
    fclose(fp);
    
    return;
}

void socketTest() {
    // mjai server --port=11600 --game_type=tonnan --games=100 --room=default
    // --log_dir=./log ./my-ai.sh ./bin/mjai-silica-master/mjai-silica
    // ./bin/mjai-silica-master/mjai-silica ./bin/mjai-silica-master/mjai-silica
    // mjai server --port=11600 --game_type=tonnan --games=100 --room=default
    // --log_dir=./log ./my-ai.sh mjai-shanten mjai-shanten mjai-shanten
    // mjai server --port=11600 --game_type=one_kyoku --room=default
    // --log_dir=./log mjai-shanten mjai-shanten mjai-shanten
    
    // coffee main.coffee mjsonp://localhost:11600/default
    
    // rm -rf log | mkdir log | mjai server --port=11600 --game_type=tonnan
    // --games=100 --room=default --log_dir=./log ./my-ai.sh
    // ./bin/mjai-silica-master/mjai-silica ./bin/mjai-silica-master/mjai-silica
    // ./bin/mjai-silica-master/mjai-silica
    
    FILE *fp_write = fopen("rank.txt", "w");
    fprintf(fp_write, "");
    fclose(fp_write);
    
    FILE *fp_write2 = fopen("stats.txt", "w");
    fprintf(fp_write2, "");
    fclose(fp_write2);
    
    Socket s;
    MJAIGame game;
    while (true) {
#ifdef SOCKET_LOCAL
        int ret = s.start("127.0.0.1", 11600, &game);
#elif TENHOUPORT == 1
        int ret = s.start("192.168.0.3", 11601, &game);
#else
        int ret = s.start("192.168.0.4", 11600, &game);
#endif
        
        if (ret == -1) {
            break;
        }
    }
    double rank[5] = {0};
    char s_tmp[256];
    double total = 0;
    double total_line = 0;
    FILE *fp_read = fopen("rank.txt", "r");
    if (fp_read != NULL) {
        while (fgets(s_tmp, 256, fp_read) != NULL) {
            int _atoi = atoi(s_tmp);
            total += _atoi;
            total_line++;
            rank[_atoi]++;
        }
    }
    fclose(fp_read);
    
    fp_write = fopen("rank.txt", "a");
    fprintf(fp_write, "total = %.0f total_line = %.0f average_rank = %f\n", total,
            total_line, total / total_line);
    fprintf(fp_write, "1位率 = %.2f%%\n", (rank[1] / total_line) * 100);
    fprintf(fp_write, "2位率 = %.2f%%\n", (rank[2] / total_line) * 100);
    fprintf(fp_write, "3位率 = %.2f%%\n", (rank[3] / total_line) * 100);
    fprintf(fp_write, "4位率 = %.2f%%\n", (rank[4] / total_line) * 100);
    fprintf(fp_write, "あがり率 = %.2f%%\n",
            (game.totalHora / game.totalKyokusuu) * 100);
    fprintf(fp_write, "放銃率 = %.2f%%\n",
            (game.totalHoujyuu / game.totalKyokusuu) * 100);
    fprintf(fp_write, "フーロ率 = %.2f%%\n",
            (game.totalFuro / game.totalKyokusuu) * 100);
    fprintf(fp_write, "あがり時リーチ率 = %.2f%%\n",
            (game.totalRichiWhenHora / game.totalMenzenHora) * 100);
    fclose(fp_write);
}

void parseForApp(string filepath, MJAIGameForApp *game) {
    
    // readFileAndParse("/Users/nibble1242/Desktop/haifu/mjson_n1/2010032120gm-00e1-0000-ff34392e&tw=2.mjson");
    // readFileAndParse("/Users/nibble1242/Desktop/haifu/mjson_n1/2011013121gm-00a9-0000-da6eda8b&tw=2.mjson");
    
    const char *path = filepath.c_str();
    DIR *dp;       // ディレクトリへのポインタ
    dirent *entry; // readdir() で返されるエントリーポイント
    
    dp = opendir(path);
    if (dp == NULL)
        exit(1);
    do {
        entry = readdir(dp);
        if (entry != NULL)
            std::cout << path << entry->d_name << std::endl;
        if (entry != NULL) {
            std::string s = entry->d_name;
            
            if (s != "" && s.find("mjson") != std::string::npos) {
                readFileAndParse(path + s, game);
            }
        }
        if (game->countOfFileNum >= 999)
            break;
    } while (entry != NULL);
}

void readJson() {
    
    MJAIGameForApp game;
    
    const char str[] = R"(
    {"bakaze":{"pai":"E","pai_id":"27","red":"0"},"chichaNum":"0","dora":[{"pai":"8m","pai_id":"7","red":"0"}],"honba":"0","kyokuNum":"2","me":"3","numPiPais":"70","players":[{"furo_sute":[{"pai":"P","pai_id":"31","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"}],"furos":[{"consume":[{"pai":"2m","pai_id":"1","red":"0"},{"pai":"2m","pai_id":"1","red":"0"}],"pai":"2m","target":"2","type":"pon"}],"identifier":"0","reachState":"0","reachSutehaiIndex":"0","score":"24400","sutehais":[{"pai":"6p","pai_id":"14","red":"0"},{"pai":"W","pai_id":"29","red":"0"},{"pai":"E","pai_id":"27","red":"0"},{"pai":"P","pai_id":"31","red":"0"},{"pai":"1m","pai_id":"0","red":"0"},{"pai":"9m","pai_id":"8","red":"0"}],"tedashi":["1","0","1","1","1","1"],"tehais":[{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"}]},{"furo_sute":[{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"}],"furos":[],"identifier":"1","reachState":"1","reachSutehaiIndex":"1","score":"24600","sutehais":[{"pai":"6p","pai_id":"14","red":"0"},{"pai":"F","pai_id":"32","red":"0"},{"pai":"F","pai_id":"32","red":"0"},{"pai":"P","pai_id":"31","red":"0"},{"pai":"2p","pai_id":"10","red":"0"},{"pai":"F","pai_id":"32","red":"0"},{"pai":"3s","pai_id":"20","red":"0"}],"tedashi":["1","1","1","1","1","1","0"],"tehais":[{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"}]},{"furo_sute":[{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"}],"furos":[],"identifier":"2","reachState":"0","reachSutehaiIndex":"0","score":"24600","sutehais":[{"pai":"P","pai_id":"31","red":"0"},{"pai":"F","pai_id":"32","red":"0"},{"pai":"4p","pai_id":"12","red":"0"},{"pai":"2m","pai_id":"1","red":"0"},{"pai":"9m","pai_id":"8","red":"0"},{"pai":"S","pai_id":"28","red":"0"}],"tedashi":["1","1","1","1","1","1"],"tehais":[{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"}]},{"furo_sute":[{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"},{"pai":"?","pai_id":"-1","red":"0"}],"furos":[],"identifier":"3","reachState":"0","reachSutehaiIndex":"0","score":"26400","sutehais":[{"pai":"N","pai_id":"30","red":"0"},{"pai":"E","pai_id":"27","red":"0"},{"pai":"C","pai_id":"33","red":"0"},{"pai":"2m","pai_id":"1","red":"0"},{"pai":"S","pai_id":"28","red":"0"}],"tedashi":["1","1","1","1","1"],"tehais":[{"pai":"5mr","pai_id":"4","red":"1"},{"pai":"7m","pai_id":"6","red":"0"},{"pai":"3p","pai_id":"11","red":"0"},{"pai":"3p","pai_id":"11","red":"0"},{"pai":"5p","pai_id":"13","red":"0"},{"pai":"6p","pai_id":"14","red":"0"},{"pai":"1s","pai_id":"18","red":"0"},{"pai":"1s","pai_id":"18","red":"0"},{"pai":"5s","pai_id":"22","red":"0"},{"pai":"7s","pai_id":"24","red":"0"},{"pai":"8s","pai_id":"25","red":"0"},{"pai":"9s","pai_id":"26","red":"0"},{"pai":"6s","pai_id":"23","red":"0"}]}],"richiKyoutaku":"0"}
    
    )";
    
    string s = str;
    
    game.fromJson(s);
    
    const char str2[] = R"(
    {"type":"tsumo","actor":3,"pai":"7p","tsumogiri":true}
    
    )";
    string s2 = str2;
    oneUpdateStateAndResponedToAction(&game, s2);
}

void startClient(char *ip, int port) {
    
    printf("startClient start %s\n", ip);
    
    Socket s;
    MJAIGame game;
    printf("try connect ip = %s , port = %d\n", ip,
           port);
    
    int ret = s.start(ip, port, &game);
    
    printf("game end with result = %d\n", ret);
    
}

bool parse_host_and_port(string url, int *port, string *host) {
    cout << "Parsing " << url << endl;
    string mjsonp("mjsonp://");
    if (url.compare(0, mjsonp.size(), mjsonp) == 0) {
        unsigned int pos = url.find_first_of("/:", mjsonp.size());
        
        if (pos == string::npos) {
            // No port or path
            pos = url.size();
        }
        
        string _host(url.substr(mjsonp.size(), pos - mjsonp.size()));
        cout << "Host is " << _host << endl;
        *host = _host;
        
        if (pos < url.size() && url[pos] == ':') {
            // A port is provided
            unsigned int ppos = url.find_first_of("/", pos);
            if (ppos == string::npos) {
                // No path provided, assume port is rest of string
                ppos = url.size();
            }
            string _port(url.substr(pos + 1, ppos - pos - 1));
            if (_port.size() > 0) {
                // Port is provided and has length
                cout << "Port is " << _port << endl;
                *port = atoi(_port.c_str());
            }
        } else {
            cout << "No port number was provided" << endl;
            return false;
        }
        
    } else {
        cerr << "Not an mjsonp url" << endl;
        return false;
    }
    return true;
}

int player_number=0;
string rec_message="";

void handle_message(const std::string &message)
{
    //cout<<"handle_message"<<endl;
    //cout<<message<<endl;
    rec_message=message;
}

int game_main()
{
    
    MJAIGame game;
    
    using easywsclient::WebSocket;
    WebSocket::pointer ws = WebSocket::from_url("ws://www.logos.t.u-tokyo.ac.jp/mjai/");
    //assert(ws);
    
    while(ws->getReadyState()!=WebSocket::CLOSED)
    {
        while(1)
        {
            rec_message="";
            ws->poll();
            ws->dispatch(handle_message);
            if(rec_message!="")
            {
                cout<<rec_message<<endl;
                if(rec_message.find("error")!=string::npos)
                {
                    cout<<"error "<<rec_message<<endl;
                    ws->close();
                    return 60;
                }
                else if(rec_message.find("end_game")==string::npos)
                {
                    const char* buf_recv=rec_message.c_str();
                    string send_json=game.updateStateAndRespondToAction(buf_recv, false);
                    
                    printf("send = %s\n",send_json.c_str());
                    
                    ws->send(send_json);
                    rec_message="";
                }
                else
                {
                    ws->close();
                    return 1;
                }
                break;
            }
        }
    }
    ws->close();
    return 1;
}

int main(int argc, const char *argv[]) {
    
    // gem build mjai.gemspec
    // sudo gem install -l mjai-0.0.7.gem
#ifdef SOCKET
    socketTest();
#elif defined SOCKET_LOCAL
    socketTest();
#elif defined RELEASE_BUILD
    // printf("引数の総個数 = %d\n", argc);
    if (argc != 2 || argv[0] == NULL) {
        // printf("Usage : MJAIOccam [IP of MJAI] [PORT of MJAI] > /dev/null 2>&1");
        printf("incorrect argument. Usage(ex.) :\nmjai-occam "
               "mjsonp://localhost:11600/default > /dev/null 2>&1 : connect to specific server.\nmjai-occam -ws : connect to http://www.logos.t.u-tokyo.ac.jp/mjai/\n");
        
        return 0;
    }
    
    if(strcmp(argv[1], "-ws") == 0) {
        printf("try connect to http://www.logos.t.u-tokyo.ac.jp/mjai/\n");
        
        return game_main();
    }
    
    // parse mjsonp://localhost:11600/default
    int port = 11600;
    string host = "localhost";
    
    if (parse_host_and_port(argv[1], &port, &host)) {
        
        // TODO inet_ntoa
        if (host == "localhost")
            host = "127.0.0.1";
        
        const char *c = host.c_str();
        cout << "start client. host = " << c << endl;
        startClient((char *)c, port);
    } else {
        printf("incorrect argument. Usage(ex.) : mjai-occam "
               "mjsonp://localhost:11600/default > /dev/null 2>&1");
    }
    
#else
    readJson();
#endif
    
    //    // mjson読み込み
    //    MJAIGameForApp game;
    //    readFileAndParse("/Users/nibble1242/mjai/log/2016-08-04-201349.mjson",&game);
    
    // protocolTest();
    // MJAIGameForApp game;
    // game.countOfFileNum = 0;
    // parseForApp("/Users/nibble1242/Desktop/haifu/mjson_n1/",&game);
    // parseForApp("/Users/nibble1242/Desktop/haifu/mjson_n2/",&game);
    // parseForApp("/Users/nibble1242/Desktop/haifu/mjson_n3/",&game);
    // parseForApp("/Users/nibble1242/Desktop/haifu/mjson_n4/",&game);
    // parseForApp("/Users/nibble1242/Desktop/haifu/mjson_n5/",&game);
    // parseForApp("/Users/nibble1242/Desktop/haifu/mjson_n6/",&game);
    return 0;
}
