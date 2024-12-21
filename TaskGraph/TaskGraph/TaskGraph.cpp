//14. ������  ������ - �����������  ���������  �����  � �������
//���������������� �����.������� ����� ������������� ����������
//������������, �   ����   ����������  �����������  ����������
//������������(������  ������������  �����  ��������  ��  �����
//���������   ��������������   ������������).�����������������
//������� ������������ ��������.����� �������� ���� ����������
//�������.���������  ����������  ������������ �������� �������
//��� ���������� ������� ������������, �����  ������  ���  ����
//�������� � ������������ �����(9).
//�������� ������� ��-24
//Visual Studio 2022
//

#include <iostream>
#include <fstream>
#include <queue>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <Windows.h>
#undef max
#undef min

struct Task {
    int duration;
    std::vector<int> dependencies;
};

class Project {
private:
    std::map<int, Task> tasks;
    int deadline;

public:
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: �� ������� ������� ����." << std::endl;
            return;
        }

        int numTasks;
        file >> numTasks >> deadline;

        for (int i = 0; i < numTasks; ++i) {
            int id, duration, numDeps;
            file >> id >> duration >> numDeps;

            Task task;
            task.duration = duration;

            for (int j = 0; j < numDeps; ++j) {
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

    void findIntervals() {
        std::map<int, int> earlyStart, earlyFinish;
        std::map<int, int> lateStart, lateFinish;

        // ��� 1: �������������� ����������
        std::map<int, int> inDegree;
        for (const auto& [id, task] : tasks) {
            inDegree[id] = 0; // ������������� inDegree
        }
        for (const auto& [id, task] : tasks) {
            for (int dep : task.dependencies) {
                inDegree[id]++; // ����������� inDegree ��� ������� ������
            }
        }

        std::queue<int> q;
        for (const auto& [id, degree] : inDegree) {
            if (degree == 0) {
                q.push(id); // ��������� � ������� ������ ��� �������� ����
            }
        }

        std::vector<int> topoOrder;
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            topoOrder.push_back(current);

            // �������� �� �������, ������� ������� �� �������
            for (const auto& [id, task] : tasks) {
                // ���� ������ ������� �� �������
                if (std::find(task.dependencies.begin(), task.dependencies.end(), current) != task.dependencies.end()) {
                    if (--inDegree[id] == 0) {
                        q.push(id); // ��������� ������ ���� ��� ����������� ����������
                    }
                }
            }
        }

        if (topoOrder.size() != tasks.size()) {
            std::cerr << "������: ���� �������� ����." << std::endl;
            return;
        }

        // ��� 2: ���������� ������� ������ (Early Start)
        for (int id : topoOrder) {
            earlyStart[id] = 0;
            for (int dep : tasks[id].dependencies) {
                earlyStart[id] = std::max(earlyStart[id], earlyFinish[dep]);
            }
            earlyFinish[id] = earlyStart[id] + tasks[id].duration;
        }

        // ��� 3: ������������� �������� ������� (Late Finish)
        std::map<int, std::vector<int>> dependents; // �������� �����������
        for (const auto& [id, task] : tasks) {
            for (int dep : task.dependencies) {
                dependents[dep].push_back(id);
            }
        }

        for (const auto& [id, task] : tasks) {
            lateFinish[id] = deadline; // ������������� �������� ��������� ��� �������� �����
        }

        // ��� 4: �������� ������ ��� �������� ������� (Late Finish)
        for (auto it = topoOrder.rbegin(); it != topoOrder.rend(); ++it) {
            int id = *it;

            if (!dependents[id].empty()) { // ���� ���� ��������� ������
                lateFinish[id] = INT_MAX;
                for (int dependent : dependents[id]) {
                    lateFinish[id] = std::min(lateFinish[id], lateStart[dependent]);
                }
            }

            lateStart[id] = lateFinish[id] - tasks[id].duration;
            if (lateStart[id] < 0) {
                lateStart[id] = 0; // �������������� ������������� ��������
            }
        }

        // ��� 5: ����� �����������
        std::cout << "���������� �������:\n";
        for (const auto& [id, task] : tasks) {
            std::cout << "������ " << id << ": "
                << "������ ������: " << earlyStart[id] << ", "
                << "������� ���������: " << lateFinish[id] << "\n";
        }

        // ��������� ����� ��� �����������
        std::cout << "\n������ ����������:\n";
        for (int id : topoOrder) {
            std::cout << "������ " << id
                << ": ������ ������ = " << earlyStart[id]
                << ", ������ ��������� = " << earlyFinish[id]
                << ", ������� ������ = " << lateStart[id]
                << ", ������� ��������� = " << lateFinish[id] << "\n";
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
        std::cout << "\n���������� ������� ������ � �������� ��������� �����...\n";
        project.findIntervals();

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

