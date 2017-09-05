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

/** \file node.hh
 * A Node is a tagged unions of int, float, symbol and void* used in the implementation of CTrees. 
 * Nodes are completly described by the node.h file, there is no node.cpp file.
 * 
 * <b>API:</b>
 * 
 * 	Node(symbol("abcd")); 	: node with symbol content
 * 	Node(10);				: node with int content
 * 	Node(3.14159);			: node with float content
 * 	
 * 	n->type();				: kIntNode or kFloatNode or kSymNode 
 * 	
 * 	n->getInt();			: int content of n 
 * 	n->getFloat();			: float content of n 
 * 	n->getSym();			: symbol content of n 
 * 
 * 	if (isInt(n, &i))	... : int i = int content of n
 * 	if (isFloat(n, &f))	... : float f = float content of n
 * 	if (isSym(n, &s))	... : Symbol* s = Symbol* content of n
 * 	
 */
 	
/*****************************************************************************
\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/ 
/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/
*****************************************************************************/


#ifndef     __NODE__
#define     __NODE__

#include "assert.h"
#include "symbol.h"
#include <fstream>

using namespace std;

/**
 * Class Node = (type x (int + float + Symbol* + void*))
 */
class Node
{
	enum 	{ kIntType, kFloatType, kSymbolType, kPointerType };
	
	int		fType;
	union 	{ 
		int 	i; 
		double 	f; 
		Symbol* s; 
		void* 	p; 
	} fData;

 public:
	// constructors
	Node (int x) 				: fType(kIntType) 		{ fData.f = 0; fData.i = x; }
	Node (double x) 			: fType(kFloatType) 	{ fData.f = x; }
	Node (Symbol* x) 			: fType(kSymbolType) 	{ fData.f = 0; fData.s = x; }
	Node (void* x) 				: fType(kPointerType) 	{ fData.f = 0; fData.p = x; }
	
	Node (const Node& n) 		: fType(n.fType) 		{ fData.f = n.fData.f; }
 	
	// predicats
	bool 	equal (const Node& n) const { return fType == n.fType && fData.f == n.fData.f; }
	
	bool 	isInt ()		const	{ return fType == kIntType; }
	bool 	isFloat ()		const	{ return fType == kFloatType; }
	bool 	isSymbol ()		const	{ return fType == kSymbolType; }
	bool 	isPointer ()	const	{ return fType == kPointerType; }
	
	// accessors
	int		getInt() 		const 	{ assert(fType == kIntType); return fData.i; }
	double 	getFloat() 		const 	{ assert(fType == kFloatType); return fData.f; }
	Symbol* getSymbol() 	const 	{ assert(fType == kSymbolType); return fData.s; }	
	void* 	getPointer() 	const 	{ assert(fType == kPointerType); return fData.p; }	
	
	// others
	unsigned int hashcode()	const	{ return fType ^ fData.i; }
	ostream& print (ostream& fout) const;
};

 	
// predicats
inline bool operator == (const Node& a, const Node& b) { return a.equal(b); }
inline bool operator != (const Node& a, const Node& b) { return ! a.equal(b); }


#endif    
