#include "Model.h"
//#include <stdio.h>
//#include "json/json.h"
#include <fstream>

int readFile();
struct Pre_Node * paser_lineToPreNode(char * line,bool flag);
bool pre_Node_compare(Pre_Node * a,Pre_Node * b);
void * precmp_proccessor_thread(void * args);
