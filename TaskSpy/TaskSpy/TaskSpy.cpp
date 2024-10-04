#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <windows.h>

struct Spy {
    std::string name;
    int observationTime;
    int maxWaitTime;
    Spy* next;
};

class SpyQueue {
private:
    Spy* front;
    Spy* rear;
    int elapsedTime;

public:
    SpyQueue() : front(nullptr), rear(nullptr), elapsedTime(0) {}

    void enqueue(const std::string& name, int observationTime, int maxWaitTime) {
        if (observationTime <= 0 || maxWaitTime <= 0) {
            std::cerr << "Ошибка: Время наблюдения и максимальное время ожидания должны быть положительными." << std::endl;
            return;
        }

        Spy* newSpy = new Spy{ name, observationTime, maxWaitTime, nullptr };
        if (rear) {
            rear->next = newSpy;
        }
        else {
            front = newSpy;
        }
        rear = newSpy;
    }

    void processSpies() {
        while (front) {
            elapsedTime++;
            if (front->maxWaitTime < elapsedTime) {
                std::cout << front->name << " покидает очередь (время ожидания истекло).\n";
                Spy* temp = front;
                front = front->next;
                delete temp;
                if (!front) rear = nullptr;
                continue;
            }
            std::cout << front->name << " наблюдает за объектом.\n";
            elapsedTime += front->observationTime;
            Spy* temp = front;
            front = front->next;
            enqueue(temp->name, temp->observationTime, temp->maxWaitTime);
            delete temp;
        }
    }
};

void readDataFromFile(const std::string& filename, SpyQueue& queue) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name;
        int observationTime, maxWaitTime;
        if (iss >> name >> observationTime >> maxWaitTime) {
            if (observationTime <= 0 || maxWaitTime <= 0) {
                std::cerr << "Ошибка в строке '" << line << "': Время наблюдения и максимальное время ожидания должны быть положительными." << std::endl;
                continue;
            }
            queue.enqueue(name, observationTime, maxWaitTime);
        }
        else {
            std::cerr << "Ошибка в строке '" << line << "': Неверный формат данных." << std::endl;
        }
    }
    file.close();
}

int main() {
    setlocale(LC_ALL, "rus");

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    char choice;
    do {
        SpyQueue queue;
        std::string filename;

        std::cout << "Введите имя файла с данными о шпионах: ";
        std::cin >> filename;

        readDataFromFile(filename, queue);

        queue.processSpies();
        std::cout << "Хотите продолжить? (y/n): ";
        std::cin >> choice;
    } while (choice == 'y');

    return 0;
}
