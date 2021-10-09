#include <iostream>
#include<vector>
#include<string>
#include<regex>
#include<thread>
#include<exception>
#include<mutex>
#include<chrono>

using namespace std;

const char* crt = "\n---------------------------------------\n";

char* Alociraj(const char* sadrzaj) {
    if(sadrzaj == nullptr)
        return nullptr;
    int velicina = strlen(sadrzaj) + 1;
    char* temp = new char[velicina];
    strcpy_s(temp, velicina, sadrzaj);
    return temp;
}

class DatumVrijeme
{
    int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
public:
    DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0)
    {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
        _sati = new int(sati);
        _minuti = new int(minuti);
    }

    DatumVrijeme(const DatumVrijeme& original) {
        _dan =    new int(*original._dan);
        _mjesec = new int(*original._mjesec);
        _godina = new int(*original._godina);
        _sati =   new int(*original._sati);
        _minuti = new int(*original._minuti);
    }

    DatumVrijeme& operator=(const DatumVrijeme& original) {
        if (this!=&original)
        {
            delete _dan;
            delete _mjesec;
            delete _godina;
            delete _sati;
            delete _minuti;

            _dan = new int(*original._dan);
            _mjesec = new int(*original._mjesec);
            _godina = new int(*original._godina);
            _sati = new int(*original._sati);
            _minuti = new int(*original._minuti);
        }
        return *this;
    }

    ~DatumVrijeme()
    {
        delete _dan;    _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
        delete _sati;   _sati = nullptr;
        delete _minuti; _minuti = nullptr;
    }
    friend ostream& operator<< (ostream& COUT, DatumVrijeme& obj)
    {
        COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
        return COUT;
    }

    friend bool operator==(DatumVrijeme& d1, DatumVrijeme& d2) {
        if (*d1._dan == *d2._dan && *d1._mjesec == *d2._mjesec && *d1._godina == *d2._godina)
            return true;
        return false;
    }

};

template<class T1, class T2 = int>
class FITKolekcija
{
    T1* _elementi1;
    T2* _elementi2;
    int _trenutno;
public:
    FITKolekcija()
    {
        _elementi1 = nullptr;
        _elementi2 = nullptr;
        _trenutno = 0;
    }

    FITKolekcija(const FITKolekcija& original) {
        _trenutno = original._trenutno;
        _elementi1 = new T1[_trenutno];
        _elementi2 = new T2[_trenutno];
        for (size_t i = 0; i < _trenutno; i++)
        {
            _elementi1[i] = original._elementi1[i];
            _elementi2[i] = original._elementi2[i];
        }
    }

    FITKolekcija& operator=(const FITKolekcija& original) {
        if (this!=&original)
        {
            delete[] _elementi1;
            delete[] _elementi2;

            _trenutno = original._trenutno;
            _elementi1 = new T1[_trenutno];
            _elementi2 = new T2[_trenutno];
            for (size_t i = 0; i < _trenutno; i++)
            {
                _elementi1[i] = original._elementi1[i];
                _elementi2[i] = original._elementi2[i];
            }

        }
        return *this;
    }

    ~FITKolekcija()
    {
        delete[] _elementi1; _elementi1 = nullptr;
        delete[] _elementi2; _elementi2 = nullptr;
    }

    void Dodaj(const T1& t1, const T2& t2) {
        T1* temp1 = new T1[_trenutno + 1];
        T2* temp2 = new T2[_trenutno + 1];

        for (size_t i = 0; i < _trenutno; i++)
        {
            temp1[i] = _elementi1[i];
            temp2[i] = _elementi2[i];
        }
        delete[] _elementi1;
        delete[] _elementi2;

        temp1[_trenutno] = t1;
        temp2[_trenutno] = t2;

        _elementi1 = temp1;
        _elementi2 = temp2;

        _trenutno++;
    }

    void RemoveElement(int indeks) {
        if (indeks < 0 || indeks >= _trenutno)
            return;
        T1* temp1 = new T1[_trenutno];
        T2* temp2 = new T2[_trenutno];
        int brojac = 0;

        for (size_t i = 0; i < _trenutno; i++)
        {
            if (i != indeks) {
                temp1[brojac] = _elementi1[i];
                temp2[brojac++] = _elementi2[i];
            }
        }
        delete[] _elementi1;
        delete[] _elementi2;

        _elementi1 = temp1;
        _elementi2 = temp2;

        _trenutno--;
    }

    void Sortiraj() {
        bool flag = true;
        while (flag)
        {
            flag = false;
            for (size_t i = 0; i < _trenutno-1; i++)
            {
                if (_elementi1[i]>_elementi1[i+1])
                {
                    T1 temp1 = _elementi1[i];
                    _elementi1[i] = _elementi1[i + 1];
                    _elementi1[i + 1] = temp1;

                    T2 temp2 = _elementi2[i];
                    _elementi2[i] = _elementi2[i + 1];
                    _elementi2[i + 1] = temp2;

                    flag = true;
                }
            }
        }
    }

    void DodajNaLokaciju(const T1& t1, const T2& t2, int indeks) {
        if (indeks >= _trenutno || indeks < 0)
            return;
        T1* temp1 = new T1[_trenutno + 1];
        T2* temp2 = new T2[_trenutno + 1];
        int brojac = 0;

        for (size_t i = 0; i < _trenutno; i++)
        {
            if (i!=indeks) {
                temp1[i] = _elementi1[brojac];
                temp2[i] = _elementi2[brojac++];
            }
            else {
                temp1[i] = t1;
                temp2[i] = t2;
                _trenutno++;
            }
        }

        delete[] _elementi1;
        delete[] _elementi2;

        _elementi1 = temp1;
        _elementi2 = temp2;

    }

    bool PostojiLi(FITKolekcija* nova, const T1& t1, const T2& t2) {
        for (size_t i = 0; i < nova->_trenutno; i++)
            if (nova->_elementi1[i] == t1 && nova->_elementi2[i] == t2)
                return true;
        return false;
        
    }

    FITKolekcija<T1, T2>* GetJedinstveni() {
        FITKolekcija<T1, T2>* nova = new FITKolekcija<T1, T2>;
        for (size_t i = 0; i < _trenutno; i++)
        {
            if (!PostojiLi(nova, _elementi1[i], _elementi2[i]))
                nova->Dodaj(_elementi1[i], _elementi2[i]);
        }
        return nova;
    }

    T1* GetT1() { return _elementi1; }
    T2* GetT2() { return _elementi2; }
    int GetTrenutno() { return _trenutno; }

    friend ostream& operator<< (ostream& COUT, FITKolekcija& obj)
    {
        for (size_t i = 0; i < obj._trenutno; i++)
            COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
        return COUT;
    }
};

enum vrstaDokumenta { PDF, DOC, TXT, HTML };
const char* charVrstaDokumenta[]{"PDF", "DOC", "TXT", "HTML"};
class Dokument
{
    vrstaDokumenta _vrsta;
    string _naziv;
    string _sadrzaj;
    int _brojStranica;
public:
    Dokument() { _brojStranica = 0; }
    Dokument(vrstaDokumenta vrsta, string naziv)
    {
        _naziv = naziv; _vrsta = vrsta; _brojStranica = 0;
    }

    Dokument(const Dokument& original) {
        _vrsta = original._vrsta;
        _naziv = original._naziv;
        _sadrzaj = original._sadrzaj;
        _brojStranica = original._brojStranica;
    }


    void DodajSadrzaj(string sadrzaj) {
        _sadrzaj += sadrzaj;
        _brojStranica = (strlen(_sadrzaj.c_str())) / 30;
    }

    string GetNaziv() { return _naziv; };
    string GetSadrzaj() { return _sadrzaj; };
    vrstaDokumenta GetVrsta() { return _vrsta; };
    void UvecajBrojStranica() { _brojStranica++; }
    int GetBrojStranica() { return _brojStranica; }
    friend ostream& operator<<(ostream& COUT, Dokument& obj)
    {
        COUT << obj._naziv << "." << obj._vrsta << " (" << obj._brojStranica << ")\n" << obj._sadrzaj << endl;
        return COUT;
    }
};

class Uredjaj
{
protected:
    char* _proizvodjac;
    char* _model;
public:
    Uredjaj(const char* proizvodjac, const char* model)
    {
        _proizvodjac = Alociraj(proizvodjac);
        _model = Alociraj(model);
    }
    ~Uredjaj()
    {
        delete[] _proizvodjac; _proizvodjac = nullptr;
        delete[] _model;       _model = nullptr;
    }

    Uredjaj(const Uredjaj& original) {
        _proizvodjac = Alociraj(original._proizvodjac);
        _model = Alociraj(original._model);
    }

    Uredjaj& operator=(const Uredjaj& original) {
        if (this!=&original)
        {
            delete[] _proizvodjac;
            delete[] _model;

            _proizvodjac = Alociraj(original._proizvodjac);
            _model = Alociraj(original._model);

        }
        return *this;
    }

    friend ostream& operator<<(ostream& COUT, Uredjaj& obj)
    {
        COUT << obj._proizvodjac << "." << obj._model << endl;
        return COUT;
    }
};

mutex cuvar;

class Printer:public Uredjaj
{
    FITKolekcija<DatumVrijeme, Dokument> _printaniDokumenti;
    vector<string> _zabranjeneRijeci;
public:
    Printer(const char* proizvodjac, const char* model) :Uredjaj(proizvodjac, model) {}
    Printer(const Printer& original) :Uredjaj(original) {
        _printaniDokumenti = original._printaniDokumenti;
        _zabranjeneRijeci = original._zabranjeneRijeci;
    }

    Printer& operator=(const Printer& original) {
        if (this!=&original)
        {
            Uredjaj::operator=(original);
            _printaniDokumenti = original._printaniDokumenti;
            _zabranjeneRijeci = original._zabranjeneRijeci;
        }
        return *this;
    }

    bool SadrziRijec(string rijec) {
        vector<string>::iterator it;
        for (it = _zabranjeneRijeci.begin(); it != _zabranjeneRijeci.end(); it++)
            if (*it == rijec)
                return true;
        return false;
    }

    void DodajZabranjenuRijec(string zabranjenaRijec) {
        if (!SadrziRijec(zabranjenaRijec))
            _zabranjeneRijeci.push_back(zabranjenaRijec);
    }

    bool IspravanNazivDokument(string naziv) {
        string pravilo = "[a-zA-Z]{5,15}\\.(";
        for (int i = 0; i < 4; i++) {
            string temp = charVrstaDokumenta[i];
            for (int j = 0; j < strlen(temp.c_str()); j++) {
                temp[j] = tolower(temp[j]);
            }
            pravilo += temp;
            if (i != 3)
                pravilo += "|";
        }
        pravilo += ")";
        if (!regex_match(naziv, regex(pravilo)))
            return false;
        return true;
    }

    bool SadrziNedozvoljenuRijec(string sadrzaj) {
        string pravilo = "(";
        for (int i = 0; i < _zabranjeneRijeci.size(); i++) {
            pravilo += _zabranjeneRijeci[i];
            if (i != _zabranjeneRijeci.size() - 1)
                pravilo += "|";
        }
        pravilo += ")";
        if (regex_search(sadrzaj, regex(pravilo)))
            return true;
        return false;
    }
    void Printaj(DatumVrijeme& datum, Dokument& dokument) {
        _printaniDokumenti.Dodaj(datum, dokument);
        if (!IspravanNazivDokument(dokument.GetNaziv()) || SadrziNedozvoljenuRijec(dokument.GetSadrzaj()))
            throw exception("Nesto ne stima.\n");
        thread t1([&]() {
            cuvar.lock();
            this_thread::sleep_for(chrono::seconds(2));
            cout << endl << endl << endl << dokument.GetSadrzaj();
            cuvar.unlock();
            });
        t1.join();
    }
    float GetProsjecanBrojStranicaPoDatumu(DatumVrijeme& datum) {
        float suma = 0; int brojac = 0;
        for (size_t i = 0; i < _printaniDokumenti.GetTrenutno(); i++)
        {
            if (_printaniDokumenti.GetT1()[i] == datum) {
                suma += _printaniDokumenti.GetT2()[i].GetBrojStranica();
                brojac++;
            }
        }
        return suma / brojac;
    }

    void UkloniDokumente() {
        for (size_t i = 0; i < _printaniDokumenti.GetTrenutno(); i++)
        {
            if (!IspravanNazivDokument(_printaniDokumenti.GetT2()[i].GetNaziv()) || SadrziNedozvoljenuRijec(_printaniDokumenti.GetT2()[i].GetSadrzaj()))
                _printaniDokumenti.RemoveElement(i);
        }
    }

    friend ostream& operator<<(ostream& ispis, Printer& printer) {
        ispis << (*(Uredjaj*)(&printer)) << endl;
        ispis << crt << "PRINTANI DOKUMENTI: " << crt;
        ispis << printer._printaniDokumenti << crt << "ZABRANJENE RIJECI: " << crt;
        for (int i = 0; i < printer._zabranjeneRijeci.size(); i++)
            ispis << printer._zabranjeneRijeci[i] << ", ";
        ispis << crt;
        return ispis;
    }

    FITKolekcija<DatumVrijeme, Dokument>& GetPrintaniDokumenti() { return _printaniDokumenti; };
    vector<string>& GetZabranjeneRijeci() { return _zabranjeneRijeci; };
};

void main()
{
    /****************************************************************************
    // 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
    // 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR
    //    ÆE BITI OZNACENO KAO "RE"
    // 3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
    // 4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
    // 5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U,
    //    OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE
    //    IMENOVATI I DODAVATI PO ŽELJI.
    // 6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
    ****************************************************************************/
#pragma region TestiranjeDatumVrijeme
    DatumVrijeme danas(1, 2, 2017, 10, 15);
    DatumVrijeme sutra(danas);
    DatumVrijeme prekosutra;
    prekosutra = danas;
    cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region FITKolekcija
    int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

    FITKolekcija<int, int> brojevi;
    brojevi.Dodaj(v196, v6);
    brojevi.Dodaj(v13, v32);
    brojevi.Dodaj(v98, v196);
    brojevi.Dodaj(v63, v13);
    brojevi.Dodaj(v98, v196);
    brojevi.Dodaj(v196, v6);

    cout << brojevi << crt;
    ////SORTIRANJE ÈLANOVA KOLEKCIJE SE VRŠI U RASTUÆEM REDOSLIJEDU NA OSNOVU VRIJEDNOSTI ELEMENTA TIPA T1
    brojevi.Sortiraj();
    cout << brojevi << crt;
    //BROJ 2 SE ODNOSI NA LOKACIJU/INDEKS UNUTAR LISTE NA KOJI JE POTREBNO DODATI NOVE ELEMENTE
    brojevi.DodajNaLokaciju(v109, v6, 2);
    cout << brojevi << crt;
    brojevi.Sortiraj();
    cout << brojevi << crt;
    // METODA GetJedinstveni VRAÆA LISTU JEDINSTVENIH ELEMENATA TJ. ELEMENATA KOJI NE SADRŽE DUPLIKATE 
    // (POJAM DUPLIKAT SE ODNOSI NA ISTE VRIJEDNOSTI ELEMENATA T1 I T2). ELEMENTI KOJI SE DUPLIRAJU SE U 
    // OVOJ LISTI TREBAJU POJAVITI SAMO JEDNOM.
    FITKolekcija<int, int>* jedinstveni = brojevi.GetJedinstveni();
    cout << *jedinstveni << crt;
    *jedinstveni = brojevi;
    cout << *jedinstveni << crt;

    Dokument ispitPRIII(DOC, "ispitPRIII.doc");
    //BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREÐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO
    //DA NA JEDNU STRANICU MOŽE STATI 30 ZNAKOVA UKLJUÈUJUÆI I RAZMAKE
    ispitPRIII.DodajSadrzaj("NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO RE");
    cout << "Broj stranica -> " << ispitPRIII.GetBrojStranica() << endl;
    Dokument ispitBaze(DOC, "ispitBaze.doc");
    ispitBaze.DodajSadrzaj("PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT");
    cout << ispitBaze << endl;//ISPISUJE SVE DOSTUPNE PODATKE O DOKUMENTU

    Printer hp420("HP", "420");

    Printer hp3200("HP", "3200");
    //PRINTER NEÆE DOZVOLITI PRINTANJE DOKUMENATA U ÈIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEÈI
    hp3200.DodajZabranjenuRijec("RE");// :)
    hp3200.DodajZabranjenuRijec("RAT");
    hp3200.DodajZabranjenuRijec("UBITI");
    hp3200.DodajZabranjenuRijec("RE");// ONEMOGUÆITI PONAVLJANJE ZABRANJENIH RIJEÈI, KORISTITI ITERATORE
    hp3200.DodajZabranjenuRijec("MRZITI");
    try
    {
        // DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEÆA PRAVILA:
        // 1. NAZIV DOKUMENTA MORA BITI U SLJEDEÆEM FORMATU npr: ispitPRIII.doc
        // NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE 
        // BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
        // 2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEÈ
        // UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA 
        // ODGOVARAJUÆOM PORUKOM, ALI ÆE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti. 
        // UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI 
        // SADRŽAJ DOKUMENTA KOJI SE PRINTA

        hp3200.Printaj(danas, ispitPRIII);
        hp3200.Printaj(danas, ispitBaze);


        // IMPLEMENTIRATI PRINTANJE U ZASEBNIM NITIMA UZ KORIŠTENJE sleep_for FUNKCIJE
        // VODITI RAÈUNA O DIJELJENJU ZAJEDNIÈKIH RESURSA
    }
    catch (exception & err)
    {
        cout << err.what() << endl;
    }

    //KOD POREÐENJA DATUMA PRINTANJA NIJE POTREBNO POREDITI MINUTE, SAMO DATUM I SATE
    cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp3200.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
    //cout << "Najcesce koristena zabranjena rijec je -> " << hp3200.GetTopZabranjenuRijec() << crt;

    Printer hp4000(hp3200);
    cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp4000.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
    //cout << "Najcesce koristena zabranjena rijec je -> " << hp4000.GetTopZabranjenuRijec() << crt;
    hp4000.UkloniDokumente();//UKLANJA SVE DOKUMENTE KOJI NISU ZADOVOLJILI USLOVE ZA PRINTANJE
    cout << hp4000 << crt;

#pragma endregion
}