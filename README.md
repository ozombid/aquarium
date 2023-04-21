# Projet Réseaux
Le but du projet est de réaliser un aquarium centralisé de poissons. Chaque équipe devra programmer  un “programme d’affichage” et un “programme contrôleur”. Le contrôleur permet de centraliser la gestion de l’aquarium, il pourrait être supprimé dans une version distribuée. Chaque poisson a pour mission de se déplacer dans l’aquarium selon un modèle de mobilité prédéfini (vitesse, direction, etc.). Le programme contrôleur s'occupe d'informer les programmes d’affichage (Vue) de la position actuelle des poissons. On optera pour une programmation suivant le modèle MVC. Le modèle contient les données sur un poisson, sa taille, son pattern de mobilité, etc. La vue, c’est l’IHM, elle représente l’affichage graphique de l’aquarium. Le contrôleur permet de faire le lien entre le modèle et la vue et il permet de contrôler les données à transmettre d’un affichage à l’autre selon la topologie de l’aquarium.

Ce projet est actuellement conçu pour être compilé et exécuté sur un système Linux.

## Serveur Controleur
* Dans la repertoire /controleur, compiler le serveur :
```
make
```
* Lancer le serveur :
```
make server 
```
Par défaut, le port est 8000, pour le parametrer, lancer :
```
make server PORT=9898
```

## Client Afficheur Administrateur
* Dans la repertoire /afficheur_admin, compiler le client:
```
make
```
* Lancer le client :
```
make client
```
Par défaut, l'adresse IP du serveur est localhost, et le port est 8000, pour les parametrer, lancer :
```
make client IP=127.0.0.1 PORT=9898
```

## Commandes Administrateur
Il ne faut pas forcer l'arret du client pour éviter la segmentation fault, Par contre utiliser la commande bye
* ### Arrêter l'exécution
```
$ bye
```
* ### Arrêter l'exécution du serveur
```
$ down
```
* ### Charger un aquarium
Au début de la communication, aucun aquarium n'est chargé.
Tout aquarium chargé doit être existant.
Par défaut, un aquarium aq1 existe.
Permet de charger une topologie:
```
$ load aq1
    -> Aquarium loaded (4 display views)
```
Les commandes qui suivent ne sont possibles qu'après un chargement d'un aquarium.
* ### Afficher la topologie
Affiche les dimensions de l'aquarium ainsi que les vues actuellement configurées :
```
$ show 
```
* ### Ajouter une vue
Permet d'ajouter une nouvelle vue à la topologie :
```
$ add view N5 400x400+400+200
    -> View added
```
* ### Supprimer une vue
Permet de supprimer une vue existante :
```
$ del view N5
    -> View N5 deleted
```
* ### Enregistrer la topologie
Permet d'enregistrer la topologie actuelle dans un fichier de configuration :
```
$ save aq2
    -> Aquarium saved (5 display views)
```
Un enregistrement n'est pas un ajout d'un aquarium, mais une modification de cet aquarium.
Après un enregistrement, aucun aquarium n'est chargé.
* ### Historique d'Aquarium enregistrés
Permet d'afficher les aquariums enregistrés y compris l'aquarium initiale aq1 :
```
cat aquarium.txt
```
Ce fichier est supprimé après l'arrêt du serveur.


## Commandes Afficheur
Il ne faut pas forcer l'arret du client pour éviter la segmentation fault !, Par contre utiliser la commande bye
* ### Arrêter l'exécution
```
$ bye
```
Un afficheur ne peut pas arrêter l'exécution du serveur, au lieu de cela il s'arrête.
* ### Charger un aquarium
Permet de charger une topologie :
```
$ load aq1
    -> Aquarium loaded (4 display views)
```
Les commandes qui suivent ne sont possibles qu'après un chargement d'un aquarium.
* ### Prendre une vue
Permet de pointer sur une vue si elle est libre :
```
$ hello .
    -> Greeting N1
```
* ### Afficher la topologie
Affiche les poissons et leurs dimensions et états de l'aquarium :
```
$ status 
```
* ### Ajouter un poisson
Permet d'ajouter un nouveau poisson à la topologie :
```
$ add fish F1 400x400+400+200 random
    -> Fish F1 added
```
* ### Supprimer un poisson
Permet de supprimer un poisson s'il existe :
```
$ del fish F1
    -> Fish F1 deleted
```
* ### Lancer un poisson
Permet de lancer un poisson à mouvoir dans l'aquarium :
```
$ start F1
    -> Fish F1 started
```


