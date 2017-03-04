#include<bits/stdc++.h>
using namespace std;

int main () {
	int n,m;
	cin >> n >> m;

	vector< pair<int,int> > edges;

	for(int i=0;i<m;i++) {
		int a,b;
		cin >> a >> b;
		a--;
		b--;
		edges.emplace_back(a,b);
	}

	int k;
	cin >> k;
	
	for(int mask=0;mask<(1<<n);mask++) if(__builtin_popcount(mask) == k) {
		bool ok = true;
		
		for(auto p: edges) {
			int v = p.first;
			int w = p.second;
			if(!(mask&(1<<v)) and !(mask&(1<<w))) {
				ok = false;
				break;
			}
		}

		if(!ok)
			continue;

		cout << "TAK\n";
		return 0;
	}

	cout << "NIE\n";
	return 0;
}	
