SCRIPT=$(readlink -f $0)
SCRIPTPATH=`dirname $SCRIPT`
#cat $SCRIPTPATH/desktop.conf > LudoMuseEditor.desktop
echo "[Desktop Entry]" > LudoMuseEditor.desktop 
echo "Comment=Editeur de scènes pour le moteur de jeux collaboratifs LudoMuse" >> LudoMuseEditor.desktop 
echo "Terminal=false" >> LudoMuseEditor.desktop 
echo "Name=LudoMuse Editor" >> LudoMuseEditor.desktop
echo "Exec=$SCRIPTPATH/LudoMuseEditor" >> LudoMuseEditor.desktop 
echo "Type=Application" >> LudoMuseEditor.desktop 
echo "Icon=$SCRIPTPATH/resources/icon.png" >> LudoMuseEditor.desktop
echo "Voudriez-vous créer un lanceur dans le menu d'applications de votre système ? o/n"
read ANSWER

case $ANSWER in
    "o" | "O" | "oui" | "y" | "Y" | "yes")
	echo "création d'un lanceur système ..."
	cp $SCRIPTPATH/LudoMuseEditor.desktop $HOME/.local/share/applications
	MESSAGE="Vous pouvez directement lancer LudoMuseEditor depuis votre menu d'applications"	;;
    *)
	echo "aucun lanceur ne sera créé"
	MESSAGE="Vous pouvez lancer le fichier LudoMuseEditor.desktop"
	;;
esac

echo
echo
echo "==============  Installation terminée avec succès ===================="
echo
echo $MESSAGE
echo 
