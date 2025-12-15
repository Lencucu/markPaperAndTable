#ifndef sql_wrapper_hpp
#define sql_wrapper_hpp

#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <functional>


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


void query_sqlite_db(std::function<void(QString&)> callback) {
    {   // 添加 SQLite 数据库连接
        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "mpt_temp");

        // 设置数据库文件路径
        db.setDatabaseName("mpt.db");  // 例如数据库文件在当前目录下叫 mpt.db

        if (!db.open()) {
            qDebug() << "无法连接数据库:" << db.lastError().text();
            return;
        }

        // 查询数据
        {
            QSqlQuery query(db);
            if (query.exec("SELECT name FROM notes ORDER BY updated_at DESC;")) {
                while (query.next()) {
                    QString name = query.value(0).toString();
                    callback(name);
                    // papers.emplace_back(name); // 如果你需要存到容器里
                }
            }
        }

        db.close();
    }
    QSqlDatabase::removeDatabase("mpt_temp");
}


#endif // sql_wrapper_hpp
