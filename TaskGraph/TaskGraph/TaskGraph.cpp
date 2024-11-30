//14. Проект  научно - технической  программы  задан  с помощью
//ориентированного графа.Вершина графа соответствует отдельному
//исследованию, а   дуги   показывают  очередность  выполнения
//исследований(каждое  исследование  может  начаться  не  ранее
//окончания   предшествующих   исследований).Продолжительность
//каждого исследования известна.Задан конечный срок выполнения
//проекта.Требуется  определить  максимальный интервал времени
//для проведения каждого исследования, чтобы  проект  мог  быть
//завершен к назначенному сроку(9)
//Трофимов Ярослав ПС-24
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
            std::cerr << "Error: Не удалось открыть файл." << std::endl;
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

    void findMaxIntervals()
    {
        // Шаг 1: Подготовка данных
        std::map<int, int> earlyStart, earlyFinish; // Ранние времена начала и завершения
        std::map<int, int> lateStart, lateFinish;  // Поздние времена начала и завершения
        std::map<int, int> slack;                  // Максимальные интервалы           
        std::map<int, int> inDegree;               // Степень входа для каждой вершины
        
        // Вычисляем степень входа для построения топологического порядка
        for (const auto& [id, task] : tasks)  
        {
            inDegree[id] = 0; // Инициализация
        }
        for (const auto& [id, task] : tasks) 
        {
            for (int dep : task.dependencies) 
            {
                if (tasks.find(dep) == tasks.end()) { // Проверка существования задачи
                    std::cerr << "Ошибка: Задача " << dep << " не существует." << std::endl;
                    return;
                }
                inDegree[id]++; // Увеличиваем степень входа текущей задачи
            }
        }

        std::cout << "Зависимости задач:\n";
        for (const auto& [id, task] : tasks) 
        {
            std::cout << "Задача " << id << ": ";
            for (int dep : task.dependencies) 
            {
                std::cout << dep << " ";
            }
            std::cout << "\n";
        }

        std::cout << "Степени входа:\n";
        for (const auto& [id, degree] : inDegree) 
        {
            std::cout << "Задача " << id << ": " << degree << "\n";
        }

        // Топологическая сортировка
        std::queue<int> q; // Очередь для задач с нулевой степенью входа
        std::vector<int> topoOrder; // Вектор для хранения топологического порядка

        // Инициализация очереди задач без входящих зависимостей
        for (const auto& [id, degree] : inDegree) 
        {
            if (degree == 0) 
            {
                q.push(id);
            }
        }

        std::cout << "Инициализация очереди задач с нулевой степенью входа:" << std::endl;
        std::queue<int> tmpQueue = q;  // Копия очереди для вывода содержимого
        while (!tmpQueue.empty()) 
        {
            std::cout << "Задача в очереди: " << tmpQueue.front() << std::endl;
            tmpQueue.pop();
        }

        // Топологическая сортировка
        while (!q.empty()) 
        {
            int current = q.front();
            q.pop();
            std::cout << "Обрабатываем задачу: " << current << std::endl;
            topoOrder.push_back(current);// Сохраняем задачу в топологическом порядке

            std::cout << "Зависимости для задачи " << current << ": ";
            for (int dep : tasks[current].dependencies) 
            {
                std::cout << dep << " ";
            }
            std::cout << std::endl;

            if (!tasks[current].dependencies.empty()) // Если зависимости есть
            { 
                for (int dep : tasks[current].dependencies) 
                {
                    if (--inDegree[dep] == 0) 
                    {
                        q.push(dep); // Добавляем зависимость в очередь, если степень входа стала 
                    }
                }
            }
            else 
            {
                // Если зависимостей нет, обрабатываем задачу как начальную
                for (const auto& [id, task] : tasks) 
                {
                    if (id != current && inDegree[id] == 1) // Обновляем для задач, где степень входа 1
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
                    std::cout << "Задача " << dep << " теперь добавляется в очередь (степень входа = 0)" << std::endl;
                    q.push(dep);
                }
            }*/
        }

        // Вывод топологического порядка
        std::cout << "Топологический порядок задач:\n";
        for (int task : topoOrder) 
        {
            std::cout << task << " ";
        }
        std::cout << "\n";

        // Шаг 2: Вычисление ранних времён
        for (const auto& [id, task] : tasks) 
        {
            earlyStart[id] = 0; // Все задачи могут начинаться с 0
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

            std::cout << "Вычисление ранних времён:" << std::endl;
            for (int current : topoOrder) 
            {
                std::cout << "Раннее начало для задачи " << current << ": " << earlyStart[current] << std::endl;
            }
        }

        // Шаг 3: Вычисление поздних времён
        for (const auto& [id, task] : tasks) 
        {
            lateFinish[id] = deadline; // Инициализируем поздние времена крайним сроком
        }

        // Идем в обратном порядке топологического сортированного графа
        for (auto it = tasks.rbegin(); it != tasks.rend(); ++it) 
        {
            int current = it->first;
            lateStart[current] = lateFinish[current] - tasks[current].duration;
            for (int dep : tasks[current].dependencies) 
            {
                lateFinish[dep] = std::min(lateFinish[dep], lateStart[current]);
            }
        }

        std::cout << "Вычисление поздних времён:" << std::endl;
        for (int current : topoOrder) 
        {
            std::cout << "Позднее окончание для задачи " << current << ": " << lateFinish[current] << std::endl;
        }

        // Шаг 4: Вычисление максимальных интервалов
        for (const auto& [id, task] : tasks) 
        {
            slack[id] = lateStart[id] - earlyStart[id];
        }

        std::cout << "Максимальные интервалы:" << std::endl;
        for (int current : topoOrder) 
        {
            std::cout << "Задача " << current << ": Максимальный интервал = " << slack[current] << std::endl;
        }

        // Вывод результатов
        std::cout << "Результаты анализа:\n";
        for (const auto& [id, task] : tasks) 
        {
            std::cout << "Задача " << id << ": "
            << "Раннее начало: " << earlyStart[id] << ", "
            << "Позднее окончание: " << lateFinish[id] << ", "
            << "Максимальный интервал: " << slack[id] << "\n";
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
        std::cout << "\nНахождение максимальных интервалов для задач...\n";
        project.findMaxIntervals();

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
