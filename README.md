# Projet Analyse Syntaxique

## Prérequis

### Bison et Flex

Pour faire fonctionner le projet, flex et bison sont requis. 
Leurs versions recommandés sont :

- Flex 2.6.4
- Bison 2.7

**Installation recommandé** :

Pour installer bison et flex sous windows, vous pouvez suivre ces étapes :
> 1) Télécharger [Win flex-bison](https://sourceforge.net/projects/winflexbison/).
> 2) Extraire l'archive dans l'emplacement de votre choix.
> 3) Ajouter une variable d'environement PATH vers le dossier extrait. ([Tutoriel](https://www.malekal.com/comment-modifier-la-variable-path-sous-windows-10-11/))
> 4) Renommez win_bison.exe par bison.exe et win_flex.exe par flex.exe à l'intérieur du dossier.
> 5) Redémarrez windows.

Verifiez que tout fonctionne avec les commandes :
```bash
bison --version
flex --version
```


### Clang

Les scripts de compilation utilisent clang. 
La version *19* ou supérieur est recommandé.

Pour installer clang sous windows, vous pouvez suivre ces étapes : 
> 1) Téléchargez *'LLVM-x.x.x-win64.exe'* sur le [github officiel](https://github.com/llvm/llvm-project/releases/tag/llvmorg-21.1.8)
> 2) Lancez l'executable et suivez le consignes d'installation.
> 3) Redémarrez windows.

Verifiez que tout fonctionne avec la commande :
```bash
clang --version
clang++ --version
```

## Compiler le projet

Pour compiler le projet sous windows, lancez simplement le script nommé *'Compile.bat"*.