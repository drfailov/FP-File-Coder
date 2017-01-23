#include "mainwindow.h"
#include <windows.h>
#include "ui_mainwindow.h"
#include <QtGui>
#include <QFlags>
#include <QWaitCondition>

int plusmod(int num1, int num2, int mod)//���� �� ������
{
    int res=num1+num2;
    if(res>mod)
    {
	res=res-mod;
    }
    return res;
}

bool isPath(QChar ch)
{
    QString allowed="1234567890qwertyuiop[]asdfg/\\ hjkl;!�'zxcvbnm,.`!@#$%^&()_+=-}{POIUYTREWQASDFGHJKLMNBVCXZ�������������������������������ި�������������������������������������";
    bool ok=false;
    for(int i=0;i<allowed.length();++i)
	if(allowed[i]==ch)
	    ok=true;
    return ok;
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

void setAttrib(QString path,QString attrib)//"+++" (system)(hidden)(readOnly)
{
    WCHAR *m=new WCHAR[path.length()+3];
    path.toWCharArray(m);//������������ ������
    m[path.length()]='\0';//�������� ����� ������ �������
    DWORD attr;
    SetFileAttributes(m,attr | FILE_ATTRIBUTE_NORMAL);//������ ���������� ���������� ����
    attr=GetFileAttributes(m);//��������� ��������
    if(attrib[0]=='+')//system +
	    SetFileAttributes(m,attr | FILE_ATTRIBUTE_SYSTEM);
    attr=GetFileAttributes(m);//��������� ��������
    if(attrib[1]=='+')//hidden +
	    SetFileAttributes(m,attr | FILE_ATTRIBUTE_HIDDEN);
    attr=GetFileAttributes(m);//��������� ��������
    if(attrib[2]=='+')//readonly +
	    SetFileAttributes(m,attr | FILE_ATTRIBUTE_READONLY);
    delete []m;  //������� ������

}

QString getAttrib(QString path)//"+++" (system)(hidden)(readOnly)
{
    QString result;
    WCHAR *m=new WCHAR[path.length()+3];
    path.toWCharArray(m);//������������ �������
    m[path.length()]='\0';//�������� ����� ������ �������
    DWORD attr;
    attr=GetFileAttributes(m);//��������� ��������
    if (attr & FILE_ATTRIBUTE_SYSTEM) result.append('+');   //�������� �� �����������
    else			      result.append('-');
    if (attr & FILE_ATTRIBUTE_HIDDEN) result.append('+');   //�������� �� ���������
    else			      result.append('-');
    if (attr & FILE_ATTRIBUTE_READONLY) result.append('+');   //�������� �� ����������������
    else			        result.append('-');
    delete []m;  //������� ������
    return result;  //����������� ����������
}

void MainWindow::close_window()//������������� �������� ����
{
    int i;
    if(opened==true)
	i=sizeY;
    else
	i=msizeY;
    for(; i>0; i-=9)
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
	for(int i=msizeY; i<sizeY; i+=15)
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
    refreshInterface();
    ui->selectFolder_button->setFocus();
}

void MainWindow::on_decode_clicked()
{
    open_window();
    coding=false;
    refreshInterface();
    ui->selectFile_button->setFocus();
}

void MainWindow::on_password_line_returnPressed()
{
    if(ui->start_button->isEnabled())//�������� ������� ��
        on_start_button_clicked();
}

void MainWindow::OnStart()
{
    if(argc>1)
    {
        ui->password_line->setFocus();
    }
    else
    {
        ui->code->setFocus();
    }
}

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)//�����������
{
    ui->setupUi(this);
    opened=false;
    cont=true;
    setMouseTracking(true);
    centralWidget()->setMouseTracking(true);
    ui->start_button->setEnabled(exist);
    ui->stop_button->setEnabled(false);
    //ui->copyrigth->setStyleSheet("QLabel{color:white;}");
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
    if(coding==true)
        path=QFileDialog::getOpenFileName(0,"�������� ������� ����","","*");
    if(coding==false)
        path=QFileDialog::getOpenFileName(0,"�������� ������� ����","","*.ffc");
    open_window();
    ui->filename_line->setText(path);
    on_filename_line_editingFinished();
    ui->password_line->setFocus();
}

void MainWindow::on_selectFolder_button_clicked()//����� �����
{
    QString path;
    path=QFileDialog::getExistingDirectory(0,"�������� ������� �����","");
    open_window();
    ui->filename_line->setText(path);
    on_filename_line_editingFinished();
    ui->password_line->setFocus();
}

void MainWindow::refreshInterface()
{
    if(argc>1)
	ui->filename_line->setText(argv);
    if(exist==true)
    {
        if(folder==true)
            ui->file_info_label->setText("�����");
        if(folder==false)
            ui->file_info_label->setText("����");
    }
    else
    {
        ui->file_info_label->setText("������� �� ����������");
    }
    if(coding==true)
    {
	ui->code->setChecked(true);
	ui->selectFolder_button->setEnabled(true);
	ui->start_button->setEnabled(exist);
    }
    if(coding==false)
    {
	ui->decode->setChecked(true);
	ui->selectFolder_button->setEnabled(false);
	ui->start_button->setEnabled(exist && !folder);
    }
}

void MainWindow::on_filename_line_editingFinished()
{
    QString path=ui->filename_line->text();
    if(QFileInfo(path).isFile())//���� ����
    {
        folder=false;
	exist=true;
	refreshInterface();
    }
    else if(QFileInfo(path).isDir())//���� �����
    {
        folder=true;
	exist=true;
	refreshInterface();
    }
    else                            //���� ������ �� �������
    {
	exist=false;
	refreshInterface();
    }
}

void MainWindow::fileCallback(QString path, QString arg2, QString password)
{
    QString newPath=path.mid(arg2.length(),-1);
    codeFile(path,password,newPath);
}

void MainWindow::processFolder(QString path, QString arg2, QString password)
{
    QDir dir(path);
    QStringList list;
    QStringList filter;
    filter<<"*";//<<QDir::System;
    list=dir.entryList(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
    if(list.count()==0)
    {
        QFile inF;			    //��������� ���� ��� write
	inF.setFileName(path+"\\thumbs.db");
        inF.open(QIODevice::WriteOnly);
        in.setDevice(&inF);
        in<<"t";
        inF.close();
        setAttrib(path+"\\thumbs.db","+++");
        list=dir.entryList(QDir::AllEntries | QDir::Hidden | QDir::System | QDir::NoDotAndDotDot);
    }
    for(int i=0;i<list.count() && cont;++i)
    {
        if(QFileInfo(path+"\\"+list[i]).isDir())
            processFolder(path+"\\"+list[i], arg2, password);
        else
            fileCallback(path+"\\"+list[i], arg2, password);
    }
}

bool MainWindow::checkArchive(QString path, QString password)//�������� �������� �� ���� �������(�� �����)
{
    QFile file(path);	    //��������� ������� ����
    file.open(QIODevice::ReadOnly);
    QDataStream str(&file);
    QChar ch;
    bool ok=true;
    for(int i=0;i<mark.length();++i)//���������
    {
        str>>ch;
        QChar no=plusmod(mark[i].unicode(),password[i%password.length()].unicode(),65536);
        if(no != ch)
            ok=false;
    }
    file.close();
    return ok;
}

void MainWindow::codeFile(QString InPath, QString password, QString RelPath)
{
    QFile inF(InPath);		    //��������� ���� ��� ������
    inF.open(QIODevice::ReadOnly);
    in.setDevice(&inF);
    out<<'|';			    //������ �����������
    QString size;		    //������ ������� �����
    size=QString::number(inF.size());
    for(int i=0;i<size.length();i++)
	out<<size[i];
    out<<'|';			    //������ �����������
    for(int i=0;i<RelPath.length();i++)//������ �������������� ������
	out<<RelPath[i];
    out<<'|';			    //������ �����������
    QString atrrib=getAttrib(InPath);
    for(int i=0;i<3;i++)//������ ���������(����� ������ 3)
	out<<atrrib[i];
    QChar ch;			    //������ �� �����
    int len=password.length();	    //����� ������
    cont=true;			    //������� �����������
    uint cur=0;			    //������� �������
    int counter=0;		    //������� ��� ������������ �������
    ui->progress->setValue(0);	    //������ �������� 0
    ui->status_label->setText(RelPath);//��������� ���� �������
    while(!in.atEnd() && cont)	    //������� �����������
    {
        in>>ch;//��������� � ������
	ch=plusmod(ch.unicode(),password[cur%len].unicode(),65536);//�������
	if(counter==10000)	    //������������� ��������
        {
            ui->progress->setValue(200*(double)cur/inF.size());
            QApplication::processEvents();
            counter=0;
        }
	out<<ch;		    //������ ������� � �����
	cur++;			    //����� �������
	counter++;		    //������������� �������
    }
    inF.close();		    //��������� ���� ��� ������
}

bool MainWindow::mkDir(QString path)
{
    bool ok=true;
    for(int i=0;i<path.length();++i)
    {
	if(path[i]=='\\' || i==path.length()-1)
	{
	    QString cur=path.mid(0,i+1);
	    if(!QFileInfo(cur).isDir())
	    {
		ok=QDir().mkdir(cur);
	    }
	}
    }
    return ok;
}

void MainWindow::decodeFile(QString destination, QString InPath, QString password)
{
    QFile inF;			    //��������� ���� ��� ������
    inF.setFileName(InPath);
    inF.open(QIODevice::ReadOnly);
    in.setDevice(&inF);
    QChar ch;			    //"���������" ����������� �����
    for(int i=0;i<mark.length();++i)
	in>>ch;
    while(!inF.atEnd() && cont)
    {
	in>>ch;
	while(ch!='|')
	{
	    in>>ch;
	}
	QString sz;			    //��������� ������ �����
	in>>ch;
	while(ch!='|')
	{
	    if(ch.isNumber()) sz.append(ch);
	    in>>ch;
	}
	bool ok;
	long int size=sz.toInt(&ok,10);
	if(!ok)		//���� �� ������� �������� ������
	{
	    QMessageBox::critical(0,"������!", "�� ������� �������� ������ ����� - �������� ������ ���������. ��������� �� ����� ���������� ����������.");
	    cont=false;
	}
	if(size%2==1) size++;//������ � ����� ������ ������� ����. ���� ���� ������ ������ - �� "������������".
	QString Rpath;			    //��������� ���� �����
	in>>ch;
	while(ch!='|')
	{
	    Rpath.append(ch);
	    in>>ch;
	}
	if(Rpath==".")
	    Rpath="\\";
	QString Apath=destination;          //������������ ���������� � ����
	Apath.append(Rpath);
	QString Adir=Apath;
	for(int i=Adir.length()-1;i>=0 && Adir[i]!='\\' && Adir[i]!='/';--i)Adir[i]='\0';//�������� ��� ����� � ���� ����� �������� ����� �����
	ok=true;
	if(!QFileInfo(Adir).isDir())//���� ����� ��� - ������� ��
	    ok=mkDir(Adir);
	if(!ok)			    //���� ������� ����� �� �������
	{
	    QMessageBox::critical(0,"������!", "�� ������� ������� ����� "+Adir);
	    cont=false;
	}
	else
	{
	    ui->status_label->setText(Rpath);//��������� ���� �������
	    QFile outF(Apath);		//��������� ���� ��� ������
	    outF.open(QIODevice::WriteOnly);
	    out.setDevice(&outF);
	    QChar ch;			//������ ����������� �� �����
	    QString attrib;		//���������� ���������
	    for(int i=0;i<3;i++)
	    {
		in>>ch;
		attrib.append(ch);
	    }
	    int counter=0;		//������� ��� ������������� ��������
            for(uint i=0;i<(size/2) && cont && !inF.atEnd();++i)
	    {
		if(counter==10000)	    //������������� ��������
		{
		    ui->progress->setValue(200*(double)i/size);
		    QApplication::processEvents();
		    counter=0;
		}
		in>>ch;
		ch=minusmod(ch.unicode(),password[i%password.length()].unicode(),65536);
		out<<ch;
		counter++;
	    }
	    outF.close();
	    setAttrib(Apath,attrib);
	}//�� ������� ������� �����
    }//���� �� ����� �������� �����
}

void MainWindow::on_start_button_clicked()//������ �����������
{
    cont=true;//������ ������� �����������
    QString InPath=ui->filename_line->text();
    QString password=ui->password_line->text();
    if(coding==true)//���� ����� ����������
    {
        if(InPath==NULL)					    //���� ��� ��������� �����
	    QMessageBox::critical(0,"������!", "�������� ������� ����!");
        else if(!QFile::exists(InPath))			    //���� ������� ���� �� ����������
	    QMessageBox::critical(0,"������!", "������� ���� �� ����������");
        else if(password.length()==0)			    //���� ��� ������
	    QMessageBox::critical(0,"������!", "������� ������!");
        else
        {
            QString browse_path=InPath;
            for(int i=browse_path.length()-1;i>=0 && browse_path[i]!='\\' && browse_path[i]!='/';--i)browse_path[i]='\0';//�������� ��� ����� � ���� ����� �������� ����� �����
	    QString OutPath=QFileDialog::getSaveFileName(0,"�������� �������� ����",browse_path,"*.ffc");
            if(OutPath==NULL)				    //���� ��� ��������� �����
		QMessageBox::critical(0,"������!", "�������� �������� ����!");
            else if(OutPath==InPath)			    //���� ������� � �������� ���� ���� � ��� ��
		QMessageBox::critical(0,"������!", "�����! ������� � �������� ����� ���������!");
            else
            {
                ui->status_label->setText("����������� ������.");//������ � �����������
                ui->decode->setEnabled(false);
                ui->code->setEnabled(false);
                ui->selectFile_button->setEnabled(false);
                ui->selectFolder_button->setEnabled(false);
                ui->start_button->setEnabled(false);
                ui->stop_button->setEnabled(true);
		QFile outF;			    //��������� ���� ��� ������
		outF.setFileName(OutPath);
		outF.open(QIODevice::WriteOnly);
		out.setDevice(&outF);
                for(int i=0;i<mark.length();++i)    //��������� �����, ������������� �������
                {
                    QChar ch=plusmod(mark[i].unicode(),password[i%password.length()].unicode(),65536);
                    out<<ch;
                }
                if(folder==true)                    //���� ���� ���� � �����
                {
		    processFolder(InPath, InPath, password);
                }
                else                                //���� ���� ���� � �����
		{
		    int i=InPath.length()-1;
		    for(;i>=0 && InPath[i]!='\\' && InPath[i]!='/';--i);//������� ����� ���� �����
		    QString filename=InPath.mid(i,-1);		//��������� ��� �����
		    codeFile(InPath, password,filename);
                }
		outF.close();			    //��������� ���� ��� ������
                ui->stop_button->setEnabled(false);//������ � �����������
                ui->decode->setEnabled(true);
                ui->code->setEnabled(true);
                ui->selectFile_button->setEnabled(true);
                ui->selectFolder_button->setEnabled(true);
                ui->start_button->setEnabled(true);
	    }
            ui->progress->setValue(100);
            ui->status_label->setText("����������� ���������.");
        }
    }
    if(coding==false)//���� ����� ������������
    {
	if(InPath==NULL)					    //���� ��� ��������� �����
	    QMessageBox::critical(0,"������!", "�������� ������� ����!");
	else if(!QFile::exists(InPath))			    //���� ������� ���� �� ����������
	    QMessageBox::critical(0,"������!", "������� ���� �� ����������");
	else if(password.length()==0)			    //���� ��� ������
	    QMessageBox::critical(0,"������!", "������� ������!");
	else
	{
	    if(!checkArchive(InPath,password))		    //���� �� �������� �������� �����
            {
		QMessageBox::critical(0,"������!", "������ �� ������, ���� �� ������� �����-�� ����������� ����!");
                ui->password_line->setFocus();
            }
	    else
	    {
		QString browse_path=InPath;
		for(int i=browse_path.length()-1;i>=0 && browse_path[i]!='\\' && browse_path[i]!='/';--i)browse_path[i]='\0';//�������� ��� ����� � ���� ����� �������� ����� �����
		QString destination=QFileDialog::getExistingDirectory(0,"�������� ����� ��� ����������",browse_path);
		if(destination==NULL)
		    QMessageBox::critical(0,"������!", "�������� ����� �� �������!");
		else
                {
                    ui->status_label->setText("����������� ������.");//������ � �����������
                    ui->decode->setEnabled(false);
                    ui->code->setEnabled(false);
                    ui->selectFile_button->setEnabled(false);
                    ui->selectFolder_button->setEnabled(false);
                    ui->start_button->setEnabled(false);
                    ui->stop_button->setEnabled(true);
                    decodeFile(destination, InPath, password);
                    ui->decode->setEnabled(true);       //������ � �����������
                    ui->code->setEnabled(true);
                    ui->selectFile_button->setEnabled(true);
                    ui->selectFolder_button->setEnabled(false);
                    ui->start_button->setEnabled(true);
                    ui->stop_button->setEnabled(false);
		    ui->progress->setValue(100);
		    ui->status_label->setText("����������� ���������.");
		}
            }//������ �� ������
        }//��� ����� ��� ������
    }//�������������
}

void MainWindow::on_exit_clicked()
{
    QApplication::exit();
}

void MainWindow::on_about_clicked()
{
    QMessageBox about;
    about.setIconPixmap(QPixmap(":/images/logo.png"));
    about.setWindowTitle("� ���������");
    about.setInformativeText("<h2>"+WinTitle+"</h2>��������� ������������� ��� ��������� ����������� ������.<br><br>�����������:<br> ������ �����, ������� ������ ��-11, 1 ����� ���� \"���\" 2011-2012 ����.<br><br>���� 2011");
    about.exec();
}
