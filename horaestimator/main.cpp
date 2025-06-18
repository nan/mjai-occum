
#include "MJHoraEstimator.hpp"
#include "MJRonHoraEstimator.hpp"
#include <iostream>

static OpponentHoraEstimator o;

void printMetricsVector(std::vector<Metrics> met) {
    for (int i = 0; i < met.size(); i++) {
        
        cout << met[i].toString() << endl;
    }
}

void example1() {
    MJHoraEstimator estimator;
    
    std::vector<Metrics> ret = estimator.getHoraEstimation(
                                                           Pai::strToPais("2m 2m 3m 1p 2p 3p 4p 5p 6p 7p 8p 9p 2s 2s"), // 自分手牌
                                                           std::vector<Furo>{},  //自分フーロ
                                                           Pai::strToPais("8s"), // ドラ
                                                           Pai::strToPais(""),   // 自分河
                                                           70,                   //残り牌
                                                           0,                    //立直棒
                                                           0,                    //積み棒
                                                           Pai("E"),             // 場風
                                                           Pai("E"),             // 自風
                                                           Pai::strToPais(
                                                                          "2m 2m 3m 1p 2p 3p 4p 5p 6p 7p 8p 9p 2s 2s"), // 見えている牌
                                                           Pai("2s"),                                        // ツモ牌
                                                           false,
                                                           true, //立直
                                                           &o);
    printMetricsVector(ret);
}

void example2() {
    MJHoraEstimator estimator;
    
    std::vector<Metrics> ret = estimator.getHoraEstimation(
                                                           Pai::strToPais("7p 8p 9p 2s F"), // 自分手牌
                                                           std::vector<Furo>{
                                                               Furo("chi", Pai("1p"), 3, Pai::strToPais("2p 3p")),
                                                               Furo("pon", Pai("9s"), 3, Pai::strToPais("9s 9s")),
                                                               Furo("ankan", Pai("C"), 3, Pai::strToPais("C C C"))}, //自分フーロ
                                                           Pai::strToPais("5mr 3s"),                                 // ドラ
                                                           Pai::strToPais("1m"),                                     // 自分河
                                                           30,                                                       //残り牌
                                                           0,                                                        //立直棒
                                                           0,                                                        //積み棒
                                                           Pai("S"),                                                 // 場風
                                                           Pai("S"),                                                 // 自風
                                                           Pai::strToPais(""), // 見えている牌
                                                           Pai("2s"),          // ツモ牌
                                                           false,
                                                           true, //立直
                                                           &o);
    printMetricsVector(ret);
}

void example3() {
    // アンカン
    MJHoraEstimator estimator;
    
    std::vector<Metrics> ret = estimator.getHoraEstimation(
                                                           Pai::strToPais("1m 1m 1m 1m 2m 3m 5p 5p 6p 2s 2s 2s E S"), // 自分手牌
                                                           std::vector<Furo>{},           //自分フーロ
                                                           Pai::strToPais("8s"),          // ドラ
                                                           Pai::strToPais(""),            // 自分河
                                                           70,                            //残り牌
                                                           0,                             //立直棒
                                                           0,                             //積み棒
                                                           Pai("E"),                      // 場風
                                                           Pai("E"),                      // 自風
                                                           Pai::strToPais("1m 1m 1m 1m"), // 見えている牌
                                                           Pai("2s"),                     // ツモ牌
                                                           false, true,                   //立直
                                                           &o);
    printMetricsVector(ret);
}

void example4() {
    // カカン
    MJHoraEstimator estimator;
    
    std::vector<Metrics> ret = estimator.getHoraEstimation(
                                                           Pai::strToPais("2m 3m 4m 5m 5p 6p 1s 1s 5s 5s 6s"), // 自分手牌
                                                           std::vector<Furo>{
                                                               Furo("pon", Pai("E"), 3, Pai::strToPais("E E"))}, //自分フーロ
                                                           Pai::strToPais("5mr"),                                   // ドラ
                                                           Pai::strToPais("1m"),                                    // 自分河
                                                           30,                                                      //残り牌
                                                           0,                                                       //立直棒
                                                           0,                                                       //積み棒
                                                           Pai("E"),                                                // 場風
                                                           Pai("E"),                                                // 自風
                                                           Pai::strToPais(""), // 見えている牌
                                                           Pai("S"),           // ツモ牌
                                                           false,
                                                           true, //立直
                                                           &o);
    printMetricsVector(ret);
}

void example5() {
    // チー
    MJHoraEstimator estimator;
    
    std::vector<Metrics> ret = estimator.getHoraEstimationWhenFuro(
                                                                   Pai("8s"), // 鳴く牌
                                                                   3,         // 3は上家 2は対面
                                                                   Pai::strToPais("3m 3m 4m 5m 5m 6m 7m 7m 4p 5p 7p 6s 7s"), // 自分手牌
                                                                   std::vector<Furo>{},   //自分フーロ
                                                                   Pai::strToPais("5mr"), // ドラ
                                                                   Pai::strToPais("1m"),  // 自分河
                                                                   30,                    //残り牌
                                                                   0,                     //立直棒
                                                                   0,                     //積み棒
                                                                   Pai("E"),              // 場風
                                                                   Pai("E"),              // 自風
                                                                   Pai::strToPais(""),    // 見えている牌
                                                                   false,                 //立直
                                                                   &o);
    printMetricsVector(ret);
}

void example6() {
    // ポン
    MJHoraEstimator estimator;
    
    std::vector<Metrics> ret = estimator.getHoraEstimationWhenFuro(
                                                                   Pai("C"), // 鳴く牌
                                                                   2,        // 3は上家 2は対面
                                                                   Pai::strToPais("3m 4m 6m 7m 2p 4p 7p 8p 9p C C P P"), // 自分手牌
                                                                   std::vector<Furo>{},                                    //自分フーロ
                                                                   Pai::strToPais("E"),                                    // ドラ
                                                                   Pai::strToPais("1m"),                                   // 自分河
                                                                   30,                                                     //残り牌
                                                                   0,                                                      //立直棒
                                                                   0,                                                      //積み棒
                                                                   Pai("E"),                                               // 場風
                                                                   Pai("E"),                                               // 自風
                                                                   Pai::strToPais(""), // 見えている牌
                                                                   false,              //立直
                                                                   &o);
    printMetricsVector(ret);
}

void example7() {
    // http://critter.sakura.ne.jp/agari_keisan_setumei.html
    MJHoraEstimator estimator;
    
    std::vector<Metrics> ret = estimator.getHoraEstimation(
                                                           Pai::strToPais("2m 3m 4m 6m 8p 4s 6s 7s 8s 9s 9s E N F"), // 自分手牌
                                                           std::vector<Furo>{},  //自分フーロ
                                                           Pai::strToPais("1s"), // ドラ
                                                           Pai::strToPais("1m"), // 自分河
                                                           17 * 4,               //残り牌
                                                           0,                    //立直棒
                                                           0,                    //積み棒
                                                           Pai("E"),             // 場風
                                                           Pai("E"),             // 自風
                                                           Pai::strToPais(""), // Pai::strToPais("2m 3m 4m 6m 8p 4s 6s 7s 8s 9s 9s E
                                                           // N F"), // 見えている牌
                                                           Pai("F"),           // ツモ牌
                                                           false, true,        //立直
                                                           &o);
    
    printMetricsVector(ret);
}

void example8() {
    // https://github.com/gimite/mjai/commit/b482e2860f69c62fdaf3578cf724b067d1a1fd54
    
    ShantenAnalysis analysis =
    ShantenAnalysis(Pai::strToPais("2p 2p 2p 2p 3p 3p 3p 3p 4p 4p"), 1);
    cout << "shanten = " << analysis.shanten << endl;
}

void example9() {
    // ポン
    MJHoraEstimator estimator;
    
    std::vector<Metrics> ret = estimator.getHoraEstimationWhenFuro(
                                                                   Pai("9s"), // 鳴く牌
                                                                   3,         // 3は上家 2は対面
                                                                   Pai::strToPais("4m 5m 8m 1p 1p 5p 7p 8p 7s 7s 8s E C"), // 自分手牌
                                                                   std::vector<Furo>{},                                    //自分フーロ
                                                                   Pai::strToPais("2s"),                                   // ドラ
                                                                   Pai::strToPais("1m"),                                   // 自分河
                                                                   50,                                                     //残り牌
                                                                   0,                                                      //立直棒
                                                                   0,                                                      //積み棒
                                                                   Pai("E"),                                               // 場風
                                                                   Pai("E"),                                               // 自風
                                                                   Pai::strToPais(""), // 見えている牌
                                                                   false,              //立直
                                                                   &o);
    printMetricsVector(ret);
}

void example10() {
    MJHoraEstimator estimator;
    
    std::vector<Metrics> ret = estimator.getHoraEstimation(
                                                           
                                                           Pai::strToPais("2m 2m 4m 1p 1p 2p 4p 5p 2s 2s 2s 7s 8s 9s"), // 自分手牌
                                                           // Pai::strToPais("1m 4m 7m 2p 5p 7p 3s 6s 9s E E N F C"), // 自分手牌
                                                           std::vector<Furo>{},  //自分フーロ
                                                           Pai::strToPais("1s"), // ドラ
                                                           Pai::strToPais("1m"), // 自分河
                                                           50,                   //残り牌
                                                           0,                    //立直棒
                                                           0,                    //積み棒
                                                           Pai("E"),             // 場風
                                                           Pai("E"),             // 自風
                                                           Pai::strToPais("4m 5m 6m 7m 8m 8m 8m 2p 3p 6s 7s 7s C C"), // 見えている牌
                                                           Pai("8p"),                                                 // ツモ牌
                                                           false, true,                                               //立直
                                                           &o);
    
    printMetricsVector(ret);
}

void example11() {
    MJHoraEstimator estimator;
    
    // これ4m必須だけど7s切ってるからなおす必要がある
    std::vector<Metrics> ret = estimator.getHoraEstimation(
                                                           Pai::strToPais("4m 5m 6m 7m 8m 8m 8m 2p 3p 6s 7s 7s C C"), // 自分手牌
                                                           std::vector<Furo>{},  //自分フーロ
                                                           Pai::strToPais("1s"), // ドラ
                                                           Pai::strToPais("1m"), // 自分河
                                                           50,                   //残り牌
                                                           0,                    //立直棒
                                                           0,                    //積み棒
                                                           Pai("E"),             // 場風
                                                           Pai("E"),             // 自風
                                                           Pai::strToPais("4m 5m 6m 7m 8m 8m 8m 2p 3p 6s 7s 7s C C"), // 見えている牌
                                                           Pai("8p"),                                                 // ツモ牌
                                                           false, true,                                               //立直
                                                           &o);
    
    printMetricsVector(ret);
}

void example12() {
    MJHoraEstimator estimator;
    
    // これ4m必須だけど7s切ってるからなおす必要がある
    std::vector<Metrics> ret = estimator.getHoraEstimation(
                                                           Pai::strToPais("1m 1m 1m 4m 6m 2p 3p 4p 4p 6p 8p 3s 3s 4s"), // 自分手牌
                                                           std::vector<Furo>{},  //自分フーロ
                                                           Pai::strToPais("9s"), // ドラ
                                                           Pai::strToPais(""),   // 自分河
                                                           30,                   //残り牌
                                                           0,                    //立直棒
                                                           0,                    //積み棒
                                                           Pai("E"),             // 場風
                                                           Pai("E"),             // 自風
                                                           Pai::strToPais(
                                                                          ""), // 見えている牌
                                                           Pai("8p"),                                       // ツモ牌
                                                           false,
                                                           true, //立直
                                                           &o);
    
    printMetricsVector(ret);
}

void example13() {
    // チー
    MJHoraEstimator estimator;
    
    std::vector<Metrics> ret = estimator.getHoraEstimationWhenFuro(
                                                                   Pai("F"),                              // 鳴く牌
                                                                   3,                                      // 3は上家 2は対面
                                                                   Pai::strToPais("8m 8m 8m 1p 2p 3p 2s 2s 5s 7s 9s F F"), // 自分手牌
                                                                   std::vector<Furo>{
                                                                       //          Furo("pon", Pai("1s"), 3, Pai::strToPais("1s 1s")),
                                                                       //          Furo("pon", Pai("F"), 3, Pai::strToPais("F F"))
                                                                   }, //自分フーロ
                                                                   Pai::strToPais("2s"),                                 // ドラ
                                                                   Pai::strToPais(""),                                   // 自分河
                                                                   50,                                                   //残り牌
                                                                   0,                                                    //立直棒
                                                                   0,                                                    //積み棒
                                                                   Pai("E"),                                             // 場風
                                                                   Pai("E"),                                             // 自風
                                                                   Pai::strToPais(""), // 見えている牌
                                                                   false,              //立直
                                                                   &o);
    printMetricsVector(ret);
}

void example14() {
    MJHoraEstimator estimator;
    
    // これ4m必須だけど7s切ってるからなおす必要がある
    std::vector<Metrics> ret = estimator.getHoraEstimation(
                                                           Pai::strToPais("3m 4m 4m 8m 9m 3p 4p 6p 1s 2s 3s 4s 5s 6s"), // 自分手牌
                                                           std::vector<Furo>{},  //自分フーロ
                                                           Pai::strToPais("9s"), // ドラ
                                                           Pai::strToPais(""),   // 自分河
                                                           30,                   //残り牌
                                                           0,                    //立直棒
                                                           0,                    //積み棒
                                                           Pai("E"),             // 場風
                                                           Pai("E"),             // 自風
                                                           Pai::strToPais(
                                                                          ""), // 見えている牌
                                                           Pai("8p"),                                       // ツモ牌
                                                           false,
                                                           true, //立直
                                                           &o);
    
    printMetricsVector(ret);
}

void example15() {
    MJHoraEstimator estimator;
    
    // これ4m必須だけど7s切ってるからなおす必要がある
    std::vector<Metrics> ret = estimator.getHoraEstimation(
                                                           Pai::strToPais("1p 2p 3p 4p 4p 5p 8p 9p 9p 3s 5sr 5s 6s"), // 自分手牌
                                                           std::vector<Furo>{},  //自分フーロ
                                                           Pai::strToPais("9s"), // ドラ
                                                           Pai::strToPais(""),   // 自分河
                                                           30,                   //残り牌
                                                           0,                    //立直棒
                                                           0,                    //積み棒
                                                           Pai("E"),             // 場風
                                                           Pai("E"),             // 自風
                                                           Pai::strToPais(
                                                                          ""), // 見えている牌
                                                           Pai("8p"),                                       // ツモ牌
                                                           false,
                                                           true, //立直
                                                           &o);
    
    printMetricsVector(ret);
}

void example16() {
    MJHoraEstimator estimator;
    
    std::vector<Metrics> ret = estimator.getHoraEstimationWhenFuro(
                                                                   Pai("9p"),                              // 鳴く牌
                                                                   3,                                      // 3は上家 2は対面
                                                                   Pai::strToPais("3m 3m 8m 5p 5p 6p 9p 9p 3s S N P C"), // 自分手牌
                                                                   std::vector<Furo>{
                                                                       //          Furo("pon", Pai("1s"), 3, Pai::strToPais("1s 1s")),
                                                                       //          Furo("pon", Pai("F"), 3, Pai::strToPais("F F"))
                                                                   }, //自分フーロ
                                                                   Pai::strToPais("2s"),                                 // ドラ
                                                                   Pai::strToPais(""),                                   // 自分河
                                                                   60,                                                   //残り牌
                                                                   0,                                                    //立直棒
                                                                   0,                                                    //積み棒
                                                                   Pai("E"),                                             // 場風
                                                                   Pai("N"),                                             // 自風
                                                                   Pai::strToPais(""), // 見えている牌
                                                                   false,              //立直
                                                                   &o);
    printMetricsVector(ret);
}

void example17() {
    MJHoraEstimator estimator;
    
    // これ4m必須だけど7s切ってるからなおす必要がある
    std::vector<Metrics> ret = estimator.getHoraEstimation(
                                                           Pai::strToPais("2m 3m 4m 4m 5m 6m 8m 8m 1p 1p 2p 3p 5s 7p"), // 自分手牌
                                                           std::vector<Furo>{},  //自分フーロ
                                                           Pai::strToPais("N"), // ドラ
                                                           Pai::strToPais(""),   // 自分河
                                                           20,                   //残り牌
                                                           0,                    //立直棒
                                                           0,                    //積み棒
                                                           Pai("E"),             // 場風
                                                           Pai("E"),             // 自風
                                                           Pai::strToPais(
                                                                          ""), // 見えている牌
                                                           Pai("8p"),                                       // ツモ牌
                                                           false,
                                                           true, //立直
                                                           &o);
    
    printMetricsVector(ret);
}

int main(int argc, const char * argv[]) {
    
    //    example1();
    //    printf("----------------------------\n");
    //    example2();
    //    printf("----------------------------\n");
    //    example3();
    //    printf("----------------------------\n");
    //    example4();
    //    printf("----------------------------\n");
    //    example5();
    //    printf("----------------------------\n");
    //    example6();
    //    printf("----------------------------\n");
    //    example7();
    //    printf("----------------------------\n");
    //    example8();
    //    printf("----------------------------\n");
    //    example9();
    //    printf("----------------------------\n");
    //    example10();
    //    printf("----------------------------\n");
    //    example12();
    //    printf("----------------------------\n");
    //    example12();
    //    printf("----------------------------\n");
    //    example13();
    //example14();
//    printf("----------------------------\n");
//    //example15();
//    printf("----------------------------\n");
//    example16();
//    printf("----------------------------\n");
    example17();
    printf("----------------------------\n");
    return 0;
}
