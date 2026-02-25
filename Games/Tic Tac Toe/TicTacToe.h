#ifndef VELHA_H
#define VELHA_H

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

#include <string>
using std::string;                      //objeto string

#include <cstring>
using std::memcpy;                      //copia de elementos

#include <vector>
using std::vector;                      //objeto vector

#include <algorithm>                    //funcoes de busca
using std::find_if;                     //algoritmo de busca
using std::sort;                        //função com algoritmo de organização
using std::copy;

#include <cstdlib>
#include <ctime>        

#include <random>                       //geração de números aleatórios
using std::random_device;               //semente de números não determinísticos
using std::mt19937;                     //motor de geração gen(rd)
using std::uniform_int_distribution;    //probabilidade uniforme de inteiros

#include <initializer_list>             //lista de inicialização(permite utilizar uma lista como parâmetro)
using std::initializer_list;

#include <cassert>                      //verifica condições(como em um debug retornando true ou false) retornando erro

#include <iomanip>                      //formatação de entrada e saída de fluxos
using std::setw;                        //largura do campo de inserção
using std::setfill;                     //preenche o campo de inserção com um caractere ' '
using std::setbase;                     //base 10, 16, 8 ...
using std::setprecision;                //numero de digitos de presição de ponto flutuante
using std::left;                        //alinhamento a esquerda
using std::right;                       
using std::internal;                       


#include <cmath>

// 2. (5,0) O Jogo da Velha é um jogo simples e fácil de ser ensinado. Em um tabuleiro 3x3, jogadores se
// alternam escolhendo casas do tabuleiro. O objetivo final do jogo é ser o primeiro a traçar uma linha reta
// de três casas, seja na vertical, na horizontal ou na diagonal. Esse jogo pode ser generalizado para um
// tabuleiro n x n, onde o objetivo é também traçar uma linha reta, agora de tamanho n. Seu objetivo é
// implementar esse jogo. Implemente uma classe que gerencie um jogo completo de jogo da velha. Sua
// classe deve conter
// a) (1,5) Um construtor que recebe o tamanho n do tabuleiro. O tabuleiro deve ser alocado
// dinamicamente.
// b) (2,5) Um método jogar que recebe as coordenadas da casa a ser jogada e retorna um enum com a
// situação após a jogada. As situações possíveis são: Vitória do Jogador O, Vitória do Jogador X,
// Empate, Última Jogada Inválida, Última Jogada Válida. Uma Jogada Inválida ocorre quando são
// passadas coordenadas fora do tabuleiro ou já ocupadas. O método deve controlar de quem é a vez de
// jogar.
// d) (1,0) Um método que imprime o tabuleiro na sua situação corrente. A impressão deve ser similar ao
// seguinte exemplo.


enum player {x = 1, o = 9};
enum situation {vitoria_do_jogador_O = 0, vitoria_do_jogador_X, Empate, ultima_jogada_invalida, ultima_jogada_valida};

class Game
{
    private:
    size_t size;                            //tabuleiro nxn
    int max;                                //maximo de jogadas: baliza empate
    int min;                                //minimo de jogada para que haja um ganhador
    int counter;
    int time;                               //define de quem é a vez
    int **field;                            //tabuleiro

    public:
    Game(int n) : size(n), max(n*n), min(n), counter(0), time(x)    //construtor que recebe o tamamnho n do tabuleiro
    {
        field = new int*[n];
        for (size_t i = 0; i < size; i++)
        {
            field[i] = new int[n];
        }

        for(size_t i = 0; i < size; i++)
            for(size_t j = 0; j < size; j++)
            {
                field[i][j] = 0;
            }
    }
    ~Game(){delete[] field;}

    Game(const Game&) = delete;             //desabilitando o construtor por cópia
    Game& operator=(const Game&) = delete;

    //metodos privados
    private:
    void display();
    situation verify(int, int);
    situation turn();
    //metodos publicos
    public:
    void play();
};

void Game::display()
{
    system("cls");
    std::cout << "JOGO DA VELHA\n\n";
    for(size_t k = 0; k < (2*size + 1); k++) cout << "-";
    cout << endl;

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j <  size; j++)
        {
            if(field[i][j] == 0) cout << "|" << " ";
            else if(field[i][j] == x) cout << "|" << "x";
            else if(field[i][j] == o) cout << "|" << "o";
        }
        cout << "|" << endl;
        for(size_t k = 0; k < (2*size + 1); k++) cout << "-";
        cout << endl;
    }
}

void Game::play()
{
    while(true)
    {
        situation time = turn();
        if(time == Empate)
        {
            display();
            cout << "Jogo empatado" << endl;
            break;
        }
        else if(time == vitoria_do_jogador_X)
        {
            {
            display();
            cout << "Vitoria do jogador X" << endl;
            break;
            }
        }
        else if(time == vitoria_do_jogador_O)
        {
            {
            display();
            cout << "Vitoria do jogador O" << endl;
            break;
            }
        }
        else if(time == ultima_jogada_invalida)
        {
            {
            cout << "Jogada invalida" << endl;
            continue;
            }
        }
        else if(time == ultima_jogada_valida) continue;
    }
}

situation Game::turn()
{
    display();
    //CAPTURANDO DADOS DE TABULEIRO
    int i, j;
    cout << "Insira as coordenadas de 1 a " << size << ": ";
    while(true)
    {
        cin.clear();
        cin >> i >> j;
        if(!(i < 1 || i > (int)size || j < 1 || j > (int)size)) break;
        else cout << "Campo fora dos limites. Insira novamente: ";
    }
    i--; j--;                       //ajustar para as coordenadas do tabuleiro
    
    //INSERINDO DADOS
    if(field[i][j] != 0) return ultima_jogada_invalida;
    if(time == x)                   //vez de x
    {
        field[i][j] = x;            //insere x no campo determinado
        time = o;                   //muda a vez
        counter++;
        if(counter < min) return ultima_jogada_valida;
        if(verify(i,j) == vitoria_do_jogador_X)  return vitoria_do_jogador_X;
        if(counter == max) return Empate;
        else return ultima_jogada_valida;
    }
    else                            //vez de o
    {
        field[i][j] = o;            //insere o no campo determinado
        time = x;                   //muda a vez
        counter++;
        if(counter < min) return ultima_jogada_valida;
        if(verify(i,j) == vitoria_do_jogador_O) return vitoria_do_jogador_O;
        if(counter == max) return Empate;
        else return ultima_jogada_valida;
    }
}

situation Game::verify(int i, int j)
{
    if(i == j || i == (int)size - j)             //pertence as diagonais
    {
        int sum = 0;
        for(int m = 0; m < (int)size; m++)            //diagonal principal
        {
            sum += field[m][m];
        }            
        if(sum == (int)size * x) return vitoria_do_jogador_X;
        if(sum == (int)size * o) return vitoria_do_jogador_O;

        sum = 0;  
        for(int n = 0; n < (int)size; n++)           //diagonal secundaria
        {
            sum += field[n][size - n];
        }
        if(sum == (int)size * x) return vitoria_do_jogador_X;
        if(sum == (int)size * o) return vitoria_do_jogador_O;
    }
    //vericar linha e coluna
    int sum = 0;
    for(int m = 0; m < (int)size; m++)               //linha
    {
        sum += field[i][m];
    }
    if(sum == (int)size * x) return vitoria_do_jogador_X;
    if(sum == (int)size * o) return vitoria_do_jogador_O;

    sum = 0;
    for(int m = 0; m < (int)size; m++)               //coluna
    {
        sum += field[m][j];
    }
    if(sum == (int)size * x) return vitoria_do_jogador_X;
    if(sum == (int)size * o) return vitoria_do_jogador_O;

    return ultima_jogada_valida;
}

#endif