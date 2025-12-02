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
    ~globalResource(){delete db;}
    QSqlDatabase* db = nullptr;
    std::vector<singlePaper> papers;

    void init_db(){
        db = new QSqlDatabase(QSqlDatabase::addDatabase("QODBC"));

        // 使用 PostgreSQL ODBC 驱动连接
        db->setDatabaseName(
                    "Driver={PostgreSQL Unicode(x64)};"
                    "Server=localhost;"
                    "Port=5432;"
                    "Database=mpt;"
                    "Uid=mpt_user;"
                    "Pwd=mpt_password;"
                    );

        if (!db->open()) {
            qDebug() << "无法连接数据库:" << db->lastError().text();
            return;
        }

        QSqlQuery query(*db);

        if(query.exec("SELECT name FROM notes")) {
            while(query.next()) {
                papers.emplace_back(query.value(0).toString());
//                QString name = query.value(0).toString();
//                qDebug() << name;
            }
        } else {
            qDebug() << "查询失败:" << query.lastError().text();
        }
    }
};


extern globalResource globalresource;

#endif // GLOBALRESOURCE_H
