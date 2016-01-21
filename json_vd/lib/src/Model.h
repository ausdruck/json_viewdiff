#ifndef _Model_
#define _Model_
#include <stdio.h>
#include <map>
#include <ext/hash_map>
#include <queue>
#include <list>
#include <string.h>
#include "json/json.h"
#include <set>
//#include "precompare.h"
//#include "compare.h"
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

using namespace std;
using namespace __gnu_cxx;

namespace __gnu_cxx
{
	template<> struct hash<string>
	{
		size_t operator()(const string& s) const
		{
			return __stl_hash_string(s.c_str());
		}
	};
}


#define FILE_BUFFER_SIZE 10 //100
#define MAX_ONELINE 204800//2048

struct Pre_Node{
	string main_key;
	string query_data;
	bool old_file;
};
struct Cmp_Node{
	string main_key;
	Json::Value * value_old;//new + old
	Json::Value * value_new;
};
struct Sig_Node{
	string main_key;
	Json::Value * value_merge;
};
//���ݼ��ϣ��������н�������
class VDData{
public:
	int all_lines ;//������
	int diff_lines ;//diff ����
	FILE * new_file;
	FILE * old_file;
	char *input_line_new[FILE_BUFFER_SIZE];
	int buffer_count_new;

	char *input_line_old[FILE_BUFFER_SIZE];
	int buffer_count_old;
	list<Sig_Node *> single_query;//singlequery �����б�
	hash_map<string,string> QueryList;	//querylist��¼����������querylistչ��

	//queue<Pre_Node> pre_queue; //pre �������
	hash_map<string,Pre_Node *> pre_map; //pre ������
	queue<Cmp_Node *> *cmp_queue; //cmp �������


	Json::Value * cmp_result; //�ԱȽ��

    pthread_mutex_t mutex;

	//char *a[3];
	Json::Reader json_reader;
	Json::StyledWriter json_writer;
};

#endif
