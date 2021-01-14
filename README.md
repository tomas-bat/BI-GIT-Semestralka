# Dokumentace k semestrální práci BI-GIT

Jméno: Tomáš Batěk
E-mail: batekto2@fit.cvut.cz

## 0. Inicializace (vzor)
Postup:
- Naklonuji vzdálený repozitář pro odevzdání,
- přidám vzdálené repozitáře s obsahem.

Použité příkazy:
```sh
git clone fit@gitc.cz:ZS2020/repo/novakjos_sem.git
for name in alice bob charlie
do
  git remote add $name https://gitlab.fit.cvut.cz/BI-GIT/semestral/$name.git
  git fetch $name
done
```

## 1. Spojení repozitářů
Postup:

- ze vzoru jsem dostal fetchnuté repozitáře Alice, Boba a Charlieho
- každý repozotář má jenom jednu větev (master), takže jsem pro každý master vytvořil vlastní větev (alice, bob, charlie)
- přepnul jsem do větve alice a mergnul bobův master, to způsobilo konflikty v src/dfi.sh - nechal jsem obě funkcionality a commitnul
- poté jsem mergnul charlieho, při tom žádné konflikty nezvnikly

Použité příkazy pro spojení repozitářů:
```sh
git branch alice alice/master
git branch bob bob/master
git branch charlie charlie/master
git checkout alice
git merge bob
git add src/dfi.sh
git commit
git merge charlie
```
- pro odstranění merge commitů jsem udělal rebase interactive na commit před narušením linearity
- musel jsem vyřešit konflikty u `Makefile` a `src/dfi.sh`
- pro odstranění fixupů jsem udělal rebase interactive na ten stejný commit a squasnul jsem fixup! commity
- nakonec jsem přejmenoval větev a pushnul ji

Použité příkazy pro odstranění `merge` a `fixup!` commitů:
```sh
git rebase -i 221e11b
git add Makefile
git rebase --continue
git add src/dfi.sh
git rebase --continue
git rebase -i 221e11b
git branch -m merged
git push origin merged
```


## 2. CRLF
Postup:

- soubor .gitattributes jsem navrhl tak, aby v repozitáři normalizoval všechny soubory, které považuje za text
- také jsem do .gitattributes přímo specifikoval, aby soubory .ics měly v repozotáři CRLF konce řádků
- když jsem chtěl provést změnu v celé historii, použil jsem příkaz `git filter-branch`
- soubor .gitattributes není verzovaný a nikde v historii není, takže jsem ho musel při každém kroku vytvořit,
než jsem mohl normalizovat

Návrh obsahu souboru `.gitattributes`:
```
* text=auto
*.ics text=crlf
```

Použité příkazy pro normalizaci konců řádků:
```sh
git checkout -b normalised
git filter-branch --tree-filter 'echo "* text=auto" > .gitattributes && echo "*.ics text=crlf" >> .gitattributes && git add --renormalize . && rm -f .gitattributes'
git push origin normalised
```

## 3. Osobní data
Postup:

- soubory `events.ics` a `events.ics.peek` byly přidány v commitu `a177898` a od té doby nebyly upraveny
- použil jsem tedy rebase a upravil tento commit tak, abych ze souborů odstranil zmíněná osobní data


Použité příkazy:
```sh
git checkout -b safe
git rebase -i a177898^
git add --all
git commit --amend
git rebase --continue
git push origin safe
```

## 4. Kompilovaný soubor
Postup:

- opět jsem si našel commit `c6926e1`, kde byl přidán soubor `a.out` a všiml si, že od té doby již nebyl nikdy upravován
- znovu jsem tedy použil rebase na tento commit, abych soubor smazal
- zadání nijak blíže nespecifikovalo, jaké soubory chceme ignorovat, do `.gitignore` jsem tedy pouze napsal soubor `a.out`,
žádné jiné soubory, které by neměly být verzovány, jsem v repozitáři nenašel
- git bude tedy od teď ignorovat všechny soubory názvu `a.out`

Použité příkazy:
```sh
git checkout -b clean
git rebase -i c6926e1^
rm -f a.out
git add src/image_histogram/a.out
git commit --amend
git rebase --continue
git push origin clean
```

Návrh `.gitignore`:
```
a.out
```

## 5. Oprava autora
Postup:

- použil jsem rebase na commit, kde byl uveden špatný autor a autora opravil
- kdybych chtěl opravit autory u mnoha commitů, použil bych způsob ukázaný přímo na přednášce - použití skriptu na filter-branch

Použité příkazy:
```sh
git checkout -b fix-author
git rebase -i ebc7a20^
git commit --author "Charlie <charlie@deepfileinfo.com>" --amend
git rebase --continue
git push origin fix-author
```

Příkazy pro případ zpracování mnoha commitů se špatnými jmény a adresami:
```sh
git filter-branch --env-filter '
    if [ "$GIT_COMMITTER_NAME" = "charlie" ];
    then
            GIT_COMMITTER_NAME="Charlie";
            GIT_AUTHOR_NAME="Charlie";
            GIT_COMMITTER_EMAIL="charlie@deepfileinfo.com";
            GIT_AUTHOR_EMAIL="charlie@deepfileinfo.com";
	fi' HEAD
```

## 6. Oddělení vývojové větve
Postup:

- chceme mít repozitář tak, aby větev master neobsahovala commity `ical`, a větev dev, která bude z masteru vycházet a tyto 3 commity bude obsahovat
- nejdříve jsem si tedy vytvořil větev `dev` vycházející z `fix-author`
- použil jsem rebase před první `ical` commit, abych mohl `ical` commity přesunout nahoru (těmto commitům jsem zachoval pořadí)
- v tomto místě jsem nyní vytvořil větev `master`
- protože ve větvi master tyto 3 commity nechceme, použil jsem reset na commit `4cdfac9`, který byl před těmito `ical` commity


Použité příkazy:
```sh
git checkout -b dev
git rebase -i 8b3a4a2^
git checkout -b master
git reset --hard 4cdfac9
git push origin master
git push origin dev:dev
```

## 7. Ochrana větve `master`
Postup:

- pokud chceme, aby git spustil hook skript před pushnutím, tak k tomu slouží hook `pre-push`
- v tomto skriptu spustím oba testy a pokud jejich návratová hodnota není 0, skript vrátí 1, což zabrání pushnutí

Cesta k souboru v adresáři `.git`: hooks/pre-push

Návrh obsahu souboru:
```sh
(cd ../../src/image_histogram/test; sh test.sh)

if [ $? -ne 0 ]
then
	exit 1
fi

(cd ../../src/ical_peek/test; sh test.sh)
if [ $? -ne 0 ]
then
	exit 1
fi

exit 0
```
