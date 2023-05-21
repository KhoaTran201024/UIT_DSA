#include <iostream>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;

// Cây AVL
struct AVLNode {
    int data;
    AVLNode* left;
    AVLNode* right;
    int height;
};

// Cây Đỏ-Đen
enum Color { RED, BLACK };

struct RBNode {
    int data;
    Color color;
    RBNode* left;
    RBNode* right;
    RBNode* parent;
};

// Hàm tạo nút AVL
AVLNode* createAVLNode(int data) {
    AVLNode* newNode = new AVLNode;
    newNode->data = data;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->height = 1;
    return newNode;
}

// Hàm lấy chiều cao của cây AVL
int getHeight(AVLNode* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

// Hàm lấy giá trị tối đa
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Hàm lấy hiệu số cân bằng của cây AVL
int getBalanceFactor(AVLNode* node) {
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Hàm cân bằng cây AVL sau khi thêm một nút
AVLNode* balanceAVL(AVLNode* node) {
    if (node == nullptr)
        return node;
    
    node->height = 1 + max(getHeight(node->left), getHeight(node->right));

    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1) {
        if (getBalanceFactor(node->left) >= 0) {
            // Trường hợp Left-Left
            AVLNode* newNode = node->left;
            node->left = newNode->right;
            newNode->right = node;
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
            newNode->height = 1 + max(getHeight(newNode->left), getHeight(newNode->right));
            return newNode;
        }
        else {
            // Trường hợp Left-Right
            AVLNode* newNode = node->left;
            node->left = newNode->right;
            newNode->right = node;
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
            newNode->height = 1 + max(getHeight(newNode->left), getHeight(newNode->right));
            node = newNode;
            
            newNode = node->left;
            node->left = newNode->right;
            newNode->right = node;
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
            newNode->height = 1 + max(getHeight(newNode->left), getHeight(newNode->right));
            return newNode;
        }
    }
    else if (balanceFactor < -1) {
        if (getBalanceFactor(node->right) <= 0) {
            // Trường hợp Right-Right
            AVLNode* newNode = node->right;
            node->right = newNode->left;
            newNode->left = node;
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
            newNode->height = 1 + max(getHeight(newNode->left), getHeight(newNode->right));
            return newNode;
        }
        else {
            // Trường hợp Right-Left
            AVLNode* newNode = node->right;
            node->right = newNode->left;
            newNode->left = node;
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
            newNode->height = 1 + max(getHeight(newNode->left), getHeight(newNode->right));
            node = newNode;
            
            newNode = node->right;
            node->right = newNode->left;
            newNode->left = node;
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));
            newNode->height = 1 + max(getHeight(newNode->left), getHeight(newNode->right));
            return newNode;
        }
    }

    return node;
}

// Hàm thêm nút vào cây AVL
AVLNode* insertAVL(AVLNode* root, int data) {
    if (root == nullptr)
        return createAVLNode(data);

    if (data < root->data)
        root->left = insertAVL(root->left, data);
    else if (data > root->data)
        root->right = insertAVL(root->right, data);
    else
        return root;

    root = balanceAVL(root);

    return root;
}

// Hàm tạo nút cây Đỏ-Đen
RBNode* createRBNode(int data) {
    RBNode* newNode = new RBNode;
    newNode->data = data;
    newNode->color = RED;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = nullptr;
    return newNode;
}

// Hàm xoay sang trái trong cây Đỏ-Đen
void rotateLeft(RBNode*& root, RBNode*& node) {
    RBNode* nodeRight = node->right;
    node->right = nodeRight->left;

    if (node->right != nullptr)
        node->right->parent = node;

    nodeRight->parent = node->parent;

    if (node->parent == nullptr)
        root = nodeRight;
    else if (node == node->parent->left)
        node->parent->left = nodeRight;
    else
        node->parent->right = nodeRight;

    nodeRight->left = node;
    node->parent = nodeRight;
}

// Hàm xoay sang phải trong cây Đỏ-Đen
void rotateRight(RBNode*& root, RBNode*& node) {
    RBNode* nodeLeft = node->left;
    node->left = nodeLeft->right;

    if (node->left != nullptr)
        node->left->parent = node;

    nodeLeft->parent = node->parent;

    if (node->parent == nullptr)
        root = nodeLeft;
    else if (node == node->parent->left)
        node->parent->left = nodeLeft;
    else
        node->parent->right = nodeLeft;

    nodeLeft->right = node;
    node->parent = nodeLeft;
}

// Hàm cân bằng cây Đỏ-Đen sau khi thêm một nút
void fixRedRed(RBNode*& root, RBNode*& node) {
    if (node == root) {
        node->color = BLACK;
        return;
    }

    RBNode* parent = node->parent;
    RBNode* grandparent = parent->parent;
    RBNode* uncle = nullptr;

    if (parent == grandparent->left)
        uncle = grandparent->right;
    else
        uncle = grandparent->left;

    if (parent->color != BLACK) {
        if (uncle != nullptr && uncle->color == RED) {
            parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED;
            fixRedRed(root, grandparent);
        }
        else {
            if (parent == grandparent->left && node == parent->right) {
                rotateLeft(root, parent);
                node = parent;
                parent = node->parent;
            }
            else if (parent == grandparent->right && node == parent->left) {
                rotateRight(root, parent);
                node = parent;
                parent = node->parent;
            }

            if (parent == grandparent->left && node == parent->left)
                rotateRight(root, grandparent);
            else
                rotateLeft(root, grandparent);

            parent->color = BLACK;
            grandparent->color = RED;
        }
    }
}

// Hàm thêm nút vào cây Đỏ-Đen
RBNode* insertRB(RBNode*& root, RBNode*& node) {
    if (root == nullptr)
        return node;

    if (node->data < root->data) {
        root->left = insertRB(root->left, node);
        root->left->parent = root;
    }
    else if (node->data > root->data) {
        root->right = insertRB(root->right, node);
        root->right->parent = root;
    }

    return root;
}

// Hàm tạo cây AVL từ dữ liệu
AVLNode* createAVLTree(int data[], int size) {
    AVLNode* root = nullptr;

    for (int i = 0; i < size; i++) {
        root = insertAVL(root, data[i]);
    }

    return root;
}

// Hàm tạo cây Đỏ-Đen từ dữ liệu
RBNode* createRBTree(int data[], int size) {
    RBNode* root = nullptr;

    for (int i = 0; i < size; i++) {
        RBNode* newNode = createRBNode(data[i]);
        root = insertRB(root, newNode);
        fixRedRed(root, newNode);
    }

    root->color = BLACK;
    return root;
}

// Hàm lưu dữ liệu vào file txt
void saveDataToFile(int data[], int size, const string& filename) {
    ofstream file(filename);

    if (file.is_open()) {
        for (int i = 0; i < size; i++) {
            file << data[i] << " ";
        }

        file.close();
    }
    else {
        cout << "Failed to open file: " << filename << endl;
    }
}

// Hàm tạo bộ dữ liệu ngẫu nhiên
void generateRandomData(int data[], int size) {
    srand(time(nullptr));

    for (int i = 0; i < size; i++) {
        data[i] = rand() % 1000000 + 1;
    }
}

int main() {
    const int dataSize = 1000000;
    const int numDataSets = 10;
    const string filenamePrefix = "in";
    const string filenameExtension = ".txt";

    for (int i = 1; i <= numDataSets; i++) {
        int data[dataSize];
        generateRandomData(data, dataSize);
        string filename = filenamePrefix + to_string(i) + filenameExtension;
        saveDataToFile(data, dataSize, filename);
    }

    for (int i = 1; i <= numDataSets; i++) {
        string filename = filenamePrefix + to_string(i) + filenameExtension;
        ifstream file(filename);

        if (file.is_open()) {
            int data[dataSize];
            int index = 0;
            int value;

            while (file >> value && index < dataSize) {
                data[index++] = value;
            }

            file.close();

            AVLNode* avlRoot = createAVLTree(data, dataSize);
            RBNode* rbRoot = createRBTree(data, dataSize);

            // Tính chiều cao của cây AVL và cây Đỏ-Đen
            int avlHeight = getHeight(avlRoot);
            int rbHeight = 0;
            RBNode* rbNode = rbRoot;

            while (rbNode != nullptr) {
                rbHeight++;
                rbNode = rbNode->left;
            }

            // In ra chiều cao của cây AVL và cây Đỏ-Đen
            cout << "Dataset " << i << ":" << endl;
            cout << "AVL Height: " << avlHeight << endl;
            cout << "Red-Black Height: " << rbHeight << endl;
        }
        else {
            cout << "Failed to open file: " << filename << endl;
        }
    }

    return 0;
}
