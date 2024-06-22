#include <iostream>

extern "C"{
const int ASCII_A = 65;
const int ASCII_Z = 90;
const int ASCII_a = 97;
const int ALPHABET_SIZE = 26;
const char SKIP_SYMBOLS[] = {' ', '\n', '\t', '.', ',', '!', '?', ':', ';', '\'', '\"', '(', ')', '[', ']', '{', '}', '<', '>', '/', '\\', '|', '@', '#', '$', '%', '^', '&', '*', '-', '+', '=', '~', '`', '_'};

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
        bool is_upper = text[i] >= ASCII_A && text[i] <= ASCII_Z;
        bool is_lower = text[i] >= ASCII_a && text[i] <= ASCII_a + ALPHABET_SIZE;
        if (is_upper){
            encr[i] = ((text[i] + key - ASCII_A) % ALPHABET_SIZE) + ASCII_A;
        }
        else if (is_lower){
            encr[i] = ((text[i] + key - ASCII_a) % ALPHABET_SIZE) + ASCII_a;
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
        bool is_upper = text[i] >= ASCII_A && text[i] <= ASCII_Z;
        bool is_lower = text[i] >= ASCII_a && text[i] <= ASCII_a + ALPHABET_SIZE;
        if (is_upper){
            encr[i] = ((text[i] - key - ASCII_A + ALPHABET_SIZE) % ALPHABET_SIZE) + ASCII_A;
        }
        else if (is_lower){
            encr[i] = ((text[i] - key - ASCII_a + ALPHABET_SIZE) % ALPHABET_SIZE) + ASCII_a;
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




}