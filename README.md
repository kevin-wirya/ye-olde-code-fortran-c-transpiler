# Fortran 77 to C Transpiler

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

Ikuti langkah-langkah berikut untuk mengompilasi transpiler dari kode sumber:

### 1. Kloning Repositori & Masuk ke Direktori
```bash
cd ye-olde-code-fortran-c-transpiler
```

### 2. Generate Build Files dengan CMake
```bash
cmake -B build
```
*(Catatan pada Windows MinGW)*: Jika Anda menggunakan MinGW, gunakan:
```bash
cmake -G "MinGW Makefiles" -B build
```

### 3. Kompilasi Proyek
```bash
cmake --build build
```
Setelah proses kompilasi selesai, eksekutabel `f77_transpiler` (atau `f77_transpiler.exe` di Windows) akan terbentuk di dalam folder `build/`.

---

## 💻 Cara Menggunakan Transpiler

Menjalankan transpiler untuk mentranspilasikan berkas Fortran 77:

### Menggunakan Argumen CLI
```bash
./build/f77_transpiler sample_1.f
```
atau (di Windows PowerShell/CMD):
```cmd
.\build\f77_transpiler.exe sample_1.f
```

### Mode Interaktif
Jika dijalankan tanpa argumen
```bash
./build/f77_transpiler
```
Program akan meminta Anda memasukkan nama file Fortran 77
```text
Input Fortran 77 filename (ex: sample_1.f): sample_1.f
```

---

## 📂 Hasil Output Transpilasi

Setiap kali transaksi dijalankan, transpiler secara otomatis menghasilkan keluaran terstruktur di folder `tests/`
- `tests/lexer/<nama_file>_lexer` : Hasil *tokenizing* dari Lexer.
- `tests/parser/<nama_file>_parser` : Visualisasi pohon AST (*Abstract Syntax Tree*).
- `tests/semantic/<nama_file>_semantic` : Tabel Simbol (`tab`, `btab`, `atab`) dan *Decorated AST*.
- `tests/codegen/<nama_file>.c` : Kode target C murni yang siap dikompilasi oleh `gcc`.

Contoh cara mengompilasi kode C hasil transpilasi
```bash
gcc tests/codegen/sample_1.c -o sample_1.exe -lm
./sample_1.exe
```

---

## 🏛️ Arsitektur Sistem

Transpiler ini bekerja dalam 4 tahap utama (*Multi-Pass Pipeline*):

```
Fortran 77 Code ---> [Lexer] ---> [Parser] ---> [Semantic Analyzer] ---> [CodeGen] ---> Target C Code
```

1. **Lexer (Flex)**: Mengonversi teks Fortran 77 menjadi *stream of tokens*.
2. **Parser**: Membangun pohon sintaks *Abstract Syntax Tree* (AST) dengan pendekatan *Recursive Descent*.
3. **Semantic Analyzer**: Memvalidasi tipe data, scope, serta membentuk tabel simbol (`tab`, `btab`, `atab`).
4. **CodeGen (Visitor Pattern)**: Menelusuri AST untuk menghasilkan kode C yang ekuivalen (termasuk *array flattening*, *pass-by-reference*, dan *struct COMMON*).