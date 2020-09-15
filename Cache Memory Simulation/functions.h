#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include<bits/stdc++.h>
#include <iostream> 
#include <string>
#define ll long long
#define endl '\n'
#define separator cout<<"---------------------------------------------------------------------"<<endl;
using namespace std;


class block
{
public:
	string tag;
	unsigned long long data;
	bool dirty;
	int lastupdate;
	block(string tag, unsigned long long data, int lastupdate)
	{
		this->tag = tag;
		this->data = data;
		this->lastupdate = lastupdate;
	}

	string getTag()
	{
		string str="0";
		int i=0;
		for(i=0; i<tag.length(); i++)
		{
			if(tag[i] == '0')
				continue;
			else break;
		}
		str += tag.substr(i, tag.length()-i);
		return str;
	}
};

class group 
{
public:
	vector<block*> blocks;
	group()
	{
		blocks.clear();
	}
	int size()
	{
		return this->blocks.size();
	}
	void insert(block* b)
	{
		this->blocks.push_back(b);
	}
};

class Set 
{
public:
	group* HIGH;
	group* LOW;
	int size;
	Set(int size)
	{
		HIGH = new group();
		LOW = new group();
		this->size = size;		
	}
	int validBlocks()
	{
		return (HIGH->size()+LOW->size());
	}
};

int getAddr(string cmd) {
	if(cmd.size() <2)
		return -1;
	stringstream ss(cmd);
	int ret;
	ss>>ret;
	return ret;
}

char getType(string cmd) {
	char ret;
	for(int i=0; i<cmd.size(); i++)
	{
		if(cmd[i]=='W' || cmd[i] == 'R')
		{
			ret = cmd[i];
			return ret;
		}
	}
	return 'i';
}

unsigned long long getData(string cmd) {
	string ret = "";
	for(int i=cmd.size()-1; i>=0; i--)
	{
		if( i != cmd.size()-1 && (cmd[i] == ',' || cmd[i] == ' ') )
			break;
		ret = cmd[i] + ret;
	}
	stringstream ss(ret);
	unsigned long long ret1;
	ss>>ret1;
	return ret1;
}

int getOffsetLength(int blockSize) {
	if(blockSize == 1)
		return 0;	
	else return 1+getOffsetLength(blockSize/2);
}

string getTag(string addr, int tagbits) {
	string ret = "";
	for(int i=0; i<tagbits; i++)
		ret = ret + addr[i];
	return ret;
}

string getSetLocation(string addrbin, int tagbits, int setAddbits) {
	string ret = "";
	for(int i=tagbits; i<(tagbits+setAddbits); i++)
		ret += addrbin[i];
	return ret;
}

bool isHit(Set* s, string tag) {
	for(int i=0; i<s->HIGH->size(); i++)
	{
		if(s->HIGH->blocks[i]->tag == tag)
			return true;
	}
	for(int i=0; i<s->LOW->size(); i++)
	{
		if(s->LOW->blocks[i]->tag == tag)
			return true;
	}
	return false;
}

void updateData(Set* s, string tag, unsigned long long data, int clock, char type) {
	for(int i=0; i<s->HIGH->size(); i++)
	{
		if(s->HIGH->blocks[i]->tag == tag)
		{
			if(type == 'W') {
				s->HIGH->blocks[i]->data = data;
				s->HIGH->blocks[i]->dirty = true;
			}
			s->HIGH->blocks[i]->lastupdate = clock;
			return;
		}
	}
	for(int i=0; i<s->LOW->size(); i++)
	{
		if(s->LOW->blocks[i]->tag == tag)
		{
			block* b = new block(s->LOW->blocks[i]->tag, s->LOW->blocks[i]->data, clock);
			b->dirty=s->LOW->blocks[i]->dirty;
			s->LOW->blocks.erase(s->LOW->blocks.begin()+i);
			if(type == 'W') {
				b->data = data;
				b->dirty = true;
			}
			s->HIGH->insert(b);
			return;
		}
	}
	cout<<"update Data impossible\n";
	return;
}
pair<string, int> eraseBlock(group* g, block* b)
{
	int index = 0;
	int time = g->blocks[0]->lastupdate;
	for(int i=0; i<g->size(); i++)
	{
		if(time > g->blocks[i]->lastupdate)
		{
			index = i;
			time = g->blocks[index]->lastupdate;
		}
	}
	pair<string, int> p;
	p.first = g->blocks[index]->tag;
	p.second = g->blocks[index]->data;
	g->blocks.erase(g->blocks.begin()+index);//[index] = b;
	return p;

}
pair<string, int> insertBlock(Set* s, block* b) {
	pair<string, int> p;
	int invalid = s->size - s->validBlocks();
	if(invalid != 0)
	{
		s->LOW->insert(b);
		p = {"null", 0};
	}
	else {
		if(s->LOW->size() != 0)
		{
			p=eraseBlock(s->LOW, b);
		}
		else p=eraseBlock(s->HIGH, b);
		s->LOW->insert(b);
	}
	return p;
}

void showBlock(block* b) {
		cout<<"tag(binary) : "<<b->getTag()<<" data(int) : "<<b->data<<" lastupdate : "<<b->lastupdate<<" valid : "<<1<<" dirty : "<<b->dirty<<endl;
}

void showSet(Set* s) {
	for(int i=0; i<s->HIGH->size(); i++)
	{
		cout<<"HIGH ";
		showBlock(s->HIGH->blocks[i]);
	}
	for(int i=0; i<s->LOW->size(); i++)
	{
		cout<<"LOW ";
		showBlock(s->LOW->blocks[i]);
	}
	for(int i=0; i<(s->size) - (s->validBlocks()); i++)
	{
		cout<<"LOW tag(binary) : 0 data(int) : 0 lastupdate : 0 valid : 0 dirty : 0"<<endl;
	}
}

void showSets(vector<Set*> v) {
	separator
	for(int i=0; i<v.size(); i++) {
		cout<<"set "<<i<<endl;
		showSet(v[i]);
		separator
	}
}

void updatePriorities(Set* s, int clock, int T) {
	for(int i=0; i<s->HIGH->size(); i++)
	{
		if(clock - s->HIGH->blocks[i]->lastupdate >= T)
		{
			block *b = s->HIGH->blocks[i];
			s->LOW->insert(b);
			s->HIGH->blocks.erase(s->HIGH->blocks.begin()+i);
			i=i-1;
		}
	}
}

void updatePriorities(vector<Set*> sets, int clock, int T) {
	for(int i=0; i<sets.size(); i++)
		updatePriorities(sets[i], clock, T);
}

int readData(Set* currSet, string tag) {
	for(int i=0; i<currSet->HIGH->size(); i++) {
		if(currSet->HIGH->blocks[i]->tag == tag)
			return currSet->HIGH->blocks[i]->data;
	}
	cout<<"ERROR_IN_READDATA\n";
	exit(1);
}

unsigned long long getMaxData(int blockSize) {

	unsigned long long ret = pow(2, blockSize);
	ret = ret*ret;
	ret = ret*ret;
	ret = ret*ret;
	return (ret-1);
}
bool isValid(int blockSize)
{
	string binary = bitset<32> (blockSize).to_string();
	int count = 0;
	for(int i=0; i<binary.length(); i++)
	{
		if(binary[i] == '1') count++;
	}
	if(count==1)
		return true;
	return false;
}

bool isValid(int cacheSize, int blockSize, int setAssoc) 
{
	int setsize = blockSize*setAssoc;
	if(blockSize>8)
	{
		cout<<"block size too high\n";
		return false;
	}
	if(setsize == 0)
		return false;
	if(cacheSize%setsize == 0)
	{
		if(isValid(blockSize) && isValid(setAssoc))
			return true;
	}
	return false;
}

#endif