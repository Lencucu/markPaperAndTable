#include <widgets/XHorizontalScrollArea.hpp>
#include <widgets/XHoverPlane.hpp>
#include <widgets/XPreview.hpp>
#include <interface_database/sql_wrapper.hpp>
#include <global_uniforms.hpp>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QScreen>
#include <QRect>
#include <QSpacerItem>
#include <QGuiApplication>
#include <QString>

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


XHoverPlane::XHoverPlane(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setObjectName("mpt");
    // QRect screenGeometry = QApplication::primaryScreen()->geometry();
    // int screenWidth = screenGeometry.width();
    // int screenHeight = screenGeometry.height();

    // int margin = 10;
    // width:344.443871|height:193.749677|2.690968
    int screenWidth = physicalSize2pix(344.443871);
    int screenHeight = physicalSize2pix(193.749677);

    int margin = physicalSize2pix(2.690968);
    setStyleSheet(QString::asprintf(
        "#mpt, #previews {"
        " background-color: #D0E8D8;"
        "}"
        "QLineEdit {"
        " border: none;"
        " margin-left: %dpx;"
        " margin-right: %dpx;"
        "}"
        "QTextBrowser {"
        " border: none;"
        "}"
        "QScrollArea {"
        " border-top: 0px;"
        " border-bottom: 0px;"
        " border-left: 1px solid #D2F0E0;"
        " border-right: 1px solid #D2F0E0;"
        "}",
        margin,margin
    ));

    move(screenWidth - screenWidth/3 - 2*margin,
         screenHeight - screenHeight/4 - /*2*margin -*/ getTaskbarHeight());// * LC_mark * 获取到的任务栏高度偏长
    resize(screenWidth/3, screenHeight/4);

    /*
    布局会改变自适应大小的子控件，但是固定大小的子控件会改变布局的限制从而影响父控件大小，但是父控件对应的scrollarea的viewport大小不是父控件，所以父控件的大小实际上是改变了，也正因为如此出现了滚动条
    也就是实际上父控件不管怎么move，他的左上角始终在viewport的左上角
    也就是说scrollArea的大小就是它对应的viewport的大小，当对应控件超出范围后就会出现滚动条
    */

    searchBar = new SearchBar;
    // ** lambda reaction
    /*
        new preview() or repalce
        focusOn()
        insertIntoPreviews()
    */
    connect(searchBar, &QLineEdit::textEdited, this, [&](const QString &text){
        // qDebug() << "用户编辑文本:" << text;
        // ** matchTextAndProcess(text.c_str(),fileBoxs,reaction(float score,size_t line,size_t column,size_t length));
    });
    searchBar->setFixedHeight(screenHeight/4/6);
    // QScreen *screen = QGuiApplication::primaryScreen();
    // searchBar->setText(QString::asprintf(
    //                        "width:%f|height:%f|%f",
    //                        pix2physicalSize(screenWidth),
    //                        pix2physicalSize(screenHeight),
    //                        pix2physicalSize(10)
    //                    ));

    XHorizontalScrollArea* scrollArea = new XHorizontalScrollArea;
    GradientOverlay* gradientoverlay = new GradientOverlay{scrollArea};
    gradientoverlay->radio = double(margin)*3/screenWidth;
    scrollArea->gradientoverlay = gradientoverlay;
    scrollArea->setWidgetResizable(true);// 不知道怎么回事，好像不加这一条或者说是false的情况时视口不会追踪widget，即没内容，而且只会在widget resize时才会更新视口一样，所以默认还是加上，不知道不加和加到底有什么区别
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);   // 总显示
    // scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);   // 默认，超出才显示
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QWidget* container = new QWidget;
    container->setObjectName("previews");
    previews = new QHBoxLayout{container};
    previews->setAlignment(Qt::AlignLeft);
    previews->setSpacing(margin);
    previews->setContentsMargins(0,0,0,0);
    scrollArea->setWidget(container);

    QVBoxLayout* vLayout = new QVBoxLayout{this};
    vLayout->setContentsMargins(0, margin, 0, margin);//left, top, right, bottom
    vLayout->setSpacing(0);
    vLayout->addWidget(searchBar);
    vLayout->addSpacerItem(new QSpacerItem(0,margin,QSizePolicy::Expanding,QSizePolicy::Fixed));
    vLayout->addWidget(scrollArea);

    vLayout->activate();
    previews->addSpacerItem(new QSpacerItem(margin,0,QSizePolicy::Fixed,QSizePolicy::Expanding));
    query_sqlite_db([&](QString& name){
        globalresource::papers.emplace_back(name);
        XPreview* preview = new XPreview;
        preview->textbrowser->setMarkdown(globalresource::papers.back().content());
        preview->setFixedSize(scrollArea->height()*4/3,scrollArea->height());
        previews->addWidget(preview);
        XPreview* preview2 = new XPreview;
        preview2->textbrowser->setMarkdown(globalresource::papers.back().content());
        preview2->setFixedSize(scrollArea->height()*4/3,scrollArea->height());
        previews->addWidget(preview2);
    });
    previews->addSpacerItem(new QSpacerItem(margin,0,QSizePolicy::Fixed,QSizePolicy::Expanding));

    effect = new QGraphicsOpacityEffect(this);
    setGraphicsEffect(effect);
    qDebug() << "opacity" << effect->opacity();
    anim = new QPropertyAnimation(effect, "opacity", this);
    anim->setEasingCurve(QEasingCurve::OutCubic);
}


bool XHoverPlane::event(QEvent *e){
    if (e->type() == QEvent::WindowDeactivate) {
        qDebug() << "窗口及子控件失去焦点，脱离激活";

        anim->stop();
        anim->setDuration(1000);
        anim->setStartValue(effect->opacity());
        anim->setEndValue(0.05);
        QObject::connect(anim, &QPropertyAnimation::finished, [&]() {
            // 动画结束后的操作
            if(effect->opacity()<0.5)
                enableMousePassthrough(((HWND)winId()));// 或许设置下延迟多久以后再贯穿更好点
        });
        anim->start();
        // setWindowOpacity(0.05);
    }
    if (e->type() == QEvent::WindowActivate) {
        qDebug() << "窗口获得焦点";
        qDebug() << "opacity" << effect->opacity();

        disableMousePassthrough(((HWND)winId()));
        searchBar->setFocus();
        anim->stop();
        anim->setDuration(300);
        anim->setStartValue(effect->opacity());
        anim->setEndValue(1);
        anim->start();
        // setWindowOpacity(1);
    }
    return QWidget::event(e);
}


XHoverPlane::~XHoverPlane()
{
}

