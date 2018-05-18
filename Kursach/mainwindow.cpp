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
#include <QTextStream>
#include <QTextCodec>
//--------------------------------------------------------

//-----------Включены для установки обоев-------
#include <QTimer>
#include <QFileDialog>
#include <QFileInfo>
//--------------------------




//---------------------------------------------------------------------------------------
//Функция установки заставки
void setWall()
{ 

    QDir dir ("C:\\Users\\Lisa\\Documents\\Kursach\\Wallpapers\\"); //объявляем директорию
    QStringList filters; //фильтры
    filters <<  "*.png" << "*.jpeg" << "*.jpg" << "*.bmp";
    dir.setNameFilters(filters); //устанавливаем фильтр  файлов
    QFileInfoList list = dir.entryInfoList(); //получаем список файлов директории
    int result;
    for (int i = 0; i < list.size(); ++i)
    {
    QFileInfo fileInfo = list[i];
    wchar_t wcPath[1024];
    QString path;
    path = fileInfo.absoluteFilePath();
    path.toWCharArray(wcPath);
   /*QTimer timer = new QTimer();
    timer->start(5000);
    if(timer->remainingTime() == 0)*/

       {
       result = SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,  wcPath , SPIF_UPDATEINIFILE);
      Sleep(10000); //Такой себе способ, разобраться с таймером
       }
    }
}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//Функция очистки папки (для папки с обоями)
int removeFolder(QDir & dir)
{
  int res = 0;
QStringList lstDirs = dir.entryList(QDir::Dirs |QDir::AllDirs |QDir::NoDotAndDotDot);
 //Получаем список файлов
 QStringList lstFiles = dir.entryList(QDir::Files);

 //Удаляем файлы
 foreach (QString entry, lstFiles)
 {
  QString entryAbsPath = dir.absolutePath() + "/" + entry;
  QFile::setPermissions(entryAbsPath, QFile::ReadOwner | QFile::WriteOwner);
  QFile::remove(entryAbsPath);
 }

 //Для папок делаем рекурсивный вызов
 foreach (QString entry, lstDirs)
 {
  QString entryAbsPath = dir.absolutePath() + "/" + entry;
  QDir dr(entryAbsPath);
  removeFolder(dr);
 }

 //Удаляем обрабатываемую папку
 if (!QDir().rmdir(dir.absolutePath()))
 {
   res = 1;
 }
 return res;
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

    QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) ); //Чтобы кодировка поддерживалась
    QFile out("Walls.txt");
    out.open(QIODevice::ReadOnly |QIODevice::Text); //поиск строчки с адресом картинки (ну вдруг она уже сохранялась)

        QString line;
        QTextStream stream(&out);
        QStringList arr;
        bool flag2 = false;
        int countline(0);
        while(!out.atEnd())
        {
           line =  out.readLine();
           countline++;
           if((line.trimmed())==name)
              {
               flag2 = true;
               out.close();
              }
        }
           if (flag2 == true)
           {
               out.open(QIODevice::ReadOnly |QIODevice::Text);
               QTextStream streams(&out);
               for(int i(0); i<countline; i++)
                 {
                    QString copyline = streams.readLine();
                    arr.append(copyline);
                 }
               QString copyline =  ui->lineEdit->text();
               arr.append(copyline);
               QString rubbish = out.readLine();

                        while (true)
                         {
                          QString copylines = out.readLine();
                          if (copylines.isNull())
                               break;
                          else
                               arr.append(copylines);
                          }


            out.close();
            out.open(QIODevice::WriteOnly);
            out.resize(0); //чтобы удалить все старые строки в файле
            QTextStream stream(&out);
            for (QStringList::Iterator it = arr.begin(); it!=arr.end(); ++ it)
                   stream << * it << '\n'<< '\r';
            out.close();

           }
       else
           {

out.close();
               out.open(QIODevice::Append);//Append = дописывание в конец файла
                  QTextStream stream( &out );
                    stream << newobject.qspicture;
                      stream << "\r\n";
                     stream << newobject.qsheshtegs;
                     stream << "\r\n";
                   out.close();

           }

}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//Текст кода, запускающий заставки по хештегу
void MainWindow::on_TurnOnSplashScreenButton_clicked()
{
    QDir dir ("C:\\Users\\Lisa\\Documents\\Kursach\\Wallpapers\\");
    removeFolder(dir); //очищаем папку с предыдущими обоями
    QString heshtegsline = ui->lineEdit->text(); //Получаем хештеги из формы
    QString name; //Переменная для пути
    Q_ASSERT(ui->treeView->currentIndex().isValid());
    name = (static_cast<QDirModel *>(ui->treeView->model()))->filePath(ui->treeView->currentIndex()); //Получение пути до картинки
    QFileInfo fileInfo(name);
    QString filename(fileInfo.fileName());
    QFile::copy(name, "C:\\Users\\Lisa\\Documents\\Kursach\\Wallpapers\\" +  filename); //Копирование файла в папку
    setWall(); //запуск функции-установщика заставок
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

}
//---------------------------------------------------------------------------------------

