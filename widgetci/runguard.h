/**
--------------------------------------------------------------------------------
-   Module      :   runGuard.h
-   Description :   This file for usage with QT makes sure only a single
-                   instance of the program is run.
-   Author      :   SaZ from Stackoverflow
-                   Tim Zaman, 18-FEB-2016
--------------------------------------------------------------------------------
*/

#ifndef RUNGUARD_H
#define RUNGUARD_H

#include <QObject>
#include <QSharedMemory>
#include <QSystemSemaphore>


class RunGuard
{

public:
    RunGuard( const QString& key );
    ~RunGuard();

    bool isAnotherRunning();
    bool tryToRun();
    void release();

private:
    const QString key;
    const QString memLockKey;
    const QString sharedmemKey;

    QSharedMemory sharedMem;
    QSystemSemaphore memLock;

    Q_DISABLE_COPY( RunGuard )
};


#endif // RUNGUARD_H
