#include<mainwindow.cpp>

int plusmod(int num1, int num2, int mod)//���� �� ������
{
    int res=num1+num2;
    if(res>mod)
    {
	res=res-mod;
    }
    return res;
}

int minusmod(int num1, int num2, int mod)//����� �� ������
{
    int res=num1-num2;
    if(res<0)
    {
	res=res+mod;
    }
    return res;
}

void MainWindow::close_window()//������������� �������� ����
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

void MainWindow::open_window()//������������ ��������� ����
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

void MainWindow::sleep(int ms)			//������=)
{
    QTime t;
    t.start();
    while(t.elapsed()<ms);
}

void MainWindow::on_stop_button_clicked()//���������� �����������
{
    cont=false;
    ui->progress->setValue(100);
    ui->status_label->setText("��������.");
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

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)//�����������
{
    ui->setupUi(this);
    opened=false;
    setMouseTracking(true);
    centralWidget()->setMouseTracking(true);
//    QString path=QFileDialog::getExistingDirectory();
//    processFolder(path, path);
}

MainWindow::~MainWindow()			//����������
{
    cont=false;
    ui->progress->setValue(100);
    ui->status_label->setText("��������.");
    close_window();
    delete ui;
}

void MainWindow::on_selectFile_button_clicked()//����� �����
{
    QString path;
    path=QFileDialog::getOpenFileName(0,"�������� ������� ����","","*");
    open_window();
    ui->filename_line->setText(path);
}

