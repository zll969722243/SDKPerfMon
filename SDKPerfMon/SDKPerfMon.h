#pragma once

#include <QtWidgets/QDialog>
#include "ui_SDKPerfMon.h"
#include <QTimer>

#include "Monitor.h"

class SDKPerfMon : public QDialog
{
    Q_OBJECT

public:
    SDKPerfMon(QWidget *parent = Q_NULLPTR);
    virtual ~SDKPerfMon();

public:
    bool Init();
    bool UnInit();

public slots:
    void onPressureTestBtn(bool checked);
    void onReliableTestBtn(bool checked);
    void onTimeout();
    void onValueChanged(int value);

private:
    Ui::SDKPerfMonClass ui;
    Monitor m_Monitor;
    QTimer m_timer;
    int m_nMaxReqs;
    qint64 m_secs;

public:
    static int m_nSuccessNums;
    static int m_nFailureNums;
    static int m_nTimedoutNums;
};
