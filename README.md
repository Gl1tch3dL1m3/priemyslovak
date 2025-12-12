# PRIEMYSLOVAK
PRIEMYSLOVAK je programovací jazyk, ktorý bol vytvorený ako maturitný projekt. Jeho syntax je odvodená z jazyka BASIC. Je to taktiež môj prvý projekt v C++ a prvý programovací jazyk, aký som kedy spravil.
<br><br>
**UPOZORNENIE** ⚠️<br>
*Maximálne neodporúčam využívať tento jazyk v akomkoľvek projekte! Jazyk neobsahuje funkcie, triedy, ani základné atribúty. Rýchlosť tohto jazyka je 1000x pomalšia ako Python (pre porovnanie: Python napočíta do 1 000 000 za zlomok sekundy, PRIEMYSLOVAK za 3 minúty. Dôvodom je to, že Python je z časti aj kompilovaný, pretože celý kód sa preloží do Python bytecodu, ktorý sa následne interpretuje a nekontroluje. PRIEMYSLOVAK môže skontrolovať jeden riadok aj 5x, čo spôsobuje brutálne spomalenie. Taktiež, parser nekontroluje iba syntax, ale aj datatypy. Viem, že to tak nemá byť, ale keď som si to už uvedomil, tak bolo neskoro.*

## Ako používať tento jazyk?
### Datatypy
V jazyku PRIEMYSLOVAK sa využívajú tieto dátové typy:<br>
- INTEGER
- FLOAT
- STRING
- BOOL
- NONETYPE
- ARRAY

### Operácie
Sú tu klasické matematické operácie: +, -, *, /, ^, %.
Platia matematické operácie! Najskôr sa vypočíta ^ (mocnina), potom *, /, %, potom +, -.

Zátvorky sa tiež dajú použiť. Interpreter sa vždy bude venovať zátvorkam ako prvým!

Príklad:<br>
`5 + 6 * 2` je 17.<br>
`(5 + 6) * 2` je 22.

Stringy sa môžu zlúčiť pomocou +:<br>
`"ahoj " + "svet"` ("ahoj svet")

### Logické operátory
Existuje 7 logických operátorov:
- ~
- ==
- ~=
- \>=
- <=
- \>
- <

Použitie: `1 == 1` (vráti TRUE)
<br><br>
Znamieko ~ (NOT) je možné použiť len na samostatnú hodnotu.<br>
Príklad: `~8` (prevráti všetky bity čísla 8 a vráti -9)

### Komentáre
Komentáre sú riadky kódu, ktoré sú ignorované interpreterom. Môžu sa vložiť hocikde do kódu:
```
// Toto je komentár
1 + 5 // Toto je tiež komentár, ale 1 + 5 už nie!

/* Toto
je
viacriadkový
komentár */

/* Môže byť aj v jednom riadku */
```

### Escape characters
Escape characters sú znaky v stringe, ktoré majú špeciálne funkcie. PRIEMYSLOVAK podporuje tieto znaky:<br>
- \n (nový riadok)
- \t (tabulátor)
- 

### Premenné
Premenná sa môže vytvoriť takto:<br>
`a = 1` (do premennej a je vložená hodnota 1)
<br><br>
Premenná sa môže aktualizovať použitím úplne rovnakého spôsobu.<br>
K hodnote premennej sa môže pristupovať ako v každom jazyku - jednoducho napíšeme názov premennej:<br>
`9 + a` (a sa nahradí 1, potom sa vypočíta 9 + 1 = 10)

### Arrays

### Keywordy
PRIEMYSLOVAK má zadefinovaných 27 keywordov:<br>
- PRINT
- INPUT
- IF
- ELIF
- ELSE
- AND
- OR
- END
- GOTO
- THEN
- FOR
- TO
- STEP
- WHILE
- BREAK
- CONTINUE
- DO
- SLEEP
- RAISE
- IMPORT
- DELETE
- EXECUTE
- TERMINATE
- TO_STRING
- TO_INT
- TO_FLOAT
- RANDOM

**PRINT**<br>
Používa sa na výpis hodnoty do konzole.<br>
Použitie: `PRINT <hodnota>`
<br><br>
**INPUT**<br>
Používa sa na získanie vstupu od používateľa.<br>
Použitie: `INPUT` (môže sa už rovno vložiť do premennej: `a = INPUT`)
<br><br>
**IF**<br>
Používa sa na vykonanie rozhodnutí.
Použitie:<br>
`IF (condition) THEN`<br>
Musí sa končiť keywordom *END*!
<br><br>
**ELIF**<br>
Má takú istú funkciu ako *IF*, ale vykoná sa len vtedy, ak je predošlá condition FALSE.<br>
Použitie:<br>
```
IF (FALSE) THEN // AK BY TOTO BOLA PRAVDA, ELIF CONDITION BY SA NEPLATIL
  PRINT "A"
END

ELIF (TRUE) THEN
  PRINT "B"
END
```
<br><br>
**ELSE**<br>
Má takú istú funkciu ako ELIF, ale je vždy TRUE. Dáva sa na koniec condition reťazca.<br>
```
IF (FALSE) THEN
  PRINT "A"
END

ELIF (FALSE) THEN
  PRINT "B"
END

ELSE // VYKONÁ SA ELSE
  PRINT "C"
END
```
<br><br>
**AND**<br>
Vráti TRUE, ak sú podmienky na ľavej aj pravej strane TRUE.<br>
Použitie: `0 == 0 AND 1 == 0` (vráti FALSE, pretože 1 == 0 neplatí)
<br><br>
**OR**<br>
Vráti TRUE, ak je aspoň jedno z podmienok TRUE.<br>
Použitie: `0 == 0 OR 1 == 0` (vráti TRUE, pretože 0 == 0 platí)
<br><br>
**END**<br>
Používa sa na ukončenie *IF*, *ELIF*, *ELSE*, *WHILE* a *FOR* blokov.
<br><br>
**GOTO**<br>
Skočí na určitý riadok kódu, vráti sa a pôjde ďalej.<br>
Použitie: `GOTO 1` (skočí na riadok 1 a potom pokračuje tam, kde skončil)
<br><br>
**WHILE**<br>
Klasický podmienkový cyklus. Určitý blok kódu sa opakuje dokým platí zadaná podmienka.<br>
Použitie:<br>
```
WHILE (0 == 0) DO
  PRINT "ahoj svet" // nekonečný cyklus
END
```
<br><br>
**FOR**<br>
Cyklus, ktorý sa opakuje, kým vložená premenná sa nerovná už vopred stanovému číslu.<br>
Môže sa kombinovať s keywordom *STEP*, ktorý určuje o koľko sa hodnota danej premennej zvýši každým cyklom.
Použitie:<br>
```
FOR i = 0 TO 10 DO
  PRINT i
END // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10

FOR i = 0 TO 10 STEP 2 DO
  PRINT i // 0, 2, 4, 6, 8, 10
END
```
**BREAK**<br>
Používa sa na ukončenie loopu, v ktorom sa keyword nachádza.<br>
Použitie: `BREAK`
<br><br>
**CONTINUE**<br>
Ukončí blok loopu a začne ďalšiu iteráciu.<br>
Použitie: `CONTINUE`
<br><br>
**SLEEP**<br>
Pozastaví chod programu na určitý počet sekúnd.<br>
Použitie: `SLEEP 3` (pozastaví na tri sekundy)
<br><br>
**RAISE**<br>
Používa sa na definíciu a vyhodenie vlastnej chyby a zrušenie celého chodu programu.<br>
Použitie: `RAISE: "You can't type a number higher than 0"`
<br><br>
**IMPORT**<br>
Používa sa na zlúčenie viacerých scriptov. Po zlúčení je možné použiť premenné daných scriptov.<br>
Použitie: `IMPORT "scripts/TEST.priem"`
<br><br>
**DELETE**<br>
Používa sa na výmaz premennej.<br>
Použitie: `DELETE a`
<br><br>
**EXECUTE**<br>
Vykoná kód napísaný v PRIEMYSLOVAK-u v podobe reťazca (string).<br>
Použitie: `EXECUTE "PRINT \"ahoj svet\""`
<br><br>
**TERMINATE**<br>
Používa sa na zrušenie celého chodu programu.<br>
Použitie: `TERMINATE`
<br><br>
**TO_STRING**<br>
Používa sa na konverziu akéhokoľvek datatypu na string.<br>
Použitie: `TO_STRING 5`
<br><br>
**TO_INT**<br>
Používa sa na konverziu z datatypu string na integer.<br>
Môže sa aj použiť na zaokruhlenie float.<br>
Použitie: `TO_INT "5"`
<br><br>
**TO_FLOAT**<br>
Používa sa na konverziu z datatypu string na float.<br>
Použitie: `TO_FLOAT "5.5"`
<br><br>
**RANDOM**<br>
Používa sa na generáciu náhodného desatinného čísla od 0 do 1.<br>
Použitie s príkladmi:<br>
```
a = RANDOM // premenná a má vložené náhodné desatinné číslo od 0 do 1 (0.107861)

PRINT TO_INT (RANDOM * 100) // náhodné číslo od 0 do 100
PRINT TO_INT (RANDOM * 98 + 2) // náhodné číslo od 2 do 100
PRINT TO_INT (RANDOM + 3) // náhodné číslo od 3 do 4
```
