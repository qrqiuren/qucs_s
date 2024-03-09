/***************************************************************************
                           ngspicesimdialog.h
                             ----------------
    begin                : Sun Nov 9 2014
    copyright            : (C) 2014 by Vadim Kuznetsov
    email                : ra3xdh@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef EXTERNSIMDIALOG_H
#define EXTERNSIMDIALOG_H

#include <QtGui>

#include "schematic.h"
#include "ngspice.h"
#include "xyce.h"
#include "spicecompat.h"

class ExternSimDialog : public QDialog
{
    Q_OBJECT
private:

    Schematic *Sch;

    QPushButton *buttonStopSim;
    QPushButton *buttonSaveNetlist;
    QPushButton *buttonExit;

    QPlainTextEdit *editSimConsole;
    QListWidget *simStatusLog;

    QProgressBar *simProgress;

    QString workdir;

    Ngspice *ngspice;
    Xyce *xyce;

public:
    explicit ExternSimDialog(Schematic *sch,
                             bool netlist_mode = false);
    ~ExternSimDialog();

    bool wasSimulated;
    bool hasError;

private:
    void saveLog();
    void addLogEntry(const QString&text, const QIcon &icon);
    bool logContainsError(const QString &out);
    bool logContainsWarning(const QString &out);
    
signals:
    void simulated(ExternSimDialog *);
    void warnings();
    void success();
    
public slots:
    void slotSaveNetlist();
    void slotStart();

private slots:
    void slotProcessOutput();
    //void slotProcessXyceOutput();
    void slotNgspiceStarted();
    void slotNgspiceStartError(QProcess::ProcessError err);
    void slotStop();
    void slotSetSimulator();
    
};

#endif // EXTERNSIMDIALOG_H
