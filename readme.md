# Victory-Forge : Simulation de tournois de football en langage C

Le projet Victory-Forge vise à créer un logiciel en langage C permettant la simulation de tournois de football. Le programme prend en charge un nombre d'équipes fixé à l'avance, collecte les données d'entrée à partir d'un fichier texte, et exécute chaque match sur un processus distinct pour assurer une exécution fluide du tournoi.

## MIT License
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)  

## Fonctionnalités

### 1. Gestion des équipes

Le programme prend en charge un nombre d'équipes égal à une puissance de 2 et récupère les noms des équipes à partir d'un fichier texte.

### 2. Exécution concurrente des matchs 

Chaque match est exécuté sur un processus distinct, garantissant une exécution fluide du tournoi. La synchronisation entre les tours est assurée.

### 3. Affichage en temps réel 

Le programme affiche en temps réel le score de chaque match ainsi que les noms des équipes impliquées, permettant de suivre plusieurs matchs simultanément.

### 4. Fichier de sortie

À la fin du programme, un fichier texte resultats.txt est généré, récapitulant le déroulement du tournoi, les scores de chaque match, et le vainqueur.



L'exécutable sera généré sous le nom tournoi.

## Procédure de lancement du programme

### Compilation avec Makefile

Assurez-vous que le compilateur C est installé. Si ce n'est pas le cas, vous pouvez l'installer avec la commande suivante :

```bash
sudo apt install make
```

Ensuite, pour compiler et exécuter le programme, utilisez la commande suivante dans le terminal du dossier contenant les fichiers "tournoi.c" et "Makefile":

```bash
make run
```

### Compilation sans Makefile

Si vous ne souhaitez pas utiliser le Makefile, vous pouvez compiler le programme avec les commandes suivantes :

```bash
gcc -o tournoi tournoi.c -lpthread
./tournoi [chemin_fichier_equipes]
```

Si aucun chemin de fichier n'est fourni, le programme utilisera le fichier par défaut equipes.txt.

## Généralités du programme

Après le lancement du programme, vous assisterez à la simulation des tours du tournoi. Chaque tour se compose de quatre matchs, où deux équipes s'affrontent par match. À la fin de chaque tour, l'équipe avec le score le plus bas est éliminée pour le tour suivant.

## Statistiques du tournoi

Une fois tous les tours terminés, le programme affiche un récapitulatif complet du tournoi, montrant les scores obtenus par chaque équipe. L'équipe avec le meilleur score est désignée comme gagnante du tournoi. Les résultats sont également exportés dans un fichier nommé "resultats.txt" pour une consultation ultérieure.



## Technologies utilisées

- C

## Contribution

Pour toute contribution ou réutilisation de ce projet, veuillez mentionner les noms des développeurs de ce projet : [DAMODARANE Jean-Baptiste](https://github.com/JeanBaptiste02) et [ELUMALAI Sriguru](https://github.com/Sriguru95). Pour collaborer, veuillez les contacter pour obtenir leur consentement formel.
