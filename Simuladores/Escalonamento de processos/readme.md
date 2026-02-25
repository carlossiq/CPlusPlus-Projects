# Simulador de Escalonamento de Processos (MLFQ + Envelhecimento) — C++14

Simulador de escalonamento de processos desenvolvido para a disciplina de **Sistemas Operacionais**.  
O projeto implementa um escalonador com **múltiplas filas de prioridade (MLFQ)**, combinando **Round Robin** nas filas superiores e **FCFS** na fila inferior, além de simular **operações de E/S** e gerar como saída um **Diagrama de Gantt** com a execução ao longo do tempo.

**Autor:** Carlos Alexandre Siqueira de Almeida  
**Linguagem:** C++14
**IA:** Uso de IA somente para a função "print"

---

## 📌 Funcionalidades

- **MLFQ (Multi-Level Feedback Queue)** com 3 filas:
  - **Q0 (alta prioridade):** Round Robin (quantum = 10)
  - **Q1 (média prioridade):** Round Robin (quantum = 15)
  - **Q2 (baixa prioridade):** FCFS
- **Envelhecimento / rebaixamento de prioridade** conforme o processo consome quantum
- **Simulação de E/S (I/O)** com tempo fixo de conclusão (30 unidades)
- **Estados do processo:** `READY`, `WAITING`, `TERMINATED`
- **Geração e impressão de um diagrama de Gantt** ao final da simulação

---

## ⚙️ Requisitos

- Compilador C++ com suporte a **C++14**
  - Linux/macOS: `g++`
  - Windows: MinGW (`g++`) ou WSL

---

## 🧱 Estrutura Conceitual

### Filas de prioridade
- Processos entram em **Q0** (alta prioridade).
- Ao consumir o quantum de Q0, o processo é **rebaixado** para Q1.
- Ao consumir o quantum de Q1, é **rebaixado** para Q2.
- Em Q2, o escalonamento segue **FCFS** até terminar o burst (ou ir para E/S).

### E/S (I/O)
- Quando o burst chega a zero:
  - Se ainda restam operações de E/S, o processo vai para `WAITING`.
  - Quando o tempo de E/S completa (30), o processo retorna para `READY`.
  - Quando não restam operações de E/S, o processo vai para `TERMINATED`.

---

## ▶️ Como compilar e executar

### Linux / macOS
```bash
g++ -std=c++14 -O2 -Wall -Wextra SO.cpp -o scheduler
./scheduler