//14. ������  ������ - �����������  ���������  �����  � �������
//���������������� �����.������� ����� ������������� ����������
//������������, �   ����   ����������  �����������  ����������
//������������(������  ������������  �����  ��������  ��  �����
//���������   ��������������   ������������).�����������������
//������� ������������ ��������.����� �������� ���� ����������
//�������.���������  ����������  ������������ �������� �������
//��� ���������� ������� ������������, �����  ������  ���  ����
//�������� � ������������ �����(9)
//�������� ������� ��-24
//Visual Studio 2022
//

#include <iostream>
#include <fstream>
#include <queue>
#include <limits>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <Windows.h>
#include <unordered_map>
#undef max
#undef min

struct Task 
{
    int duration;
    std::vector<int> dependencies;
};

class Project 
{
private:
    std::map<int, Task> tasks;
    int deadline;

public:
    void loadFromFile(const std::string& filename) 
    {
        std::ifstream file(filename);
        if (!file.is_open()) 
        {
            std::cerr << "Error: �� ������� ������� ����." << std::endl;
            return;
        }

        int numTasks;
        file >> numTasks >> deadline;

        for (int i = 0; i < numTasks; ++i) 
        {
            int id, duration, numDeps;
            file >> id >> duration >> numDeps;

            Task task;
            task.duration = duration;

            for (int j = 0; j < numDeps; ++j) 
            {
                int dep;
                file >> dep;
                task.dependencies.push_back(dep);
            }

            tasks[id] = task;
        }

        file.close();
    }

    void printTasks() {
        std::cout << "�������: " << deadline << "\n";
        std::cout << "������ � �� �����������:\n";
        for (const auto& [id, task] : tasks) {
            std::cout << "������ " << id << " (������������: " << task.duration << "): �����������: ";
            if (task.dependencies.empty()) {
                std::cout << "���";
            }
            else {
                for (int dep : task.dependencies) {
                    std::cout << dep << " ";
                }
            }
            std::cout << "\n";
        }
    }

    void findMaxIntervals()
    {
        // ��� 1: ���������� ������
        std::map<int, int> earlyStart, earlyFinish; // ������ ������� ������ � ����������
        std::map<int, int> lateStart, lateFinish;  // ������� ������� ������ � ����������
        std::map<int, int> slack;                  // ������������ ���������           
        std::map<int, int> inDegree;               // ������� ����� ��� ������ �������
        
        // ��������� ������� ����� ��� ���������� ��������������� �������
        for (const auto& [id, task] : tasks)  
        {
            inDegree[id] = 0; // �������������
        }
        for (const auto& [id, task] : tasks) 
        {
            for (int dep : task.dependencies) 
            {
                if (tasks.find(dep) == tasks.end()) { // �������� ������������� ������
                    std::cerr << "������: ������ " << dep << " �� ����������." << std::endl;
                    return;
                }
                inDegree[id]++; // ����������� ������� ����� ������� ������
            }
        }

        std::cout << "����������� �����:\n";
        for (const auto& [id, task] : tasks) 
        {
            std::cout << "������ " << id << ": ";
            for (int dep : task.dependencies) 
            {
                std::cout << dep << " ";
            }
            std::cout << "\n";
        }

        std::cout << "������� �����:\n";
        for (const auto& [id, degree] : inDegree) 
        {
            std::cout << "������ " << id << ": " << degree << "\n";
        }

        // �������������� ����������
        std::queue<int> q; // ������� ��� ����� � ������� �������� �����
        std::vector<int> topoOrder; // ������ ��� �������� ��������������� �������

        // ������������� ������� ����� ��� �������� ������������
        for (const auto& [id, degree] : inDegree) 
        {
            if (degree == 0) 
            {
                q.push(id);
            }
        }

        std::cout << "������������� ������� ����� � ������� �������� �����:" << std::endl;
        std::queue<int> tmpQueue = q;  // ����� ������� ��� ������ �����������
        while (!tmpQueue.empty()) 
        {
            std::cout << "������ � �������: " << tmpQueue.front() << std::endl;
            tmpQueue.pop();
        }

        // �������������� ����������
        while (!q.empty()) 
        {
            int current = q.front();
            q.pop();
            std::cout << "������������ ������: " << current << std::endl;
            topoOrder.push_back(current);// ��������� ������ � �������������� �������

            std::cout << "����������� ��� ������ " << current << ": ";
            for (int dep : tasks[current].dependencies) 
            {
                std::cout << dep << " ";
            }
            std::cout << std::endl;

            if (!tasks[current].dependencies.empty()) // ���� ����������� ����
            { 
                for (int dep : tasks[current].dependencies) 
                {
                    if (--inDegree[dep] == 0) 
                    {
                        q.push(dep); // ��������� ����������� � �������, ���� ������� ����� ����� 
                    }
                }
            }
            else 
            {
                // ���� ������������ ���, ������������ ������ ��� ���������
                for (const auto& [id, task] : tasks) 
                {
                    if (id != current && inDegree[id] == 1) // ��������� ��� �����, ��� ������� ����� 1
                    { 
                        if (--inDegree[id] == 0) 
                        {
                            q.push(id);
                        }
                    }
                }
            }

            /*for (int dep : tasks[current].dependencies) 
            {
                inDegree[dep]--;
                if (inDegree[dep] == 0) {
                    std::cout << "������ " << dep << " ������ ����������� � ������� (������� ����� = 0)" << std::endl;
                    q.push(dep);
                }
            }*/
        }

        // ����� ��������������� �������
        std::cout << "�������������� ������� �����:\n";
        for (int task : topoOrder) 
        {
            std::cout << task << " ";
        }
        std::cout << "\n";

        // ��� 2: ���������� ������ �����
        for (const auto& [id, task] : tasks) 
        {
            earlyStart[id] = 0; // ��� ������ ����� ���������� � 0
        }
        while (!topoOrder.empty()) 
        {
            int current = topoOrder.front();
            topoOrder.erase(topoOrder.begin());
            earlyFinish[current] = earlyStart[current] + tasks[current].duration;

            for (int dep : tasks[current].dependencies) 
            {
                earlyStart[dep] = std::max(earlyStart[dep], earlyFinish[current]);
            }

            std::cout << "���������� ������ �����:" << std::endl;
            for (int current : topoOrder) 
            {
                std::cout << "������ ������ ��� ������ " << current << ": " << earlyStart[current] << std::endl;
            }
        }

        // ��� 3: ���������� ������� �����
        for (const auto& [id, task] : tasks) 
        {
            lateFinish[id] = deadline; // �������������� ������� ������� ������� ������
        }

        // ���� � �������� ������� ��������������� �������������� �����
        for (auto it = tasks.rbegin(); it != tasks.rend(); ++it) 
        {
            int current = it->first;
            lateStart[current] = lateFinish[current] - tasks[current].duration;
            for (int dep : tasks[current].dependencies) 
            {
                lateFinish[dep] = std::min(lateFinish[dep], lateStart[current]);
            }
        }

        std::cout << "���������� ������� �����:" << std::endl;
        for (int current : topoOrder) 
        {
            std::cout << "������� ��������� ��� ������ " << current << ": " << lateFinish[current] << std::endl;
        }

        // ��� 4: ���������� ������������ ����������
        for (const auto& [id, task] : tasks) 
        {
            slack[id] = lateStart[id] - earlyStart[id];
        }

        std::cout << "������������ ���������:" << std::endl;
        for (int current : topoOrder) 
        {
            std::cout << "������ " << current << ": ������������ �������� = " << slack[current] << std::endl;
        }

        // ����� �����������
        std::cout << "���������� �������:\n";
        for (const auto& [id, task] : tasks) 
        {
            std::cout << "������ " << id << ": "
            << "������ ������: " << earlyStart[id] << ", "
            << "������� ���������: " << lateFinish[id] << ", "
            << "������������ ��������: " << slack[id] << "\n";
        }
    }
};

int main() {
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Project project;

    std::string filename;
    std::cout << "������� ��� ����� � ��������: ";
    std::cin >> filename;

    project.loadFromFile(filename);
    project.printTasks();

    char choice;
    do {
        std::cout << "\n���������� ������������ ���������� ��� �����...\n";
        project.findMaxIntervals();

        std::cout << "\n������ �� �� ���������������� ������ ������? (y/n): ";
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            std::cout << "������� ��� ����� � ��������: ";
            std::cin >> filename;
            project.loadFromFile(filename);
        }

    } while (choice == 'y' || choice == 'Y');

    return 0;
}
