#include "authorization.h"
#include "ui_authorization.h"
#include <QMessageBox>
#include <QPixmap>

authorization::authorization(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::authorization)
{
    ui->setupUi(this);
}

authorization::~authorization()
{
    delete ui;
}

void authorization::on_pushButton_clicked()
{
    //Картинка с замочком, ее ширина и высота
    QPixmap pictureLock(":/img/images/avatar_addon_11cb4_180x180.png");
    int picLockWidth = ui ->picLock -> width();
    int picLockHeight = ui ->picLock -> height();

    //Заливаем переменную с адресом картинки в label, устанавливаем подстройку под разные разрешения
    ui -> picLock -> setPixmap(pictureLock.scaled(picLockWidth, picLockHeight, Qt::KeepAspectRatio));

    //Берем из окошка авторизации логин и пароль
    QString Login = ui -> LoginLine -> text();
    QString Password = ui -> PasswordLine -> text();

    //сравниваем логин и пароль с необходимыми
    if (Login == /*Логин*/"Cat" && Password == /*Пароль*/"Cat")
    {
      QMessageBox::information(this, "Авторизация", "Вы успешно авторизовались!");
    }
    else
    {
      QMessageBox::critical(this, "Ошибка!", "Логин или пароль не правильный!");
    }
}
