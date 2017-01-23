#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP1251"));//������ ����� ������ ��� ��������� ������� ��������
    //-------------------------------------������������� �������� ����������
    QApplication a(argc, argv);
    MainWindow w;
    w.sizeX=650;					//������ ������ �
    w.msizeY=103;//235					//������ ���������
    w.sizeY=450;					//������ �����������
    w.WinTitle="FP FileCoder 2.5";			//��������� ������� ����
    w.mark="FPFILECODERCHECKCOMBINATION";		//������ ����� ��� �������� ������
    w.exist=false;                                      //����� ���� ���
    w.argc=argc;					//�������� �������� � ���� ������
    //--------------------------------------------------------------
    if(argc>=2)						//���� ���� ���������...
    {
	w.argv=QString::fromAscii(argv[1]);
	if(QFileInfo(w.argv).isDir())			//���� �����
        {
	    w.folder=true;//��� ����� �����
	    w.coding=true;//�� ����� ����� ����������
            w.exist=true;
	}
	else if(QFileInfo(w.argv).isFile())			//���� ����
        {
	    w.folder=false;//��� ����� �� �����
            w.exist=true;
	    //�������� ���������� ��� ������������
	    QFile tmp(w.argv);	    //��������� �������� ����(�������� �� �����������)
	    tmp.open(QIODevice::ReadOnly);
	    QDataStream str(&tmp);
	    QChar tmp1;
	    w.coding=false;
	    for(int i=0;i<w.mark.length();++i)//���������
		str>>tmp1;
	    str>>tmp1;
	    str>>tmp1;
	    if(tmp1=='|')
		w.coding=false;
	    tmp.close();
	    if(w.argv.mid(w.argv.length()-4,-1)==".ffc" && !w.coding)//�������� �� ����� �����
		w.coding=false;
	    else
		w.coding=true;
	    if(w.coding)	    //���� ����������
                w.coding=true;
	    else		    //���� ������������
                w.coding=false;
	}
	else
	{
            QMessageBox::critical(0,"������!","����� �� ������, �������� � ��������� �� ����������!");
            w.exist=false;
            argc=1;
	}
    }
    w.setWindowTitle(w.WinTitle);			//������ ��������� ����
    w.show();						//��������� ����
    if(argc>1)						//���� ��������� ����
    {
	for(int i=1; i<w.sizeY; i+=15)		//����������� �������������
	{
	    w.resize(w.sizeX,i);
	    QApplication::processEvents();
	    w.sleep(4);
	}
	w.setFixedSize(w.sizeX,w.sizeY);
        w.refreshInterface();
	w.opened=true;
    }
    else						//���� ���������� ���
    {
	for(int i=1; i<w.msizeY; i+=6)		//����������� �������������
	{
	    w.resize(w.sizeX,i);
	    QApplication::processEvents();
	    w.sleep(4);
	}
	w.setFixedSize(w.sizeX,w.msizeY);
    }
    w.OnStart();
    return a.exec();
}
