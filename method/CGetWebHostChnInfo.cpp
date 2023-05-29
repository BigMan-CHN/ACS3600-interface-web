#include "CGetWebHostChnInfo.h"
#include <boost/lexical_cast.hpp>

CGetWebHostChnInfo::CGetWebHostChnInfo(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_channal(cJSON_CreateObject())
    ,_addressList(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CGetWebHostChnInfo::getWebChnConfig(int ChnNum,CData &oResult, char *cResult)
{
    mLogDebug("run getChnConfig(...)");
    //获取状态

    //add fwb 20201104
    //int  i = 0;
    //int  itmplen   = 0;
    //char cChnTmpbuf[512] = {0};
    char cChnCmd[4096]   = {0};
	int VidCapNum;
	char *ReqBody = NULL;
	//ChnNum大于13，返回所有（13组）通道信息
//	if(ChnNum > 14)
//	{
//		sprintf(cChnCmd,"eth1.conf\nnet_send%d.app_info\ncomp1.osd1.content\ncomp2.osd1.content\n", ChnNum);
//		for(int i = 1; i <= 4; i++)
//		{
//			ReqBody = cChnCmd + strlen(cChnCmd);
//			sprintf(ReqBody,"yuan%d.info\n", i);
//		}
//		for(int i = 1; i <= 8; i++)
//		{
//			ReqBody = cChnCmd + strlen(cChnCmd);
//			sprintf(ReqBody,"vid_cap%d.ch1.osd1.content\n", i);
//		}
//		for(int i = 1; i <= 14; i++)
//		{
//			ReqBody = cChnCmd + strlen(cChnCmd);
//			sprintf(ReqBody,"net_send%d.rec_current_status\nnet_send%d.rec_task_id\n", i, i);
//		}
//	}
	if(ChnNum > 14)
	{
		sprintf(cChnCmd,"eth1.conf\nnet_send100.app_info\ncomp100.osd1.content\nyuan100.info\nvid_cap100.ch1.osd1.content\n");
	}
#if 0	
	else if(ChnNum <= 8)//ChnNum小于等于8，返回对应的通道信息
	{
		VidCapNum = ChnNum;
		sprintf(cChnCmd,"net_send%d.app_info\nvid_cap%d.ch1.osd1.content\n", net_send_num, VidCapNum);
		ReqBody = cChnCmd + strlen(cChnCmd);
		sprintf(ReqBody,"eth1.conf\nnet_send%d.rec_current_status\nnet_send%d.rec_task_id\n", net_send_num, net_send_num);
	}//ChnNum介于9-13，返回对应的通道信息，无vid_cap网管协议，vid_cap只有1-8通道
	else if((ChnNum > 8)&&(ChnNum <= 13))
	{
		sprintf(cChnCmd,"eth1.conf\nnet_send%d.app_info\nnet_send%d.rec_current_status\nnet_send%d.rec_task_id\n", net_send_num, net_send_num, net_send_num);
//		ReqBody = cChnCmd + strlen(cChnCmd);
//		sprintf(ReqBody,"net_send%d.rec_current_status\nnet_send%d.rec_task_id\n", ChnNum, ChnNum);
	}
#else	
	//ChnNum小于14，返回单通道信息
	else if(ChnNum <= 2)//合成,前面按使用
	{
		sprintf(cChnCmd,"eth1.conf\nnet_send%d.app_info\nnet_send%d.rec_current_status\nnet_send%d.rec_task_id\ncomp%d.osd1.content\nnet_send%d.usr\nnet_send%d.encrypt\n", ChnNum, ChnNum, ChnNum, ChnNum, ChnNum, ChnNum);
	}
	else if((ChnNum >= 3)&&(ChnNum <= 10))//ChnNum小于等于8，返回对应的通道信息
	{
		sprintf(cChnCmd,"eth1.conf\nnet_send%d.app_info\nvid_cap%d.ch1.osd1.content\nnet_send%d.rec_current_status\nnet_send%d.rec_task_id\n", ChnNum, ChnNum-2, ChnNum, ChnNum);
	}
	else if((ChnNum >= 11)&&(ChnNum <= 14))//ChnNum介于11-13，返回对应的通道信息，无字幕
	{
		sprintf(cChnCmd,"eth1.conf\nnet_send%d.app_info\nnet_send%d.rec_current_status\nnet_send%d.rec_task_id\n", ChnNum, ChnNum, ChnNum);
	}
#endif
#if 0
    for (i = 1; i < RtspNum + 1; i++)
    {
        sprintf(cChnTmpbuf,INTERFACE_GETHOSTCHNINFO_SENDCMD,i,i,i,i,i);
        //printf("%d RtspNum %d ctmpbuff %s", i, RtspNum, cChnTmpbuf);
        memcpy(cChnCmd + itmplen, cChnTmpbuf, strlen(cChnTmpbuf));
        itmplen += strlen(cChnTmpbuf);
    }
    //printf("len %d \n%s", strlen(cDecCmd), cDecCmd);
#endif 

    //add fwb 20201027
    char cCmdSend[1024] = {0};
    char cHeadCmd[4] = {0};
    //char *cTmpCmd = INTERFACE_GETHOSTCONFIG_SENDCMD;
    int  cCmdLength = 0;
    memset(cCmdSend, 0, sizeof(1024)); 

    cCmdLength = strlen(cChnCmd) + 4;
    cHeadCmd[0] = 0; //当Buf[0]为0x00时，表示网管协议不加密,当Buf[0]为0x01时，表示网管协议加密
    cHeadCmd[1] = 2; //获取参数指令（GET）
    cHeadCmd[2] = strlen(cChnCmd) >> 8;  
    cHeadCmd[3] = strlen(cChnCmd) & 0xff;

    memcpy(cCmdSend, cHeadCmd, 4);
    memcpy(cCmdSend+4, cChnCmd, strlen(cChnCmd));

    MPSOperationRes opResCode = eMPSResultOK;
    ResponseCode resCode = _client->GetConfigNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConfig(...) error:"<<resCode);
        return false;
    }
	mLogDebug("run getChnConfig(...) cResult="<<cResult+4);

    return true;
}

bool CGetWebHostChnInfo::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    //result中添加节点
    cJSON_AddItemToObject(_result,GET_HOST_CONFIG_RES_WEB_CHN,_addressList);
    return true;
}

string CGetWebHostChnInfo::ComposeResult()
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

InterfaceResCode CGetWebHostChnInfo::GetWebHostChnInfo(string &sResult,int ChnNum)
{
    //TODO
    mLogInfo("GetWebHostChnInfo...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
	char Result[8192] = {0};
	cJSON* temList[15];
	string Method = "GetWebHostChnInfo";
    //printf("GetWebHostChnInfo ChnNum %d\n", ChnNum);
#if 1
    if(getWebChnConfig(ChnNum, oResult, cResult) == false){
        mLogError("Failed to run  getMPSConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 
    if(ChnNum > 14)
    {
   		for(int i = 1; i <= 14; i++)
		{//用于生成节点，存储解析后的参数
			temList[i] = cJSON_CreateObject();
		}
    }else{
		temList[ChnNum] = cJSON_CreateObject();
	}
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

    if( _AnalyParas.ConvertSTRING2CJSONGetWebHostChnInfo(oResult, cResult, _result, temList, _addressList, Method, ChnNum) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONGetWebHostChnInfo(...)");
        return eInterfaceResCodeError;
    }


    sResult = ComposeResult();
	cJSON_Delete(_result);
    return eInterfaceResCodeSuccess;
}


