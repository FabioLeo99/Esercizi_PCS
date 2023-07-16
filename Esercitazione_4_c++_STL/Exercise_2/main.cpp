#include <iostream>
#include "Eigen/Eigen"
#include <fstream>
#include "map"

using namespace std;
using namespace Eigen;

struct PolygonalMesh
{
    unsigned int NumberCell0D = 0;
    vector<unsigned int> Cell0DId = {};
    vector<Vector2d> Cell0DCoordinates = {};
    map<unsigned int, list<unsigned int>> Cell0DMarkers = {};

    unsigned int NumberCell1D = 0;
    vector<unsigned int> Cell1DId = {};
    vector<Vector2i> Cell1DVertices = {};
    map<unsigned int, list<unsigned int>> Cell1DMarkers = {};

    unsigned int NumberCell2D = 0;
    vector<unsigned int> Cell2DId = {};
    vector<unsigned int> NumVertices = {};
    vector<vector<unsigned int>> Cell2DVertices = {};
    vector<unsigned int> NumEdges = {};
    vector<vector<unsigned int>> Cell2DEdges = {};
    map<unsigned int, list<unsigned int>> Cell2DMarkers = {};
};

///\brief Import the Polygonal mesh and test if the mesh is correct
///\param mesh: a PolygonalMesh struct
///\return the result of the reading, true if is success, false otherwise
bool ImportMesh(PolygonalMesh& mesh);

///\brief Import the Cell0D properties from Cell0Ds.csv file
///\param mesh: a PolygonalMesh struct
///\return the result of the reading, true if is success, false otherwise
bool ImportCell0Ds(PolygonalMesh& mesh);

///\brief Import the Cell1D properties from Cell1Ds.csv file
///\param mesh: a PolygonalMesh struct
///\return the result of the reading, true if is success, false otherwise
bool ImportCell1Ds(PolygonalMesh& mesh);

///\brief Import the Cell2D properties from Cell2Ds.csv file
///\param mesh: a PolygonalMesh struct
///\return the result of the reading, true if is success, false otherwise
bool ImportCell2Ds(PolygonalMesh& mesh);



int main()
{
    PolygonalMesh mesh;

    if(!ImportMesh(mesh))
    {
        return 1;
    }

    return 0;
}



bool ImportMesh(PolygonalMesh& mesh)
{
    if(!ImportCell0Ds(mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!ImportCell1Ds(mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
      }
    }

    if(!ImportCell2Ds(mesh))
    {
        return false;
    }
    else
    {
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            vector<unsigned int> edges = mesh.Cell2DEdges[c];

            for(unsigned int e = 0; e < mesh.NumEdges[c]; e++)
            {
                const unsigned int origin = mesh.Cell1DVertices[edges[e]][0];
                const unsigned int end = mesh.Cell1DVertices[edges[e]][1];

                auto findOrigin = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), origin);
                if(findOrigin == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.Cell2DVertices[c].begin(), mesh.Cell2DVertices[c].end(), end);
                if(findEnd == mesh.Cell2DVertices[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

           cout << "c: " << c << ", origin: " << *findOrigin << ", end: " << *findEnd << endl;

            }
        }
    }
    return true;
}

bool ImportCell0Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("D:\\PoliTo\\Matematica\\3-Anno\\Programmazione_e_calcolo_scientifico\\Progetto\\Esercizi_Esame\\Esercitazione_4_c++_STL\\Exercise_2\\PolygonalMesh\\Cell0Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumberCell0D = listLines.size();

    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string value;

        // Leggi i valori separati da ';'
        getline(converter, value, ';');
        unsigned int id = stoi(value);

        getline(converter, value, ';');
        unsigned int marker = stoi(value);

        getline(converter, value, ';');
        double x = stod(value);

        getline(converter, value, ';');
        double y = stod(value);

        Vector2d coord(x, y);

        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);

        if (marker != 0)
        {
            if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())
                mesh.Cell0DMarkers.insert({marker, {id}});
            else
                mesh.Cell0DMarkers[marker].push_back(id);
        }
    }
    return true;
}

bool ImportCell1Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("D:\\PoliTo\\Matematica\\3-Anno\\Programmazione_e_calcolo_scientifico\\Progetto\\Esercizi_Esame\\Esercitazione_4_c++_STL\\Exercise_2\\PolygonalMesh\\Cell1Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);
        string value;

        // Leggi i valori separati da ';'
        getline(converter, value, ';');
        unsigned int id = stoi(value);

        getline(converter, value, ';');
        unsigned int marker = stoi(value);

        getline(converter, value, ';');
        int v0 = stoi(value);

        getline(converter, value, ';');
        int v1 = stoi(value);

        Vector2i vertices(v0, v1);

        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);

        if (marker != 0)
        {
            if (mesh.Cell1DMarkers.find(marker) == mesh.Cell1DMarkers.end())
                mesh.Cell1DMarkers.insert({marker, {id}});
            else
                mesh.Cell1DMarkers[marker].push_back(id);
        }
    }
    return true;
}

bool ImportCell2Ds(PolygonalMesh& mesh)
{
    ifstream file;
    file.open("D:\\PoliTo\\Matematica\\3-Anno\\Programmazione_e_calcolo_scientifico\\Progetto\\Esercizi_Esame\\Esercitazione_4_c++_STL\\Exercise_2\\PolygonalMesh\\Cell2Ds.csv");

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);



    for (const string& line : listLines)
        {
            istringstream converter(line);
            string value;

            unsigned int id;
            unsigned int marker;
            unsigned int NumVert;
            vector<unsigned int> vertices;
            unsigned int NumEdg;
            vector<unsigned int> edges;

            getline(converter, value, ';');
            id = stoi(value);

            getline(converter, value, ';');
            marker = stoi(value);

            getline(converter, value, ';');
            NumVert = stoi(value);

            for (unsigned int i = 0; i < NumVert; i++)
            {
                getline(converter, value, ';');
                vertices.push_back(stoi(value));
            }

            getline(converter, value, ';');
            NumEdg = stoi(value);

            for (unsigned int i = 0; i < NumEdg; i++)
            {
                getline(converter, value, ';');
                edges.push_back(stoi(value));
            }

            mesh.Cell2DId.push_back(id);
            mesh.NumVertices.push_back(NumVert);
            mesh.Cell2DVertices.push_back(vertices);
            mesh.NumEdges.push_back(NumEdg);
            mesh.Cell2DEdges.push_back(edges);

            if (marker != 0)
            {
                if (mesh.Cell2DMarkers.find(marker) == mesh.Cell2DMarkers.end())
                    mesh.Cell2DMarkers.insert({marker, {id}});
                else
                    mesh.Cell2DMarkers[marker].push_back(id);
            }
        }
    return true;
}
