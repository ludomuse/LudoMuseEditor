Dépendances
===========

Quazip
-------

LudoMuseEditor s'appuie sur la librarie Quazip pour la compression et décompression d'archives LudoMuse.

Sur linux et mac installez la librairie avec votre gestionaire de paquets.

Sur windows editez LudoMuseEditor.pro et changez les variables QUAZIP_INCLUDEPATH QUAZIP_LIBPATH et ZLIB_INCLUDEPATH.


Compilation
===========

Pour compiler utiliser qmake ou Qt Creator avec le fichier LudoMuseEditor.pro


Déploiement
===========

copiez/collez le contenu de buildFiles à côté de l'exécutable LudoMuseEditor.
copiez/collez également LudoMuse.exe ainsi que ses librairies dans le dossier du projet pour pouvoir utiliser le simulateur.
(Compilé à partir du projet LudoMuse pour la plateforme PC que vous ciblez)
Attention cependant sur linux à bien compiler LudoMuse en debug car compilé en release la connexion entre les 2 entités échoue).

