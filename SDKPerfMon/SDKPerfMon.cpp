#include "SDKPerfMon.h"

#include <QPushButton>
#include <QString>

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
    ui.maxNumVSlider->setValue(1000);

    connect(ui.pressureTestBtn, &QPushButton::clicked, this, &SDKPerfMon::onPressureTestBtn);
    connect(&m_timer, &QTimer::timeout, this, &SDKPerfMon::onTimeout);
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
    m_Monitor.setMaxReqs(m_nMaxReqs);
    ui.pressureTestBtn->setEnabled(false);
    
    m_timer.start();
    
    return;
}

void SDKPerfMon::onTimeout()
{
    if (m_nSuccessNums + m_nFailureNums + m_nTimedoutNums == m_nMaxReqs)
    {
        QString strMsg = QString("���β��Խ��,������ %1 ��,�ɹ� %2 ��,ʧ�� %3 ��,��ʱ %4 ��\n").arg(m_nSuccessNums).arg(m_nFailureNums).arg(m_nTimedoutNums);
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
