#include "ETypes.h"

#include <QString>
#include <QStringList>

namespace ETypes
{

QStringList GetExtensionsList(Type a_eType)
{
    QStringList lExtensions;
    switch (a_eType)
    {
    case Image:
        lExtensions << "png" << "jpg" << "jpeg";
        break;
    case Sound:
        lExtensions << "mp3" << "wav";
        break;
    case Font:
        lExtensions << "ttf";
        break;
    default:
        break;
    }
    return lExtensions;
}

Type TypeFromString(const QString& a_sType)
{
    QString sType = a_sType.toLower();
    if (sType == "image")
    {
        return Image;
    }
    else if (sType == "sound")
    {
        return Sound;
    }
    else if (sType == "font")
    {
        return Font;
    }
    else
    {
        return Unknown;
    }
}

Type TypeFromExtension(const QString& a_sExtension)
{
    QString sExtension = a_sExtension.toLower();
    if (sExtension == "png" || sExtension == "jpg" || sExtension == "jpeg")
    {
        return Image;
    }
    else if (sExtension == "mp3" || sExtension == "wav")
    {
        return Sound;
    }
    else if (sExtension == "ttf")
    {
        return Font;
    }
    else
    {
        return Unknown;
    }
}

QString TypeToString(Type a_eType)
{
    switch (a_eType)
    {
    case Image:
        return "image";
    case Sound:
        return "sound";
    case Font:
        return "font";
    default:
        return "unknown";
    }
}

QString GetFilter(Type a_eType)
{
    switch (a_eType)
    {
    case Image:
        return "(*.jpeg, *.jpg, *.png)";
    case Sound:
        return "(*.mp3, *.wav)";
    case Font:
        return "(*.ttf)";
    default:
        return "";
    }
}

}

