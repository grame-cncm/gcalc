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
******************************************************************************/
 
/** \file tree.h
 * A tree library with hashconsing and maximal sharing capabilities.
 * 
 * A tree library with hashconsing and maximal sharing capabilities. 
 * 
 * <b>API:</b>
 * 
 * \li tree (n) 				: tree of node n with no branch				
 * \li tree (n, t1) 			: tree of node n with a branch t
 * \li tree (n, t1,...,tm)		: tree of node n with m branches t1,...,tm
 *
 * <b>Useful conversions :</b>
 * 
 * \li int 			tree2int (t)	: if t has a node of type int, return it otherwise error
 * \li float 		tree2float (t)	: if t has a node of type float, return it otherwise error
 * \li const char* 	tree2str (t)	: if t has a node of type symbol, return its name otherwise error
 * \li void* 		tree2ptr (t)	: if t has a node of type ptr, return it otherwise error
 *
 * <b>Pattern matching :</b>
 * 
 * \li if (isTree (t, n)) 		... 	: t has node n and no branches; 
 * \li if (isTree (t, n, &t1)		... : t has node n and 1 branch, t1 is set accordingly; 
 * \li if (isTree (t, n, &t1...&tm)...	: t has node n and m branches, ti's are set accordingly; 
 * 
 * <b>Accessors :</b>
 *		 
 * \li t->node()		: the node of t		{ return fNode; }
 * \li t->height() 		: lambda height such that H(x)=0, H(\x.e)=1+H(e), H(e*f)=max(H(e),H(f))
 * \li t->arity() 		: the number of branches of t { return fArity; }
 * \li t->branch(i) 	: the ith branch of t
 *
 * <b>Attributs :</b>
 *		 
 * \li t->attribut() 	: return the attribut (also a tree) of t
 * \li t->attribut(t')	: set the attribut of t to t' 
 *	 
 *	
 * <b>Properties:</b>
 * 
 * If p and q are two Tree pointers  : 
 * 		p != q  <=>  *p != *q
 *
 **/	
	
/*****************************************************************************
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ 
******************************************************************************/



#ifndef     __TREE__
#define     __TREE__

#include "symbol.h"
#include "node.h"
#include <vector>
#include <map>

//---------------------------------API---------------------------------------

class 	Tree;
//typedef Tree* Tree*;

typedef map<Tree*, Tree*>	Properties;
typedef vector<Tree*>		Branches;

/**
 * A Tree = (Node x [Tree]) is a Node associated with a list of subtrees called branches.
 * A Tree = (Node x [Tree]) is the association of a content Node and a list of subtrees 
 * called branches. In order to maximize the sharing of trees, hashconsing techniques are used.
 * Ctrees at different addresses always have a different content. A first consequence of this 
 * approach is that a fast equality test on pointers can be used as an equality test on CTrees.
 * A second consequence is that a Tree can NEVER be modified. But a property list is associated 
 * to each Tree that can be used to attach arbitrary information to it. Due to the maximal 
 * sharing property it is therefore easy to do memoization using these property lists. 
 *
 * Means are also provided to do maximal sharing on recursive trees. The idea is to start from 
 * a deBruijn representation and progressively build a classical representation such that
 * alpha-equivalent recursive CTrees are necesseraly identical (and therefore shared).
 * 
 * WARNING : in the current implementation CTrees are allocated but never deleted
 **/
 
class Tree : public Node
{
 private:
	typedef unsigned int uint;
 
	static const int 	kHashTableSize = 510511;	///< size of the hash table used for "hash consing"
	static Tree*		gHashTable[kHashTableSize];	///< hash table used for "hash consing"
	
	Properties	fProperties;						///< the properties list attached to the tree
	uint		fHashKey;							///< the hashtable key 
	Branches	fBranch;							///< the subtrees
	Tree*		fNext;								///< next in same hash table entry

	Tree (uint hk, const Node& n, const Branches& br); 					///< construction is private, uses tree::make instead
	
	bool 		equiv 			(const Node& n, const Branches& br) const;	///< used to check if an equivalent tree already exists
	static uint	calcTreeHash 	(const Node& n, const Branches& br);		///< compute the hash key of a tree according to its node and branches
	
 public:
	~Tree (); 
 	
	static Tree* make (const Node& n, int ar, Tree* br[]);		///< return a new tree or an existing equivalent one

 	// Accessors
 	int 		arity() const			{ return fBranch.size();}	///< return the number of branches (subtrees) of a tree
	Tree* 		operator[](int i) const	{ return fBranch[i];	}	///< return the ith branch (subtree) of a tree
 	uint 		hashkey() const			{ return fHashKey; 		}	///< return the hashkey of the tree
	
	
	// Print a tree and the hash table (for debugging purposes)
	ostream& 	print (ostream& fout) const; 					///< print recursively the content of a tree on a stream
	
	// Predicats
	bool 		isa (Symbol* s)			{ return isSymbol() && getSymbol() == s;}
	bool 		isa (Symbol* s, Tree** a);
	bool 		isa (Symbol* s, Tree** a, Tree** b);
	bool 		isa (Symbol* s, int* n);
	bool 		isa (Symbol* s, int* n, Tree** a);
	bool 		isa (Symbol* s, int* n, Tree** a, Tree** b);

	// Property list of a tree
	void		setProperty(Tree* key, Tree* value) { fProperties[key] = value; }
	Tree*		getProperty(Tree* key) { 
		Properties::iterator i = fProperties.find(key); 
		if (i==fProperties.end()) {
			return 0;
		} else {
			return i->second; 
		}	
	}
};

//---------------------------------API---------------------------------------
inline Tree* tree (int n) { Tree* br[]= {}; return Tree::make(Node(n), 0, br); }
inline Tree* tree (double n) { Tree* br[]= {}; return Tree::make(Node(n), 0, br); }
inline Tree* tree (Symbol* s) { Tree* br[]= {}; return Tree::make(Node(s), 0, br); }

inline Tree* tree (Symbol* s, Tree* a) { Tree* br[]= {a}; return Tree::make(Node(s), 1, br); }
inline Tree* tree (Symbol* s, Tree* a, Tree* b) { Tree* br[]= {a,b}; return Tree::make(Node(s), 2, br); }
inline Tree* tree (Symbol* s, Tree* a, Tree* b, Tree* c) { Tree* br[]= {a,b,c}; return Tree::make(Node(s), 3, br); }
inline Tree* tree (Symbol* s, Tree* a, Tree* b, Tree* c, Tree* d) { Tree* br[]= {a,b,c,d}; return Tree::make(Node(s), 4, br); }

inline Tree* tree (Symbol* s, int n) { Tree* br[]= {tree(n)}; return Tree::make(Node(s), 1, br); }
inline Tree* tree (Symbol* s, int n, Tree* b) { Tree* br[]= {tree(n),b}; return Tree::make(Node(s), 2, br); }
inline Tree* tree (Symbol* s, int n, Tree* b, Tree* c) { Tree* br[]= {tree(n),b,c}; return Tree::make(Node(s), 3, br); }
inline Tree* tree (Symbol* s, int n, Tree* b, Tree* c, Tree* d) { Tree* br[]= {tree(n),b,c,d}; return Tree::make(Node(s), 4, br); }

inline ostream& operator << (ostream& s, const Tree& t) { return t.print(s); }

// pattern matching
bool isTree (Tree* t, Symbol* s); 
bool isTree (Tree* t, Symbol* s, Tree** a); 
bool isTree (Tree* t, Symbol* s, Tree** a, Tree** b);
bool isTree (Tree* t, Symbol* s, Tree** a, Tree** b, Tree** c); 
bool isTree (Tree* t, Symbol* s, Tree** a, Tree** b, Tree** c, Tree** d);  

// print statistics on tree allocations
void printstat();


//---------------------------------------------------------------------------

#endif    
