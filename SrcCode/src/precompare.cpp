#include "precompare.h"

VDData *gb_data_precmp;
Json::Reader precmp_json_reader;
//读取两个个文件放到缓存，并记录读取长度
//读取文档，发现有文件已经读完0、两个文件都未读完 1、old文件读完 2、新文件读完 3、新旧文件都读完
int readFile()
{
	//TODO 两个FILE句柄判断
	bool new_file_flag=false;
	if(gb_data_precmp->buffer_count_new == 0)
	{
		//if(gb_data_precmp->buffer_count_old == 0)readFile();
		new_file_flag=true;
	}
	//TODO 建议修改读取每行的方法获取读取失败异常
	char * line=new char[MAX_ONELINE+1];
	for(int i=0;i<FILE_BUFFER_SIZE;i++){
//		char * line=new char[MAX_ONELINE+1];
		char * temp;
//		unsigned int len =0;
		size_t len =0;
		ssize_t offset;
		if(new_file_flag)
		{

			offset=getline(&line,&len,gb_data_precmp->new_file);
			temp = line;
			//cout<<gb_data_precmp->buffer_count_new<<endl;
			//cout<<gb_data_precmp->buffer_count_old<<endl;
//			temp = fgets(line, MAX_ONELINE, gb_data_precmp->new_file);
			if(temp==NULL || offset == -1){
				//TODO 未考虑文件打开异常问题
				 if(gb_data_precmp->buffer_count_new>0&&gb_data_precmp->buffer_count_old > 0)return 0;
				 else if(gb_data_precmp->buffer_count_new == 0 &&gb_data_precmp->buffer_count_old > 0)
				 {
					 return 2;
				 }else if(gb_data_precmp->buffer_count_new == 0 &&gb_data_precmp->buffer_count_old == 0){
					 for(int i=0;i<FILE_BUFFER_SIZE;i++){
						 if(gb_data_precmp->input_line_new[i]!=NULL){
						 	delete [] gb_data_precmp->input_line_new[i];
							gb_data_precmp->input_line_new[i]=NULL;
						 }
						 if(gb_data_precmp->input_line_old[i]!=NULL){
						 	delete [] gb_data_precmp->input_line_old[i];
							gb_data_precmp->input_line_old[i]=NULL;
						 }
					 }
					 return 3;
				 }else if(gb_data_precmp->buffer_count_new > 0 &&gb_data_precmp->buffer_count_old == 0)
				 {
				 	return 1;
				 }
			 }else if(temp!=NULL){
				 if(gb_data_precmp->input_line_new[i]==NULL)
				 {
					 delete [] gb_data_precmp->input_line_new[i];
                     			 gb_data_precmp->input_line_new[i]=NULL;
				 }
//				 gb_data_precmp->input_line_new[i]=temp;
                 //printf("%s\n",line);
//                 for(int n=0; n<=offset;n++){
//                     gb_data_precmp->input_line_new[i][n]=line[n];    
//                 }
                 //printf("%d\n",offset);
				 strcpy(gb_data_precmp->input_line_new[i],(const char*)line);
                 //printf("%s %d %s\n","new",i,gb_data_precmp->input_line_new[i]);
				 gb_data_precmp->buffer_count_new++;
			 }
		}
		else if(!new_file_flag)
		{
			offset=getline(&line,&len,gb_data_precmp->old_file);
			temp = line;

//			 temp = fgets(line, MAX_ONELINE, gb_data_precmp->old_file);
			 if(temp==NULL||offset == -1){//TODO 未考虑文件打开异常问题
				 if(gb_data_precmp->buffer_count_new>0&&gb_data_precmp->buffer_count_old > 0)return 0;
				 else if(gb_data_precmp->buffer_count_new == 0 &&gb_data_precmp->buffer_count_old == 0)
				 {
					 for(int i=0;i<FILE_BUFFER_SIZE;i++){
					 	if(gb_data_precmp->input_line_new[i]!=NULL){
					 		delete [] gb_data_precmp->input_line_new[i];
							gb_data_precmp->input_line_new[i]=NULL;
						}
					 	if(gb_data_precmp->input_line_old[i]!=NULL){
					 		delete [] gb_data_precmp->input_line_old[i];
							gb_data_precmp->input_line_old[i]=NULL;
						}
					 }
					 return 3;
				 }else if(gb_data_precmp->buffer_count_new > 0 &&gb_data_precmp->buffer_count_old == 0){
					return 1;
				 }else if(gb_data_precmp->buffer_count_new == 0 &&gb_data_precmp->buffer_count_old > 0){
				 	return 2;
				 }
			 }else if(temp!=NULL)
			 {
				if(gb_data_precmp->input_line_old[i]==NULL)
					{
						delete [] gb_data_precmp->input_line_old[i];
						gb_data_precmp->input_line_old[i]=NULL;
					}
//				gb_data_precmp->input_line_old[i]=temp;
		//printf("%s\n",line);
	       	//printf("%d\n",offset);
				strcpy(gb_data_precmp->input_line_old[i],(const char*)line);
		//printf("%s %d %s\n","old",i,gb_data_precmp->input_line_old[i]);
				gb_data_precmp->buffer_count_old++;
			}
		}

	//return 0;
	}
	return 0;
}

//解析行  生成node pre
//TODO
struct Pre_Node * paser_lineToPreNode(char * line,bool flag)
{
	//const char * tmp = " \t";
//    printf("%s\n",line);
	struct Pre_Node *pre_node = new Pre_Node;
	string linestr(line);
	int pos=linestr.find('{');
	if(pos<2)
	{
		//cout<<"There is illegal line!"<<endl;
		FILE *fp=fopen("Illegal.log","aw+");
		if(flag)
			fprintf(fp,"(old)%s\n",line);
		else
			fprintf(fp,"(new)%s\n",line);	
		fclose(fp);	
		pre_node->main_key="";
		return pre_node;
	}
	//struct Pre_Node *pre_node = new Pre_Node;
	pre_node->main_key = linestr.substr(0,pos-1);
	pre_node->query_data = linestr.substr(pos);
//	cout<<pre_node->query_data;
    pre_node->old_file = flag;
	return pre_node;
}

//pre Node compare // true->same false->diff
bool pre_Node_compare(Pre_Node * a,Pre_Node * b)
{
    if(a->main_key != b->main_key)
	{
		return false;
	}
	if(a->query_data == b->query_data)
	{
		return true;
	}
/*	if(a->query_data.empty()||b->query_data.empty())
    {
        return true;
    }
    */
	return false;
}
void NoMatchWriteLine(FILE *fp,Pre_Node *node)
{
	string tmp=node->main_key+" "+node->query_data;
	fprintf(fp,"%s\n",tmp.data()); 
}	
//TODO 添加change flag到pre map？
void * precmp_proccessor_thread(void * args)
{
	gb_data_precmp = (VDData *)args;
	bool changeflag = false;
	int tag = 0;
	int oldlines = 0;
	int newlines = 0;
	readFile();
	while(1){
            //printf("1");
			//cout<<"buffer_count_new:"<<gb_data_precmp->buffer_count_new<<endl;
			//cout<<"buffer_count_old:"<<gb_data_precmp->buffer_count_old<<endl;
			if(gb_data_precmp->buffer_count_new==0||gb_data_precmp->buffer_count_old==0)
			{
				/*if(readFile()>0){
					//读取文档，发现有文件已经读完0、两个文件都未读完 1、old文件读完 2、新文件读完 3、新旧文件都读完
					//TODO
				}*/
				int tag1=tag;
				tag = readFile();
				if(tag==0&&tag1!=0)
					tag=tag1;
			}
			if (tag==0 ) changeflag ^= true;//两个文件都未读完
			else if (tag==1)changeflag = false;//old文件读完了
			else if (tag==2)changeflag = true;//new 文件读完了
			else if (tag == 3) break;//两个文件都读完
			//cout<<"changeflag"<<changeflag<<endl;
            //cout<<tag<<endl;
			//pthread_mutex_lock(&g_mutex_file_lock);
			char * temp = 0;
			if( changeflag ){
		 		//temp = fgets(line, MAX_ONELINE, file);
				//noo = 1;
                //printf("%d %s",oldlines, gb_data_precmp->input_line_old[oldlines]);
				temp = gb_data_precmp->input_line_old[oldlines];
//				gb_data_precmp->input_line_old[newlines] = 0;
				oldlines = ++oldlines % FILE_BUFFER_SIZE;
				(gb_data_precmp->buffer_count_old)--;
			}else{
		 		//temp = fgets(line, MAX_ONELINE, oldfile);
				//noo = 0;
                //printf("%d %s",newlines, gb_data_precmp->input_line_new[newlines]);
				temp = gb_data_precmp->input_line_new[newlines];
//				gb_data_precmp->input_line_new[newlines] = 0;
				newlines = ++newlines % FILE_BUFFER_SIZE;
				(gb_data_precmp->buffer_count_new)--;
			}

			//在pre map中寻找是否有相同mainkey的node 有的话拿出来组合放入compare队列，没有的话讲node加入map中
			if(temp != NULL){
               //printf("%s\n",temp);
                		Pre_Node * temp_node;
				if(gb_data_precmp->givenkey=="")
				{
					temp_node = paser_lineToPreNode(temp,changeflag);
					if(temp_node->main_key=="")
						continue;
				}
				else
				{
					temp_node = new Pre_Node;
					temp_node->old_file=changeflag;
					temp_node->query_data=temp;
					Json::Value * temp_value =new Json::Value;
					Json::Reader temp_json_reader;
					bool illegal=false;
					if((temp[0]=='{')&&temp_json_reader.parse(temp,*temp_value,true))
					{
				        	if(temp_value->isMember(gb_data_precmp->givenkey)){
							if((*temp_value)[gb_data_precmp->givenkey].type()==Json::stringValue)
								temp_node->main_key=(*temp_value)[gb_data_precmp->givenkey].asString();
							else if((*temp_value)[gb_data_precmp->givenkey].type()==Json::realValue)
								temp_node->main_key=Json::valueToString((*temp_value)[gb_data_precmp->givenkey].asDouble());
							else if((*temp_value)[gb_data_precmp->givenkey].type()==Json::intValue)
								temp_node->main_key=Json::valueToString((*temp_value)[gb_data_precmp->givenkey].asInt());
							else if((*temp_value)[gb_data_precmp->givenkey].type()==Json::uintValue)
								temp_node->main_key=Json::valueToString((*temp_value)[gb_data_precmp->givenkey].asUInt());
							else 
								illegal=true;
							//cout<<temp_node->main_key<<endl;
						}else
							illegal=true;	
					}else{
						illegal=true;
					}
					if(illegal){
						FILE *fp=fopen("Illegal.log","aw+");
						if(changeflag)
							fprintf(fp,"(old)%s\n",temp);
						else
							fprintf(fp,"(new)%s\n",temp);
						fclose(fp);
						continue;
					}
				}
				const string a = temp_node->main_key;
				Pre_Node * new_node = gb_data_precmp->pre_map[a];
//				if(gb_data_precmp->pre_map.find(a)==gb_data_precmp->pre_map.end())
//				new_node = gb_data_precmp->pre_map[a];
				if(new_node == 0)
				{
					gb_data_precmp->pre_map[a]=temp_node;
				}
				else
				{
					//gb_data_precmp->pre_map.find("a");
					//对比两个node 相同 两个都delete 不同则生成cmpnode 并加入cmp队列
					//TODO
					if(pre_Node_compare(new_node,temp_node))
					{
						delete new_node;
						delete temp_node;
					}else
					{
						//json 处理生成valu
                        //printf("calue\t");
						Json::Value * new_value = new Json::Value;
						Json::Value * old_value = new Json::Value;
						Json::Reader precmp_json_reader;
//                        if(gb_data_precmp->pre_map.size()>1){printf("pre map too large!");return NULL;}
						bool notjson=false;
						if(!new_node->old_file&&temp_node->old_file){
							if(!precmp_json_reader.parse(new_node->query_data,*new_value,true))
							{
								notjson=true;
								FILE *fp=fopen("Illegal.log","aw+");
								fprintf(fp,"(new)%s",new_node->query_data.data());
								fclose(fp);
							}
							if(!precmp_json_reader.parse(temp_node->query_data,*old_value,true))
							{
								notjson=true;
								FILE *fp=fopen("Illegal.log","aw+");
								fprintf(fp,"(old)%s\n",temp_node->query_data.data());
								fclose(fp);
							}
							if(notjson)
							{
								delete new_node;delete temp_node;
								gb_data_precmp->pre_map.erase(a);
								continue;
							}
						}else if(new_node->old_file&&!temp_node->old_file){
							if(!precmp_json_reader.parse(temp_node->query_data,*new_value,true))
							{
								notjson=true;
								FILE *fp=fopen("Illegal.log","aw+");
								fprintf(fp,"(new)%s",temp_node->query_data.data());
								fclose(fp);
							}
							if(!precmp_json_reader.parse(new_node->query_data,*old_value,true))
							{
								notjson=true;
								FILE *fp=fopen("Illegal.log","aw+");
								fprintf(fp,"(old)%s\n",new_node->query_data.data());
								fclose(fp);
							}
							if(notjson)
							{
								delete new_node;delete temp_node;
								gb_data_precmp->pre_map.erase(a);
								continue;
							}
//							Json::Reader a ;
//							a.parse(temp_node->query_data,*new_value);
						}else {
							//
							printf("same mainkey in one file");
							FILE *fp=fopen("DuplicateKey.log","aw+");
							if(new_node->old_file&&temp_node->old_file){
								fprintf(fp,"(old)%s %s\n",new_node->main_key.data(),new_node->query_data.data());
								fprintf(fp,"(old)%s %s\n",temp_node->main_key.data(),temp_node->query_data.data());
							}
							else {
								fprintf(fp,"(new)%s %s\n",new_node->main_key.data(),new_node->query_data.data());
								fprintf(fp,"(new)%s %s\n",temp_node->main_key.data(),temp_node->query_data.data());
							}
							fclose(fp);
							delete new_node;delete temp_node;
							gb_data_precmp->pre_map.erase(a);
							continue;
						}

						Cmp_Node * cmp_tmp = new Cmp_Node;
						cmp_tmp->main_key = temp_node->main_key;//strcmp?
						cmp_tmp->value_old = old_value;
						cmp_tmp->value_new = new_value;
						//cout<<"----"<<cmp_tmp->main_key<<endl;
						//cout<<*(cmp_tmp->value_old)<<endl;
						//cout<< *(cmp_tmp->value_new)<<endl;

                        //printf("2");
                        pthread_mutex_lock(&gb_data_precmp->mutex);
						string prev_key=gb_data_precmp->QueryList[new_node->main_key];
						if(prev_key=="")
						{
							gb_data_precmp->cmp_queue->push(cmp_tmp);
							gb_data_precmp->QueryList[new_node->main_key]="difflist:";
						}
                        pthread_mutex_unlock(&gb_data_precmp->mutex);

						delete new_node;
//						delete temp_node;
//                        printf("3");
					}

						gb_data_precmp->pre_map.erase(a);
				}
			}
	}
	//遍历map pre 全部加入到cmp队列  TODO:暂时去除所有无法匹配的key的对比

	if(gb_data_precmp->pre_map.size()>0)
	{
		FILE *fpnew=fopen("NMnew.log","wr+");
		FILE *fpold=fopen("NMold.log","wr+");
    		for(hash_map<string,Pre_Node *>::iterator i=gb_data_precmp->pre_map.begin();gb_data_precmp->pre_map.size()>0&&i!=gb_data_precmp->pre_map.end();)
		{
//            printf("in no match");
		//Json::Value * new_value = new Json::Value;
		//Json::Value * old_value = new Json::Value;
		//Json::Reader precmp_json_reader;
		Pre_Node * temp_node = i->second;
		if(temp_node->old_file){
			//precmp_json_reader.parse("",*new_value,true);
			//if(!precmp_json_reader.parse(temp_node->query_data,*old_value,true))
			//{
			//	cout<<"Old file has not json line!";exit(0);
			//}
			NoMatchWriteLine(fpold,temp_node);
		}else {
			//if(!precmp_json_reader.parse(temp_node->query_data,*new_value,true))
			//{
			//	cout<<"New file has not json line!";exit(0);
			//}
			//precmp_json_reader.parse("",*old_value,true);
			NoMatchWriteLine(fpnew,temp_node);
		}
		//Cmp_Node * cmp_tmp = new Cmp_Node;

		//cmp_tmp->main_key = temp_node->main_key;//strcmp?
		//cmp_tmp->value_old = old_value;
		//cmp_tmp->value_new = new_value;

        	//pthread_mutex_lock(&gb_data_precmp->mutex);
        	//gb_data_precmp->QueryList[temp_node->main_key]="difflist: ";
		//gb_data_precmp->cmp_queue->push(cmp_tmp);
        	//pthread_mutex_unlock(&gb_data_precmp->mutex);
        
		//delete i->second;
		gb_data_precmp->pre_map.erase(i++);
		}
    		fclose(fpnew);
		fclose(fpold);
	}

	Cmp_Node * cmp_tmp = new Cmp_Node;
	cmp_tmp->main_key = string("end!");
	cmp_tmp->value_new = NULL;
	cmp_tmp->value_old = NULL;
	gb_data_precmp->cmp_queue->push(cmp_tmp);
    //printf("end pre");
//	while(1){}
	return NULL;
}
