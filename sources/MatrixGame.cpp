#include <MatrixGame.hpp>

MatrixGame::MatrixGame(std::vector<std::vector<float> >& A,
                 int                 variablesNumber,
                 int               restrictionNumber,
                 func                 funcTendention,
                 player                       player) {
    _r = 0;
    _k = 0;
    _player = player;
    _funcTendention = funcTendention;
    _variablesNumber = variablesNumber;
    _restrictionNumber = restrictionNumber;
    if (_player == playerB) {
        // var   number = num str A
        // restr number = num str b
        _A = A;
        // strats == B strats
        _strategiesNumber = _restrictionNumber;
        // columns = B strats + 1
        _columns = _restrictionNumber + 1;
        // rows = A strats + 1
        _rows    = _variablesNumber + 1;
        _table.resize(_rows);
        for (int i = 0; i < _rows; i++)
            _table[i].resize(_columns);
        // Ряд и строка с номерами переменных
        _variablesRow.resize(_restrictionNumber);
        _variablesColumn.resize(_variablesNumber);
        
        for(int i = 0; i < _restrictionNumber; i++)
            _variablesRow[i] = i + 1;
        for(int j = 0; j < _variablesNumber; j++)
            _variablesColumn[j] = j + 1 + _restrictionNumber;
    } else {
        _A = transpose(A);
        _strategiesNumber = _variablesNumber;
        _columns = _variablesNumber + 1;
        _rows    = _restrictionNumber + 1;
        _table.resize(_rows);
        for (int i = 0; i < _rows; i++)
            _table[i].resize(_columns);
        // Ряд и строка с номерами переменных
        _variablesRow.resize(_variablesNumber);
        _variablesColumn.resize(_restrictionNumber);
        
        for(int i = 0; i < _variablesNumber; i++)
            _variablesRow[i] = i + 1;
        for(int j = 0; j < _restrictionNumber; j++)
            _variablesColumn[j] = j + 1 + _variablesNumber;
    }
    if (_player == playerB) {
        _minimalWinChanceChar = 'h';
        _strategyChar = 'y';
        _variableChar = 'v';
        _functionChar = 'Z';
        _playerChar   = 'B';
    } else {
        _minimalWinChanceChar = 'g';
        _strategyChar = 'x';
        _variableChar = 'u';
        _functionChar = 'W';
        _playerChar   = 'A';
    }
}
// Составление таблицы
void MatrixGame::makeSimplexTable(){
    if (_player == playerB) {
        for(int i = 0; i < _rows - 1; i++)
            _table[i][0] = static_cast<float>(1);
        for(int i = 0; i < _rows - 1; i++){
            for(int j = 1; j < _columns; j++){
                _table[i][j] = _A[i][j-1];
            }
        }
        
        _table[_rows-1][0] = 0;
        for(int i = 1; i < _columns; i++)
            _table[_rows-1][i] = static_cast<float>(1);
    } else {
        for(int i = 0; i < _rows - 1; i++)
            _table[i][0] = static_cast<float>(-1);
        
        for(int i = 0; i < _rows - 1; i++){
            for(int j = 1; j < _columns; j++){
                _table[i][j] = -_A[i][j-1];
            }
        }
        
        _table[_rows-1][0] = 0;
        for(int i = 1; i < _columns; i++)
            _table[_rows-1][i] = static_cast<float>(-1);
    }
    
}
// Проверки для базисного и оптимального решений
bool MatrixGame::isFRowHasPositiveValues(){
    for(int i = 1; i < _columns; i++)
        if (_table[_rows-1][i] > 0) return true;
    return false;
}

bool MatrixGame::isSColumnHasNegativeValues() {
    for(int i = 0; i < _rows - 1; i++)
        if (_table[i][0] < 0) return true;
    return false;
}
// Получение разрешающего элемента
float MatrixGame::getSolvingElement() {
    return _table[_r][_k];
}
// Поиск разрешающего элемента для опорного решения
void MatrixGame::findSolvingElementBasis() {
    int k = -1;
    int r = 0;
    for (int i = 0; i < _rows - 1; i++) {
        if (_table[i][0] < 0) {
            r = i;
            for (int j = 1; j < _columns; j++) {
                if(_table[r][j] < 0) {
                    k = j;
                    break;
                }
            }
            break;
        }
    }
    try {
        if (k == -1)
            throw r;
    } catch (int k) {
        std::cerr<<"No negative values in row "<<r + 1<<", No solutions for LPP"<<std::endl;
        exit(0);
    }
    std::vector<float> buf(_rows);
    for(int i = 0; i < _rows - 1; i++){
        if(_table[i][k] != 0)
            buf[i] = _table[i][0] / _table[i][k];
        else buf[i] = -1;
        if(buf[r] > buf[i] && buf[i] > 0)
            r = i;
    }
    
     _k = k;
    _r = r;
}
// Итерация поиска опорного решения
void MatrixGame::makeSimplexIterationBasis() {
    std::vector<std::vector<float> > buffer_Table = _table;
    findSolvingElementBasis();
    int r = _r;
    int k = _k;
    printBasisSolition();
    printSolvingElement();
    float solvingElement = getSolvingElement();
    //new solving element in table
    buffer_Table[r][k] = 1 / solvingElement;
    //solving row
    for(int i = 0; i < _columns; i++) {
        if(i!=k)
            buffer_Table[r][i] = buffer_Table[r][i] / solvingElement;
        else
            continue;
    }
    //solving column
    for(int i = 0; i < _rows; i++) {
        if(i!=r)
            buffer_Table[i][k] = (buffer_Table[i][k] / solvingElement) * (-1);
        else
            continue;
    }
    //rest of table
    for(int i = 0; i < _rows; i++){
        for(int j = 0; j < _columns; j++){
            if(j!=k && i!=r){
                double buf = buffer_Table[i][j] - ( (_table[i][k]*_table[r][j])/solvingElement );
                buffer_Table[i][j] = buf;
            }
        }
    }
    _table = buffer_Table;
    
    std::swap(_variablesRow[k - 1] , _variablesColumn[r]);
    print();
}
// Поиск разрешающего элемента для оптимального решения
int MatrixGame::findSolvingColumn(){
    int k = 1;
    std::vector<float> buf;
    for(int i = 1; i < _columns; i++) {
        buf.push_back(_table[_rows-1][i]);
    }
    k = findMax(buf) + 1;
    _k = k;
    return k;
}
int MatrixGame::findSolvingRow() {
    int k = findSolvingColumn();
    int r = 0;
    std::vector<float> buf;
    for(int i = 0; i < _rows - 1; i++){
        if(_table[i][k] != 0)
            buf.push_back(_table[i][0] / _table[i][k]);
        else buf.push_back(-1);
    }
    
    try {
        r = findMinPositive(buf);
        if (r == -1)
            throw k;
    } catch (...) {
        std::cerr<<"No positive values in column "<< k <<", No optimal solution for LPP"<<std::endl;
        exit(0);
    }
    
    _r = r;
    
    return r;
}
// Итерация нахождения оптимального решения
void MatrixGame::makeSimplexIteration() {
    std::vector<std::vector<float> > buffer_Table = _table;
    int r = findSolvingRow();
    int k = findSolvingColumn();
    printBasisSolition();
    printSolvingElement();
    float solvingElement = getSolvingElement();
    //new solving element in table
    buffer_Table[r][k] = 1 / solvingElement;
    //solving row
    for(int i = 0; i < _columns; i++) {
        if(i!=k)
            buffer_Table[r][i] = buffer_Table[r][i] / solvingElement;
        else
            continue;
    }
    //solving column
    for(int i = 0; i < _rows; i++) {
        if(i!=r)
            buffer_Table[i][k] = (buffer_Table[i][k] / solvingElement) * (-1);
        else
            continue;
    }
    //rest of table
    for(int i = 0; i < _rows; i++){
        for(int j = 0; j < _columns; j++){
            if(j!=k && i!=r){
                double buf = buffer_Table[i][j] - ( (_table[i][k]*_table[r][j])/solvingElement );
                buffer_Table[i][j] = buf;
            }
        }
    }
    _table = buffer_Table;
    
    std::swap(_variablesRow[k - 1] , _variablesColumn[r]);
    print();
}
// Нахождение опорного решения
void MatrixGame::findBasisSolution() {
    while(isSColumnHasNegativeValues()){
        std::cout<<"######################################################################################################"<<std::endl;
        std::cout<<"Iteration #"<<_iterationNumber<<std::endl;
        makeSimplexIterationBasis();
        _iterationNumber++;
    }
}
// Нахождение оптимального решения
void MatrixGame::findOptimalSolution() {
    while(isFRowHasPositiveValues()){
        std::cout<<"######################################################################################################"<<std::endl;
        std::cout<<"Iteration #"<<_iterationNumber<<std::endl;
        makeSimplexIteration();
        _iterationNumber++;
    }
    if (_player == playerB)
        _functionValue = _table[_rows-1][0] * (-1.0);
    else
        _functionValue = _table[_rows-1][0];
    _minimalWinChanceValue = 1/_functionValue;
    
}
// Решение симплекс таблицы
void MatrixGame::resolve() {
    makeSimplexTable();
    print();
    if (isSColumnHasNegativeValues()) {
        findBasisSolution();
    }
    if (isFRowHasPositiveValues()) {
        findOptimalSolution();
    }
}
// Вывод информации
void MatrixGame::printSolvingElement(){
    std::cout<<"Solving Element = "<<getSolvingElement()<< "(row = " << _r + 1 << ", column = " << _k <<")"<< std::endl;
}
void MatrixGame::print(){
    std::cout<<std::endl;
    int variablesNumber;
    int restrictionNumber;
    if (_player == playerB) {
        variablesNumber = _restrictionNumber;
        restrictionNumber = _variablesNumber;
    }
    else {
        variablesNumber = _variablesNumber;
        restrictionNumber = _restrictionNumber;
    }
    std::cout<<"+--+"<<"-----------+";
    for (int i = 0; i < variablesNumber; i++)
        std::cout<<"-----------+";
    std::cout<<std::endl;
    
    std::cout<<"|  |"<<std::setw(12)<<"Si0|";
    for (int i = 0; i < variablesNumber; i++)
        std::cout<<std::setw(10)<<_variableChar<<_variablesRow[i]<<"|";
    std::cout<<std::endl;
    
    for (int i = 0; i < restrictionNumber; i++) {
        std::cout<<"+--+"<<"-----------+";
        for (int i = 0; i < variablesNumber; i++)
            std::cout<<"-----------+";
        std::cout<<std::endl;
        std::cout<<"|"<<_variableChar<<_variablesColumn[i]<<"|";
        for (int j = 0; j < _columns; j++)
            std::cout<<std::setw(11)<<std::setprecision(5)<<_table[i][j]<<"|";
        std::cout<<std::endl;
    }
    std::cout<<"+--+"<<"-----------+";
    for (int i = 0; i < variablesNumber; i++)
        std::cout<<"-----------+";
    std::cout<<std::endl;
    // func row
    std::cout<<"|"<<_functionChar<<" |";
    for (int i = 0; i < _columns; i++)
        std::cout<<std::setw(11)<<std::setprecision(5)<<_table[_rows-1][i]<<"|";
    std::cout<<std::endl;
    std::cout<<"+--+"<<"-----------+";
    for (int i = 0; i < variablesNumber; i++)
        std::cout<<"-----------+";
    std::cout<<std::endl;
}
void MatrixGame::printBasisSolition() {
    if (isSColumnHasNegativeValues() == false) {
        std::cout<<"(Basis solution) ";
        for(int i = 0; i < _columns-1; i++) {
            std::cout<<_variableChar<<_variablesRow[i]<<" = 0, ";
        }
        for(int i = 0; i < _rows-1; i++) {
            std::cout<<_variableChar<<_variablesColumn[i]<<" = "<<_table[i][0]<<", ";
        }
        if (_funcTendention == MAX)
            std::cout<< std::endl <<_functionChar<<"(max) = "<<-_table[_rows-1][0] << std::endl;
        else
            std::cout<< std::endl <<_functionChar<<"(min) = "<<_table[_rows-1][0] << std::endl;
    }
    else {
        std::cout<<"No basis solution"<<std::endl;
    }
}
void MatrixGame::printOptimalSolution(){
    if (isFRowHasPositiveValues() == false) {
        std::cout<<"(Optimal solution) ";
        for(int i = 0; i < _columns-1; i++) {
            std::cout<<_variableChar<<_variablesRow[i]<<" = 0, ";
        }
        for(int i = 0; i < _rows-1; i++) {
            std::cout<<_variableChar<<_variablesColumn[i]<<" = "<<_table[i][0]<<", ";
        }
        if (_funcTendention == MAX)
            std::cout<< std::endl <<_functionChar<<"(max) = "<<-_table[_rows-1][0] << std::endl;
        else
            std::cout<< std::endl <<_functionChar<<"(min) = "<<_table[_rows-1][0] << std::endl;
    }
    else {
        std::cout<<"No optimal solution"<<std::endl;
    }
}

void MatrixGame::printOptimalStrategyAndSolution() {
    if (isFRowHasPositiveValues() == false) {
        std::vector<float> optimalStrategies(_strategiesNumber, _minimalWinChanceValue);
        std::cout << "(Optimal solution) " << std::endl;
        for(int i = 0; i < _rows-1; i++) {
            if (_variablesColumn[i] <= _strategiesNumber) {
                std::cout<<_variableChar<<_variablesColumn[i]<<" = "<<_table[i][0]<<", ";
                optimalStrategies[_variablesColumn[i] - 1] *= _table[i][0];
            }
        }
        for(int i = 0; i < _columns-1; i++) {
            if (_variablesRow[i] <= _strategiesNumber) {
                std::cout<<_variableChar<<_variablesRow[i]<<" = 0, ";
                optimalStrategies[_variablesRow[i] - 1] *= 0;
            }
        }
        std::cout << std::endl <<_functionChar<<" = "<<_functionValue << std::endl;
        std::cout << "(Optimal strategies) " << std::endl;
        std::cout << _minimalWinChanceChar<<" = 1/"<< _functionChar <<" = "<< _minimalWinChanceValue << std::endl;
        for (int i = 0; i < _strategiesNumber; i++)
            std::cout << _strategyChar << i+1 << " = "<< _variableChar << i+1 <<" * "<<_minimalWinChanceChar <<" = " << optimalStrategies[i] << std::endl;
            
        std::cout << "Mixed strategy for player "<< _playerChar <<" is " << std::endl;
        std::cout << "(";
        for (int i = 0; i < _strategiesNumber; i++) {
            std::cout << optimalStrategies[i];
            if (i != _strategiesNumber - 1)
                std::cout  << "; ";
        }
        std::cout << ")" << std::endl;
    
        std::cout << "Check " << std::endl;
        double sum = 0;
        for (int i = 0; i < _strategiesNumber; i++) {
            std::cout << _variableChar << i+1;
            sum += optimalStrategies[i];
            if (i != _strategiesNumber - 1)
                std::cout <<" + ";
        }
        std::cout << " = " << sum << std::endl;
    }
}

