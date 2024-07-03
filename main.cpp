#include <stack>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <string>
#include <memory>

using namespace std;

#define MAX_STRING_SIZE 1024
string TYPICAL_PATH = "/Users/tim_bzz/Documents/projects/Clion/Paradigms/Assignment4/";

class textEditor;
class textNode;
class Coursor;
class EncryptConnector;
class FileProccess;

class textNode {
public:

    char* data;
    textNode *next;

    textNode(const char* text) {
        this->data = new char[strlen(text) + 1];
        strcpy(this->data, text);
        this->next = nullptr;
    }
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

    textNode* AdvanceToLine(int lineNumber) {
        textNode *temp = text;
        temp = temp->next;
        for (int i = 1; i < lineNumber; i++)
            temp = temp->next;
        return temp;
    }

    void pointVerticalMove(int direction, int positions[]){
        currentLine = (currentLine + direction) > maxLines ? maxLines :
                      (currentLine + direction) <= 1 ? 1 : currentLine + direction;
        textNode* temp = AdvanceToLine(currentLine);
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
        textNode *temp = AdvanceToLine(currentLine);

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

class FileProccess{
public:
    string path = "";
    textNode text;

    FileProccess() : text("") {}

    void PathChecker() {
        string tempPath;
        do {
            cout << "Please, enter the path to the file: \n";
            cin >> tempPath;
            cin.ignore();
            tempPath = TYPICAL_PATH + tempPath;
            ifstream file(tempPath);
            if (!file.is_open()) {
                cout << "File not found\n";
            }
            else {
                path = tempPath;
                file.close();
                break;
            }
        } while (true);
    }

    void insert(const char *input) {
        textNode *new_node = new textNode(input);

        if (text.next == nullptr) {
            text.next = new_node;
        } else {
            textNode *temp = text.next;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    }

    textNode readFile(){
            char str[MAX_STRING_SIZE];
            CheckPoint:
            ifstream file(path);
            if (!file.is_open()) {
                cout << "The start file was not found\n";
                PathChecker();
                goto CheckPoint;
            }
            while (file.getline(str, MAX_STRING_SIZE)) {
                this->insert(str);
            }
            file.close();
            return text;
    }

    void SaveStringIntoFile(const char* message = "", textNode* startNode = nullptr){
        string localPath;
        cout << "Please, enter the path to the file: \n";
        cin >> localPath;
        localPath = TYPICAL_PATH + localPath; // Ensure the path is prefixed with the typical path if needed
        ofstream file(localPath); // Use localPath to open the file
        if (!file.is_open()) {
            cout << "Error opening file at " << localPath << endl;
            return;
        }
        if (message != string("")){
            file << message;
        }
        else if (startNode != nullptr){
            for (textNode* temp = startNode; temp != nullptr; temp = temp->next) {
                file << temp->data << endl;
            }
        }
        file.close();
        cout << "The message is saved to " << localPath << endl;
    }
};

class textEditor {

public:
    textNode head;
    Coursor coursor;
    FileProccess fileOperations;

    textEditor() : head("") {
    }

    void textEditorPreset(string path) {
        this->clean();
        fileOperations.path = path;
        head = fileOperations.readFile();
        coursor.Update(CountLines(), &head);
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
        fileOperations.PathChecker();
        head = fileOperations.readFile();
        coursor.Update(CountLines(), &head);
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

    unique_ptr<textEditor> HighLight() {
        char input = 'w';
        int position[] = {coursor.currentLine, coursor.currentIndex};
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
        textNode *temp = &head;
        temp = temp->next;
        int symbolCount = 0;
        for (int i = 1; i < position[0]; i++) {
            temp = temp->next;
        }
        unique_ptr<textEditor> chosen(new textEditor());
        for (int i = position[0]; i <= endPosition[0]; i++) {
            char *current = new char[strlen(temp->data) + 1];
            for (int j = 1; j <= strlen(temp->data); j++) {
                if ((i == position[0] && i != endPosition[0] && j >= position[1])
                || (i == position[0] && i == endPosition[0] && j >= position[1] && j < endPosition[1])
                || (i > position[0] && i < endPosition[0])
                || (i == endPosition[0] && j < endPosition[1]))
                {
                    current[symbolCount] = temp->data[j - 1];
                    symbolCount++;
                }
            }
            current[strlen(current)] = '\0';
            chosen->insert(current);
            delete[] current;
            temp = temp->next;
            symbolCount = 0;
        }
        return chosen;
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

class EncryptConnector{
public:

    void* handle; // to store the handle
    int (*encrypt)(char*,int,char*);
    int (*decrypt)(char*,int,char*);
    int (*keyChecker)();
    void (*filer)(int);
    int key = -1;
    FileProccess fileOp;

    EncryptConnector(){
        // Load the dynamic library
        handle = dlopen("/Users/tim_bzz/Documents/projects/Clion/Paradigms/Assignment4/libcaesar.so", RTLD_LAZY);
        if (!handle) {
            fprintf(stderr, "Error loading library: %s\n", dlerror());
            throw runtime_error("Error loading library: " + string(dlerror()) + "\n");
        }
        // Load the encrypt function from the library
        *(void**)(&encrypt) = dlsym(handle, "encrypt");
        char* error = dlerror();
        if (error != NULL) {
            throw runtime_error("Error loading 'encrypt' function: " + string(error) + "\n");
        }
        // Load the decrypt function from the library
        *(void**)(&decrypt) = dlsym(handle, "decrypt");
        error = dlerror();
        if (error != NULL) {
            throw runtime_error("Error loading 'encrypt' function: " + string(error) + "\n");
        }
        // Load the key function from the library
        *(void**)(&keyChecker) = dlsym(handle, "keyChecker");
        error = dlerror();
        if (error != NULL) {
            throw runtime_error("Error loading 'encrypt' function: " + string(error) + "\n");
        }
        // Load the filer function from the library
        *(void**)(&filer) = dlsym(handle, "filer");
        error = dlerror();
        if (error != NULL) {
            throw runtime_error("Error loading 'encrypt' function: " + string(error) + "\n");
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
        cout << "Please, enter the path to the file or the filename: \n";
        cin >> tempPath;
        cin.ignore();
        tempPath = TYPICAL_PATH + tempPath;
        return tempPath;

    }
    void EncryptString (textEditor* head){
        char* message = head->GetLine();
        char* encryptedMessage = new char[strlen(message) + 1];
        KeyCheck();
        // Allocate more memory for the encrypted message if necessary
        encrypt(message, key, encryptedMessage);
        cout << "Encrypted text: " << encryptedMessage << endl;

        cout << "Do you want to save the encrypted message to a file? (y/n): \n";
        char answer;
        cin >> answer;
        cin.ignore();
        if (answer == 'y') {
            fileOp.SaveStringIntoFile(encryptedMessage);
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

        cout << "Do you want to save the decrypted message to a file? (y/n): \n";
        char answer;
        cin >> answer;
        cin.ignore();
        if (answer == 'y') {
            fileOp.SaveStringIntoFile(decryptedMessage);
        }
        else{
            cout << "The decrypted message is not saved. You can copy it:\n" << decryptedMessage << endl;
        }
        delete[] decryptedMessage;


    }

    void UniversalEnDecryptor(textNode* startNode, int mode = 0) {
        KeyCheck();
        textEditor decrypted;
        for (textNode* temp = startNode; temp != nullptr; temp = temp->next) {
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
        cout << "Done! your text is:\n";
        decrypted.print();
        cout <<"Do you want to save the text to a file? (y/n): ";
        char answer;
        cin >> answer;
        cin.ignore();
        if (answer == 'y') {
            fileOp.SaveStringIntoFile("", decrypted.head.next);
        }
        else{
            cout << "The text is not saved. You can copy it, though:\n";
            decrypted.print();
        }
    }

    ~EncryptConnector(){
        dlclose(handle);
    }
};


class UI{
public:
    UI(){
        cout << "Loading.";
        textEditor head;
        cout << ".";
        EncryptConnector encryptor;
        cout << ".";
        unique_ptr<textEditor> tempNode;
        cout << "+!\n";
        cout << "Welcome to the text encryptor\n";

        head.textEditorPreset("/Users/tim_bzz/Documents/projects/Clion/Paradigms/Assignment4/textStart.txt");
        cout << "The path is set, The new text was uploaded\n";
        bool isRunning = true;
        printMenu();
        while (isRunning) {
            char answer = getCommand();
            switch (answer) {

                case 'p':
                    CleanConsole();
                    head.print();
                    break;

                case '1':
                    CleanConsole();
                    head.read();
                    cout << "The path is set, The new text was uploaded\n";
                    head.print();
                    break;

                case 'k':
                    CleanConsole();
                    encryptor.SetKey();
                    break;

                case 'e':
                    CleanConsole();
                    encryptor.EncryptString(&head);
                    break;

                case 'i':
                    CleanConsole();
                    tempNode = head.HighLight();
                    encryptor.UniversalEnDecryptor(tempNode.get()->head.next, 0);
                    break;

                case '[':
                    CleanConsole();
                    encryptor.UniversalEnDecryptor(head.head.next, 0);
                    break;

                case '9':
                    CleanConsole();
                    encryptor.filer(0);
                    break;

                case '0':
                    CleanConsole();
                    encryptor.filer(1);
                    break;


                case 'l':
                    CleanConsole();
                    encryptor.DecryptString(&head);
                    break;

                case 'o':
                    CleanConsole();
                    tempNode = head.HighLight();
                    encryptor.UniversalEnDecryptor(tempNode.get()->head.next, 1);
                    break;

                case ']':
                    CleanConsole();

                    encryptor.UniversalEnDecryptor(head.head.next, 1);
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
                    CleanConsole();
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



};

int main() {
    UI ui;

    return 0;

}
