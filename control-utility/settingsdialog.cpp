#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QtSerialPort/QSerialPortInfo>
#include <QIntValidator>
#include <QLineEdit>


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);


    connect(ui->applyButton, &QPushButton::clicked,
            this, &SettingsDialog::apply);

    fillPortsParameters();
    fillPortsInfo();

    updateSettings();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

SettingsDialog::Settings SettingsDialog::settings() const
{
    return currentSettings;
}

void SettingsDialog::apply()
{
    updateSettings();
    hide();
}

void SettingsDialog::fillPortsInfo()
{
    ui->portCombo->clear();
    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos) {
        QStringList list;
        list << info.portName();
        ui->portCombo->addItem(list.first(), list);
    }
}

void SettingsDialog::fillPortsParameters()
{
    ui->baudRateCombo->addItem(QStringLiteral("9600"), QSerialPort::Baud9600);
    ui->baudRateCombo->addItem(QStringLiteral("19200"), QSerialPort::Baud19200);
    ui->baudRateCombo->addItem(QStringLiteral("38400"), QSerialPort::Baud38400);
    ui->baudRateCombo->addItem(QStringLiteral("115200"), QSerialPort::Baud115200);

    ui->dataBitsCombo->addItem(QStringLiteral("5"), QSerialPort::Data5);
    ui->dataBitsCombo->addItem(QStringLiteral("6"), QSerialPort::Data6);
    ui->dataBitsCombo->addItem(QStringLiteral("7"), QSerialPort::Data7);
    ui->dataBitsCombo->addItem(QStringLiteral("8"), QSerialPort::Data8);
    ui->dataBitsCombo->setCurrentIndex(3);

    ui->parityCombo->addItem(tr("None"), QSerialPort::NoParity);
    ui->parityCombo->addItem(tr("Even"), QSerialPort::EvenParity);
    ui->parityCombo->addItem(tr("Odd"), QSerialPort::OddParity);
    ui->parityCombo->addItem(tr("Mark"), QSerialPort::MarkParity);
    ui->parityCombo->addItem(tr("Space"), QSerialPort::SpaceParity);

    ui->stopBitsCombo->addItem(QStringLiteral("1"), QSerialPort::OneStop);
    ui->stopBitsCombo->addItem(QStringLiteral("2"), QSerialPort::TwoStop);

    ui->flowCombo->addItem(tr("None"), QSerialPort::NoFlowControl);
    ui->flowCombo->addItem(tr("RTS/CTS"), QSerialPort::HardwareControl);
    ui->flowCombo->addItem(tr("XON/XOFF"), QSerialPort::SoftwareControl);
}

void SettingsDialog::updateSettings()
{
    currentSettings.name = ui->portCombo->currentText();

    if (ui->baudRateCombo->currentIndex() == 4) {
        currentSettings.baudRate = ui->baudRateCombo->currentText().toInt();
    } else {
        currentSettings.baudRate = static_cast<QSerialPort::BaudRate>(
                    ui->baudRateCombo->itemData(ui->baudRateCombo->currentIndex()).toInt());
    }
    currentSettings.stringBaudRate = QString::number(currentSettings.baudRate);

    currentSettings.dataBits = static_cast<QSerialPort::DataBits>(
                ui->dataBitsCombo->itemData(ui->dataBitsCombo->currentIndex()).toInt());
    currentSettings.stringDataBits = ui->dataBitsCombo->currentText();

    currentSettings.parity = static_cast<QSerialPort::Parity>(
                ui->parityCombo->itemData(ui->parityCombo->currentIndex()).toInt());
    currentSettings.stringParity = ui->parityCombo->currentText();

    currentSettings.stopBits = static_cast<QSerialPort::StopBits>(
                ui->stopBitsCombo->itemData(ui->stopBitsCombo->currentIndex()).toInt());
    currentSettings.stringStopBits = ui->stopBitsCombo->currentText();

    currentSettings.flowControl = static_cast<QSerialPort::FlowControl>(
                ui->flowCombo->itemData(ui->flowCombo->currentIndex()).toInt());
    currentSettings.stringFlowControl = ui->flowCombo->currentText();

}
