#include<bits/stdc++.h>
#include <bitset>
#define ll long long
#define endl '\n'
#include "functions.h"

using namespace std;

void solve()
{   
	int cacheSize; //size of cache in bytes
	cin>>cacheSize;
	int blockSize; //size of block in bytes
	cin>>blockSize;
	int setAssoc; //set associativity
	cin>>setAssoc;
	int T;
	cin>>T;
	int offsetbits=0;
	if(!isValid(cacheSize, blockSize, setAssoc))
	{
		cout<<"INVALID_DATA\n";
		return;
	}
	offsetbits = getOffsetLength(blockSize); // how much offset bits should we take from address
	int totalsets = cacheSize/(blockSize*setAssoc);
	int setAddbits = getOffsetLength(totalsets); //no. of bits to select the respective set
	int tagbits = 32 - (setAddbits+offsetbits); //remaining are tag bits
	vector<Set*> sets; //cache
	map<pair<string, string>, int> m; //main memory : set location(binary), tag(binary), data(int)
	for(int i=0; i<totalsets; i++) {
		Set* s = new Set(setAssoc);
		sets.push_back(s);
	}
	cout<<"offset bits : "<<offsetbits<<endl;
	cout<<"total sets : "<<totalsets<<endl;
	cout<<"setAdd bits : "<<setAddbits<<endl;
	cout<<"tag bits : "<<tagbits<<endl;
	unsigned long long MAX_DATA_VALUE = getMaxData(blockSize);
	cout<<MAX_DATA_VALUE<<endl;
	showSets(sets);
	string cmd;
	int miss=0;
	int hits=0;
	int clock = 0;
	int reads=0;
	int readhits=0;
	while(getline(cin, cmd)) {
		unsigned long long data=0;
		int addr=-1;
		char type;
		addr = getAddr(cmd);
		type = getType(cmd);
		if(type == 'R')
		reads++;
		cout<<cmd<<endl;
		if(addr == -1 || type == 'i') {//if access is not read or write
			continue;}
		if(type == 'W')
			data = getData(cmd);
		if(data > MAX_DATA_VALUE) {//if data is invalid
			cout<<"INVALID DATA1 "<<data<<"\n"<<MAX_DATA_VALUE<<endl;
			return;
		}
		clock++;
		cout<<"access : ["<<cmd<<"]\taccess no. : "<<clock<<endl;
		string addrbin = bitset<32> (addr).to_string(); //address in binary
		string tag = getTag(addrbin, tagbits); //tag in binary
		string setlocs = getSetLocation(addrbin, tagbits, setAddbits); //set address in binary
		int setloc = stoi(setlocs, 0, 2); //set address in integer
		cout<<"change in set : "<<setloc<<"\t";
		Set* currSet = sets[setloc];
		if(isHit(currSet, tag)) { //if it is a HIT
			hits++;
			cout<<"HIT\t";
			updateData(currSet, tag, data, clock, type); //if 'W' update data and clock else if 'R' then only clock and move to HIGH priority group
			if(type == 'R') {
				readhits++;
				data = readData(currSet, tag); //if 'R' then read the data
			}
		}
		else {
			miss++;
			cout<<"MISS\t";
			if(type == 'R') {
				pair<string, string> p;
				p.first = tag;
				p.second = setloc;
				if(m.find(p) != m.end()) //check the valud of the corresponding block in main memory to copy it to cache
					data = m[p];
				else {
					data = rand()%MAX_DATA_VALUE; //assign a random value to the block in main memory
					m[p] = data;
				}
			}
			block* tbinsert = new block(tag, data, clock);
			tbinsert->dirty = false;
			if(type == 'W')
				tbinsert->dirty = true;
			pair<string, int> p = insertBlock(currSet, tbinsert); //returns the block that is erased from cache, null if isn't 
			if(p.first != "null") { //write back
				pair<string, string> p1;
				p1.first = p.first;
				p1.second = setloc;
				m[p1] = p.second;
			}
		}
		updatePriorities(sets, clock, T); //move high to low priority if a HIGH priority block has not been modified upto T cache access.
		cout<<"accessed data : "<<data<<endl;
		showSets(sets);
		cout<<"\n\n";
	}
	cout<<"\n\nCache Statistics : "<<endl;
	cout<<"Number of accesses : "<<clock<<endl;
	cout<<"Number of reads : "<<reads<<endl;
	cout<<"Number of read hits : "<<readhits<<endl;
	cout<<"Number of read misses : "<<reads-readhits<<endl;
	cout<<"Number of Writes : "<<(clock-reads)<<endl;
	cout<<"Number of Write Hits : "<<(hits-readhits)<<endl;
	cout<<"Number of Write Misses : "<<(miss-(reads-readhits))<<endl;
	cout<<"total hits : "<<hits<<endl;
	cout<<"total miss : "<<miss<<endl;
	cout<<"Hit ratio : "<<(hits+0.0)/(clock)<<endl;
}
 
signed main()
{
  	solve(); 
	return 0;
}