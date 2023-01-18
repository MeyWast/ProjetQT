# ProjetQT

## Pour compiler sur un IDE
  1. Créer un projet sur votre IDE (Clion de préfèrence)
  2. Copier les fichiers à l'intérieur
  3. Utiliser la compilation automatique de votre IDE (avec MobaXTerm) et normalement tout devrait bien se passer(envoyer moi un mail meywast@gmail.com, si il se passe quelque chose)

## Pour compiler sur Debian
  apt packages: 
```
sudo apt-get install qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools qtmultimedia5-dev libqt5multimedia5-plugins -y
```
  Compiling:
```sh
cd directory/
qmake -project # make sure to add QT field in the generated .pro file 
qmake
make

```
Amusez vous !

Certains mécaniques du jeu :
- Possibilité de s'aggriper sur les coins des murs
- Wall Jump
- Pluie de météorite
- Bloc qui disparait
- Crabe qui se déplace

Un gif est mis dans le fichier et vous donne un apercu du jeu
