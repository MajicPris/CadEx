#include<iostream>
using namespace std;

class countManager {
private:
	int count;
public:
	countManager() :count(0) { };
	void dec() {
		--count;
	}
	void inc() {
		++count;
	}
	int getCount() {
		return count;
	}
};

template <class T>
class my_shared_ptr {
private:
	T * ptr;
	countManager* count_ptr;
public:
	my_shared_ptr() : ptr(0), count_ptr(0) {
		count_ptr = new countManager();
		count_ptr->inc();
	}

	my_shared_ptr(T * myPtr) : ptr(myPtr){
		count_ptr = new countManager();
		count_ptr->inc();
	}
	//copy 

	my_shared_ptr(const my_shared_ptr &object){
		cout << "copy" << endl;
		ptr = object.ptr;
		count_ptr = object.count_ptr;
		count_ptr->inc();
	}

	~my_shared_ptr(){
		count_ptr->dec();
		if (count_ptr->getCount()==0) {
			cout << "delete" << endl;	
			delete ptr;
			delete count_ptr;
		}
	}

	my_shared_ptr operator=(const my_shared_ptr &object) {
		if (this != &object)
		{
			count_ptr->dec();
			if (count_ptr->getCount() == 0) {
				cout << "delete =" << endl;
				delete ptr;
				delete count_ptr;
			}
			ptr = object.ptr;
			count_ptr = object.count_ptr;
			count_ptr->inc();
			cout << "=" << endl;

		}
		return *this;
	}
	int getPtr() {
		return count_ptr->getCount();
	}
};

int main()
{
	//int *a = new int;
	my_shared_ptr<int> b(new int(10));
	cout << b.getPtr() << endl;
	my_shared_ptr<int> b1(new int(30));
	cout << b1.getPtr() << endl;
	b1 = b;
	cout << b.getPtr() << endl;
	cout << b1.getPtr() << endl;

    return 0;
}

