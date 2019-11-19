#include <iostream>
#include <fstream>
#include <algorithm> 
#include<sstream>
#include <random>   
#include <array>   
#include <set>
#include <unordered_set>
#include <iterator>
#include <string>
using namespace std;

void generate_matrix(const int m, const int n, string fileName);
void permute_matrix(string inputFN, string outputFN);

int main(int argc, char* argv[])
{
	try {
		if (argc > 2) {
			const char* key = argv[1];
			if (key[0] == '-') {
				if (argc > 3 && key[1] == 'g') {
					const int  m = atoi(argv[2]), n = atoi(argv[3]);
					string fn;
					cin >> fn;
					generate_matrix(m, n, fn);
				}
				else if (key[1] == 'p') {
					string ifn = argv[2], ofn;
					cin >> ofn;
					permute_matrix(ifn, ofn);
				}
			}
		}
	}
	catch (exception e) {
		cerr << "\n" << e.what() << endl;
		return -1;
	}
}

class Matrix
{
public:
	int m, n;
	vector<int> matrix;

	Matrix(vector<int> matr, int m, int n) {
		matrix = matr;
		this->m = m;
		this->n = n;
	}

private:
	bool validate_row(int row) {
		return row <= n && row >= 1; 
	}
	
	bool validate_col(int col) {
		return col >= 1 && col <= m;
	}

	int calc_index(int row, int col) {
		if (validate_row(row) && validate_col(col)) {
			return (row - 1) * m + col - 1;
		}
		else
			return -1;
	}

	int put(int row, int col, int value) {
		int index;
		if ((index = calc_index(row, col)) >= 0)
			return matrix[index] = value;
		else
			return -1;
	}

	int get(int row, int col) {
		int index;
		if ((index = calc_index(row, col)) >= 0)
			return matrix[index];
		else
			return -1;
	}

	void swap(int row, int col1, int col2) {
		int v1, v2;
		if ((v1 = get(row, col1)) > 0 && (v2 = get(row, col2)) > 0) {
			put(row, col1, v2);
			put(row, col2, v1);
		}
	}

	int permute(int upper, int left, int lower, int right) {
		int index1, index2;
		if ((index1 = calc_index(upper, left)) < 0
			|| (index2 = calc_index(lower, right)) < 0)
			return -1;
		int i = upper;
		int j = left;
		while (j <= right) {
			while (i <= lower) {
				int k = i + 1;

			}
		}
		return 1;
	}

public: int permute() {
		return permute(1, 1, n, m);
	}
};

void save_matrix(Matrix matr, string outputFN) {
	cout << " M = " << matr.m << ", N = " << matr.n << endl;
	vector<int>& original = matr.matrix;
	ofstream file(outputFN);
	vector <int> ::iterator it = original.begin();
	for (int i = 1; i <= matr.n; i++) {
		copy(it, it + matr.m, ostream_iterator<int>(file, " "));
		it = it + matr.m;
		file << "  " << "\n";
	}
	file.close();
}

void generate_matrix(const int m, const int n, const string fileName) {
	vector<int> original;
	for (int j = 1; j <= n; j++)
		for (int i = 1; i <= m; i++) 
			original.push_back(j);

	random_device rd;
	shuffle(original.begin(), original.end(), rd);
	Matrix matr = Matrix(original, m, n);
	save_matrix(matr, fileName);
}


Matrix load_matrix(string inputFN) {
	ifstream ifile(inputFN);
	int n = 0, m = -1;
	string str;
	vector <int> matrix, v;
	while (ifile) {
		getline(ifile, str);
		istringstream is(str);
		v.insert(v.end(), istream_iterator<int>(is), istream_iterator<int>());
		matrix.insert(matrix.end(), v.begin(), v.end());
		if (m < 0) {
			m = (int) v.size();
		}
		n++;
		v.clear();

	}
	n--;
	return Matrix(matrix, m, n);
}

void permute_matrix(string inputFN, string outputFN) {
	Matrix matr = load_matrix(inputFN);
	cout << "Matrix is " << (matr.permute() == 0 ? "solved" : "unsolvable") << endl;
	save_matrix(matr, outputFN);
}





