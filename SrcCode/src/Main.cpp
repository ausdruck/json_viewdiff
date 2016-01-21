//#include "Model.h"
#include "precompare.h"
#include "compare.h"
 VDData * gb_data ;

void init(){
	//���������ʼ��gb_data
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
	int opt;
	string newfile,oldfile;
	gb_data->givenkey="";
	while((opt = getopt(argc, argv,"k:s:")) != -1) {
		switch(opt) {
			case 'k' :
				gb_data->givenkey=optarg;
				break;
			case 's' :
				newfile = optarg;
				if(optind<argc)
					oldfile = argv[optind];
				//cout<<newfile<<endl;
				//cout<<oldfile<<endl;
				break;
			//case ':' :
				//cout<<"option needs a value"<<endl;
				//return 1;
		}
	}
	//cout<<gb_data->givenkey<<endl;
	//if(argc != 4)
	//{
		//error info
          //  printf("input error! please input '-s oldfile newfile '");
          //  return 1;
	//}


	init();
//	int a;
//	scanf("aaa%d",a);
//	printf("%s,%s",argv[2],argv[3]);
//	scanf("aaa%d",a);
	gb_data->old_file = fopen(newfile.data(),"r");
	gb_data->new_file = fopen(oldfile.data(),"r");
	
	if(gb_data->old_file==NULL)
	{
		cout<<"Cann't open old file"<<endl;
		return 1;
	}
	if(gb_data->new_file==NULL)
	{
		cout<<"Cann't open new file"<<endl;
		return 1;
	}
	if(!strcmp(argv[1],"-s")&&tids!=NULL&&thread_cmp!=NULL)
	{
		//�������
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
	// ʡ��
}
