#include "SetWebVideoMatrix.h"
#include "MPSClient.h"


CSetWebVideoMatrix::CSetWebVideoMatrix(const std::string& ip, unsigned short port, int timeOut)
	:_mpsClient(new MPSClient(ip, port, timeOut))
	,_result(cJSON_CreateObject())
{

}

CSetWebVideoMatrix::~CSetWebVideoMatrix()
{
	
}

bool CSetWebVideoMatrix::ComposeResult(cJSON* result)
{
// 	if (_result == NULL || result == NULL) {
// 		mLogError("cJSON result == NULL.");
// 		result = NULL;
// 		return false;
// 	}

	return true;
}

std::string CSetWebVideoMatrix::ComposeResult()
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


InterfaceResCode CSetWebVideoMatrix::SetWebVideoMatrix(std::string& sResult, int InPutChnnel, int OutputChnnel)
{
	mLogInfo("SetWebVideoMatrix...");

	CData oResult = SVSMAP();
	char cResult[RES_BUF_MAXLEN] = { 0 };

	if (!SetWebVideoMatrixInner( oResult, cResult, InPutChnnel, OutputChnnel))
	{
		mLogError("SetWebVideoMatrixInner failed !!!");
		return eInterfaceResCodeError;
	}

	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}


bool CSetWebVideoMatrix::SetWebVideoMatrixInner( CData& oResult, char* cResult, int InPutChnnel, int OutputChnnel)
{
	mLogDebug("run SetWebVideoMatrixInner(...)");

	//获取状态
	int SerialID = 0;//串口0
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	char Sendbuf[1024]   = {0};
	//cCmdLength = strlen(Cmd.c_str());
	//memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
	//index：命令索引，从1开始，1~50有效。

	//<指令类型,操作类型,输入/输出,通道,值>
	sprintf( cCmdSend, INTERFACE_SETWEB_SWITCH_VIDEO_SENDCMD, InPutChnnel, OutputChnnel); 
	cCmdLength = strlen(cCmdSend);
#if 1	
	MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _mpsClient->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	if(resCode != eResponseCodeSuccess){
		mLogError("GetSerialNew(...) error:"<<resCode);
		return false;
	}

	//判断是否关机成功
	char *arg[MAX_ARG_NUM] = {};
	for (int i = 0; i < MAX_ARG_NUM; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		  mLogError("calloc is error\n");
		  return false;
		 }
	 }
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s", arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9]);
	if (strcmp("ok>", arg[3]))
	{
		mLogError("SetWebVideoMatrix failed \n");
		return false;
	}
#endif	

	return true;

}
InterfaceResCode CSetWebVideoMatrix::SetWebVideoMatrixName(std::string& sResult, string InPutChnnelName1, string InPutChnnelName2, string InPutChnnelName3, string InPutChnnelName4, string InPutChnnelName5, string InPutChnnelName6, string InPutChnnelName7, string InPutChnnelName8, string InPutChnnelName9, string InPutChnnelName10, string InPutChnnelName11, string InPutChnnelName12, 
string OutPutChnnelName1, string OutPutChnnelName2, string OutPutChnnelName3, string OutPutChnnelName4, string OutPutChnnelName5, string OutPutChnnelName6, string OutPutChnnelName7, string OutPutChnnelName8)
{
	mLogInfo("SetWebVideoMatrixName...");

	sqlite3 *reg_db = NULL;
	char cmdbuf[32];
	char *register_db = "Audio_manage.db";
	
	STU stu;
	time_t cur_time = time(NULL);
	struct tm *tm_ptr;
	char *table = "create table MatrixName (id INTEGER, name TEXT, val TEXT, time TEXT)";
	char deltab[1024];
	char instab[1024];
	tm_ptr = localtime(&cur_time);
	reg_db = (sqlite3 *)create_db(register_db); //创建数据库,如已创建则忽略
	create_self_db_table(reg_db, table);  //创建注册表,如已创建则忽略
	
	//初始化数据结构
	for(int i = 1;i <= 20; i++)
	{
		memset(stu.val, 0, 256);
		stu.id = i;
		sprintf(stu.time, "%04d/%02d/%02d-%02d:%02d:%02d", tm_ptr->tm_year+1900, tm_ptr->tm_mon+1, tm_ptr->tm_mday, tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
		switch(i)
		{
			case 1:
				sprintf(stu.val,"%s",InPutChnnelName1.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 2:
				sprintf(stu.val,"%s",InPutChnnelName2.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;				
			case 3:
				sprintf(stu.val,"%s",InPutChnnelName3.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 4:
				sprintf(stu.val,"%s",InPutChnnelName4.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 5:
				sprintf(stu.val,"%s",InPutChnnelName5.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 6:
				sprintf(stu.val,"%s",InPutChnnelName6.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;				
			case 7:
				sprintf(stu.val,"%s",InPutChnnelName7.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 8:
				sprintf(stu.val,"%s",InPutChnnelName8.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 9:
				sprintf(stu.val,"%s",InPutChnnelName9.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 10:
				sprintf(stu.val,"%s",InPutChnnelName10.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;				
			case 11:
				sprintf(stu.val,"%s",InPutChnnelName11.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 12:
				sprintf(stu.val,"%s",InPutChnnelName12.c_str());
				sprintf(stu.name,"InPutChnnelName%d",i);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 13:
				sprintf(stu.val,"%s",OutPutChnnelName1.c_str());
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 14:
				sprintf(stu.val,"%s",OutPutChnnelName2.c_str());
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;				
			case 15:
				sprintf(stu.val,"%s",OutPutChnnelName3.c_str());
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 16:
				sprintf(stu.val,"%s",OutPutChnnelName4.c_str());
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 17:
				sprintf(stu.val,"%s",OutPutChnnelName5.c_str());
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 18:
				sprintf(stu.val,"%s",OutPutChnnelName6.c_str());
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;				
			case 19:
				sprintf(stu.val,"%s",OutPutChnnelName7.c_str());
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;
			case 20:
				sprintf(stu.val,"%s",OutPutChnnelName8.c_str());
				sprintf(stu.name,"OutPutChnnelName%d",i-12);
				sprintf(deltab,"delete from MatrixName where name='%s'",stu.name);
				sprintf(instab,"insert into MatrixName values(%d, \"%s\", \"%s\", \"%s\")", stu.id, stu.name, stu.val, stu.time);
				break;

		}

		delete_self_stu_db(reg_db, deltab);
		insert_self_stu_db(reg_db, instab); //add 到数据库 
		
	}
	sqlite3_close(reg_db);
	sResult = ComposeResult();
	cJSON_Delete(_result);
	return eInterfaceResCodeSuccess;
}

