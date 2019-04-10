#pragma once
#ifndef MY_QUEUE
#define MY_QUEUE_H
#include <ostream>
#include <fstream>
#include <string>
#include <bitset>

#define LACK_OF_ELEMENTS const int 0;//������� �����
#define INDEX_OUT_OF_RANGE const int 1;//�������� ������
#define FILE_OPEN_FAIL const int 2;//�� ������� ������� ����
#define ELEMENT_TYPE_DIFFER const int 3;//��� ������������ �������� �� ������������� ���� �������

template<typename T>
class my_queue
{
private:
	//������� �� ������� = �������� ���
	//�������� �������  - ��������� �����
	class Node {
	public:
		Node *pnext; // ��������� �� ����� ����
		T elem; // ������� � ����
		Node(T &_elem, Node *_pnext = nullptr) :elem(_elem), pnext(_pnext) {};
	};
	Node *head;
	unsigned size;
	//������� �����
	std::bitset<4> bits;
public:
	T buff;
	//�������������
	my_queue() {
		head = nullptr;
		size = 0;
		buff = 0;
	};
	//���������� ������� ������ ������� ���� ���� ��������
	~my_queue() {
		while (size)pop();
	}
	//���������� ��������
	void push(T _elem) {
		head = new Node(_elem, head);
		size++;
	}
	//���������� � ������� �������� �������
	T front() {
		bits.reset(0);
		if (head != nullptr) {
			Node *pointer = head;
			while (pointer->pnext != nullptr) {
				pointer = pointer->pnext;
			}
			buff = pointer->elem;
			pop();
			return buff;
		}
		else {
			bits.set(0);
		}

	}
	//����� ������� �� ������� ��� ��������
	T & operator[](unsigned _index) {
		bits.reset(1); bits.reset(0);
		if (head != nullptr) {
			if (_index < size) {
				_index = size - _index - 1;
				Node *pointer = head;
				while (_index) {
					pointer = pointer->pnext;
					_index--;
				}
				return pointer->elem;
			}
			else bits.set(1);
		}
		else bits.set(0);
	}
	//���������� � ����������
	T tail() {
		bits.reset(0);
		if (head != nullptr)return head->elem;
		else bits.set(0);
	}
	//������� ������
	void pop() {
		bits.reset(0);
		if (head != nullptr) {
			Node *pointer = head;
			while (pointer->pnext != nullptr)pointer = pointer->pnext;
			if (pointer != head) {
				Node *del = pointer;
				pointer = head;
				while (pointer->pnext != del)pointer = pointer->pnext;
				pointer->pnext = nullptr;
				delete[] del;
			}
			else {
				delete[] head;
				head = nullptr;
			}
			size--;
		}
		else bits.set(0);
	}
	//������ �� �������
	bool empty() {
		if (size)return true;
		return false;
	}
	//���������� ������ �������
	unsigned get_size() {
		return this->size;
	}
	//��������� � ����
	void save(std::string _name) {
		std::ofstream out(_name + ".bin", ios::out | ios::binary);
		while (size) {
			buff = front();
			out.write((char*)&buff, sizeof(T));
		}
		out.close();
		load(_name);
	}
	//��������� 1-���� �� ������ 2-������ ������ 0-��� ��
	int load(std::string _name) {
		bits.reset(2); bits.reset(3);
		std::ifstream in(_name + ".bin", ios::in | ios::binary);
		if (!in.is_open()) {
			bits.set(2);
			return 1;
		}
		this->~my_queue();
		while (!in.eof()) {
			in.read((char*)&buff, sizeof(T));
			if (in.fail()) {
				in.close();
				bits.set(3);
				this->~my_queue();
				return 2;
			}
			this->push(buff);
		}
		in.close();
		return 0;
	}
	//���������� ������
	int fail() {
		return bits.test(0) + bits.test(1) + bits.test(2) + bits.test(3);
	}
	//������ ��������
	int load_fail() {
		return bits.test(2) + bits.test(3);
	}
	//�������� ������
	void clear()
	{
		bits.reset(0);
		bits.reset(1);
		bits.reset(2);
		bits.reset(3);
	}
	std::string get_err() {
		std:: ifstream in("err.txt", ios::in);
		std::string error,res = "\0";
		for (unsigned i = 0; i < 4; ++i) {
			if (bits.test(i)) {
				in.seekg(20 * i, std::ios_base::beg);
				in >> error;
				res += error + " ";
			}
		}
		in.close();
		return res;
	}
	//������� ��� �������
	friend std::ostream & operator <<(std::ostream & _os, const my_queue<T> & _q) {
		Node *pointer;
		for (unsigned i = _q.size; i > 0; --i) {
			pointer = _q.head;
			for (unsigned j = 1; j < i; ++j) {
				pointer = pointer->pnext;
			}
			_os << pointer->elem << " ";
		}
		return _os;
	}
	//�������� �������
	my_queue <T> & operator =(const my_queue<T> & _q) {
		this->~my_queue();
		Node *pointer;
		for (unsigned i = _q.size; i > 0; --i) {
			pointer = _q.head;
			for (unsigned j = 1; j < i; ++j) {
				pointer = pointer->pnext;
			}
			this->push(pointer->elem);
		}
		return *this;
	}
	//����������� ������� ������� 1: ����� ��� ���� ������� (���������� �������� n + (n^2)/2)
	void invert_q() {
		my_queue<T> helper;
		Node *pointer = head;
		while (pointer != nullptr) {
			helper.push(pointer->elem);
			pointer = pointer->pnext;
		}
		for (unsigned i = size; i > 0; --i) {
			pointer = head;
			for (unsigned j = 1; j < i; ++j) {
				pointer = pointer->pnext;
			}
			pointer->elem = helper.front();
		}
	}
	//����������� ������� ������� 2 : ����� ������������ ������(���������� �������� 2n)
	void invert_a() {
		T *arr = new T[size];
		unsigned i = 0;
		while (size) {
			arr[i] = front();
			i++;
		}
		while (i) {
			i--;
			push(arr[i]);
		}
		delete[] arr;
	}
	//��������� ������� 1:����� ������������ ������ ���������(���������� �������� 3n + (n^2)/2(��� ����� ����������))
	//��������� �.�. ���� ���� ������ ����
	friend void sort_a(my_queue <T> & _q, my_queue <T> & _p, my_queue <T> & res) {
		unsigned size1 = _q.size, size2 = _p.size;
		T *arr = new T[size1 + size2];
		int i = _q.size - 1;
		while (_q.size) {
			arr[i] = _q.front();
			i--;
		}
		for (i = size1 - 1; i >= 0; --i) {
			_q.push(arr[i]);
		}
		i = _q.size + _p.size - 1;
		while (_p.size) {
			arr[i] = _p.front();
			i--;
		}
		for (i = size1 + size2 - 1; i >= _q.size; --i) {
			_p.push(arr[i]);
		}
		for (i = 1; i < size1 + size2; ++i) {
			for (unsigned j = i; j > 0 && arr[j - 1] > arr[j]; --j) {
				T tmp = arr[j - 1];
				arr[j - 1] = arr[j];
				arr[j] = tmp;
			}
		}
		for (i = 0; i < size1 + size2; ++i) {
			res.push(arr[i]);
		}
		delete[]arr;
	}
	//���������� ��� ��� ������� (��� ����� ����� ������� ��� ���� �������)
	friend void sort_q(my_queue <T> & _q, my_queue <T> & _p, my_queue <T> & res) {
		res.~my_queue();
		unsigned size1 = _q.size, size2 = _p.size;
		for (int i = 0; i < size1; ++i) {
			res.push(_q[i]);
		}
		for (int i = 0; i < size2; ++i) {
			res.push(_p[i]);
		}
		for (int i = 1; i < size1 + size2; ++i) {
			for (unsigned j = i; j > 0 && res[j - 1] > res[j]; --j) {
				T tmp = res[j - 1];
				res[j - 1] = res[j];
				res[j] = tmp;
			}
		}
	}
};
#endif // !MY_QUEUE;
