
#include "OpponentHoraEstimator.hpp"

void setBKUC_PlayerStruct(player tatya[]);
void setBKUC_FieldStruct(field *game_field);

int main(int argc, const char * argv[]) {
    
    // 公開されている爆打(R1681時代)の相手テンパイ率予想ルーチン
    // boostが必要なので適当に別途インストールして下さい
    
    BKUC1681 bkuc; // 爆打本体
    player _player[4]; // 爆打内部のプレイヤデータ構造体
    field _field;      // 爆打内部の卓データ構造体
    
    bkuc.init_engine(); // must call when start for using bakuuchi
    
    setBKUC_PlayerStruct(_player);
    setBKUC_FieldStruct(&_field);
    
    // 計算クラス
    OpponentHoraEstimator opponentHoraEstimator;
    
    // 計算
    opponentHoraEstimator.estimate(
                                 &bkuc,
                                 _player,
                                 &_field);
    
    // 出力
    std::cout <<  "0 : 下家 ";
    std::cout <<  "1 : 対面 ";
    std::cout <<  "2 : 上家 " << std::endl;
    for (auto i = 0; i < 3; i++) {
        for (auto j = 0; j < 34; j++) {
            std::cout
            << i + 1
            << "の" << j << "の放銃率 = "
            << opponentHoraEstimator.hojuProb[i][j]
            << "　の放銃点 = "
            << opponentHoraEstimator.hojuTen[i][j]
            << std::endl;
        }
    }
    
    return 0;
}



void setBKUC_PlayerStruct(player tatya[]) {
    // player info
    // 0 = 自分 1 = 下家 ...
    for (int i = 0; i < 4; i++) {
        
        tatya[i].fu_ro_suu = 0; // num of furo
        memset(tatya[i].fu_ro, -1, sizeof(int) * 4);
        
        //        副露した牌は以下に対応する
        //        チー　数字の一番小さな値に対応
        //        例:マンズ345のチー→2,ソーズ789のチー→24
        //        ポン　ポンした数字に34を足す   例:東のポン→61
        //        明槓　数字に+68を足す
        //        暗槓　数字に102を足す
        //        フーロなしなら-1
        //
        // 例
        // tatya[0].fu_ro[0] = 0; // 自分の1フーロ目が1mチー
        // tatya[0].fu_ro[0] = 0 + 34; // 自分の1フーロ目が1mポン
        // tatya[0].fu_ro[0] = 0 + 68; // 自分の1フーロ目が1m明槓
        // tatya[0].fu_ro[0] = 0 + 102; // 自分の1フーロ目が1mアンカン
        
        tatya[i].aka_fu_ro = 0; // 赤フーロの数
        tatya[i].aka_moti = 0;  // 0 none, 2 5mr, 4 5pr, 6 5sr
        tatya[i].ri_ti_flag = false; // reach or not
        tatya[i].ri_ti_sute = -1; // リーチした時の牌（してないなら−１）
        
        // int[4]  鳴き時の捨て牌 鳴いてないなら-1
        for (int j = 0; j < 4; j++) {
            tatya[i].fu_ro_sute[j] = -1;
        }
        
        tatya[i].ippatu = 0; // 一発の権利あり
        tatya[i].tenho = 0;  // 天和の権利あり
        tatya[i].tensuu = 25000; // point
        tatya[i].sutehai = vector<int>{}; // vector<int> hainum
        
        
        // 捨て牌セット
        for (int j = 0; j < 34; j++) {
            tatya[i].tegawari[j] = 3;// 謎 最初は3で打牌したら0 ?
            tatya[i].genbutu[j] = 0; // 現物なら1
        }
        tatya[i].tedasi.push_back(1); // 手出しの動的配列 手出しなら1
        tatya[i].sutehai.push_back(3); // 捨て牌のの動的配列 牌番号
        
        memset(tatya[i].hai, 0, sizeof(int) * 34);
        
        tatya[i].jifuu = 27 + i; // jikaze
        tatya[i].fold_flag = 0; // 内部で使われてるっぽい 降りフラグ？
    }
    // 自分の手牌
    tatya[0].hai[0] = 1;
    tatya[0].hai[1] = 1;
    tatya[0].hai[2] = 1;
    tatya[0].hai[3] = 1;
    tatya[0].hai[4] = 1;
    tatya[0].hai[5] = 1;
    tatya[0].hai[6] = 1;
    tatya[0].hai[7] = 1;
    tatya[0].hai[8] = 1;
    tatya[0].hai[18] = 3;
    tatya[0].hai[22] = 1;
    tatya[0].hai[23] = 1;
}


void setBKUC_FieldStruct(field *game_field) {
    game_field->bafuu = 27; // 27 = E, 28 = S
    game_field->kyoku = 1; // kyoku num
    game_field->game_long = 8;  // 最大局数(8は東南)
    game_field->ri_ti_bou = 0; // reach bou
    game_field->honba = 0;  // honba
    memset(game_field->dora, -1, sizeof(int) * 5);
    
    // dora, int[5], 牌番号(0-33) それ以外は-1
    game_field->dora[0] = 3;
    game_field->dora[1] = -1;
    game_field->dora[2] = -1;
    game_field->dora[3] = -1;
    game_field->dora[4] = -1;
    
    game_field->dora_num = -1; // ?
    game_field->haitei = 0;    // haitei flag
    // 0:none 1:some player act ankan 2:some player act minkan maybe..
    game_field->kan_flag = 0;
    game_field->dora_basho = 0; // ?
    game_field->nokori_maisuu = 40; // left of yama
    game_field->mieta_aka_hai = 0; // visible aka in kawa
    
    // 見えている牌の数
    memset(game_field->sutehai_sarasi, 0, sizeof(int) * 37);
    for (int i = 0; i < 34; i++) {
        game_field->sutehai_sarasi[i] = 1;
    }
}
