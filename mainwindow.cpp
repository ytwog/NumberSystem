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

// Функция перевода дробной части числа из системы счисления systemN в десятеричную
QString toTenthFloat(QString inp, int systemN)
{
    QString res = "";
    double tempFloat = 0;
    for(int i = 0; i < inp.length(); i++)
    {
        char ch = inp[i].toLatin1();                        // Выделение памяти под символ, который нужно перевести.
        tempFloat += atoi(&ch) * pow(systemN, -(i + 1));    // Перевод числа с учетом последнего символа
    }
    res = QString::number(tempFloat);                       // Перевод числа в строку
    res.remove(0, 2);                                       // Удаление символов '0' и '.'
    return res;
}

// Функция перевода дробной части числа из десятеричной системы счисления в systemN
QString fromTenthFloat(QString inp, int systemN)
{
    QString res = "";
    bool ok = true;
    double tempFloat;
    for(int i = 0; i < 6; i++)
    {
        tempFloat = inp.toInt(&ok);                         // Перевод в double
        if(!ok)
        {
            return "NaN";
        }
        if(tempFloat == 0)                                  // Если перевод завершен раньше, чем была достигнута точность = 6, то
        {                                                   // возвращается результат
            return res;
        }
        tempFloat = tempFloat / pow(10, inp.length());      // Получение из числа, обозначающего дробную часть, но записанного как
        tempFloat *= systemN;                               // целое, дробную часть в виде 0.xxxxxx и вычисление по алгоритму перевода
        int Integer = int(tempFloat);
        res += QString::number(Integer);                    // Добавление к результату полученной цифры (целая часть при умножении)
        inp = QString::number(tempFloat - int(tempFloat));  // Преобразование дробной части в строку для дальнейших вычислений
        if(inp.length() > 2)
        {
            inp.remove(0, 2);                               // При наличии дробной части, удаляются '0' и '.'
        }
        else
        {
            inp = "0";                                      // При отсутствии дробной части, строка для вычислений принимает значение 0
        }
    }
    return res;
}

// Функция перевод целой части числа в десятеричную систему счисления из systemN
int toTenth(int inp, int systemN)
{
    int res = 0;
    int count = 0;
    while(inp)                                              // Пока число не стало равным нулю.
    {
        int temp = inp % 10;
        inp /= 10;
        res += temp * pow(systemN, count);                  // Число переводится по алгоритму
        count++;
    }
    return res;
}

// Функция перевода дробной части числа из десятеричной системы счисления в systemN
QString fromTenth(int inp, int systemN)
{
    QString res = "";
    while(inp)                                              // Пока число не стало равным нулюч
    {
        res = (QString::number(inp % systemN) + res);       // К результату слева приписывается остаток от деления
        inp /= systemN;
    }
    return res;
}

// Функция для перевода числа inp из системы счисления systemIN в систему счисления systemOUT, результат хранится в QString
QString calculate(double inp, int systemIN, int systemOUT)
{
    QString res = "";
    int integer = int(inp);

    QString check = QString::number(inp);
    for(int i = 0; i < check.length(); i++)                 // Проверка на правильность ввода
    {
        char ch = check[i].toLatin1();
        if(ch != '.' && ch != ',')
        {
            if(atoi(&ch) >= systemIN)
            {
                return "Неверно заданы входные данные";
            }
        }
    }

    if(systemIN == systemOUT)                               // При идентичности систем счисления число не меняется
    {
        res += QString::number(integer);
    }
    else                                                    // Перевод целой части
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


    if(res == "")
    {
        res = "0";
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
    if(systemIN == systemOUT)                               // При идентичности систем счисления число не меняется
    {
        res += tempStr;
    }
    else                                                    // Перевод дробной части
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
