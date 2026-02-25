// — Após a classe estiver 100% correta, derive uma classe para o Lig4 tradicional (7,6,4) e altere os comportamentos do código necessários para que ela funcione como o jogo original preconiza, ou seja, o primeiro
// a criar uma sequência de quatro fichas da sua cor vence. Utilize para isso o método que calcula o resultado, redefinindo-o para retornar sempre o resultado atual por meio de um enum {Vitória da Cor1, Vitória da Cor2, Empate, Continua} e pare se o resultado não for Continua.
// — On topo disso, implemente dois comportamentos para os dois jogadores: o primeiro ótimo que sempre “deve” ganhar e o segundo totalmente aleatório.
// — Faça um loop infinito de jogos e somente termine se o jogador aleatório vencer uma partida (o que não deve ocorrer).

#include <iostream>                     // para operações de entrada e saída
using std::cout;
using std::cin;
using std::endl;

#include <random>                       // para preencher numeros aleatórios
#include <cstdlib>                      // para exit()
#include <iomanip>                      // para formatação de saída
using std::setw;

#include <algorithm>
#include <limits>

enum tab { vazio = 0, jogador_1 = 1, jogador_2 = 20 };
enum situation { valida = 0, invalida };
enum Resultado {VitoriaCor1, VitoriaCor2, Empate, Continua};
struct Placar{
    size_t player1, player2;
};

class Lig4 {
    protected:
        int m;                              // colunas
        int n;                              // linhas
        int k;                              // sequência para vencer (fixo em 4)
        int end;                            // número máximo de jogadas
        int counter;                        // jogadas realizadas
        int jogador;                        // vez do jogador
        int **field;                        // tabuleiro
        int player1, player2;               // pontuação de cada player
        bool endGame;                       // flag que simboliza o final do jogo
        Placar placar;                      // placar do jogo, não é alterado pelo método reiniciarTabuleiro
        

    public:
        // Construtor da classe Lig4
        Lig4(int n, int m, int k) : m(m), n(n), k(k), end(m*n), counter(0), jogador(jogador_1), player1(0), player2(0), endGame(false) {
            placar.player1 = placar.player2 = 0;            
            field = new int*[n];
            for(int i = 0; i < n; i++) {
                field[i] = new int[m];
                for(int j = 0; j < m; j++) {
                    field[i][j] = vazio;    // inicializa todas as posições como vazias
                }
            }
        }

        // Destrutor da classe Lig4
        ~Lig4() {
            for(int i = 0; i < n; i++) {
                delete[] field[i];
            }
            delete[] field;
        }

        // Função para exibir o tabuleiro
        void display() {
            for(int i = 0; i < n; i++) {
                for(int j = 0; j < m; j++) {
                    if(field[i][j] == vazio) cout << ".";
                    else if(field[i][j] == jogador_1) cout << "\033[33mX\033[0m";  // Amarelo
                    else if(field[i][j] == jogador_2) cout << "\033[31mO\033[0m";  // Vermelho
                }
                cout << endl;
            }
            for(int i = 0; i < m; i++) cout << (i+1);
            cout << endl;
        }

        // Função para alternar a vez do jogador
        void alternarVez() {
            if(endGame) return;
            if(jogador == jogador_1) jogador = jogador_2;
            else jogador = jogador_1;
        }

        // Função para verificar a validade da jogada
        situation validade(int linha, int coluna) {
            if(coluna < 0 || coluna >= m) return invalida;
            if(linha < 0 || linha >= n) return invalida;
            if(field[linha][coluna] != vazio) return invalida;
            return valida;
        }

        // Função para realizar uma jogada
        void jogar(int coluna) {
            if(endGame) return;
            coluna--;  // ajusta a coluna para índice baseado em zero

            if(counter == end) {
                endGame = true;
                cout << "Jogo encerrado!" << endl;
                exit(1);
            }

            if(field[0][coluna] != vazio) {
                // cout << "Jogada invalida." << endl << endl;
            } else {
                int i = n - 1;
                while(i >= 0 && field[i][coluna] != vazio) {
                    i--;
                }

                if(i >= 0 && validade(i, coluna) == valida) {
                    field[i][coluna] = jogador;
                    counter++;
                    alternarVez();
                } else {
                    // cout << "Jogada invalida." << endl << endl;
                }
            }
        }

        // Função para exibir o resultado (atualmente apenas exibe o tabuleiro)
        void resultado() {
            if(!endGame) return; 
            // Verifica linhas horizontais
            for (int i = 0; i < n; i++) {
                for (int j = 0; j <= m - 4; j++) {
                    if (field[i][j] == jogador_1 && field[i][j+1] == jogador_1 && field[i][j+2] == jogador_1 && field[i][j+3] == jogador_1) {
                        player1++;
                    } else if (field[i][j] == jogador_2 && field[i][j+1] == jogador_2 && field[i][j+2] == jogador_2 && field[i][j+3] == jogador_2) {
                        player2++;
                    }
                }
            }

            // Verifica colunas verticais
            for (int j = 0; j < m; j++) {
                for (int i = 0; i <= n - 4; i++) {
                    if (field[i][j] == jogador_1 && field[i+1][j] == jogador_1 && field[i+2][j] == jogador_1 && field[i+3][j] == jogador_1) {
                        player1++;
                    } else if (field[i][j] == jogador_2 && field[i+1][j] == jogador_2 && field[i+2][j] == jogador_2 && field[i+3][j] == jogador_2) {
                        player2++;
                    }
                }
            }

            // Verifica diagonais descendentes
            for (int i = 0; i <= n - 4; i++) {
                for (int j = 0; j <= m - 4; j++) {
                    if (field[i][j] == jogador_1 && field[i+1][j+1] == jogador_1 && field[i+2][j+2] == jogador_1 && field[i+3][j+3] == jogador_1) {
                        player1++;
                    } else if (field[i][j] == jogador_2 && field[i+1][j+1] == jogador_2 && field[i+2][j+2] == jogador_2 && field[i+3][j+3] == jogador_2) {
                        player2++;
                    }
                }
            }

            // Verifica diagonais ascendentes
            for (int i = 3; i < n; i++) {
                for (int j = 0; j <= m - 4; j++) {
                    if (field[i][j] == jogador_1 && field[i-1][j+1] == jogador_1 && field[i-2][j+2] == jogador_1 && field[i-3][j+3] == jogador_1) {
                        player1++;
                    } else if (field[i][j] == jogador_2 && field[i-1][j+1] == jogador_2 && field[i-2][j+2] == jogador_2 && field[i-3][j+3] == jogador_2) {
                        player2++;
                    }
                }
            }
            if(player1 > player2) placar.player1++;
            else if(player1 < player2) placar.player2++;
            system("cls");
            display();
            getPoints();            
            cout << "Jogo encerrado!" << endl;
        }

        void reiniciarTabuleiro() {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    field[i][j] = vazio;
                }
            }
            jogador = jogador_1; // Reseta o jogador
            counter = 0; // Zera o contador de jogadas
            endGame = false; // Reseta a flag de fim de jogo
        }

        void bot() {
            reiniciarTabuleiro(); // Reset
            // Configuração do gerador de números aleatórios
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, m);

            // Preencher o tabuleiro alternando jogadas até que todos os campos estejam preenchidos
            while (counter < end) {
                int coluna = dis(gen);  // gera uma coluna aleatória entre 1 e número de colunas
                jogar(coluna);
            }
            resultado();
            getPoints();
        }

        int getP1(){
            return player1;
        }

        int getP2(){
            return player2;
        }

        int getCounter(){
            return counter;
        }

        int getEnd(){
            return end;
        }

        int getJogador(){
            return jogador;
        }

        int getM(){
            return m;
        }

        void getPoints(){
            cout << "Jogador 1: " << placar.player1 << " pontos." << endl;
            cout << "Jogador 2: " << placar.player2 << " pontos." << endl;
        }

        void resetPoints(){
            placar.player1 = 0;
            placar.player2 = 0;
        }
};

class Lig4Tradicional : public Lig4
{
    public:
        Lig4Tradicional() : Lig4(7, 6, 4) {}
        //enum para método resultado
        enum Resultado { VitoriaCor1, VitoriaCor2, Empate, Continua};
        Resultado resultado() {
            // Verifica linhas horizontais
            for (int i = 0; i < n; i++) {
                for (int j = 0; j <= m - 4; j++) {
                    if (field[i][j] == jogador_1 && field[i][j+1] == jogador_1 && field[i][j+2] == jogador_1 && field[i][j+3] == jogador_1) {
                        placar.player1++;
                        endGame = true;
                        system("cls");
                        display();
                        getPoints();            
                        cout << "Jogo encerrado!" << endl;
                        return VitoriaCor1;
                    } else if (field[i][j] == jogador_2 && field[i][j+1] == jogador_2 && field[i][j+2] == jogador_2 && field[i][j+3] == jogador_2) {
                        placar.player2++;
                        endGame = true;
                        system("cls");
                        display();
                        getPoints();            
                        cout << "Jogo encerrado!" << endl;
                        return VitoriaCor2;
                    }
                }
            }

            // Verifica colunas verticais
            for (int j = 0; j < m; j++) {
                for (int i = 0; i <= n - 4; i++) {
                    if (field[i][j] == jogador_1 && field[i+1][j] == jogador_1 && field[i+2][j] == jogador_1 && field[i+3][j] == jogador_1) {
                        placar.player1++;
                        endGame = true;
                        system("cls");
                        display();
                        getPoints();            
                        cout << "Jogo encerrado!" << endl;
                        return VitoriaCor1;
                    } else if (field[i][j] == jogador_2 && field[i+1][j] == jogador_2 && field[i+2][j] == jogador_2 && field[i+3][j] == jogador_2) {
                        placar.player2++;
                        endGame = true;
                        system("cls");
                        display();
                        getPoints();            
                        cout << "Jogo encerrado!" << endl;
                        return VitoriaCor2;
                    }
                }
            }

            // Verifica diagonais descendentes
            for (int i = 0; i <= n - 4; i++) {
                for (int j = 0; j <= m - 4; j++) {
                    if (field[i][j] == jogador_1 && field[i+1][j+1] == jogador_1 && field[i+2][j+2] == jogador_1 && field[i+3][j+3] == jogador_1) {
                        placar.player1++;
                        endGame = true;
                        system("cls");
                        display();
                        getPoints();            
                        cout << "Jogo encerrado!" << endl;
                        return VitoriaCor1;
                    } else if (field[i][j] == jogador_2 && field[i+1][j+1] == jogador_2 && field[i+2][j+2] == jogador_2 && field[i+3][j+3] == jogador_2) {
                        placar.player2++;
                        endGame = true;
                        system("cls");
                        display();
                        getPoints();            
                        cout << "Jogo encerrado!" << endl;
                        return VitoriaCor2;
                    }
                }
            }

            // Verifica diagonais ascendentes
            for (int i = 3; i < n; i++) {
                for (int j = 0; j <= m - 4; j++) {
                    if (field[i][j] == jogador_1 && field[i-1][j+1] == jogador_1 && field[i-2][j+2] == jogador_1 && field[i-3][j+3] == jogador_1) {
                        placar.player1++;
                        endGame = true;
                        return VitoriaCor1;
                    } else if (field[i][j] == jogador_2 && field[i-1][j+1] == jogador_2 && field[i-2][j+2] == jogador_2 && field[i-3][j+3] == jogador_2) {
                        placar.player2++;
                        endGame = true;
                        return VitoriaCor2;
                    }
                }
            }
            if(counter == end) {
                endGame = true;
                system("cls");
                display();
                getPoints();
                cout << "Jogo empatado!" << endl;
                return Empate;
            }
            return Continua;
        }
        //algoritmo
        int evaluate() {
            int score = 0;

            // Avaliar linhas horizontais
            for (int i = 0; i < n; i++) {
                for (int j = 0; j <= m - 4; j++) {
                    int lineScore = 0;
                    for (int k = 0; k < 4; k++) {
                        if (field[i][j + k] == jogador_1) lineScore += 1;
                        if (field[i][j + k] == jogador_2) lineScore -= 1;
                    }
                    if (lineScore == 4) score += 100;
                    else if (lineScore == -4) score -= 100;
                    else if (lineScore == 3 && k == vazio) score += 50;  // Priorizar uma vitória iminente
                    else if (lineScore == -3 && k == vazio) score -= 50; // Bloquear uma derrota iminente
                }
            }

            // Avaliar colunas verticais
            for (int j = 0; j < m; j++) {
                for (int i = 0; i <= n - 4; i++) {
                    int lineScore = 0;
                    for (int k = 0; k < 4; k++) {
                        if (field[i + k][j] == jogador_1) lineScore += 1;
                        if (field[i + k][j] == jogador_2) lineScore -= 1;
                    }
                    if (lineScore == 4) score += 100;
                    else if (lineScore == -4) score -= 100;
                    else if (lineScore == 3 && k == vazio) score += 50;
                    else if (lineScore == -3 && k == vazio) score -= 50;
                }
            }

            // Avaliar diagonais descendentes
            for (int i = 0; i <= n - 4; i++) {
                for (int j = 0; j <= m - 4; j++) {
                    int lineScore = 0;
                    for (int k = 0; k < 4; k++) {
                        if (field[i + k][j + k] == jogador_1) lineScore += 1;
                        if (field[i + k][j + k] == jogador_2) lineScore -= 1;
                    }
                    if (lineScore == 4) score += 100;
                    else if (lineScore == -4) score -= 100;
                    else if (lineScore == 3 && k == vazio) score += 50;
                    else if (lineScore == -3 && k == vazio) score -= 50;
                }
            }

            // Avaliar diagonais ascendentes
            for (int i = 3; i < n; i++) {
                for (int j = 0; j <= m - 4; j++) {
                    int lineScore = 0;
                    for (int k = 0; k < 4; k++) {
                        if (field[i - k][j + k] == jogador_1) lineScore += 1;
                        if (field[i - k][j + k] == jogador_2) lineScore -= 1;
                    }
                    if (lineScore == 4) score += 100;
                    else if (lineScore == -4) score -= 100;
                    else if (lineScore == 3 && k == vazio) score += 50;
                    else if (lineScore == -3 && k == vazio) score -= 50;
                }
            }
             // Ajustar os pesos para as colunas centrais
            // for (int j = 0; j < m; j++) {
            //     int colWeight = std::abs(j - m / 2); // Peso maior para as colunas centrais
            //     score += colWeight; // Ajustar a pontuação conforme a distância ao centro
            // }
            
            return score;
        }

        int minimax(int depth, bool isMax) {
            int score = evaluate();

            if (score == 100 || score == -100)
                return score;

            if (depth == 0)
                return score;

            if (isMax) {
                int best = std::numeric_limits<int>::min();
                for (int j = 0; j < m; j++) {
                    for (int i = n - 1; i >= 0; i--) {
                        if (field[i][j] == vazio) {
                            field[i][j] = jogador_1;
                            best = std::max(best, minimax(depth - 1, !isMax));
                            field[i][j] = vazio;
                            break;
                        }
                    }
                }
                return best;
            } else {
                int best = std::numeric_limits<int>::max();
                for (int j = 0; j < m; j++) {
                    for (int i = n - 1; i >= 0; i--) {
                        if (field[i][j] == vazio) {
                            field[i][j] = jogador_2;
                            best = std::min(best, minimax(depth - 1, !isMax));
                            field[i][j] = vazio;
                            break;
                        }
                    }
                }
                return best;
            }
        }

        std::pair<int, int> findBestMove() {
            int bestVal = std::numeric_limits<int>::min();
            int bestMoveCol = -1;

            // Preferência pelas colunas centrais
            std::vector<int> columnOrder = {3, 2, 4, 1, 5, 0, 6};

            for (int j : columnOrder) {  // Itera nas colunas de acordo com a ordem de preferência
                for (int i = n - 1; i >= 0; i--) {
                    if (field[i][j] == vazio) {
                        field[i][j] = jogador_1;
                        int moveVal = minimax(6, false);  // Depth arbitrário
                        field[i][j] = vazio;

                        if (moveVal > bestVal) {
                            bestMoveCol = j;
                            bestVal = moveVal;
                        }
                        break;  // Pare após encontrar o primeiro espaço vazio
                    }
                }
            }

            return {bestMoveCol, bestVal};
        }

        int minimaxAlphaBeta(int depth, bool isMax, int alpha, int beta) {
            int score = evaluate();

            if (score == 100 || score == -100)
                return score;

            if (depth == 0)
                return score;

            if (isMax) {
                int best = std::numeric_limits<int>::min();
                for (int j = 0; j < m; j++) {
                    for (int i = n - 1; i >= 0; i--) {
                        if (field[i][j] == vazio) {
                            field[i][j] = jogador_1;
                            best = std::max(best, minimaxAlphaBeta(depth - 1, !isMax, alpha, beta));
                            field[i][j] = vazio;
                            alpha = std::max(alpha, best);
                            if (beta <= alpha)
                                break;
                        }
                    }
                }
                return best;
            } else {
                int best = std::numeric_limits<int>::max();
                for (int j = 0; j < m; j++) {
                    for (int i = n - 1; i >= 0; i--) {
                        if (field[i][j] == vazio) {
                            field[i][j] = jogador_2;
                            best = std::min(best, minimaxAlphaBeta(depth - 1, !isMax, alpha, beta));
                            field[i][j] = vazio;
                            beta = std::min(beta, best);
                            if (beta <= alpha)
                                break;
                        }
                    }
                }
                return best;
            }
        }

        std::pair<int, int> findBestMoveAlphaBeta() {
            int bestVal = std::numeric_limits<int>::min();
            int bestMoveCol = -1;

            for (int j = 0; j < m; j++) {
                for (int i = n - 1; i >= 0; i--) {
                    if (field[i][j] == vazio) {
                        field[i][j] = jogador_1;
                        int moveVal = minimaxAlphaBeta(6, false, std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
                        field[i][j] = vazio;

                        if (moveVal > bestVal) {
                            bestMoveCol = j;
                            bestVal = moveVal;
                        }
                        break;  // Pare após encontrar o primeiro espaço vazio
                    }
                }
            }

            return {bestMoveCol, bestVal};
        }
        void bot() {
            reiniciarTabuleiro(); // Reset
            // Configuração do gerador de números aleatórios
            std::mt19937 gen(time(nullptr));
            std::uniform_int_distribution<> dis(1, m);

            // Preencher o tabuleiro alternando jogadas até que todos os campos estejam preenchidos
            while (!endGame) {
                int coluna = dis(gen);  // gera uma coluna aleatória entre 1 e número de colunas
                jogar(coluna);
                resultado();
            }
        }
        void botIA(){
            reiniciarTabuleiro(); // Reset
            std::mt19937 gen(time(nullptr));
            std::uniform_int_distribution<> dis(1, m);
            while (!endGame) {
                std::pair<int, int> move;
                if (jogador == jogador_1) {
                    if(counter == 0) jogar(3);
                    else move = findBestMove();
                    jogar(move.first + 1);  // +1 para ajustar para a jogada do jogador
                } else {
                    int coluna = dis(gen);  // gera uma coluna aleatória entre 1 e número de colunas
                    jogar(coluna);
                }
                // display();
                // getchar();
                resultado();
            }            
        }

        void botIAxIA(){
            reiniciarTabuleiro(); // Reset
            while (!endGame) {
                std::pair<int, int> move;
                if (jogador == jogador_1) {
                    if(counter == 0) jogar(3);
                    else move = findBestMove();
                    jogar(move.first + 1);  // +1 para ajustar para a jogada do jogador
                } else {
                    move = findBestMove();
                    jogar(move.first + 1);
                }
                // display();
                // getchar();
                resultado();
            }
        }
};

int main() {
    // Lig4 game(6, 7, 4);  // cria um jogo de 6 linhas e 7 colunas
    // game.jogar(1);
    // game.jogar(2);
    // game.jogar(2);
    // game.jogar(2);
    // game.jogar(2);
    // game.display();
    
    // game.bot();          //bot para testar o jogo Lig4

    Lig4Tradicional gameT;   // cria um jogo de 6 linhas e 7 colunas
    
    //testa com entre jogadores que fazem escolhas aleatórias
    // {
    //     int i = 0;
    //     while(i != 200){
    //         gameT.bot();           
    //         i++;
    //     }
    // }

    
    //testa com entre um jogador AI (minimax) e um jogador que faz jogadas aleatórias
    //o jogador inteligente sempre deve vencer, ou seja, o jogador aleatório não deve vencer nenhuma partida
    {
            int i = 0;
        while(i != 200){
            gameT.botIA();
            i++;
        }
    }
    return 0;
}