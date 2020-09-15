#include <bits/stdc++.h>

using namespace std;

unsigned long long maxdata1(int d)
{
	unsigned long long ret = pow(2, d);
	ret = ret*ret;
	ret = ret*ret;
	ret = ret*ret;
	return (ret-1);
}

int main()
{
	unsigned long long int N,a,b,c;
	cin>>N>>a>>b>>c;
	cout<<N<<endl<<a<<endl<<b<<endl<<c<<endl;
	unsigned long long maxdata = maxdata1(a);
	int i=0;
	while(true)
	{
		i++;
		if(i==10001)
			break;
		int a = rand()%2;
		if(a==0)
		cout<<rand()%100<<", W, "<<(rand()%maxdata)<<endl;
		else cout<<rand()%1000<<", R"<<endl;

	}
}
