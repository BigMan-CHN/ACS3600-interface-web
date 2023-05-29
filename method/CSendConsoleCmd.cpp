#include "CSendConsoleCmd.h"

CSendConsoleCmd::CSendConsoleCmd(string ip, int port, int timeOut)
    :_ip(ip)
    ,_result(cJSON_CreateObject())
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
{

}

bool CSendConsoleCmd::GetConsoleConfig(string Cmd,CData &oResult, char *cResult)
{
    mLogDebug("run GetConsoleConfig(...)");
    //获取状态

    //add fwb 20201119
    int  cCmdLength   = 0;
    char cCmdSend[1024]   = {0};
	int SerialID = 1;

    cCmdLength = strlen(Cmd.c_str());
    memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));

    MPSOperationRes opResCode = eMPSResultOK;
	ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	mLogDebug("cResult:"<<cResult);
    //ResponseCode resCode = _client->GetConsoleNew(cCmdSend,cCmdLength,opResCode,oResult,cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConsoleNew(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSendConsoleCmd::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    return true;
}

string CSendConsoleCmd::ComposeResult()
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

InterfaceResCode CSendConsoleCmd::SendConsoleCmd(string &sResult, string cmd)
{
    mLogInfo("发送中控指令SendConsoleCmd...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
#if 1
    if(GetConsoleConfig(cmd, oResult, cResult) == false){
        mLogError("Failed to run  GetConsoleConfig(...)");
        return eInterfaceResCodeError;
    }
#endif 
//	int iRecvLength  = strlen(cResult);
//	printf("iRecvLength :%d\n",iRecvLength);
//	if(cResult == NULL)
//		printf("cResult :NULL\n");
//    for (int i = 0; i < 100; i++)
//    {
//		printf("%c %d ", cResult[i],cResult[i]);
//    }

#if 0
    int iRecvLength = (cResult[2] << 8) + cResult[3] + 4;
    printf("SendConsoleCmd\n");
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
//    if( _AnalyParas.ConvertSTRING2CJSONSendConsoleCmd(oResult, cResult) == false){
//        mLogError("Failed to run  ConvertSTRING2CJSONSendConsoleCmd(...)");
//        return eInterfaceResCodeError;
//    }
	cJSON_AddItemToObject(_result, "cResult", cJSON_CreateString(cResult));

    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;
}
