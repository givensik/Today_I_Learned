#include <iostream>
#include <set>

template <typename T>
void print_set(std::set<T>& s){
	for(typename std::set<T>::iterator itr = s.begin(); itr != s.end() ; itr++ ){
		std::cout << *itr << std::endl;
 	}
}

class ToDo{
	int priority;
	std::string job_desc;

	public : 
	ToDo::ToDo(int priority, std::string job_desc) : priority(priority), job_desc(job_desc){}	

	void print_job(int priority){
		
	}
};

int main(){

	std::set<int> s;

	s.insert(1);
	s.insert(2);
	s.insert(3);
	s.insert(4);

	print_set(s);	

	return 0;
}