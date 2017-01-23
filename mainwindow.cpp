#include "mainwindow.h"
#include <windows.h>
#include "ui_mainwindow.h"
#include <QtGui>
#include <QFlags>
#include <QWaitCondition>

int plusmod(int num1, int num2, int mod)//плюс по модулю
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
    QString allowed="1234567890qwertyuiop[]asdfg/\\ hjkl;!№'zxcvbnm,.`!@#$%^&()_+=-}{POIUYTREWQASDFGHJKLMNBVCXZЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮЁёйцукенгшщзхъфывапролджэячсмитьбюЇїєЄ";
    bool ok=false;
    for(int i=0;i<allowed.length();++i)
	if(allowed[i]==ch)
	    ok=true;
    return ok;
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

void setAttrib(QString path,QString attrib)//"+++" (system)(hidden)(readOnly)
{
    WCHAR *m=new WCHAR[path.length()+3];
    path.toWCharArray(m);//конвертируем строку
    m[path.length()]='\0';//обрезаем чтобы ничего лишнего
    DWORD attr;
    SetFileAttributes(m,attr | FILE_ATTRIBUTE_NORMAL);//задаем изначально нормальный файл
    attr=GetFileAttributes(m);//считываем атрибуты
    if(attrib[0]=='+')//system +
	    SetFileAttributes(m,attr | FILE_ATTRIBUTE_SYSTEM);
    attr=GetFileAttributes(m);//считываем атрибуты
    if(attrib[1]=='+')//hidden +
	    SetFileAttributes(m,attr | FILE_ATTRIBUTE_HIDDEN);
    attr=GetFileAttributes(m);//считываем атрибуты
    if(attrib[2]=='+')//readonly +
	    SetFileAttributes(m,attr | FILE_ATTRIBUTE_READONLY);
    delete []m;  //очистка памяти

}

QString getAttrib(QString path)//"+++" (system)(hidden)(readOnly)
{
    QString result;
    WCHAR *m=new WCHAR[path.length()+3];
    path.toWCharArray(m);//конвертируем строкку
    m[path.length()]='\0';//обрезаем чтобы ничего лишнего
    DWORD attr;
    attr=GetFileAttributes(m);//считываем атрибуты
    if (attr & FILE_ATTRIBUTE_SYSTEM) result.append('+');   //проверка на системность
    else			      result.append('-');
    if (attr & FILE_ATTRIBUTE_HIDDEN) result.append('+');   //проверка на скрытость
    else			      result.append('-');
    if (attr & FILE_ATTRIBUTE_READONLY) result.append('+');   //проверка на толькочитаемость
    else			        result.append('-');
    delete []m;  //очистка памяти
    return result;  //возвращение результата
}

void MainWindow::close_window()//анимированное закрытие окна
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

void MainWindow::open_window()//анимирование раскрытие окна
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
    if(ui->start_button->isEnabled())//проверка включен ли
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

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)//конструктор
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
    if(coding==true)
        path=QFileDialog::getOpenFileName(0,"Выберите входной файл","","*");
    if(coding==false)
        path=QFileDialog::getOpenFileName(0,"Выберите входной файл","","*.ffc");
    open_window();
    ui->filename_line->setText(path);
    on_filename_line_editingFinished();
    ui->password_line->setFocus();
}

void MainWindow::on_selectFolder_button_clicked()//выбор папки
{
    QString path;
    path=QFileDialog::getExistingDirectory(0,"Выберите входную папку","");
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
            ui->file_info_label->setText("Папка");
        if(folder==false)
            ui->file_info_label->setText("Файл");
    }
    else
    {
        ui->file_info_label->setText("Объекта не существует");
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
    if(QFileInfo(path).isFile())//если файл
    {
        folder=false;
	exist=true;
	refreshInterface();
    }
    else if(QFileInfo(path).isDir())//если папка
    {
        folder=true;
	exist=true;
	refreshInterface();
    }
    else                            //если вообще не найдено
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
        QFile inF;			    //открываем файл для write
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

bool MainWindow::checkArchive(QString path, QString password)//проверка является ли файл архивом(по метке)
{
    QFile file(path);	    //открываем входной файл
    file.open(QIODevice::ReadOnly);
    QDataStream str(&file);
    QChar ch;
    bool ok=true;
    for(int i=0;i<mark.length();++i)//проверяем
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
    QFile inF(InPath);		    //открываем файл для чтения
    inF.open(QIODevice::ReadOnly);
    in.setDevice(&inF);
    out<<'|';			    //запись разделителя
    QString size;		    //запись размера файла
    size=QString::number(inF.size());
    for(int i=0;i<size.length();i++)
	out<<size[i];
    out<<'|';			    //запись разделителя
    for(int i=0;i<RelPath.length();i++)//запись относительного адреса
	out<<RelPath[i];
    out<<'|';			    //запись разделителя
    QString atrrib=getAttrib(InPath);
    for(int i=0;i<3;i++)//запись атрибутов(длина ТОЛЬКО 3)
	out<<atrrib[i];
    QChar ch;			    //символ из файла
    int len=password.length();	    //длина пароля
    cont=true;			    //условие продолжения
    uint cur=0;			    //текущая позиция
    int counter=0;		    //счетчик для непостоянных событий
    ui->progress->setValue(0);	    //задаем значение 0
    ui->status_label->setText(RelPath);//обновляем поле статуса
    while(!in.atEnd() && cont)	    //процесс кодирования
    {
        in>>ch;//считываем с потока
	ch=plusmod(ch.unicode(),password[cur%len].unicode(),65536);//шифруем
	if(counter==10000)	    //периодические действия
        {
            ui->progress->setValue(200*(double)cur/inF.size());
            QApplication::processEvents();
            counter=0;
        }
	out<<ch;		    //кидаем обратно в поток
	cur++;			    //общий счетчик
	counter++;		    //периодический счетчик
    }
    inF.close();		    //закрываем файл для чтения
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
    QFile inF;			    //открываем файл для чтения
    inF.setFileName(InPath);
    inF.open(QIODevice::ReadOnly);
    in.setDevice(&inF);
    QChar ch;			    //"проганяем" проверочную часть
    for(int i=0;i<mark.length();++i)
	in>>ch;
    while(!inF.atEnd() && cont)
    {
	in>>ch;
	while(ch!='|')
	{
	    in>>ch;
	}
	QString sz;			    //считываем размер файла
	in>>ch;
	while(ch!='|')
	{
	    if(ch.isNumber()) sz.append(ch);
	    in>>ch;
	}
	bool ok;
	long int size=sz.toInt(&ok,10);
	if(!ok)		//если не удалось получить размер
	{
	    QMessageBox::critical(0,"Ошибка!", "Не удалось прочесть размер файла - жизненно важную инфрмацию. Программа не может продолжить распаковку.");
	    cont=false;
	}
	if(size%2==1) size++;//размер в кьюти всегда кратный двум. Если файл сильно мелкий - он "дотачивается".
	QString Rpath;			    //считываем путь файла
	in>>ch;
	while(ch!='|')
	{
	    Rpath.append(ch);
	    in>>ch;
	}
	if(Rpath==".")
	    Rpath="\\";
	QString Apath=destination;          //переписываем содержимое в файл
	Apath.append(Rpath);
	QString Adir=Apath;
	for(int i=Adir.length()-1;i>=0 && Adir[i]!='\\' && Adir[i]!='/';--i)Adir[i]='\0';//обрезаем имя файла в пути чтобы получить адрес папки
	ok=true;
	if(!QFileInfo(Adir).isDir())//если папки нет - создать ее
	    ok=mkDir(Adir);
	if(!ok)			    //если создать папку не удалось
	{
	    QMessageBox::critical(0,"Ошибка!", "Не удается создать папку "+Adir);
	    cont=false;
	}
	else
	{
	    ui->status_label->setText(Rpath);//обновляем поле статуса
	    QFile outF(Apath);		//открываем файл для записи
	    outF.open(QIODevice::WriteOnly);
	    out.setDevice(&outF);
	    QChar ch;			//символ считываемый из файла
	    QString attrib;		//Считывание атрибутов
	    for(int i=0;i<3;i++)
	    {
		in>>ch;
		attrib.append(ch);
	    }
	    int counter=0;		//счетчик для периодических действий
            for(uint i=0;i<(size/2) && cont && !inF.atEnd();++i)
	    {
		if(counter==10000)	    //периодические действия
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
	}//не удается создать папку
    }//пока не конец входного файла
}

void MainWindow::on_start_button_clicked()//запуск кодирования
{
    cont=true;//задаем условие продолжения
    QString InPath=ui->filename_line->text();
    QString password=ui->password_line->text();
    if(coding==true)//если нужно кодировать
    {
        if(InPath==NULL)					    //если нет входящего файла
	    QMessageBox::critical(0,"Ошибка!", "Выберите входной файл!");
        else if(!QFile::exists(InPath))			    //Если входной файл не существует
	    QMessageBox::critical(0,"Ошибка!", "Входной файл не существует");
        else if(password.length()==0)			    //если нет пароля
	    QMessageBox::critical(0,"Ошибка!", "Введите пароль!");
        else
        {
            QString browse_path=InPath;
            for(int i=browse_path.length()-1;i>=0 && browse_path[i]!='\\' && browse_path[i]!='/';--i)browse_path[i]='\0';//обрезаем имя файла в пути чтобы получить адрес папки
	    QString OutPath=QFileDialog::getSaveFileName(0,"Выберите выходной файл",browse_path,"*.ffc");
            if(OutPath==NULL)				    //если нет выходного файла
		QMessageBox::critical(0,"Ошибка!", "Выберите выходной файл!");
            else if(OutPath==InPath)			    //если входной и выходной файл один и тот же
		QMessageBox::critical(0,"Ошибка!", "Стопэ! Входной и выходной файлы совпадают!");
            else
            {
                ui->status_label->setText("Кодирование начато.");//работа и интерфейсом
                ui->decode->setEnabled(false);
                ui->code->setEnabled(false);
                ui->selectFile_button->setEnabled(false);
                ui->selectFolder_button->setEnabled(false);
                ui->start_button->setEnabled(false);
                ui->stop_button->setEnabled(true);
		QFile outF;			    //открываем файл для записи
		outF.setFileName(OutPath);
		outF.open(QIODevice::WriteOnly);
		out.setDevice(&outF);
                for(int i=0;i<mark.length();++i)    //вписываем метку, зашифрованную паролем
                {
                    QChar ch=plusmod(mark[i].unicode(),password[i%password.length()].unicode(),65536);
                    out<<ch;
                }
                if(folder==true)                    //Если речь идет о папке
                {
		    processFolder(InPath, InPath, password);
                }
                else                                //Если речь идео о файле
		{
		    int i=InPath.length()-1;
		    for(;i>=0 && InPath[i]!='\\' && InPath[i]!='/';--i);//находим конец пути файла
		    QString filename=InPath.mid(i,-1);		//формируем имя файла
		    codeFile(InPath, password,filename);
                }
		outF.close();			    //закрываем файл для записи
                ui->stop_button->setEnabled(false);//работа с интерфейсом
                ui->decode->setEnabled(true);
                ui->code->setEnabled(true);
                ui->selectFile_button->setEnabled(true);
                ui->selectFolder_button->setEnabled(true);
                ui->start_button->setEnabled(true);
	    }
            ui->progress->setValue(100);
            ui->status_label->setText("Кодирование завершено.");
        }
    }
    if(coding==false)//если нужно декодировать
    {
	if(InPath==NULL)					    //если нет входящего файла
	    QMessageBox::critical(0,"Ошибка!", "Выберите входной файл!");
	else if(!QFile::exists(InPath))			    //Если входной файл не существует
	    QMessageBox::critical(0,"Ошибка!", "Входной файл не существует");
	else if(password.length()==0)			    //если нет пароля
	    QMessageBox::critical(0,"Ошибка!", "Введите пароль!");
	else
	{
	    if(!checkArchive(InPath,password))		    //Если не пройдена проверка метки
            {
		QMessageBox::critical(0,"Ошибка!", "Пароль не верный, либо Вы выбрали какой-то посторонний файл!");
                ui->password_line->setFocus();
            }
	    else
	    {
		QString browse_path=InPath;
		for(int i=browse_path.length()-1;i>=0 && browse_path[i]!='\\' && browse_path[i]!='/';--i)browse_path[i]='\0';//обрезаем имя файла в пути чтобы получить адрес папки
		QString destination=QFileDialog::getExistingDirectory(0,"Выберите папку для распаковки",browse_path);
		if(destination==NULL)
		    QMessageBox::critical(0,"Ошибка!", "Выходная папка не выбрана!");
		else
                {
                    ui->status_label->setText("Кодирование начато.");//работа и интерфейсом
                    ui->decode->setEnabled(false);
                    ui->code->setEnabled(false);
                    ui->selectFile_button->setEnabled(false);
                    ui->selectFolder_button->setEnabled(false);
                    ui->start_button->setEnabled(false);
                    ui->stop_button->setEnabled(true);
                    decodeFile(destination, InPath, password);
                    ui->decode->setEnabled(true);       //работа и интерфейсом
                    ui->code->setEnabled(true);
                    ui->selectFile_button->setEnabled(true);
                    ui->selectFolder_button->setEnabled(false);
                    ui->start_button->setEnabled(true);
                    ui->stop_button->setEnabled(false);
		    ui->progress->setValue(100);
		    ui->status_label->setText("Кодирование завершено.");
		}
            }//пероль не верный
        }//нет файла или пароля
    }//декодирование
}

void MainWindow::on_exit_clicked()
{
    QApplication::exit();
}

void MainWindow::on_about_clicked()
{
    QMessageBox about;
    about.setIconPixmap(QPixmap(":/images/logo.png"));
    about.setWindowTitle("О программе");
    about.setInformativeText("<h2>"+WinTitle+"</h2>Программа предназначена для защитного кодирования данных.<br><br>Разработчик:<br> Папуша Роман, студент группы КП-11, 1 курса НТУУ \"КПИ\" 2011-2012 года.<br><br>Киев 2011");
    about.exec();
}
