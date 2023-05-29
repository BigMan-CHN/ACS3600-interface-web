#include "CSendSerialCmd.h"
#include <boost/lexical_cast.hpp>

CSendSerialCmd::CSendSerialCmd(string ip, int port, int timeOut)
    :_result(cJSON_CreateObject())
    ,_total(cJSON_CreateObject())
    ,_addressList(cJSON_CreateArray())
    ,_ip(ip)
    ,_port(port)
    ,_timeOut(timeOut)
    ,_client(new MPSClient(ip,port,timeOut))
    ,_AnalyParas()
{

}

bool CSendSerialCmd::SetSerialConfig( double Val, int ChnNum, int CtrlType, CData &oResult, char *cResult)
{
    mLogDebug("run SetSerialConfig(...)");
    //获取状态
	int SerialID = 1;//串口透传，串口1
    //add fwb 20201119
    int  cCmdLength   = 0;
    char cCmdSend[1024]   = {0};

   // cCmdLength = strlen(Cmd.c_str());
   // memcpy(cCmdSend, Cmd.c_str(), strlen(Cmd.c_str()));
	//<指令类型,操作类型,输入/输出,通道,值>
	if(ChnNum > 31)
	{
		mLogError("SetSerialConfig ChnNum ="<<ChnNum);
        return false;
	}
	float Valtmp = Val;
	float GainVal = Valtmp/10;
	char szReqBody[REQ_BODY_MAXLEN] = { 0 };

	sprintf(cCmdSend,"<9,1,%d,%d,%.1f>", CtrlType, ChnNum, Val);
//	sprintf(cCmdSend,"<9,1,%d,%d,%.1f>", CtrlType, ChnNum, GainVal);

	mLogDebug("cCmdSend:"<<cCmdSend);
    cCmdLength = strlen(cCmdSend);
	
    MPSOperationRes opResCode = eMPSResultOK;
	//发送cCmdSend信息给服务器
    ResponseCode resCode = _client->GetSerialNew(SerialID, cCmdSend, cCmdLength, opResCode, oResult,cResult);
	mLogDebug("run SetSerialConfig  cResult="<<cResult);
    if(resCode != eResponseCodeSuccess){
        mLogError("GetConsoleNew(...) error:"<<resCode);
        return false;
    }

    return true;
}

bool CSendSerialCmd::ComposeResult(cJSON *result)
{
    if(_result == NULL || result == NULL){
        mLogError("cJSON result == NULL.");
        result = NULL;
        return false;
    }

    if(_addressList == NULL){
        mLogWarn("cJSON _addressList == NULL.");
        return false;
    }
    if(_total == NULL){
        mLogWarn("cJSON _total == NULL.");
        return false;
    }

    //result中添加节点
	//cJSON_AddItemToObject(_result,"Total",_total);
    //cJSON_AddItemToObject(_result,"AddressList",_addressList);	
    return true;
}

string CSendSerialCmd::ComposeResult()
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

InterfaceResCode CSendSerialCmd::SendSerialCmd(string &sResult, double Val, int ChnNum, int CtrlType)
{
    mLogInfo("设置音量SendSerialCmd...");
    CData oResult = SVSMAP();
    char cResult[8192] = {0};
	char Result[8192] = {0};
#if 1

#endif 	//line左右声道
		if((ChnNum == 3||ChnNum == 5||ChnNum == 7||ChnNum == 9 )&& CtrlType == 1)
		{
		    if(SetSerialConfig(Val, ChnNum +1, CtrlType, oResult, Result) == false){
        	mLogError("Failed to run  GetConsoleConfig(...)");
        	return eInterfaceResCodeError;
    		}
			
			if (!_AnalyParas.ConvertSTRING2CJSONParseSerialCmdParam(oResult, Result, _result, _addressList,NULL))
		   {
			mLogError("Failed to run  ConvertSTRING2CJSONParseSerialCmdParam(...)");
			return eInterfaceResCodeError;
		   }
		}
		if(SetSerialConfig(Val, ChnNum, CtrlType, oResult, cResult) == false){
			mLogError("Failed to run  GetConsoleConfig(...)");
			return eInterfaceResCodeError;
		}


	    if (!_AnalyParas.ConvertSTRING2CJSONParseSerialCmdParam(oResult, cResult, _result, _addressList,NULL))
	   {
		mLogError("Failed to run  ConvertSTRING2CJSONParseSerialCmdParam(...)");
		return eInterfaceResCodeError;
	   }
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

    if( _AnalyParas.ConvertSTRING2CJSONSendConsoleCmd(oResult, cResult) == false){
        mLogError("Failed to run  ConvertSTRING2CJSONSendConsoleCmd(...)");
        return eInterfaceResCodeError;
    }
#endif 
    sResult = ComposeResult();
    return eInterfaceResCodeSuccess;

}
