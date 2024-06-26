#include <iostream>
#include <string>
#include <fstream>

using namespace std;

extern "C"{
    string typicalPath = "/Users/tim_bzz/Documents/projects/Clion/Paradigms/Assignment4/";
    const int ASCII_A = 65;
    const int ASCII_Z = 90;
    const int ASCII_a = 97;
    const int ALPHABET_SIZE = 26;
    const char SKIP_SYMBOLS[] = {' ', '\n', '\t', '.', ',', '!', '?', ':', ';', '\'', '\"', '(', ')', '[', ']', '{', '}', '<', '>', '/', '\\', '|', '@', '#', '$', '%', '^', '&', '*', '-', '+', '=', '~', '`', '_', 39};


    bool is_skip_symbol(char c){
        for (int i = 0; i < sizeof(SKIP_SYMBOLS); i++){
            if (c == SKIP_SYMBOLS[i]){
                return true;
            }
        }
        return false;
    }

    void encrypt(char* text, int key, char* encr){
        key = key % ALPHABET_SIZE;
        for (int i = 0; text[i] != '\0'; i++) {
            bool is_number = text[i] >= '0' && text[i] <= '9';
            bool is_upper = text[i] >= ASCII_A && text[i] <= ASCII_Z;
            bool is_lower = text[i] >= ASCII_a && text[i] <= ASCII_a + ALPHABET_SIZE;
            if (is_upper){
                encr[i] = ((text[i] + key - ASCII_A) % ALPHABET_SIZE) + ASCII_A;
            }
            else if (is_lower){
                encr[i] = ((text[i] + key - ASCII_a) % ALPHABET_SIZE) + ASCII_a;
            }
            else if (is_number){
                encr[i] = ((text[i] + key - '0') % 10) + '0';
            }
            else if (is_skip_symbol(text[i])){
                encr[i] = text[i];
            }
            else {
                encr[i] = '?';
            }
        }
        encr[strlen(text)] = '\0';
    }

    void decrypt(char* text, int key, char* encr){
        key = key % ALPHABET_SIZE;
        for (int i = 0; text[i] != '\0'; i++) {
            bool is_number = text[i] >= '0' && text[i] <= '9';
            bool is_upper = text[i] >= ASCII_A && text[i] <= ASCII_Z;
            bool is_lower = text[i] >= ASCII_a && text[i] <= ASCII_a + ALPHABET_SIZE;
            if (is_upper){
                encr[i] = ((text[i] - key - ASCII_A + ALPHABET_SIZE) % ALPHABET_SIZE) + ASCII_A;
            }
            else if (is_lower){
                encr[i] = ((text[i] - key - ASCII_a + ALPHABET_SIZE) % ALPHABET_SIZE) + ASCII_a;
            }
            else if (is_number){
                encr[i] = ((text[i] - key - '0' + 10) % 10) + '0';
            }
            else if (is_skip_symbol(text[i])){
                encr[i] = text[i];
            }
            else {
                encr[i] = '?';
            }
        }
        encr[strlen(text)] = '\0';
    }

    int keyChecker(){
        int key;
        bool isRunning = true;
        do {
            cout << "Please, enter the key: ";
            cin >> key;
            cin.ignore();
            if (key >= 0) {
                isRunning = false;
            } else {
                cout << "I can not encrypt with negative key. Try again \n" << endl;
            }
        } while (isRunning);
        cout << "The key is set!\n";
        return key;
    }


    char* PathFinder(string text, int mode = 0) {
        string tempPath;
        CheckPoint:
        cout << text;
        cin >> tempPath;
         if (mode == 0) {
            ifstream file(tempPath);
            if (!file.is_open()) {
                tempPath = typicalPath + tempPath;
                file.open(tempPath);
                if (!file.is_open()) {
                    cout << "File not found\n";
                    cout << tempPath;
                    goto CheckPoint;
                }
            } else {
                cout << "File found!\n";
            }
            file.close();
        }
        char* cstr = new char[tempPath.length() + 1];
        strcpy(cstr, tempPath.c_str());
        return cstr;
    }

    void filer(int mode = 0){
        string originPath = PathFinder("Enter the path to the file you want to encrypt: \n");

        string endPath;
        do{
            endPath = PathFinder("Enter the path to the file in which you want to save encrypted text: \n", 1);
            if (originPath == endPath)
                cout << "You can not encrypt the file in the same file. Try again\n";
        }while (originPath == endPath);
        int key = keyChecker();
        ifstream file(originPath);
        ofstream outFile(endPath, ios::trunc);
        string text;
        string encrText;

        while (!file.eof()) {
            getline(file, text);
            char* cstr = new char[text.length() + 1];
            strcpy(cstr, text.c_str()); // c_str() returns a pointer to an array
            char* encr = new char[text.length() + 1];
            if (mode == 0) {
                encrypt(cstr, key, encr);
            }
            else if (mode == 1){
                decrypt(cstr, key, encr);
            }
            outFile << encr << endl;
            delete[] cstr;
            delete[] encr;
        }
        file.close();
        outFile.close();

        cout << "The file is "<< (mode == 1 ? "de": "en") <<"crypted!\n";
    }
}