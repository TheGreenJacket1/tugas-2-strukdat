#include <iostream>
using namespace std;

struct pelanggan {
    string nama;
    pelanggan* next;
};

struct menu {
    string nama, kategori;
    double harga;
    bool tersedia;
    pelanggan* depan;
    pelanggan* belakang;
    menu* kiri;
    menu* kanan;
};

struct riwayat {
    int jenis;
    string namamenu, namapelanggan, pelanggandilayani;
    riwayat* next;
};

void enqueue(pelanggan*& depan, pelanggan*& belakang, string nama) {
    pelanggan* baru = new pelanggan{nama, nullptr};

    if (depan == nullptr) {
        depan = baru;
        belakang = baru;
    } else {
        belakang->next = baru;
        belakang = baru;
    }
}

void enqueuedepan(pelanggan*& depan, pelanggan*& belakang, string nama) {
    pelanggan* baru = new pelanggan{nama, depan};

    if (depan == nullptr) {
        belakang = baru;
    }
    depan = baru;
}

string dequeue(pelanggan*& depan, pelanggan*& belakang) {
    if (depan == nullptr) {
        return "";
    }
    pelanggan* hapus = depan;
    string nama = hapus->nama;
    depan = depan->next;

    if (depan == nullptr) {
        belakang = nullptr;
    }
    delete hapus;
    return nama;
}

void hapusqueue(pelanggan*& depan, pelanggan*& belakang) {
    while (depan != nullptr) {
        dequeue(depan, belakang);
    }
}

bool hapusbelakangqueue(pelanggan*& depan, pelanggan*& belakang, string nama) {
    if (depan == nullptr) {
        return false;
    }

    if (depan == belakang) {
        if (depan->nama != nama) {
            return false;
        }
        delete depan;
        depan = nullptr;
        belakang = nullptr;
        return true;
    }

    if (belakang->nama != nama) {
        return false;
    }
    pelanggan* bantu = depan;

    while (bantu->next != belakang) {
        bantu = bantu->next;
    }
    delete belakang;
    belakang = bantu;
    belakang->next = nullptr;
    return true;
}

void push(riwayat*& top, int jenis, string namamenu, string namapelanggan = "", string pelanggandilayani = "") {
    riwayat* baru = new riwayat{
        jenis, namamenu, namapelanggan, pelanggandilayani, top
    };
    top = baru;
}

riwayat* pop(riwayat*& top) {
    if (top == nullptr) {
        return nullptr;
    }
    riwayat* hapus = top;
    top = top->next;
    hapus->next = nullptr;
    return hapus;
}

void hapusstack(riwayat*& top) {
    while (top != nullptr) {
        riwayat* hapus = pop(top);
        delete hapus;
    }
}

menu* carimenu(menu* root, string nama) {
    if (root == nullptr || root->nama == nama) {
        return root;
    }

    if (nama < root->nama) {
        return carimenu(root->kiri, nama);
    }
    return carimenu(root->kanan, nama);
}

void tambahmenu(menu*& root, string nama, string kategori, double harga) {
    if (root == nullptr) {
        root = new menu{
            nama, kategori, harga, true, nullptr, nullptr, nullptr, nullptr
        };
        return;
    }

    if (nama < root->nama) {
        tambahmenu(root->kiri, nama, kategori, harga);
    } else if (nama > root->nama) {
        tambahmenu(root->kanan, nama, kategori, harga);
    }
}

void tampilmenu(menu* root) {
    if (root == nullptr) {
        return;
    }
    tampilmenu(root->kiri);
    cout << "nama: " << root->nama << '\n';
    cout << "kategori: " << root->kategori << '\n';
    cout << "harga: Rp" << root->harga << '\n';
    if (root->tersedia == true) {
        cout << "status: tersedia\n";
    } else {
        cout << "status: habis\n";
    }
    tampilmenu(root->kanan);
}

menu* ambilminimum(menu*& root) {
    if (root->kiri == nullptr) {
        menu* hasil = root;
        root = root->kanan;
        hasil->kanan = nullptr;
        return hasil;
    }
    return ambilminimum(root->kiri);
}

bool hapusmenu(menu*& root, string nama) {
    if (root == nullptr) {
        return false;
    }

    if (nama < root->nama) {
        return hapusmenu(root->kiri, nama);
    }

    if (nama > root->nama) {
        return hapusmenu(root->kanan, nama);
    }

    if (root->kiri == nullptr) {
        menu* hapus = root;
        root = root->kanan;
        hapus->kanan = nullptr;
        hapusqueue(hapus->depan, hapus->belakang);
        delete hapus;
        return true;
    }

    if (root->kanan == nullptr) {
        menu* hapus = root;
        root = root->kiri;
        hapus->kiri = nullptr;
        hapusqueue(hapus->depan, hapus->belakang);
        delete hapus;
        return true;
    }
    menu* hapus = root;
    menu* penerus = ambilminimum(root->kanan);
    penerus->kiri = hapus->kiri;
    penerus->kanan = hapus->kanan;
    root = penerus;
    hapus->kiri = nullptr;
    hapus->kanan = nullptr;
    hapusqueue(hapus->depan, hapus->belakang);
    delete hapus;
    return true;
}

void hapussemuamenu(menu*& root) {
    if (root == nullptr) {
        return;
    }
    hapussemuamenu(root->kiri);
    hapussemuamenu(root->kanan);
    hapusqueue(root->depan, root->belakang);
    delete root;
    root = nullptr;
}

void tambahdatamenu(menu*& root) {
    string nama, kategori;
    double harga;
    cout << "nama menu: ";
    getline(cin >> ws, nama);

    if (carimenu(root, nama) != nullptr) {
        cout << "menu sudah ada\n";
        return;
    }
    cout << "kategori: ";
    getline(cin, kategori);
    cout << "harga: Rp";

    if (!(cin >> harga) || harga < 0) {
        cout << "harga tidak valid\n";
        cin.clear();
        cin.ignore(10000, '\n');
        return;
    }
    tambahmenu(root, nama, kategori, harga);
    cout << "menu berhasil ditambahkan\n";
}

void buatpesanan(menu* root, riwayat*& top) {
    string namapelanggan, namamenu;
    cout << "nama pelanggan: ";
    getline(cin >> ws, namapelanggan);
    cout << "nama menu: ";
    getline(cin, namamenu);
    menu* data = carimenu(root, namamenu);

    if (data == nullptr) {
        cout << "menu tidak ditemukan\n";
        return;
    }

    if (data->tersedia == true) {
        push(top, 1, namamenu, namapelanggan);
        cout << "pesanan " << namapelanggan << " berhasil dibuat\n";
    } else {
        enqueue(data->depan, data->belakang, namapelanggan);
        push(top, 2, namamenu, namapelanggan);
        cout << "menu sedang habis, " << namapelanggan;
        cout << " masuk ke dalam waiting list\n";
    }
}

void tandaimenuhabis(menu* root) {
    string namamenu;
    cout << "nama menu: ";
    getline(cin >> ws, namamenu);
    menu* data = carimenu(root, namamenu);

    if (data == nullptr) {
        cout << "menu tidak ditemukan\n";
        return;
    }

    if (data->tersedia == false) {
        cout << "menu sudah habis\n";
        return;
    }
    data->tersedia = false;
    cout << "status menu berhasil diubah menjadi habis\n";
}

void menutersedia(menu* root, riwayat*& top) {
    string namamenu;
    cout << "nama menu: ";
    getline(cin >> ws, namamenu);
    menu* data = carimenu(root, namamenu);

    if (data == nullptr) {
        cout << "menu tidak ditemukan\n";
        return;
    }

    if (data->tersedia == true) {
        cout << "menu sudah tersedia\n";
        return;
    }
    data->tersedia = true;
    string pelanggan = "";

    if (data->depan != nullptr) {
        pelanggan = dequeue(data->depan, data->belakang);
    }
    push(top, 3, namamenu, "", pelanggan);
    cout << "menu tersedia kembali\n";

    if (pelanggan != "") {
        cout << "pesanan langsung diberikan kepada " << pelanggan;
        cout << " dari waiting list\n";
    }
}

void hapusdatamenu(menu*& root) {
    string namamenu;
    cout << "nama menu yang dihapus: ";
    getline(cin >> ws, namamenu);

    if (hapusmenu(root, namamenu)) {
        cout << "menu berhasil dihapus\n";
    } else {
        cout << "menu tidak ditemukan\n";
    }
}

void undo(menu* root, riwayat*& top) {
    if (top == nullptr) {
        cout << "tidak ada aksi untuk di-undo\n";
        return;
    }
    riwayat* aksi = pop(top);
    menu* data = carimenu(root, aksi->namamenu);

    if (data == nullptr) {
        cout << "menu tidak ditemukan, undo tidak dapat dilakukan\n";
        delete aksi;
        return;
    }

    if (aksi->jenis == 1) {
        cout << "undo: pesanan menu " << aksi->namamenu;
        cout << " dibatalkan\n";
    } else if (aksi->jenis == 2) {
        if (hapusbelakangqueue(data->depan, data->belakang, aksi->namapelanggan)) {
            cout << "undo: pesanan menu " << aksi->namamenu;
            cout << " dibatalkan\n";
        } else {
            cout << "undo gagal dilakukan\n";
        }
    } else if (aksi->jenis == 3) {
        data->tersedia = false;

        if (aksi->pelanggandilayani != "") {
            enqueuedepan(data->depan, data->belakang, aksi->pelanggandilayani);
        }
        cout << "undo: menu " << aksi->namamenu;
        cout << " kembali habis\n";
    }
    delete aksi;
}

void tampilantrian(menu* root) {
    string namamenu;
    cout << "nama menu: ";
    getline(cin >> ws, namamenu);
    menu* data = carimenu(root, namamenu);

    if (data == nullptr) {
        cout << "menu tidak ditemukan\n";
        return;
    }

    if (data->depan == nullptr) {
        cout << "antrian kosong\n";
        return;
    }
    pelanggan* bantu = data->depan;

    while (bantu != nullptr) {
        cout << bantu->nama << " menunggu menu " << data->nama << '\n';
        bantu = bantu->next;
    }
}

int main() {
    menu* root = nullptr;
    riwayat* top = nullptr;
    int pilihan = -1;

    do {
        cout << "\nsistem pesanan restoran\n";
        cout << "1. tambah menu\n";
        cout << "2. tampilkan daftar menu\n";
        cout << "3. buat pesanan\n";
        cout << "4. tandai menu habis\n";
        cout << "5. catat menu tersedia kembali\n";
        cout << "6. hapus menu\n";
        cout << "7. undo aktivitas terakhir\n";
        cout << "8. tampilkan waiting list\n";
        cout << "0. keluar\n";
        cout << "pilihan: ";

        if (!(cin >> pilihan)) {
            cout << "input tidak valid\n";
            cin.clear();
            cin.ignore(10000, '\n');
            pilihan = -1;
            continue;
        }

        switch (pilihan) {
            case 1:
                tambahdatamenu(root);
                break;
            case 2:
                if (root == nullptr) {
                    cout << "belum ada data menu\n";
                } else {
                    tampilmenu(root);
                }
                break;
            case 3:
                buatpesanan(root, top);
                break;
            case 4:
                tandaimenuhabis(root);
                break;
            case 5:
                menutersedia(root, top);
                break;
            case 6:
                hapusdatamenu(root);
                break;
            case 7:
                undo(root, top);
                break;
            case 8:
                tampilantrian(root);
                break;
            case 0:
                cout << "program selesai\n";
                break;
            default:
                cout << "pilihan tidak tersedia\n";
        }

    } while (pilihan != 0);
    hapussemuamenu(root);
    hapusstack(top);
    return 0;
}
