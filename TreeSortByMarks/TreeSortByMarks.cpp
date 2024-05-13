	#include <iostream>
#include "tree.h"
#include "Header.h"
#include <fstream>

using namespace std;

struct student {
	char* second_name = new char[1024];
	float average_mark;
};

tree<student> load(const char* file_name) {
	ifstream f(file_name);
	tree<student> tree;
	if (!f.is_open()) return tree;

	while (!f.eof()) {
		student s{};
		f >> s.second_name >> s.average_mark;
		add(tree, s);
	}
	return tree;
}

std::ostream& operator<<(std::ostream& o, const student& s)
{
	return o << "<div>Фамилия: " << s.second_name << " средняя оценка: " << s.average_mark << "</div>";
}

bool operator<(const student s1, const student s2) {
	return s1.average_mark < s2.average_mark;
}
bool operator>(const student s1, const student s2) {
	return s1.average_mark > s2.average_mark;
}
bool operator==(const student s1, const student s2) {
	return s1.average_mark == s2.average_mark;
}

template <typename T>
bool show_tree(node<T>* root, T* compare) {
	if (!root) return false;
	bool f = false;
	if (root->val > *compare) {
		cout << root->val << endl;
		show_tree(root->left, compare);
		f = true;
	}
	if (show_tree(root->right, compare))
		f = true;
	return f;
}

template <typename T>
bool show_tree(tree<T> tree, T* compare) {
	return show_tree(tree.root, compare);
}

void save(char* name, char* mark) {
	ofstream f("student.txt", ios::app);
	f << name << ' ' << mark << endl;
}


void main() {
	cout << "Content-type: text/html; charset=windows-1251\n\n";

	ifstream f("index.tmpl");
	if (!f.is_open()) return;
	char* line = new char[1024];
	while (!f.eof()) {
		f.getline(line, 1024);
		if (!strcmp(line, "<!--CONTENT-->")) {


			cout << "<form action='TreeSortByMarks.cgi' method='post'>\n";
			cout << "<div><span>Фамилия</span> <input type='text'name='second_name'></div>";
			cout << "<div><span>Балл</span> <input type='number' step='0.01' name='mark'></div>";
			cout << "<div><input type='submit' value='Отправить'></div>";
			cout << "</form><br/>\n" << endl;

			cout << "<form action='TreeSortByMarks.cgi' method='post'>\n";
			cout << "<div><span>Средний балл</span> <input type='number'step='0.01' name='average'></div>";
			cout << "<div><input type='submit' value='Отправить'></div>";
			cout << "</form><br/>\n" << endl;
			char* data = nullptr;
			get_form_data(data);
			char* aver = nullptr;
			char* second_name = nullptr;
			char* mark = nullptr;
			get_param_value(data, "average", aver);
			get_param_value(data, "second_name", second_name);
			get_param_value(data, "mark", mark);
			if (aver != nullptr && aver != "") {
				auto tree = load("student.txt");
				cout << "<div>Минимальный балл " << aver << "</div>" << endl;
				student s{};
				float av;
				sscanf_s(aver, "%f", &av);
				s.average_mark = av;
				if (!show_tree(tree, &s))
					cout << "<div>Все тупые</div>" << endl;
			}
			else if ((second_name != nullptr && second_name != "") && (mark != nullptr && mark != "")) {
				save(second_name, mark);
			}
		}
		else if (!strcmp(line, "<!--MENU-->")) {
			cout << "<div id='menu'>\n";
			cout << "<div id='menu-info'>Навигация по сайту</div>\n";
			cout << "<a href='/index.cgi'>Биография</a></br>\n";
			cout << "<a href='/rofls.cgi'>Анекдоты</a>\n";
			cout << "</div>" << endl;
		}
		else
			cout << line << endl;
	}
	delete[] line;
	f.close();
}
