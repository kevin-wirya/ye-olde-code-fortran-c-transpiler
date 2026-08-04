# Fortran 77 to C Transpiler

Transpiler ini mengonversi *source code* **Fortran 77** menjadi kode bahasa **C**

## Arsitektur

Secara garis besar, transpiler ini dikembangkan menggunakan **C++** dengan arsitektur pipeline
1. **Lexer**: Membaca format kolom Fortran menjadi *stream* token.
2. **Parser & AST**: Membentuk pohon hierarki memori menggunakan teknik *recursive descent*.
3. **Semantic Analyzer**: Memvalidasi tipe dan variabel.
4. **Code Generator**: Menghasilkan kode C yang ekuivalen beserta resolusi struktur data

## Cara Menjalankan Program