#include <vector>
#include <lcctoolbox_filebox.hpp>
#include <QDebug>


class singlePaper: public LccToolbox::fileBox{
    QString dirAddTemplate = "notes/%1.md";
public:
    singlePaper(QString& name){
        QByteArray ba = dirAddTemplate.arg(name).toUtf8();
        read(ba.constData());
        qDebug() << content();
    }
};


namespace globalresource{
	extern std::vector<singlePaper> papers;
}
