#include "restic.h"

#include <iostream>
#include <ostream>
#include <qprocess.h>

void processFinished(int exitCode, QProcess::ExitStatus exitStatus);

Restic::Restic(QObject *parent) : QObject(parent), process(new QProcess(this)) {
    QObject::connect(this->process, &QProcess::finished, this,
                     &Restic::processFinished);
}

void Restic::snapshots() {
    if (process->state() == QProcess::NotRunning) {
        this->process->startCommand("restic snapshots");
    }
}

void Restic::processFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitStatus == QProcess::NormalExit) {
        auto buf = this->process->readAllStandardOutput();
        std::cout << "Process exited with code: " << exitCode << std::endl;
        std::cout << "Stdout: " << buf.toStdString() << std::endl;
        std::cout << "EOF" << std::endl;
    } else {
        auto buf = this->process->readAllStandardError();
        std::cout << "Process exited with code: " << exitCode << std::endl;
        std::cout << "Stderr: " << buf.toStdString() << std::endl;
        std::cout << "EOF" << std::endl;
    }
}
