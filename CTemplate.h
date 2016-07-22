#ifndef CTEMPLATE_H
#define CTEMPLATE_H

#include <QString>;
#include <QPixmap>

class CTemplate
{
public:
    CTemplate(QString a_sPath, QString a_sPicturePath,
              bool a_bIsGame = false, QString a_sName = "", QString a_sDescription = "");
    bool IsGame();
    bool HasDescription();
    bool HasName();
    const QString& GetName();
    const QString& GetDescription();
    const QString& GetPath();
    const QPixmap& GetImage();

private:
    QString m_sPath;
    QString m_sName;
    QString m_sDescription;
    bool m_bIsGame;
    QPixmap m_imPicture;


};

#endif // CTEMPLATE_H
