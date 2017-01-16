/*
 * CoRo Tactile Sensor UI
 * Copyright (C) 2016  Shahbaz Youssefi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "communicator.h"
#include <QSerialPortInfo>

void MainWindow::refreshPorts()
{
    refreshPortsAutoconnect(true);
}

void MainWindow::refreshPortsAutoconnect(bool allowAutoconnect)
{
    int index = 0;
    ui->availablePorts->clear();
    bool foundFinger = false;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QString desc = info.description();
        if (desc.length() >= 20)
            desc = desc.left(17) + "...";
        ui->availablePorts->addItem(info.portName() + " (" + desc + ")");

        // Note: for some strange reason, on windows the description is not the updated CoRo Tactile Sensor
        if (info.description() == "CoRo Tactile Sensor" || info.description() == "Cypress USB UART")
        {
            ui->availablePorts->setCurrentIndex(index);
            foundFinger = true;
        }

        ++index;
    }

    if (foundFinger && ui->autoconnect->isChecked() && allowAutoconnect)
        openConnection();
}

void MainWindow::openCloseConnection()
{
    if (communicator)
        closeConnection();
    else
        openConnection();
}

void MainWindow::openConnection()
{
    if (ui->availablePorts->count() < 0)
    {
        connectionFailed("No serial ports detected");
        return;
    }

    QString port = ui->availablePorts->currentText();
    port = port.left(port.indexOf(' '));

    communicator = new Communicator(this, port.toUtf8().data(), READ_DATA_PERIOD_MS);
    if (communicator->portError())
    {
        switch (communicator->portError())
        {
        case QSerialPort::PermissionError:
            connectionFailed("Insufficient permission to open port");
            break;
        case QSerialPort::OpenError:
            connectionFailed("Port is already opened by another application");
            break;
        default:
            connectionFailed("Could not open port");
            break;
        }
        delete communicator;
        communicator = NULL;
        return;
    }

    connectionOpened(port.toUtf8().data());
    communicator->start();
}

void MainWindow::closeConnection()
{
    delete communicator;
    communicator = false;

    connectionClosed("Not connected");
    refreshPortsAutoconnect(false);
}

void MainWindow::connectionFailed(const char *status)
{
    connectionClosed(status);
    refreshPortsAutoconnect(false);

    stopLog();
    ui->log->setEnabled(false);
}

void MainWindow::connectionClosed(const char *status)
{
    ui->connectionStatus->setText(status);
    ui->connectionDataRate->hide();
    ui->connectionStatusSeparator->hide();

    for (int i = 1; i < ui->alltabs->count(); ++ i)
        ui->alltabs->setTabEnabled(i, false);

    ui->connect->setText("Connect");

    fingerData.clear();

    stopLog();
    ui->log->setEnabled(false);
}

void MainWindow::connectionOpened(const char *status)
{
    ui->connectionStatus->setText(status);
    ui->connectionDataRate->setText("0 KB/s");
    ui->connectionDataRate->show();
    ui->connectionStatusSeparator->show();

    for (int i = 1; i < ui->alltabs->count(); ++ i)
        ui->alltabs->setTabEnabled(i, true);

    ui->connect->setText("Disconnect");

    // switch to static data
    ui->alltabs->setCurrentIndex(1);

    ui->log->setEnabled(true);
}

void MainWindow::updateConnectionDataRate(unsigned int bs)
{
    ui->connectionDataRate->setText(QString().asprintf("%u.%03u KB/s", bs / 1000, bs % 1000));
}
