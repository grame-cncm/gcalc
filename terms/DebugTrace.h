#include <stdio.h>

extern FILE* gLogFile;

void OpenDebugFile();
void WriteDebugFile(char* s);
void WriteDebugFile(long n);
void CloseDebugFile();
void Indent();
void NewLine();
void Unindent();
