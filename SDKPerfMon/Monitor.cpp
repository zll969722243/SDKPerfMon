#include "Monitor.h"

#include "alilang_vpn_config.h"
#include <Windows.h>
#include <Psapi.h>

#include <QJsonObject>
#include <QJsonDocument>

Monitor::Monitor() : QThread(nullptr)
{
    m_nMaxReqs = 0;
    m_bReliableTest = false;
}

Monitor::~Monitor()
{
    m_nMaxReqs = 0;
    m_bReliableTest = false;
}

bool Monitor::Init()
{
    bool bOk = m_IPCDingTalkHelper.Init();
    if (!bOk)
    {
        return false;
    }

    start();
	return true;
}

bool Monitor::UnInit()
{
    m_IPCDingTalkHelper.UnInit();

	terminate();

	return true;
}

void Monitor::run()
{
	while (true)
	{
        if (m_nMaxReqs)
        {
            for (int index = 0; index < m_nMaxReqs; index++)
            {
                //调用DingDing的IPC接口;
                static bool bConnected = true;
                QJsonObject jsonObject;
                jsonObject.insert(VPN_STATUS, bConnected ? VPN_CONNECTED_STATUS : VPN_DISCONNECTED_STATUS);

                QJsonDocument jsonDoc(jsonObject);
                QByteArray baJsonData = jsonDoc.toJson();

                QString strMsg = baJsonData.data();
                QByteArray baMsg = strMsg.toLatin1();

                m_IPCDingTalkHelper.InvokeServer(VPN_STATUS_CHANGED_NOTIFY, baMsg.data());

                bConnected = !bConnected;
            }

            m_nMaxReqs = 0;
        }

        if (m_bReliableTest)
        {
            while (m_bReliableTest)
            {
                //调用DingDing的IPC接口;
                static bool bConnected = true;
                QJsonObject jsonObject;
                jsonObject.insert(VPN_STATUS, bConnected ? VPN_CONNECTED_STATUS : VPN_DISCONNECTED_STATUS);

                QJsonDocument jsonDoc(jsonObject);
                QByteArray baJsonData = jsonDoc.toJson();

                QString strMsg = baJsonData.data();
                QByteArray baMsg = strMsg.toLatin1();

                m_IPCDingTalkHelper.InvokeServer(VPN_STATUS_CHANGED_NOTIFY, baMsg.data());

                bConnected = !bConnected;
                sleep(60);//1min测一次
            }

        }

		sleep(1 * 1);//1s
	}

	return;
}

void Monitor::setMaxReqs(int nMaxNum)
{
    m_nMaxReqs = nMaxNum;

    return;
}

void Monitor::setReliableTest(bool bOk)
{
    m_bReliableTest = bOk;

    return;
}
