# LudoMuseEditor 

| Linux / OSX  | Windows    |
|:-----------:|:-----------:|
|[![Build Status](https://travis-ci.org/ludomuse/LudoMuseEditor.svg?branch=master)](https://travis-ci.org/ludomuse/LudoMuseEditor) | [![Build status](https://ci.appveyor.com/api/projects/status/wwknl987osm43uph?svg=true&branch=master)](https://ci.appveyor.com/project/IHMTEK/ludomuseeditor?branch=master) |

## Téléchargement 
Vous pouvez télécharger la dernière version stable de LudoMuseEditor en cliquant sur le lien ci-dessous : 
## [Télécharger LudoMuseEditor version stable](https://github.com/ludomuse/LudoMuseEditor/releases/latest)

Si vous êtes sur linux, vous devez pour le moment avoir Qt5 installé sur votre machine : 

``
sudo apt-get install libqt5opengl5
``

pour les système basés sur debian/ubuntu devrait résoudre les dépendances nécessaires.

Les version de développement (nightlies) sont disponibles ici :

[Parcourir les versions de développement linux et mac](https://ihmtek-services.com/files/LudoMuse/releases/nightlies/LudoMuseEditor)


[Dernière version de développement windows](https://ci.appveyor.com/project/IHMTEK/ludomuseeditor/branch/master/artifacts)


## Dépendances


### Quazip


LudoMuseEditor s'appuie sur la librarie Quazip pour la compression et décompression d'archives LudoMuse.

Sur linux et mac installez la librairie avec votre gestionaire de paquets.

Sur windows editez LudoMuseEditor.pro et changez les variables QUAZIP_INCLUDEPATH QUAZIP_LIBPATH et ZLIB_INCLUDEPATH.


## Compilation


Pour compiler utiliser qmake ou Qt Creator avec le fichier LudoMuseEditor.pro


## Déploiement


copiez/collez le contenu de buildFiles à côté de l'exécutable LudoMuseEditor.
copiez/collez également LudoMuse.exe ainsi que ses librairies dans le dossier du projet pour pouvoir utiliser le simulateur.
(Compilé à partir du projet LudoMuse pour la plateforme PC que vous ciblez)
Attention cependant sur linux à bien compiler LudoMuse en debug car compilé en release la connexion entre les 2 entités échoue).

Il faut ensuite copier coller toutes les bibliothèques logicielles dont LudoMuseEditor dépend.

- Sur windows cela peut être fait avec l'outil de déploiement windeployqt fourni à l'installation de Qt.
- Sur mac il faut coller les principaux frameworks Qt utilisés : Qt5Core, Qt5Widgets, Qt5Gui et Qt5OpenGL dans le dossier de frameworks du .app
- Sur Linux l'idéal est de créer un sous dossier lib dans le dossier de déploiement. Ce sous-dossier permettra de stocker toutes les librairies dont dépend l'éxécutable dedans. Le fichier .pro référence déjà ce dossier comme source pour les dépendance, vous n'avez pas besoin de modifier l'exécutable ou de faire u script pour ça.
Les dépendances peuvent être listées avec la commande

``
$ ldd LudoMuseEditor
``
Vous pouvez trouver lesquelles nécessitent d'être copiées à partir du dossier lib de votre installation LudoMuse.
