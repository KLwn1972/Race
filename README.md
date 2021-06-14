
# Vorbereitung

* Version prüfen
    ``` 
    git --version
    ``` 
* Identität angeben:
    ``` 
    $ git config --global user.name ”Max Mustermann"
    $ git config --global user.email max.mustermann@beispiel.de
    ``` 
## Neues Repository (Sandbox) erstellen
* Neues lokales Verzeichnis erstellen, dorthin wechseln
    ``` 
    git init
    ``` 
* Ein Repo auschecken (Arbeitskopie erstellen)
    * Von einem lokalen Repo
        ```
        git clone /pfad/zum/repository
        ```
    * Von einem entfernten Repo:
        ```
        git clone benutzername@host:/pfad/zum/repository
        ```
* Ein existierendes lokales Repo an ein entferntes (im Nachhinein) verbinden
    ```
    git remote add origin <server>
    ```
## Änderungen vornehmen
* Änderungen vom entfernten Repo holen
    ```
    git fetch origin
    git pull: macht ein fetch vom remote UND ein merge mit Arbeitskopie
    ```
* Zum Stage/Index hinzufügen:
    ```
    git add <dateiname> bzw. git add *
    git add -A (wie All) stages Alle
    git add . stages neue und modifizierte, aber nicht gelöschte
    git add -u (wie update) stages modifizierte und gelöschte, ohne neue
    ```
* Stage/Index zum lokalen Repo zufügen (Bestätigung, neues HEAD):
    ```
    git commit -m "Kommentar”
    ```
* Änderungen zum entfernten Repo senden:
    ```
    git push origin master (master mit beliebigem branch ersetzen)
    ```
## Branch, switch & checkout
    git branch Zeigt alle branches in Repo (git branch --list)
    git branch[Optionen] Zum erstellen, zeigen, und löschen von Zweigen
    git switch <branch> Wechselt Zweige
    git switch -c <branch> Erstellt einen neuen Branch und wechselt dorthinein
    git checkout –b <branch> Erstellt einen neuen Branch und wechselt dorthinein
    git checkout [Optionen] Aktualisiert Dateien in dem Arbeitsverzeichnis so, dass sie
    
