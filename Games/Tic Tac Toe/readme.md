# Jogo da Velha Generalizado — C++

Implementação em C++ de um jogo da velha para tabuleiros **n × n**, com alocação dinâmica, controle de turnos, detecção de vitória/empate e validação de jogadas.

---

## Compilar e Executar

```bash
g++ -std=c++11 main.cpp -o velha && ./velha
```

---

## Como Jogar

O programa solicita o tamanho do tabuleiro e, a cada turno, pede as coordenadas `linha coluna` (de 1 a n). Os jogadores **X** e **O** se alternam até que haja vitória ou empate.

```
JOGO DA VELHA

-------
| | | |
-------
| | | |
-------
| | | |
-------
Insira as coordenadas de 1 a 3: 2 2
```

---

## Estrutura

| Arquivo   | Descrição                              |
|-----------|----------------------------------------|
| `velha.h` | Classe `Game` — lógica completa do jogo |
| `main.cpp` | Ponto de entrada                      |

| Situação                  | Descrição                              |
|---------------------------|----------------------------------------|
| `vitoria_do_jogador_X`    | Jogador X completou uma linha          |
| `vitoria_do_jogador_O`    | Jogador O completou uma linha          |
| `Empate`                  | Tabuleiro cheio sem vencedor           |
| `ultima_jogada_valida`    | Jogada aceita, jogo continua           |
| `ultima_jogada_invalida`  | Coordenada inválida ou casa ocupada    |

---

## Observações

- `system("cls")` é compatível com **Windows**; substitua por `system("clear")` no Linux/macOS.
- Correção recomendada na diagonal secundária: `field[n][size - n]` → `field[n][size - 1 - n]`.