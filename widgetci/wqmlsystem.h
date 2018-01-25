#ifndef WQMLSYSTEM_H
#define WQMLSYSTEM_H

#include <QObject>
#include <QTimer>
#include <QList>


class wqmlsystem : public QObject
{
    Q_OBJECT

public:
    wqmlsystem();

    // Timer & Refreshing values
    QTimer* timer;
    int timerInterval = 100;

    // CPU
    QList<int> cpu_cores;
    void updateCPUValues();
    int run_idles1[64][2] = {{0}}; // MAX 64 CORES (FOR XEON or EPYC PROCESSORS MAYBE...) [RUN, IDLE]
    int run_idles2[64][2] = {{0}}; // MAX 64 CORES (FOR XEON or EPYC PROCESSORS MAYBE...) [RUN, IDLE]

public slots:
    // CPU
    QList<int> getCPUCoresUsages() const; // Usage percents by core numbers
    int getCPUUsage() const; // Usage percent main CPU
    QString getCPUName() const; // CPU Name
    QList<int> getCPUCurrentClockSpeeds() const; // CPU Current Clock Speeds (Hz)

    int getGPU();
    int getStorage();
    int getNetwork();
    int getProcessList(const int count);
};

#endif // WQMLSYSTEM_H
