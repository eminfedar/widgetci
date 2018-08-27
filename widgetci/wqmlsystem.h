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
    QString getCPUName() const; // CPU Name
    QList<int> getCPUCoresUsages() const; // Usage percents by core numbers
    int getCPUUsage() const; // Usage percent main CPU
    QList<int> getCPUCurrentClockSpeeds() const; // CPU Current Clock Speeds (Hz)

    // GPU
    QString getNvidiaGPUName() const; // GPU Name
    QString getIntelGPUName() const;
    QString getAMDGPUName() const;

    int getNvidiaGPUUsage() const; // GPU Usage percent (%)
    int getIntelGPUUsage() const;
    int getAMDGPUUsage() const;

    int getNvidiaGPUMemoryUsage() const; // GPU RAM Usage percent (MB)
    int getIntelGPUMemoryUsage() const;
    int getAMDGPUMemoryUsage() const;

    int getNvidiaGPUCurrentClockSpeed() const; // GPU Clock Speeds (MHz)
    int getIntelGPUCurrentClockSpeed() const;
    int getAMDGPUCurrentClockSpeed() const;

    int getNvidiaGPUCurrentMemorySpeed() const; // GPU Memory Clock Speeds (MHz)
    int getIntelGPUCurrentMemorySpeed() const;
    int getAMDGPUCurrentMemorySpeed() const;

    int getNvidiaGPUFanSpeed() const; // Fan Speed (RPM)
    int getAMDGPUFanSpeed() const;

    int getNvidiaGPUTemperature() const; // Temperature (Celcius)
    int getIntelGPUTemperature() const;
    int getAMDGPUTemperature() const;

    int getNvidiaGPUPowerDraw() const; // Power Draw (Watts)
    int getIntelGPUPowerDraw() const;
    int getAMDGPUPowerDraw() const;

    // STORAGE
    QList<QMap<QString, int>> getStorage(); // First Dimension is filesystems. (like /dev/sda1, /dev/sda2),
    // Second Dimension is informations about the filesystem. ( Total, Used, Empty, Percent, Mounted On )

    // NETWORK
    int getNetwork();

    // PROCESS LIST
    int getProcessList(const int count);
};

#endif // WQMLSYSTEM_H
