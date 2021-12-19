// OBJparser.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

map<int, int> clearizraz(string izraz) {
    map<int, int> mapa;
    bool sles = false, theNUM = false;
    string noviExpr = "", broj1 = "", broj2 = "";

    for (auto chr : izraz) {
        if (!sles) {
            noviExpr.push_back(chr);
        }
        if (chr == '/') {
            sles = !sles;
        }
    }

    for (auto chr : noviExpr) {
        if (chr == '/') {
            theNUM = true;
        }
        else {
            if (!theNUM) {
                broj1.push_back(chr);
            }
            else {
                broj2.push_back(chr);
            }
        }
    }

    mapa.insert({ stof(broj1), stof(broj2) });
    return mapa;
    
}

int main()
{
    
    ifstream inFile;
    string line;
    vector<string> linije;
    vector<vector<float>> vertex;
    vector<vector<map<int, int>>> faces;
    vector<vector<float>> normals;

    inFile.open("test.obj");
    if (!inFile) {
        cout << "Unable to open file";
        exit(1);
    }

    while (getline(inFile, line)) {
        if (line[0] == 'v' || line[0] == 'vn' || line[0] == 'f') {
            if (line[1] != 't') {
                linije.push_back(line);
            }
        }
    }


    for (auto lines : linije) {
        if (lines[0] == 'v' && lines[1] != 'n') {
            string broj = "";
            vector<float> verPOM;
            for (int i = 2; i <= lines.size(); i++)
            {
                if (lines[i] != ' ') {
                    broj.push_back(lines[i]);
                }
                else {
                    verPOM.push_back(stof(broj));
                    broj = "";
                }
            }
            verPOM.push_back(stof(broj));
            vertex.push_back(verPOM);
            
        }

        else if (lines[0] == 'f') {
            vector<map<int, int>> fejsPOM;
            string izraz;
            for (int i = 2; i <= lines.size(); i++)
            {
                if (lines[i] != ' ') {
                    izraz.push_back(lines[i]);
                }
                else {
                    fejsPOM.push_back(clearizraz(izraz));
                    izraz = "";
                }
                
            }
            fejsPOM.push_back(clearizraz(izraz));
            faces.push_back(fejsPOM);
        }

        else if (lines[0] == 'v' && lines[1] == 'n') {
            string broj = "";
            vector<float> normPOM;
            for (int i = 3; i <= lines.size(); i++)
            {
                if (lines[i] != ' ') {
                    broj.push_back(lines[i]);
                }
                else {
                    normPOM.push_back(stof(broj));
                    broj = "";
                }
            }
            normPOM.push_back(stof(broj));
            normals.push_back(normPOM);
        }
    }
    
    cout << "verteksi:" << endl << endl;
    for (auto lines : vertex) {
        for (auto koord : lines) {
            cout << koord << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "faces:" << endl << endl;
    for (auto lines : faces) {
        for (auto expr : lines) {
            for (auto koord : expr) {
                cout << koord.first << " " << koord.second << " ";
            }
            cout << " ";
        }
        cout << endl;
    }
    
    cout << endl;
    cout << "normale:" << endl << endl;
    for (auto lines : normals) {
        for (auto koord : lines) {
            cout << koord << " ";
        }
        cout << endl;
    }

    inFile.close();
    
    
    cout << "Hello World!\n";
}
