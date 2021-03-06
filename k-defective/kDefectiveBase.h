#ifndef __K_DEFECTIVE_BASE_H__
#define __K_DEFECTIVE_BASE_H__

#include "Common.h"

class KDefectiveBase {
	protected:
    size_t count; // 用于统计搜索树的节点数量
    clock_t st, ed; // 用于计时
	int ans; // 用于统计答案
	int size; // 图的规模
	int *dis; // 用于计算距离
    int *maxDis; // 用于统计最长的距离
    bool *isInPC; // 用于加速计算距离(calcDisFrom)
	vector <int> *from; // 邻接链表;

	virtual void __init__(void *P, void *C) = 0; // special initial for subclass

	// virtual API for set Operation
	virtual void addVertexToSet(void *ptr, int idx) = 0;
	virtual void removeVertexFromSet(void *ptr, int idx) = 0;
	virtual void* setIntersection(void *A, void *B) = 0;
	virtual void* neighborSetOf(int idx) = 0;
	virtual bool existsInSet(void *ptr, int idx) = 0;
	virtual int sizeOfSet(void *ptr) = 0;
	virtual void deleteSet(void *ptr) = 0;
	virtual void* newSet() = 0;
	virtual void setCopyTo(void *src, void *dst) = 0;

	// reduction using edge number
	virtual void reductionByEdge(void *P, void *C, int m); 

	// calc the upper bound of the diameter
	virtual int calcLimOfDiam(void *P, void *C, int k); 

	// reduction using diameter
	virtual void reductionByDiam(void *P, void *C, int k); 

	// reduction when a vertex connect to all other verteces
	virtual void reductionByConnectToAll(void *P, void *C);

	// calc the distance from idx
	virtual void calcDisFrom(void *P, void *C, int s); 

	// calc the number of needed edge if add idx from C to P
	virtual int calcNeedEdge(void *P, void *C, int idx);

	// calc the order for branch
	static bool cmpForOrder(const pair<int, int> &a, const pair<int, int> &b);
	void calcBranchOrder(void *P, void *C, vector<pair<int, int> > &order);

	virtual void branch(void *P, void *C, int k, int m);
	virtual void branchWhenCouldNotReduceM(void *P, void *C, int k, int m);
	virtual void branchWhenCouldReduceM(void *P, void *C, int k, int m);
	virtual bool couldRecudeM(void *P, void *C);

	virtual void prework(void *P, void *C, int k);

	void init(void *P, void *C);
	virtual void solve(void *_P, void *_C, int k, int m);

	public:
	KDefectiveBase(int n);
	virtual ~KDefectiveBase();

	virtual int Solve(int k);
	void AddEdge(int a, int b);
    
    // API for read the Protected var
    virtual size_t GetCount();
};

#endif
