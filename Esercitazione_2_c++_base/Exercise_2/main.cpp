#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

/// \brief ImportText import the text for encryption
/// \param inputFilePath: the input file path
/// \param text: the resulting text
/// \return the result of the operation, true is success, false is error
bool ImportText(const string& inputFilePath,
                string& text);

/// \brief Encrypt encrypt the text
/// \param text: the text to encrypt
/// \param password: the password for encryption
/// \param encryptedText: the resulting encrypted text
/// \return the result of the operation, true is success, false is error
bool Encrypt(const string& text,
             const string& password,
             string& encryptedText);

/// \brief Decrypt decrypt the text
/// \param text: the text to decrypt
/// \param password: the password for decryption
/// \param decryptedText: the resulting decrypted text
/// \return the result of the operation, true is success, false is error
bool Decrypt(const string& text,
             const string& password,
             string& decryptedText);

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        cerr<< "Password shall passed to the program"<< endl;
        return -1;
    }
    string password = argv[1];

    string inputFileName = "./text.txt", text;
    if (!ImportText(inputFileName, text))
    {
        cerr << "Something goes wrong with import" << endl;
        return -1;
    }
    else
        cout << "Import successful: result= " << text<< endl;

    string encryptedText;
    if (!Encrypt(text, password, encryptedText))
    {
        cerr << "Something goes wrong with encryption" << endl;
        return -1;
    }
    else
        cout << "Encryption successful: result= " << encryptedText<< endl;

    string decryptedText;
    if (!Decrypt(encryptedText, password, decryptedText) || text != decryptedText)
    {
        cerr << "Something goes wrong with decryption" << endl;
        return -1;
    }
    else
        cout << "Decryption successful: result= " << decryptedText << endl;

    return 0;
}

bool ImportText(const string& inputFilePath,
                string& text)
{
    ifstream file;                                // Dichiarazione oggetto ifstream per leggere il file di input

    cout << "Apertura file..." << endl;
    file.open(inputFilePath);                     // Apertura del file

    if (file.fail())                              // Verifica se l'apertura del file è fallita
    {
        cerr<< "file open failed" << endl;
        return false;                             // Restituisce false per segnalare l'errore
    }
    else
    {
        cout << "File aperto" << endl;
        getline(file, text);
        return true;
    }
}

bool Encrypt(const string& text,
             const string& password,
             string& encryptedText)
{
    if (password.length() > text.length())        // Controllo se la password è più lunga del testo
    {
        cerr << "Errore: la password non può essere più lunga del testo." << endl;
        return false;
    }

    encryptedText = "";                           // Inizializzo la stringa cifrata come una stringa vuota
    unsigned int i=0;

    int passwordIndex = 0;                        // Inizializzo l'indice della password a 0
        for (i=0; i < text.length(); i++)         // Ciclo attraverso ogni carattere del testo
        {
            char c = text[i];                     // Prendo il carattere corrente

            if (isupper(c))                       // Controllo se il carattere è maiuscolo
            {
                int offset = 'A';                 // Offset per i caratteri maiuscoli
                char passwordChar = password[passwordIndex % password.length()]; // Prendo il carattere corrente della password

                if (isupper(passwordChar))        // Controllo se il carattere della password è maiuscolo
                {
                    passwordChar -= offset;       // Sottraggo l'offset per i caratteri maiuscoli
                }
                else
                {
                    cerr << "La password contiene un carattere in minuscolo" << endl;
                    return false;
                }

                char encrypted = (c - offset + passwordChar ) % 26 + offset;// Cifro il carattere sommando il carattere del testo e il carattere della password
                encryptedText += encrypted;       // Aggiungo il carattere cifrato alla stringa cifrata

                passwordIndex++;                  // Passo al carattere successivo della password
            }
            else if (islower(c))
            {
                cerr << "Il testo contiene un carattere in minuscolo" << endl;
                return false;
            }
            else if (c == ' ')
            {
                encryptedText += c;               // Preservo gli spazi senza cifrarli
            }
        }

    return true;
}

bool Decrypt(const string& text,
             const string& password,
             string& decryptedText)
{

    if (password.length() > text.length())   // Controllo se la password è più lunga del testo
    {
        cerr << "Errore: la password non può essere più lunga del testo." << endl;
        return false;
    }

    decryptedText = "";                      // Inizializzo la stringa decifrata come una stringa vuota
    unsigned int i=0;

    int passwordIndex = 0;                  // Inizializzo l'indice della password a 0
    for (i=0; i < text.length(); i++)       // Ciclo attraverso ogni carattere del testo cifrato
    {
        char c = text[i];                   // Prendo il carattere corrente

        if (isupper(c))                     // Controllo se il carattere è maiuscolo
        {
            int offset = 'A';               // Offset per i caratteri maiuscoli
            char passwordChar = password[passwordIndex % password.length()]; // Prendo il carattere corrente della password

            if (isupper(passwordChar))      // Controllo se il carattere della password è maiuscolo
            {
                passwordChar -= offset;     // Sottraggo l'offset per i caratteri maiuscoli
            }
            else
            {
                cerr << "La password contiene un carattere in minuscolo" << endl;
                return false;
            }

            char decrypted = (c - passwordChar + offset ) % 26 + offset;// Decifro il carattere sottraendo il carattere della password dal carattere cifrato
            decryptedText += decrypted;     // Aggiungo il carattere decifrato alla stringa decifrata

            passwordIndex++;                // Passo al carattere successivo della password

        }
        else if (islower(c))
        {
            cerr << "Il testo contiene un carattere in minuscolo" << endl;
            return false;
        }
        else if (c == ' ')
        {
            decryptedText += c;            // Preservo gli spazi senza decifrarli
        }
    }
    return true;
}
