#include "SDKPerfMon.h"

#pragma execution_character_set("utf-8")

#include <QPushButton>
#include <QString>
#include <QDateTime>
#include <QDebug>

int SDKPerfMon::m_nSuccessNums = 0;
int SDKPerfMon::m_nFailureNums = 0;
int SDKPerfMon::m_nTimedoutNums = 0;

SDKPerfMon::SDKPerfMon(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);
}

SDKPerfMon::~SDKPerfMon()
{
    m_nMaxReqs = 0;
    m_secs = 0;
}

bool SDKPerfMon::Init()
{
    bool bOk = m_Monitor.Init();
    if (!bOk)
    {
        ui.infoLabel->setText("Monitor.Init() Failed!");
        ui.pressureTestBtn->setDisabled(true);
        return false;
    }

    ui.maxNumVSlider->setRange(1, 100000);
    ui.maxNumVSlider->setValue(3000);
    int nValue = ui.maxNumVSlider->value();
    ui.maxReqsLabel->setText(QString::number(nValue));

    connect(ui.pressureTestBtn, &QPushButton::clicked, this, &SDKPerfMon::onPressureTestBtn);
    connect(ui.reliableTestBtn, &QPushButton::clicked, this, &SDKPerfMon::onReliableTestBtn);
    connect(&m_timer, &QTimer::timeout, this, &SDKPerfMon::onTimeout);
    connect(ui.maxNumVSlider, &QSlider::valueChanged, this, &SDKPerfMon::onValueChanged);
    m_timer.setInterval(100);

    return true;
}

bool SDKPerfMon::UnInit()
{
    m_Monitor.UnInit();

    disconnect(ui.pressureTestBtn, &QPushButton::clicked, this, &SDKPerfMon::onPressureTestBtn);
    disconnect(&m_timer, &QTimer::timeout, this, &SDKPerfMon::onTimeout);

    return true;
}

void SDKPerfMon::onPressureTestBtn(bool checked)
{
    m_nMaxReqs = ui.maxNumVSlider->value();

    m_secs = QDateTime::currentSecsSinceEpoch();
    m_Monitor.setMaxReqs(m_nMaxReqs);
    m_Monitor.setReliableTest(false);
    ui.pressureTestBtn->setEnabled(false);
    setWindowTitle("正在进行压测!");
    m_timer.start();

    return;
}

void SDKPerfMon::onReliableTestBtn(bool checked)
{
    m_timer.stop();
    m_Monitor.setMaxReqs(0);
    m_Monitor.setReliableTest(true);
    ui.reliableTestBtn->setEnabled(false);
    ui.pressureTestBtn->setEnabled(false);//进入可靠性测试将不允许再进入压力测试
    ui.infoLabel->setText("正在进行稳定性测试!\n");
}

void SDKPerfMon::onTimeout()
{
    if (m_nSuccessNums + m_nFailureNums + m_nTimedoutNums >= m_nMaxReqs)
    {
        static int nCnt = 0;
        qint64 secs = QDateTime::currentSecsSinceEpoch() - m_secs;

        QString strMsg;
        strMsg.sprintf("第%u轮测试,本轮耗时%llus,并发量%u,成功%u次,失败%u次,超时%u次\n", ++nCnt, secs, m_nMaxReqs, m_nSuccessNums, m_nFailureNums, m_nTimedoutNums);
        QString strText = ui.infoLabel->text();
        strMsg = strText + strMsg;

        ui.infoLabel->setText(strMsg);

        m_nSuccessNums = 0;
        m_nFailureNums = 0;
        m_nTimedoutNums = 0;

        m_timer.stop();

        ui.pressureTestBtn->setEnabled(true);
    }

    return;
}

void SDKPerfMon::onValueChanged(int value)
{
    ui.maxReqsLabel->setText(QString::number(value));

    return;
}
