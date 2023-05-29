#include "GetWebVideoMatrix.h"
#include "MPSClient.h"


CGetWebVideoMatrix::CGetWebVideoMatrix(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
	, _ParamList(cJSON_CreateArray())
{

}

CGetWebVideoMatrix::~CGetWebVideoMatrix()
{
	
}

bool CGetWebVideoMatrix::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CGetWebVideoMatrix::ComposeResult()
{
// 	cJSON* result = cJSON_CreateObject();
// 	if (ComposeResult(result) == false) {
// 		cJSON_Delete(result);
// 		return "";
// 	}
// 	else {
// 		char* cRes = cJSON_Print(_result);
// 		std::string res = cRes;
// 		cJSON_Delete(result);
// 		return res;
// 	}

	//assume the setmethod's response code is always okay. -_-!
	char* cRes = cJSON_Print(_result);
	std::string res = cRes;
	return res;
}


InterfaceResCode CGetWebVideoMatrix::GetWebVideoMatrix(std::string& sResult, int VideoMatrix)
{
	mLogInfo("GetWebVideoMatrix...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!GetWebVideoMatrixInner( oResult, cResult, VideoMatrix))
	{
		mLogError("GetWebVideoMatrixInner failed !!!");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}


bool CGetWebVideoMatrix::GetWebVideoMatrixInner( CData& oResult, char* cResult, int VideoMatrix)
{
	mLogDebug("run GetWebVideoMatrixInner(...)");


	int SerialID = 0;
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Sendbuf[1024]   = {0};


	//<指令类型,操作类型,输入/输出,通道,值>
	sprintf( cCmdSend, INTERFACE_GETWEB_SWITCH_VIDEO_SENDCMD); 
	cCmdLength = strlen(cCmdSend);
#if 1	
	MPSOperationRes opResCode = eMPSResultOK;
	
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetSerialNew(...) error:"<<resCode);
		return false;
	}
	mLogDebug("cResult = "<<cResult);
	char *arg[32] = {0};
	for (int i = 0; i < 20; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return false;
		 }
	 }
	
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9], arg[10], arg[11], arg[12], arg[13], arg[14], arg[15], arg[16], arg[17], arg[18]);
	if (strcmp("<GetVideoStatus", arg[0]))
	{
		mLogError("GetWebVideoMatrix failed \n");
		return false;
	}
	char tmp[5] = {0};
	//0：获取输入条选择接口 1：获取视频矩阵接口
	if(VideoMatrix == 0)
	{
		VideoMatrix = 0;
	}
	else if(VideoMatrix == 1)
	{
		VideoMatrix = 8;
	}else{
		mLogError("VideoMatrix val is no right \n");
		return false;
	}
		
	for(int j = 1; j < 9; j++)
	{
		
		if ((j+VideoMatrix) == 16)		{
			int ret = strlen(arg[j+VideoMatrix]);
			memcpy(tmp, arg[j+VideoMatrix], ret-1);
			cJSON* arrayItem = cJSON_CreateObject();
			cJSON_AddItemToObject(arrayItem, "InPutChnnel", cJSON_CreateNumber(atoi(tmp)));
			cJSON_AddItemToObject(arrayItem, "OutputChnnel", cJSON_CreateNumber(j+VideoMatrix));
			cJSON_AddItemToArray(_ParamList, arrayItem);	
		}else{
			cJSON* arrayItem = cJSON_CreateObject();
			cJSON_AddItemToObject(arrayItem, "InPutChnnel", cJSON_CreateNumber(atoi(arg[j+VideoMatrix])));
			cJSON_AddItemToObject(arrayItem, "OutputChnnel", cJSON_CreateNumber(j+VideoMatrix));
			cJSON_AddItemToArray(_ParamList, arrayItem);
		}
	}
	cJSON_AddItemToObject(_result, "VideoMatrixlist", _ParamList);
#endif	

	return true;

}
InterfaceResCode CGetWebVideoMatrix::GetWebVideoMatrixName(std::string& sResult)
{
	mLogInfo("GetWebVideoMatrixName...");
	sqlite3 *reg_db = NULL;
	char buf[1024];
	char *register_db = "Audio_manage.db";
	
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	char find_table[1024];
	char *table = "create table MatrixName (id INTEGER, name TEXT, val TEXT, time TEXT)";
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库,如已创建则忽略
	create_self_db_table(reg_db, table);  //创建注册表,如已创建则忽略
	
	for(int i = 1;i <= 20; i++)
	{
		memset(buf, 0, 1024);
		switch(i)
		{
			case 1:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 2:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;				
			case 3:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 4:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 5:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 6:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;				
			case 7:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 8:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 9:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 10:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;				
			case 11:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 12:
				
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 13:
				
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 14:
				
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;				
			case 15:
				
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 16:
				
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 17:
				
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 18:
				
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;				
			case 19:
				
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;
			case 20:
				
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(find_table,"select * from MatrixName where name='%s'", stu.name);
				break;

		}
		
		find_self_stu_db(reg_db, find_table, buf);
		cJSON_AddItemToObject(_result, stu.name, cJSON_CreateString(buf));

	}
	sqlite3_close(reg_db);
	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}


