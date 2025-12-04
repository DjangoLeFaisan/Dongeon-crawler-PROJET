# Environnement de développement – Projet Raylib (C)

Ce projet utilise **raylib** pour le rendu graphique et est développé en **C**, avec une compilation via **Makefile**.  
Ce document explique comment installer l’environnement de développement sur :

- **Windows**
- **macOS (Apple Silicon & Intel)**

---

## Prérequis communs

- Un éditeur de code (recommandé : **Visual Studio Code**)
- Un terminal (PowerShell / Terminal macOS)
- Le projet cloné sur votre machine

```bash
git clone <url-du-depot>
cd raylib-board-game
````

---

# ✅ Installation sous macOS

### 1. Installer les outils de compilation Apple

```bash
xcode-select --install
```

---

### 2. Installer Homebrew (si ce n’est pas déjà fait)

```bash
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Puis suivez les instructions affichées pour activer `brew` dans votre shell.

---

### 3. Installer raylib et pkg-config

```bash
brew install raylib pkg-config
```

---

### 4. Vérifier l’installation

```bash
pkg-config --libs raylib
```

Une ligne de librairies doit s’afficher (sinon, l’installation a échoué).

---

### 5. Compiler le projet

À la racine du projet :

```bash
make
```

---

### 6. Lancer le programme

```bash
./game
```

---

# ✅ Installation sous Windows (via MSYS2 – méthode recommandée)

Cette méthode fournit un environnement Linux-like propre pour compiler en C sous Windows.

---

## 1. Installer MSYS2

Téléchargement :

> [https://www.msys2.org](https://www.msys2.org)

Installer avec les options par défaut.

---

## 2. Mettre à jour MSYS2

Ouvrir **MSYS2 MINGW64** et taper :

```bash
pacman -Syu
```

Fermer la fenêtre si demandé, puis rouvrir et relancer :

```bash
pacman -Syu
```

---

## 3. Installer le compilateur + raylib

Toujours dans **MINGW64** :

```bash
pacman -S mingw-w64-x86_64-gcc
pacman -S mingw-w64-x86_64-gdb
pacman -S mingw-w64-x86_64-raylib
pacman -S mingw-w64-x86_64-pkg-config
pacman -S make
```

---

## 4. Vérifier l’installation

```bash
pkg-config --libs raylib
gcc --version
gcdb --version
```

---

## 5. Compiler le projet

Dans le dossier du projet (depuis le terminal MSYS2) :

```bash
make
```

---

## 6. Lancer le programme

```bash
./game.exe
```

---

# ✅ Utilisation avec VS Code

Recommandé pour tous :

Extensions à installer :

* **C/C++ (Microsoft)**

Fichiers déjà fournis dans le projet :

* `.vscode/tasks.json` → compilation
* `.vscode/launch.json` → exécution + debug

---

### Raccourcis utiles

| Action         | Raccourci          |
| -------------- | ------------------ |
| Compiler       | `Ctrl + Shift + B` |
| Lancer + Debug | `F5`               |

---

# ✅ Commandes utiles

| Action            | Commande     |
| ----------------- | ------------ |
| Compiler          | `make`       |
| Nettoyer          | `make clean` |
| Compiler + lancer | `make run`   |

---

# ✅ Problèmes courants

### ❌ Le programme ne se met pas à jour dans VS Code

Ou si vous observez une erreur récurrente et étrange...
C'est peut-être parceque le programme exécuté ne correspond plus au code source.

Donc dans le doute... Reboot !
Tapez la commande `make clean` et recompilez le tout.

---

### ❌ Correction de l’erreur IntelliSense sur `#include "raylib.h"`

Si l’éditeur VS Code affiche une erreur sur `#include "raylib.h"` alors que la compilation fonctionne, c’est normal :  
**IntelliSense ne connaît pas automatiquement les chemins d’inclusion fournis par `pkg-config`.**  
Il faut les ajouter manuellement.

---

#### ✅ macOS

1. Ouvrir la palette de commandes :

```
Cmd + Shift + P
```

2. Taper :

```
C/C++: Edit Configurations (JSON)
```

3. Dans le fichier `.vscode/c_cpp_properties.json`, ajouter (ou vérifier) :

```json
{
"configurations": [
 {
   "name": "macOS",
   "compilerPath": "/usr/bin/clang",
   "cStandard": "c17",
   "intelliSenseMode": "macos-clang-arm64",
   "includePath": [
     "${workspaceFolder}/include",
     "/opt/homebrew/include"
   ]
 }
],
"version": 4
}
````

4. Redémarrer VS Code.

✅ L’erreur sur `raylib.h` doit disparaître.

---

#### ✅ Windows (MSYS2 / MinGW64)

1. Ouvrir la palette de commandes :

```
Ctrl + Shift + P
```

2. Taper :

```
C/C++: Edit Configurations (JSON)
```

3. Dans `.vscode/c_cpp_properties.json` :

```json
{
  "configurations": [
    {
      "name": "Windows",
      "compilerPath": "C:/msys64/mingw64/bin/gcc.exe",
      "cStandard": "c17",
      "intelliSenseMode": "windows-gcc-x64",
      "includePath": [
        "${workspaceFolder}/include",
        "C:/msys64/mingw64/include"
      ]
    }
  ],
  "version": 4
}
```

4. Redémarrer VS Code.

✅ L’erreur disparaît et l’auto-complétion raylib fonctionne.

---

Bon développement ! 🎮

