#include <stack>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <string>

using namespace std;

#define MAX_STRING_SIZE 1024

struct MyStringHAHAIDIDIT {
    char* data;
    long lineNumber;
};


class textEditor;
class textNode;
class Coursor;
class EncryptConnector;

class textNode {
public:

    char* data;
    textNode *next;

    textNode(const char* text) {
        this->data = new char[strlen(text) + 1];
        strcpy(this->data, text);
        this->next = nullptr;
    }

//    ~textNode() { // destructor to free memory
//        delete[] this->data;
//    }

};

class Coursor{
public:
    int currentLine =  1;
    int currentIndex = 1;
    int maxLines;
    textNode* text;

    void Update(int max, textNode* newText){
        text = newText;
        maxLines = max;
    }

    void pointVerticalMove(int direction, int positions[]){
        currentLine = (currentLine + direction) > maxLines ? maxLines :
                      (currentLine + direction) <= 1 ? 1 : currentLine + direction;
        textNode* temp = text;
        for (int i = 1; i < currentLine; i++)
            temp = temp->next;
        if (currentIndex > strlen(temp->data)) {
            currentIndex = strlen(temp->data) > 0 ? strlen(temp->data)+1 : 1;
        }
        if (positions[0] == positions[1] && positions[0] == -1) {
            pointVisualisator();
        }
        else {
            pointHVisualisator(positions);
        }
    }

    void pointHorizontalMove(int direction, int positions[]){
        textNode *temp = text;
        temp = temp->next;
        for (int i = 1; i < currentLine; i++)
            temp = temp->next;

        if (currentIndex + direction > strlen(temp->data) + 1){
            if (currentLine < maxLines){
                currentIndex = 1;
                currentLine++;
            }
        }
        else if (currentIndex + direction < 1){
            if (currentLine > 1){
                currentLine--;
                textNode *temp = text;
                temp = temp->next;
                for (int i = 1; i < currentLine-1; i++)
                    temp = temp->next;
                currentIndex = strlen(temp->data) + 1;
            }
            else if (currentLine == 1){
                currentIndex = 1;
            }
        } else {
            currentIndex += direction;
        }
        if (positions[0] == positions[1] && positions[0] == -1) {
            pointVisualisator();
        }
        else {
            pointHVisualisator(positions);
        }
    }

    void pointVisualisator(){
        textNode *temp = text;
        temp = temp->next;
        for (int i = 1; i < currentLine-1; i++)
            temp = temp->next;

        if (currentLine != 1){
            cout << currentLine-1 << ". " << temp->data << endl;
            temp = temp->next;
        }

        cout << currentLine << ". ";

        for (int i = 1; i <= strlen(temp->data); i++) {
            if (i == currentIndex) {
                cout << "|";
            }
            cout << temp->data[i - 1];
        }
        if (currentIndex >= strlen(temp->data) + 1) {
            cout << "|";
        }
        cout << endl;

        if (currentLine != maxLines){
            temp = temp->next;
            cout << currentLine+1 << ". " << temp->data << endl;
        }
    }

    void pointHVisualisator(int positions[]){
        cout << positions[0] << " " << positions[1] << endl;
        textNode *temp = text;
        temp = temp->next;
        for (int i = 1; i < positions[0]-1; i++)
            temp = temp->next;

        if (positions[0] != 1){
            cout << positions[0]-1 << ". " << temp->data << endl;
            temp = temp->next;
        }
        for (int j = positions[0]; j <= currentLine; j++) {
            cout << j << ". ";
            for (int i = 1; i <= strlen(temp->data); i++) {
                if ((j == positions[0] && i == positions[1]) || (j == currentLine && i == currentIndex)) {
                    cout << "|";
                }
                if (j <= currentLine && j >= positions[0] && ((j == positions[0] && i >= positions[1]) || (j > positions[0] && j < currentLine) || (j == currentLine && i < currentIndex))) {
                    cout << "\033[1;31m" << temp->data[i - 1] << "\033[0m";
                }
                else {
                    cout << temp->data[i - 1];
                }
            }

            if (currentIndex >= strlen(temp->data) + 1 && j == currentLine) {
                cout << "|";
            }
            cout << endl;
            temp = temp->next;
        }
        if (currentLine != maxLines){
            cout << currentLine+1 << ". " << temp->data << endl;
        }
    }
};


class textEditor {
public:
    textNode head;
    string path = "./../textStart.txt";
    Coursor coursor;

    textEditor() : head("") {
    }

    void textEditorPreset(string path) {
        this->path = path;
        this->clean();
        this->read();
        int lines = this->CountLines();
        coursor.Update(lines, &head);
    }

    int CountLines() {
        int lineCounter = 0;
        textNode *temp = &head;
        temp = temp->next;
        do {
            lineCounter++;
            temp = temp->next;
        } while (temp != nullptr);
        return lineCounter;
    }
    void clean(){
        textNode *temp = head.next;
        while (temp != nullptr) {
            textNode *next = temp->next;
            delete[] temp->data;
            delete temp;
            temp = next;
        }
        head.next = nullptr;
    }
    void read() {
        ifstream file(path);
        char str[MAX_STRING_SIZE];
        if (file.is_open()) {
            while (file.getline(str, MAX_STRING_SIZE)) {
                this->insert(str);
            }
            file.close();
            coursor.Update(CountLines(), &head);
        } else {
            cout << "Unable to open file";
        }

    }

    void insert(const char *input) {
        textNode *new_node = new textNode(input);

        if (head.next == nullptr) {
            head.next = new_node;
        } else {
            textNode *temp = head.next;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    }

    void print() {
        textNode *temp = head.next;
        while (temp != nullptr) {
            cout << temp->data << endl;
            temp = temp->next;
        }
    }

    void deleteLines(int numLines) {
        // Move the cursor up numLines lines
        std::cout << "\033[" << numLines << "A";
        // Clear from the cursor position to the end of the screen
        std::cout << "\033[J";
    }

    textNode HighLight() {
        char input = 'w';
        int position[] = {coursor.currentLine, coursor.currentIndex};
        cout << position[0] << " " << position[1] << endl;
        cout << "Go on, press any letter but 'w' 'a' 's' 'd' to end:" << endl;
        while (input == 'w' || input == 'a' || input == 's' || input == 'd') {
            cin >> input;
            cin.ignore();
            switch (input) {
                case 'w':
                    coursor.pointVerticalMove(-1, position);
                    break;
                case 's':
                    coursor.pointVerticalMove(1, position);
                    break;
                case 'a':
                    coursor.pointHorizontalMove(-1, position);
                    break;
                case 'd':
                    coursor.pointHorizontalMove(1, position);
                    break;
            }
        }

        int endPosition[] = {coursor.currentLine, coursor.currentIndex};
        textEditor chosen;
        textNode *temp = &head;
        temp = temp->next;
        for (int i = 1; i < position[0]; i++) {
            temp = temp->next;
        }

        for (int i = position[0]; i <= endPosition[0]; i++) {
            char *current = new char[strlen(temp->data) + 1];
            for (int j = position[1]; j <= endPosition[1]; j++) {
                current[j - position[1]] = temp->data[j - 1];
            }
            current[endPosition[1] - position[1]] = '\0';
            chosen.insert(current);
            delete[] current;
            temp = temp->next;
        }
        return chosen.head;
    }

    void PathChecker() {
        string tempPath;
        CheckPoint:
            cout << "Please, enter the path to the file: ";
            cin >> tempPath;
            cin.ignore();

            if ('/' != tempPath[0] || '.' != tempPath[0]){
                tempPath = "./../" + tempPath;
            }
                ifstream file(tempPath);
                if (!file.is_open()) {
                    cout << "File not found\n";
                    goto CheckPoint;
                } else {
                    path = tempPath;
                    cout << "Path changed\n";
                }

    }

    char* GetLine(){
        textNode* temp = &head;
        for (int i = 0; i < coursor.currentLine; i++){
            temp = temp->next;
        }
        return temp->data;
    }

    ~textEditor() {
        textNode *temp = head.next;
        while (temp != nullptr) {
            textNode *next = temp->next;
            delete[] temp->data;
            delete temp;
            temp = next;
        }
    }

};

/*
class UndoRedoManager {
public:


    void CheckStack(char WhatStack){
        if (WhatStack == 'u'){
            if (undoStack.size() >= 3){
                delete[] undoStack.front().data;
                undoStack.pop_front();
            }
        }
        else if (WhatStack == 'r'){
            if (redoStack.size() >= 3){
                delete[] redoStack.front().data;
                redoStack.pop_front();
            }
        }
    }

    void StackFulfill(char* input_str, long line_number){
        MyStringHAHAIDIDIT input{};
        input.data = new char[strlen(input_str) + 1];
        strcpy(input.data, input_str);
        input.lineNumber = line_number;
        // If the undo stack already contains 3 elements, pop the oldest one
        CheckStack('u');
        undoStack.push_front(input);
    }

    void Undo(textNode* node){
        if (undoStack.empty()){
            cout << "Nothing to undo" << endl;
            return;
        }
        MyStringHAHAIDIDIT temp = undoStack.back();
        undoStack.pop_back();

        textNode* temporaryNode = node;
        for (int i = 1; i <= temp.lineNumber; i++)
            temporaryNode = temporaryNode->next;

        MyStringHAHAIDIDIT tempNode;
        tempNode.data = new char[strlen(temporaryNode->data) + 1]; // allocate memory with new[]
        strcpy(tempNode.data, temporaryNode->data); // copy the data
        tempNode.lineNumber = temp.lineNumber;
        CheckStack('r');
        redoStack.push_front(tempNode);

        delete[] temporaryNode->data; // delete old memory
        temporaryNode->data = new char[strlen(temp.data) + 1]; // allocate new memory
        strcpy(temporaryNode->data, temp.data); // copy the data

        cout << "Undo: the " << temp.lineNumber << " line go back to " << temp.data << endl;
    }

    void Redo(textNode* node){
        if (redoStack.empty()){
            cout << "Nothing to redo" << endl;
            return;
        }
        MyStringHAHAIDIDIT temporary = redoStack.front();
        redoStack.pop_front();

        textNode* temporaryNode = node;
        for (int i = 1; i <= temporary.lineNumber; i++)
            temporaryNode = temporaryNode->next;

        MyStringHAHAIDIDIT oldNode;
        oldNode.data = new char[strlen(temporaryNode->data) + 1]; // allocate memory with new[]
        strcpy(oldNode.data, temporaryNode->data); // copy the data
        oldNode.lineNumber = temporary.lineNumber;
        CheckStack('u');
        undoStack.push_front(oldNode);

        delete[] temporaryNode->data; // delete old memory
        temporaryNode->data = new char[strlen(temporary.data) + 1]; // allocate new memory
        strcpy(temporaryNode->data, temporary.data); // copy the data

        cout << "Redo: the " << temporary.lineNumber << " line went back to " << temporary.data << endl;
    }
};
*/
class EncryptConnector{
public:

    void* handle; // to store the handle
    int (*encrypt)(char*,int,char*);
    int (*decrypt)(char*,int,char*);
    int (*keyChecker)();
    int key = -1;


    EncryptConnector(){
        // Load the dynamic library
        handle = dlopen("/Users/tim_bzz/Documents/projects/Clion/Paradigms/Assignment4/libcaesar.so", RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "Error loading library: %s\n", dlerror());
            exit(EXIT_FAILURE);
        }

        // Load the encrypt function from the library
        *(void**)(&encrypt) = dlsym(handle, "encrypt");
        char* error = dlerror();
        if (error != NULL) {
            fprintf(stderr, "Error loading 'encrypt' function: %s\n", error);
            exit(EXIT_FAILURE);
        }

        // Load the decrypt function from the library
        *(void**)(&decrypt) = dlsym(handle, "decrypt");
        error = dlerror();
        if (error != NULL) {
            fprintf(stderr, "Error loading 'decrypt' function: %s\n", error);
            exit(EXIT_FAILURE);
        }

        // Load the key function from the library
        *(void**)(&keyChecker) = dlsym(handle, "keyChecker");
        error = dlerror();
        if (error != NULL) {
            fprintf(stderr, "Error loading 'keyChecker' function: %s\n", error);
            exit(EXIT_FAILURE);
        }
    }

    void KeyCheck(){
        if (key == -1){
            key = keyChecker();
        }
    }

    void SetKey(){
        key = keyChecker();
    }

    string PathChecker() {
        string tempPath;
        cout << "Please, enter the path to the file or the filename: ";
        cin >> tempPath;
        cin.ignore();

        if ('/' != tempPath[0] || '.' != tempPath[0]){
            tempPath = "./../" + tempPath;
        }
        return tempPath;

    }
    void EncryptString (textEditor* head){
        char* message = head->GetLine();
        char* encryptedMessage = new char[strlen(message) + 1];
        KeyCheck();
        // Allocate more memory for the encrypted message if necessary
        encrypt(message, key, encryptedMessage);
        cout << "Encrypted text: " << encryptedMessage << endl;

        cout << "Do you want to save the encrypted message to a file? (y/n): ";
        char answer;
        cin >> answer;
        cin.ignore();
        if (answer == 'y') {
            SaveStringIntoFile(encryptedMessage);
        }
        else{
            cout << "The encrypted message is not saved. You can copy it:\n" << encryptedMessage << endl;
        }
        delete[] encryptedMessage;
    }

    void DecryptString (textEditor* head){
        char* message = head->GetLine();
        char* encryptedMessage = new char[strlen(message) + 1];
        KeyCheck();
        // Allocate more memory for the decrypted message if necessary
        char* decryptedMessage = new char[strlen(message) * 2]; // Change this according to your needs
        decrypt(message, key, decryptedMessage);
        cout << "Decrypted text: " << decryptedMessage << endl;

        cout << "Do you want to save the decrypted message to a file? (y/n): ";
        char answer;
        cin >> answer;
        cin.ignore();
        if (answer == 'y') {
            SaveStringIntoFile(decryptedMessage);
        }
        else{
            cout << "The decrypted message is not saved. You can copy it:\n" << decryptedMessage << endl;
        }
        delete[] decryptedMessage;


    }

    void UniversalEnDecryptor(textNode* startNode, int mode = 0) {
        KeyCheck();
        textEditor decrypted;

        for (textNode* temp = startNode->next; temp != nullptr; temp = temp->next) {
            char* encryptedMessage = new char[strlen(temp->data) * 2];
            if (mode == 0) {
                encrypt(temp->data, key, encryptedMessage);
            }
            else if (mode == 1){
                decrypt(temp->data, key, encryptedMessage);
            }
            decrypted.insert(encryptedMessage);
            delete[] encryptedMessage;
        }
        decrypted.print();
    }
    ~EncryptConnector(){ // Add this destructor
        dlclose(handle);
    }

    void SaveStringIntoFile(char* message = "", textNode* startNode = nullptr){
        string path = this->PathChecker();
        ofstream file(path);
        if (file.is_open()) {
            if (message != ""){
                file << message;
            }
            else if (startNode != nullptr){
                for (textNode* temp = startNode->next; temp != nullptr; temp = temp->next) {
                    file << temp->data << endl;
                }
            }
            file.close();
            cout<< "The message is saved\n";
        } else {
            cout << "Unable to open file";
        }
    }
};


class UI{
public:
    UI(){
        cout << "Welcome to the text encryptor\n";
        textNode tempNode("");
        textEditor head;
        EncryptConnector encryptor;
        char* message;

        head.textEditorPreset("./../textStart.txt");
        bool isRunning = true;
        printMenu();
        while (isRunning) {
            char answer = getCommand();
            switch (answer) {

                case 'p':
                    head.print();
                    break;

                case '1':
                    head.PathChecker();
                    head.textEditorPreset(head.path);
                    cout << "The path is set, The new text was uploaded\n";
                    head.print();
                    break;

                case 'k':
                    encryptor.SetKey();
                    break;



                case 'e':
                    encryptor.EncryptString(&head);
                    break;

                case 'i':
                    tempNode = head.HighLight();
                    encryptor.UniversalEnDecryptor(&tempNode);
                    break;



                case 'l':
                    encryptor.DecryptString(&head);
                    break;

                case '0':
                    tempNode = head.HighLight();
                    encryptor.UniversalEnDecryptor(&tempNode);
                    break;

                case 'w':
                    CleanConsole();
                    head.coursor.pointVerticalMove(-1, (int[]){-1, -1});
                    break;

                case 's':
                    CleanConsole();
                    head.coursor.pointVerticalMove(1, (int[]){-1, -1});
                    break;

                case 'a':
                    CleanConsole();
                    head.coursor.pointHorizontalMove(-1, (int[]){-1, -1});
                    break;

                case 'd':
                    head.coursor.pointHorizontalMove(1, (int[]){-1, -1});
                    break;

                case 'h':
                    printMenu();
                    break;

                case 'q':
                    isRunning = false;
                    break;

                default:
                    printInvalidCommand();
                    break;
            }
        }
    }

    void static CleanConsole(){
        system("clear");
    }

    void static printMenu(){
        cout << "Please, choose the option:\n"
            << "1 - change the origin's path\n"
            << "2 - change the destination's path\n"
            << "p - print the text\n"
            << "k - set the key \n"

            << "Encryption:\n"
                << "    e - encrypt the line\n"
                << "    i - highlight and encrypt\n"
                << "    [ - encrypt the whole text\n"
                << "    9 - encrypt the whole file without opening\n"
            << "Decryption:\n"
                << "    l - decrypt the line\n"
                << "    o - highlight and decrypt\n"
                << "    ] - decrypt the whole text\n"
                << "    0 - decrypt the whole file without opening\n"
             << "Pointer:\n"
             << "    w - move up\n"
             << "    s - move down\n"
             << "    a - move left\n"
             << "    d - move right\n"
             << " \n"
             << "q - quit\n"
             << "h - to see the list of commands\n";
    }

    char static getCommand(){
        char answer;
        cin >> answer;
        cin.ignore();
        return answer;
    }

    void static printInvalidCommand(){
        cout << "Invalid command. Choose 'h' to see all commands\n";
    }

    void static printProcessFinished(){
        cout << "Process finished.\n";
    }



};

int main() {
    UI ui;

    return 0;

}
