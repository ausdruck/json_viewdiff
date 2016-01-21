#include "Model.h"
//#include "json/json.h"
//#include "precompare.h"
int JsonCompare (Cmp_Node * temp);

void merge_value(Json::Value & new_value,bool flag,Json::Value & result);
bool Compare(Json::Value* new_value , Json::Value* old_value , Json::Value * cmp_result , string main_key , Json::Value * single_query,hash_map<string,string>& QL);
void singleListWriteToFile();
void QLWriteToFile();
void CmpResultWriteToFile();
void * cmp_proccessor_thread(void * args);
bool cmp_arrayvalue(Json::Value *new_value ,Json::Value *old_value ,string main_key,Json::Value* cmp_result ,Json::Value* single_query,set<string>::iterator i);
