#include "stepper.h"
#include "ui_stepper.h"

Stepper::Stepper(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Stepper)
{
    ui->setupUi(this);
    this->mode = 1;
    this->steps = 0;
    this->dir = 0;
    this->cont = 0;

    CmdToSend.fill(0xAA, 8);

    ui->steps_lineEdit->setValidator( new QIntValidator(0, 0xffff, this) );

    connect(ui->start_PB, SIGNAL(clicked()),
            this, SLOT(ST_slotStartButtonPressed()));

    ui->control_groupBox->setDisabled(1);

}

Stepper::~Stepper()
{
    delete ui;
}

void Stepper::slotClearFocus(void)
{
    ui->steps_lineEdit->clearFocus();
}

uint32_t Stepper::getSteps(void)
{
    return this->steps;
}
uint8_t Stepper::getMode(void)
{
    return this->mode;
}
uint8_t Stepper::getDir(void)
{
    return this->dir;
}

QByteArray Stepper::getCmd(void)
{
    return this->CmdToSend;
}

uint8_t Stepper::getCont(void)
{
    return this->cont;
}

void Stepper::setCmdByte(uint8_t i, uint8_t var)
{
    this->CmdToSend[i] = var;
}


void Stepper::ST_slotStartButtonPressed(void)
{
    QString str_cur_steps = ui->steps_lineEdit->text();
    uint32_t ui32_steps = (str_cur_steps.toInt());
    this->steps = ui32_steps;
    emit signalStartButtonPressed(ui32_steps);
}

/* Steps-Continous groupbox slots */
void Stepper::on_cont_Radio_clicked()
{
    ui->steps_lineEdit->setDisabled(1);
    ui->m1_pushButton->setDisabled(0);
    this->cont = 1;
}

void Stepper::on_steps_Radio_clicked()
{
    ui->steps_lineEdit->setDisabled(0);
    ui->m1_pushButton->setDisabled(0);
    this->cont = 0;
}

/* Motor init pushbutton slot */
void Stepper::on_m1_pushButton_clicked()
{
    ui->m1_pushButton->setDisabled(1);
    ui->control_groupBox->setDisabled(0);
    emit signalSetupM1(this->mode, this->dir, this->cont);
}

/* Stop pushbutton slot */
void Stepper::on_stop_PB_clicked()
{
    emit signalStopButtonPressed();
}


/* Mode radiobuttons slots */



void Stepper::on_m_fs_Radio_clicked() {
    if (mode != 1){
        this->mode = 1;
        ui->m1_pushButton->setDisabled(0);
    }
}
void Stepper::on_m_hs_Radio_clicked() {
    if (mode != 2){
    this->mode = 2;
    ui->m1_pushButton->setDisabled(0);
    }
}
void Stepper::on_m_1_4_Radio_clicked() {
    if (mode != 4){
    this->mode = 4;
    ui->m1_pushButton->setDisabled(0);
    }
}
void Stepper::on_m_1_8_Radio_clicked() {
    if (mode != 8){
    this->mode = 8;
    ui->m1_pushButton->setDisabled(0);
    }
}
void Stepper::on_m_1_16_Radio_clicked() {
    if (mode != 16){
    this->mode = 16;
    ui->m1_pushButton->setDisabled(0);
    }
}
void Stepper::on_m_1_32_Radio_clicked() {
    if (mode != 32){
    this->mode = 32;
    ui->m1_pushButton->setDisabled(0);
    }
}
void Stepper::on_m_1_64_Radio_clicked() {
    if (mode != 64){
    this->mode = 64;
    ui->m1_pushButton->setDisabled(0);
    }
}
void Stepper::on_m_1_128_Radio_clicked() {
    if (mode != 128){
    this->mode = 128;
    ui->m1_pushButton->setDisabled(0);
    }
}

void Stepper::on_cw_radioButton_clicked()
{
    this->dir = 0;
    ui->m1_pushButton->setDisabled(0);
}

void Stepper::on_ccw_radioButton_clicked()
{
    this->dir = 1;
    ui->m1_pushButton->setDisabled(0);
}
