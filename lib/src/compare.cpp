#include <unistd.h>
#include "compare.h"
#define toString(x) #x
//#define toString(x) new string(tostring(x))
VDData *gb_data_cmp;
char b[4]="-->";
string a = string(b);

//json value 对比 产出：QueryList 的difflist；获取对比结果：拼接成Jsonvalue -》singleQuery 然后放入single list
int JsonCompare (Cmp_Node * temp)
{
	string main_key = temp->main_key;
	Json::Value* single_query = new Json::Value;
	hash_map<string,string> *QL = &gb_data_cmp->QueryList;
//	gb_data_cmp->cmp_result = new Json::Value;
	//收集对比信息
	//Compare(Json::Value* new_value , Json::Value* old_value , Json::Value & cmp_result , string main_key , Json::Value & single_query)
	Compare(temp->value_new,temp->value_old,gb_data_cmp->cmp_result,main_key,single_query,*QL);

	Sig_Node* single_tmp = new Sig_Node;
	single_tmp->main_key = main_key;
	single_tmp->value_merge = single_query;
	//TODO 引用
	gb_data_cmp->single_query.insert(gb_data_cmp->single_query.end(),single_tmp);

	return 0;
}
void merge_value(Json::Value * new_value , bool flag ,Json::Value & result)
{
	if(new_value->isNull())return ;
	//Json::Value result = new Json::Value;

	Json::Value::Members mem = new_value->getMemberNames();
	for (Json::Value::Members::iterator iter = mem.begin(); iter != mem.end(); iter++)
	{
	    if ((*new_value)[*iter].type() == Json::objectValue)
	    {
	    	Json::Value child_result = new Json::Value;
	    	merge_value(&(*new_value)[*iter],flag , child_result);
	    	//result.append(child_result);
	    	result[*iter]=child_result;
	    }
	    else if ((*new_value)[*iter].type() == Json::arrayValue)
	    {
	    	//TODO arrayvalue 不一定能成功 待验证
	        int cnt = (*new_value)[*iter].size();
	        for (int i = 0; i < cnt; i++)
	        {
	           // print(v[*iter][i]);TODO
	        	result[*iter][i]=flag?a+(*new_value)[*iter][i].asString():(*new_value)[*iter][i].asString()+a;
	        	//merge_value(new_value[*iter][i],flag);
	        }
	    }
	    else if ((*new_value)[*iter].type() == Json::stringValue)
	    {
	        //cout<<v[*iter].asString()<<endl;
	    	result[*iter]=flag?a+(*new_value)[*iter].asString():(*new_value)[*iter].asString()+a;
	    }
	    else if ((*new_value)[*iter].type() == Json::realValue)
	    {
	    	result[*iter]=flag?a+toString(new_value[*iter].asDouble()):toString(new_value[*iter].asDouble())+a;
	        //cout<<new_value[*iter].asDouble().toString()<<endl;
	     }
	     else if ((*new_value)[*iter].type() == Json::uintValue)
	     {
	    	 result[*iter]=flag?a+toString(new_value[*iter].asUInt()):toString(new_value[*iter].asUInt())+a;
	     }
	     else
	     {
	    	 result[*iter]=flag?a+toString(new_value[*iter].asInt()):toString(new_value[*iter].asInt())+a;
	         //cout<<v[*iter].asInt()<<endl;
	     }
	 }
}
//single query 赋值收集  内存泄漏
bool Compare(Json::Value* new_value , Json::Value* old_value , Json::Value * cmp_result , string main_key , Json::Value * single_query,hash_map<string,string> &QL){
    if ( new_value->isNull()||old_value->isNull()){
          printf("new_value is Null || old value is Null!");  
          return false;
    }
    bool ret = true;
	Json::Value::Members mem_new = new_value->getMemberNames();
	Json::Value::Members mem_old = old_value->getMemberNames();

	Json::Value::Members::iterator iter_new = mem_new.begin() ;
	Json::Value::Members::iterator iter_old = mem_old.begin();

	set<string> tmp_set;
	bool flag_set =true;
	cout<<"new value:"<<*new_value<<endl;
	cout<<"old value:"<<*old_value<<endl;

	while (iter_old != mem_old.end()||iter_new != mem_new.end())
	{
		if(iter_old == mem_old.end())flag_set = false;

		if(flag_set){
				//tmp_set.insert(*iter_old);
				//tmp_set[*iter_old]=true;
				tmp_set.insert(*iter_old);
				iter_old++;
			}
		else{
			//tmp_set.insert(*iter_new);
			tmp_set.insert(*iter_new);
			iter_new++;
		}
	}
	for(set<string>::iterator i = tmp_set.begin();i!=tmp_set.end();i++)
	{
		cout<<"JsonKey:"<<*i<<endl;
		cout<<"new value:"<<(*new_value)[*i]<<endl;
		cout<<"old value:"<<(*old_value)[*i]<<endl;
		if((*new_value)[*i].isNull()||(*old_value)[*i].isNull()){
			cout<<"One is Null!"<<endl;
			//写入 本层对比+写入
//            int temp=0;
            //(*cmp_result)[*i].isNull()?(*cmp_result)[*i]=1:(*cmp_result)[*i]=(*cmp_result)[*i].asInt()+1;
	    		//if(!(*new_value)[*i].type()==Json::objectValue&&!(*old_value)[*i].type()==Json::objectValue)
			//{
    			//	if((*cmp_result)[*i].isNull())
			//		(*cmp_result)[*i]=1;
			//	else if((*cmp_result)[*i].type()==Json::intValue)
			//		(*cmp_result)[*i]=(*cmp_result)[*i].asInt()+1;
			//}
			string tmp1=*i;tmp1 +="_number";
			(*cmp_result)[*i][tmp1].isNull()?(*cmp_result)[*i][tmp1]=1:(*cmp_result)[*i][tmp1]=(*cmp_result)[*i][tmp1].asInt()+1;
// 			(*cmp_result)[*i].isObject()?printf("1"):printf("2");
			string tmp = *i;tmp +="_Main_Key";
			(*cmp_result)[*i][tmp].isNull()?(*cmp_result)[*i][tmp]=main_key:(*cmp_result)[*i][tmp]=(*cmp_result)[*i][tmp].asString()+"_"+main_key;
			cout<<"cmp_result:(OneisNull)"<<(*cmp_result)<<endl;
//			(*cmp_result)[*i]["MainKey"].isNull()?(*cmp_result)[*i]["MainKey"]=main_key:(*cmp_result)[*i]["MainKey"]=(*cmp_result)[*i]["MainKey"].asString()+main_key;
			//写入QL 列表
			QL[main_key]= QL[main_key] + *i;
			cout<<"QL["<<main_key<<"]:"<<QL[main_key]<<endl;

			//Json::Value* child_single ;//= new Json::Value;
			if((*new_value)[*i].isNull()){
				(*single_query)[*i]["diff_new"]="null";
				(*single_query)[*i]["diff_old"]=(*old_value)[*i];
				//child_single = &(*old_value)[*i];
//				merge_value(&(*old_value)[*i],false,child_single);
			}else{
			//	child_single = &(*new_value)[*i];
				(*single_query)[*i]["diff_new"]=(*new_value)[*i];
				(*single_query)[*i]["diff_old"]="null";
//				merge_value(&(*new_value)[*i],true,child_single);
			}

			//(*single_query)[*i]=*child_single;
            continue;
		}else if((*new_value)[*i].type()!=(*old_value)[*i].type()){
			//本层对比+写入
			cout<<"Not the same type!"<<endl;
			QL[main_key]= QL[main_key] + *i;
			//TODO 未知是否可行 可能不可预知
//			(*single_query)[*i]=(*old_value)[*i].asString()+"-->"+(*new_value)[*i].asString();
			//(*single_query)[*i]="Warning!!! old type and new type are different";
			//可以数组 或者子obj
			(*single_query)[*i]["diff_new"]=(*new_value)[*i];
			(*single_query)[*i]["diff_old"]=(*old_value)[*i];
			string tmp1=*i;tmp1 +="_number";
			(*cmp_result)[*i][tmp1].isNull()?(*cmp_result)[*i][tmp1]=1:(*cmp_result)[*i][tmp1]=(*cmp_result)[*i][tmp1].asInt()+1;
			string tmp = *i;tmp +="_Main_Key";
			(*cmp_result)[*i][tmp].isNull()?(*cmp_result)[*i][tmp]=main_key:(*cmp_result)[*i][tmp]=(*cmp_result)[*i][tmp].asString()+"_"+main_key;
            ret &=false;
            continue;
		}else if ((*new_value)[*i].type()==(*old_value)[*i].type()){
                if((*old_value)[*i].type()==Json::objectValue){
			        //递归子层对比+写入
			        //Json::Value* child_value = new Json::Value;
				Json::Value* child_value =&(*cmp_result)[*i];
			        Json::Value* child_single = new Json::Value;
				//cout<<"000.old_cmp_result:(Object)key:"<<*i<<endl;
				//cout<<*child_value<<endl;
			        hash_map<string,string>* subQL = new hash_map<string,string>;
                    		(*subQL)[main_key]=" ";
			        bool result = Compare(&((*new_value)[*i]),&((*old_value)[*i]),child_value,main_key,child_single,*subQL);
				if(!result)
				{
					QL[main_key]= QL[main_key] +*i;
					string tmp1=*i;tmp1 +="_number";
					(*cmp_result)[*i][tmp1].isNull()?(*cmp_result)[*i][tmp1]=1:(*cmp_result)[*i][tmp1]=(*cmp_result)[*i][tmp1].asInt()+1;
					string tmp = *i;tmp +="_Main_Key";
					(*cmp_result)[*i][tmp].isNull()?(*cmp_result)[*i][tmp]=main_key:(*cmp_result)[*i][tmp]=(*cmp_result)[*i][tmp].asString()+"_"+main_key;
				}
				//QL[main_key]= QL[main_key] +(*subQL)[main_key];
//			        Compare(&((*new_value)[*i]),&((*old_value)[*i]),child_value,*i,child_single);
			        //(*cmp_result).append(child_value);
			        Json::Value tmp = *child_value;
				//cout<<"111.cmp_result:(Object)"<<(*cmp_result)<<endl;
			        //if(!tmp.isNull()){
                            	//	(*cmp_result)[*i]=tmp;
                    		//}
//			        (*cmp_result)[*i]["MainKey"].isNull()?(*cmp_result)[*i]["MainKey"]=main_key:(*cmp_result)[*i]["MainKey"]+=main_key;
			        //(*single_query).append(child_single);
			        (*single_query)[*i]=*child_single;
                    //TODO 
                    delete subQL;
                    ret &=result;
                    continue;
                }else if(Json::stringValue ==(*new_value)[*i].type() ){
					if((*new_value)[*i].asString()!=(*old_value)[*i].asString()){
						QL[main_key]= QL[main_key] + *i;
						string old = (*old_value)[*i].asString();
						string nw = (*new_value)[*i].asString();
						old+=a;old+=nw;
//						(*single_query)[*i]=(*old_value)[*i].asString()+a+(*new_value)[*i].asString();
						(*single_query)[*i]=old;
						string tmp1=*i;tmp1 +="_number";
						(*cmp_result)[*i][tmp1].isNull()?(*cmp_result)[*i][tmp1]=1:(*cmp_result)[*i][tmp1]=(*cmp_result)[*i][tmp1].asInt()+1;
						string tmp = *i;tmp +="_Main_Key";
						(*cmp_result)[*i][tmp].isNull()?(*cmp_result)[*i][tmp]=main_key:(*cmp_result)[*i][tmp]=(*cmp_result)[*i][tmp].asString()+"_"+main_key;
						ret &=false;
                        continue;
					}
				}else if(Json::realValue ==(*new_value)[*i].type()){//TODO
					if((*new_value)[*i].asDouble()!=(*old_value)[*i].asDouble()){
						QL[main_key]= QL[main_key] + *i;
						(*single_query)[*i]=Json::valueToString((*old_value)[*i].asDouble())+a+Json::valueToString((*new_value)[*i].asDouble());
						string tmp1=*i;tmp1 +="_number";
						//(*cmp_result)[tmp1].isNull()?(*cmp_result)[tmp1]=1:(*cmp_result)[tmp1]=(*cmp_result)[tmp1].asInt()+1;
						(*cmp_result)[*i][tmp1].isNull()?(*cmp_result)[*i][tmp1]=1:(*cmp_result)[*i][tmp1]=(*cmp_result)[*i][tmp1].asInt()+1;
						string tmp = *i;tmp +="_Main_Key";
						(*cmp_result)[*i][tmp].isNull()?(*cmp_result)[*i][tmp]=main_key:(*cmp_result)[*i][tmp]=(*cmp_result)[*i][tmp].asString()+"_"+main_key;


//						(*cmp_result)[*i]["MainKey"].isNull()?(*cmp_result)[*i]["MainKey"]=main_key:(*cmp_result)[*i]["MainKey"]=(*cmp_result)[*i]["MainKey"].asString()+main_key;
						ret &=false;
                        continue;
					}
				}else if(Json::uintValue ==(*new_value)[*i].type()){//TODO
					if((*new_value)[*i].asUInt()!=(*old_value)[*i].asUInt()){
						QL[main_key]= QL[main_key] + *i;
						(*single_query)[*i]=Json::valueToString((*old_value)[*i].asUInt())+a+Json::valueToString((*new_value)[*i].asUInt());
						string tmp1=*i;tmp1 +="_number";
						//(*cmp_result)[tmp1].isNull()?(*cmp_result)[tmp1]=1:(*cmp_result)[tmp1]=(*cmp_result)[tmp1].asInt()+1;
						(*cmp_result)[*i][tmp1].isNull()?(*cmp_result)[*i][tmp1]=1:(*cmp_result)[*i][tmp1]=(*cmp_result)[*i][tmp1].asInt()+1;
						string tmp = *i;tmp +="_Main_Key";
						(*cmp_result)[*i][tmp].isNull()?(*cmp_result)[*i][tmp]=main_key:(*cmp_result)[*i][tmp]=(*cmp_result)[*i][tmp].asString()+"_"+main_key;

//						(*cmp_result)[*i]["MainKey"].isNull()?(*cmp_result)[*i]["MainKey"]=main_key:(*cmp_result)[*i]["MainKey"]=(*cmp_result)[*i]["MainKey"].asString()+main_key;
						ret &=false;
                        continue;
					}
				}else if(Json::intValue ==(*new_value)[*i].type()){//TODO
					if((*new_value)[*i].asInt()!=(*old_value)[*i].asInt()){
						QL[main_key]= QL[main_key] + *i;
						(*single_query)[*i]=Json::valueToString((*old_value)[*i].asInt())+a+Json::valueToString((*new_value)[*i].asInt());
						string tmp1=*i;tmp1 +="_number";
						//(*cmp_result)[tmp1].isNull()?(*cmp_result)[tmp1]=1:(*cmp_result)[tmp1]=(*cmp_result)[tmp1].asInt()+1;
						(*cmp_result)[*i][tmp1].isNull()?(*cmp_result)[*i][tmp1]=1:(*cmp_result)[*i][tmp1]=(*cmp_result)[*i][tmp1].asInt()+1;
						string tmp = *i;tmp +="_Main_Key";
						(*cmp_result)[*i][tmp].isNull()?(*cmp_result)[*i][tmp]=main_key:(*cmp_result)[*i][tmp]=(*cmp_result)[*i][tmp].asString()+"_"+main_key;

//					(*cmp_result)[*i]["MainKey"].isNull()?(*cmp_result)[*i]["MainKey"]=main_key:(*cmp_result)[*i]["MainKey"]=(*cmp_result)[*i]["MainKey"].asString()+main_key;
						ret &=false;
					    continue;
                    }
				}else if(Json::booleanValue ==(*new_value)[*i].type()){//TODO
					if((*new_value)[*i].asBool()!=(*old_value)[*i].asBool()){
						if((*new_value)[*i].asBool())(*single_query)[*i]="false-->true";
                        else (*single_query)[*i]="true-->false";
                        QL[main_key]= QL[main_key] + *i;
						string tmp1=*i;tmp1 +="_number";
						//(*cmp_result)[*i].isNull()?(*cmp_result)[*i]=1:(*cmp_result)[*i]=(*cmp_result)[*i].asInt()+1;
						//(*cmp_result)[tmp1].isNull()?(*cmp_result)[tmp1]=1:(*cmp_result)[tmp1]=(*cmp_result)[tmp1].asInt()+1;
						(*cmp_result)[*i][tmp1].isNull()?(*cmp_result)[*i][tmp1]=1:(*cmp_result)[*i][tmp1]=(*cmp_result)[*i][tmp1].asInt()+1;
						string tmp = *i;tmp +="_Main_Key";
						(*cmp_result)[*i][tmp].isNull()?(*cmp_result)[*i][tmp]=main_key:(*cmp_result)[*i][tmp]=(*cmp_result)[*i][tmp].asString()+"_"+main_key;
						ret &=false;
                        continue;
					}
				}else if(Json::arrayValue == (*new_value)[*i].type()){
					Json::Value* child_result = new Json::Value;
					Json::Value* single_arr = new Json::Value;

//					int ts = strcmp((*new_value)[*i].asCString(),(*old_value)[*i].asCString());
//                    if(! (*new_value)[*i].compare((*old_value)[*i])){
//                    if(!ts){
				    if(!cmp_arrayvalue(new_value,old_value,main_key,child_result,single_arr,i)){
                        			QL[main_key]= QL[main_key] + *i;
						string tmp1=*i;tmp1 +="_number";
						//(*cmp_result)[*i].isNull()?(*cmp_result)[*i]=1:(*cmp_result)[*i]=(*cmp_result)[*i].asInt()+1;
						//(*cmp_result)[tmp1].isNull()?(*cmp_result)[tmp1]=1:(*cmp_result)[tmp1]=(*cmp_result)[tmp1].asInt()+1;
						(*cmp_result)[*i][tmp1].isNull()?(*cmp_result)[*i][tmp1]=1:(*cmp_result)[*i][tmp1]=(*cmp_result)[*i][tmp1].asInt()+1;
						string tmp = *i;tmp +="_Main_Key";
						(*cmp_result)[*i][tmp].isNull()?(*cmp_result)[*i][tmp]=main_key:(*cmp_result)[*i][tmp]=(*cmp_result)[*i][tmp].asString()+"_"+main_key;

						//收集
//						cmp_arrayvalue(new_value,old_value,main_key,child_result,single_arr,i);
						(*single_query)[*i]=*single_arr;
					}else {
						//TODO bug
						(*single_query)[*i]=(*new_value)[*i];
					}
					delete child_result;
//					QL[main_key]= QL[main_key] + *i;
				    continue;
                }
            	printf("single");
	}
	printf("check single");
        (*single_query)[*i]=(*old_value)[*i];
        //(*single_query)[*i]="123";
    }
    return ret;
//		(*single_query)[*i]=(*old_value)[*i].asString();
}


bool cmp_arrayvalue(Json::Value *new_value ,Json::Value *old_value ,string main_key,Json::Value* array_result ,Json::Value* single_query,set<string>::iterator i)
{

	unsigned int tmp = 0;
	bool result = true;
    if((*new_value)[*i].size()!=(*old_value)[*i].size()){	
	    	string tmp1=*i;tmp1 +="_number";
    		//(*array_result)[tmp1].isNull()?(*array_result)[tmp1]=1:(*array_result)[tmp1]=(*array_result)[tmp1].asInt()+1;		
	    //(*array_result)[*i].isNull()?(*array_result)[*i]=1:(*array_result)[*i]=(*array_result)[*i].asInt()+1;
		(*array_result)[*i][tmp1].isNull()?(*array_result)[*i][tmp1]=1:(*array_result)[*i][tmp1]=(*array_result)[*i][tmp1].asInt()+1;
		string tmp = *i;tmp +="_Main_Key";
		(*array_result)[*i][tmp].isNull()?(*array_result)[*i][tmp]=main_key:(*array_result)[*i][tmp]=(*array_result)[*i][tmp].asString()+"_"+main_key;
        (*single_query)["diff_old"]=(*old_value)[*i];
        (*single_query)["diff_new"]=(*new_value)[*i];
        result = false;
//        return result;
    }else{
        tmp = (*new_value)[*i].size();
    }
//	(*new_value)[*i].size()<(*old_value)[*i].size()?tmp = (*new_value)[*i].size():tmp=(*old_value)[*i].size();
//				if((*new_value)[*i].size()==(*old_value)[*i].size()){
    for(unsigned int j=0;j<tmp;j++){
		Json::Value* cmp_result = new Json::Value;
		Json::Value* single = new Json::Value;
		hash_map<string,string> *ql = new hash_map<string,string>;
		if((*new_value)[*i][j].type()==(*old_value)[*i][j].type()){
			bool flag =true ;
			if((*new_value)[*i][j].type()==Json::objectValue){
//				Json::Value* child = new Json::Value;
//				Json::Value* single = new Json::Value;
                (*ql)[main_key]=" ";
				bool _ret = Compare(&((*new_value)[*i][j]),&((*old_value)[*i][j]),cmp_result,*i,single,*ql);
				flag = false;
                result&=_ret;
			}else if((*new_value)[*i][j].type()==Json::stringValue&&(*new_value)[*i][j].asString()!=(*old_value)[*i][j].asString()){
//				QL[main_key]= gb_data_cmp->QueryList[main_key] + *i;
				string old = (*old_value)[*i][j].asString();
				string nw = (*new_value)[*i][j].asString();
				old+=a;old+=nw;
				*single=old;
//				(*cmp_result)[*i].isNull()?(*cmp_result)[*i]=1:(*cmp_result)[*i]=(*cmp_result)[*i].asInt()+1;
//				(*cmp_result)[*i]["MainKey"].isNull()?(*cmp_result)[*i]["MainKey"]=main_key:(*cmp_result)[*i]["MainKey"]=(*cmp_result)[*i]["MainKey"].asString()+main_key;
				flag = false;
                result &=false;
			}else if((*new_value)[*i][j].type()==Json::realValue&&(*new_value)[*i][j].asDouble()!=(*old_value)[*i][j].asDouble()){
//				QL[main_key]= QL[main_key] + *i;
				*single=Json::valueToString((*old_value)[*i][j].asDouble())+a+Json::valueToString((*new_value)[*i][j].asDouble());

//				(*cmp_result)[*i].isNull()?(*cmp_result)[*i]=1:(*cmp_result)[*i]=(*cmp_result)[*i].asInt()+1;
//				(*cmp_result)[*i]["MainKey"].isNull()?(*cmp_result)[*i]["MainKey"]=main_key:(*cmp_result)[*i]["MainKey"]=(*cmp_result)[*i]["MainKey"].asString()+main_key;
				flag =false;
                result &=false;
			}else if((*new_value)[*i][j].type()==Json::uintValue){
				if((*new_value)[*i][j].asUInt()!=(*old_value)[*i][j].asUInt()){
//					QL[main_key]= QL[main_key] + *i;
					*single=Json::valueToString((*old_value)[*i][j].asUInt())+a+Json::valueToString((*new_value)[*i][j].asUInt());

//					(*cmp_result)[*i].isNull()?(*cmp_result)[*i]=1:(*cmp_result)[*i]=(*cmp_result)[*i].asInt()+1;
//					(*cmp_result)[*i]["MainKey"].isNull()?(*cmp_result)[*i]["MainKey"]=main_key:(*cmp_result)[*i]["MainKey"]=(*cmp_result)[*i]["MainKey"].asString()+main_key;
					flag =false;
                    result &=false;
				}
			}else if((*new_value)[*i][j].type()==Json::intValue){
				if((*new_value)[*i][j].asInt()!=(*old_value)[*i][j].asInt()){
//					QL[main_key]= QL[main_key] + *i;
					*single=Json::valueToString((*old_value)[*i][j].asInt())+a+Json::valueToString((*new_value)[*i][j].asInt());

//					(*cmp_result)[*i].isNull()?(*cmp_result)[*i]=1:(*cmp_result)[*i]=(*cmp_result)[*i].asInt()+1;
//					(*cmp_result)[*i]["MainKey"].isNull()?(*cmp_result)[*i]["MainKey"]=main_key:(*cmp_result)[*i]["MainKey"]=(*cmp_result)[*i]["MainKey"].asString()+main_key;
					flag =false;
                    result &=false;
				}
			}else if((*new_value)[*i][j].type()==Json::arrayValue){
                if((*new_value)[*i][j].asBool()!=(*old_value)[*i][j].asBool())
                {
                       if((*new_value)[*i][j].asBool()){
                               *single="false-->true";
                       }else{
                            *single="true-->false";
                       }
                       flag = false;
                       result &=false;
                }
            }else if((*new_value)[*i][j].type()==Json::arrayValue){
				flag = false;
				//TODO bug
			    	bool temp_=cmp_arrayvalue(new_value,old_value,main_key,cmp_result,single,i);
                		result &=temp_;
			}
			if(flag){
				if((*old_value)[*i][j].type()==Json::stringValue)*single=(*old_value)[*i][j].asString();
				if((*old_value)[*i][j].type()==Json::realValue)*single=(*old_value)[*i][j].asDouble();
				if((*old_value)[*i][j].type()==Json::intValue)*single=(*old_value)[*i][j].asInt();
				if((*old_value)[*i][j].type()==Json::uintValue)*single=(*old_value)[*i][j].asUInt();
				if((*old_value)[*i][j].type()==Json::booleanValue)*single=(*old_value)[*i][j].asBool();
			}
		}
        if (!(*cmp_result).isNull()){
		    array_result->append(*cmp_result);
        }
        if (!(*single).isNull()){
                single_query->append(*single);
        }
        delete cmp_result;
		delete ql;
	}

	return result;
}

//single list 写入文件
void singleListWriteToFile()
{
	Json::FastWriter fw;
	FILE *fp = fopen("SQ.log","aw+");
		while(gb_data_cmp->single_query.size()>0)
		{
			Sig_Node* tmp = gb_data_cmp->single_query.back();
//			gb_data_cmp->single_query.pop_back();
            
			string result = fw.write(*tmp->value_merge);
//			string result = tmp->value_merge->toStyledString();
			string TAT = tmp->main_key + " "+ result;
            fprintf(fp,"%s",TAT.data());
			gb_data_cmp->single_query.pop_back();
			delete tmp->value_merge;
            delete tmp;
		}
	fclose(fp);
}
//querylist 写入文件 TODO 如果是超级大文件可能list 占用挺大内存
void QLWriteToFile()
{
	FILE *fp = fopen("QL.log","wr+");
	for(hash_map<string,string>::iterator iter=gb_data_cmp->QueryList.begin();iter!=gb_data_cmp->QueryList.end();iter++)
	{
		string tmp =iter->first+" "+iter->second;
		fprintf(fp,"%s\n",tmp.data());
//		fprintf(fp,"%s",tmp.data());

	}
	fclose(fp);
}
void CmpResultWriteToFile()
{
	Json::FastWriter fw;
//	string result = gb_data_cmp->cmp_result->toStyledString();
	string result = fw.write(*gb_data_cmp->cmp_result);
	FILE *fp = fopen("cmpresult.log","wr+");
	fprintf(fp,"%s",result.c_str());
    delete gb_data_cmp->cmp_result;
//	fprintf(fp,"%s",result.data());
	fclose(fp);
}
//compare 线程入口
void * cmp_proccessor_thread(void * args)
{
	gb_data_cmp = (VDData *)args;
	gb_data_cmp->cmp_result = new Json::Value;
	while(1)
	{
        int tt =0;
//            pthread_mutex_lock(&gb_data_cmp->mutex);
		if(!gb_data_cmp->cmp_queue->empty()){
            pthread_mutex_lock(&gb_data_cmp->mutex);
			Cmp_Node * temp = gb_data_cmp->cmp_queue->front();
//            pthread_mutex_unlock(&gb_data_cmp->mutex);
			if(temp->main_key=="end!")
			{
				singleListWriteToFile();
				QLWriteToFile();
				CmpResultWriteToFile();
//				while(1){}
				return NULL;
//				exit(0);
			}else
			{
				JsonCompare(temp);
				singleListWriteToFile();tt =0;
			}
            delete temp->value_old;
            delete temp->value_new;
            delete temp;
//            pthread_mutex_lock(&gb_data_cmp->mutex);
			gb_data_cmp->cmp_queue->pop();
            pthread_mutex_unlock(&gb_data_cmp->mutex);
		}else{
            sleep(1);
            tt++;
            if(tt>20)return NULL;
            printf("sleep");    
        }
	}
}
