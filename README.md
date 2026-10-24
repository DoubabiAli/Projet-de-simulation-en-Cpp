# Simulateur IoT

Projet C++ qui simule un serveur
recevant les mesures de plusieurs capteurs IoT et les enregistre dans des
fichiers CSV.

## Fonctionnalités

- Simulation de quatre types de capteurs :
  - température ;
  - humidité ;
  - niveau sonore ;
  - luminosité.
- Génération de valeurs aléatoires pour chaque mesure.
- Affichage des mesures dans la console.
- Enregistrement des mesures dans un fichier CSV par type de capteur.
- Attribution automatique d'un identifiant unique à chaque capteur.
- Exécution périodique de la simulation grâce à un ordonnanceur
  (`Scheduler`).

## Architecture

```text
Projet/
├── CMakeLists.txt
├── Headers/
│   ├── HumiditySensor.h
│   ├── LightSensor.h
│   ├── Scheduler.h
│   ├── Sensor.h
│   ├── Server.h
│   ├── SoundSensor.h
│   └── TemperatureSensor.h
├── Sources/
│   ├── HumiditySensor.cpp
│   ├── LightSensor.cpp
│   ├── Scheduler.cpp
│   ├── Sensor.cpp
│   ├── Server.cpp
│   ├── SoundSensor.cpp
│   ├── TemperatureSensor.cpp
│   └── main.cpp
├── Humidity.csv
├── Light.csv
├── Sound.csv
└── Temperature.csv
```

### Rôle des principales classes

- `Sensor` : classe abstraite commune à tous les capteurs.
- `TemperatureSensor`, `HumiditySensor`, `SoundSensor` et `LightSensor` :
  implémentations concrètes des capteurs.
- `Server` : affiche les mesures et les écrit dans les fichiers CSV.
- `Scheduler` : conserve la liste des capteurs et déclenche leurs mises à
  jour.
- `main.cpp` : initialise le serveur, les capteurs et l'ordonnanceur.

## Prérequis

- Un compilateur compatible avec **C++17** :
  - Visual Studio 2019 ou ultérieur sous Windows ;
  - GCC ou Clang sous Linux/macOS.
- CMake 3.10 ou ultérieur.

## Compilation avec CMake

Depuis le dossier `Projet` :

```bash
cmake -S . -B build
cmake --build build
```

Sous Windows avec Visual Studio, il peut être nécessaire de préciser le
générateur :

```powershell
cmake -S . -B build -G "Visual Studio 17 2022"
cmake --build build --config Release
```

## Exécution

Depuis le dossier `Projet` :

### Windows

Avec une compilation Visual Studio :

```powershell
.\build\Release\simulator.exe
```

Avec une compilation MinGW ou Ninja :

```powershell
.\build\simulator.exe
```

### Linux/macOS

```bash
./build/simulator
```

La simulation s'exécute en continu. Pour l'arrêter, utilisez `Ctrl+C`.

## Format des données

Chaque mesure est ajoutée au fichier CSV correspondant au type du capteur :

- `Temperature.csv`
- `Humidity.csv`
- `Sound.csv`
- `Light.csv`

Les fichiers utilisent le séparateur `;` et contiennent les colonnes
suivantes :

```text
Date;Heure;SensorId;SensorType;Valeur
```

Les mesures sont également affichées dans la console sous la forme :

```text
Server = Server1 ;; Sensor = 1 ;; Type = Temperature ;; Valeur = 42.500000
```

## Fonctionnement de la simulation

Au démarrage, le programme :

1. crée un serveur nommé `Server1` ;
2. crée un capteur de chaque type ;
3. ajoute les capteurs à l'ordonnanceur ;
4. met à jour chaque capteur toutes les secondes ;
5. transmet chaque valeur au serveur pour affichage et sauvegarde.

Les valeurs générées sont aléatoires :

- température et humidité : valeur décimale comprise entre `0` et `100` ;
- niveau sonore : entier compris entre `0` et `100` ;
- luminosité : `true` ou `false`.

## Nettoyage des fichiers générés

Les fichiers CSV sont ouverts en mode ajout. Pour repartir d'une simulation
vierge, supprimez ou videz les fichiers suivants avant de relancer le
programme :

```text
Humidity.csv
Light.csv
Sound.csv
Temperature.csv
```

Ils seront recréés avec leur en-tête lors de la prochaine écriture.
