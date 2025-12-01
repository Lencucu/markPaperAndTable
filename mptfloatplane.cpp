#include "mptfloatplane.h"
#include "./ui_mptfloatplane.h"
#include <QApplication>
#include <QScreen>
#include <QRect>
#include <windows.h>
#include <shellapi.h>

int getTaskbarHeight() {
    APPBARDATA abd{};
    abd.cbSize = sizeof(APPBARDATA);

    if (SHAppBarMessage(ABM_GETTASKBARPOS, &abd)) {
        RECT r = abd.rc;
        int height = r.bottom - r.top;
        int width  = r.right - r.left;

        // 判断任务栏是在上下还是左右
        bool horizontal = (height < width);
        return horizontal ? height : width;
    }
    return 0;
}

mptfloatplane::mptfloatplane(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mptfloatplane)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);

    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    int margin = 10;
    move(screenWidth - screenWidth/3 - 2*margin,
         screenHeight - screenHeight/4 - /*2*margin -*/ getTaskbarHeight());// * LC_mark * 获取到的任务栏高度偏长
    resize(screenWidth/3, screenHeight/4);
    ui->lineEdit->move(margin, margin);
    ui->lineEdit->resize(width() - 2*margin, ui->lineEdit->height());
}

mptfloatplane::~mptfloatplane()
{
    delete ui;
}

