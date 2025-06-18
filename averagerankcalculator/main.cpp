
#include <iostream>

#include "AverageRankCalculator.hpp"

int main(int argc, const char *argv[]) {
    
    AverageRankCalculator calculator;
    
    // 東発なのでほぼ2.5のはず
    std::cout << calculator.calc(0, 25000, 25000, 25000, 25000, 7) << std::endl;
    
    // ラス親なので2.5より少し低いはず
    std::cout << calculator.calc(3, 25000, 25000, 25000, 25000, 7) << std::endl;
    
    // ゲーム終了時なので2位のはず
    std::cout << calculator.calc(0, 30000, 25000, 33000, 25000, -1) << std::endl;
    
    // オーラスでダンラスなのでほぼ4のはず
    std::cout << calculator.calc(0, 5000, 30000, 25000, 25000, 1) << std::endl;
    
    std::cout << calculator.calc(0, 26000, 24000, 25000, 25000, 1) << std::endl;
    std::cout << calculator.calc(0, 27000, 23000, 25000, 25000, 12) << std::endl;
    
    std::cout << calculator.calc(0, 22500, 17700, 26500, 32300, 4) << std::endl;
    
    return 0;
}
