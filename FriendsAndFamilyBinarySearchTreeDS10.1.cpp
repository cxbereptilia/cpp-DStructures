#include <iostream>
#include <fstream>
#include <string>

// Clase para representar una fecha
class Date {
public:
    Date(int month = 1, int day = 1, int year = 1900) : month(month), day(day), year(year) {}
    int getMonth() const { return month; }
    int getDay() const { return day; }
    int getYear() const { return year; }
private:
    int month;
    int day;
    int year;
};

// Clase para representar información personal
class PersonInfo {
public:
    PersonInfo(const std::string& name = "", const Date& birthday = Date()) : name(name), birthday(birthday) {}
    const std::string& getName() const { return name; }
    const Date& getBirthday() const { return birthday; }
private:
    std::string name;
    Date birthday;
};

// Estructura del árbol binario de búsqueda
class BinarySearchTree {
private:
    struct TreeNode {
        PersonInfo person;
        TreeNode* left;
        TreeNode* right;
        TreeNode(const PersonInfo& person) : person(person), left(nullptr), right(nullptr) {}
    };
    TreeNode* root;

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { clear(root); }

    // Funciones para insertar, buscar y eliminar personas
    void insert(const PersonInfo& person) {
        root = insertRec(root, person);
    }

    bool search(const std::string& name, PersonInfo& result) const {
        return searchRec(root, name, result);
    }

    void remove(const std::string& name) {
        root = removeRec(root, name);
    }

    // Función para recorrer el árbol en orden y mostrar la información
    void inOrderTraversal() const {
        inOrderTraversalRec(root);
    }

    // Función para listar personas nacidas en un mes específico
    void listByBirthMonth(int month) const {
        listByBirthMonthRec(root, month);
    }

    // Public interface to save the tree to a file
    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (file.is_open()) {
            saveToFileRec(root, file);
            file.close();
        }
    }

    // Acceso a la raíz para guardar/cargar
    TreeNode* getRoot() const { return root; }

private:
    // Funciones recursivas para soporte de operaciones del BST
    TreeNode* insertRec(TreeNode* root, const PersonInfo& person) {
        if (!root) return new TreeNode(person);

        if (person.getName() < root->person.getName()) {
            root->left = insertRec(root->left, person);
        }
        else if (person.getName() > root->person.getName()) {
            root->right = insertRec(root->right, person);
        }

        return root;
    }

    bool searchRec(TreeNode* root, const std::string& name, PersonInfo& result) const {
        if (!root) return false;

        if (name == root->person.getName()) {
            result = root->person;
            return true;
        }

        if (name < root->person.getName()) {
            return searchRec(root->left, name, result);
        }
        else {
            return searchRec(root->right, name, result);
        }
    }

    TreeNode* removeRec(TreeNode* root, const std::string& name) {
        if (!root) return root;

        if (name < root->person.getName()) {
            root->left = removeRec(root->left, name);
        }
        else if (name > root->person.getName()) {
            root->right = removeRec(root->right, name);
        }
        else {
            if (!root->left) {
                TreeNode* temp = root->right;
                delete root;
                return temp;
            }
            else if (!root->right) {
                TreeNode* temp = root->left;
                delete root;
                return temp;
            }
            TreeNode* temp = findMin(root->right);
            root->person = temp->person;
            root->right = removeRec(root->right, temp->person.getName());
        }
        return root;
    }

    TreeNode* findMin(TreeNode* root) const {
        while (root->left) root = root->left;
        return root;
    }

    void clear(TreeNode* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

    void inOrderTraversalRec(TreeNode* root) const {
        if (root) {
            inOrderTraversalRec(root->left);
            std::cout << "Name: " << root->person.getName() << ", Birthday: " << root->person.getBirthday().getMonth() << "/"
                << root->person.getBirthday().getDay() << "/" << root->person.getBirthday().getYear() << std::endl;
            inOrderTraversalRec(root->right);
        }
    }

    void listByBirthMonthRec(TreeNode* node, int month) const {
        if (node) {
            listByBirthMonthRec(node->left, month);
            if (node->person.getBirthday().getMonth() == month) {
                std::cout << "Name: " << node->person.getName() << ", Birthday: "
                    << node->person.getBirthday().getMonth() << "/"
                    << node->person.getBirthday().getDay() << "/"
                    << node->person.getBirthday().getYear() << std::endl;
            }
            listByBirthMonthRec(node->right, month);
        }
    }

    // Private utility function used by the public save interface
    void saveToFileRec(TreeNode* node, std::ofstream& file) const {
        if (node) {
            saveToFileRec(node->left, file);
            file << node->person.getName() << " "
                << node->person.getBirthday().getMonth() << " "
                << node->person.getBirthday().getDay() << " "
                << node->person.getBirthday().getYear() << std::endl;
            saveToFileRec(node->right, file);
        }
    }
};

// Clase DB para manejar la base de datos de personas
class DB {
private:
    BinarySearchTree bst;

public:
    void insertPerson() {
        std::string name;
        int month, day, year;

        std::cout << "Enter name: ";
        std::cin >> name;
        std::cout << "Enter birthday (MM DD YYYY): ";
        std::cin >> month >> day >> year;

        Date birthday(month, day, year);
        PersonInfo person(name, birthday);
        bst.insert(person);
    }

    void removePerson() {
        std::string name;
        std::cout << "Enter name to remove: ";
        std::cin >> name;
        bst.remove(name);
    }

    // Muestra los cumpleaños en orden
    void showBirthdays() {
        std::cout << "Birthdays:" << std::endl;
        bst.inOrderTraversal();
    }

    // Guarda la base de datos en un archivo
    void saveToFile(const std::string& filename) {
        bst.saveToFile(filename); // Directly use BST's functionality
    }


    // Carga la base de datos desde un archivo
    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string name;
        int month, day, year;
        while (file >> name >> month >> day >> year) {
            Date birthday(month, day, year);
            PersonInfo person(name, birthday);
            bst.insert(person);
        }
    }

};

// Muestra el menú de opciones y maneja la entrada del usuario
void displayMenu(DB& database) {
    std::cout << "Options Menu:" << std::endl;
    std::cout << "1. Enter person's data" << std::endl;
    std::cout << "2. Remove person's data" << std::endl;
    std::cout << "3. Show the list of birthdays" << std::endl;
    std::cout << "4. Save data to file" << std::endl; // Opción para guardar datos
    std::cout << "5. Load data from file" << std::endl; // Opción para cargar datos
    std::cout << "6. Quit the program" << std::endl;
}

int main() {
    DB database;
    std::string filename = "database.txt"; // Nombre del archivo para guardar/cargar

    int choice;

    do {
        displayMenu(database);

        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            database.insertPerson();
            break;
        case 2:
            database.removePerson();
            break;
        case 3:
            database.showBirthdays();
            break;
        case 4:
            database.saveToFile(filename); // Guardar en archivo
            break;
        case 5:
            database.loadFromFile(filename); // Cargar desde archivo
            break;
        case 6:
            std::cout << "Exiting the program." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }

    } while (choice != 6);

    return 0;
}
