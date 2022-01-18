#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "datacollector.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    widgets_to_control.push_back(ui->setPos_SM);
    widgets_to_control.push_back(ui->setStartPos_RM);
    widgets_to_control.push_back(ui->setEndPos_RM);
    widgets_to_control.push_back(ui->setStep_RM);
    widgets_to_control.push_back(ui->setStep_FM);

    widgets_to_control.push_back(ui->start_PB);
    widgets_to_control.push_back(ui->goHome_PB);

    widgets_to_control.push_back(ui->write_to_file_PB);
    widgets_to_control.push_back((ui->show_plot_PB));
    widgets_to_control.push_back(ui->disconnect_PB);

    widgets_to_control.push_back(ui->param1_label);
    widgets_to_control.push_back(ui->param2_label);
    widgets_to_control.push_back(ui->param3_label);

    //init of the arduino status
    arduino_is_available = false;
    arduino_port_name = "";

    hide_widgets(widgets_to_control);

}

MainWindow::~MainWindow()
{
    arduino->close();

    if (data_collection) delete data_collection;
    if (arduino) delete arduino;

    delete ui;
}

void MainWindow::hide_widgets(std::vector<QWidget *> & widgets)
{
    for (auto w: widgets)
    {
        w->hide();
    }

    if (file_RB)
    {
        for (unsigned int i = 0; i<created_files.size(); i++)
        {
            file_RB[i]->hide();
        }

        delete [] file_RB;
    }

}


void MainWindow::on_connect_PB_clicked()
{
    arduino = new QSerialPort;

    qDebug() <<"Number of available ports: "<<QSerialPortInfo::availablePorts().length();

    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if (serialPortInfo.hasVendorIdentifier())
        {
            qDebug() <<"Vendor ID: "<<serialPortInfo.vendorIdentifier();
        }
    }
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if (serialPortInfo.hasProductIdentifier())
        {
            qDebug() <<"Product ID: "<<serialPortInfo.productIdentifier();
        }
    }


    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id)
        {
            if(serialPortInfo.hasProductIdentifier() && serialPortInfo.productIdentifier() == arduino_uno_product_id)
            {
                arduino_port_name = serialPortInfo.portName();
                arduino_is_available = true;
            }
        }
    }

    if (arduino_is_available)
    //open and configure the port
    {
        arduino->setPortName(arduino_port_name);
        arduino->open(QSerialPort::ReadWrite);
        arduino->setBaudRate(9600);
        arduino->setDataBits(QSerialPort::Data8);
        arduino->setParity(QSerialPort::NoParity);
        arduino->setStopBits(QSerialPort::OneStop);
        arduino->setFlowControl(QSerialPort::NoFlowControl);

        //QMessageBox::information(this,"Port Status", "Arduino connected");


        QPalette pal = ui->connect_PB->palette();
        pal.setColor(QPalette::ColorRole::Background, Qt::green);
        ui->connect_PB->setText("CONNECTED");
        ui->connect_PB->setAutoFillBackground(true);
        ui->connect_PB->setPalette(pal);
        ui->connect_PB->show();

        ui->disconnect_PB->show();

        ui->goHome_PB->show();


        connect(arduino, SIGNAL(readyRead()), this, SLOT(read_data_from_port()) );


        request_data();



    }
    else
    {
        QMessageBox::warning(this,"Port error", "Couldn't find the arduino");
    }
}

void MainWindow::request_data()
{
    QString command = prepare_command(ReqDataComm);

    send_command(command);
}


void MainWindow::on_start_PB_clicked()
{
    if (write_data_to_file)
    {
        if (!data_collection)
        {
            data_collection = new DataCollector<typeof(actual_position), typeof(measured_distance)>(file_name);
        }
        else
        {
            delete data_collection;

            data_collection = new DataCollector<typeof(actual_position), typeof(measured_distance)>(file_name);
        }


        ui->write_to_file_PB->show();
    }

    QString command = prepare_command(command_to_arduino, set_endPos, set_startPos, set_step);

    send_command(command);

    for (unsigned int i = 0; i<created_files.size(); i++)
    {
        file_RB[i]->hide();
    }
}


void MainWindow::send_command(QString command)
{
    if (arduino->isWritable())
    {
        arduino->write(command.toStdString().c_str());
        qDebug()<<"Send Value: "<<command;

    }
    else
    {
        qDebug()<<"Arduino is not writable";
    }
}

void MainWindow::read_data_from_port()
{
    QString line;
    QString pos_terminator = ": ";
    QString terminator = "\r";

    while(arduino->canReadLine() and arduino->isReadable())
    {
        line = arduino->readLine();

        int pos = line.lastIndexOf(terminator);
        qDebug() << line.left(pos);

        int pos_index = line.lastIndexOf(pos_terminator);
        QString act_pos = line.left(pos_index);

        line.remove(0, pos_index+2);
        QString rest_message = line;

        int dist_index = rest_message.lastIndexOf(terminator);
        QString last_dist = rest_message.left(dist_index);

        qDebug()<<"POS: "<<act_pos;
        qDebug()<<"DIST: "<<last_dist;

        actual_position = act_pos.toDouble();
        measured_distance = last_dist.toInt();

        ui->position_display->display(actual_position);
        ui->distance_display->display(measured_distance);

        if (data_collection != nullptr)
        {
            data_collection->put_data(actual_position, measured_distance);
        }

    }




}


void MainWindow::on_singleModeRB_clicked()
{
    command_to_arduino = SingleMesureComm;

    ui->setPos_SM ->show();

    ui->setStartPos_RM->hide();
    ui->setEndPos_RM->hide();
    ui->setStep_RM->hide();

    ui->setStep_FM->hide();

    ui->start_PB->hide();

    ui->param1_label->show();
    ui->param2_label->hide();
    ui->param3_label->hide();
    ui->param1_label->setText("Set Pos:");

    //Erase the parameters data at the beggining of the new mode
    set_endPos = 0;
    set_startPos = 0;
    set_step = 0;
}


void MainWindow::on_rangedModeRB_clicked()
{
    command_to_arduino = MultMesureInRangeComm;

    ui->setPos_SM ->hide();

    ui->setStartPos_RM->show();
    ui->setEndPos_RM->show();
    ui->setStep_RM->show();

    ui->setStep_FM->hide();

    ui->start_PB->hide();

    ui->param1_label->show();
    ui->param2_label->show();
    ui->param3_label->show();
    ui->param1_label->setText("Start Pos:");
    ui->param2_label->setText("End Pos:");
    ui->param3_label->setText("Step:");

    //Erase the parameters data at the beggining of the new mode
    set_endPos = 0;
    set_startPos = 0;
    set_step = 0;
}


void MainWindow::on_fullModeRB_clicked()
{
    command_to_arduino = FullMeasureComm;

    ui->setPos_SM ->hide();

    ui->setStartPos_RM->hide();
    ui->setEndPos_RM->hide();
    ui->setStep_RM->hide();

    ui->setStep_FM->show();

    ui->start_PB->hide();

    ui->param1_label->show();
    ui->param2_label->hide();
    ui->param3_label->hide();
    ui->param1_label->setText("Step:");

    //Erase the parameters data at the beggining of the new mode
    set_endPos = 0;
    set_startPos = 0;
    set_step = 0;
}


void MainWindow::on_setPos_SM_valueChanged(int arg1)
{
    set_endPos = arg1;

    ui->start_PB->show();
}

QString MainWindow::prepare_command(QString command, int endPos, int startPos, int step)
{

    if (sizeof(command)>0)
    {
        QString set_comm = command;

        if (set_comm ==SingleMesureComm)
        {
            set_comm += " " + QString::number(endPos) + "\n";
        }

        else if (set_comm ==MultMesureInRangeComm)
        {
            set_comm += " " +   QString::number(startPos) + " " +
                                QString::number(endPos) + " " +
                                QString::number(step) + "\n";
        }

        else if (set_comm ==FullMeasureComm)
        {
            set_comm += " " + QString::number(step) + "\n";
        }

        else
        {
            set_comm += " ";
        }

        return set_comm;
    }
    else
    {
        return "";
    }

}





void MainWindow::on_disconnect_PB_clicked()
{
    arduino->close();

    if (!arduino->isOpen() )
    {
        QPalette pal = ui->connect_PB->palette();
        pal.setColor(QPalette::Background, Qt::black);
        ui->connect_PB->setText("CONNECT");
        ui->connect_PB->setAutoFillBackground(true);
        ui->connect_PB->setPalette(pal);
        ui->connect_PB->show();


        hide_widgets(widgets_to_control);
    }
}


void MainWindow::on_setStartPos_RM_valueChanged(int arg1)
{
    set_startPos = arg1;
}


void MainWindow::on_setEndPos_RM_valueChanged(int arg1)
{
    set_endPos = arg1;

    if (set_step > 0)
    {
        ui->start_PB->show();
    }
}


void MainWindow::on_setStep_RM_valueChanged(int arg1)
{
    set_step = arg1;

    if (set_endPos > 0)
    {
        ui->start_PB->show();
    }
}


void MainWindow::on_setStep_FM_valueChanged(int arg1)
{
    set_step = arg1;
    ui->start_PB->show();
}



void MainWindow::on_goHome_PB_clicked()
{
    QString command = prepare_command(GoHomeComm);

    send_command(command);
}


void MainWindow::on_write_to_file_PB_clicked()
{
    if (write_data_to_file and data_collection != nullptr)
    {
        data_collection->write_to_file("Position", "Distance");

        QMessageBox::information(this,"File info", QString("File created at: %1") .arg(data_collection->get_filename() ), "OK");

        created_files.push_back(data_collection->get_filename());

        delete data_collection;
        data_collection = nullptr;

        ui->show_plot_PB->show();
    }

}


void MainWindow::on_show_plot_PB_clicked()
{
    show_files();
}


void MainWindow::show_files()
{
    int i = 0;

    file_RB = new  QRadioButton* [created_files.size()];



    for (auto fil: created_files)
    {
        if (file_RB != nullptr)
        {
            file_RB[i] = new QRadioButton(this);

            file_RB[i]->setAutoFillBackground(true);
            QPalette pal = file_RB[i]->palette();
            pal.setColor(QPalette::Button, QColor(Qt::yellow));


            file_RB[i]->setStyleSheet("QRadioButton{font-size: 12px;font-family: "
                                        "Arial;color: rgb(255, 255, 255);"
                                        "background-color: rgb(38,56,76);}");

            //file_RB[i]->setAccessibleName(QString("%1%s").arg(fil,i));
            QString text_RB = fil.remove("C:\\QT_Apps\\Datafiles\\");
            file_RB[i]->setText(fil);
            file_RB[i]->setBaseSize(220,30);
            file_RB[i]->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred );
            file_RB[i]->setEnabled(true);
            file_RB[i]->setVisible(true);

            file_RB[i]->setGeometry(860,390+i*30,220,30);
            file_RB[i]->setPalette(pal);
            file_RB[i]->show();

            connect(file_RB[i], SIGNAL(clicked()), SLOT(show_button_to_plot() ) );

            i++;
        }
    }

}

void MainWindow::show_button_to_plot()
{


    draw_plot_PB  = new QPushButton (this);

    draw_plot_PB->setAutoFillBackground(true);
    QPalette pal = draw_plot_PB->palette();
    pal.setColor(QPalette::Button, QColor(Qt::yellow));

    draw_plot_PB->setStyleSheet("QPushButton{font-size: 14px;font-family: "
                                "Arial;color: rgb(255, 255, 255);"
                                "background-color: rgb(38,56,76);}");

    draw_plot_PB->setText("DRAW PLOT");
    draw_plot_PB->setGeometry(860, 180, 220, 80);
    draw_plot_PB->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred );
    draw_plot_PB->setEnabled(true);
    draw_plot_PB->setBaseSize(160,60);
    draw_plot_PB->setPalette(pal);
    draw_plot_PB->show();


    connect(draw_plot_PB, SIGNAL(clicked()), this, SLOT(plot_data()));



}

void MainWindow::hide_button_to_plot()
{
    draw_plot_PB ->hide();
    delete draw_plot_PB;
}

void MainWindow::plot_data()
{
    QString file_to_plot = check_selected_file();

    if (file_to_plot != "")
    {
        qDebug()<<"PLOTTING..."<< file_to_plot;

        QString path = "C:\\Python\\Files_Analize";
        QString  command("python");
        QStringList params { "read_data.py", file_to_plot};

        QProcess *process = new QProcess();
        bool act = process->startDetached(command, params, path);
        process->waitForFinished();
        process->close();

        qDebug()<<"Python: "<<act;

        hide_button_to_plot();
    }

    else
    {
        QMessageBox::warning(this,"File Error", "No File selected");
    }

}

QString MainWindow::check_selected_file()
{
    QString selected_file ="";

    for (unsigned int i = 0; i<created_files.size(); i++)
    {
        if (file_RB[i]->isChecked())
        {
            selected_file = created_files[i];
            break;
        }
    }

    return selected_file;
}
