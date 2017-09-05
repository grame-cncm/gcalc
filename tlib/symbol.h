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
 
/** \file symbol.hh
 * A library to create and manipulate symbols with a unique name. 
 * A library of functions to create and manipulate symbols with a unique name.
 * 
 *  <b>API:</b>
 *
 *   - Sym q = symbol("abcd");     <i>returns the symbol of name "abcd"</i>
 *   - const char* s = name(q);    <i>returns the name of symbol q</i>
 *   - Sym q = unique("TOTO");	   <i>returns a new unique symbol of name "TOTOnnn"</i>  
 *
 *  <b>Properties:</b>
 *
 *     If p and q are two symbols then : 
 *  	   p != q  <=>  name(p) != name(q)
 *
 */ 
 

#ifndef     __SYMBOL__
#define     __SYMBOL__


//using namespace std;

//--------------------------------SYMBOL-------------------------------------

/**
 * Symbols are unique objects with a name stored in a hash table.
 * For any two symbol pointers s1 and s2, the following property holds :
 * 		s1 != s2   <=>   names(s1) != name(s2)
 * 
 */
class Symbol;

Symbol*		symbol 	(const char* str); 							///< Returns (and creates if new) the symbol of name \p str
Symbol*		unique 	(const char* str);							///< Returns a new unique symbol of name strxxx
const char* name 	(Symbol* sym);						///< Returns the name of a symbol

#endif    
