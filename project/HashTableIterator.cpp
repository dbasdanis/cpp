/*
 * Basdanis Dionisis 2166
 * Antoniou Theodoros 2208
 * CE325 - C++ project 
 */


#include "HashTable.h"
#include "HashTableException.h"
#include <stdio.h>

using namespace std;

HashTable::Iterator::Iterator(const HashTable *t) {
	this->ht = t;
	this->curr = t->table;
}

HashTable::Iterator::Iterator(const HashTable *t, bool start){
	
	this->ht = t;
	if(start == false){
		this->curr = &(t->table[t->getCapacity()-1]); 
		(this->curr)++;
	}
	else{
		this->curr = t->table;
	}
}

HashTable::Iterator::Iterator(const HashTable::Iterator &it) {
	this->curr = it.curr;
	this->ht = it.ht;
}

HashTable::Iterator& HashTable::Iterator::operator=(const HashTable::Iterator &it) {
	this->curr = it.curr;
	this->ht = it.ht;
	return *this;
}
 
HashTable::Iterator HashTable::Iterator::operator++() {
	string str = *this->curr;
	int i =0;
	for(i=0;i<this->ht->capacity;i++){
		if(this->ht->table[i].compare(str) == 0)
			break;
	}
	i++;
	while(this->ht->isEmpty(i) || this->ht->isTomb(i)){
		i++;
	}
	this->curr = &(this->ht->table[i]);
	return *this;
}
 
HashTable::Iterator HashTable::Iterator::operator++(int a) {
	Iterator newIt(*this);
	
	string str = *this->curr;
	
	int i;
	for(i=0;i<this->ht->capacity;i++){
		if(this->ht->table[i].compare(str) == 0){
			break;
		}
	}
	i++;
	while(this->ht->isEmpty(i)||this->ht->isTomb(i)){
		i++;
	}
	this->curr = &this->ht->table[i];
	return newIt;
}

bool HashTable::Iterator::operator==(const HashTable::Iterator &it) const {
	string str1 = *this->curr;
	string str2 = *it.curr;
	if(str1.compare(str2)==0)
		return true;
	return false;
}

bool HashTable::Iterator::operator!=(const HashTable::Iterator &it) const {

	if((this->curr) == it.curr)
		return false;
	return true;  
}

const string& HashTable::Iterator::operator*() {
	return *this->curr;  
}

const string* HashTable::Iterator::operator->() {
	return this->curr;  
}