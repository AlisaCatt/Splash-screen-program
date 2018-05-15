 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingswindow.h"
#include <QPixmap>

//-----------Включены для дерева папок--------
#include <QFileSystemModel>
#include <QTreeView>
#include <QListView>
#include <QDirModel>
#include <QStringList>
//--------------------------------------------------------------

//-----------Включены для изменения рабочего стола--------
#include <windows.h>
#include <iostream>
//--------------------------------------------------------

//-----------Включены для сохранения объектов класса--------
#include <QtCore>
//--------------------------------------------------------

//-----------ПРОБНИК-ПЕРЕЛИСТЫВАНИЕ ОБОЕВ-------
#include <Wininet.h>
#include <Shlobj.h.>
//--------------------------
#include <QFileDialog>
#include <QFileInfo>


//---------------------------------------------------------------------------------------
//Функция установки заставки
void setWall(QString name)
{ 
    //просмотр файлов в папке, затем брать адрес, менять в нужный тип, дальше ставить на рабочий стол
    wchar_t wcPath[1024];
     name.toWCharArray(wcPath);
    int result;
    result = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, wcPath, SPIF_UPDATEINIFILE);
}
//---------------------------------------------------------------------------------------





//-------------Класс Картинка+Хештег---------------
class PictureHeshtegs
{
public:

        QString qspicture; //поле в классе для адреса картинки
        QString qsheshtegs; //поле в классе для хештегов

    void TakePictureHeshtegs(QString Name, QString text) //функция присобачивания значений объекту класса
    {
        qspicture = Name;
        qsheshtegs = text;
    }

};
//Рекомендации Антона:
//Сохранять просто в конец файла и брать потом адрес и хештеги к адресу
//Заменить метатайп
//сделать поиск по хештегу
//чтобы выводил все картинки с хештегами
Q_DECLARE_METATYPE(PictureHeshtegs); //Для сохранения в файл
QDataStream& operator<<(QDataStream& out, const PictureHeshtegs& v) {
    out << v.qspicture << v.qsheshtegs;
    return out;
}
QDataStream& operator>>(QDataStream& in, PictureHeshtegs& v) {
    in >> v.qspicture;
    in >> v.qsheshtegs;
    return in;
}
//-------------------------------------------------



MainWindow::MainWindow(QWidget *parent) : //основное чет
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    //-------Дерево папок с фильтрацией на картинки-------------
    /*Неплохо бы доделать, чтоб папки без картинок вообще не показывались*/
    QStringList filters; //фильтры
    filters <<  "*.png" << "*.jpeg" << "*.jpg" << "*.bmp";
    QDirModel* dirModel = new QDirModel;
    dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot); //Это вроде спасает от фильтрации только верхнего уровня дерева
    dirModel->setNameFilters(filters); //применение фильтров
    ui->treeView->setModel(dirModel); //Присобачивание к формочке из рисовалки
    ui->treeView->setAnimated(true); //анимация списка
    ui->treeView->hideColumn(1);//Убираются столбцы времени изменения, размера и типа
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
    //--------------------------------


    //---------------------ПРОБНИИК----------------------------------------
    //


    //---------------------------------------------------------------------------------------



}

MainWindow::~MainWindow()
{
    delete ui;
}


//---------------------------------------------------------------------------------------
//
void MainWindow::on_SearchHashtagButton_clicked()
{
    //Кусок кода, который ищет картинки по хештегу
}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//Применение хештегов к картинке
void MainWindow::on_ChangeHashtagButton_clicked()
{
    QString heshtegsline = ui->lineEdit->text(); //Получаем хештеги из формы
    QString name; //Переменная для пути
    Q_ASSERT(ui->treeView->currentIndex().isValid());
    name = (static_cast<QDirModel *>(ui->treeView->model()))->filePath(ui->treeView->currentIndex()); //Получение пути до картинки
    PictureHeshtegs newobject; //создаем объект класса
    newobject.TakePictureHeshtegs(name, heshtegsline); //Применяем к нему адрес и хештеги
    ui -> statusBar -> showMessage("Хештеги успешно применены", 4000); //оповещаем пользователя


   qRegisterMetaTypeStreamOperators<PictureHeshtegs>("PictureHeshtegs"); //Вроде как сохраняет в файл данные класса
    {
           PictureHeshtegs t;
           t.qspicture = name;
           t.qsheshtegs = heshtegsline;
           QSettings s("config.ini", QSettings::IniFormat);
           s.setValue("pichesh", QVariant::fromValue(t));

     }



}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//Текст кода, запускающий заставки по хештегу
void MainWindow::on_TurnOnSplashScreenButton_clicked()
{
    //очистить папку с обоями

    bool QDir::removeRecursively()
    QString heshtegsline = ui->lineEdit->text(); //Получаем хештеги из формы
    QString name; //Переменная для пути
    Q_ASSERT(ui->treeView->currentIndex().isValid());
    name = (static_cast<QDirModel *>(ui->treeView->model()))->filePath(ui->treeView->currentIndex()); //Получение пути до картинки
    QFileInfo fileInfo(name);
    QString filename(fileInfo.fileName());
    QFile::copy(name, "C:\\Users\\Lisa\\Documents\\Kursach\\Wallpapers\\" +  filename); //Копирование файла в папку
    setWall(name); //запуск функции-установщика заставок
}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//кнопка настроек
void MainWindow::on_SettingsButton_clicked()
{
    //Новый объект класса settingswindow с названием window, открытие его.
    SettingsWindow window;
    window.setModal(true);
    window.exec();
}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//Двойное нажатие на изображении в дереве (Открывает картинку)
void MainWindow::on_treeView_doubleClicked(const QModelIndex  &index)
{
    ui ->lineEdit ->setText(" "); //очищение формы от предыдущих хештегов
    QString name; //Переменная для пути
       Q_ASSERT(ui->treeView->currentIndex().isValid());
       name = (static_cast<QDirModel *>(ui->treeView->model()))->filePath(ui->treeView->currentIndex()); //Получение пути до картинки (Так как модель QDirModel старая, работает вот так ток)
       QPixmap myPixmap(name); //Создание переменной для картинки, которая лежит по адресу..

       int width = ui ->Field -> width(); //Получение размеров label
       int height = ui ->Field -> height();
       ui -> Field ->setPixmap(myPixmap.scaled(width, height, Qt::KeepAspectRatio) ); //Впихивание картинки в label (с подгоном под его размеры)

       qRegisterMetaTypeStreamOperators<PictureHeshtegs>("PictureHeshtegs");
       QSettings s("config.ini", QSettings::IniFormat);
       QVariant value = s.value("pichesh");
       PictureHeshtegs t = value.value<PictureHeshtegs>();
       if (t.qspicture == name)
       ui ->lineEdit ->setText(t.qsheshtegs);


}
//---------------------------------------------------------------------------------------

