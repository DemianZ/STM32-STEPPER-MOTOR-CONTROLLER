#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "stepper.h"
#include "settingsdialog.h"

#include <QtSerialPort/QSerialPort>
#include <QLabel>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    stepper = new Stepper;
    serial = new QSerialPort;
    settings = new SettingsDialog;

    status = new QLabel;

    ui->statusBar->addWidget(status);

    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);

    initActionsConnections();

    stepperActionsConnections();

    connect(serial, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error),
            this, &MainWindow::handleError);

    setCentralWidget(stepper);
    stepper->setDisabled(0);
}

MainWindow::~MainWindow()
{
    delete settings;
    delete ui;
}

void MainWindow::openSerialPort()
{
    SettingsDialog::Settings p = settings->settings();
    serial->setPortName(p.name);
    serial->setBaudRate(p.baudRate);
    serial->setDataBits(p.dataBits);
    serial->setParity(p.parity);
    serial->setStopBits(p.stopBits);
    serial->setFlowControl(p.flowControl);
    if (serial->open(QIODevice::ReadWrite)) {
        ui->actionConnect->setEnabled(false);
        ui->actionDisconnect->setEnabled(true);
        ui->actionConfigure->setEnabled(false);
        showStatusMessage(tr("Connected"));
        stepper->setDisabled(0);
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        showStatusMessage(tr("Open error"));
    }
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    ui->actionConnect->setEnabled(true);
    ui->actionDisconnect->setEnabled(false);
    ui->actionConfigure->setEnabled(true);
    showStatusMessage(tr("Disconnected"));
    stepper->setDisabled(1);
}


void MainWindow::writeData(const QByteArray &data)
{
    serial->write(data);
}

void MainWindow::readData()
{
    QByteArray data = serial->readAll();
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}


void MainWindow::initActionsConnections()
{
    connect(this, SIGNAL(signalMouseClicked()),
                         stepper, SLOT(slotClearFocus()));

    connect(ui->actionConnect, &QAction::triggered,
            this, &MainWindow::openSerialPort);

    connect(ui->actionDisconnect, &QAction::triggered,
            this, &MainWindow::closeSerialPort);

    connect(ui->actionConfigure, &QAction::triggered,
            settings, &SettingsDialog::show);

    connect(this, SIGNAL(signalMouseClicked()),
            stepper, SLOT(slotClearFocus()));
}

void MainWindow::stepperActionsConnections()
{
    connect(serial, SIGNAL(readyRead()),
            this, SLOT(readData()));

    connect(stepper, SIGNAL(signalStartButtonPressed(uint32_t)),
            this, SLOT(MW_slotStartButtonPressed(uint32_t)));

    connect(stepper, SIGNAL(signalStopButtonPressed()),
            this, SLOT(MW_slotStopButtonPressed()));
    connect(stepper, SIGNAL(signalSetupM1(uint8_t,uint8_t,uint8_t)),
            this, SLOT(slotSetupM1(uint8_t,uint8_t,uint8_t)));
}

void MainWindow::showStatusMessage(const QString &message)
{
    status->setText(message);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    emit signalMouseClicked();
}

void MainWindow::MW_slotStartButtonPressed(uint32_t stepvar)
{
    uint8_t xorByte;
    stepper->setCmdByte(0, SYNC_BYTE);
    if (stepper->getCont() == 0)
    {
        stepper->setCmdByte(1, 0x04);

        uint8_t step_0 = (stepvar >> (8*0)) & 0xff;
        uint8_t step_1 = (stepvar >> (8*1)) & 0xff;
        uint8_t step_2 = (stepvar >> (8*2)) & 0xff;
        uint8_t step_3 = (stepvar >> (8*3)) & 0xff;

        stepper->setCmdByte(2, step_0);
        stepper->setCmdByte(3, step_1);
        stepper->setCmdByte(4, step_2);
        stepper->setCmdByte(5, step_3);
        stepper->setCmdByte(6, 0xFF);
        xorByte = 0xAA^0x04^step_0^step_1^step_2^step_3^0xFF;
    }
    else
    {
        stepper->setCmdByte(1, 0x07);
        stepper->setCmdByte(2, 0x01);
        stepper->setCmdByte(3, 0xFF);
        stepper->setCmdByte(4, 0xFF);
        stepper->setCmdByte(5, 0xFF);

        stepper->setCmdByte(6, 0xFF);
        xorByte = 0xAA^0x07^0x01^0xFF^0xFF^0xFF^0xFF;
    }
    stepper->setCmdByte(7, xorByte);
    serial->write(stepper->getCmd());
}

void MainWindow::MW_slotStopButtonPressed(void)
{
    stepper->setCmdByte(0, SYNC_BYTE);
    stepper->setCmdByte(1, 0x07);
    stepper->setCmdByte(2, 0x00);
    stepper->setCmdByte(3, 0xFF);
    stepper->setCmdByte(4, 0xFF);
    stepper->setCmdByte(5, 0xFF);
    stepper->setCmdByte(6, 0xFF);

    uint8_t xorByte = 0xAA^0x07^0x00^0xFF^0xFF^0xFF^0xFF;
    stepper->setCmdByte(7, xorByte);
    serial->write(stepper->getCmd());
}

void MainWindow::slotSetupM1(uint8_t mode,uint8_t dir,uint8_t cont)
{
    stepper->setCmdByte(0, SYNC_BYTE);
    stepper->setCmdByte(1, 0x06);
    stepper->setCmdByte(2, mode);
    stepper->setCmdByte(3, dir);
    stepper->setCmdByte(4, cont);
    stepper->setCmdByte(5, 0xFF);
    stepper->setCmdByte(6, 0xFF);
    uint8_t xorByte = 0xAA^0x06^mode^dir^cont^0xFF^0xFF;
    stepper->setCmdByte(7, xorByte);
    serial->write(stepper->getCmd());
}
