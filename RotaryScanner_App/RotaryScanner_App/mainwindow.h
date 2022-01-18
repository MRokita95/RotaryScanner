#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSerialPort>
#include <QSerialPortInfo>
#include <QtDebug>
#include <iostream>
#include <vector>
#include <string>
#include <QWidget>
#include <QMessageBox>
#include <QQuickWidget>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>
#include <QProcess>
#include <QStringList>
#include <QRadioButton>
#include <QPushButton>

#include "datacollector.h"


#define SingleMesureComm "SM_P"		//Single Measurement Position (...) ex: SM_P 90 (single mesurment at position 90 degrees)
#define MultMesureInRangeComm "MM_IR_P"		//Multiple Measurements In Range Position ex: MM_IR_P 10 90 2 (mesurments from 10 to 90 every 2 degrees)
#define FullMeasureComm "FM_P"		//Full Measurement (360 degrees) ex: FM_P 10 (mesurments from 0 to 360 every 10 degrees)
#define ReqDataComm "GET_DAT"           //Request for data from Arduino (Position + distance)
#define GoHomeComm "HOM"           //Go to 0.0 position (home)

#define write_data_to_file true     //parameter if write data to file are necessary

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    //send prepared command to arduino via opened serial port
    void send_command(QString command);

    //single measurement mode (RadioButton)
    void on_singleModeRB_clicked();

    //ranged mode (from pos, to pos, with step) RadioButton
    void on_rangedModeRB_clicked();

    //full measurement mode (from 0 to 360 with step) RadioButton
    void on_fullModeRB_clicked();

    //Single mode position set value
    void on_setPos_SM_valueChanged(int arg1);

    //connect to arduino start button
    void on_connect_PB_clicked();

    //start selected procedure on arduino (with sending data)
    void on_start_PB_clicked();

    //disconnect to arduino and delete arranged memory
    void on_disconnect_PB_clicked();

    //slot reading data from arduino if available
    void read_data_from_port();

    //Ranged mode start position set value
    void on_setStartPos_RM_valueChanged(int arg1);

    //Ranged mode end position set value
    void on_setEndPos_RM_valueChanged(int arg1);

    //Ranged mode step value
    void on_setStep_RM_valueChanged(int arg1);

    //Full mode step value
    void on_setStep_FM_valueChanged(int arg1);

    //go to 0.0 position
    void on_goHome_PB_clicked();

    //write collected data to the file in selected folder
    void on_write_to_file_PB_clicked();

    //show accesible files
    void on_show_plot_PB_clicked();

    //after selecting file show button that start plotting
    void show_button_to_plot();

    //hide button for plotting
    void hide_button_to_plot();

    //plot data (run python script)
    void plot_data();


private:
    Ui::MainWindow *ui;

    //collection of the widgets that should be hidden at the start
    std::vector<QWidget *> widgets_to_control;

    //hide widgets
    void hide_widgets(std::vector<QWidget *> &);

    //create new during connecting (necessary arduino data for connecting)
    QSerialPort *arduino;
    static const quint16 arduino_uno_vendor_id = 10755;
    static const quint16 arduino_uno_product_id = 67;
    QString arduino_port_name;
    bool arduino_is_available;

    //send data for arduino
    QString command_to_arduino = "";
    int set_startPos;
    int set_endPos;
    int set_step;

    //received (and converted from string) actual arduino data
    double actual_position;
    int measured_distance;

    //request data from arduino (send command)
    void request_data();


    //file name for writting collected data and data collector class pointer membed
    QString file_name = "Pos_Dist_Data";
    DataCollector<typeof(actual_position), typeof(measured_distance)> *data_collection = nullptr;

    //holds history of created files
    std::vector <QString> created_files;
    //dynamic radiobuttons creation for every file created
    QRadioButton **file_RB;
    //draw plot push button (dynamically created)
    QPushButton *draw_plot_PB;

    //check if any file was selected for analysis
    QString check_selected_file();

    //data frame preparation (depends on selected mode)
    QString prepare_command(QString command, int endPos=0, int startPos=0, int step=0);

    //shows created files
    void show_files();

};
#endif // MAINWINDOW_H
