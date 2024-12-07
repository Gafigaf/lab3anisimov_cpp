#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <iomanip>
#include <random>
#include <chrono>


const std::string BLUE = "\033[1;34m";
const std::string RESET = "\033[1;37m";

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int priority;
    int waitingTime;
    int completionTime;

    Process(int _id, int _arrival, int _burst, int _priority)
        : id(_id), arrivalTime(_arrival), burstTime(_burst),
        remainingTime(_burst), priority(_priority),
        waitingTime(0), completionTime(0) {
    }
};

class ProcessScheduler {
private:
    std::vector<Process> processes;
    std::mt19937 gen;

public:
    ProcessScheduler() : gen(std::chrono::steady_clock::now().time_since_epoch().count()) {}

    void generateRandomProcesses(int count) {
        std::uniform_int_distribution<> arrivalDist(0, 10);
        std::uniform_int_distribution<> burstDist(1, 20);
        std::uniform_int_distribution<> priorityDist(1, 10);

        processes.clear();
        for (int i = 0; i < count; ++i) {
            processes.emplace_back(
                i + 1,
                arrivalDist(gen),
                burstDist(gen),
                priorityDist(gen)
            );
        }

        std::sort(processes.begin(), processes.end(),
            [](const Process& a, const Process& b) {
                return a.arrivalTime < b.arrivalTime;
            });
    }

    void simulateFCFS() {
        std::cout << "\n*---- FCFS Scheduling ----*" << std::endl;

        std::vector<Process> fcfsProcesses = processes;
        int currentTime = 0;
        double totalWaitingTime = 0;
        double totalTurnaroundTime = 0;

        for (auto& process : fcfsProcesses) {
            if (currentTime < process.arrivalTime) {
                currentTime = process.arrivalTime;
            }

            process.waitingTime = currentTime - process.arrivalTime;
            totalWaitingTime += process.waitingTime;

            currentTime += process.burstTime;
            process.completionTime = currentTime;

            int turnaroundTime = process.completionTime - process.arrivalTime;
            totalTurnaroundTime += turnaroundTime;
        }

        printTable(fcfsProcesses);

        std::cout << "\nFCFS Average Waiting Time: "
            << totalWaitingTime / fcfsProcesses.size()
            << std::endl;
        std::cout << "FCFS Average Turnaround Time: "
            << totalTurnaroundTime / fcfsProcesses.size()
            << std::endl;
    }


    void simulateSJF() {
        std::cout << "\n*---- Shortest Job First (SJF) Scheduling ----*" << std::endl;

        std::vector<Process> sjfProcesses = processes;
        int currentTime = 0;
        double totalWaitingTime = 0;
        double totalTurnaroundTime = 0;

        auto compareBurstTime = [](const Process& a, const Process& b) {
            return a.remainingTime < b.remainingTime;
            };

        while (!sjfProcesses.empty()) {
            auto it = std::min_element(sjfProcesses.begin(), sjfProcesses.end(), compareBurstTime);

            if (currentTime < it->arrivalTime) {
                currentTime = it->arrivalTime;
            }

            it->waitingTime = currentTime - it->arrivalTime;
            totalWaitingTime += it->waitingTime;

            currentTime += it->burstTime;
            it->completionTime = currentTime;

            int turnaroundTime = it->completionTime - it->arrivalTime;
            totalTurnaroundTime += turnaroundTime;

            sjfProcesses.erase(it);
        }

        printTable(processes);

        std::cout << "\nSJF Average Waiting Time: "
            << totalWaitingTime / processes.size()
            << std::endl;
        std::cout << "SJF Average Turnaround Time: "
            << totalTurnaroundTime / processes.size()
            << std::endl;
    }



    void simulatePrioritySchedulingWithAging(int agingFactor = 1) {
        std::cout << "\n*---- Priority Scheduling with Aging ----*" << std::endl;

        std::vector<Process> priorityProcesses = processes;
        int currentTime = 0;
        double totalWaitingTime = 0;
        double totalTurnaroundTime = 0;

        while (!priorityProcesses.empty()) {
            for (auto& process : priorityProcesses) {
                if (currentTime >= process.arrivalTime) {
                    process.priority -= agingFactor;
                }
            }

            auto it = std::min_element(priorityProcesses.begin(), priorityProcesses.end(),
                [](const Process& a, const Process& b) {
                    return a.priority < b.priority;
                });

            if (currentTime < it->arrivalTime) {
                currentTime = it->arrivalTime;
            }

            it->waitingTime = currentTime - it->arrivalTime;
            totalWaitingTime += it->waitingTime;

            currentTime += it->burstTime;
            it->completionTime = currentTime;

            int turnaroundTime = it->completionTime - it->arrivalTime;
            totalTurnaroundTime += turnaroundTime;

            priorityProcesses.erase(it);
        }

        printTable(processes);

        std::cout << BLUE << "\nPriority Scheduling with Aging Average Waiting Time: "
            << totalWaitingTime / processes.size()
            << std::endl;
        std::cout << "Priority Scheduling with Aging Average Turnaround Time: "
            << totalTurnaroundTime / processes.size()
            << RESET << std::endl;
    }



    void simulateRoundRobin(int timeQuantum = 4) {
        std::cout << "\n*---- Round Robin Scheduling ----*" << std::endl;

        std::vector<Process> rrProcesses = processes;
        std::queue<Process*> readyQueue;
        int currentTime = 0;
        double totalWaitingTime = 0;
        int processesCompleted = 0;

        for (auto& process : rrProcesses) {
            if (process.arrivalTime == 0) {
                readyQueue.push(&process);
            }
        }

        while (!readyQueue.empty() || processesCompleted < rrProcesses.size()) {
            for (auto& process : rrProcesses) {
                if (process.arrivalTime == currentTime) {
                    readyQueue.push(&process);
                }
            }

            if (!readyQueue.empty()) {
                Process* currentProcess = readyQueue.front();
                readyQueue.pop();

                int executionTime = std::min(timeQuantum, currentProcess->remainingTime);
                currentProcess->remainingTime -= executionTime;
                currentTime += executionTime;

                for (auto& process : rrProcesses) {
                    if (process.arrivalTime <= currentTime &&
                        process.remainingTime > 0 &&
                        &process != currentProcess) {
                        readyQueue.push(&process);
                    }
                }

                if (currentProcess->remainingTime <= 0) {
                    currentProcess->completionTime = currentTime;
                    processesCompleted++;
                    currentProcess->waitingTime = currentProcess->completionTime - currentProcess->arrivalTime - currentProcess->burstTime;
                    totalWaitingTime += currentProcess->waitingTime;
                }
                else {
                    readyQueue.push(currentProcess);
                }
            }
            else {
                currentTime++;
            }
        }

        printTable(rrProcesses);

        std::cout << "\nRound Robin Average Waiting Time: "
            << totalWaitingTime / rrProcesses.size()
            << std::endl;
    }


    void simulatePriorityScheduling() {
        std::cout << "\n*---- Priority Scheduling ----*" << std::endl;

        std::vector<Process> priorityProcesses = processes;
        int currentTime = 0;
        double totalWaitingTime = 0;
        double totalTurnaroundTime = 0;

        std::sort(priorityProcesses.begin(), priorityProcesses.end(),
            [](const Process& a, const Process& b) {
                return a.priority < b.priority;
            });

        for (auto& process : priorityProcesses) {
            if (currentTime < process.arrivalTime) {
                currentTime = process.arrivalTime;
            }

            process.waitingTime = currentTime - process.arrivalTime;
            totalWaitingTime += process.waitingTime;

            currentTime += process.burstTime;
            process.completionTime = currentTime;

            int turnaroundTime = process.completionTime - process.arrivalTime;
            totalTurnaroundTime += turnaroundTime;

            std::cout << "Process " << process.id
                << ": Priority = " << process.priority
                << ", Burst Time = " << process.burstTime
                << ", Waiting Time = " << process.waitingTime
                << ", Completion Time = " << process.completionTime
                << std::endl;
        }

        std::cout << BLUE << "\nPriority Scheduling Average Waiting Time: "
            << totalWaitingTime / priorityProcesses.size()
            << std::endl;
        std::cout << "Priority Scheduling Average Turnaround Time: "
            << totalTurnaroundTime / priorityProcesses.size()
            << RESET << std::endl;
    }

    void printProcesses() {
        std::cout << "\n*---- Generated Processes ----*" << std::endl;
        for (const auto& process : processes) {
            std::cout << "Process " << process.id
                << ": Arrival = " << process.arrivalTime
                << ", Burst = " << process.burstTime
                << ", Priority = " << process.priority
                << std::endl;
        }
    }
    void printTable(const std::vector<Process>& processes) {
        std::cout << std::setw(10) << "Process"
            << std::setw(15) << "Arrival Time"
            << std::setw(10) << "Burst Time"
            << std::setw(12) << "Wait Time"
            << std::setw(18) << "Completion Time"
            << std::setw(15) << "Turnaround Time"
            << std::endl;

        for (const auto& process : processes) {
            int turnaroundTime = process.completionTime - process.arrivalTime;
            std::cout << std::setw(10) << process.id
                << std::setw(15) << process.arrivalTime
                << std::setw(10) << process.burstTime
                << std::setw(12) << process.waitingTime
                << std::setw(18) << process.completionTime
                << std::setw(15) << turnaroundTime
                << std::endl;
        }
    }

};



int main() {
    ProcessScheduler scheduler;

    // генерація випадкових процесів, в моєму випадку 7 випадкових процесів
    scheduler.generateRandomProcesses(7);

    scheduler.printProcesses();

    scheduler.simulateFCFS();
    scheduler.simulateRoundRobin();
    scheduler.simulatePriorityScheduling();
    scheduler.simulateSJF();
    scheduler.simulatePrioritySchedulingWithAging();

    return 0;
}
