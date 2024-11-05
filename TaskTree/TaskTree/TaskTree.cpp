#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

struct Node {
    std::string name;
    std::vector<Node*> children;
    Node* parent = nullptr;

    Node(const std::string& name) : name(name) {}

    ~Node() {
        for (auto child : children) {
            delete child;
        }
    }
};

// Функция для добавления дочернего узла
void AddChild(Node* parent, Node* child) {
    child->parent = parent;
    parent->children.push_back(child);
}

// Загрузка дерева из файла
void LoadTreeFromFile(Node* root, const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    std::vector<Node*> node_stack = { root };
    int prev_indent = 0;

    while (std::getline(file, line)) {
        // Определяем количество отступов
        int indent = line.find_first_not_of(" ");
        if (indent == std::string::npos) {
            continue;
        }
        std::string name = line.substr(indent);

        // Создаем новый узел
        Node* new_node = new Node(name);

        // Проверяем корректность уровня вложенности
        if (indent > prev_indent) {
            if (!node_stack.empty()) {
                Node* parent = node_stack.back();
                AddChild(parent, new_node);
            }
        }
        else {
            // Уменьшаем размер `node_stack` до текущего уровня вложенности
            node_stack.resize((indent / 2) + 1);
            Node* parent = node_stack.back();
            AddChild(parent, new_node);
        }

        // Добавляем новый узел в стек
        node_stack.push_back(new_node);
        prev_indent = indent;
    }
}

// Функция проверки наличия `.cpp` файлов в узле или его потомках
bool ContainsCppFiles(const Node* node) {
    if (node->children.empty()) {
        return node->name.find(".cpp") != std::string::npos;
    }

    for (const auto& child : node->children) {
        if (ContainsCppFiles(child)) {
            return true;
        }
    }
    return false;
}

// Рекурсивное копирование узлов, содержащих `.cpp` файлы
Node* FilterCppFolders(const Node* node) {
    if (!ContainsCppFiles(node)) {
        return nullptr;
    }

    Node* new_node = new Node(node->name);
    for (const auto& child : node->children) {
        Node* filtered_child = FilterCppFolders(child);
        if (filtered_child) {
            AddChild(new_node, filtered_child);
        }
    }
    return new_node;
}

// Функция для отображения дерева
void DisplayTree(const Node* node, int depth = 0) {
    if (!node) return;
    std::cout << std::string(depth * 2, ' ') << node->name << std::endl;
    for (const auto& child : node->children) {
        DisplayTree(child, depth + 1);
    }
}

int main() {

    setlocale(LC_ALL, "rus");

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    Node* root = new Node("root");
    LoadTreeFromFile(root, "Tree2.txt");

    std::cout << "Исходное дерево:\n";
    DisplayTree(root);

    // Создаем дерево, содержащее только узлы с `.cpp` файлами
    Node* filtered_root = FilterCppFolders(root);

    std::cout << "\nДерево с папками, содержащими `.cpp` файлы:\n";
    DisplayTree(filtered_root);

    // Удаление деревьев для предотвращения утечек памяти
    delete root;
    delete filtered_root;

    return 0;
}