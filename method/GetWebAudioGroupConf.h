#ifndef __GETWEBAUDIOGROUPCONF__H__
#define __GETWEBAUDIOGROUPCONF__H__

#include <TransmitClient.h>
#include <LibXML.h>
#include <cJSON.h>

#include "interfaceDefines.h"
#include "CAnalyzeParaProc.h"

/*
	获取音频分组方法类
*/

class MPSClient;
class CGetWebAudioGroupConf
{
public:
	CGetWebAudioGroupConf(const std::string& ip = MPS_IP, unsigned short port = MPS_PORT, int timeOut = MPS_TIMEOUT);
	~CGetWebAudioGroupConf();
	/**
	 * @brief GetWebAudioGroupConf 内部调用GetWebAudioGroupConfInner实现功能
	 * @param sResult
	 * @return true：成功，false：失败.
	 */
	InterfaceResCode GetWebAudioGroupConf(string& sResult);

private:
	/**
	 * @brief GetWebAudioGroupConfInner 构造、发送业务请求并获取响应
	 * @param oResult
	 * @param cResult
	 * @return true：成功，false：失败.
	 */
	bool GetWebAudioGroupConfInner(CData& oResult, char* cResult);
	bool ComposeResult(cJSON* result);
	/**
	 * @brief ComposeResult 将业务响应构造为Json数据格式
	 * @return Json数据：成功，""：失败.
	 */
	std::string ComposeResult();

	MPSClient*			_mpsClient;
	cJSON*				_result;
	cJSON*				_audioGroupList;
	CAnalyzeParaProc	_analyPara;
};


#endif //__GETWEBAUDIOGROUPCONF__H__
