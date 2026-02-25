// Projeto de um simulador de escalonamento de processos com múltiplas filas de prioridade e envelhecimento,
// implementado em C++14. O programa recebe como entrada o número total de processos, a duração do surto de cada
// processo e o número de operações de E/S de cada processo. A saída é um diagrama de Gantt que mostra a execução
// dos processos ao longo do tempo.

// Aula de Sistemas Operacionais

// Aluno: Carlos Alexandre Siqueira de Almeida

#include <bits/stdc++.h>
using namespace std;

// o programa deve receber como entrada:
// 1. total de processos
// 2. duração surto de cada processo
// 3. numero de operações de E/S de cada processo

// saida: diagrama de gantt
bool switchCommentary = false;

enum State
{
    NONE,
    READY,
    WAITING,
    TERMINATED
};

class Process
{
    string name;
    int burstTime;
    int remainingBurstTime;
    int arrivalTime;
    int ioOperations;
    int remainingIoOperations;
    int age;
    State state;

public:
    Process(string name, int burstTime, int ioOperations)
    {
        this->name = name;
        this->burstTime = burstTime;
        this->remainingBurstTime = burstTime;
        this->arrivalTime = 0;
        this->ioOperations = ioOperations;
        this->remainingIoOperations = ioOperations;
        this->age = 0;
        this->state = NONE;
    }

    Process &setArrivalTime(int arrivalTime)
    {
        this->arrivalTime = arrivalTime;
        return *this;
    }

    Process &operator++()
    {
        this->age++;
        return *this;
    }

    Process &decrementRemainingBurstTime()
    {
        this->remainingBurstTime--;
        return *this;
    }

    Process &decrementIoOperations()
    {
        this->remainingIoOperations--;
        return *this;
    }

    Process &execute()
    {
        this->remainingBurstTime--;
        return *this;
    }

    Process &reset()
    {
        this->remainingBurstTime = this->burstTime;
        this->age = 0;
        return *this;
    }

    Process &setState(State state)
    {
        this->state = state;
        return *this;
    }

    int getAge() const
    {
        return this->age;
    }

    State getState() const
    {
        return this->state;
    }

    int getRemainingBurstTime() const
    {
        return this->remainingBurstTime;
    }

    int getBurstTime() const
    {
        return this->burstTime;
    }

    int getRemainingIoOperations() const
    {
        return this->remainingIoOperations;
    }

    string getName() const
    {
        return this->name;
    }
};

class Scheduler
{
private:
    // entrada do programa: construtor
    int numberOfProcesses;
    vector<Process> processes;
    vector<int> bursts;
    vector<int> ioOperations;

    // filas do escalonador
    int quantum_0 = 10;
    int quantum_1 = 15;
    queue<Process *> Q0; // fila de alta prioridade: Round Robin
    queue<Process *> Q1; // fila de média prioridade: Round Robin
    queue<Process *> Q2; // fila de baixa prioridade: FCFS

    // funcionamento do programa
    int clock;
    int ioClock; // 30ms

    // Gantt
    vector<string> gantt;

    queue<Process *> readyQueue;
    queue<Process *> waitingQueue;
    queue<Process *> terminatedQueue;

    // verifica se o processo que estava em espera no IO terminou
    bool ioCompleted()
    {
        if (ioClock == 30)
            return true;
        else
            return false;
    }

    // tempo
    Scheduler &incrementClock()
    {
        clock++;
        if (!waitingQueue.empty())
        {
            ioClock++;
            if(switchCommentary) cout << "IO Clock: " << ioClock << endl;
        }
        if (!Q0.empty())
            Q0.front()->decrementRemainingBurstTime();
        else if (!Q1.empty())
            Q1.front()->decrementRemainingBurstTime();
        else if (!Q2.empty())
        {
            Q2.front()->decrementRemainingBurstTime();
            if(switchCommentary) cout << Q2.front()->getRemainingBurstTime() << endl;
        }
        return *this;
    }

    // move os processos entre as filas
    // só vai para prontos se estiver entrando pela primeira vez ou se estiver saindo de waiting
    Scheduler &moveToReadyQueue(Process &p)
    {
        State state = p.getState(); // obtem o estado do processo na fila antes de mover
        p.setState(READY).reset();  // muda o estado do processo para pronto e reseta o processo
        readyQueue.push(&p);        // adiciona o processo na fila de prontos
        if (state == WAITING)
        { // se o processo estava esperando, remove da fila de espera
            waitingQueue.pop();
            ioClock = 0; // reseta o clock de I/O
            if (p.getRemainingIoOperations() != 0)
                p.decrementIoOperations();
        }
        return *this;
    }

    // só vai para waiting se estiver saindo de qualquer fila
    Scheduler &MoveToWaitingQueue(Process &p)
    {
        p.setState(WAITING);
        waitingQueue.push(&p);
        auto age = p.getAge();
        if (age == 0)
            Q0.pop();
        else if (age == 1)
            Q1.pop();
        else
            Q2.pop();
        return *this;
    }

    // só vai para terminated se estiver saindo de qualquer fila
    Scheduler &MoveToTerminated(Process &p)
    {
        p.setState(TERMINATED);
        terminatedQueue.push(&p);
        auto age = p.getAge();
        if (age == 0)
            Q0.pop();
        else if (age == 1)
            Q1.pop();
        else
            Q2.pop();

        return *this;
    }

    // move os processos entre as filas de prioridade
    Scheduler &scheduleProcess(Process &p)
    {
        int age = p.getAge();
        if (age == 0)
        {
            p.setArrivalTime(clock);
            Q0.push(&p);
            readyQueue.pop();
        }
        else if (age == 1)
        {
            Q1.push(&p);
            Q0.pop();
        }
        else if (age == 2)
        {
            Q2.push(&p);
            Q1.pop();
        }
        else
        {
            ;
        }
        return *this;
    }

    // display queues
    Scheduler &displayReadyQueue()
    {
        queue<Process *> q(readyQueue);
        cout << "Ready Queue: ";
        while (!q.empty())
        {
            cout << q.front()->getName() << " ";
            q.pop();
        }
        cout << endl;
        return *this;
    }

    Scheduler &displayWaitingQueue()
    {
        queue<Process *> q(waitingQueue);
        cout << "Waiting Queue: ";
        while (!q.empty())
        {
            cout << q.front()->getName() << " ";
            q.pop();
        }
        cout << endl;
        return *this;
    }

    Scheduler &displayTerminatedQueue()
    {
        queue<Process *> q(terminatedQueue);
        cout << "Terminated Queue: ";
        while (!q.empty())
        {
            cout << q.front()->getName() << " ";
            q.pop();
        }
        cout << endl;
        return *this;
    }

    Scheduler &displayQ0()
    {
        queue<Process *> q(Q0);
        cout << "Q0: ";
        while (!q.empty())
        {
            cout << q.front()->getName() << " ";
            q.pop();
        }
        cout << endl;
        return *this;
    }

    Scheduler &displayQ1()
    {
        queue<Process *> q(Q1);
        cout << "Q1: ";
        while (!q.empty())
        {
            cout << q.front()->getName() << " ";
            q.pop();
        }
        cout << endl;
        return *this;
    }

    Scheduler &displayQ2()
    {
        queue<Process *> q(Q2);
        cout << "Q2: ";
        while (!q.empty())
        {
            cout << q.front()->getName() << " ";
            q.pop();
        }
        cout << endl;
        return *this;
    }

    void tryToEnd(Process &p)
    {
        if (p.getRemainingBurstTime() == 0)
        {
            if (p.getRemainingIoOperations() == 0)
                MoveToTerminated(p); // retira automaticamente da fila anterior
            else
                MoveToWaitingQueue(p);
        }
    }

    // processador
    // 1. verifica se há processos completos no IO e passa para pronto
    Scheduler &verifyIO()
    {
        if (ioCompleted())
        {
            Process *p = waitingQueue.front();
            moveToReadyQueue(*p);
        }
        return *this;
    }
    // 2. verifica se há processos na fila de pronto e escalona
    Scheduler &prepareScheduler()
    {
        while (!readyQueue.empty())
        {
            Process *p = readyQueue.front();
            scheduleProcess(*p);
            // schedule já remove processo da fila de prontos
        }
        return *this;
    }
    // 3. verificar envelhecimento dos processos e encerra se necessário
    Scheduler &verifyAging()
    {
        if (!Q0.empty())
        {
            Process *p = Q0.front();
            tryToEnd(*p);
            if (p->getState() == READY && p->getBurstTime() - p->getRemainingBurstTime() == quantum_0)
            {
                p->operator++();     // aumenta a idade do processo
                scheduleProcess(*p); // tira automaticamente da fila de Q0
            }
        }
        else if (!Q1.empty())
        {
            Process *p = Q1.front();
            tryToEnd(*p);
            if (p->getState() == READY && p->getBurstTime() - p->getRemainingBurstTime() == quantum_1 + quantum_0)
            {
                p->operator++();     // aumenta a idade do processo
                scheduleProcess(*p); // tira automaticamente da fila de Q1
            }
        }
        else if (!Q2.empty())
        {
            Process *p = Q2.front();
            tryToEnd(*p);
        }
        return *this;
    }

    // vetores de gantt
    string getRunningProcess()
    {
        if (!Q0.empty())
            return Q0.front()->getName();
        else if (!Q1.empty())
            return Q1.front()->getName();
        else if (!Q2.empty())
            return Q2.front()->getName();
        else
            return "  ";
    }

    Scheduler &fillGantt()
    {
        gantt.push_back(getRunningProcess());
        return *this;
    }

public:
    void printGanttChart()
    {
        if (gantt.empty())
        {
            cout << "No processes were executed." << endl;
            return;
        }

        // Primeira linha: nomes dos processos e slots de tempo
        cout << "Grafico de Gantt: \n";
        string previousProcess = gantt[0];
        int startTime = 0;

        // Variáveis para armazenar o que será impresso
        string ganttBar = "| " + previousProcess + " ";
        string timeMarks = to_string(startTime);

        for (size_t i = 1; i < gantt.size(); i++)
        {
            if (gantt[i] != previousProcess)
            {
                // Adiciona o marcador de tempo abaixo da mudança de processo
                timeMarks += string(ganttBar.length() - timeMarks.length() - 1, ' ') + to_string(i);

                // Inicia a nova barra para o próximo processo
                ganttBar += "| " + gantt[i] + " ";
                previousProcess = gantt[i]; // Atualiza o processo anterior
            }
        }

        // Adiciona o marcador do tempo final
        timeMarks += string(ganttBar.length() - timeMarks.length() - 1, ' ') + to_string(gantt.size());

        // Imprime as barras de Gantt e os tempos correspondentes
        cout << ganttBar << "|\n";
        cout << timeMarks << endl;
    }

    int getCurrentTime() const
    {
        return clock;
    }

    // construtor (já adiciona os processos na fila de prontos)
    Scheduler(int numberOfProcesses, initializer_list<int> bursts, initializer_list<int> ioOperations)
    {
        if (bursts.size() != static_cast<size_t>(numberOfProcesses) || ioOperations.size() != static_cast<size_t>(numberOfProcesses))
        {
            throw invalid_argument("Number of bursts and I/O operations must be equal to the number of processes.");
        }
        else
        {
            this->numberOfProcesses = numberOfProcesses;
            this->bursts.assign(bursts);
            this->ioOperations.assign(ioOperations);
            this->clock = 0;
            this->ioClock = 0;

            for (int i = 0; i < numberOfProcesses; i++)
            {
                Process *p = new Process("P" + to_string(i), this->bursts[i], this->ioOperations[i]);
                moveToReadyQueue(*p);
            }
        }
    }

    // 4. executa os processos
    void execute()
    {
        switchCommentary = true;
        while (terminatedQueue.size() != static_cast<size_t>(numberOfProcesses))
        {
            verifyIO();
            prepareScheduler();
            verifyAging();
            incrementClock();
            displayWaitingQueue().displayQ0().displayQ1().displayQ2().displayTerminatedQueue();
            cout << "Clock: " << getCurrentTime() << endl;
            getchar();
        }
        switchCommentary = false;
    }

    void executeStep()
    {
        verifyIO();
        prepareScheduler();
        verifyAging();
        incrementClock();
        displayWaitingQueue().displayQ0().displayQ1().displayQ2().displayTerminatedQueue();
        cout << "Clock: " << getCurrentTime() << endl;
    }

    void executeStep(int steps)
    {
        for (int i = 0; i < steps; i++)
        {
            verifyIO();
            prepareScheduler();
            verifyAging();
            incrementClock();
        }
        displayQ0().displayQ1().displayQ2().displayTerminatedQueue();
        cout << "Clock: " << getCurrentTime() << endl;
    }

    void executeToGantt()
    {
        while (terminatedQueue.size() != static_cast<size_t>(numberOfProcesses))
        {
            verifyIO().prepareScheduler().verifyAging().incrementClock();
            if(terminatedQueue.size() != static_cast<size_t>(numberOfProcesses)) fillGantt();
        }
    }
};

int main()
{
    //Scheduler cpu(2, {50, 20}, {1, 2});
    Scheduler cpu(4, {8, 40, 10, 30}, {3, 1, 2, 1});
    // cpu.execute();
    cpu.executeToGantt();
    cpu.printGanttChart();
    return 0;
}