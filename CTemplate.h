#ifndef CTEMPLATE_H
#define CTEMPLATE_H

#include <QString>;

class CTemplate
{
public:
    CTemplate(QString a_sPath, bool a_bIsGame = false);
    bool isGame();
    const QString& GetPath();

private:
    QString m_sPath;
    bool m_bIsGame;

};

#endif // CTEMPLATE_H
