//#include "Model.h"
#include "precompare.h"
#include "compare.h"
 VDData * gb_data ;

void init(){
	//根据输入初始化gb_data
//	gb_data = new VDData();

    pthread_mutex_init(&gb_data->mutex,NULL); 
	//gb_data->QueryList = new hash_map<string,string>;
	gb_data->all_lines=0;
	gb_data->diff_lines=0;
	gb_data->buffer_count_new=gb_data->buffer_count_old=0;
	gb_data->cmp_queue = new queue<Cmp_Node *> ;
	for(int i=0;i<FILE_BUFFER_SIZE;i++){
		gb_data->input_line_new[i]=new char[MAX_ONELINE+1];
		gb_data->input_line_old[i]=new char[MAX_ONELINE+1];
//		gb_data->input_line_new[i]=NULL;
//		gb_data->input_line_old[i]=NULL;
	}
//	gb_data->cmp_result = new Json::Value;

	//gb_data->json_reader=new Json::Reader;
	//gb_data->json_writer = new Json::StyledWriter;
	gb_data->new_file=gb_data->old_file=NULL;
//    gb_data->mutex = PTHREAD_MUTEX_INITIALIZER;
//    gb_data->mutex=PTHREAD_MUTEX_INITIALIZER ;
}

int main(int argc, char ** argv){
//	gb_data = (VDData *)malloc(sizeof(VDData)+4);
	gb_data = new VDData;
	pthread_t * tids=NULL;
	tids = (pthread_t *)malloc(sizeof(pthread_t) * 2);

	pthread_t * thread_cmp=NULL;
	thread_cmp = (pthread_t *)malloc(sizeof(pthread_t) * 2);
//	char * tmp=NULL;
	if(argc != 4)
	{
		//error info
            printf("input error! please input '-s oldfile newfile '");
            return 1;
	}


	init();
//	int a;
//	scanf("aaa%d",a);
//	printf("%s,%s",argv[2],argv[3]);
//	scanf("aaa%d",a);
	gb_data->old_file = fopen(argv[2],"r");
	gb_data->new_file = fopen(argv[3],"r");
	
	if(gb_data->old_file==NULL)
	{
		printf("Cann't open file %s \n",argv[2]);
		return 1;
	}
	if(gb_data->new_file==NULL)
	{
		printf("Cann't open file %s \n",argv[3]);
		return 1;
	}
	if(!strcmp(argv[1],"-s")&&tids!=NULL&&thread_cmp!=NULL)
	{
		//程序入口
		if(pthread_create(tids, NULL, precmp_proccessor_thread,(void *)gb_data)){
		    printf("precompare pthread create fail \n");
		  }
		if(pthread_create(thread_cmp, NULL, cmp_proccessor_thread,(void *)gb_data)){
			printf("compare pthread create fail \n");
		}
		pthread_join(*tids, NULL);
		delete tids;
		pthread_join(*thread_cmp, NULL);
		delete thread_cmp;
	}

	fclose(gb_data->old_file);
	fclose(gb_data->new_file);
	// 省略
}
