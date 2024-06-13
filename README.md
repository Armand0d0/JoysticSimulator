# JoysticSimulator

J'ai codé un module noyau `JoysticSimulModule` qui simule un joystick usb et qui est programmable par un processus utilisateur.
Je m'en sert pour réaliser un série d'entrées de façon systématique dans Trackmania.

- `JoysticSimulUserProg` réalise une série d'entrées prédéterminées.
- `userProgMan` permet de controler le joystick virtuel manuelement depuis le terminal.
- `ScreenWatcher` lit la vitesse affichée à l'écran dans trackmania et affiche un graphe de la vitesse en fonction du temps.
- `MouseJs` permet d'utiliser la souris comme un joystick grâce a ce module.
- `InputRecorder` utilise X11 et Xlib pour enregistrer les donnes d'entree pendant une partie.

## Usage

Pour compiler le module  :
```sh
make 
```
Pour compiler et insérer le module dans le noyau :
```sh
make ins
```
On suppose ensuite que le module est inséré.

Pour recompiler le module :
```sh
make mod
```
Pour compiler et exécuter "JoysticSimulUserProg.c" : 
```sh
make prog
```
Pour recompiler le module, compiler et exécuter "JoysticSimulUserProg.c" : 
```sh
make modprog
```
Pour supprimer le module du noyau :
```sh
make rm
```
Pour supprimer les fichiers de compilation du module :
```sh
make clean
```


