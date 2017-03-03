#include<bits/stdc++.h>
using namespace std;

#define RAND_VERTEX_SIZE_MAX 100
#define RAND_EDGES_SIZE_MAX 1000
#define K_RAND_MAX 10

#ifndef DEBUG
#define cerr if(0) cerr
#endif

typedef vector< int > VertexList;
#define VERTEX_LIST_NULL (vector<int>){-1}
typedef vector< pair < int, int >> Matching;
typedef tuple <VertexList, VertexList, VertexList> Crown;
#define CROWN_NULL make_tuple(VERTEX_LIST_NULL, VERTEX_LIST_NULL, VERTEX_LIST_NULL)

void debug_vl(VertexList& li) {
	cerr << "	";
	for(int v: li)
		cerr << v << " ";
	cerr << endl;
}

struct Graph {
	unordered_map < int, VertexList > adj;
	VertexList vertexes;
	
	Graph() {
		adj.clear();
		vertexes.clear();
	}

	void add_n_vertexes(int n) {
		for(int i=1;i<=n;i++)
			vertexes.push_back(i);
	}

	void set_to_null() {
		vertexes.clear();
		vertexes.push_back(-1);
	}

	bool is_null() {
		return (int)vertexes.size() == 1 and vertexes[0] == -1;
	}	

	void erase_vertex(int v) {
		vertexes.erase(find(vertexes.begin(), vertexes.end(), v));
		for(int w: adj[v]) {
			adj[w].erase(find(adj[w].begin(), adj[w].end(), v));
		}
		adj[v].clear();
	}

	void debug(string msg) {
		cerr << "Graph structure of " << msg << endl;
		cerr << "	vertexes: ";
		for(int v: vertexes)
			cerr << v << " ";
		cerr << endl;
		cerr << "	edges: " << endl;
		for(int v: vertexes) {
			cerr << "	" << v << ": ";
			for(int w: adj[v]) 
				cerr << w << " ";
			cerr << endl;
		}
		cerr << "------------------------" << endl;
	}

	void input() {
		int n, m;
		cin >> n >> m;
		
		while(m--) {
			int a,b;
			cin >> a >> b;
			adj[a].push_back(b);
			adj[b].push_back(a);
		}
		
		add_n_vertexes(n);
	}

	void random_gen() {
		cerr << "Graph" << endl;
		int n = rand()%RAND_VERTEX_SIZE_MAX + 1;
		int m = rand()%(RAND_EDGES_SIZE_MAX + 1);
		cerr << "n: " << n << " m: " << m << endl;

		add_n_vertexes(n);

		while(m--) {
			int a,b;
			a = b = 1;
			while( a == b ) {
				a = rand()%n + 1;
				b = rand()%n + 1;
				adj[a].push_back(b);
				adj[b].push_back(a);
				cerr << a << " <-> " << b << endl;
			}
		}
	}

	VertexList vertex_cover_from_matching(Matching matching) {
		VertexList answer;
		answer.reserve(matching.size());
		for(auto p: matching) {
			int v = p.first;
			int w = p.second;
			int to_add = -1;
			if(adj[v].size() == 1) {
				to_add = w;
			} else if(adj[w].size() == 1) {
				to_add = v;
			} else {
				cerr << "ERROR: MATCHING IS NOT MAXIMAL\n";
			}
			assert(to_add != -1);
			answer.push_back(to_add);
		}
		return answer;
	}

	Matching turbo_matching(vector<int> left_set) {	
		unordered_map<int,bool> visited;
		unordered_map<int,bool> matched_with;

		function<bool(int)> find_matching_path = [&](int start_vertex) {
			visited[start_vertex] = true;
	
			for(int w: adj[start_vertex]) {
				if(visited[w]) continue;
				if(!matched_with[w]) {
					matched_with[w] = start_vertex;
					matched_with[start_vertex] = w;
					return true;
				} else {
					int u = matched_with[w];
					if(!visited[u] and find_matching_path(u)) {
						matched_with[start_vertex] = w;
						matched_with[w] = start_vertex;
						return true;
					}
				}
			}
			return false;
		};

		while(true) {
			for(int v: vertexes)
				visited[v] = true;

			bool found_one = false;

			for(int v: left_set) 
				if(!visited[v] and !matched_with[v] and find_matching_path(v))
					found_one = true;

			if(!found_one)
				break;
		}

		Matching answer;

		for(int v: left_set) {
			if(matched_with[v])
				answer.emplace_back(v, matched_with[v]);
		}

		return answer;
	}

	Matching ls_general_matching() {
		unordered_map<int, bool> added;
		Matching answer;

		for(int v: vertexes) if(!added[v]) {
			for(int w: adj[v]) if(!added[w]) {
				added[v] = true;
				added[w] = true;
				answer.emplace_back(v,w);
			}
		}
		return answer;
	}

	bool isolated(int v) {
		return adj[v].size() == 0;
	}

	VertexList k_vertex_cover(int k) {
		VertexList cover;
		k_vertex_cover_kernel(k, cover).k_vertex_cover_easy(k, cover);
		return cover;
	}

	Graph k_vertex_cover_kernel(int k, VertexList& cover_result) {
		debug("licze na tym kernel " + to_string(k));

		// Rule #1 Removing isolated vertexes
		for(int i=0;i<(int)vertexes.size();i++) {
			if(isolated(vertexes[i])) {
				vertexes.erase(vertexes.begin() + i);
				i--;
			}
		}
		
		if((int)vertexes.size() > k * 3) {
			vector<int> H,C,R;
			tie(H,C,R) = crown_decomposition(k);

			cerr << "Crown Decomposition " << endl;
			cerr << "H: " << endl;
			debug_vl(H);
			cerr << "C: " << endl;
			debug_vl(C);
			cerr << "R: " << endl;
			debug_vl(R);

			if(H[0] == -1) {
				Graph kernel = Graph();
				kernel.set_to_null();
				return kernel;

			} else {
				Graph nowy;
				for(int v: H)
					cover_result.push_back(v);

				map<int,bool> inR;

				for(int v: R)
					inR[v] = true;
				
				k -= H.size();
				nowy.vertexes = R;

				for(int v: R) 
					for(int w: adj[v])
						if(inR[w])
							nowy.adj[v].push_back(w);

				return nowy.k_vertex_cover_kernel(k, cover_result);
						
			}
		} else {
			return *this;
		}
	}

	
	Crown crown_decomposition(int k) {
		Matching inclusive_matching = ls_general_matching();
		cerr << "I am counting the inclusive matching " << endl;


		if((int)inclusive_matching.size() > k) {
			return CROWN_NULL;
		}

		unordered_map<int, bool> inM;
		vector<int> M;

		for(auto p: inclusive_matching) {
			inM[p.first] = 1;
			inM[p.second] = 1;
			M.push_back(p.first);
			M.push_back(p.second);
		}

		Graph bipartialIM = Graph();
		bipartialIM.vertexes = vertexes;

		for(int v: vertexes) if(inM[v]) {
			for(int w: adj[v]) if(!inM[w]) {
				bipartialIM.adj[v].push_back(w);
			}
		}

		Matching IM_matching = bipartialIM.turbo_matching(M);

		if((int)IM_matching.size() > k) {
			return CROWN_NULL;
		} 

		VertexList X = vertex_cover_from_matching(IM_matching);
		unordered_map<bool, int> inX, inHC;
		
		for(int v: X)
			inX[v] = 1;

		VertexList H,C,R;

		for(int v: X) if(inM[v]) {
			H.push_back(v);
			inHC[v] = 1;
		}

		for(int i: vertexes) if(!inM[i] and !isolated(i) and !inX[i]) {
			C.push_back(i);
			inHC[i] = 1;
		}

		for(int i: vertexes) if(!inHC[i]) {
			inHC[i] = 1;
			R.push_back(i);
		}

		return make_tuple(H,C,R);
	}

	void k_vertex_cover_easy(int k, VertexList& cover_result) {
		#define end_with_null \
			cover_result.clear(); \
			cover_result.push_back(-1); \
			return;
		
		if(is_null()) {
			end_with_null;
		}
		if(vertexes.size() == 0 and k == 0) return;
		if(vertexes.size() == 0) {
			end_with_null;
		}

		pair<int,int> max_deg_vertex = {-1,-1};
		for(int v: vertexes)
			max_deg_vertex = max(make_pair((int)adj[v].size(), v), max_deg_vertex); 

		if(max_deg_vertex.first > 0 and k <= 0) {
			end_with_null;
		}

		int r = max_deg_vertex.second;

		// let's add r neighbours to the cover
		
		vector<int> second_cover_result = cover_result;

		Graph nowy = *this;

		for(int w: adj[r])
			second_cover_result.push_back(w);

		int new_k = k - adj[r].size() - 1;

		for(int w: adj[r]) 
			nowy.erase_vertex(w);
		
		nowy.erase_vertex(r);

		nowy.k_vertex_cover_easy(new_k, second_cover_result);

		if(second_cover_result != VERTEX_LIST_NULL) {
			cover_result.clear();
			for(int v: second_cover_result)
				cover_result.push_back(v);
			return;
		}
		
		// let's add r to the cover
		vector<int> first_cover_result = cover_result;
		first_cover_result.push_back(r);

		nowy = *this;
		nowy.erase_vertex(r);

		new_k = k-1;

		nowy.k_vertex_cover_easy(new_k, first_cover_result);


		if(first_cover_result != VERTEX_LIST_NULL) {
			cover_result.clear();
			for(int v: first_cover_result)
				cover_result.push_back(v);
			return;
		}

	}	

};



int main () {
	srand(time(NULL));
	Graph G = Graph();
	int k;

	#ifdef RANDOM
		G.random_gen();
		k = rand()%K_RAND_MAX+1;
	#else
		G.input();
		cin >> k;
	#endif

	VertexList ans = G.k_vertex_cover(k);

	if(ans == VERTEX_LIST_NULL) {
		cout << "Pokrycie o " << k << " wierzcholkach nie istnieje " << endl;  
	} else {
		cout << "Pokrycie o " << k << " wierzcholkach istnieje: " << endl;
		for(int v: ans) {
			cout << v << " ";
		}
		cout << endl;
	}
}
