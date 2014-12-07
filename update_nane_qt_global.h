#ifndef UPDATE_NANE_QT_GLOBAL_H
#define UPDATE_NANE_QT_GLOBAL_H

#include <QStringList>
#include <QDateTime>

namespace UpdateNameQt {
static const QStringList endStringList = QStringList() << "_"
                                                       << "."
                                                       << ","
                                                       << "*"
                                                       << "-";
static QString randomEndString()
{
    return endStringList[QDateTime::currentMSecsSinceEpoch() % (endStringList.size() - 1)];
}
}

#endif // UPDATE_NANE_QT_GLOBAL_H
