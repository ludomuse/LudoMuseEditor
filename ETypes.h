#ifndef ETYPES_H
#define ETYPES_H

#include <QString>
#include <QStringList>

namespace ETypes {
enum Type
{
    Image,
    Sound,
    Font,
    Unknown
};

QStringList GetExtensionsList(Type a_eType);
Type TypeFromString(const QString& a_sType);
Type TypeFromExtension(const QString& a_sExtension);
QString TypeToString(Type a_eType);
QString GetFilter(Type a_eType);
}


#endif // ETYPES_H
