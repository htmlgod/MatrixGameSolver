#include <MatrixGame.hpp>
#include <tools.hpp>

int main(int argc, const char * argv[]) {
    int numStratA, numStratB;
    
    //printTask();

    std::cout<<"Enter number of strategies for player A: "; std::cin >> numStratA;
    std::cout<<"Enter number of strategies for player B: "; std::cin >> numStratB;
    std::vector< std::vector<float> > gameMatrix;
    
    std::cout<<"Enter game matrix: ";
    fill(gameMatrix,numStratA,numStratB);
    
    std::cout << R"(
#######################################################################
Player A
#######################################################################
    )"<<std::endl;
    MatrixGame player_A(gameMatrix, numStratA, numStratB, MIN, playerA);
    player_A.resolve();
    player_A.printOptimalStrategyAndSolution();
    std::cout << R"(
#######################################################################
Player B
#######################################################################
    )"<<std::endl;
    MatrixGame player_B(gameMatrix, numStratA, numStratB, MAX, playerB);
    player_B.resolve();
    player_B.printOptimalStrategyAndSolution();
    return 0;
}
// 4 5 16 17 8 15 17 0 3 19 8 2 13 19 7 15 9 11 15 2 16 2
// 10 VAR 4 5 12 6 3 17 9 0 5 16 0 15 16 19 12 18 11 19 12 7 2 13
