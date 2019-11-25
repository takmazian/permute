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

void permute_matrix(string inputFN, string outputFN);

class Matrix {
	vector<set<int>> available_sets;
	vector<set<int>> prohibited_sets;
public:
	int m, n;
	vector<int> matrix;
	int memory_iterations = 0;
	int read_iterations = 0;

	Matrix(const vector<int>& transpon, int m, int n) {
		this->m = m;
		this->n = n;
		set<int> generator;
		for (int j = 1; j <= n; j++) {
			generator.insert(j);
		}
		matrix.clear();
		for (int row = 1; row <= n; row++) 
			for (int col = 1; col <= m; col++) 
				matrix.push_back(0);
		for (int row = 1; row <= n; row++) {
			for (int col = 1; col <= m; col++) {
				set<int> s;
				prohibited_sets.push_back(s);
				s.insert(generator.begin(), generator.end());
				available_sets.push_back(s);
				put(row, col, transpon[(row - 1) * m + (col - 1)]);
			}
		}
	}

	Matrix(int m, int n) {
		this->m = m;
		this->n = n;
		for (int j = 1; j <= n; j++) {
			for (int i = 1; i <= m; i++)
				matrix.push_back(j);
		}
		random_device rd;
		shuffle(matrix.begin(), matrix.end(), rd);
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
			return (row - 1) + (col - 1) * n;
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
		read_iterations++;
		int index;
		if ((index = calc_index(row, col)) >= 0)
			return matrix[index];
		else
			return -1;
	}

	set<int>* get_set(vector<set<int>>& sets, int row, int col) {
		int index;
		if ((index = calc_index(row, col)) >= 0)
			return &sets[index];
		else
			throw exception("Index out of bounds");
	}

	void swap(int row, int col1, int col2) {
		int v1, v2;
		if (col1 != col2 && 
			(v1 = get(row, col1)) > 0 && (v2 = get(row, col2)) > 0) {
			put(row, col1, v2);
			put(row, col2, v1);
		}
	}

	bool permute(int top, int left, int bottom, int right) {
		int index1, index2;
		if ((index1 = calc_index(top, left)) < 0
			|| (index2 = calc_index(bottom, right)) < 0)
			return false;
		if (top >= bottom)
			return true;
		if (left >= right)
			return true;
		int col = left;
		bool success = true;
		while (col <= right) {
			if (permute_one_column(top, bottom, right, col))
				col++;
			else if (col > left) {
				get_set(available_sets, top, col)->insert(get(top, col));
				cout << "shift one column lef" << endl;
				col--;
				get_set(available_sets, top, col)->erase(get(top, col));
			} else {
				success = false;
				break;
			}
		}
		return success;
	}

	inline static bool contains(set<int>* a_set, int value) {
		return a_set->find(value) != a_set->end();
	}

	bool permute_one_column(const int top, const int bottom, const int right, const int col) {
		if (top >= bottom)
			return true;
		if (col >= right)
			return true;
		int row = top, test_value;
		bool success;
		while (row <= bottom) {
			set<int>* a_set = get_set(available_sets, row, col);
			int cur_col = col;
			success = contains(a_set, (test_value = get(row, col)));
			while (!success && cur_col < right) {
				cur_col++;
				success = contains(a_set, (test_value = get(row, cur_col)));
			}
			if (success) {
				erase_or_insert_after(test_value, row, col, bottom, right, false);
				if (cur_col != col)
					swap(row, col, cur_col);
				row++;
			}
			else if (row > top) {
				set<int>* ps = get_set(prohibited_sets, row, col);
				get_set(available_sets, row, col)->insert(ps->begin(), ps->end());
				ps->clear();
				row--;
				test_value = get(row, col);
				erase_or_insert_after(test_value, row, col, bottom, right, true);
				get_set(prohibited_sets, row, col)->insert(test_value);
				get_set(available_sets, row, col)->erase(test_value);
				memory_iterations++;
			}
			else 
				return false;
		}
		return success;
	}

	void inline erase_or_insert_after(const int value, const int row, const int col, const int bottom, const int right, const bool insert_if_true) {
		for (int i = row + 1; i <= bottom; i++) {
			memory_iterations++;
			if (insert_if_true)
				get_set(available_sets, i, col)->insert(value);
			else
				get_set(available_sets, i, col)->erase(value);
		}
	}


public:
	bool permute() {
		return permute(1, 1, n, m);
	}

	int get_value(int row, int col) {
		read_iterations--;
		return get(row, col);
	}
};

void save_matrix(Matrix matr, string outputFN) {
	cout << " M = " << matr.m << ", N = " << matr.n << endl;
	vector<int> transpon; 
	for (int i = 1; i <= matr.n; i++)
		for (int j = 1; j <= matr.m; j++)
			transpon.push_back(matr.get_value(i, j));
	ofstream file(outputFN);
	vector <int> ::iterator it = transpon.begin();
	for (int i = 1; i <= matr.n; i++) {
		copy(it, it + matr.m, ostream_iterator<int>(file, " "));
		it = it + matr.m;
		file << "  " << "\n";
	}
	file.close();
}

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
					Matrix matr = Matrix(m, n);
					save_matrix(matr, fn);
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

Matrix load_matrix(string inputFN) {
	ifstream ifile(inputFN);
	int n = 0, m = -1;
	string str;
	vector <int> transpon, v;
	while (ifile) {
		getline(ifile, str);
		istringstream is(str);
		v.insert(v.end(), istream_iterator<int>(is), istream_iterator<int>());
		transpon.insert(transpon.end(), v.begin(), v.end());
		if (m < 0) {
			m = (int)v.size();
		}
		n++;
		v.clear();

	}
	n--;
	return Matrix(transpon, m, n);
}

void permute_matrix(string inputFN, string outputFN) {
	Matrix matr = load_matrix(inputFN);
	bool success = matr.permute();
	cout << "Matrix is " << (success ? ("solved. \nSee output in " + outputFN) : "unsolvable") << endl;
	cout << "Total " << matr.read_iterations << " read iterations and " << matr.memory_iterations << " aux iterations.";
	if (success) save_matrix(matr, outputFN);
}





