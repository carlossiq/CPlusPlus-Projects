# Mine Sweeper — C++

Implementação em C++ do clássico jogo **Campo Minado** em um tabuleiro **n × n**, com geração aleatória de minas, abertura recursiva de áreas vazias e detecção de vitória/derrota.

---

## Compilar e Executar

```bash
g++ -std=c++11 main.cpp -o minesweeper && ./minesweeper
```

---

## Como Jogar

O programa gera automaticamente o tabuleiro com minas distribuídas aleatoriamente (~15% de chance por célula). A cada turno, informe as coordenadas `linha coluna` (de 1 a 8).

```
MINE SWEEPER
   1 2 3 4 5 6 7 8
1 | | | | | | | | |
2 | | | | | | | | |
...
Escolha um espaco de [1 a 8][1 a 8]:
```

- Acertar uma **mina** → derrota imediata.
- Abrir uma célula com valor **0** → abre recursivamente todas as células vizinhas vazias.
- Revelar todas as células **sem mina** → vitória.

---

## Estrutura

| Arquivo    | Descrição                                    |
|------------|----------------------------------------------|
| `mine.h`   | Classe `MineField` — lógica completa do jogo |
| `main.cpp` | Ponto de entrada                             |

| Retorno | Descrição                                        |
|---------|--------------------------------------------------|
| `lost`  | Jogador pisou em uma mina                        |
| `win`   | Jogador revelou todas as células seguras         |

---

## Detalhes de Implementação

| Método         | Descrição                                                                  |
|----------------|----------------------------------------------------------------------------|
| `sortField()`  | Aloca o tabuleiro e distribui minas aleatoriamente com `mt19937`           |
| `build()`      | Calcula o número de minas vizinhas de cada célula                          |
| `openZero()`   | Abre recursivamente células adjacentes quando o valor da célula é zero     |
| `display()`    | Exibe o estado atual do tabuleiro (células reveladas e ocultas)            |
| `showAnswer()` | Exibe o gabarito completo com todas as minas e valores                     |
| `play()`       | Loop principal — lê coordenadas, processa jogada e retorna o resultado     |

---

## Observações

- O tamanho do tabuleiro é configurável via construtor `MineField(int size)`, mas a validação de entrada em `play()` está fixada em `[1 a 8]` — ajuste conforme o tamanho escolhido.
- O destrutor libera apenas o array externo (`delete[] field`); para evitar vazamento de memória, libere também cada linha individualmente antes do `delete[] field`.