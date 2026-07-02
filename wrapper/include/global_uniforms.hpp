#include <vector>
#include <lcctoolbox_filebox.hpp>
#include <QDebug>
#include <QCoreApplication>


class singlePaper: public LccToolbox::fileBox{
    QString dirAddTemplate = QCoreApplication::applicationDirPath()+"/notes/%1.md";
public:
    singlePaper(QString& name){
        QByteArray ba = dirAddTemplate.arg(name).toUtf8();
        read(ba.constData());
        // qDebug() << content();
    }
};

extern size_t physicalSize2pix(double l);
extern size_t physicalSize2pix_inch(double l);
extern qreal pix2physicalSize(size_t l);
extern qreal pix2physicalSize_inch(size_t l);


namespace globalresource{
	extern std::vector<singlePaper> papers;
}








#include <windows.h>
// 让一个 Qt 窗口鼠标穿透（贯穿）
extern void enableMousePassthrough(HWND WID);
extern void disableMousePassthrough(HWND WID);
