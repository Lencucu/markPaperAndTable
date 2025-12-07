#include "mptfloatplane.h"
#include <QApplication>
#include <QScreen>
#include <QRect>
#include <windows.h>
#include <shellapi.h>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>

preview::preview(QWidget *parent)
    : QWidget(parent)
{
    textbrowser = new QTextBrowser{this};
}

preview::~preview()
{
}

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
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);

    QRect screenGeometry = QApplication::primaryScreen()->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    int margin = 10;
    move(screenWidth - screenWidth/3 - 2*margin,
         screenHeight - screenHeight/4 - /*2*margin -*/ getTaskbarHeight());// * LC_mark * 获取到的任务栏高度偏长
    resize(screenWidth/3, screenHeight/4);

    /*
    布局会改变自适应大小的子控件，但是固定大小的子控件会改变布局的限制从而影响父控件大小，但是父控件对应的scrollarea的viewport大小不是父控件，所以父控件的大小实际上是改变了，也正因为如此出现了滚动条
    也就是实际上父控件不管怎么move，他的左上角始终在viewport的左上角
    也就是说scrollArea的大小就是它对应的viewport的大小，当对应控件超出范围后就会出现滚动条
    */

    searchBar = new QLineEdit;
    searchBar->setFixedHeight(screenHeight/4/6);

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);// 不知道怎么回事，好像不加这一条或者说是false的情况时视口不会追踪widget，即没内容，而且只会在widget resize时才会更新视口一样，所以默认还是加上，不知道不加和加到底有什么区别
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 总显示
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);   // 默认，超出才显示
    QWidget* container = new QWidget;
    previews = new QHBoxLayout{container};
    previews->setSpacing(margin);
    previews->setContentsMargins(0,0,0,0);
    scrollArea->setWidget(container);

    QVBoxLayout* vLayout = new QVBoxLayout{this};
    vLayout->setContentsMargins(margin, margin, margin, margin);//left, top, right, bottom
    vLayout->setSpacing(0);
    vLayout->addWidget(searchBar);
    vLayout->addSpacerItem(new QSpacerItem(0,margin,QSizePolicy::Expanding,QSizePolicy::Fixed));
    vLayout->addWidget(scrollArea);

    vLayout->activate();
    for(int i=0;i<10;++i)
    {   ::preview* preview = new ::preview;
        preview->setFixedSize(scrollArea->height()*4/3,scrollArea->height());
        previews->addWidget(preview);
    }
}

mptfloatplane::~mptfloatplane()
{
}

