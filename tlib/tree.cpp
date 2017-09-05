/************************************************************************
 ************************************************************************
    FAUST compiler
	Copyright (C) 2003-2004 GRAME, Centre National de Creation Musicale
    ---------------------------------------------------------------------
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 ************************************************************************
 ************************************************************************/
 
/*****************************************************************************
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ 
/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ 
******************************************************************************
								TREE 
						Y. Orlarey, (c) Grame 2002
------------------------------------------------------------------------------
Trees are made of a Node associated with a list of branches : (Node x [Tree]).
Up to 4 branches are allowed in this implementation. A hash table is used to 
maximize the sharing of trees during construction : trees at different 
addresses always have a different content. Reference counting is used for 
garbage collection, and smart pointers P<Tree> should be used for permanent
storage of trees.

 API:
 ----
 tree (n) 				: tree of node n with no branch				
 tree (n, t1) 			: tree of node n with a branch t
 tree (n, t1,...,tm)	: tree of node n with m branches t1,...,tm
 
 Pattern matching :
 
 if (isTree (t, n)) 		... : t has node n and no branches; 
 if (isTree (t, n, &t1)		... : t has node n and 1 branch, t1 is set accordingly; 
 if (isTree (t, n, &t1...&tm)...: t has node n and m branches, ti's are set accordingly; 
 
 Accessors :
		 
 t->node()			: the node of t		{ return fNode; }
 t->arity() 		: the number of branches of t return fArity; }
 t->branch(i) 		: the ith branch of t

 Attributs :
		 
 t->attribut() 		: return the attribut (also a tree) of t
 t->attribut(t')	: set the attribut of t to t' 
		 
 Warning :
 ---------
 Since reference counters are used for garbage collecting, one must be careful not to 
 create cycles in trees The only possible source of cycles is by setting the attribut
 of a tree t to a tree t' that contains t as a subtree.  
	
 Properties:
 -----------
	If p and q are two Tree pointers  : 
		p != q  <=>  *p != *q

 History :
 ---------
 	2002-02-08 : First version
	2002-10-14 : counts for height and recursiveness added
	
****************************************************************************** 
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ 
/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ 
*****************************************************************************/


#include <stdio.h>
#include <limits.h>
#include "tree.h"
#include <iostream>
#include <fstream>

static int tree_search = 0;
static int tree_make = 0;
static int tree_new = 0;

void printstat()
{
	cout << "number of different trees : " << tree_new << endl;
	cout << "number of requested trees : " << tree_make << endl;
	//cout << "number of searched trees : " << tree_search << endl;	
	if (tree_make > 0) 
		cout << "percent of searched trees : " << (tree_search*100.0)/tree_make << "%" << endl;	
}

Tree* Tree::gHashTable[kHashTableSize];

///< build the tree and add it to the hash table
Tree::Tree (uint hk, const Node& n, const Branches& br) 
	:	Node(n), 
		fHashKey(hk), 
		fBranch(br) 
{ 
	tree_new++;
	// Add to hash table
   	int j = hk % kHashTableSize;
	fNext = gHashTable[j];
	gHashTable[j] = this;

}

// Destructor : remove the tree form the hash table
Tree::~Tree () 
{
	int		i = fHashKey % kHashTableSize;
	Tree*	t = gHashTable[i];
	
	//printf("Delete of "); this->print(); printf("\n");
	if (t == this) {
		gHashTable[i] = fNext;
	} else {
		Tree* p;
		while (t != this) {
			p = t;
			t = t->fNext;
		}
		p->fNext = fNext;
	}
}

// equivalence 
bool Tree::equiv (const Node& n, const Branches& br) const
{
	return equal(n) && (fBranch == br);
}

unsigned int Tree::calcTreeHash( const Node& n, const Branches& br )
{
	unsigned int 			hk = n.hashcode();
	Branches::const_iterator  b = br.begin();
	Branches::const_iterator  z = br.end();
	
	while (b != z) {
    	//hk = (hk << 1) ^ (hk >> 20) ^ ((*b)->fHashKey); // version d'origine
    	hk = hk + 1103515245 * ((*b)->fHashKey);	// inpirée de random faust : marche bien
    	//hk = (hk << 1) ^ ((*b)->fHashKey);	// autre essai
		++b;
	}
	return hk;
}

#if 1
// improved make with toward grandfather movement 
Tree* Tree::make(const Node& n, int ar, Tree** tbl)
{
	Branches	br(ar); 
	
	tree_make++;
	
	for (int i=0; i<ar; i++)  br[i] = tbl[i];
	
	uint 	hk  = calcTreeHash(n, br);
	Tree*	t = gHashTable[hk % kHashTableSize];
	Tree*	f = 0;	// father
	Tree*	g = 0;	// grand father
	
	while (t && !t->equiv(n, br)) {
		tree_search++;
		
		g = f; f = t; t = t->fNext;
	}
	if (t == 0) {
		return new Tree(hk,n,br);
	} else {
		if (g) {
			// if grandfather move t to head of list
			f->fNext = t->fNext;
			t->fNext = f;
			g->fNext = t;
		}
		return t;
	}
}
#endif

#if 0
Tree* Tree::make(const Node& n, int ar, Tree** tbl)
{
	Branches	br(ar); 
	
	tree_make++;
	
	for (int i=0; i<ar; i++)  br[i] = tbl[i];
	
	uint 	hk  = calcTreeHash(n, br);
	Tree*	t = gHashTable[hk % kHashTableSize];
	
	while (t && !t->equiv(n, br)) {
		tree_search++;
		t = t->fNext;
	}
	return (t) ? t : new Tree(hk, n, br);
}
#endif

ostream& Tree::print (ostream& fout) const
{
	Node::print(fout);
	{ // print properties
		Properties::const_iterator i = fProperties.begin(); 
		if (i != fProperties.end()) {
			fout << '<';
			while (i != fProperties.end()) {
				i->first->print(fout);
				fout << ':';
				i->second->print(fout);
				fout << ';';
				i++;
			}
			fout << '>';
		}	
	}

	int a = arity();
	if (a > 0) {
		int i; char sep;
		for (sep = '[', i = 0; i < a; sep = ',', i++) {
			fout << sep; fBranch[i]->print(fout); 
		}
		fout << ']';
	} 
	
	return fout;
}


bool isTree (Tree* t, Symbol* s)
{ 
	return t->isSymbol() && t->getSymbol() == s;
}

bool Tree::isa (Symbol* s, Tree** a)
{ 
	if ( isSymbol() && getSymbol() == s && arity() == 1) {
		*a = (*this)[0];
		return true;
	} else {
		return false;
	}
}

bool Tree::isa (Symbol* s, Tree** a, Tree** b)
{ 
	if ( isSymbol() && getSymbol() == s && arity() == 2) {
		*a = (*this)[0];
		*b = (*this)[1];
		return true;
	} else {
		return false;
	}
}

bool Tree::isa (Symbol* s, int* n)
{ 
	if ( isSymbol() && getSymbol() == s && arity() == 1) {
		*n = (*this)[0]->getInt();
		return true;
	} else {
		return false;
	}
}

bool Tree::isa (Symbol* s, int* n, Tree** a)
{ 
	if ( isSymbol() && getSymbol() == s && arity() == 2) {
		*n = (*this)[0]->getInt();
		*a = (*this)[1];
		return true;
	} else {
		return false;
	}
}

bool Tree::isa (Symbol* s, int* n, Tree** a, Tree** b)
{ 
	if ( isSymbol() && getSymbol() == s && arity() == 3) {
		*n = (*this)[0]->getInt();
		*a = (*this)[1];
		*b = (*this)[2];
		return true;
	} else {
		return false;
	}
}

bool isTree (Tree* t, Symbol* s, Tree** a)
{ 
	if ( t->isSymbol() && t->getSymbol() == s && t->arity() == 1) {
		*a = (*t)[0];
		return true;
	} else {
		return false;
	}
}

bool isTree (Tree* t, Symbol* s, Tree** a, Tree** b)
{ 
	if ( t->isSymbol() && t->getSymbol() == s && t->arity() == 2) {
		*a = (*t)[0];
		*b = (*t)[1];
		return true;
	} else {
		return false;
	}
}


bool isTree (Tree* t, Symbol* s, Tree** a, Tree** b, Tree** c)
{ 
	if ( t->isSymbol() && t->getSymbol() == s && t->arity() == 3) {
		*a = (*t)[0];
		*b = (*t)[1];
		*c = (*t)[2];
		return true;
	} else {
		return false;
	}
}

 
bool isTree (Tree* t, Symbol* s, Tree** a, Tree** b, Tree** c, Tree** d)
{ 
	if ( t->isSymbol() && t->getSymbol() == s && t->arity() == 4) {
		*a = (*t)[0];
		*b = (*t)[1];
		*c = (*t)[2];
		*d = (*t)[3];
		return true;
	} else {
		return false;
	}
}

