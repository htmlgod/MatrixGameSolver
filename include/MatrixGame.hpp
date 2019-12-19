#ifndef Simplex_hpp
#define Simplex_hpp

#include "tools.hpp"

enum func {MIN, MAX};
enum player {playerA, playerB};

class MatrixGame {
protected:
    player      _player;
    int         _iterationNumber = 1;
    int         _r;
    int         _k;
    int         _rows;
    int         _columns;
    int         _variablesNumber;
    int         _restrictionNumber;
    func        _funcTendention;
    
    double      _functionValue;
    int         _strategiesNumber;
    char        _variableChar;
    char        _minimalWinChanceChar;
    double       _minimalWinChanceValue;
    char        _strategyChar;
    char        _functionChar;
    char        _playerChar;
    
    std::vector< std::vector<float> > _A;
    std::vector<std::vector<float> > _table;
    std::vector<int>                 _variablesRow;
    std::vector<int>                 _variablesColumn;
public:
    MatrixGame(std::vector<std::vector<float> >& A,
            int                 variablesNumber,
            int               restrictionNumber,
            func                 funcTendention,
            player                       player
    );
    
    void makeSimplexTable();
    void makeSimplexIteration();
    void makeSimplexIterationBasis();
    
    int findSolvingColumn();
    int findSolvingRow();
    
    void findSolvingElementBasis();

    float getSolvingElement();
    
    bool isFRowHasPositiveValues();
    bool isSColumnHasNegativeValues();
    
    void findOptimalSolution();
    void printOptimalSolution();
    
    void findBasisSolution();
    void printBasisSolition();
    
    void resolve();
    
    void printSolvingElement();
    void print();
    
    void printOptimalStrategyAndSolution();
};

#endif
