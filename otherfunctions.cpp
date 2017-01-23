#include<mainwindow.cpp>

int plusmod(int num1, int num2, int mod)//плюс по модулю
{
    int res=num1+num2;
    if(res>mod)
    {
	res=res-mod;
    }
    return res;
}

int minusmod(int num1, int num2, int mod)//минус по модулю
{
    int res=num1-num2;
    if(res<0)
    {
	res=res+mod;
    }
    return res;
}

void MainWindow::close_window()//анимированное закрытие окна
{
    int i;
    if(opened==true)
	i=sizeY;
    else
	i=msizeY;
    for(; i>0; i-=5)
    {
	setFixedSize(sizeX,i);
	QApplication::processEvents();
	MainWindow::sleep(5);
    }
}

void MainWindow::open_window()//анимирование раскрытие окна
{
    if(opened==false)
    {
	for(int i=msizeY; i<sizeY; i+=5)
	{
	    setFixedSize(sizeX,i);
	    QApplication::processEvents();
	    MainWindow::sleep(1);
	}
	setFixedSize(sizeX,sizeY);
	opened=true;
    }
}

void MainWindow::sleep(int ms)			//таймер=)
{
    QTime t;
    t.start();
    while(t.elapsed()<ms);
}

void MainWindow::on_stop_button_clicked()//остановить кодирование
{
    cont=false;
    ui->progress->setValue(100);
    ui->status_label->setText("Отменено.");
}

void MainWindow::on_code_clicked()
{
    open_window();
    coding=true;
}

void MainWindow::on_decode_clicked()
{
    open_window();
    coding=false;
}

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)//конструктор
{
    ui->setupUi(this);
    opened=false;
    setMouseTracking(true);
    centralWidget()->setMouseTracking(true);
//    QString path=QFileDialog::getExistingDirectory();
//    processFolder(path, path);
}

MainWindow::~MainWindow()			//деструктор
{
    cont=false;
    ui->progress->setValue(100);
    ui->status_label->setText("Отменено.");
    close_window();
    delete ui;
}

void MainWindow::on_selectFile_button_clicked()//Выбор файла
{
    QString path;
    path=QFileDialog::getOpenFileName(0,"Выберите входной файл","","*");
    open_window();
    ui->filename_line->setText(path);
}

