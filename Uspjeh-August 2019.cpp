#include <iostream>
#include <vector>
#include <string>
#include<thread>
#include<regex>
#include<mutex>
#include <exception>

using namespace std;

bool ValidanFormat(string sadrzaj) {
	string pravilo = "([a-zA-Z]{1,}[@](outlook.com|edu.fit.ba))";
	if (regex_match(sadrzaj, regex(pravilo)))
		return true;
	return false;
}

mutex cuvar;

const char* crt = "\n-------------------------------------------\n";
enum eRazred { PRVI = 1, DRUGI, TRECI, CETVRTI };
enum SortirajPo { T1, T2 };

char* Alociraj(const char* sadrzaj) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
template<class T1, class T2>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
	bool _omoguciDupliranje;
public:
	Kolekcija(bool omoguciDupliranje = true) {
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_omoguciDupliranje = omoguciDupliranje;
	}

	Kolekcija(const Kolekcija& original) {
		_trenutno = original._trenutno;
		_omoguciDupliranje = original._omoguciDupliranje;
		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = original._elementi1[i];
			_elementi2[i] = original._elementi2[i];
		}
	}

	Kolekcija& operator=(const Kolekcija& original) {
		if (this != &original)
		{
			delete[]_elementi1;
			delete[]_elementi2;

			_trenutno = original._trenutno;
			_omoguciDupliranje = original._omoguciDupliranje;
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

	~Kolekcija() {
		delete[]_elementi1; _elementi1 = nullptr;
		delete[]_elementi2; _elementi2 = nullptr;
	}
	T1& getElement1(int lokacija)const { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija)const { return _elementi2[lokacija]; }
	int getTrenutno() { return _trenutno; }
	friend ostream& operator<< (ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}

	void AddElement(const T1& t1, const T2& t2) {
		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		if (!_omoguciDupliranje)
			for (int i = 0; i < _trenutno; i++)
				if (_elementi1[i] == t1 && _elementi2[i] == t2)
					throw exception("Nedozvoljeno dupliranje.");
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

	void SortirajRastuci(SortirajPo) {
		if (SortirajPo::T1)
		{
			bool flag = true;
			while (flag)
			{
				flag = false;
				for (size_t i = 0; i < _trenutno-1; i++)
				{
					if (_elementi1[i] > _elementi1[i + 1]) {
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


		if (SortirajPo::T2)
		{
			bool flag = true;
			while (flag)
			{
				flag = false;
				for (size_t i = 0; i < _trenutno - 1; i++)
				{
					if (_elementi2[i] > _elementi2[i + 1]) {
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

	}

};

class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}
	~DatumVrijeme() {
		delete _dan;    _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati;   _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}

	DatumVrijeme& operator=(const DatumVrijeme& obj) {
		if (this != &obj)
		{
			delete _dan;
			delete _mjesec;
			delete _godina;
			delete _sati;
			delete _minuti;

			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
			_sati = new int(*obj._sati);
			_minuti = new int(*obj._minuti);

		}
		return *this;
	}

	friend ostream& operator<< (ostream& COUT, const DatumVrijeme& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti << endl;
		return COUT;
	}
	DatumVrijeme(const DatumVrijeme& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		_sati = new int(*obj._sati);
		_minuti = new int(*obj._minuti);
	}


	int intoMinutes() const {
		return *_minuti + *_sati * 10 + *_dan * 100 + *_mjesec * 1000 + *_godina * 10000;
	}

	friend int operator-(DatumVrijeme& dat1, DatumVrijeme& dat2) {
		return (dat1.intoMinutes() - dat2.intoMinutes());
	}

	friend bool operator<(DatumVrijeme& dat1, DatumVrijeme& dat2) {
		int lijevi = *dat1._minuti + *dat1._sati * 10 + *dat1._dan * 100 + *dat1._mjesec * 1000 + *dat1._godina * 10000;
		int desni= *dat2._minuti + *dat2._sati * 10 + *dat2._dan * 100 + *dat2._mjesec * 1000 + *dat2._godina * 10000;
		if (desni-lijevi < 5)
			return false;
		return true;
	}

	friend bool operator== (const DatumVrijeme& dat1, const  DatumVrijeme& dat2) {
		
		if (dat1.intoMinutes()==dat2.intoMinutes())
		return true;
			return false;
	}

};

class Predmet {
	char* _naziv;
	int _ocjena;
	string _napomena;
public:
	Predmet(const char* naziv = "", int ocjena = 0, string napomena = "") {
		_naziv = Alociraj(naziv);
		_ocjena = ocjena;
		_napomena = napomena;
	}

	Predmet(const Predmet& original) {
		_naziv = Alociraj(original._naziv);
		_ocjena = original._ocjena;
		_napomena = original._napomena;
	}

	friend bool operator==(const Predmet& p1, const Predmet& p2) {
		if (strcmp(p1._naziv, p2._naziv) == 0 && p1._napomena == p2._napomena && p1._ocjena == p2._ocjena)
			return true;
		return false;
	}

	Predmet& operator=(const Predmet& original) {
		if (this!=&original)
		{
			delete[] _naziv;
			_naziv = Alociraj(original._naziv);
			_ocjena = original._ocjena;
			_napomena = original._napomena;
		}
		return *this;
	}

	~Predmet() {
		delete[] _naziv; _naziv = nullptr;
	}
	friend ostream& operator<< (ostream& COUT, Predmet& obj) {
		COUT << obj._naziv << " (" << obj._ocjena << ") " << obj._napomena << endl;
		return COUT;
	}
	string GetNapomena() { return _napomena; }
	char* GetNaziv() { return _naziv; }
	int GetOcjena() { return _ocjena; }

	void DodajNapomenu(string napomena) {
		_napomena += " " + napomena;
	}
};

class Uspjeh {
	eRazred* _razred;
	//formalni argument DatumVrijeme se odnosi na vrijeme evidentiranja polozenog predmeta
	Kolekcija<Predmet, DatumVrijeme> _predmeti;
public:
	Uspjeh(eRazred razred) {
		_razred = new eRazred(razred);
	}

	Uspjeh(const Uspjeh& original) {
		_razred = new eRazred(*original._razred);
		_predmeti = original._predmeti;
	}

	~Uspjeh() { delete _razred; _razred = nullptr; }

	Kolekcija<Predmet, DatumVrijeme>* GetPredmeti() { return &_predmeti; }
	eRazred* GetERazred() { return _razred; }
	friend ostream& operator<< (ostream& COUT, const Uspjeh& obj) {
		COUT << *obj._razred << " " << obj._predmeti << endl;
		return COUT;
	}
};

class Kandidat {
	char* _imePrezime;
	string _emailAdresa;
	string _brojTelefona;
	vector<Uspjeh> _uspjeh;
public:
	Kandidat(const char* imePrezime, string emailAdresa, string brojTelefona) {
		_imePrezime = Alociraj(imePrezime);
		if (ValidanFormat(emailAdresa))
			_emailAdresa = emailAdresa;
		else
			_emailAdresa = "notSet@edu.fit.ba";
		_brojTelefona = brojTelefona;
	}
	~Kandidat() {
		delete[] _imePrezime; _imePrezime = nullptr;
	}

	Kandidat(const Kandidat& original) {
		_imePrezime = Alociraj(original._imePrezime);
		_emailAdresa = original._emailAdresa;
		_brojTelefona = original._brojTelefona;
		_uspjeh = original._uspjeh;
	}

	Kandidat& operator=(const Kandidat& original) {
		if (this!=&original)
		{
			delete[] _imePrezime;
			_imePrezime = Alociraj(original._imePrezime);
			_emailAdresa = original._emailAdresa;
			_brojTelefona = original._brojTelefona;
			_uspjeh = original._uspjeh;
		}
		return *this;
	}

	/*nakon evidentiranja uspjeha na bilo kojem predmetu kandidatu se salje email sa porukom:
FROM:info@edu.fit.ba
TO: emailKorisnika
Postovani ime i prezime, evidentirali ste uspjeh za X razred.
Pozdrav.
FIT Team.

ukoliko je prosjek na nivou tog razreda veci od 4.5 kandidatu se salje SMS sa porukom: "Svaka cast za uspjeh X.X u X razredu".
slanje poruka i emailova implemenitrati koristeci zasebne thread-ove.
*/

	void IspisThreada(string from, string to, string body) {
		cuvar.lock();
		cout << "FROM: " << from << endl;
		cout << "TO: " << to << endl;
		cout << body << endl;
		cuvar.unlock();
	}


	bool AddPredmet(eRazred razred, Predmet& predmet, DatumVrijeme& datum) {
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			if (*_uspjeh[i].GetERazred() == razred) {
				for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
				{
					if (strcmp(_uspjeh[i].GetPredmeti()->getElement1(j).GetNaziv(), predmet.GetNaziv()) == 0)
						return false;
					if (datum - _uspjeh[i].GetPredmeti()->getElement2(j)<5)
						return false;
				}

				_uspjeh[i].GetPredmeti()->AddElement(predmet, datum);
				thread t1(&Kandidat::IspisThreada, this, "info@edu.fit.ba", _emailAdresa, "Postovani ime i prezime, evidentirali ste uspjeh za " + to_string(razred) + " razred.\n Pozdrav.\n");
				t1.join();
				return true;
			}
		}
		Uspjeh r(razred);
		r.GetPredmeti()->AddElement(predmet, datum);
		_uspjeh.push_back(r);
		thread t(&Kandidat::IspisThreada, this, "info@edu.fit.ba", _emailAdresa, "Postovani ime i prezime, evidentirali ste uspjeh za " + to_string(razred) + " razred.\n Pozdrav.\nFIT Team.");
		t.join();
		return true;
			
	}

	vector<Predmet> operator()(DatumVrijeme d1, DatumVrijeme d2) {
		vector<Predmet> predmeti;
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			for (size_t j = 0; j < _uspjeh[i].GetPredmeti()->getTrenutno(); j++)
			{
				if (d1.intoMinutes() - _uspjeh[i].GetPredmeti()->getElement2(j).intoMinutes() < 0 && d2.intoMinutes() - _uspjeh[i].GetPredmeti()->getElement2(j).intoMinutes() > 0)
					predmeti.push_back(_uspjeh[i].GetPredmeti()->getElement1(j));
			}
		}
		return predmeti;
	}

	Uspjeh* operator[](eRazred razred) {
		for (size_t i = 0; i < _uspjeh.size(); i++)
		{
			if (*_uspjeh[i].GetERazred() == razred) {
				return &_uspjeh[i];
			}
		}
		return nullptr;
	}

	friend ostream& operator<< (ostream& COUT, Kandidat& obj) {
		COUT << obj._imePrezime << " " << obj._emailAdresa << " " << obj._brojTelefona << endl;
		return COUT;
	}
	vector<Uspjeh>* GetUspjeh() { return &_uspjeh; }
	string GetEmail() { return _emailAdresa; }
	string GetBrojTelefona() { return _brojTelefona; }
	char* GetImePrezime() { return _imePrezime; }
};

void main() {
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR AE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEEILO GUBLJENJE URA?ENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKO?ER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIENI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUEAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOANE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAEENO.
	7. IMATE POTPUNU SLOBODU DA U MAIN FUNKCIJI, ZA POTREBE TESTIRANJA, DODAJETE VISE POZIVA ILI SALJETE DRUGE VRIJEDNOSTI PARAMETARA
	****************************************************************************/

	DatumVrijeme temp,
	datum19062019_1015(19, 6, 2019, 10, 15),
		datum20062019_1115(20, 6, 2019, 11, 15),
		datum30062019_1215(30, 6, 2019, 12, 15),
		datum05072019_1231(5, 7, 2019, 12, 31);

	const int kolekcijaTestSize = 9;
	Kolekcija<int, int> kolekcija1(false);
	for (size_t i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i + 1, 20 - i);

	try {
		//ukoliko nije dozvoljeno dupliranje elemenata, metoda AddElement baca izuzetak
		kolekcija1.AddElement(6, 15);
	}
	catch (exception & err) {
		cout << err.what() << endl;
	}
	cout << kolekcija1 << endl;

	////na osnovu vrijednosti parametra sortira clanove kolekcije u rastucem redoslijedu 
	kolekcija1.SortirajRastuci(SortirajPo::T2);
	cout << kolekcija1 << endl;


	Kolekcija<int, int> kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;

	Kolekcija<int, int> kolekcija3;
	kolekcija3 = kolekcija1;
	cout << kolekcija3 << crt;

	//napomena se moze dodati i prilikom kreiranja objekta
	Predmet Matematika("Matematika", 5, "Ucesce na takmicenju"),
		Fizika("Fizika", 5),
		Hemija("Hemija", 2),
		Engleski("Engleski", 5);
	Fizika.DodajNapomenu("Pohvala za ostvareni uspjeh");
	cout << Matematika << endl;

	/*
	email adresa mora biti u formatu: text@outlook.com ili text@edu.fit.ba
	u slucaju da email adresa nije validna, postaviti je na defaultnu: notSet@edu.fit.ba
	za provjeru koristiti regex
	*/
	Kandidat jasmin("Jasmin Azemovic", "jasmin@outlook.com", "033 281 172");
	Kandidat adel("Adel Handzic", "adel@edu.fit.ba", "033 281 170");
	Kandidat emailNotValid("Ime Prezime", "korisnik@klix.ba", "033 281 170");

	
	/* uspjeh (tokom srednjoskolskog obrazovanja) se dodaje za svaki predmet na nivou razreda.
	tom prilikom onemoguciti:
	- dodavanje istoimenih predmeta na nivou jednog razreda,
	- dodavanje vise predmeta u kratkom vremenskom periodu (na nivou jednog razreda, razmak izmedju dodavanja pojedinih predmeta mora biti najmanje 5 minuta).
	razredi (predmeti ili uspjeh) ne moraju biti dodavani sortiranim redoslijedom (npr. prvo se moze dodati uspjeh za II razred, pa onda za I razred i sl.).
	Funkcija vraca true ili false u zavisnosti od (ne)uspjesnost izvrsenja */
	
	if (jasmin.AddPredmet(DRUGI, Fizika, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(DRUGI, Hemija, datum30062019_1215))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Engleski, datum19062019_1015))
		cout << "Predmet uspjesno dodan!" << crt;
	if (jasmin.AddPredmet(PRVI, Matematika, datum20062019_1115))
		cout << "Predmet uspjesno dodan!" << crt;
	//ne treba dodati Matematiku jer je vec dodana u prvom razredu
	if (!jasmin.AddPredmet(PRVI, Matematika, datum05072019_1231))
		cout << "Predmet BEZuspjesno dodan!" << crt;
	//ne treba dodati Fiziku jer nije proslo 5 minuta od dodavanja posljednjeg predmeta
	if (!jasmin.AddPredmet(PRVI, Fizika, datum20062019_1115))
		cout << "Predmet BEZuspjesno dodan! " << crt;

	cout << "USPJEH ISPISATI KORISTECI OSTREAM_ITERATOR" << endl;
	cout << jasmin << endl;
	cout << emailNotValid << endl;
	////vraca broj ponavljanja odredjene rijeci u napomenama, koristiti sregex_iterator
	//cout << "Rijec takmicenje se pojavljuje " << jasmin.BrojPonavljanjaRijeci("takmicenju") << " puta." << endl;

	//vraca niz predmeta koji su evidentiranih u periodu izmedju vrijednosti proslijedjenih parametara
	vector<Predmet> jasminUspjeh = jasmin(DatumVrijeme(18, 06, 2019, 10, 15), DatumVrijeme(21, 06, 2019, 10, 10));
	for (size_t i = 0; i < jasminUspjeh.size(); i++)
		cout << jasminUspjeh[i] << endl;

	Uspjeh* uspjeh_Irazred = jasmin[PRVI];//vraca uspjeh kandidata ostvaren u prvom razredu
	if (uspjeh_Irazred != nullptr)
		cout << *uspjeh_Irazred << endl;

	cin.get();
	system("pause>0");
}