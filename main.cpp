#include <iostream>
#include <fstream>
#include <ncurses.h>

using namespace std;

// Variabel global
char papan[3][3];
int posisiBaris = 0;
int posisiKolom = 0;
char pemain = 'X';

int skorX = 0;
int skorO = 0;
int seri = 0;

// Membaca skor dari file
void bacaSkor() {
    ifstream file("skor.txt");

    if (file.is_open()) {
        file >> skorX >> skorO >> seri;
        file.close();
    }
}

// Menyimpan skor ke file
void simpanSkor() {
    ofstream file("skor.txt");

    if (file.is_open()) {
        file << skorX << " "
             << skorO << " "
             << seri;
        file.close();
    }
}

// Mengatur papan permainan
void resetPapan() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            papan[i][j] = ' ';
        }
    }

    posisiBaris = 0;
    posisiKolom = 0;
    pemain = 'X';
}

// Menampilkan papan permainan
void tampilkanPapan() {
    clear();

    mvprintw(1, 10, "===== TIC TAC TOE =====");

    mvprintw(3, 10, "Giliran Pemain: %c", pemain);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            int baris = 5 + i * 2;
            int kolom = 10 + j * 4;

            // Menandai posisi kursor
            if (i == posisiBaris && j == posisiKolom) {
                attron(A_REVERSE);
            }

            // Memberikan warna X dan O
            if (papan[i][j] == 'X') {
                attron(COLOR_PAIR(1));
                mvaddch(baris, kolom, 'X');
                attroff(COLOR_PAIR(1));
            }
            else if (papan[i][j] == 'O') {
                attron(COLOR_PAIR(2));
                mvaddch(baris, kolom, 'O');
                attroff(COLOR_PAIR(2));
            }
            else {
                mvaddch(baris, kolom, ' ');
            }

            if (i == posisiBaris && j == posisiKolom) {
                attroff(A_REVERSE);
            }
        }
    }

    // Garis papan
    for (int i = 0; i < 2; i++) {
        mvhline(6 + i * 2, 9, ACS_HLINE, 10);
    }

    for (int j = 0; j < 2; j++) {
        mvvline(5, 12 + j * 4, ACS_VLINE, 5);
    }

    mvprintw(12, 5, "Gunakan tombol panah untuk bergerak");
    mvprintw(13, 5, "ENTER : Pilih kotak");
    mvprintw(14, 5, "R     : Restart permainan");
    mvprintw(15, 5, "ESC   : Keluar");

    mvprintw(17, 5, "Skor X: %d   Skor O: %d   Seri: %d",
             skorX, skorO, seri);

    refresh();
}

// Memeriksa pemenang
bool cekPemenang() {

    // Memeriksa baris
    for (int i = 0; i < 3; i++) {
        if (papan[i][0] == pemain &&
            papan[i][1] == pemain &&
            papan[i][2] == pemain) {
            return true;
        }
    }

    // Memeriksa kolom
    for (int j = 0; j < 3; j++) {
        if (papan[0][j] == pemain &&
            papan[1][j] == pemain &&
            papan[2][j] == pemain) {
            return true;
        }
    }

    // Memeriksa diagonal
    if (papan[0][0] == pemain &&
        papan[1][1] == pemain &&
        papan[2][2] == pemain) {
        return true;
    }

    if (papan[0][2] == pemain &&
        papan[1][1] == pemain &&
        papan[2][0] == pemain) {
        return true;
    }

    return false;
}

// Memeriksa apakah papan penuh
bool cekSeri() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (papan[i][j] == ' ') {
                return false;
            }
        }
    }

    return true;
}

// Mengatur pergerakan keyboard
void gerakKursor(int tombol) {

    switch (tombol) {

        case KEY_UP:
            if (posisiBaris > 0)
                posisiBaris--;
            break;

        case KEY_DOWN:
            if (posisiBaris < 2)
                posisiBaris++;
            break;

        case KEY_LEFT:
            if (posisiKolom > 0)
                posisiKolom--;
            break;

        case KEY_RIGHT:
            if (posisiKolom < 2)
                posisiKolom++;
            break;
    }
}

// Memulai permainan
void mulaiGame() {

    resetPapan();

    while (true) {

        tampilkanPapan();

        int tombol = getch();

        // Keluar dari permainan
        if (tombol == 27) {
            break;
        }

        // Restart permainan
        if (tombol == 'r' || tombol == 'R') {
            resetPapan();
            continue;
        }

        // Menggerakkan kursor
        gerakKursor(tombol);

        // Memilih kotak
        if (tombol == 10 || tombol == 13 ||
            tombol == KEY_ENTER) {

            if (papan[posisiBaris][posisiKolom] == ' ') {

                papan[posisiBaris][posisiKolom] = pemain;

                if (cekPemenang()) {

                    tampilkanPapan();

                    mvprintw(19, 5,
                        "Pemain %c MENANG!", pemain);

                    if (pemain == 'X')
                        skorX++;
                    else
                        skorO++;

                    simpanSkor();
                    refresh();
                    getch();
                    break;
                }

                else if (cekSeri()) {

                    tampilkanPapan();

                    mvprintw(19, 5, "PERMAINAN SERI!");

                    seri++;
                    simpanSkor();
                    refresh();
                    getch();
                    break;
                }

                // Mengganti giliran pemain
                if (pemain == 'X')
                    pemain = 'O';
                else
                    pemain = 'X';
            }
        }
    }
}

// Program utama
int main() {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Mengaktifkan warna
    if (has_colors()) {
        start_color();

        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
    }

    // Membaca data skor
    bacaSkor();

    bool berjalan = true;

    while (berjalan) {

        clear();

        mvprintw(3, 10, "===== TIC TAC TOE =====");
        mvprintw(6, 10, "1. Mulai Permainan");
        mvprintw(7, 10, "2. Lihat Skor");
        mvprintw(8, 10, "3. Keluar");
        mvprintw(10, 10, "Pilih menu: ");

        refresh();

        int pilihan = getch();

        switch (pilihan) {

            case '1':
                mulaiGame();
                break;

            case '2':
                clear();
                mvprintw(5, 10, "===== DATA SKOR =====");
                mvprintw(7, 10, "Pemain X: %d", skorX);
                mvprintw(8, 10, "Pemain O: %d", skorO);
                mvprintw(9, 10, "Seri: %d", seri);
                mvprintw(11, 10, "Tekan tombol untuk kembali");
                refresh();
                getch();
                break;

            case '3':
                berjalan = false;
                break;
        }
    }

    simpanSkor();

    endwin();

    return 0;
}
