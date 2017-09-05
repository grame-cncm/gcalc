/************************************************************************
 ************************************************************************
    Tree Library
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

#include    "symbol.h"
#include 	"string.h"
#include 	"stdio.h"


/************************************************************************
    					IMPLEMENTATION
 ************************************************************************/

typedef unsigned int uint;						


/**
 * Implement the concept of symbol as in Lisp using a hash table
 */ 
 
class Symbol
{
	
 private:
		 
	static const int 	kHashTableSize = 511;					///< Size of the hash table (a prime number is recommended)
    static Symbol*		gSymbolTable[kHashTableSize];			///< Hash table used to store the symbols
		
 // Fields
    char*			fName;										///< Name of the symbol
    uint			fHash;										///< Hash key computed from the name and used to determine the hash table entry
    Symbol*			fNext;										///< Next symbol in the same hash table entry
    void*			fData;										///< Pointer to data associated with the symbol
	
 // Constructors & destructors
    Symbol (const char *str, uint hsh, Symbol *nxt); 			///< Constructs a new symbol ready to be placed in the hash table
   ~Symbol () {}												///< The Destructor is never used
	
 // Others
	bool			equiv (uint hash, const char* str) const ;	///< Check if the name of the symbol is equal to string \p str
	static uint 	calcHashKey (const char* str);				///< Compute the 32-bits hash key of string \p str

 // Static methods
	static bool 	isnew (const char* str);					///< Returns \b true if no symbol of name \p str exists
	
 public:

	static Symbol* 	getsymbol 	(const char* str);				///< Get the symbol of name \p str
	static Symbol* 	prefix 		(const char* str);				///< Creates a new symbol of name prefixed by \p str
	const char* 	name ()				{ return fName;}		///< Returns the name of a symbol
	void* 			getdata ()			{ return fData;}		///< Returns the name of a symbol
	void 			setdata (void* d)	{ fData = d;}			///< Returns the name of a symbol
		
};

/**
 * Hash table used to store the symbols
 */
 
Symbol*	Symbol::gSymbolTable[kHashTableSize];



/**
 * Search the hash table for the symbol of name \p str or returns a new one.
 * \param str the name of the symbol
 * \return a symbol of name str
 */ 

Symbol* Symbol::getsymbol(const char* str)
{
    uint 	hsh  = calcHashKey(str);
    int 	bckt = hsh % kHashTableSize;
	Symbol*	item = gSymbolTable[bckt];

    while ( item && !item->equiv(hsh,str) ) item = item->fNext;
	Symbol* r = item ? item : gSymbolTable[bckt] = new Symbol(str, hsh, gSymbolTable[bckt]);
	return r;
}


/**
 * Static method that searches the symbol table for a string. 
 * \param str string to search 
 * \return true if the string is NOT in the table (it is a new string) 
 */

bool Symbol::isnew(const char* str)
{
    uint 			hsh  = calcHashKey(str);
    int 			bckt = hsh % kHashTableSize;
	const Symbol*	item = gSymbolTable[bckt];
	
    while ( item && !item->equiv(hsh,str) ) item = item->fNext;
	return item == 0;
}


/**
 * Creates a new symbol with a name obtained by concatenating the \p str prefix with a number in order to make it unique
 * \param str the prefix of the name
 * \return a symbol of name \p prefix++n 
 */

Symbol* Symbol::prefix (const char* str)
{
	char 	name[256];
	
	for (int n = 0; n<10000; n++) {
		snprintf(name, 256, "%s%d", str, n);
		if (isnew(name)) return getsymbol(name);
	}
	return getsymbol("UNIQUEOVERFLOW");
}	


/**
 * Check if the name of the symbol is equal to string \p str
 * This method is used by isnew() and make() when searching the hashtable 
 * for an existing symbol.
 *
 * \param hash the hash key of the string (used to speedup the comparison)
 * \param str the string to compare
 * \return \p true if the name of the symbol and \p str are the same
 */
 
bool Symbol::equiv (uint hash, const char *str) const
{
	return (fHash == hash) && (strcmp(fName,str) == 0);
}



/**
 * Compute the 32-bits hash key of string \p str
 * \param str the string
 * \return a 32-bits hash key
 */

uint Symbol::calcHashKey (const char* str)
{
    uint h = 0;

    while (*str) h = (h << 1) ^ (h >> 20) ^ (*str++);
    return h;
}



/**
 * Constructs a symbol ready to be placed in the hash table. 
 * It makes a private copy of its name.
 * \param str the name of the symbol
 * \param hsh the hash key of the symbol
 * \param nxt a pointer to the next symbol in the hash table entry
 */

Symbol::Symbol(const char* str, uint hsh, Symbol* nxt)
{
	int len = strlen(str);
	
    fName = new char [len+1];
    memcpy(fName, str, len+1);
    fHash = hsh;
    fNext = nxt;
	fData = 0;
}





/************************************************************************
    								API
 ************************************************************************/


/**
 * Search the hash table for the symbol of name \p str or returns a new one.
 * \param str the name of the symbol
 * \return a symbol of name str
 */ 

Symbol*		symbol 	(const char* str) 		{ return Symbol::getsymbol(str); } 	


/**
 * Creates a new symbol with a name obtained by concatenating the \p str prefix with a number in order to make it unique
 * \param str the prefix of the name
 * \return a symbol of name \p prefix++n 
 */

Symbol*		unique 	(const char* str) 		{ return Symbol::prefix(str);}	


/**
 * Returns the name of a symbol
 * \param sym the adress of the symbol
 * \return the name of the symbol 
 */

const char* name (Symbol* sym) 	{ return sym->name(); }	


/**
 * Returns the data associated to a symbol
 * \param sym the adress of the symbol
 * \return a pointer to the data associated 
 */

void* getdata (Symbol* sym) 	{ return sym->getdata(); }	

