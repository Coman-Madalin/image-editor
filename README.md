##### Copyright 2024 Andrei-Madalin Coman (comanandreimadalin@gmail.com)

# Image Editor

## Vezi ca pagina web

```
sudo pip3 install grip
grip  README
# deschide http://localhost:6419/
```

## Cuprins

1. [Precizari Generale](#precizari-generale)
2. [Load](#load)
3. [Select](#select)
4. [Histogram](#histogram)
5. [Equalize](#equalize)
6. [Rotate](#rotate)
7. [Crop](#crop)
8. [Apply](#apply)
9. [Save](#save)
10. [Exit](#exit)

## Precizari Generale

Pentru a retine toate informatiile pe care le primim de la tastatura si pe
care lucram am creat o structura cu numele `DATA_t` care tine urmatoarele
variabile:

```
1. Cifra corespunzatoare tipului pozei.
2. Marimile pozei, inclusiv scala.
3. Coordonatele sectiuni din poza selectate.
4. Poza propriu-zisa, memorata intr-o matrice de pixeli.
```

Observatii:
1. De asemenea, majoritatea comenzilor au o functie inainte de ele care extrage
   parametrii necesari si ii valideaza.
2. Pentru mai multe detalii, vezi comentariile din cod.

## Load

### Sintaxa:

```
LOAD <fisier>
```

### Algoritm:

```
1. Deschide fisierul.
2. Citim header-ul care este comun tuturor pozelor.
3. Citim pixelii pozei si ii memoram intr-o matrice.
4. Inchidem fisierul de intrare.
```

## Select

### Sintaxa:

```
SELECT <x1> <y1> <x2> <y2>
```

SAU

```
SELECT ALL
```

### Algoritm:

```
0. Exista o functie inainte care verifica daca comanda este valida.
1. Verificam daca primul parametru este ALL.
2. Daca nu este, verificam daca parametrii sunt in ordine.
3. Daca sunt, verificam daca sunt in interiorul pozei.
4. Daca sunt, memoram coordonatele.
```

## Histogram

### Sintaxa:

```
HISTOGRAM <x> <y>
```

### Algoritm:

```
1. Verificam daca imaginea este alb-negru.
2. Daca este, calculam frecventa fiecarui pixel.
3. Grupam frecventele in bin-uri.
4. Calculam numarul de stelute cu ajutorul formulei:
    numar_stelute = valoare_bin * numar_stelute_max / valoare_bin_max
```

## Equalize

### Sintaxa:

```
EQUALIZE
```

### Algoritm:

```
1. Verificam daca imaginea este alb-negru.
2. Daca este, calculam frecventa fiecarui pixel.
4. Calculam suma cumulativa a frecventelor dupa formula:
  suma_cumulativa_i = suma_cumulativa_(i-1) + frecventa_i
5. Gasim valoarea minima din suma cumulativa, dar cum frecventele  
  sunt pozitive, valoarea minima este prima valoare nenula.
7. Calculam noua valoare a fiecarui pixel dupa formula:
  noua_valoare_i = (suma_cumulativa_i - valoare_minima) / 
                     (inaltime * latime - valoare_minima) * scala
8. Inlocuim fiecare pixel cu noua valoare.
```

Observatie: Operatia se va face pe toata poza, indiferent de selectia curenta.

## Rotate

### Sintaxa:

```
ROTATE <unghi>
```

### Algoritm:

```
1. Verificam daca imaginea este un patrat sau daca toata imaginea este
  selectata.
2. Daca este, verificam daca unghiul este multiplu de 90.
3. Daca este, si este negativ, il facem pozitiv.
4. In cazul in care toata imaginea este selectata, vom face urmatorele
   operatii:
    4.1. Modificam inaltimea si latimea pozei daca unghiul este 90 
      sau 270.
    4.2. In caz contrar, nu modificam nimic.
    4.3. Calculam noua pozitie a fiecarui pixel dupa unghiul dat:
        4.3.1. Pentru 90 grade:
            (i,j) = (inaltime - 1 - j, i)
        4.3.2. Pentru 180 grade:
            (i,j) = (inaltime - 1 - i, latime - 1 - j)
        4.3.3. Pentru 270 grade:
            (i,j) = (j, latime - 1 - i)
       
5. In cazul in care este selectata o sectiune din poza, vom face 
  urmatoarele  operatii:
    5.1. Calculam noua pozitie a fiecarui pixel dupa unghiul dat:
        5.1.1. Pentru 90 grade:
            (i,j) = (y2 - 1 - (j - x1), x1 + (i - y1))
        5.1.2. Pentru 180 grade:
            (i,j) = (y2 - 1 - (i - y1), x2 - 1 - (j - x1))
        5.1.3. Pentru 270 grade:
            (i,j) = (y1 + (j - x1), x2 - 1 - (i - y1))
    
```

## Crop

### Sintaxa:

```
CROP
```

### Algoritm:

```
1. Cream o noua poza cu dimensiunile sectiunii selectate.
2. Copiem pixelii din poza initiala in noua poza.
3. Inlocuim poza initiala cu noua poza.
```

## Apply

### Sintaxa:

```
APPLY <PARAMETRU>
```

### Algoritm:

```
1. Verificam daca imaginea este color.
2. Daca este, cream kernel-ul corespunzator filtrului.
3. Aplicam filtrul pe fiecare pixel din poza, inafara de margini.
```

## Save

### Sintaxa:

```
SAVE <fisier> [ascii]
```

### Algoritm:

```
1. Deschide fisierul.
2. Daca trebuie sa fie salvata in format ascii, vom face urmatoarele
  operatii:
    2.1. Scriem header-ul.
    2.2. Scriem pixelii pozei.
3. Daca trebuie sa fie salvata in format binar, vom face urmatoarele
  operatii:
        3.1. Scriem header-ul, modificand tipul pozei.
                               (P2 -> P5 sau P3 -> P6)
        3.2. Scriem pixelii pozei pe 8 biti.
4. Inchidem fisierul de iesire.
```

## Exit

### Sintaxa:

```
EXIT
```

### Algoritm:

```
1. Eliberam memoria alocata pentru matricea de pixeli.
2. Eliberam memoria alocata pentru uniunea de matrici de pixeli.
3. Eliberam memoria alocata pentru structura de date.
```
