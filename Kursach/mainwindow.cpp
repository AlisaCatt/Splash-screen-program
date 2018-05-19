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
#include <iostream>
#include "shobjidl.h"
#include <Windows.h>
#include <QApplication>
//--------------------------------------------------------

//-----------Включены для сохранения объектов класса--------
#include <QTextStream>
#include <QTextCodec>
//--------------------------------------------------------

//-----------Включены для установки обоев-------
#include <QTimer>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>
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
    for (int i = 0; i < list.size(); ++i)
    {
    QFileInfo fileInfo = list[i];
    wchar_t wcPath[1024];
    QString path;
    path = fileInfo.absoluteFilePath();
    path.toWCharArray(wcPath);




         SystemParametersInfo(SPI_SETDESKWALLPAPER, 0,  wcPath , SPIF_UPDATEINIFILE);

    }
}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//Функция очистки папки (для папки с обоями)
int removeFolder(QDir & dir)
{
int res = 0;
//Получаем список папок
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

//---------------------------------------------------------------------------------------
//Поиск картинки по хештегу
void MainWindow::on_SearchHashtagButton_clicked()
{
QString heshtegsline = ui->lineEdit->text(); //Получаем хештеги из формы
QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) ); //Чтобы кодировка поддерживалась
QFile out("Walls.txt");
out.open(QIODevice::ReadOnly |QIODevice::Text); //поиск строчки с хештегами из формы в файле
    QString line1; //Переменная для адресов из файла
    QString line2; //Переменная для хештегов из файла
    int width = ui ->Field -> width(); //Получение размеров label
    int height = ui ->Field -> height();
    bool flag = false; //Флаг проверки нахождения картинки
    while(!out.atEnd()) //пока не конец файла
    {
       line1 =  out.readLine(); //читаем  адрес
       line2 =  out.readLine(); //читаем  хештеги

       if((line2.trimmed())==heshtegsline) //trimmed - отбрасывание конца строки. Сравниваем с хештегами
          {
           QPixmap myPixmap(line1.trimmed()); //Создание переменной для картинки, которая лежит по адресу..
           ui -> Field ->setPixmap(myPixmap.scaled(width, height, Qt::KeepAspectRatio));
           flag = true; //Нашли хотя б одну по хештегу, меняем флаг
          }
    }
    if (flag==false)
    {
      ui -> statusBar -> showMessage("Картинки с таким хештегом нет", 4000); //оповещаем пользователя
    }
     out.close();
}
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
//Применение хештегов к картинке и сохранение их в файл
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
        QString line; //Переменная для строк из файла
        QStringList arr; //массив, куда будет временно сохраняться инфа из файла
        bool flag2 = false; //Флаг для отслеживания, есть ли уже адрес картинки в файле или надо его написать
        int countline(0); //подсчет строк до адреса, если он уже есть в файле
        while(!out.atEnd()) //пока не конец файла
        {
           line =  out.readLine(); //ччитаем и считаем строки
           countline++;
           if((line.trimmed())==name) //trimmed - отбрасывание конца строки. Сравниваем с адресом "свежей" картинки.
              {
               flag2 = true; //адреса совпали - меняем флаг
               out.close();
              }
        }
           if (flag2 == true) //раз адреса совпали, меняем хештеги в файле
           {
               out.open(QIODevice::ReadOnly |QIODevice::Text);
               QTextStream streams(&out);
               for(int i(0); i<countline; i++)
                 {
                    QString copyline = streams.readLine();
                    arr.append(copyline); //копируем во временный массив все строчки до адреса картинки и сам адрес
                 }
               QString copyline =  ui->lineEdit->text();
               arr.append(copyline); //следующая строка в массиве - новые хештеги картинки
               streams.readLine(); //съедаем строчку со старыми хештегами
               while (true) //бесконечный цикл
                  {
                    QString copylines = streams.readLine(); //берем строку из файла
                    if (copylines.isNull())
                         break; //если она нулевая, то выходим из цикла
                    else
                         arr.append(copylines); //копируем оставшиеся адреса и хештеги
                  }
                out.close(); //Закрываем файл
                out.open(QIODevice::WriteOnly); //Открываем для записи
                out.resize(0); //чтобы удалить все старые строки в файле
                QTextStream stream(&out);
                for (int i = 0; i < arr.size(); ++i) //записываем из массива в файл
                    {
                     stream << arr.at(i) << '\r' << '\n';
                    }
                out.close();

           }
           else //если адреса картинки еще нет в файле
           {
              out.close(); //Закрываем чтение файла
              out.open(QIODevice::Append);//Append = дописывание в конец файла
              QTextStream stream( &out );
              stream << newobject.qspicture; //Записываем две новые строки в файл
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
//Двойное нажатие на изображении в дереве (Открывает картинку и ее хештеги)
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
       QTextCodec::setCodecForLocale( QTextCodec::codecForName( "UTF-8" ) ); //Чтобы кодировка поддерживалась
       QFile out("Walls.txt");
       out.open(QIODevice::ReadOnly |QIODevice::Text); //поиск строчки с адресом картинки (ну вдруг она уже сохранялась)
           QString line; //Переменная для строк из файла
           while(!out.atEnd()) //пока не конец файла
           {
              line =  out.readLine(); //читаем  строки
              if((line.trimmed())==name) //trimmed - отбрасывание конца строки. Сравниваем с адресом картинки.
                 {
                  line =  out.readLine(); //Адрес совпал, читаем строчку хештегов (след.строка)
                  ui ->lineEdit ->setText(line); //отправляем хештеги в форму
                  out.close();
                 }
           }


}
//---------------------------------------------------------------------------------------

