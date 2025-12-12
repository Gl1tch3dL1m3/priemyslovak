# PRIEMYSLOVAK
PRIEMYSLOVAK je programovací jazyk, ktorý bol vytvorený ako maturitný projekt. Jeho syntax je odvodená z jazyka BASIC. Je to takisto môj prvý projekt v C++ a prvý programovací jazyk, aký som kedy spravil.
<br><br>
**UPOZORNENIE** ⚠️

*Maximálne neodporúčam využívať tento jazyk v akomkoľvek projekte! Jazyk neobsahuje funkcie, triedy, ani základné atribúty. Rýchlosť tohto jazyka je 1000x pomalšia ako Python (pre porovnanie: Python napočíta do 1 000 000 za zlomok sekundy, PRIEMYSLOVAK za 3 minúty. Dôvodom je to, že Python je z časti aj kompilovaný, pretože celý kód sa preloží do Python bytecodu, ktorý sa následne interpretuje a nekontroluje. PRIEMYSLOVAK môže skontrolovať ten istý riadok aj 5x. Taktiež, parser nekontroluje iba syntax, ale aj datatypy. Viem, že to tak nemá byť, ale keď som si to už uvedomil, tak bolo neskoro.*

## Ako používať tento jazyk?
### Datatypy
V jazyku PRIEMYSLOVAK sa využívajú tieto dátové typy:<br>
- INTEGER
- FLOAT
- STRING
- BOOL
- NONETYPE

### Operácie
Sú tu klasické matematické operácie: +, -, *, /, ^, %.
Platia matematické operácie! Najskôr sa vypočíta ^ (mocnina), potom *, /, %, potom +, -.

Zátvorky sa tiež dajú použiť. Interpreter sa vždy bude venovať zátvorkam ako prvým!

Príklad:<br>
`5 + 6 * 2` je 17.<br>
`(5 + 6) * 2` je 22.

Stringy sa môžu zlúčiť pomocou +:<br>
`"ahoj " + "svet"` ("ahoj svet")

### Premenné
Premenná sa môže vytvoriť takto:<br>
`a = 1` (do premennej a je vložená hodnota 1)
<br><br>
Premenná sa môže aktualizovať použitím úplne rovnakého spôsobu.<br>
K hodnote premennej sa môže pristupovať ako v každom jazyku - jednoducho napíšeme názov premennej:<br>
`9 + a` (a sa nahradí 1, potom sa vypočíta 9 + 1 = 10)

### Keywordy
PRIEMYSLOVAK má zadefinovaných 27 keywordov:<br>
- PRINT
- INPUT
- IF
- ELIF
- AND
- OR
- END
- ELSE
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
`IF (FALSE) THEN
  PRINT "A" //
END

ELIF (TRUE) THEN
  PRINT "B"
END`
