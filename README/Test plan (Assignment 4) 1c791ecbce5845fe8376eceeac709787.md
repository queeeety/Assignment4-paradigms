# Test plan (Assignment 4)

This is the test plan for the fourth assignment. There will be my steps done and the screenshot with the output, divided by the functionality.

The program is a text viewer with encryption and decryption capabilities. It uses a linked list to store the text data, with each node in the list representing a line of text. The program allows the user to navigate through the text using a cursor, and select portions of the text to encrypt or decrypt. The encryption and decryption functions are loaded dynamically from a shared library at runtime. The program also includes functionality to read text from a file and write the encrypted or decrypted text back to a file. The encryption and decryption algorithms used are Caesar ciphers, which shift the characters in the text by a certain amount. The program uses the C++ Standard Library for file I/O and dynamic memory management, and the dlfcn.h library for dynamic loading of shared libraries.

## Development plan

- The functional was defined
- The encryptor library was transferred
- The dynamic library connector class in main file was implemented
- The textNode class was transferred from the 2 assignment
- The textEditor class was created to separate editing functions from saving class
- The Coursor class was also created to operate the movement and highlight
- The UI class was created
- The executable file was configured

# Main function test

The `Main` function is just supposed to call the UI class.

# UI Class

The `UI` class is for user’s interaction with the program. It connects all classes, prints the menu, receives option, validates and call corresponding function.

**>program start**

![Знімок екрана 2024-06-25 о 21.32.42.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_21.32.42.png)

**>validation**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled.png)

k, lets go to the functional:

## Encryption

### encrypt the line

This option offers to encrypt the entire line, where the cursor is. 

**>usecase**

![3.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/3.png)

**>saving**

![Знімок екрана 2024-06-25 о 21.52.37.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_21.52.37.png)

**>not saving**

![4.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/4.png)

### highlight and encrypt

The highlight feature allows to manually choose what text to encrypt.

**>Highlight functional**

![Знімок екрана 2024-06-25 о 22.05.30.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.05.30.png)

**>encryption**

![Знімок екрана 2024-06-25 о 22.13.03.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.13.03.png)

**>save in file**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%201.png)

**>the file itself**

![Знімок екрана 2024-06-25 о 22.13.59.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.13.59.png)

**>not saving**

![Знімок екрана 2024-06-25 о 22.16.17.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.16.17.png)

### encrypt the whole text

This function encrypts all text that is saved in the program’s memory.

**>functional**

![Знімок екрана 2024-06-25 о 22.19.03.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.19.03.png)

**>saving into file**

![Знімок екрана 2024-06-25 о 22.20.19.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.20.19.png)

**>not saving**

![Знімок екрана 2024-06-25 о 22.20.35.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.20.35.png)

### encrypt the whole file without opening

This is a functional of a library. The library is able to open the file, encrypt it line by line and save into another file

**>functional**

![Знімок екрана 2024-06-25 о 22.26.56.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.26.56.png)

**>the text itself**

![Знімок екрана 2024-06-25 о 22.28.44.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.28.44.png)

## Decryption

### Decrypt the line

This option offers to decrypt the entire line, where the cursor is. 

**>functional**

![Знімок екрана 2024-06-26 о 05.59.10.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-26_%25D0%25BE_05.59.10.png)

**>saving**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%202.png)

**>not saving**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%203.png)

### Highlight and decrypt

 The highlight feature allows to manually choose what text to decrypt.

**>the functional**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%204.png)

**>decryption**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%205.png)

**>saving**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%206.png)

**>the file itself**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%207.png)

**>not saving**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%208.png)

### Decrypt the whole text

This function encrypts all text that is saved in the program’s memory.

**>functional**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%209.png)

**>saving**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%2010.png)

**>not saving**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%2011.png)

### Decrypt the whole file without opening

This is a functional of a library. The library is able to open the file, encrypt it line by line and save into another file

**>functional**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%2012.png)

**>the text itself**

![Untitled](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/Untitled%2013.png)

## Helpers

### Change the origin's path

Change the text in process with the text in the file.

**>file path validation**

![Знімок екрана 2024-06-25 о 22.03.17.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.03.17.png)

**>functional**

![Знімок екрана 2024-06-25 о 22.03.45.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.03.45.png)

### Print the text

Print the text that is under operation right now.

![Знімок екрана 2024-06-25 о 22.01.39.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.01.39.png)

### Set the key

Once once the program is launched, if the keys not set and user wants to use one of the functions, the program will offer to set a key. Once the key is set the program will not offer the possibility to change it. The only way to change it after is to call change key option.

**>auto-key-missing-detection**

![5.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/5.png)

**>manual key set**

![6.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/6.png)

**>validation (can not be ≤0)**

![Знімок екрана 2024-06-25 о 21.55.14.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_21.55.14.png)

### Quit

Just quits the program. Also free the memory

![Знімок екрана 2024-06-25 о 22.00.06.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.00.06.png)

### Help

Prints the menu

![Знімок екрана 2024-06-25 о 21.59.33.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_21.59.33.png)

# Cursor move and visualisator

`pointVerticalMove` and `pointHorizontalMove` move the cursor through the text. And `pointVisualisator` helps to see what happens. It’s called after each movement, btw.

![Знімок екрана 2024-06-25 о 22.36.50.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.36.50.png)

![Знімок екрана 2024-06-25 о 22.37.06.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.37.06.png)

![Знімок екрана 2024-06-25 о 22.37.24.png](Test%20plan%20(Assignment%204)%201c791ecbce5845fe8376eceeac709787/%25D0%2597%25D0%25BD%25D1%2596%25D0%25BC%25D0%25BE%25D0%25BA_%25D0%25B5%25D0%25BA%25D1%2580%25D0%25B0%25D0%25BD%25D0%25B0_2024-06-25_%25D0%25BE_22.37.24.png)