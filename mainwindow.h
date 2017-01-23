#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtGui>
namespace Ui {
    class MainWindow;
}
class MainWindow : public QMainWindow {
    Q_OBJECT					//макрос для работы сигалов-слотов
public:
    int sizeX;//размер окна			//переменные
    int msizeY;
    int sizeY;
    bool opened;//разверну то ли откно (после выбора файла)
    bool coding;//миссия - кодировать. иначе - декодировать
    bool folder;//цель папка. Иначе - файл
    bool exist;	//файл существует
    bool cont;	//условие продолжения каких либо операций
    int argc;	//аргументы(кол-во)
    QString argv;//аргумент 2
    QString mark;//метка в начале файла для обнаружения архива
    QString check;//комбинация для проверки пароля
    QString WinTitle;//заголовок окна
    QDataStream out;//поток вывода информации
    QDataStream in;//поток ввода информации

    explicit MainWindow(QWidget *parent = 0);	//функции
    void sleep(int);
    bool mkDir(QString);
    bool checkArchive(QString,QString);
    void codeFile(QString, QString, QString);
    void decodeFile(QString, QString, QString);
    void fileCallback(QString, QString, QString);
    void processFolder(QString, QString, QString);
    void open_window();
    void close_window();
    void refreshInterface();
    void OnStart();
    ~MainWindow();
private slots:
    void on_selectFile_button_clicked();	//слоты
    void on_start_button_clicked();
    void on_stop_button_clicked();
    void on_code_clicked();
    void on_decode_clicked();
    void on_filename_line_editingFinished();
    void on_selectFolder_button_clicked();
    void on_password_line_returnPressed();
    void on_exit_clicked();
    void on_about_clicked();

private:
    Ui::MainWindow *ui;				//пользовательский интерфейс
};
#endif // MAINWINDOW_H
