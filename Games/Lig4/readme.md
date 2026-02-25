# Lig4 (Connect Four) — IA Ótima vs Jogador Aleatório (C++)

Implementação do jogo **Lig4 (Connect Four)** em C++ com:
- Uma classe base configurável (`Lig4`) e uma derivada para o **Lig4 tradicional (7x6, 4 em sequência)**.
- Detecção de resultado por meio de um `enum` (`Vitória da Cor1`, `Vitória da Cor2`, `Empate`, `Continua`).
- Dois comportamentos de jogador:
  - **Jogador 1 (IA “ótima”)**: escolhe jogadas usando **Minimax** (com preferência por colunas centrais).
  - **Jogador 2 (aleatório)**: joga colunas aleatórias válidas.
- Simulações repetidas de partidas para testar o comportamento (espera-se que a IA vença praticamente sempre).

> Projeto acadêmico — **Sistemas Operacionais / Programação (exercício de OOP + estratégia)**  
> Autor: **Carlos Alexandre Siqueira de Almeida**

---

## 🎮 Regras do jogo (Lig4 tradicional)

- Tabuleiro **7 colunas x 6 linhas**.
- Jogadores alternam inserindo uma peça em uma coluna (a peça “cai” até a última posição livre).
- Vence quem formar **4 peças consecutivas** da mesma cor em:
  - horizontal
  - vertical
  - diagonal descendente
  - diagonal ascendente
- Se o tabuleiro encher sem vencedor: **empate**.

---

## 🧠 IA (Jogador 1)

A IA utiliza uma heurística de avaliação (`evaluate()`) e busca por melhor jogada via:
- `minimax(depth, isMax)`
- (há também uma versão com poda) `minimaxAlphaBeta(depth, isMax, alpha, beta)` — opcional

Além disso, existe uma **ordem de preferência por colunas centrais** para melhorar a qualidade das escolhas:
`{3, 2, 4, 1, 5, 0, 6}` (considerando índice 0..6).

---

## ✅ Resultados

A função `resultado()` na classe `Lig4Tradicional` retorna sempre o estado atual do jogo:

```cpp
enum Resultado { VitoriaCor1, VitoriaCor2, Empate, Continua };