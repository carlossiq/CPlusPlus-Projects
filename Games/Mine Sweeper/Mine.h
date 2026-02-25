#ifndef MINE_H
#define MINE_H

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



class MineField
{
    private:
    struct Bomb
    {
        int ground;
        bool check;
    };

    int size;
    int mineOn;
    int mineOff;
    Bomb **field;

    private:
    void sortField();
    void build();
    void openZero(int, int);
    void display();

    public:
    MineField(int size) : size(size), mineOn(0), mineOff(0)
    {
        sortField();
        build();
    }

    ~MineField(){delete[] field;}

    MineField(const MineField&) = delete;
    MineField& operator=(const MineField&) = delete;
    
    void showAnswer();
    int play();

};

using MF = MineField;
enum end{lost = 0, win};

void MF::sortField()
{
    mt19937 gen(time(nullptr));
    uniform_int_distribution<mt19937::result_type> dist (0, 99);

    field = new Bomb*[size];
    for(int i = 0; i < size; i++) field[i] = new Bomb[size];
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
        {
            if(dist(gen) < 15)
            {
                field[i][j].ground = 999;                       //preenche com minas com 10% de chance
                mineOn++;
            }
            else field[i][j].ground = 0;
            field[i][j].check = false;
        }
    mineOff = size*size - mineOn;
}

void MF::build()
{
    for(int i = 0; i < size; i++)
        for(int j = 0; j < size; j++)
        {
            if(field[i][j].ground == 999)
            {
                for(int m = i - 1; m <= (i + 1); m++ )
                    for(int n = j - 1; n <= (j + 1); n++)
                        if(i == m && j == n) continue;
                        else if((m >= 0 && m < size) && (n>=0 && n < size) && field[m][n].ground != 999) field[m][n].ground++;
                        //soma os arredores de onde há minas
            }
        }
}

void MF::showAnswer()
{
    char bomb = '*';
    for(int m = 0; m <= size; m++)
        if(m == 0) cout << "  ";
        else cout << " " << m;
    cout << endl;
    for(int i = 0; i < size; i++)
    {
        cout << (i + 1) << " ";
        for(int j = 0; j < size; j++)
        {
            if(field[i][j].ground == 999) cout << "|" << bomb;
            else cout << "|" << field[i][j].ground;
        }
        cout << "|" << endl;
    }
}

void MF::display()
{
    char blank = ' ';
    for(int m = 0; m <= size; m++)
        if(m == 0) cout << "  ";
        else cout << " " << m;
    cout << endl;
    for(int i = 0; i < size; i++)
    {
        cout << (i + 1) << " ";
        for(int j = 0; j < size; j++)
        {
            if(field[i][j].ground != 999 && field[i][j].check == true) cout << "|" << field[i][j].ground;
            else cout << "|" << blank;
        }
        cout << "|" << endl;
    }
}

void MF::openZero(int i, int j)
{
    for(int m = (i - 1); m <= (i + 1); m++)
        for(int n = (j - 1); n <= (j + 1); n++)
            {
                if((m >= 0 && m < size) && (n>=0 && n < size))
                {
                    if(field[m][n].ground != 999 && field[m][n].check == false)
                    {
                        if(field[m][n].ground == 0)
                        {
                            field[m][n].check = true;
                            openZero(m,n);
                        }
                        field[m][n].check = true;
                        mineOff--;
                    }
                }
            }
}

int MF::play()
{
    while(true)
    {
        cout.clear();
        int i, j;
        cout << "MINE SWEEPER" << endl;
        display();
        while(true)
        {
            cin.clear();
            cout << "Escolha um espaco de [1 a 8][1 a 8]: " << endl;
            cin >> i >> j;
            if((1 <= i && i <= 8) && (1 <= j && j <= 8)) break;
            else cout << "espaco invalido" << endl;
        }
        i--; j--;
        if(field[i][j].ground == 999)
        {
            display();
            cout << "Voce perdeu!"<< endl;
            return lost;
        }
        else if(field[i][j].ground == 0)
        {
            openZero(i,j);
        }
        else
            {
                if(field[i][j].check == false)
                {                 
                    field[i][j].check = true;
                    mineOff--;
                    if(mineOff == 0)
                    {
                        display();
                        cout << "Voce ganhou!" << endl;
                        return win;
                    }
                }   
            }
    }
}

#endif