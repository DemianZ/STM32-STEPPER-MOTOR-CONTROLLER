#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QMainWindow>
#include <QtSerialPort/QSerialPort>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class SettingsDialog;
class Stepper;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openSerialPort();
    void closeSerialPort();

    void writeData(const QByteArray &data);
    void readData();

    void handleError(QSerialPort::SerialPortError error);

public slots:
    void slotSetupM1(uint8_t mode,uint8_t dir, uint8_t cont);
    void MW_slotStartButtonPressed(uint32_t);
    void MW_slotStopButtonPressed(void);

signals:
    void signalMouseClicked(void);

private:
    void initActionsConnections();
    void stepperActionsConnections();

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    void showStatusMessage(const QString &message);

    Ui::MainWindow *ui;
    QLabel *status;
    SettingsDialog *settings;
    QSerialPort *serial;
    Stepper *stepper;
};

#endif // MAINWINDOW_H
