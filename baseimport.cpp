#include "baseimport.h"

bool ServiceLessThan(const QStringList &a, const QStringList &b)
{
    return a.back().toInt() < b.back().toInt();
}

BaseImport::BaseImport(QObject *parent) : QObject(parent)
{
    files       << "Firms.txt";
    columCount  << 15;

    files       << "Services.txt";
    columCount  << 4;

    files       << "ServicePresence.txt";
    columCount  << 5;

    files       << "CarMarksEN.txt";
    columCount  << 3;

    files       << "CarModelsEN.txt";
    columCount  << 5;

    files       << "CarModelGroupsEN.txt";
    columCount  << 3;

    files       << "CarMarkGroupsEN.txt";
    columCount  << 2;

    files       << "CarBodyModelsEN.txt";
    columCount  << 6;

    files       << "CarBodyModelGroupsEN.txt";
    columCount  << 4;

    files       << "CarENDetailNames.txt";
    columCount  << 2;

    files       << "CarENLinkedDetailNames.txt";
    columCount  << 2;

    files       << "CarEngineModelsEN.txt";
    columCount  << 5;

    files       << "CarEngineModelGroupsEN.txt";
    columCount  << 3;

    files       << "CarEngineAndBodyCorrespondencesEN.txt";
    columCount  << 4;

    files       << "CarEngineAndModelCorrespondencesEN.txt";
    columCount  << 3;

    files       << "CarPresenceEN.txt";
    columCount  << 11;

    files       << "CatalogNumbersEN.txt";
    columCount  << 3;
}

BaseImport::~BaseImport()
{

}

void BaseImport::startImport(QString baseName_)
{
    baseName = baseName_;

    emit messages("Импорт запущен из " + path);

    if(baseName.isEmpty() || baseName == "mysql"
                          || baseName == "performance_schema"
                          || baseName == "information_schema")
    {
        emit fail("ОШИБКА! Не выбрана база для импорта или имя базы не корректно!");
        return;
    }

    emit messages("Начинаю очистку таблиц от старых данных");
    // Таблицы отчищаем в обратном порядке чтобы избажать проблем с ключами
    for (int i = files.count() - 1; i > 0; --i) {
        if(!clearTable(files[i].split(".").first().toLower()))
        {
            emit fail("Не могу очистить таблицу");
            return;
        }
    }
    emit messages("Очистка закончена");

    emit setMaxBar(files.count());
    for (int i = 0; i < files.count(); ++i)
    {
        emit messages("Импорт файла " + files[i]);
        importFile(files[i], columCount[i]);
        emit setValueToBar(i);
    }

    emit setValueToBar(files.count());
    emit messages("Импорт окончен");
}

void BaseImport::setPathToFiles(QString path_)
{
    path = path_;
}

void BaseImport::setBaseName(QString base_)
{
    baseName = base_;
}

bool BaseImport::importFile(QString fileName, int coulumCount)
{
    QFile file(path + "/" + fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        emit messages("ОШИБКА!!! Нет файла " + fileName);
        return false;
    }

    QSqlQuery query;
    // получаем список столбцов таблицы
    query.prepare("SHOW columns FROM " + baseName +"."+ fileName.split(".").first().toLower());
    if(!query.exec())
    {
        emit fail("Не могу получить список столбцов таблицы " + fileName.split(".").first().toLower());
        emit messages(query.lastQuery());
        return false;
    }

    QString filds = "(";
    int i = 0;
    while (query.next())
    {
        if(query.size() - 1 == i)
            filds += query.value(0).toString();
        else
            filds += query.value(0).toString() + ",";
        i++;
    }
    filds += ") VALUES(";
    for(;i -1 > 0; --i)
    {
        filds += "?,";
    }
    filds += "?)";

    QList<QStringList> swap;
    QTextStream in(&file);
    in.setCodec("CP1251");
    swap.clear();
    while(!in.atEnd())
    {
        QString text = in.readLine();
        QStringList line = text.split(";");

        if(line.count() < coulumCount)
        {
            while(line.count() < coulumCount && !in.atEnd())
            {
                QStringList tmp = in.readLine().split(";");
                line.back() = line.back() + tmp.takeFirst();
                if(!tmp.isEmpty())
                    line += tmp;
            }
        }

        if(fileName == "Services.txt")
        {
             swap << line; // отправляем в свап если файл нужно считать весь и обработать
        }
        else
        {
            if(fileName == "CarPresenceEN.txt")
            {
                line.back().replace(" ?", "");
                line.back().replace(",", ".");
            }
            insertData(fileName, line, filds, coulumCount);
        }
    }
    if(!swap.isEmpty())
    {
        // обрабатываем свап
        qSort(swap.begin(), swap.end(), ServiceLessThan);
        while(!swap.isEmpty())
        {
            QSqlQuery query;
            QStringList tmp = swap.takeFirst();

            if(tmp[3].isEmpty())
                tmp[3] = "NULL";
            query.prepare("INSERT INTO " + baseName + ".services"
                          " (ID, Name, ID_Parent) "
                          "VALUES (" + tmp[0] + ", \'" + tmp[1] + "\'," + tmp[3] +")");
            if(!query.exec())
            {
                emit messages("ОШИБКА: не могу добавить строку в таблицу Service " + query.lastQuery());
            }
        }
    }
    file.close();
    return true;
}

bool BaseImport::insertData(QString fileName, QStringList data, QString filds, int count)
{
    QSqlQuery insert;
    QString tableName = fileName.split(".").first().toLower();
    QString sql = "INSERT INTO " + baseName + "." + tableName;


    sql += filds;
    insert.prepare(sql);

    for (int i = 0; i < count; ++i)
    {
       insert.addBindValue(data.takeFirst());
    }

    if(!insert.exec())
    {
        emit fail("----------------------------------------------\n"
                  "Не вставить строку в таблицу " + tableName + "\n"
                  + insert.lastQuery() + "\n"
                  + insert.lastError().text() );
        return false;
    }
    return true;
}

bool BaseImport::clearTable(QString tableName)
{
    QSqlQuery query;
    if(tableName == "services")
    {
        query.prepare("DELETE FROM " + baseName + ".services WHERE ID_Parent != \'\'");
        if(!query.exec())
        {
            QString err = "ОШИБКА!! Ну удалось очистить таблицу " + tableName + "\n"
                        + "Операция прервана!\n" + query.lastError().text();
            emit messages(err);
            return false;
        }
    }
    query.prepare("DELETE FROM " + baseName + "." + tableName);
    if(!query.exec())
    {
        QString err = "ОШИБКА!! Ну удалось очистить таблицу " + tableName + "\n"
                    + "Операция прервана!\n" + query.lastError().text();
        emit messages(err);
        return false;
    }
    return true;
}

