#ifndef GLOBALRESOURCE_H
#define GLOBALRESOURCE_H
#include <vector>
#include <list>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <lcctoolbox_filebox.hpp>

class singlePaper: public LccToolbox::fileBox{
    QString dirAddTemplate = "notes/%1.md";
public:
    singlePaper(QString&& name){
        QByteArray ba = dirAddTemplate.arg(name).toUtf8();
        read(ba.constData());
        qDebug() << content();
    }
};


class globalResource{
public:
    std::vector<singlePaper> papers;

    void query_db() {
        {   QSqlDatabase db = QSqlDatabase::addDatabase("QODBC", "mpt_temp");

            db.setDatabaseName(
                "Driver={PostgreSQL Unicode(x64)};"
                "Server=localhost;"
                "Port=5432;"
                "Database=mpt;"
                "Uid=mpt_user;"
                "Pwd=mpt_password;"
            );

            if (!db.open()) {
                qDebug() << "无法连接数据库:" << db.lastError().text();
                return;
            }

            {
                QSqlQuery query(db);
                if (query.exec("SELECT name FROM notes")) {
                    while (query.next()) {
//                        papers.emplace_back(query.value(0).toString());
                    }
                }
            }

            db.close();
        }
        QSqlDatabase::removeDatabase("mpt_temp");
    }
};


extern globalResource globalresource;

#endif // GLOBALRESOURCE_H
