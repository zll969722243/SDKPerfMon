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
    void onTimeout();

private:
    Ui::SDKPerfMonClass ui;
    Monitor m_Monitor;
    QTimer m_timer;
    int m_nMaxReqs;

public:
    static int m_nSuccessNums;
    static int m_nFailureNums;
    static int m_nTimedoutNums;
};
