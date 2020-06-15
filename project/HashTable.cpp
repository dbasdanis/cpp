/*
 * Basdanis Dionisis 2166
 * Antoniou Theodoros 2208
 * CE325 - C++ project 
 */

#include "HashTable.h"
#include "HashTableException.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int HashTable::getHashCode(const char *str) {
  int code = 0;
  while(*str != 0) {
    code += *(str++);
  }
  return code;
}

int HashTable::getSize() const { return size; }
int HashTable::getCapacity() const {return capacity; }

bool HashTable::isEmpty(int pos) const { 
	if(pos<0 || pos>(capacity-1))
		return false;
	if(table[pos].empty()) 
		return true;
	return false;
}

bool HashTable::isTomb(int pos) const {
	if(pos<0 || pos>(capacity-1))
		return false;
	if(table[pos].compare("##tomb##")==0)
		return true; 
	return false;
}

bool HashTable::isAvailable(int pos) const { 
	if(isEmpty(pos) || isTomb(pos))
		return true;
	return false;
}

HashTable::HashTable(int capacity)  {
	table =  new string[capacity];
	// try{
	// 	table = new string[capacity];
	// }catch(std::bad_alloc){
	// 	cerr<<"std::bad_alloc occured\n";
	// 	exit(-1);
	// }
	
	for (int i = 0; i < capacity; i++)
		table[i] = "";
	this->size = 0;
	this->capacity = capacity;
}

HashTable::HashTable(const HashTable &ht) {
	this->capacity = ht.capacity;
	this->size = ht.size;
	table = new string[capacity];
	for (int i = 0; i < capacity; i++)
		this->table[i] = ht.table[i];
}

bool HashTable::contains(const string &s) const {
	int counter=0, pos, init_pos, x=0;

	for(int i=0; i<s.length(); i++){
		x += s[i];
	}
	
	init_pos = (counter + x) % capacity;
	pos = init_pos;
	counter = 0;

	do{
		if(isEmpty(pos))
			return false;
		if(table[pos].compare(s) == 0)
			return true;

		counter++;
		pos = (counter + x) % capacity;

	}while(pos != init_pos);

	return false;
}

bool HashTable::contains(const char *s) const {

	return contains(string(s));

}

string HashTable::print() const {
  string str;
  char buf[128];
  int j=0;
/* Remove the following comment if you want 
 * to try the iterator version.
 */
// #define __USE_ITERATOR__
#ifdef  __USE_ITERATOR__
  for(HashTable::Iterator it = begin(); it!=end(); it++) {
    sprintf(buf, "%2d. -%s-\n", j++, (*it).c_str());
    str.append(buf);
  }
#else
  for(int i=0, j=0; i<capacity; i++)
    if(!isAvailable(i)) {
      sprintf(buf, "%2d. -%s-\n", j++, table[i].c_str());
      str.append(buf);
    }
#endif
  sprintf(buf, " --- CAPACITY: %d, SIZE: %d ---\n", capacity, size);
  str.append(buf);
  return str;
}

bool HashTable::add(const string &str) {
	
	int counter=0;
	int pos, init_pos;
	int x=0;
	for(int i=0; i<str.length(); i++){
		x += str[i];
	}

	if(contains(str))
		return false;
	
	init_pos = (counter + x) % capacity;
	pos = init_pos;
	counter = 0;
	do{
		if(isEmpty(pos) || isTomb(pos)){
			table[pos] =  str;
			size++;
			return true;
		}
		counter ++;
		pos = (counter + x) % capacity;

	}while(init_pos != pos);

	cout << "THROW Exception!" << endl;
	throw HashTableException();
	return false;
}

bool HashTable::add(const char *s) {

	return add(string(s));
	
}

bool HashTable::remove(const string &str) {
	int counter = 0, pos, init_pos, x=0;
	
	for(int i=0; i<str.length(); i++){
		x += str[i];
	}

	init_pos = (counter + x) % capacity;
	pos = init_pos;
	do{
		if(table[pos].compare(str) == 0){
			table[pos] = "##tomb##";
			size--;
			return true;
		}
		counter++;
		pos = (counter + x) % capacity;

	}while(init_pos != pos);

	return false;
}

bool HashTable::remove(const char *s) {

	return remove(string(s));
	
}

HashTable& HashTable::operator=(const HashTable &ht) {
  
	delete[] table;
	this->capacity = ht.getCapacity();
	this->size = ht.getSize();
	this->table = new string[capacity];
	if(this->table == NULL){
		cout << "Error on allocation memory\n";
		exit(-1);
	}

	for(int i=0; i<capacity; i++){
		this->table[i] =  ht.table[i];
	}   

	return *this;
}

bool HashTable::operator += (const string &str) { 
 
 	return add(str);

}
bool HashTable::operator += (const char* s) { 
	
	return add(s);

}
bool HashTable::operator -= (const string &str) {

	return remove(str);
  
}
bool HashTable::operator -= (const char *s) { 

	return remove(s);
  
}

HashTable HashTable::operator + (const string &str) const {

	HashTable newTable(*this);

	if(!this->contains(str))
		newTable.add(str);

	return newTable;
}

HashTable HashTable::operator + (const char* s) const {
	HashTable newTable(*this);

	if(!this->contains(s))
		newTable.add(s);

	return newTable;
}

HashTable HashTable::operator - (const string &str) const {
	HashTable newTable(*this);

	if(this->contains(str))
		newTable.remove(str);

	return newTable;
}

HashTable HashTable::operator - (const char *s) const {
	HashTable newTable(*this);

	if(this->contains(s))
		newTable.remove(s);

	return newTable;
}

HashTable HashTable::operator+(const HashTable &t) const {
	int newCapacity = capacity + t.getCapacity();
	HashTable newTable(newCapacity);
	
	for(int i=0; i<this->capacity; i++){
		if(!isEmpty(i) && !isTomb(i))
			newTable.add(this->table[i]);
	}

	for(int i=0; i<t.capacity; i++){
		if(!t.isEmpty(i) && !t.isTomb(i))
			newTable.add(t.table[i]);
	}
	
	return newTable;
}

HashTable& HashTable::operator+=(const HashTable &t) {
	int old_capacity = this->capacity;
	this->capacity = this->capacity + t.getCapacity();
	

	string *buffer = new string[size];
	int count = 0;
	for(int i=0; i<old_capacity; i++){
		if(table[i].compare("") != 0 && table[i].compare("##tomb##") !=0 ){
			buffer[count] = table[i];
		 	count++;
		}	
	}
	delete [] table;
	table = new string[capacity];
	if (table == NULL){
		cout << "Allocation problem"<<endl;
		exit(-1);
	}

	size = 0;
	for(int i=0; i<count; i++){
		add(buffer[i]);
	}

	delete [] buffer;

	for(int i=0; i<t.getCapacity(); i++){
		if(!t.isTomb(i) && !t.isEmpty(i)){
			this->add(t.table[i]);
		}
	}
	
	return *this;
}

std::ostream& operator<<(std::ostream &os, HashTable &t) {
	
	os << t.print();

	return os;
}

HashTable::Iterator HashTable::begin() const {
	Iterator newIt(this);
	return newIt;
}

HashTable::Iterator HashTable::end() const {
	Iterator newIt(this,false);
	return newIt;
}