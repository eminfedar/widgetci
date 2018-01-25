#include "wqmlsystem.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTimer>

wqmlsystem::wqmlsystem()
{
    // UPDATE CPU VALUES EVERY 100 MS.
    timer = new QTimer(this);
    updateCPUValues();
    connect(timer, &QTimer::timeout, [=]{
        updateCPUValues();
    });
    timer->start(100);
}

void wqmlsystem::updateCPUValues(){
#ifdef Q_OS_LINUX
    QFile file("/proc/stat");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    // Transfer the new array to the old one because we will fill new array with new values.
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 2; ++j) {
            run_idles1[i][j] = run_idles2[i][j];
            run_idles2[i][j] = 0;
        }
    }

    int core_count = 0;

    // FIRST CHECK
    QTextStream in(&file);
    QString line = in.readLine();
    while(!line.isNull()){
        if(line.left(3) == "cpu" && line.left(4) != "cpu "){

            QStringList usageList = line.split(" ");
            usageList.removeFirst();
            run_idles2[core_count][0] = usageList.at(0).toInt() + usageList.at(1).toInt() + usageList.at(2).toInt() + usageList.at(5).toInt() + usageList.at(6).toInt() + usageList.at(7).toInt() + usageList.at(8).toInt() + usageList.at(9).toInt(); // RUN
            run_idles2[core_count][1] = usageList.at(3).toInt() + usageList.at(4).toInt(); // IDLE
            core_count++;
        }
        line = in.readLine();
    }
    file.close();

    // COMPARE:
    cpu_cores.clear(); // Clear old ones.
    for (int i = 0; i < core_count; ++i) {
        int diffRun = run_idles2[i][0] - run_idles1[i][0];
        int diffIdle = run_idles2[i][1] - run_idles1[i][1];
        int diffTotal = diffIdle + diffRun;

        // Write new values.
        cpu_cores.append(100 * diffRun / diffTotal);
    }
#endif
}

QList<int> wqmlsystem::getCPUCoresUsages() const{
#ifdef Q_OS_LINUX
    return cpu_cores;
#endif
}

int wqmlsystem::getCPUUsage() const{
#ifdef Q_OS_LINUX
    int totalUsage = 0;
    for (int i = 0; i < cpu_cores.length(); ++i) {
        totalUsage += cpu_cores.at(i);
    }
    totalUsage /= cpu_cores.length();
    return totalUsage;
#endif
}

QString wqmlsystem::getCPUName() const{
#ifdef Q_OS_LINUX
    QFile file("/proc/cpuinfo");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return tr("ERR: Can't access the file\n(maybe some program using it):") + "/proc/cpuinfo";

    QTextStream in(&file);
    QString line = in.readLine();
    while(!line.isNull()){
        if(line.left(10) == "model name"){
            return (QString)(line.split(":")[1]).remove(0, 1);
            break;
        }
        line = in.readLine();
    }

    return tr("Can't find the CPU Name");
#endif
}

QList<int> wqmlsystem::getCPUCurrentClockSpeeds() const{
#ifdef Q_OS_LINUX
    QList<int> coreClocks;
    for(int i=0; i<64; i++){
        QString path = "/sys/devices/system/cpu/cpu" + QString::number(i) + "/cpufreq/scaling_cur_freq";
        QFile file(path);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            break;

        QTextStream in(&file);
        QString line = in.readAll();
        coreClocks.append( line.toInt() ); // For MHz

        file.close();
    }

    return coreClocks;
#endif
}

int wqmlsystem::getGPU(){

}

int wqmlsystem::getNetwork(){

}

int wqmlsystem::getProcessList(const int count = -1){

}

int wqmlsystem::getStorage(){

}
