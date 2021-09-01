# Programme IOT-end2end-arduino

Ce programme est utilisé dans le cadre de la démonstration Maquette IoT End-To-End.
Il permet d'afficher la direction que doit prendre le prochain véhicule qui arrivera à son niveau.


## Composants électroniques nécessaires

- Arduino Uno Wifi Rev2  ([voir la référence](https://store.arduino.cc/arduino-uno-wifi-rev2))
- Grove - RGB LED Matrix w/Driver ([voir la référence](https://wiki.seeedstudio.com/Grove-RGB_LED_Matrix_w-Driver/))
- Cable Grove 

## Librairies Arduino nécessaires

```
#include "grove_two_rgb_led_matrix.h"
#include <ChainableLED.h>
#include "arduino_secrets.h"
#include <LiveObjects.h>
```

## Lancer la démonstration

Il n'y a pas d'interaction manuelle à avoir avec l'Arduino.

1. Quelques secondes après le branchement de l'Arduino, il affiche un carré vert. Cela indique qu'il s'est bien connecté au Wifi. 
2. Il suffit ensuite de démarrer la démonstration Maquette IoT End-To-End ([Code ici](https://github.com/Smart-Stadium/IOT-end2end-maquette)) en cliquant sur "Lancer la démonstration" sur l'interface Web du CIC Démo: Maquette ([Code ici](https://github.com/Smart-Stadium/django-demos-cic)) 


## Point technique

Le programme permet d'utiliser LiveObjects afin de communiquer avec l'Arduino en utilisant le protocole MQTTS. 
C'est le programme Maquette sur le raspberry qui permet d'envoyer les commandes vers l'Arduino. 

Il permet de souscrire à trois commandes différentes sur LiveObjects.
```
lo.addCommand("continueOnTheRoad", goStraight);
lo.addCommand("stopAtLoadingDock", goToLoadingDock);
lo.addCommand("turnOffMatrixLeds", turnOffMatrixLeds);
```

1. La commande `goStraight` exécute la fonction de callback `continueOnTheRoad()` : elle affiche une flèche bleue indiquant d'aller tout droit.
Elle est lancée dès le début de l'exécution du programme maquette puis après l'arrivée d'un camion sur le quai de chargement.

2. La commande `goToLoadingDock` exécute la fonction de callback `stopAtLoadingDock()` : elle affiche une rouge indiquant d'aller à droite, sur le quai de chargement.
Elle est lancée dès qu'un camion est détecté et classifié (loaded_truck ou empty_truck) par la skill sur le Hilens [Code ici](https://github.com/Smart-Stadium/skill_hilens_aac_truck_and_van_detection) 


3. La commande `turnOffMatrixLeds` exécute la fonction de callback `turnOffMatrixLeds()` : elle permet de ne plus rien afficher sur la matrice Led.
Elle est lancée dès que l'exécution du programme Maquette est terminée. 

## Problèmes possibles

Dans certains cas, il peut être utile de débrancher l'Arduino puis le rebrancher car il arrive qu'il perde la connexion Wifi. 

Aussi, il peut arriver qu'il y ait un bug du côté du programme Maquette, et qu'il y ait de ce fait un cumul de commandes envoyées à l'Arduino, restées au statut "Pending". Dans ce cas, il suffit de se rendre sur [la page de commandes de l'Arduino](https://liveobjects.orange-business.com/#/devices/view/urn:lo:nsid:arduinowifi:maquettee2e/commands?groupPath=%2FCIC) (StreamId pour trouver l'équipement concerné: urn:lo:nsid:arduinowifi:maquettee2e) et supprimer les commandes reçues et posant problème. 


