//14. Проект  научно - технической  программы  задан  с помощью
//ориентированного графа.Вершина графа соответствует отдельному
//исследованию, а   дуги   показывают  очередность  выполнения
//исследований(каждое  исследование  может  начаться  не  ранее
//окончания   предшествующих   исследований).Продолжительность
//каждого исследования известна.Задан конечный срок выполнения
//проекта.Требуется  определить  максимальный интервал времени
//для проведения каждого исследования, чтобы  проект  мог  быть
//завершен к назначенному сроку(9).
//Трофимов Ярослав ПС-24
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
            std::cerr << "Error: Не удалось открыть файл." << std::endl;
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
        std::cout << "Дедлайн: " << deadline << "\n";
        std::cout << "Задачи и их зависимости:\n";
        for (const auto& [id, task] : tasks) {
            std::cout << "Задача " << id << " (Длительность: " << task.duration << "): Зависимости: ";
            if (task.dependencies.empty()) {
                std::cout << "Нет";
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

        // Шаг 1: Топологическая сортировка
        std::map<int, int> inDegree;
        for (const auto& [id, task] : tasks) {
            inDegree[id] = 0; // Инициализация inDegree
        }
        for (const auto& [id, task] : tasks) {
            for (int dep : task.dependencies) {
                inDegree[id]++; // Увеличиваем inDegree для текущей задачи
            }
        }

        std::queue<int> q;
        for (const auto& [id, degree] : inDegree) {
            if (degree == 0) {
                q.push(id); // Добавляем в очередь задачи без входящих рёбер
            }
        }

        std::vector<int> topoOrder;
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            topoOrder.push_back(current);

            // Проходим по задачам, которые зависят от текущей
            for (const auto& [id, task] : tasks) {
                // Если задача зависит от текущей
                if (std::find(task.dependencies.begin(), task.dependencies.end(), current) != task.dependencies.end()) {
                    if (--inDegree[id] == 0) {
                        q.push(id); // Добавляем только если все зависимости обработаны
                    }
                }
            }
        }

        if (topoOrder.size() != tasks.size()) {
            std::cerr << "Ошибка: Граф содержит цикл." << std::endl;
            return;
        }

        // Шаг 2: Вычисление раннего старта (Early Start)
        for (int id : topoOrder) {
            earlyStart[id] = 0;
            for (int dep : tasks[id].dependencies) {
                earlyStart[id] = std::max(earlyStart[id], earlyFinish[dep]);
            }
            earlyFinish[id] = earlyStart[id] + tasks[id].duration;
        }

        // Шаг 3: Инициализация позднего времени (Late Finish)
        std::map<int, std::vector<int>> dependents; // Обратные зависимости
        for (const auto& [id, task] : tasks) {
            for (int dep : task.dependencies) {
                dependents[dep].push_back(id);
            }
        }

        for (const auto& [id, task] : tasks) {
            lateFinish[id] = deadline; // Инициализация позднего окончания для конечных задач
        }

        // Шаг 4: Обратный проход для позднего времени (Late Finish)
        for (auto it = topoOrder.rbegin(); it != topoOrder.rend(); ++it) {
            int id = *it;

            if (!dependents[id].empty()) { // Если есть зависимые задачи
                lateFinish[id] = INT_MAX;
                for (int dependent : dependents[id]) {
                    lateFinish[id] = std::min(lateFinish[id], lateStart[dependent]);
                }
            }

            lateStart[id] = lateFinish[id] - tasks[id].duration;
            if (lateStart[id] < 0) {
                lateStart[id] = 0; // Предотвращение отрицательных значений
            }
        }

        // Шаг 5: Вывод результатов
        std::cout << "Результаты анализа:\n";
        for (const auto& [id, task] : tasks) {
            std::cout << "Задача " << id << ": "
                << "Раннее начало: " << earlyStart[id] << ", "
                << "Позднее окончание: " << lateFinish[id] << "\n";
        }

        // Пошаговый вывод для диагностики
        std::cout << "\nДетали вычислений:\n";
        for (int id : topoOrder) {
            std::cout << "Задача " << id
                << ": Раннее начало = " << earlyStart[id]
                << ", Раннее окончание = " << earlyFinish[id]
                << ", Позднее начало = " << lateStart[id]
                << ", Позднее окончание = " << lateFinish[id] << "\n";
        }
    }
};

int main() {
    setlocale(LC_ALL, "rus");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Project project;

    std::string filename;
    std::cout << "Введите имя файла с проектом: ";
    std::cin >> filename;

    project.loadFromFile(filename);
    project.printTasks();

    char choice;
    do {
        std::cout << "\nНахождение раннего старта и позднего окончания задач...\n";
        project.findIntervals();

        std::cout << "\nХотите ли вы проанализировать другой проект? (y/n): ";
        std::cin >> choice;

        if (choice == 'y' || choice == 'Y') {
            std::cout << "Введите имя файла с проектом: ";
            std::cin >> filename;
            project.loadFromFile(filename);
        }

    } while (choice == 'y' || choice == 'Y');

    return 0;
}

