#pragma once
// Matrix.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

struct Matrix
{
	// matricu mozemo definirati kao vektor vektora
	vector<vector<float>> A;
	Matrix() {}

	// konstruktor za kvadratnu matricu
	Matrix(int n, int m) { A.assign(n, vector<float>(m)); }

	//operator dohvacanja retka matrice 
	vector<float>& operator[](int n) { return A[n]; }
};


Matrix operator *(Matrix A, Matrix B)
{
	// operator mnozenja za kvadratne matrice
	if (A.A[0].size() != B.A.size()) {
		cout << "nemoguce mnozenje" << endl;
		exit(-1);
	}
	int n = A.A.size();
	int m = B.A[0].size();
	Matrix C(n, m);
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			for (int k = 0; k < B.A.size(); ++k)
				C[i][j] += A[i][k] * B[k][j];

	return C;
}


