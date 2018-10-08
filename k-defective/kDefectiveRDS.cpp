#include "kDefectiveRDS.h"

KDefectiveRDS::KDefectiveRDS(int n):size(n) {
	from = new vector<int>[n];
	LB = new int[n];
}

KDefectiveRDS::~KDefectiveRDS() {
	delete[] from;
	delete[] LB;
}

void KDefectiveRDS::AddEdge(int a, int b) {
	from[a].push_back(b);
	from[b].push_back(a);
}

void KDefectiveRDS::getOrder(vector<int> &vec) {
	// sort
	vector <pair<int, int> > order_pair; order_pair.clear();
	for (int i = 0; i < size; i++) 
		order_pair.push_back(make_pair(from[i].size(), i));
	sort(order_pair.begin(), order_pair.end());

	vec.clear(); // initial
	for (int i = 0; i < size; i++) vec.push_back(order_pair[i].second);
}

int KDefectiveRDS::minInSet(void *C) {
	for (int i = 0; i < size; i++) 
		if (this -> existsInSet(C, order[i])) 
			return i;
    return -1;
}

int KDefectiveRDS::calcNeedEdge(void *P, void *C, int idx) {
    // @todo need 可以通过O(n)的维护来优化
	void *nei = this -> neighborSetOf(idx);
	void *neiP = this -> setIntersection(P, nei);
	int need = this -> sizeOfSet(P) - this -> sizeOfSet(neiP);
	this -> deleteSet(nei); this -> deleteSet(neiP);
    return need;
}

void KDefectiveRDS::solve(void *_P, void *_C, int k) {
    if ((clock() - st)/CLOCKS_PER_SEC > 600) exit(2); // 卡时间
    
	void *P = this -> newSet(), *C = this -> newSet();
	this -> setCopyTo(_P, P); this -> setCopyTo(_C, C);

	// reduction
	for (int i = 0; i < size; i++) if (this -> existsInSet(C, i)) {
		int need = calcNeedEdge(P, C, i);
		if (need > k) this -> removeVertexFromSet(C, i);
	}

	// update ans
	if (this -> sizeOfSet(C) == 0) {
		int sz = this -> sizeOfSet(P);
		if (sz > ans) ans = sz;
	}

	// branch
	while (this -> sizeOfSet(C) != 0) {
		if (this -> sizeOfSet(C) + this -> sizeOfSet(P) <= ans) return;
		int idx = minInSet(C);
		if (LB[idx] + this -> sizeOfSet(P) <= ans) return;
		this -> removeVertexFromSet(C, order[idx]);
		int need = calcNeedEdge(P, C, order[idx]);
		this -> addVertexToSet(P, order[idx]);
		solve(P, C, k - need);
        this -> removeVertexFromSet(P, order[idx]);
	}

	this -> deleteSet(P); this -> deleteSet(C);
}

int KDefectiveRDS::Solve(int k) {
    ans = 0;
    st = clock();
	memset(LB, 0, sizeof(int) * size);
	getOrder(order);
	for (int i = size-1; i >= 0; i--) {
		void *P = this -> newSet(), *C = this -> newSet();

		this -> addVertexToSet(P, order[i]); // i 一定在当前的答案中
		for (int j = i + 1; j < size; j++) this -> addVertexToSet(C, order[j]);
		solve(P, C, k);
		LB[i] = ans;

		this -> deleteSet(P); this -> deleteSet(C);
	}
    ed = clock();
    fprintf(stderr, "time: %fms\n", float(ed-st)/CLOCKS_PER_SEC * 1000);
    return ans;
}
