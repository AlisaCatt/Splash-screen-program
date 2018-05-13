 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingswindow.h"
#include <QPixmap>

//-----------Включены для РАБОЧЕГО ВАРИАНТА дерева папок--------
#include <QFileSystemModel>
#include <QTreeView>
#include <QListView>
#include <QDirModel>
#include <QStringList>
//-------------------------------------------



//-----------ПРОБНИК--------
#include

//--------------------------

//-------------ПРОБНИК-КЛАСС-------------
class PictureHeshtegs
{
public:
    QString picture; //поле в классе для адреса картинки
    QString heshtegs; //поле в классе для хештегов

    void TakePictureHeshtegs(QString Name, QString text) //функция присобачивания значений объекту класса
    {
        picture = Name;
        heshtegs = text;
    }



};
//----------------------------------


MainWindow::MainWindow(QWidget *parent) : //основное чет
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //-------Дерево папок с фильтрацией на картинки-------------
    /*Неплохо бы доделать, чтоб папки без картинок вообще не показывались*/
    QStringList filters; //фильтры
    filters <<  "*.png" << "*.jpeg" << "*.jpg";
    QDirModel* dirModel = new QDirModel;
    dirModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot); //Это вроде спасает от фильтрации только верхнего уровня дерева
    dirModel->setNameFilters(filters); //применение фильтров
    ui->treeView->setModel(dirModel); //Присобачивание к формочке из рисовалки
    ui->treeView->setAnimated(true); //анимация списка
    ui->treeView->hideColumn(1);//Убираются столбцы времени изменения, размера и типа
    ui->treeView->hideColumn(2);
    ui->treeView->hideColumn(3);
    //--------------------------------






}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_SearchHashtagButton_clicked()
{
    //Кусок кода, который ищет картинки по хештегу
}

void MainWindow::on_ChangeHashtagButton_clicked()
{
    //Кусок кода, применяющий написанные хештеги к картинке, создает класс
    QString heshtegsline = ui->lineEdit->text();
    QString name; //Переменная для пути
    Q_ASSERT(ui->treeView->currentIndex().isValid());
    name = (static_cast<QDirModel *>(ui->treeView->model()))->filePath(ui->treeView->currentIndex()); //Получение пути до картинки
    PictureHeshtegs newobject;
    newobject.TakePictureHeshtegs(name, heshtegsline);
    ui -> statusBar -> showMessage("Хештеги успешно применены");
}

void MainWindow::on_TurnOnSplashScreenButton_clicked()
{
    //Текст кода, запускающий заставки по хештегу
}

void MainWindow::on_SettingsButton_clicked() //кнопка настроек
{
    //Новый объект класса settingswindow с названием window, открытие его.
    SettingsWindow window;
    window.setModal(true);
    window.exec();
}

//Двойное нажатие на изображении в дереве (Открывает картинку)
void MainWindow::on_treeView_doubleClicked(const QModelIndex  &index)
{
    QString name; //Переменная для пути
       Q_ASSERT(ui->treeView->currentIndex().isValid());
       name = (static_cast<QDirModel *>(ui->treeView->model()))->filePath(ui->treeView->currentIndex()); //Получение пути до картинки (Так как модель QDirModel старая, работает вот так ток)
       QPixmap myPixmap(name); //Создание переменной для картинки, которая лежит по адресу..
      /* int width = QPixmap(адрес).width(); получение размеров картинки, оставлю это тут, вдруг потом понадобится
           int height = QPixmap(адрес).height();*/
       int width = ui ->Field -> width(); //Получение размеров label
       int height = ui ->Field -> height();
       ui -> Field ->setPixmap(myPixmap.scaled(width, height, Qt::KeepAspectRatio) ); //Впихивание картинки в label (с подгоном под его размеры)
}

//----------------ПРОБНИК----------------------

//_____________________________________________


