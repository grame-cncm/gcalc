#include "node.h"
#include <fstream>

using namespace std;

ostream& Node::print (ostream& fout) const
{
	switch (fType) {
		case kIntType : 
			fout << fData.i; 
			break;
		case kFloatType : 
			fout << fData.f; 
			break;
		case kSymbolType : 
			fout << name(fData.s); 
			break;
		case kPointerType : 
			fout << fData.p; 
			break;
	}
	
	return fout;
}
