#include "CGetWebHostConfig.h"
#include <boost/lexical_cast.hpp>

CGetWebHostConfig::CGetWebHostConfig(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_network(cJSON_CreateObject())
    ,_channels(cJSON_CreateArray())
    ,_sysinfo(cJSON_CreateObject())
    ,_diskinfo(cJSON_CreateObject())
    ,_decnum(cJSON_CreateObject())
    ,_otherinfo(cJSON_CreateObject())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

InterfaceResCode CGetWebHostConfig::GetWebHostConfig(string &sResult)
{
    //TODO
    mLogInfo("GetWebHostConfig...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
	string Method = "GetWebHostConfig";
#if 1
    if(getMPSConfig(oResult, cResult) == false){
        mLogError("Failed to run  getMPSConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 
    //getMPSConfig(oResult, cResult);
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("getMPSConfig\n");
    for (int i = 0; i < iRecvLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d", cResult[i]);
	    }
	    else 
	    {
		    printf("%c", cResult[i]);
	    }
    }
#endif 
 
#if 1
    if( _AnalyParas.ConvertSTRING2CJSONGetWebHostConfig(oResult, cResult, _network, _sysinfo, _otherinfo, Method) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetWebHostConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 
        //result中添加节点
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_SYSINFO,_sysinfo);
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_NETWORK,_network);
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_OTHERINFO,_otherinfo);

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}

bool CGetWebHostConfig::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    return true;
}

string CGetWebHostConfig::ComposeResult()
{
    cJSON * result = cJSON_CreateObject();
    if(ComposeResult(result) == false){
        cJSON_Delete(result);
        return "";
    }else{
        char *cRes = cJSON_Print(_result);
        string res = cRes;
        cJSON_Delete(result);
        return res;
    }
}

bool CGetWebHostConfig::getMPSConfig(CData &oResult, char *cResult)
{
    mLogDebug("run GetMPSConfig(...)");
    //获取状态
    //CData oResult = SVSMAP();

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETWEBHOSTCONFIG_SENDCMD;
//	char *cTmpCmd = "sys.name\nsys._hwver\nsys._swver\nsys.sn\nsys.dev_id\neth1.conf\neth1.macaddr\nstatus.totalsize\nstatus.freedisk\nstatus.cpu\nstatus.mem\nstatus.temp\n";
	char *cTmpCmd = "sys.all\neth1.all\nstatus.all\n";
    int  cCmdLength = 0;
	//sprintf(cTmpCmd,"sys.name\nsys._hwver\nsys._swver\nsys.sn\nsys.dev_id\neth1.conf\neth1.macaddr\nstatus.totalsize\nstatus.freedisk\nstatus.cpu\nstatus.mem\nstatus.temp\n");
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cTmpCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cTmpCmd) >> 8;  
    cHeadCmd[3] = strlen(cTmpCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cTmpCmd, strlen(cTmpCmd));
    MPSOperationRes opResCode = eMPSResultOK;

    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CGetWebHostConfig::GetDiskStateInner(CData &oResult, char *cResult)
{
    mLogDebug("run GetDiskStateInner(...)");
 #if 0   
    //获取状态
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};

	char *cTmpCmd = "sys.all\neth1.all\nstatus.all\n";
    int  cCmdLength = 0;
	//sprintf(cTmpCmd,"sys.name\nsys._hwver\nsys._swver\nsys.sn\nsys.dev_id\neth1.conf\neth1.macaddr\nstatus.totalsize\nstatus.freedisk\nstatus.cpu\nstatus.mem\nstatus.temp\n");
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cTmpCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cTmpCmd) >> 8;  
    cHeadCmd[3] = strlen(cTmpCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cTmpCmd, strlen(cTmpCmd));
    MPSOperationRes opResCode = eMPSResultOK;

    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }
#else
    int mntFlag = 0;
    string strtmp;
    FILE *mntfile;  
    struct mntent *mntent;
    char *filename = "/proc/mounts";

    // setmntent用来打开/etc/mtab或者同样格式的table文件，参数filename为table文件的路径，例如”/etc/mtab”
    // 参数type为打开文件的模式（与open类型，例如“r”为只读打开），成功时，返回FILE指针（用于mntent操作），失败时返回NULL
    mntfile = setmntent(filename, "r");
    if (!mntfile) 
    {
            //printf("Failed to read mtab file, error [%s]\n",strerror(errno));
            mLogDebug("Failed to read mtab file(/proc/mounts), error :"<<strerror(errno));
            return false;
    }  

    // getmntent用来读取文件的每一行，解析每一行的参数到mntent结构，mntent结构的存储空间是静态分配的（不需要free），结构的值会在下一次getmntent时被覆盖。
    // 参数filep是setmntent返回的FILE指针，成功时返回指向mntent的指针，错误时返回NULL
    while(mntent = getmntent(mntfile))
    {
        // printf("%s, %s, %s, %s\n",
        //     mntent->mnt_fsname,  /* 文件系统对应的设备路径或者服务器地址  */
        //     mntent->mnt_dir,     /* 文件系统挂载到的系统路径 */
        //     mntent->mnt_type,    /* 文件系统类型: ufs, nfs, 等  */
        //     mntent->mnt_opts);   /* 文件系统挂载参数，以逗号分隔  */
        mLogDebug("Mount info:"<<mntent->mnt_fsname<<","<<mntent->mnt_dir<<","<<mntent->mnt_type<<","<< mntent->mnt_opts);

        strtmp = mntent->mnt_fsname;
        if(strtmp.find("/dev/sda") != strtmp.npos)
        {
            mntFlag = 1;
            mLogDebug("Disk mount success!");
        }
    }

    //endmntent用来关闭打开的table文件，总是返回1
    endmntent(mntfile);

    if(mntFlag == 0)
    {       
        mLogDebug("Disk mount failed!");
    }

     cJSON_AddItemToObject(_result, "DiskMountState", cJSON_CreateNumber(mntFlag));
#endif

    return true;
}

bool CGetWebHostConfig::GetAudioAllGainInner(CData& oResult, char* cResult)
{
	mLogDebug("run GetAudioAllGainInner(...)");
		
	//获取状态
	int SerialID = 1;//串口透传，串口1
	
	int val = 0;
	int Flag = 0;
	int  cCmdLength   = 0;
	char cCmdSend[1024]   = {0};
	
	//<指令类型,操作类型,输入/输出,通道,值>
	
	sprintf(cCmdSend,"<9,2,2,0,0>");

	//sprintf(cCmdSend,INTERFACE_GET_AUDIO_GAIN_CONF_SENDCMD, CtrlType, ChnNum, val);

	
	cCmdLength = strlen(cCmdSend);
	
	MPSOperationRes opResCode = eMPSResultOK;
ReSend:	
	//发送cCmdSend信息给服务器
	ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	mLogDebug("GetAudioAllGainInner cResult:"<<cResult);
	
	if(resCode != eResponseCodeSuccess && resCode == eResponseCodeErrorRecvTimeOut){
		
		cJSON_AddItemToObject(_result, "AudioBoardState", cJSON_CreateNumber(0));
		return true;
	}
	else if(resCode != eResponseCodeSuccess && resCode != eResponseCodeErrorRecvTimeOut)
	{
		mLogError("GetConsoleNew(...) error:"<<resCode);
		return false;		
	}
		
	 if(!strcmp(cResult, "<error>"))//发送指令格式错误，Business直接返回此数据
	{
		if (Flag < 2)
		{
			mLogDebug("GetAudioAllGainInner Send GetSerialNew:"<<Flag);
			Flag++;
			goto ReSend;
		}else{
			mLogError("GetAudioAllGainInner GetSerialNew error:"<<Flag);
			//cJSON_AddItemToObject(_result, "AudioBoardState", cJSON_CreateNumber(2));
			return false;
		}
	}

	//解析
	char *arg[40] = {};
	for (int i = 0; i < 40; i ++)
	{
		arg[i] = (char *)calloc(1, strlen(cResult)+1);
		if(NULL == arg[i]) 
		{
		   mLogError("calloc is error\n");
		   return false;
		}
	}
	
	//mLogDebug("cResult:"<<cResult);
	
	//命令格式cResult: "<9（指令类型）,2（操作类型，1:SET，2:GET）,2（输入2/输出1）,1（通道0~31）,80（值）,ok（状态）>"
	sscanf(cResult, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",\
	arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], arg[6], arg[7], arg[8], arg[9],\
	arg[10], arg[11], arg[12], arg[13], arg[14], arg[15], arg[16], arg[17], arg[18], arg[19],\
	arg[20], arg[21], arg[22], arg[23], arg[24], arg[25], arg[26], arg[27], arg[28], arg[29], \
	arg[30], arg[31], arg[32], arg[33], arg[34], arg[35], arg[36], arg[37], arg[38], arg[39]);
	//2:输入
	if(!strcmp(arg[2], "2"))
	{	//判断是否出错
		if(strcmp(arg[27], "ok>"))
	    {
		  mLogError("GetAudioAllGainInner input Get gain failed !!!\n");
		  cJSON_AddItemToObject(_result, "AudioBoardState", cJSON_CreateNumber(2));
	    }
	    else
	    {
			cJSON_AddItemToObject(_result, "AudioBoardState", cJSON_CreateNumber(1));
			mLogDebug("GetAudioAllGainInner input Get gain Success !!!\n");
	    }
	}
	else
	{  //1:输出
		if(strcmp(arg[23], "ok>"))
		{
			 mLogError("GetAudioAllGainInner output Get gain failed !!!\n");
			 cJSON_AddItemToObject(_result, "AudioBoardState", cJSON_CreateNumber(2));

		}
		else
	    {
			cJSON_AddItemToObject(_result, "AudioBoardState", cJSON_CreateNumber(1));
			mLogDebug("GetAudioAllGainInner output Get gain Success !!!\n");
	    }
	}
	return true;

}

InterfaceResCode CGetWebHostConfig::GetHardDevsState(string &sResult)
{
    //TODO
    mLogInfo("GetHardDevsState...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
	string Method = "GetHardDevsState";
#if 1
    if(GetDiskStateInner(oResult, cResult) == false){
        mLogError("Failed to run  GetDiskStateInner(...)");
        return eInterfaceResCodeError;
    }

//    if(GetAudioAllGainInner(oResult, cResult) == false){
//	    mLogError("Failed to run  GetAudioAllGainInner(...)");
//	    return eInterfaceResCodeError;
//    }
#endif 
    //getMPSConfig(oResult, cResult);
#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("getMPSConfig\n");
    for (int i = 0; i < iRecvLength; i++)
    {
	    if (i < 4)
	    {
		    printf("%d", cResult[i]);
	    }
	    else 
	    {
		    printf("%c", cResult[i]);
	    }
    }
#endif 
 
#if 0
    if( _AnalyParas.ConvertSTRING2CJSONGetWebHostConfig(oResult, cResult, _network, _sysinfo, _otherinfo, Method) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetWebHostConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}
