#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP1251"));//задаем кодек текста дл€ поддержки русских символов
    //-------------------------------------»нициализаци€ основных переменных
    QApplication a(argc, argv);
    MainWindow w;
    w.sizeX=650;					//задаем размер ’
    w.msizeY=103;//235					//размер свернутый
    w.sizeY=450;					//размер развернутый
    w.WinTitle="FP FileCoder 2.5";			//указываем заговок окна
    w.mark="FPFILECODERCHECKCOMBINATION";		//задаем метку дл€ проверки архива
    w.exist=false;                                      //файла пока нет
    w.argc=argc;					//копируем аргумент в поле класса
    //--------------------------------------------------------------
    if(argc>=2)						//если есть агрументы...
    {
	w.argv=QString::fromAscii(argv[1]);
	if(QFileInfo(w.argv).isDir())			//если папка
        {
	    w.folder=true;//это точно папка
	    w.coding=true;//ее точно нужно кодировать
            w.exist=true;
	}
	else if(QFileInfo(w.argv).isFile())			//если файл
        {
	    w.folder=false;//это точно не папка
            w.exist=true;
	    //проверка кодировать или декодировать
	    QFile tmp(w.argv);	    //открываем выходной файл(проверка по содержимому)
	    tmp.open(QIODevice::ReadOnly);
	    QDataStream str(&tmp);
	    QChar tmp1;
	    w.coding=false;
	    for(int i=0;i<w.mark.length();++i)//проган€ем
		str>>tmp1;
	    str>>tmp1;
	    str>>tmp1;
	    if(tmp1=='|')
		w.coding=false;
	    tmp.close();
	    if(w.argv.mid(w.argv.length()-4,-1)==".ffc" && !w.coding)//проверка по имени файла
		w.coding=false;
	    else
		w.coding=true;
	    if(w.coding)	    //если кодировать
                w.coding=true;
	    else		    //если декодировать
                w.coding=false;
	}
	else
	{
            QMessageBox::critical(0,"ќшибка!","‘айла по адресу, прин€том в аргументе не существует!");
            w.exist=false;
            argc=1;
	}
    }
    w.setWindowTitle(w.WinTitle);			//задать заголовок окна
    w.show();						//открываем окно
    if(argc>1)						//если аргументы есть
    {
	for(int i=1; i<w.sizeY; i+=15)		//красивенько разворачиваем
	{
	    w.resize(w.sizeX,i);
	    QApplication::processEvents();
	    w.sleep(4);
	}
	w.setFixedSize(w.sizeX,w.sizeY);
        w.refreshInterface();
	w.opened=true;
    }
    else						//если аргументов нет
    {
	for(int i=1; i<w.msizeY; i+=6)		//красивенько разворачиваем
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
