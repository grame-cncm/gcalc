#ifndef __TFile__
#define __TFile__

#include <QFile>
#include <QTextStream>
#include <QString>

class TFWriter {
 private:
 	QFile	file;
 	QTextStream	dst;
 public:
 	TFWriter 	(const QString& fname) : file(fname) { 
			file.open(QIODevice::WriteOnly); 
			dst.setDevice(&file); 
			}
 	
	virtual ~TFWriter() { file.close(); }
	
 	void		Putc(char c) 	{ dst << c; }					// write a char
 	void		Puts(char* s) 	{ dst << s; }					// write a C string
 	void		Putl(long n) 	{ dst << n; }					// write a signed long
 	void		Putf(double n) 	{ dst << n; }					// write a float (4 decimals)
	void		PutStrRsrc(short) {}							// write a 'STR ' ressource
 	int			Error() 		{ return 0; }					// return current error code
 };
 
#endif
