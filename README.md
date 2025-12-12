# PRIEMYSLOVAK
<details>
<summary>Slovak 🇸🇰</summary>
  
PRIEMYSLOVAK je programovací jazyk, ktorý bol vytvorený ako maturitný projekt. Jeho syntax je odvodená z jazyka BASIC. Je to taktiež môj prvý projekt v C++ a prvý programovací jazyk, aký som kedy spravil.
<br><br>
**UPOZORNENIE** ⚠️<br>
*Maximálne neodporúčam využívať tento jazyk v akomkoľvek projekte! Jazyk neobsahuje funkcie, triedy, ani základné atribúty. Rýchlosť tohto jazyka je 1000x pomalšia ako Python (pre porovnanie: Python napočíta do 1 000 000 za zlomok sekundy, PRIEMYSLOVAK za 3 minúty). Dôvodom je to, že Python je z časti aj kompilovaný, pretože celý kód sa preloží do Python bytecodu, ktorý sa následne interpretuje a nekontroluje. PRIEMYSLOVAK môže skontrolovať jeden riadok aj 5x, čo spôsobuje brutálne spomalenie. Taktiež, parser nekontroluje iba syntax, ale aj datatypy. Viem, že to tak nemá byť, ale keď som si to už uvedomil, tak bolo neskoro.*

# Kompilácia
Linux:<br>
```
g++ -std=c++23 main.cpp statuses.cpp lexer.cpp basicfunctions.cpp parser.cpp interpreter.cpp -o priemyslovak
```
Windows (kompilované na Linuxe):<br>
```
x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++ -std=c++23 main.cpp statuses.cpp lexer.cpp basicfunctions.cpp parser.cpp interpreter.cpp -o priemyslovak.exe
```

## Ako používať tento jazyk?
Jazyk môžete spustiť v termináli príkazom `priemyslovak.exe` (alebo `./priemyslovak`, ak používate Linux). Scripty v jazyku PRIEMYSLOVAK končia *.priem*. Ak chcete spustiť script, jednoducho pridajte cestu, ktorá odkazuje na script: `priemyslovak.exe cesta/odkazujuca/na/script`.

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
- \t (horizontálny tabulátor)
- \r (návrat na začiatok riadku)
- \\\ (backslash)
- \\' (apostrof)
- \\" (úvodzovky)
- \b (backspace)

### Premenné
Premenná sa môže vytvoriť takto:<br>
`a = 1` (do premennej a je vložená hodnota 1)
<br><br>
Premenná sa môže aktualizovať použitím úplne rovnakého spôsobu.<br>
K hodnote premennej sa môže pristupovať ako v každom jazyku - jednoducho napíšeme názov premennej:<br>
`9 + a` (a sa nahradí 1, potom sa vypočíta 9 + 1 = 10)

### Arrays
Arrays (polia) zoskupujú viaceré hodnoty do jedného datatypu.<br>
Príklad: `a = [1,2,3]` (premenná a obsahuje pole, v ktorom sú čísla 1, 2, 3)
<br><br>
K hodnotám poľa sa dá pristupovať jednotlivo pomocou indexovania. Hodnoty poľa sa dajú meniť ako premenné.<br>
```
a = [1,2,3,4]
PRINTLN a[0] // 1

a[0] = 5
PRINT a // [5,2,3,4]
```
<br>
Indexy sa začínajú nulou, takže prvá hodnota je na indexe 0, druhá na indexe 1, atď.

### Keywordy
Keywordy sú príkazy interpretera, ktoré majú určité funkcie. PRIEMYSLOVAK má zadefinovaných 28 keywordov:<br>
- PRINT
- PRINTLN
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
Existuje ešte *PRINTLN*, ktorý po výpise prejde na nový riadok. *PRINT* ostane na jednom riadku.<br>
Použitie: `PRINT <hodnota>`
<br><br>
**INPUT**<br>
Používa sa na získanie vstupu od používateľa.<br>
Použitie: `INPUT` (môže sa už rovno vložiť do premennej: `a = INPUT`)
<br><br>
**IF**<br>
Používa sa na vykonanie rozhodnutí.<br>
Použitie: `IF (condition) THEN`<br>
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
  PRINTLN i
END // 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10

FOR i = 0 TO 10 STEP 2 DO
  PRINTLN i // 0, 2, 4, 6, 8, 10
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
Môže sa aj použiť na zaokrúhlenie float.<br>
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

PRINTLN TO_INT (RANDOM * 100) // náhodné číslo od 0 do 100
PRINTLN TO_INT (RANDOM * 98 + 2) // náhodné číslo od 2 do 100
PRINTLN TO_INT (RANDOM + 3) // náhodné číslo od 3 do 4
```
</details>
<details>
<summary>English 🇬🇧</summary>

PRIEMYSLOVAK is a programming language created as a graduation project. Its syntax is derived from BASIC. It is also my first project in C++ and the first programming language I have ever created. <br><br>
**WARNING** ⚠️<br>
*I strongly advise against using this language in any real project! The language does not support functions, classes, or basic attributes. The speed of this language is 1000x slower than Python (for comparison: Python counts to 1,000,000 in a fraction of a second, PRIEMYSLOVAK takes 3 minutes). The reason is that Python is partially compiled, as the whole code is converted into Python bytecode, which is then interpreted without checking. PRIEMYSLOVAK may check the same line up to 5 times, causing severe slowdown. Also, the parser checks not only syntax but also data types. I know it shouldn’t be like this, but by the time I realized it, it was too late.*

# Compilation

Linux:<br>

```
g++ -std=c++23 main.cpp statuses.cpp lexer.cpp basicfunctions.cpp parser.cpp interpreter.cpp -o priemyslovak
```

Windows (compiled on Linux):<br>

```
x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++ -std=c++23 main.cpp statuses.cpp lexer.cpp basicfunctions.cpp parser.cpp interpreter.cpp -o priemyslovak.exe
```

## How to use this language?

You can run the language in the terminal using `priemyslovak.exe` (or `./priemyslovak` on Linux). Scripts in PRIEMYSLOVAK end with *.priem*. To run a script, simply provide the path to the script: `priemyslovak.exe path/to/script`.

### Data types

The following data types are used in PRIEMYSLOVAK:<br>

- INTEGER
- FLOAT
- STRING
- BOOL
- NONETYPE
- ARRAY

### Operations

Classic math operations are supported: +, -, *, /, ^, %.
Mathematical order of operations applies! First `^` (power), then *, /, %, then +, -.

Parentheses can also be used. The interpreter always evaluates parentheses first!

Example:<br>
`5 + 6 * 2` equals 17.<br>
`(5 + 6) * 2` equals 22.

Strings can be concatenated using +:<br>
`"hello " + "world"` ("hello world")

### Logical operators

There are 7 logical operators:

- ~
- ==
- ~=
- \>=
- <=
- \>
- <

Usage: `1 == 1` (returns TRUE) <br><br>
The `~` (NOT) operator can only be used on a single value.<br>
Example: `~8` (inverts all bits of the number 8, returning -9)

### Comments

Comments are lines of code ignored by the interpreter. They can be placed anywhere:

```
// This is a comment
1 + 5 // This is also a comment, but 1 + 5 still executes!

/* This
is
a
multi-line
comment */

/* Can also be on a single line */
```

### Escape characters

Escape characters are characters in strings with special functions. PRIEMYSLOVAK supports these:<br>

* \n (newline)
* \t (horizontal tab)
* \r (carriage return)
* \\\ (backslash)
* \\' (apostrophe)
* \\" (quotation marks)
* \b (backspace)

### Variables

Variables can be created like this:<br>
`a = 1` (assigns value 1 to variable `a`) <br><br>
Variables can be updated in the same way.<br>
You can access a variable’s value like in any other language:<br>
`9 + a` (`a` is replaced with 1, then 9 + 1 = 10)

### Arrays

Arrays group multiple values into a single data type.<br>
Example: `a = [1,2,3]` (variable `a` contains an array with values 1, 2, 3) <br><br>
Array values can be accessed individually by index and modified like variables.<br>

```
a = [1,2,3,4]
PRINTLN a[0] // 1

a[0] = 5
PRINT a // [5,2,3,4]
```

<br>
Indexes start at zero: the first value is at index 0, the second at index 1, etc.

### Keywords

Keywords are interpreter commands with specific functions. PRIEMYSLOVAK has 28 defined keywords:<br>

* PRINT
* PRINTLN
* INPUT
* IF
* ELIF
* ELSE
* AND
* OR
* END
* GOTO
* THEN
* FOR
* TO
* STEP
* WHILE
* BREAK
* CONTINUE
* DO
* SLEEP
* RAISE
* IMPORT
* DELETE
* EXECUTE
* TERMINATE
* TO_STRING
* TO_INT
* TO_FLOAT
* RANDOM

**PRINT**<br>
Used to output a value to the console.
`PRINTLN` prints the value and moves to a new line. `PRINT` stays on the same line.
Usage: `PRINT <value>` <br><br>
**INPUT**<br>
Used to get user input.
Usage: `INPUT` (can be assigned directly to a variable: `a = INPUT`) <br><br>
**IF**<br>
Used for conditional execution.
Usage: `IF (condition) THEN`<br>
Must end with the keyword `END`! <br><br>
**ELIF**<br>
Same as IF, but executes only if the previous condition was FALSE.<br>
Usage:<br>

```
IF (FALSE) THEN // IF was FALSE, ELIF condition will execute
  PRINT "A"
END

ELIF (TRUE) THEN
  PRINT "B"
END
```

<br><br>
**ELSE**<br>
Same as ELIF, but always TRUE. Placed at the end of a condition chain.<br>

```
IF (FALSE) THEN
  PRINT "A"
END

ELIF (FALSE) THEN
  PRINT "B"
END

ELSE // ELSE executes
  PRINT "C"
END
```

<br><br>
**AND**<br>
Returns TRUE if both left and right conditions are TRUE.<br>
Usage: `0 == 0 AND 1 == 0` (returns FALSE) <br><br>
**OR**<br>
Returns TRUE if at least one condition is TRUE.<br>
Usage: `0 == 0 OR 1 == 0` (returns TRUE) <br><br>
**END**<br>
Ends IF, ELIF, ELSE, WHILE, and FOR blocks. <br><br>
**GOTO**<br>
Jumps to a specific line of code. Execution continues from there.<br>
Usage: `GOTO 1` (jumps to line 1 and continues from there) <br><br>
**WHILE**<br>
Classic conditional loop. A block of code repeats while the condition is TRUE.<br>
Usage:<br>

```
WHILE (0 == 0) DO
  PRINT "hello world" // infinite loop
END
```

<br><br>
**FOR**<br>
A loop that repeats until the variable reaches a predefined value.
Can be combined with `STEP` to define the increment per iteration.
Usage:<br>

```
FOR i = 0 TO 10 DO
  PRINTLN i
END // 0,1,2,3,4,5,6,7,8,9,10

FOR i = 0 TO 10 STEP 2 DO
  PRINTLN i // 0,2,4,6,8,10
END
```

**BREAK**<br>
Exits the loop in which it appears.<br>
Usage: `BREAK`<br><br>
**CONTINUE**<br>
Ends the current iteration and starts the next one.<br>
Usage: `CONTINUE` <br><br>
**SLEEP**<br>
Pauses program execution for a number of seconds.<br>
Usage: `SLEEP 3` (pauses for 3 seconds) <br><br>
**RAISE**<br>
Defines and throws a custom error, terminating the program.<br>
Usage: `RAISE: "You can't type a number higher than 0"` <br><br>
**IMPORT**<br>
Used to merge multiple scripts. After merging, variables from the scripts can be used.<br>
Usage: `IMPORT "scripts/TEST.priem"` <br><br>
**DELETE**<br>
Deletes a variable.<br>
Usage: `DELETE a` <br><br>
**EXECUTE**<br>
Executes code written in PRIEMYSLOVAK as a string.<br>
Usage: `EXECUTE "PRINT \"hello world\""` <br><br>
**TERMINATE**<br>
Terminates the program.<br>
Usage: `TERMINATE` <br><br>
**TO_STRING**<br>
Converts any data type to a string.<br>
Usage: `TO_STRING 5` <br><br>
**TO_INT**<br>
Converts a string to an integer (also rounds floats).<br>
Usage: `TO_INT "5"` <br><br>
**TO_FLOAT**<br>
Converts a string to a float.<br>
Usage: `TO_FLOAT "5.5"` <br><br>
**RANDOM**<br>
Generates a random decimal number between 0 and 1.<br>
Example usage:<br>
```
a = RANDOM // a now has a random decimal from 0 to 1 (0.107861)

PRINTLN TO_INT (RANDOM * 100) // random number from 0 to 100
PRINTLN TO_INT (RANDOM * 98 + 2) // random number from 2 to 100
PRINTLN TO_INT (RANDOM + 3) // random number from 3 to 4
```
</details>
