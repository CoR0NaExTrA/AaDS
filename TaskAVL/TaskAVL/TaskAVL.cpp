#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>

// Структура для узла бинарного дерева поиска
struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;
    BSTNode(int x) : key(x), left(nullptr), right(nullptr) {}
};

// Класс бинарного дерева поиска
class BinarySearchTree {
public:
    BSTNode* root;

    BinarySearchTree() : root(nullptr) {}

    void Insert(int key) {
        root = Insert(root, key);
    }

    std::vector<int> InOrder() {
        std::vector<int> result;
        InOrder(root, result);
        return result;
    }

private:
    BSTNode* Insert(BSTNode* node, int key) {
        if (node == nullptr) {
            return new BSTNode(key);
        }

        BSTNode* current = node;
        while (true) {
            if (key < current->key) {
                if (current->left == nullptr) {
                    current->left = new BSTNode(key);
                    break;
                }
                current = current->left;
            }
            else {
                if (current->right == nullptr) {
                    current->right = new BSTNode(key);
                    break;
                }
                current = current->right;
            }
        }
        return node;
    }

    void InOrder(BSTNode* node, std::vector<int>& result) {
        if (node == nullptr) return;
        InOrder(node->left, result);
        result.push_back(node->key);
        InOrder(node->right, result);
    }
};


// Структура для узла АВЛ-дерева
struct AVLNode {
    int key;
    AVLNode* left;
    AVLNode* right;
    int height;
    AVLNode(int x) : key(x), left(nullptr), right(nullptr), height(1) {}
};

// Класс АВЛ-дерева
class AVLTree {
public:
    AVLNode* root;

    AVLTree() : root(nullptr) {}

    void Insert(int key) {
        root = Insert(root, key);
    }

    std::vector<int> InOrder() {
        std::vector<int> result;
        return result;
    }

private:
    AVLNode* Insert(AVLNode* node, int key) {
        if (node == nullptr) {
            return new AVLNode(key);
        }
        if (key < node->key) {
            node->left = Insert(node->left, key);
        }
        else {
            node->right = Insert(node->right, key);
        }

        node->height = 1 + std::max(GetHeight(node->left), GetHeight(node->right));

        int balance = GetBalance(node);

        if (balance > 1 && key < node->left->key) {
            return RightRotate(node);
        }
        if (balance < -1 && key > node->right->key) {
            return LeftRotate(node);
        }
        if (balance > 1 && key > node->left->key) {
            node->left = LeftRotate(node->left);
            return RightRotate(node);
        }
        if (balance < -1 && key < node->right->key) {
            node->right = RightRotate(node->right);
            return LeftRotate(node);
        }

        return node;
    }

    int GetHeight(AVLNode* node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    int GetBalance(AVLNode* node) {
        if (node == nullptr) return 0;
        return GetHeight(node->left) - GetHeight(node->right);
    }

    AVLNode* RightRotate(AVLNode* y) {
        if (!y || !y->left) return y;

        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = std::max(GetHeight(y->left), GetHeight(y->right)) + 1;
        x->height = std::max(GetHeight(x->left), GetHeight(x->right)) + 1;

        return x;
    }

    AVLNode* LeftRotate(AVLNode* x) {
        if (!x || !x->right) return x;

        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = std::max(GetHeight(x->left), GetHeight(x->right)) + 1;
        y->height = std::max(GetHeight(y->left), GetHeight(y->right)) + 1;

        return y;
    }

    void inorder(AVLNode* node, std::vector<int>& result) {
        if (node == nullptr) return;
        inorder(node->left, result);
        result.push_back(node->key);
        inorder(node->right, result);
    }
};

bool isTimeExceeded(clock_t start, double limitInSeconds) {
    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    return elapsed > limitInSeconds;
}

void testSorting(const std::string& testName, const std::vector<int>& data, BinarySearchTree& bst, AVLTree& avl, double timeLimit) {
    std::cout << "\nTesting " << testName << ":\n";

    clock_t start = clock();
    for (int num : data) {
        bst.Insert(num);
        if (isTimeExceeded(start, timeLimit)) {
            std::cout << "BST insertion exceeded time limit!" << std::endl;
            break;
        }
    }
    clock_t end = clock();
    double elapsed = double(end - start) / CLOCKS_PER_SEC;
    if (elapsed > timeLimit) {
        std::cout << "BST took " << elapsed << " seconds (exceeded time limit).\n";
    }
    else {
        std::cout << "BST completed in " << elapsed << " seconds.\n";
    }

    start = clock();
    for (int num : data) {
        avl.Insert(num);
        if (isTimeExceeded(start, timeLimit)) {
            std::cout << "AVL insertion exceeded time limit!" << std::endl;
            break;
        }
    }
    end = clock();
    elapsed = double(end - start) / CLOCKS_PER_SEC;
    if (elapsed > timeLimit) {
        std::cout << "AVL took " << elapsed << " seconds (exceeded time limit).\n";
    }
    else {
        std::cout << "AVL completed in " << elapsed << " seconds.\n";
    }
}

int main() {
    srand(time(0));

    const int N = 1000000;
    const double timeLimit = 10.0;

    std::vector<int> numbers(N);

    for (int i = 0; i < N; ++i) {
        numbers[i] = rand() % 1000000;
    }

    std::vector<int> sorted_numbers = numbers;
    sort(sorted_numbers.begin(), sorted_numbers.end());
    std::vector<int> reverse_sorted_numbers = sorted_numbers;
    reverse(reverse_sorted_numbers.begin(), reverse_sorted_numbers.end());

    BinarySearchTree bst;
    AVLTree avl;

    testSorting("sorted array", sorted_numbers, bst, avl, timeLimit);

    testSorting("reverse sorted array", reverse_sorted_numbers, bst, avl, timeLimit);

    testSorting("random array", numbers, bst, avl, timeLimit);

    return 0;
}
