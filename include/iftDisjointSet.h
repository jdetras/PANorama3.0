#ifndef IFT_DISJOINT_SET_H_
#define IFT_DISJOINT_SET_H_

//Disjoint-set data structure
typedef struct{
	int *parent; //Pointer to representative
	int *rank;
	int n; //Number of sets
} iftDisjointSet;

//Creates a disjoint-set datastructure with n sets
iftDisjointSet* iftCreateDisjointSet(int n);

//Joins the sets containing elem1 and elem2 and returns the representative
int iftDisjointSetUnion(iftDisjointSet* dset, int elem1, int elem2);

//Finds the representative for the set containing elem
int iftDisjointSetFind(iftDisjointSet* dset, int elem);

#endif

