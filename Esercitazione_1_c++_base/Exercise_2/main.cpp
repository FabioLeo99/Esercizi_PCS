#include <iostream>
#include <fstream>
#include <sstream>

#include <iomanip>

using namespace std;

/// \brief ImporData reads the input data from the data file
/// \param inputFilePath: path name of the input file
/// \param S: the resulting initial wealth
/// \param n: the resulting number of assets
/// \param w: the resulting vector of the weights of assets in the portfolio
/// \param r: the resulting vector of the rates of return of assets
/// \return the result of the reading: true is success, false is error
bool ImportData(const string& inputFilePath,
                double& S,
                size_t& n,
                double*& w,
                double*& r);

/// \brief ComputeRateOfReturn computes the rate of return of the portfolio and the final amount of wealth
/// \param S: the initial wealth
/// \param n: the number of assets
/// \param w: the vector of the weights of assets in the portfolio
/// \param r: the vector of the rates of return of assets
/// \param rateOfReturn: the resulting rate of return of the portfolio
/// \param V: the resulting final wealth
void ComputeRateOfReturn(const double& S,
                         const size_t& n,
                         const double* const& w,
                         const double* const& r,
                         double& rateOfReturn,
                         double& V);

/// \brief ExportData prints data on an output stream
/// \param out: object of type ostream
/// \param S: the initial wealth
/// \param n: the number of assets
/// \param w: the vector of the weights of assets in the portfolio
/// \param r: the vector of the rates of return of assets
/// \param rateOfReturn: the rate of return of the portfolio
/// \param V: the final wealth
void ExportData(ostream& out,
                const double& S,
                const size_t& n,
                const double* const& w,
                const double* const& r,
                const double& rateOfReturn,
                const double& V);

/// \brief Export a vector in a string
/// \param n: size of the vector
/// \param v: vector
/// \return the resulting string
string ArrayToString(const size_t& n,
                     const double* const& v)
{
    ostringstream toString;
    toString << "[ ";
    for (unsigned int i = 0; i < n; i++)
        toString<< v[i]<< " ";
    toString << "]";

    return toString.str();
}



int main()
{
    string inputFileName = "D:\\PoliTo\\Matematica\\3-Anno\\Programmazione_e_calcolo_scientifico\\Progetto\\Esercizi_Esame\\Esercitazione_1_c++_base\\Exercise_2\\data.csv";
    double S = 0.0;
    size_t n = 0;
    double* w = nullptr;
    double* r = nullptr;

    if (!ImportData(inputFileName, S, n, w, r))
    {
        cerr<< "Something goes wrong with import"<< endl;
        return -1;
    }
    else
    {
        cout << "Import file eseguito" << endl;
    }

    double rateOfReturn;
    double V;
    ComputeRateOfReturn(S, n, w, r, rateOfReturn, V);       // Calcola il rate of return

    ExportData(cout, S, n, w, r, rateOfReturn, V);          // Esporta i dati

    string outputFileName = "D:\\PoliTo\\Matematica\\3-Anno\\Programmazione_e_calcolo_scientifico\\Progetto\\Esercizi_Esame\\Esercitazione_1_c++_base\\Exercise_2\\\\result.txt";      // Write data to a file
    ofstream file;
    file.open(outputFileName);

    if (!file.fail())
    {
        ExportData(file, S, n, w, r, rateOfReturn, V);
        cout << "Export file eseguito" << endl;
    }

    file.close();

    delete[] w;
    delete[] r;

    return 0;
}



bool ImportData(const string& inputFilePath,
                double& S,
                size_t& n,
                double*& w,
                double*& r)
{
    ifstream file;                                // Dichiarazione oggetto ifstream per leggere il file di input
    string line;                                  // Dichiarazione stringa per contenere le righe del file
    unsigned int i=0, N=0;                        // Dichiarazione contatori: i per i loop e N per il numero di righe del file

    cout << "Apertura file..." << endl;
    file.open(inputFilePath);                     // Apertura del file

    if (file.fail())                              // Verifica se l'apertura del file è fallita
    {
        cerr<< "file open failed" << endl;
        return false;                            // Restituisce false per segnalare l'errore
    }
    else
    {
        cout << "File aperto" << endl;
    }

    while(getline(file, line))                    // Conta il numero di righe nel file
    {
        N=N+1;
    }

    file.clear();                                 // Riposiziona il cursore del file all'inizio
    file.seekg(0, ios::beg);

    string* lines = new string[N];

    for(i=0; i < N; i++)                          // Leggi il file e salva ogni riga nell'array line
    {
        getline(file, lines[i]);
    }

    file.close();

    if (N > 0)                                     // Se ci sono righe nel file
    {
        string strS = lines[0];                   // accede al secondo elemento dell'array
        string _strS = strS.substr(2);            // ottiene la sottostringa a partire dalla posizione 2
        S = stod(_strS);                          // converte la sottostringa in un double

        string strn = lines[1];                   // accede al secondo elemento dell'array
        string _strn = strn.substr(2);            // ottiene la sottostringa a partire dalla posizione 2
        n = stoi(_strn);                          // converte la sottostringa in un numero intero

        w = new double[n];                        // Alloca la memoria per l'array w
        r = new double[n];                        // Alloca la memoria per l'array r

        for (i=3; i<N; i++)
        {
            string strwr = lines[i];
            size_t pos = strwr.find(";");         // Trova la posizione del carattere ";"
            string _strw = strwr.substr(0, pos);  // Estrarre la sottostringa prima ";"
            string _strr = strwr.substr(pos+1);   // Estrarre la sottostringa dopo ";"
            w[i-3] = stod(_strw);                 // converte la sottostringa in un doble
            r[i-3] = stod(_strr);                 // converte la sottostringa in un double
        }

        delete[] lines;                           // Libera la memoria allocata per l'array lines

        return true;                              // Restituisce true per segnalare il successo dell'operazione
    }
    else                                          // Se il file è vuoto
    {
        delete[] lines;                           // Libera la memoria allocata per l'array lines

        cerr << "File vuoto" << endl;             // Stampa un messaggio di errore
        return false;                             // Restituisce false per segnalare l'errore
    }
}


void ComputeRateOfReturn(const double& S,
                         const size_t& n,
                         const double* const& w,
                         const double* const& r,
                         double& rateOfReturn,
                         double& V)
{
    unsigned int i=0;
    for(i=0;i<n;i++)
    {
        V=V+(1+r[i])*w[i];
    }
    V=V*S;
    rateOfReturn=V/S-1;
    return;
}


void ExportData(ostream& out,
                const double& S,
                const size_t& n,
                const double* const& w,
                const double* const& r,
                const double& rateOfReturn,
                const double& V)
{
    out << "S = " << S << ", n = " << n << endl;           // scriviamo i dati richiesti nel file di output
    out << "w = " << ArrayToString(n, w) << endl;
    out << "r = " << ArrayToString(n, r) << endl;
    out << "Rate of return of the portfolio: " << rateOfReturn << endl;
    out << "V: " << V << endl;
}
