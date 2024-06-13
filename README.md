# JoysticSimulator

J'ai codé un module noyeau "JoysticSimulModule" qui simule un joystick usb et qui est programmable par un processus utilisateur.
Je m'en sert pour réaliser un série d'entrées de façon systématique dans Trackmania.

"JoysticSimulUserProg" réalise une série d'entrées prédéterminées.
"userProgMan" permet de controler le joystick virtuel manuelement depuis le terminal.
"ScreenWatcher" lit la vitesse affichée à l'écran dans trackmania et affiche un graphe de la vitesse en fonction du temps.
"MouseJs" permet d'utiliser la souris comme un joystick grâce a ce module.
"InputRecorder" utilise X11 et Xlib pour enregistrer les donnes d'entree pendant une partie.

## Usage

```sh
make all 
```


