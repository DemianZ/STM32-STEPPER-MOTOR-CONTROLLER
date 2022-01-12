#ifndef STEPPER_H
#define STEPPER_H

#define SYNC_BYTE 0xAA
#define CW 0
#define CCW 1

#include <QWidget>

namespace Ui {
class Stepper;
}

class Stepper : public QWidget
{
    Q_OBJECT

public:
    explicit Stepper(QWidget *parent = 0);
    ~Stepper();

signals:
    void signalStartButtonPressed(uint32_t);
    void signalStopButtonPressed(void);
    void signalSetupM1(uint8_t mode, uint8_t direction, uint8_t cont);

private slots:
    void slotClearFocus(void);

    void on_cont_Radio_clicked();
    void on_steps_Radio_clicked();

    void on_m1_pushButton_clicked();

    void ST_slotStartButtonPressed(void);
    void on_stop_PB_clicked();

    void on_m_fs_Radio_clicked();
    void on_m_hs_Radio_clicked();
    void on_m_1_4_Radio_clicked();
    void on_m_1_8_Radio_clicked();
    void on_m_1_16_Radio_clicked();
    void on_m_1_32_Radio_clicked();
    void on_m_1_64_Radio_clicked();
    void on_m_1_128_Radio_clicked();

    void on_cw_radioButton_clicked();
    void on_ccw_radioButton_clicked();

public:
    uint32_t getSteps(void);
    uint8_t getMode(void);
    uint8_t getDir(void);
    uint8_t getCont(void);

    QByteArray getCmd(void);
    void setCmdByte(uint8_t i, uint8_t var);

private:
    Ui::Stepper *ui;
    QByteArray CmdToSend;

    uint8_t mode;
    uint8_t cont;
    uint32_t steps;
    uint8_t dir;
};

#endif // STEPPER_H
