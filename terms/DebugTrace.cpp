#include "DebugTrace.h"

FILE* 	gLogFile=0;
int		gIndent=0;

void Tabulate();

void OpenDebugFile()
{
	gLogFile = fopen("gcalcul.log", "w");
}

void WriteDebugFile(char* s)
{
	if (!gLogFile) OpenDebugFile();
	fprintf(gLogFile, s);
}

void WriteDebugFile(long n)
{
	if (!gLogFile) OpenDebugFile();
	fprintf(gLogFile, "%ld", n);
}

void Indent()
{
	gIndent++;
}

void NewLine()
{
	fprintf(gLogFile, "\n");
	Tabulate();
}

void Unindent()
{
	gIndent--;
}

void Tabulate()
{
	int i = gIndent;
	while (--i >= 0) fprintf(gLogFile, "  ");
}

void CloseDebugFile()
{
	fclose(gLogFile);
}

