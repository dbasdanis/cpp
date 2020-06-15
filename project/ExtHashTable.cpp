/*
 * Basdanis Dionisis 2166
 * Antoniou Theodoros 2208
 * CE325 - C++ project 
 */

#include <iostream>
#include <cstdlib>
#include <string.h>

#include "ExtHashTable.h"

static int flag = -1; //0 -> add, 1 -> remove

ExtHashTable::ExtHashTable( double upper_bound_ratio, double lower_bound_ratio,int size) : HashTable(size){
	this->upper_bound_ratio = upper_bound_ratio;
	this->lower_bound_ratio = lower_bound_ratio; 
}
  
ExtHashTable::ExtHashTable(const ExtHashTable &t) : HashTable(t) {
	this->upper_bound_ratio = t.upper_bound_ratio;
	this->lower_bound_ratio = t.lower_bound_ratio;  
}

void ExtHashTable::rehash() {
	int old_capacity = capacity;
 
	if(flag == 0){
    	if((double(size) / double(capacity)) > upper_bound_ratio){
    		capacity = capacity*2;
    	}
    	else
    		return;
  	}
  	else if(flag == 1){
    	if((double(size) / double(capacity)) < lower_bound_ratio && capacity > 8){
    		capacity = capacity / 2;
    	}
    	else
    		return;
  	}
  	else{
  		cout << "flag = -1" << endl;
  		return;
  	}

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
	int length = size;
	size = 0;
	for(int i=0; i<length; i++){
		this->HashTable::add(buffer[i]);
	}
	delete [] buffer;

  	cout << "--> Size: " << this->size << ", New capacity: " << this->capacity << endl;

  	return;
}

bool ExtHashTable::add(const string &str) {
	flag = 0;
	bool result = HashTable::add(str);
	this->rehash();
	return result;
} 
bool ExtHashTable::add(const char *s) {
	return add(string(s));
}

bool ExtHashTable::remove(const string &str) {
	flag = 1;
	bool result = HashTable::remove(str); 
	this->rehash();
	return result;
}

bool ExtHashTable::remove(const char *s) {
	return remove(string(s));
}

ExtHashTable& ExtHashTable::operator=(const ExtHashTable &t){

	delete [] table;
	capacity = t.getCapacity();
	size = t.getSize();
	table = new string[capacity];
	for(int i=0; i<capacity; i++){
		this->table[i] =  t.table[i];
	}   
                                                                                                       
	return *this;
}

ExtHashTable ExtHashTable::operator+(const string &str) const {
	ExtHashTable newTable(*this);

	if(!newTable.contains(str))
		newTable.add(str);

	return newTable;
}

ExtHashTable ExtHashTable::operator+(const char* s) const {
	return operator+(string(s));
}

ExtHashTable ExtHashTable::operator-(const string &str) const{
	ExtHashTable newTable(*this);

	if(newTable.contains(str))
		newTable.remove(str);

	return newTable;	
}

ExtHashTable ExtHashTable::operator-(const char *s) const{
	return operator-(string(s));	
}

bool ExtHashTable::operator += (const string &str) { 
	return add(str);	
}

bool ExtHashTable::operator += (const char* s) { 
	return add(s);  
}

bool ExtHashTable::operator -= (const string &str) {
	return remove(str);  
}

bool ExtHashTable::operator -= (const char *s) {
	return remove(s);  
}

ExtHashTable ExtHashTable::operator+(const ExtHashTable &t) const {
	ExtHashTable newTable(*this);
	
	for(int i=0; i<t.getCapacity(); i++){
		if(!t.isTomb(i) && !t.isEmpty(i)){
			newTable.add(t.table[i]);
		}
	}	

	return newTable;
}

ExtHashTable & ExtHashTable::operator+=(const ExtHashTable &t) {
	for(int j=0; j<t.getCapacity(); j++){
		if(!t.isEmpty(j) && !t.isTomb(j))
			this->add(t.table[j]);	
	} 

	return *this;
}
