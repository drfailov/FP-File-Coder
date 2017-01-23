#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtGui>
namespace Ui {
    class MainWindow;
}
class MainWindow : public QMainWindow {
    Q_OBJECT					//������ ��� ������ �������-������
public:
    int sizeX;//������ ����			//����������
    int msizeY;
    int sizeY;
    bool opened;//�������� �� �� ����� (����� ������ �����)
    bool coding;//������ - ����������. ����� - ������������
    bool folder;//���� �����. ����� - ����
    bool exist;	//���� ����������
    bool cont;	//������� ����������� ����� ���� ��������
    int argc;	//���������(���-��)
    QString argv;//�������� 2
    QString mark;//����� � ������ ����� ��� ����������� ������
    QString check;//���������� ��� �������� ������
    QString WinTitle;//��������� ����
    QDataStream out;//����� ������ ����������
    QDataStream in;//����� ����� ����������

    explicit MainWindow(QWidget *parent = 0);	//�������
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
    void on_selectFile_button_clicked();	//�����
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
    Ui::MainWindow *ui;				//���������������� ���������
};
#endif // MAINWINDOW_H
