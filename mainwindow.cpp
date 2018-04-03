#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString toTenthFloat(QString inp, int systemN)
{
    QString res = "";
    double tempFloat = 0;
    for(int i = 0; i < inp.length(); i++)
    {
        char ch = inp[i].toLatin1();
        tempFloat += atoi(&ch) * pow(systemN, -(i + 1));
    }
    res = QString::number(tempFloat);
    res.remove(0, 2);
    return res;
}

QString fromTenthFloat(QString inp, int systemN)
{
    QString res = "";
    bool ok = true;
    double tempFloat;
    for(int i = 0; i < 6; i++)
    {
        tempFloat = inp.toInt(&ok);
        if(!ok)
        {
            return "NaN";
        }
        if(tempFloat == 0)
        {
            return res;
        }
        tempFloat = tempFloat / pow(10, inp.length());
        tempFloat *= systemN;
        int Integer = int(tempFloat);
        res += QString::number(Integer);
        inp = QString::number(tempFloat - int(tempFloat));
        if(inp.length() > 2)
        {
            inp.remove(0, 2);
        }
        else
        {
            inp = "0";
        }
    }
    return res;
}

int toTenth(int inp, int systemN)
{
    int res = 0;
    int count = 0;
    while(inp)
    {
        int temp = inp % 10;
        inp /= 10;
        res += temp * pow(systemN, count);
        count++;
    }
    return res;
}

QString fromTenth(int inp, int systemN)
{
    QString res = "";
    while(inp)
    {
        res = (QString::number(inp % systemN) + res);
        inp /= systemN;
    }
    return res;
}

QString calculate(double inp, int systemIN, int systemOUT)
{
    QString res = "";
    int integer = int(inp);

    if(systemIN == systemOUT)
    {
        res += QString::number(integer);
    }
    else
    {
        int tempInteger = integer;
        if(systemIN != 10)
        {
            tempInteger = toTenth(integer, systemIN);
            res = QString::number(tempInteger);
        }

        if(systemOUT != 10)
        {
            res = fromTenth(tempInteger, systemOUT);
        }
    }

    double longFloatTemp = inp - int(inp);
    QString tempStr = "";
    tempStr += QString::number(longFloatTemp);
    if(tempStr.length() > 2)
    {
        tempStr.remove(0, 2);
    }
    else
    {
        tempStr = "";
    }
    if(tempStr == "")
    {
        return res;
    }
    res += ".";
    if(systemIN == systemOUT)
    {
        res += tempStr;
    }
    else
    {
        if(systemIN != 10)
        {
            tempStr = toTenthFloat(tempStr, systemIN);
        }
        if(systemOUT != 10)
        {
            tempStr = fromTenthFloat(tempStr, systemOUT);
        }
        res += tempStr;
    }
    return res;
}

void MainWindow::on_calculateButton_clicked()
{
    int systemIN = ui->NSin->text().toInt();
    int systemOUT = ui->NSout->text().toInt();
    double inp = ui->input->value();

    ui->output->setText(calculate(inp, systemIN, systemOUT));
    //ui->output->setText(QString::number(inp));
}
