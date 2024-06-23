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

deque<MyStringHAHAIDIDIT> undoStack;
deque<MyStringHAHAIDIDIT> redoStack;
char copyBuffer [MAX_STRING_SIZE];
int currentLine =  1;
int currentIndex = 1;



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

    void Undo(){
        if (undoStack.empty()){
            cout << "Nothing to undo" << endl;
            return;
        }
        MyStringHAHAIDIDIT temp = undoStack.back();
        undoStack.pop_back();

        textNode* temporaryNode = this;
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

    void Redo(){
        if (redoStack.empty()){
            cout << "Nothing to redo" << endl;
            return;
        }
        MyStringHAHAIDIDIT temporary = redoStack.front();
        redoStack.pop_front();

        textNode* temporaryNode = this;
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

    void insert(const char* input) {
        textNode *new_node = new textNode(input);

        if (this->next == nullptr) {
            this->next = new_node;
        } else {
            textNode *temp = this->next;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    }

    void insertWithReplacement(){

        textNode* temp = this;
        for (int i = 1; i <= currentLine; i++)
            temp = temp->next;

        cout << "Please, be aware that you can type max 100 characters\n";
        cout << "Enter text to append: ";
        char text[MAX_STRING_SIZE];
        cin.getline(text, MAX_STRING_SIZE);

        char newLine[strlen(temp->data) + strlen(text) + 1];
        strncpy(newLine, temp->data, currentIndex);
        newLine[currentIndex-1] = '\0';
        strcat(newLine, text);
        strcat(newLine, temp->data + currentIndex + strlen(text));
        cout << "Your line is: " << newLine << endl;

        char answer;
        cout << "Do you want to save changes? (y/n): ";
        cin >> answer;
        cin.ignore();
        if (answer == 'y') {
            StackFulfill(temp->data, currentLine);
            delete[] temp->data;
            temp->data = new char[strlen(newLine) + 1];
            strcpy(temp->data, newLine);
            cout << "Changes saved" << endl;
        }
        else {
            cout << "Changes not saved" << endl;
        }
    }


    void read() {
        ifstream file("./../textStart.txt");
        char str[MAX_STRING_SIZE];
        if (file.is_open()) {
            while (file.getline(str, MAX_STRING_SIZE)) {
                this->insert(str);
            }
            file.close();
        } else {
            cout << "Unable to open file";
        }
    }

    void print() {
        textNode *temp = this->next;
        while (temp != nullptr) {
            cout << temp->data << endl;
            temp = temp->next;
        }
    }

    int CountLines(){
        int lineCounter = 0;
        struct textNode temp = *this;
        do
        {
            lineCounter++;
            temp = *temp.next;
        } while (temp.next != nullptr);
        return lineCounter;
    }


    void lineAppend() {
        textNode* temp = this;
        for (int i = 1; i <= currentLine; i++)
            temp = temp->next;

        cout << "Your line is: " << temp->data << endl;
        cout << "Please enter the text to append (max 1000 characters): ";
        string text;
        getline(cin, text);

        int oldSize = strlen(temp->data);
        int appendSize = text.length();
        int newSize = oldSize + appendSize + 1; // +1 for the null terminator

        char* newData = new char[newSize];
        strcpy(newData, temp->data);
        strcat(newData, text.c_str());

        cout << "Your line is now: " << newData << endl;

        char answer;
        cout << "Do you want to save changes? (y/n): ";
        cin >> answer;
        cin.ignore();
        if (answer == 'y') {
            StackFulfill(temp->data, currentLine);
            delete[] temp->data; // delete old memory
            temp->data = newData;
            cout << "Changes saved" << endl;
        }
        else {
            delete[] newData;
            cout << "Changes not saved" << endl;
        }
    }

    void textAddIntoLineByIndex() {
        textNode* temp = this;
        for (int i = 1; i <= currentLine; i++)
            temp = temp->next;

        cout << "Please, be aware that you can type max 100 characters\n";
        cout << "Enter text to append: ";
        char text[MAX_STRING_SIZE];
        cin.getline(text, MAX_STRING_SIZE);

        char newLine[strlen(temp->data) + strlen(text) + 1];
        strncpy(newLine, temp->data, currentIndex);
        newLine[currentIndex-1] = '\0';
        strcat(newLine, text);
        strcat(newLine, temp->data + currentIndex);
        cout << "Your line is: " << newLine << endl;

        char answer;
        cout << "Do you want to save changes? (y/n): ";
        cin >> answer;
        cin.ignore();
        if (answer == 'y') {
            StackFulfill(temp->data, currentLine);
            delete[] temp->data;
            temp->data = new char[strlen(newLine) + 1];
            strcpy(temp->data, newLine);
            cout << "Changes saved" << endl;
        }
        else {
            cout << "Changes not saved" << endl;
        }
    }

    void AddNewLine(){
        char answer;
        cout << "Wanna add some text or just an empty line? (a/e)" << endl;
        cin >> answer;
        cin.ignore();
        if (answer == 'a'){
            StackFulfill("", this->CountLines()+1);
            char* newLine = new char [MAX_STRING_SIZE];
            cout << "Go ahead, write new line " << endl;
            cin.getline(newLine, MAX_STRING_SIZE);
            this->insert(newLine);
        }
        else
        {
            this->insert("");

            cout << "Done!" << endl;
        }

    }

    int lineChecker() {
        int lineCounter = CountLines();
        int lineNum = 0;
        do {
            cout << "Founded " << lineCounter << " lines. Enter number of the line (i.e. 1): ";
            cin >> lineNum;
            cin.ignore();
        } while (lineNum < 1 || lineNum > lineCounter);
        return lineNum;
    }

    void searcher() {
        char text[MAX_STRING_SIZE];
        cout << "Enter the text to search: ";
        cin.getline(text, MAX_STRING_SIZE);

        int lineCounter = CountLines();
        int** positions = new int*[lineCounter];
        for(int i = 0; i < lineCounter; ++i)
            positions[i] = new int[2];

        textNode* temp = this->next;
        int occurrences = 0;
        int lineNum = 0;
        while (temp != nullptr) {
            char* found = strstr(temp->data, text);
            lineNum++;
            if (found != nullptr) {
                positions[occurrences][0] = lineNum;
                positions[occurrences][1] = found - temp->data;
                occurrences++;
            }
            temp = temp->next;
        }
        if (occurrences == 0) {
            cout << "No occurrences found\n";
            return;
        }
        else if (occurrences == 1) {
            cout << "Found 1 occurrence\n";
        }
        else {
            cout << "Found " << occurrences << " occurrence(s)\n";
        }
        for (int i = 0; i < occurrences; i++) {
            cout << "Line: " << positions[i][0] << ", index: " << positions[i][1] << "\n";
        }

        for(int i = 0; i < lineCounter; ++i)
            delete [] positions[i];
        delete [] positions;
    }

    void deleter(){
        long number, stringLength;
        bool isRunning = true;
        textNode* temp = this;

        do {
            cout << "Please specify number of symbols to delete" << endl;
            cin >> number;
            cin.ignore();
            if (number > 0) {
                isRunning = false;
            } else {
                cout << "I can not delete less than 1 symbol. Try again " << endl;
            }
        } while (isRunning);

        for (int i = 1; i <= currentLine; i++)
            temp = temp->next;

        char* newLine = new char[strlen(temp->data)];
        int actualRewritten = 0;
        stringLength = strlen(temp->data);
        for (int i = 0; i < stringLength; i++)
        {
            if (i < currentIndex-1 || i > currentIndex-1 + number)
            {
                newLine[actualRewritten] = temp->data[i];
                actualRewritten++;
            }
        }

        cout << "Your line is now: " << newLine << endl;
        cout << "Do you want to save changes? (y/n): ";
        char answer;
        cin >> answer;
        cin.ignore();
        if (answer == 'y') {
            StackFulfill(temp->data, currentLine);
            delete[] temp->data;
            temp->data = newLine;
            cout << "Changes saved" << endl;
        }
        else {
            cout << "Changes not saved" << endl;
            delete[] newLine;
        }
    }

    void Copy() {
        textNode *temp = this;
        bool isRunning = true;
        long number;
        do
        {
            cout << "Please specify number of symbols to copy" << endl;
            cin >> number;
            cin.ignore();
            if (number > 0) {
                isRunning = false;
            } else {
                cout << "I can not copy less than 1 symbol. Try again " << endl;
            }
        } while (isRunning);
        for (int i = 1; i <= currentLine; i++)
            temp = temp->next;

        strncpy(copyBuffer, temp->data + currentIndex - 1, number);
        copyBuffer[number] = '\0';
        cout << "Copied text: " << copyBuffer << endl;
    }

    void Paste() {
        if (strlen(copyBuffer) == 0) {
            cout << "Nothing to paste" << endl;
            return;
        }

        textNode *temp = this;
        long stringLength;

        for (int i = 1; i <= currentLine; i++)
            temp = temp->next;
        stringLength = strlen(temp->data);

        char newLine[stringLength + strlen(copyBuffer) + 1];
        strncpy(newLine, temp->data, currentIndex-1);
        newLine[currentIndex-1] = '\0';
        strcat(newLine, copyBuffer);
        strcat(newLine, temp->data + currentIndex-1);

        StackFulfill(temp->data, currentLine);
        delete[] temp->data;
        temp->data = new char[strlen(newLine) + 1];
        strcpy(temp->data, newLine);
        cout << "Changes saved: " << newLine << endl;
    }

    void Cut() {
        textNode *temp = this;
        bool isRunning = true;
        long number, stringLength;
        do
        {
            cout << "Please specify number of symbols to copy" << endl;
            cin >> number;
            cin.ignore();
            if (number > 0) {
                isRunning = false;
            } else {
                cout << "I can not copy less than 1 symbol. Try again " << endl;
            }
        } while (isRunning);
        for (int i = 1; i <= currentLine; i++)
            temp = temp->next;
        stringLength = strlen(temp->data);
        strncpy(copyBuffer, temp->data + currentIndex - 1, number);
        copyBuffer[number] = '\0';
        cout << "Copied text: " << copyBuffer << endl;

        char* newLine = new char[stringLength - number + 1];
        int actualRewritten = 0;
        for (int i = 0; i < stringLength; i++)
        {
            if (i < currentIndex - 1 || i > currentIndex - 1 + number)
            {
                newLine[actualRewritten] = temp->data[i];
                actualRewritten++;
            }
        }
        StackFulfill(temp->data, currentLine);
        temp->data = newLine;
        cout << "Your line is now: " << newLine << endl;

    }

};


class textEditor {
public:
    textNode head;
    string path = "./../textStart.txt";

    textEditor() : head("") {
        this->read();
    }

    int CountLines(){
        int lineCounter = -1;
        textNode* temp = &head;
        do
        {
            lineCounter++;
            temp = temp->next;
        } while (temp != nullptr);
        cout << "Lines: " << lineCounter << endl;
        return lineCounter;
    }

    void read() {
        ifstream file(path);
        char str[MAX_STRING_SIZE];
        if (file.is_open()) {
            while (file.getline(str, MAX_STRING_SIZE)) {
                this->insert(str);
            }
            file.close();
        } else {
            cout << "Unable to open file";
        }
    }

    void insert(const char* input) {
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

    void print(){
        textNode *temp = head.next;
        while (temp != nullptr) {
            cout << temp->data << endl;
            temp = temp->next;
        }

    }
};

class Coursor{
public:
    int currentLine =  1;
    int currentIndex = 1;



    void pointVerticalMove(int direction, int maxLines, textNode* text){

        currentLine = (currentLine + direction) >= maxLines ? maxLines :
                      (currentLine + direction) <= 1 ? 1 : currentLine + direction;
        textNode *temp = text;
        for (int i = 1; i < currentLine; i++)
            temp = temp->next;
        if (currentIndex > strlen(temp->data)) {
            currentIndex = strlen(temp->data) > 0 ? strlen(temp->data) : 1;
        }

        pointVisualisator(text, maxLines);
    }

    void pointHorizontalMove(int direction, int maxLines, textNode* text) {
        textNode *temp = text;
        temp = temp->next;
        for (int i = 1; i < currentLine; i++)
            temp = temp->next;

        if (currentIndex + direction > strlen(temp->data) + 1){
            currentIndex = 1;
            if (currentLine < maxLines){
                currentLine++;
            }
        } else if (currentIndex + direction < 1){
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
        pointVisualisator(text, maxLines);
    }

    void pointVisualisator(textNode* text, int maxLines){
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
};

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

class EncryptConnector{
public:

    void* handle; // to store the handle
    int (*encrypt)(char*,int,char*);
    int (*decrypt)(char*,int,char*);

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
                cout << "I can not encrypt with negative key. Try again " << endl;
            }
        } while (isRunning);
        return key;
    }

    void EncryptMessage (char message[]){
        int key = keyChecker();
        // Allocate more memory for the encrypted message if necessary
        char* encryptedMessage = new char[strlen(message) * 2]; // Change this according to your needs
        encrypt(message, key, encryptedMessage);
        cout << "Encrypted text: " << encryptedMessage << endl;
        delete[] encryptedMessage;
    }

    void DecryptMessage (char message[]){
        int key = keyChecker();
        // Allocate more memory for the decrypted message if necessary
        char* decryptedMessage = new char[strlen(message) * 2]; // Change this according to your needs
        decrypt(message, key, decryptedMessage);
        cout << "Decrypted text: " << decryptedMessage << endl;
        delete[] decryptedMessage;
    }
    ~EncryptConnector(){ // Add this destructor
        dlclose(handle);
    }
};

/*
class UI{
public:
    UI(){
        cout << "Welcome to the text encryptor\n";
        textNode head("");
        head.read();
        bool isRunning = true;
        printMenu();
        while (isRunning) {
            char answer = getCommand();
            switch (answer) {
                case 'l':
                    head.lineChecker();
                    break;

                case 'p':
                    head.print();
                    break;

                case '0':
                    head.lineAppend();
                    break;

                case '1':
                    head.AddNewLine();
                    break;

                case '2':
                    head.insert("\n");
                    cout << "New line was started\n";
                    break;

                case '3':
                    head.deleter();
                    break;

                case '4':
                    head.read();
                    break;

                case '5':
                    head.insertWithReplacement();
                    break;

                case '6':
                    head.textAddIntoLineByIndex();
                    break;

                case '7':
                    head.searcher();
                    break;

                case '8':
                    system("clear");
                    break;

                case 'h':
                    printMenu();
                    break;

                case 'u':
                    head.Undo();
                    break;

                case 'r':
                    head.Redo();
                    break;

                case 'c':
                    head.Copy();
                    break;

                case 'v':
                    head.Paste();
                    break;

                case 'x':
                    CleanConsole();
                    head.Cut();
                    break;

                case 'w':
                    CleanConsole();
                    head.pointVerticalMove(-1);
                    break;

                case 's':
                    CleanConsole();
                    head.pointVerticalMove(1);
                    break;

                case 'a':
                    CleanConsole();
                    head.pointHorizontalMove(-1);
                    break;

                case 'd':
                    head.pointHorizontalMove(1);
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
             << "l - change line\n"
             << "p - print all lines\n"
             << "0 - append existing line in the file\n"
             << "1 - add new line to the end of the file\n"
             << "2 - start a new line\n"
             << "3 - delete some symbols\n"
             << "4 - read the file\n"
             << "5 - insert with replacement\n"
             << "6 - add some text by line and symbol index\n"
             << "7 - search a text\n"
             << "8 - clear the console\n"
             << "u - undo\n"
             << "r - redo\n"
             << "c - copy\n"
             << "v - paste\n"
             << "x - cut\n"
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
*/

int main() {
    textEditor a;
    a.CountLines();
    a.print();
    return 0;
}
