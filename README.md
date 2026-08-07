# Ye Olde Code: Fortran 77 to C Transpiler

Transpiler ini adalah program utilitas berbasis C++ yang mengonversi kode sumber **Fortran 77** menjadi kode sumber bahasa **C** yang ekuivalen dan siap dikompilasi oleh compiler C standar (seperti `gcc`).

---

## 🛠️ Prasyarat & Lingkungan Pengembangan

Sebelum membangun (*build*) transpiler ini, pastikan sistem Anda telah terinstal dependensi berikut:
- **C++ Compiler** (Mendukung standar C++17 atau lebih baru, misal: `g++` / MinGW-w64).
- **C Compiler** (Untuk menguji/mengompilasi hasil transpilasi C, misal: `gcc`).
- **CMake** (Versi 3.10 atau yang lebih baru).
- **Flex** (Fast Lexical Analyzer Generator, untuk kompilasi Lexer `.l`).
- **Make / MinGW Makefiles** (Opsional, untuk utilitas *build*).

---

## 🚀 Instalasi & Kompilasi Transpiler

Proyek ini telah dilengkapi dengan `Makefile` untuk mempermudah kompilasi dan eksekusi.

### 1. Kompilasi Transpiler
Untuk melakukan *build* penuh transpiler:
```bash
make build
# atau cukup jalankan
make
```
Eksekutabel `f77_transpiler` (atau `f77_transpiler.exe` di Windows) akan terbentuk secara otomatis di dalam direktori `build/`.

### 2. Jalankan Mode Interaktif
```bash
make run
```

### 3. Membersihkan Artifact Build
Untuk menghapus folder `build/` dan file biner sementara:
```bash
make clean
```

---

## 💻 Cara Menggunakan Transpiler

### 1. Menggunakan CLI (Command-Line Interface)
Jalankan transpiler dengan menyertakan argumen file Fortran 77:
```bash
./build/f77_transpiler sample_1.f
```
atau pada Windows (PowerShell/CMD):
```cmd
.\build\f77_transpiler.exe sample_1.f
```

### 2. Mode Interaktif
Jika transpiler dijalankan tanpa argumen:
```bash
./build/f77_transpiler
```
Program akan menampilkan prompt interaktif:
```text
Input Fortran 77 filename (ex: sample_1.f): sample_1.f
```

---

## 📂 Hasil Output Transpilasi (Intermediate Output)

Setiap kali transpiler dijalankan, luaran tiap komponen pipeline disimpan secara terorganisir di dalam folder `tests/`:
- `tests/lexer/<nama_file>_lexer` : Stream token hasil scanning *Lexer*.
- `tests/parser/<nama_file>_parser` : Visualisasi struktur pohon *Abstract Syntax Tree* (AST).
- `tests/semantic/<nama_file>_semantic` : Tabel Simbol (*Identifier* `tab`, *Block* `btab`, *Array* `atab`) & *Decorated AST*.
- `tests/codegen/<nama_file>.c` : Kode target C murni yang valid dan siap dikompilasi.

---

## 🏛️ Arsitektur Sistem

Transpiler dirancang dengan pola *Multi-Pass Pipeline* menggunakan arsitektur modern (*Visitor Pattern*):

```
Kode Fortran 77 ---> [Lexer (Flex)] ---> [Parser] ---> [Semantic Analyzer] ---> [CodeGen (Visitor)] ---> Kode Target C
```

1. **Lexer (Flex)**: Membaca format *fixed-form* Fortran 77 (kolom 1-5 label, kolom 6 continuation, kolom 7-72 kode, kolom 1 `C`/`*` komentar) dan mengubahnya menjadi *stream of tokens*.
2. **Parser**: Menggunakan pendekatan *Recursive Descent* manual untuk membangun pohon *Abstract Syntax Tree* (AST).
3. **Semantic Analyzer**: Melakukan verifikasi `IMPLICIT NONE`, pemetaan *scope*, validasi tipe data, serta pembentukan tabel simbol (`tab`, `btab`, `atab`).
4. **CodeGenVisitor**: Menelusuri AST menggunakan *Visitor Design Pattern* untuk menghasilkan kode C (termasuk *1D Column-Major Array Flattening*, *Pass-by-Reference Pointers*, *Struct COMMON Block*, *String CHARACTER Handling*, *Computed GOTO*, dan *Source Mapping*).

---

## 🧪 Validasi Pengujian (3 Program Uji & Bukti Output)

Berikut adalah 3 program uji Fortran 77 beserta kode C hasil transpilasi dan bukti eksekusinya:

### 1. Program Uji 1 (`tests/sample_1.f`)
**Kode Fortran 77 Original:**
```fortran
      PROGRAM ARITH
      IMPLICIT NONE
      INTEGER A, B, C
      REAL X, Y, Z
      A = 10
      B = 20
      C = A + B * 2
      X = 3.5
      Y = 2.0
      Z = X / Y
      IF (C .GT. 40) THEN
          PRINT *, C, Z
      ELSE
          PRINT *, 0
      ENDIF
      END
```
**Hasil Transpilasi C Target (`tests/codegen/sample_1.c`):**
```c
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(void){
    // line 3
    int A, B, C;
    // line 4
    float X, Y, Z;
    // line 5
    A = 10;
    // line 6
    B = 20;
    // line 7
    C = (A + (B * 2));
    // line 8
    X = 3.5;
    // line 9
    Y = 2.0;
    // line 10
    Z = (X / Y);
    // line 11
    if((C > 40)){
        // line 12
        printf("%d %f\n", C, Z);
    } else {
        // line 14
        printf("%d\n", 0);
    }
    return 0;
}
```
**GCC output:**
```text
50 1.750000
```

---

### 2. Program Uji 2: Array 2D (*Column-Major Flattening*) & Nested Loop (`tests/sample_4.f`)
**Kode Fortran 77 Original:**
```fortran
      PROGRAM MATSUM
      IMPLICIT NONE
      INTEGER A(3,2)
      INTEGER I, J, TOTAL

      A(1,1) = 1
      A(2,1) = 2
      A(3,1) = 3
      A(1,2) = 4
      A(2,2) = 5
      A(3,2) = 6

      TOTAL = 0
      DO 10 I = 1, 3
          DO 10 J = 1, 2
              TOTAL = TOTAL + A(I,J)
   10 CONTINUE

      PRINT *, TOTAL
      END
```
**Hasil Transpilasi C Target (`tests/codegen/sample_4.c`):**
```c
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main(void){
    // line 3
    int A[6];
    // line 4
    int I, J, TOTAL;
    // line 6
    A[(1 - 1) + (1 - 1) * 3] = 1;
    // line 7
    A[(2 - 1) + (1 - 1) * 3] = 2;
    // line 8
    A[(3 - 1) + (1 - 1) * 3] = 3;
    // line 9
    A[(1 - 1) + (2 - 1) * 3] = 4;
    // line 10
    A[(2 - 1) + (2 - 1) * 3] = 5;
    // line 11
    A[(3 - 1) + (2 - 1) * 3] = 6;
    // line 13
    TOTAL = 0;
    // line 14
    for(I=1; I<=3; I+=1){
        // line 15
        for(J=1; J<=2; J+=1){
            // line 16
            TOTAL = (TOTAL + A[(I - 1) + (J - 1) * 3]);
        }
    }
    // line 17
label_10:;
    // line 19
    printf("%d\n", TOTAL);
    return 0;
}
```
**GCC output:**
```text
21
```

---

### 3. Program Uji 3 (`tests/sample_bonus_2.f`)
**Kode Fortran 77 Original:**
```fortran
      PROGRAM BONUS
      IMPLICIT NONE
      CHARACTER*10 STR
      INTEGER I
      COMMON /STAT/ COUNT
      INTEGER COUNT
      COUNT = 0
      STR = 'HELLOF77'
      I = 2
      GOTO (10, 20, 30), I
 10   CONTINUE
      GOTO 40
 20   CONTINUE
      CALL INC(COUNT)
 30   CONTINUE
 40   CONTINUE
      PRINT *, STR, COUNT
      END

      SUBROUTINE INC(VAL)
      IMPLICIT NONE
      INTEGER VAL
      VAL = VAL + 10
      END
```
**Hasil Transpilasi C Target (`tests/codegen/sample_bonus_2.c`):**
```c
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define F77_STR_ASSIGN(dest, src, len) \
    do { \
        strncpy(dest, src, len); \
        int _l = strlen(src); \
        for(int _i = _l; _i < len; _i++) dest[_i] = ' '; \
        dest[len] = '\0'; \
    } while(0)

struct stat_t {
    int count;
} stat_block;

void inc(int *val);

int main(void){
    // line 3
    char STR[11];
    STR[0] = '\0';
    // line 4
    int I;
    // line 7
    stat_block.count = 0;
    // line 8
    F77_STR_ASSIGN(STR, "HELLOF77", 10);
    // line 9
    I = 2;
    // line 10
    switch(I){
        case 1: goto label_10; break;
        case 2: goto label_20; break;
        case 3: goto label_30; break;
    }
    // line 11
label_10:;
    // line 12
    goto label_40;
    // line 13
label_20:;
    // line 14
    inc(&stat_block.count);
    // line 15
label_30:;
    // line 16
label_40:;
    // line 17
    printf("%s %d\n", STR, stat_block.count);
    return 0;
}

void inc(int *val){
    // line 23
    *val = (*val + 10);
}
```
**Bukti Eksekusi (GCC output):**
```text
HELLOF77   10
```

---

## 🔗 Referensi Tambahan

- **Stanford University Fortran 77 Tutorial**: [https://web.stanford.edu/class/me200c/tutorial_77/](https://web.stanford.edu/class/me200c/tutorial_77/)
- **Flex (Fast Lexical Analyzer Generator) Manual**: [https://westes.github.io/flex/manual/](https://westes.github.io/flex/manual/)
- **GCC C Compiler Reference Manual**: [https://gcc.gnu.org/onlinedocs/](https://gcc.gnu.org/onlinedocs/)
- **f2c: Fortran 77 to C Converter Project**: [https://en.wikipedia.org/wiki/F2c](https://en.wikipedia.org/wiki/F2c)