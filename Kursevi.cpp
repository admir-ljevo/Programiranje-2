#include <iostream>
#include<string>
#include<vector>
#include<exception>
#include<thread>
#include<regex>
#include<mutex>



using namespace std;

bool ProvjeriFormatTelefona(string sadrzaj) {
    string pravilo = "(\\()(\\d{3})(\\))(\\d{3})[-](\\d{3})";
    if (regex_match(sadrzaj, regex(pravilo)))
        return true;
    return false;
}

class Izuzetak :public exception
{
    // Datum _datum;//KORISTITE DANASNJI DATUM
    string _funkcija;//FUNKCIJA U KOJOJ JE NASTAO IZUZETAK
    int _linija;//LINIJA CODE-A U KOJOJ JE NASTAO IZUZETAK
public:
     Izuzetak(const char* poruka, string funkcija, int linija): exception(poruka), _funkcija(funkcija), _linija(linija){}
     friend ostream& operator<<(ostream& cout, const Izuzetak& iz) {
         cout << "Greska: " << iz.what() << endl;
         cout << "Funkcija: " << iz._funkcija << endl;
         cout << "Linija: " << iz._linija << endl;
         return cout;
     }

};

char* AlocirajNizKaraktera(const char* sadrzaj) {
    if (sadrzaj == nullptr)
        return nullptr;
    int velicina = strlen(sadrzaj) + 1;
    char* temp = new char[velicina];
    strcpy_s(temp, velicina, sadrzaj);
    return temp;
}

template<class T1, class T2, int max>
class Kolekcija
{
    T1 _elementi1[max];
    T2 _elementi2[max];
    int* _trenutno;
public:
    Kolekcija() { _trenutno = new int(0); }
    Kolekcija(const Kolekcija& original) {
        _trenutno = new int(*original._trenutno);
        for (size_t i = 0; i < *_trenutno; i++)
        {
            _elementi1[i] = original._elementi1[i];
            _elementi2[i] = original._elementi2[i];
        }
    }

    Kolekcija& operator=(const Kolekcija& original) {
        if (this!=&original)
        {
            delete _trenutno;
            _trenutno = new int(*original._trenutno);
            for (size_t i = 0; i < *_trenutno; i++)
            {
                _elementi1[i] = original._elementi1[i];
                _elementi2[i] = original._elementi2[i];
            }
        }
        return *this;
    }

    ~Kolekcija() { delete _trenutno; _trenutno = nullptr; }
    int GetTrenutno() const { return *_trenutno; }
    T1& GetElement1(int indeks)  {
        if (indeks < 0 || indeks >= *_trenutno)
            throw Izuzetak("Nepostojeca lokacija.", __FUNCTION__, __LINE__);
        return _elementi1[indeks];
    }

    T2& GetElement2(int indeks)   {
        if (indeks < 0 || indeks >= *_trenutno)
            throw Izuzetak("Nepostojeca lokacija.", __FUNCTION__, __LINE__);
        return _elementi2[indeks];
    }

    int GetMax()const { return max; }
    int size() { return *_trenutno; }
    T1& operator[](int indeks) { return _elementi1[indeks]; }

    void AddElement(T1 t1, T2 t2) {
        if (*_trenutno >= max)
            throw Izuzetak("Prekoracenje opsega.", __FUNCTION__, __LINE__);
        _elementi1[*_trenutno] = t1;
        _elementi2[*_trenutno] = t2;
        (*_trenutno)++;
    }

    friend ostream& operator<<(ostream& cout,  Kolekcija& k) {
        for (size_t i = 0; i < k.GetTrenutno(); i++)
            cout << k.GetElement1(i) << "  " << k.GetElement2(i) << endl;
        return cout;
       
    }

    friend bool operator==(Kolekcija& k1, Kolekcija& k2) {
        if (k1.GetTrenutno() != k2.GetTrenutno())
            return false;
        for (size_t i = 0; i < k1.GetTrenutno(); i++)
        {
            if (!(k1.GetElement1(i) == k2.GetElement1(i)) || !(k1.GetElement2(i) == k2.GetElement2(i)))
                return false;
        }
        return true;
    }

};

class Datum
{
    int* _dan, * _mjesec, * _godina;
public:
    Datum(int dan = 1, int mjesec = 1, int godina = 2000)
    {
        _dan = new int(dan);
        _mjesec = new int(mjesec);
        _godina = new int(godina);
    }

    Datum(const Datum& original) {
        _dan =    new int(*original._dan);
        _mjesec = new int(*original._mjesec);
        _godina = new int(*original._godina);
    }

    Datum& operator=(const Datum& original) {
        if (this!=&original)
        {
            delete _dan;
            delete _mjesec;
            delete _godina;

            _dan = new int(*original._dan);
            _mjesec = new int(*original._mjesec);
            _godina = new int(*original._godina);
        }
        return *this;
    }

    int intoDays() { return *_dan + *_mjesec * 30 + *_godina * 365; }

    ~Datum()
    {
        delete _dan;    _dan = nullptr;
        delete _mjesec; _mjesec = nullptr;
        delete _godina; _godina = nullptr;
    }
    friend ostream& operator<< (ostream& COUT,const Datum& obj)
    {
        COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
        return COUT;
    }
};



enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
const char* kurseviIspis[]{ "HTMLCSSJavaScript", "SoftwareEngeneeringFundamentals", "MasteringSQL", "WindowsSecurity" };






class Kurs
{
    enumKursevi _kurs;
    Datum _pocetak;
    Datum _kraj;
    char* _imePredavaca;
public:
    Kurs(enumKursevi kurs, const char* imePredavaca, Datum pocetak, Datum kraj) {
        _kurs = kurs;
        _imePredavaca = AlocirajNizKaraktera(imePredavaca);
        _pocetak = pocetak;
        _kraj = kraj;
    }

    Kurs(){}

    Kurs(const Kurs& original) {
        _kurs = original._kurs;
        _pocetak = original._pocetak;
        _kraj = original._kraj;
        _imePredavaca = AlocirajNizKaraktera(original._imePredavaca);
    }

    Kurs& operator=(const Kurs& original) {
        if (this!=&original)
        {
            delete[] _imePredavaca;
            _kurs = original._kurs;
            _pocetak = original._pocetak;
            _kraj = original._kraj;
            _imePredavaca = AlocirajNizKaraktera(original._imePredavaca);
        }
        return *this;
    }

    ~Kurs() { delete[] _imePredavaca; _imePredavaca = nullptr; }

    Datum GetPocetak() { return _pocetak; }
    Datum GetKraj() { return _kraj; }
    const char* GetImePredavaca() { return _imePredavaca; }
    enumKursevi GetKurs() { return _kurs; }

    friend bool operator==( Kurs& k1, Kurs& k2) {
        if (strcmp(k1._imePredavaca, k2._imePredavaca) == 0 && k1._kurs == k2._kurs)
            return true;
        return false;
    }
    friend ostream& operator<<(ostream& cout,  Kurs& k) {
        cout << "Ime predavavaca: " << k._imePredavaca << endl;
        cout << "Naziv kursa: " <<kurseviIspis[k._kurs]<< endl;
        cout << "Datum pocetka: " << k._pocetak << endl;
        cout << "Datum zavrsetka: " << k._kraj << endl;
        return cout;
    }

};



class Polaznik
{
    static int ID;
    const int _polaznikID;
    //SVAKOM NOVOM POLAZNIKU AUTOMATSKI DODIJELITI NOVI ID (AUTOINCREMENT) POCEVSI OD BROJA 1
    char* _imePrezime;
    string _kontaktTelefon; //BROJ TELEFONA TREBA BITI U FORMATU (06X)XXX-XXX 
    Kolekcija<Kurs*, int, 10> _uspjesnoOkoncaniKursevi;
    //INT PREDSTAVLJA OSTVARENI PROCENAT NA ISPITU, A JEDAN POLAZNIK MOZE POLOZITI NAJVISE 10 KURSEVA
public:
    Polaznik(const char* imePrezime, string telefon) : _polaznikID(ID++)
    {
        int size = strlen(imePrezime) + 1;
        _imePrezime = new char[size];
        strcpy_s(_imePrezime, size, imePrezime);
        _kontaktTelefon = telefon;
    }

    Polaznik(const Polaznik& original) : _polaznikID(original._polaznikID) {
        ID = original.ID;
        _imePrezime = AlocirajNizKaraktera(original._imePrezime);
        _kontaktTelefon = original._kontaktTelefon;
        for (size_t i = 0; i < _uspjesnoOkoncaniKursevi.size(); i++)
        {
            _uspjesnoOkoncaniKursevi.AddElement(new Kurs(*const_cast<Polaznik*>(&original)->_uspjesnoOkoncaniKursevi.GetElement1(i)), const_cast<Polaznik&>(original)._uspjesnoOkoncaniKursevi.GetElement2(i));
        }
    }

    Polaznik& operator=(const Polaznik& original)  {
        if (this!=&original)
        {
            delete[] _imePrezime; _imePrezime = nullptr;
            for (size_t i = 0; i < _uspjesnoOkoncaniKursevi.size(); i++)
            {
                delete _uspjesnoOkoncaniKursevi[i];
                _uspjesnoOkoncaniKursevi[i] = nullptr;
            }
        }
            ID = original.ID;
            _imePrezime = AlocirajNizKaraktera(original._imePrezime);
            _kontaktTelefon = original._kontaktTelefon;
            for (size_t i = 0; i < _uspjesnoOkoncaniKursevi.size(); i++)
                _uspjesnoOkoncaniKursevi.AddElement(new Kurs(*const_cast<Polaznik*>(&original)->_uspjesnoOkoncaniKursevi.GetElement1(i)), const_cast<Polaznik&>(original)._uspjesnoOkoncaniKursevi.GetElement2(i));
            
            
        
        return *this;
    }

    int GetID() { return _polaznikID; }

    Kolekcija<Kurs*, int, 10>& GetUspjesniOkoncaniKursevni() { return _uspjesnoOkoncaniKursevi; }


    const char* GetImePrezime() { return _imePrezime; }

    ~Polaznik()
    {
        delete[] _imePrezime; _imePrezime = nullptr;
        for (size_t i = 0; i < _uspjesnoOkoncaniKursevi.size(); i++)
        {
            delete _uspjesnoOkoncaniKursevi.GetElement1(i);
            _uspjesnoOkoncaniKursevi.GetElement1(i) = nullptr;
        }
    }

    friend ostream& operator<<(ostream& cout, Polaznik& p) {
        cout << "Ime polaznika: " << p._imePrezime << endl;
        cout << "Kontakt telefona: " << p._kontaktTelefon << endl;
        cout << "ID: " << p._polaznikID << endl;

        return cout;
    }

    int GetProcenat(enumKursevi kurs) {
        for (int i = 0; i < _uspjesnoOkoncaniKursevi.GetTrenutno(); i++)
            if (_uspjesnoOkoncaniKursevi.GetElement1(i)->GetKurs() == kurs)
                return _uspjesnoOkoncaniKursevi.GetElement2(i);
        return 0;
    }

    friend bool operator==( Polaznik& p1,  Polaznik& p2) {
        if (strcmp(p1._imePrezime, p2._imePrezime) == 0 && p1._uspjesnoOkoncaniKursevi == p2._uspjesnoOkoncaniKursevi)
            return true;
        return false;
    }

};

int Polaznik::ID = 1;

class SkillsCentar
{
    string _nazivCentra;
    vector<Kurs> _kursevi;//KURSEVI KOJE NUDI ODREDJENI CENTAR
    Kolekcija<Kurs, Polaznik*, 150> _aplikanti;
public:
    SkillsCentar(string naziv) { _nazivCentra = naziv; }
    SkillsCentar(const SkillsCentar& obj) :_kursevi(obj._kursevi), _aplikanti(obj._aplikanti)
    {
        _nazivCentra = obj._nazivCentra;
    }

    SkillsCentar& operator=(const SkillsCentar& original) {
        if (this!=&original)
        {
            _nazivCentra = original._nazivCentra;
            _kursevi = original._kursevi;
            _aplikanti = original._aplikanti;
        }
        return *this;
    }

    bool ProvjeriKoliziju(Kurs& kurs) {
        for (size_t i = 0; i < _kursevi.size(); i++)
        {
            if ((_kursevi[i].GetPocetak().intoDays() < kurs.GetPocetak().intoDays() && kurs.GetPocetak().intoDays() < _kursevi[i].GetKraj().intoDays())
                || (kurs.GetPocetak().intoDays() < _kursevi[i].GetPocetak().intoDays() && kurs.GetKraj().intoDays() > _kursevi[i].GetPocetak().intoDays()))
            {
                cout << "Kolizija\n";
                return false;
            }
        }
        return true;

        

    }

    Kolekcija<Kurs,Polaznik*, 150>& GetAplikaciju() { return _aplikanti; }
     
    void AddKurs(Kurs& kurs) {
        for (size_t i = 0; i < _kursevi.size(); i++)
        {
            if (_kursevi[i] == kurs) {
                throw Izuzetak("Ne moze dupli kurs.", __FUNCTION__, __LINE__);
            }
        }
        _kursevi.push_back(kurs);
        cout << "Kurs " << kurs << " uspjesno dodan" << endl;

    }

    void AddAplikaciju(Kurs& kurs, Polaznik& polaznik) {
        int indeks=-1;
        for (size_t i = 0; i < _kursevi.size(); i++)
            if (_kursevi[i] == kurs)
                indeks = i;
        
        if(indeks==-1)
            throw Izuzetak("Ne moze. ", __FUNCTION__, __LINE__);

        for (size_t i = 0; i < _aplikanti.GetTrenutno(); i++)
        {
            if (_aplikanti.GetElement1(i) == kurs && strcmp(_aplikanti.GetElement2(i)->GetImePrezime(), polaznik.GetImePrezime())==0)
                throw Izuzetak("Ne moze. ", __FUNCTION__, __LINE__);
        }
        cout << "DODANO\n";
        _aplikanti.AddElement(kurs, &polaznik);

    }

    void DodajUspjesnoOkoncanKurs(int id, Kurs& kurs, int procenat) {
        for (size_t i = 0; i < _aplikanti.GetTrenutno(); i++)
        {
            if (_aplikanti.GetElement1(i) == kurs && _aplikanti.GetElement2(i)->GetID() == id && procenat > 55) {
                _aplikanti.GetElement2(i)->GetUspjesniOkoncaniKursevni().AddElement(new Kurs(kurs), procenat);
                return;
            }
        }
        throw Izuzetak("Ne moze. ", __FUNCTION__, __LINE__);

    }

    vector<Polaznik> GetPolazniciByPredavac(const char* imePredavaca, enumKursevi kurs) {
        vector<Polaznik> polaznici;
        for (size_t i = 0; i < _aplikanti.size(); i++)
        {
            if (strcmp(_aplikanti.GetElement1(i).GetImePredavaca(), imePredavaca) == 0 && _aplikanti.GetElement1(i).GetKurs() == kurs && _aplikanti.GetElement2(i)->GetProcenat(kurs) > 55)
                polaznici.push_back(*_aplikanti.GetElement2(i));
        }
        return polaznici;
    }


    bool RemoveKurs(Kurs& kurs) {

        if (_aplikanti.GetTrenutno() != 0) {
            for (size_t i = 0; i < _aplikanti.size(); i++)
                if (_aplikanti.GetElement1(i) == kurs)
                    return false;
        }


        for (size_t i = 0; i < _kursevi.size(); i++)
        {
            if (_kursevi[i] == kurs) {
                _kursevi.erase(_kursevi.begin() + i);
                return true;
            }
        }
        return false;

    }

    friend ostream& operator<<(ostream& cout, SkillsCentar& sc) {
        cout << "Naziv centra: " << endl;
        cout << "Kursevi: " << endl;
        for (size_t i = 0; i < sc._kursevi.size(); i++)
        {
            cout << sc._kursevi[i] << endl;
        }
        cout << "Aplikanti: " << endl;
        for (size_t i = 0; i < sc._aplikanti.GetTrenutno(); i++)
        {
            cout << sc._aplikanti[i] << endl;
        }
        return cout;
    }

};
const char* crt = "\n---------------------------------------\n";
void main()
{
    /****************************************************************************
    // 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
    // 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU
    //    RUNTIME ERROR ÆE BITI OZNACENO KAO "RE"
    // 3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
    // 4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
    // 5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U
    //    TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE,
    //    POMOÆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
    // 6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
    ****************************************************************************/

#pragma region TestiranjeDatuma
    Datum danas(26, 11, 2015);
    Datum sutra(danas);
    Datum prekosutra;
    prekosutra = danas;
    cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region TestiranjeKolekcije
    Kolekcija<int, int, 10> kolekcija1;
    try
    {
        for (size_t i = 0; i < kolekcija1.GetMax(); i++)
            kolekcija1.AddElement(i, i + 2);
        //FUNKCIJA TREBA BACITI IZUZETAK U SLUCAJ DA NEMA VISE PROSTORA ZA DODAVANJE NOVIH ELEMENATA
        cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
        cout << kolekcija1.GetElement1(20) << " " << kolekcija1.GetElement2(20) << endl;
        //FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO ELEMENT NA ZAHTIJEVANOJ LOKACIJI NE POSTOJI
    }
    catch (Izuzetak & err)
    {
        cout << "Greska -> " << err << endl;
    }

    cout << kolekcija1 << endl;
    Kolekcija<int, int, 10> kolekcija2;
    kolekcija2 = kolekcija1;
    cout << kolekcija2 << crt;
    if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
        cout << "ISTI BROJ ELEMENATA" << endl;
    Kolekcija<int, int, 10> kolekcija3(kolekcija2);
    cout << kolekcija3 << crt;
#pragma endregion

#pragma region TestiranjeKursa
    Kurs sef(SoftwareEngeneeringFundamentals, "Jasmin Azemovic", Datum(28, 1, 2016), Datum(15, 2, 2016));
    cout << sef << endl;
    Kurs msql(MasteringSQL, "Adel Handzic", Datum(28, 2, 2016), Datum(15, 3, 2016));
    sef = msql;
    if (sef == msql)//KURSEVI SU ISTI AKO POSJEDUJU IDENTICNE VRIJEDNOSTI SVIH ATRIBUTA
        cout << "ISTI SU KURSEVI" << endl;
    Kurs msql2(msql);
    cout << msql2 << endl;
#pragma endregion
//
#pragma region TestiranjePolaznika
    string telefon1 = "(061)111-111";
    string telefon2 = "(061)111222";
    string telefon3 = "(061)111-333";

    if (!ProvjeriFormatTelefona(telefon1))
        cout << "Broj " << telefon1 << " nije validan" << endl;
    if (!ProvjeriFormatTelefona(telefon2))
        cout << "Broj " << telefon2 << " nije validan" << endl;
    if (!ProvjeriFormatTelefona(telefon3))
        cout << "Broj " << telefon3 << " nije validan" << endl;


    Polaznik denis("Denis Music", telefon1);
    Polaznik denis2("Denis2 Music2", telefon3);

#pragma endregion

#pragma region TestiranjeCentra
    try
    {
        SkillsCentar mostar("Skills Center Mostar");
        if (mostar.ProvjeriKoliziju(sef)) 
            mostar.AddKurs(sef);
           
        
        //KURSA SE NE SMIJE POKLAPAT
                                             //JEDAN KURS NE SMIJE POCETI DOK DRUGI TRAJE TJ. VRIJEME ODRZAVANJA I
       /* if (mostar.ProvjeriKoliziju(msql))
            mostar.AddKurs(msql);*/
       /* if (mostar.ProvjeriKoliziju(msql2))
            mostar.AddKurs(msql2);*/
        //ONEMOGUCITI DODAVANJE IDENTICNIH KURSEVA. FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO SE POKUSA DODATI IDENTICAN KURS
        mostar.AddAplikaciju(sef, denis);
        ////ONEMOGUCITI APLICIRANJE ZA KURSEVE KOJI NISU REGISTROVANI U CENTRU
        //mostar.AddAplikaciju(msql, denis);
        //mostar.AddAplikaciju(msql, denis);
        cout<<mostar.GetAplikaciju().GetTrenutno();
        //ONEMOGUCITI APLICIRANJE ZA ISTI KURS. FUNKCIJA BAZA IZUZETAK
        cout << denis2 << endl;
        denis2 = denis;
        if (denis == denis2) //POLAZNICI SU ISTI AKO POSJEDUJU ISTO IME I AKO SU POHADJALI ISTE KURSEVE
            cout << "ISTI SU" << endl;
//
        mostar.DodajUspjesnoOkoncanKurs(1, sef, 60);
        // BROJ 1 OZNACAVA ID POLAZNIKA. FUNKCIJA JE ZADUZENA DA POLAZNIKU 
        // DODA INFORMACIJU O USPJESNO POLOZENOM KURSU KOJI JE POSLAN KAO PARAMETAR. PREDUSLOV ZA DODAVANJE JE DA JE 
        // POLAZNIK PRETHODNO APLICIRAO ZA TAJ KURS, TE DA JE NA ISPITU OSTVARIO VISE OD 55%
        mostar.DodajUspjesnoOkoncanKurs(1, msql, 83);
        vector<Polaznik> listaPolaznika = mostar.GetPolazniciByPredavac("Jasmin Azemovic", SoftwareEngeneeringFundamentals);
//        //VRACA INFORMACIJE O POLAZNICIMA KOJI SU KOD ODREDJENOG PREDAVACA USPJESNO OKONCALI ODREDJENI KURS
//
        if (mostar.RemoveKurs(sef))//ONEMOGUCITI UKLANJANJE KURSA KOJI SU VEC PRIJAVILI NEKI OD POLAZNIKA
            cout << "Kurs " << sef << " uspjesno uklonjen iz centra " << mostar << endl;
//
        SkillsCentar sarajevo = mostar;
        cout << sarajevo << endl;
    }
    catch (Izuzetak & err)
    {
        cout << err << endl;
    }
    
#pragma endregion
    system("pause");
}