#include "../src/vector.hpp"
//#include <vector>
#include <iostream>
//#include <list>
#include "../src/list.hpp"

template<class T>
void print(lix::vector<T> vec) {
	auto it = vec.begin();
	for (; it != vec.end(); ++it) std::cout << *it;
	std::cout << std::endl;
}

struct x
{
	double arr[10000];
};

int main(int argc, char* argv[]) {
	lix::vector<x> *v=new lix::vector<x>(30000);
	lix::list<int> l{};
	l.begin();
	/*v.push_back(3);

	for (int i = 0; i < 10; i++) v.push_back(i);
	lix::vector<int> x = v;
	print(x);
	x.erase(x.begin() + 3,x.end()-3);
	print(x);*/
	//print(x);
	delete v;
	system("pause");

}
