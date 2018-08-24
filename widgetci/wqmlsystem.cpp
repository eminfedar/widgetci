#include "wqmlsystem.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QProcess>

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

////// ---- --- ---- //////
////// ---- CPU ---- //////
////// ---- --- ---- //////
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
        if(diffTotal == 0) diffTotal = 1;
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


////// ---- --- ---- //////
////// ---- GPU ---- //////
////// ---- --- ---- //////
QString wqmlsystem::getNvidiaGPUName() const{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("/bin/bash -c \"nvidia-smi --query-gpu=name --format=csv,noheader\"");
    process.waitForFinished(1000);

    QString err = process.readAllStandardError();
    if(err.length() > 0) // If an error occur, -1
        return "Err: " + err;

    QString output = process.readAllStandardOutput();
    output = "NVIDIA " + output;
    output.remove('\n');
    process.kill();
    return output;
#endif
}
QString wqmlsystem::getIntelGPUName() const{
#ifdef Q_OS_LINUX

#endif
}
QString wqmlsystem::getAMDGPUName() const{
#ifdef Q_OS_LINUX

#endif
}

int wqmlsystem::getNvidiaGPUUsage() const{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("/bin/bash -c \"nvidia-smi --query-gpu=utilization.gpu --format=csv,noheader\"");
    process.waitForFinished(1000);

    if(process.readAllStandardError().length() > 0) // If an error occur, -1
        return -1;

    QString output = process.readAllStandardOutput();
    output.remove('%'); // Remove % Tag
    output.remove('\n');
    process.kill();
    return output.toInt();
#endif
}
int wqmlsystem::getIntelGPUUsage() const{
#ifdef Q_OS_LINUX

#endif
}
int wqmlsystem::getAMDGPUUsage() const{
#ifdef Q_OS_LINUX

#endif
}

int wqmlsystem::getNvidiaGPUMemoryUsage() const{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("/bin/bash -c \"nvidia-smi --query-gpu=memory.used --format=csv,noheader\"");
    process.waitForFinished(1000);

    if(process.readAllStandardError().length() > 0) // If an error occur, -1
        return -1;

    QString output = process.readAllStandardOutput();
    output.remove('\n');
    output.chop(4); // Remove MiB Tag
    process.kill();
    return output.toInt();
#endif
}
int wqmlsystem::getIntelGPUMemoryUsage() const{
#ifdef Q_OS_LINUX

#endif
}
int wqmlsystem::getAMDGPUMemoryUsage() const{
#ifdef Q_OS_LINUX

#endif
}

int wqmlsystem::getNvidiaGPUCurrentClockSpeed() const{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("/bin/bash -c \"nvidia-smi --query-gpu=clocks.gr --format=csv,noheader\"");
    process.waitForFinished(1000);

    if(process.readAllStandardError().length() > 0) // If an error occur, -1
        return -1;

    QString output = process.readAllStandardOutput();
    output.remove('\n');
    output.chop(4); // Remove MHz tag.
    process.kill();
    return output.toInt();
#endif
}
int wqmlsystem::getIntelGPUCurrentClockSpeed() const{
#ifdef Q_OS_LINUX

#endif
}
int wqmlsystem::getAMDGPUCurrentClockSpeed() const{
#ifdef Q_OS_LINUX

#endif
}

int wqmlsystem::getNvidiaGPUCurrentMemorySpeed() const{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("/bin/bash -c \"nvidia-smi --query-gpu=clocks.mem --format=csv,noheader\"");
    process.waitForFinished(1000);

    if(process.readAllStandardError().length() > 0) // If an error occur, -1
        return -1;

    QString output = process.readAllStandardOutput();
    output.remove('\n');
    output.chop(4); // Remove MHz tag.
    process.kill();
    return output.toInt();
#endif
}
int wqmlsystem::getIntelGPUCurrentMemorySpeed() const{
#ifdef Q_OS_LINUX

#endif
}
int wqmlsystem::getAMDGPUCurrentMemorySpeed() const{
#ifdef Q_OS_LINUX

#endif
}

int wqmlsystem::getNvidiaGPUFanSpeed() const{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("/bin/bash -c \"nvidia-smi --query-gpu=fan.speed --format=csv,noheader\"");
    process.waitForFinished(1000);

    if(process.readAllStandardError().length() > 0) // If an error occur, -1
        return -1;

    QString output = process.readAllStandardOutput();
    output.remove('\n');

    if(output == "[Not Supported]")
        return -1;

    process.kill();
    return output.toInt();
#endif
}
int wqmlsystem::getAMDGPUFanSpeed() const{
#ifdef Q_OS_LINUX

#endif
}

int wqmlsystem::getNvidiaGPUTemperature() const{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("/bin/bash -c \"nvidia-smi --query-gpu=temperature.gpu --format=csv,noheader\"");
    process.waitForFinished(1000);

    if(process.readAllStandardError().length() > 0) // If an error occur, -1
        return -1;

    QString output = process.readAllStandardOutput();
    output.remove('\n');

    if(output == "[Not Supported]")
        return -1;

    process.kill();
    return output.toInt();
#endif
}
int wqmlsystem::getIntelGPUTemperature() const{
#ifdef Q_OS_LINUX

#endif
}
int wqmlsystem::getAMDGPUTemperature() const{
#ifdef Q_OS_LINUX

#endif
}

int wqmlsystem::getNvidiaGPUPowerDraw() const{
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("/bin/bash -c \"nvidia-smi --query-gpu=power.draw --format=csv,noheader\"");
    process.waitForFinished(1000);

    if(process.readAllStandardError().length() > 0) // If an error occur, -1
        return -1;

    QString output = process.readAllStandardOutput();
    output.remove('\n');

    if(output == "[Not Supported]")
        return -1;

    process.kill();
    return output.toInt();
#endif
}
int wqmlsystem::getIntelGPUPowerDraw() const{
#ifdef Q_OS_LINUX

#endif
}
int wqmlsystem::getAMDGPUPowerDraw() const{
#ifdef Q_OS_LINUX

#endif
}



////// ---- ------- ---- //////
////// ---- STORAGE ---- //////
////// ---- ------- ---- //////
QList<QMap<QString, int>> wqmlsystem::getStorage(){
#ifdef Q_OS_LINUX
    QProcess process;
    process.start("df --type=ext4 --output=source,size,used,avail,pcent,target --sync -BM");
    process.waitForFinished(1000);

    QList<QMap<QString, QVariant>> driveList;
    QMap<QString, QVariant> drive;

    if(process.readAllStandardError().length() > 0){ // If an error occur, -1
        drive.insert("error", -1);
        driveList.append(drive);
        return driveList;
    }

    QString output = process.readAllStandardOutput();
    QList<QString> lines = output.split('\n');
    for (int i = 1; i < lines.length(); ++i) {
        qDebug() << lines[i];
    }

    process.kill();
    return driveList;
#endif
}



int wqmlsystem::getNetwork(){

}

int wqmlsystem::getProcessList(const int count = -1){

}

