// Semi_2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <conio.h>
#include <iostream>
#include <sstream>
#include "my_queue.h"
#include "my_queue.cpp"
using namespace std;

int main()
{
	my_queue<int> q[2];
	my_queue<int> res;
	unsigned ch = 0,current = 0;
	string menu_actions = "F1 : push elements || F2 : save queue || F3 :load queue || F4 : change queue || DEL :delete queue || ESC : exit \n\n 1 : invert q || 2 : invert a || 3 : sort q || 4 : sort a", err_str = " ", line;
	while (ch != 0x1B) {
		system("cls");
		cout << menu_actions << err_str << "->current queue:" << current + 1<<endl <<"queue 1: "<< q[0]<<endl<<"queue 2: "<<q[1] <<endl <<"res is: " <<res;
		rewind(stdin);
		ch = _getch();
		err_str = "\n";
		switch (ch)
		{
		case 0x3B://f1
			while (ch != 0x1B) {
				system("cls");
				cout << "Enter values fo queue " << current + 1 << endl << "current queue:" << q[current] << err_str << endl << "enter queue:";
				getline(cin, line);
				istringstream s_line(line);
				while (s_line.peek() != '\0') {
					s_line >> q[current].buff;
					if (s_line.fail()) {
						err_str += "input error";
						ch = 0x1B;
						break;
					}
					else q[current].push(q[current].buff);
				}
			}
			ch = 0;
			break;
		case 0x3C://f2
			cout << "enter name: ";
			cin >> line;
			q[current].save(line);
			break;
		case 0x3D://f3
			cout << "enter name: ";
			cin >> line;
			q[current].clear();
			q[current].load(line);
			if (q[current].load_fail()) {
				err_str += q[current].get_err();
			}
			break;
		case 0x3E://f4
			current++;
			current = current % 2;
			break;
		case 83://del
			q[current].~my_queue();
			break;
		case 49://1
			q[current].invert_q();
			break;
		case 50://2
			q[current].invert_a();
			break;
		case 51://3
			res.~my_queue();
			sort_q(q[0], q[1], res);
			break;
		case 52://4
			res.~my_queue();
			sort_a(q[0], q[1], res);
			break;
		default:
			break;
		}
	}
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
