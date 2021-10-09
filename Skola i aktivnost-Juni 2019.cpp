#include<iostream>
#include <string>
#include<vector>
#include<thread>
#include<regex>
#include<mutex>
#include<exception>

using namespace std;

/****************************************************************************
1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
4. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUČAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
5. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
****************************************************************************/

/*
	koristeci regex, osigurati sljedeci format za broj telefona: +387(6X)XXX-XXX ili +387 6X XXX-XXX
	onemoguciti pojavljivanje samo jedne zagrade, a ukoliko format nije adekvatna koristiti vrijednost not_set
	*/

bool IspravanFormat(string sadrzaj) {
	string pravilo = "(\\+387)(\\(6)(\\d{1})(\\))(\\d{3})[-](\\d{3})|(\\+387 )(\\6)(\\d{1} )(\\d{3})[-](\\d{3})";
	regex reg(pravilo);
	if (regex_match(sadrzaj, reg))
		return true;
	return false;

}

const char* nedozvoljena_operacija = "Nedozvoljena operacija";
const char* not_set = "NOT_SET";

enum Sortiranje { ASC, DESC };
enum Predmet { MATEMATIKA, FIZIKA, HEMIJA, GEOGRAFIJA, NOT_SET };
const char* predmetIspis[]{ "MATEMATIKA", "FIZIKA", "HEMIJA", "GEOGRAFIJA", "NOT_SET" };
enum Razred { I = 1, II, III, IV };

char* AlocirajNizKaraktera(const char* sadrzaj) {
	if (sadrzaj == nullptr)
		return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	return temp;
}
template <class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max] = { nullptr };
	T2* _elementi2[max] = { nullptr };
	int _trenutnoElemenata;
	bool _dozvoliDupliranje;
	Sortiranje _sortiranje;
	void Sortiraj(Sortiranje sortBy) {
		if (sortBy == ASC) {
			bool flag = true;
			while (flag)
			{
				flag = false;
				for (size_t i = 0; i < _trenutnoElemenata-1; i++)
				{
					if (*_elementi1[i] > * _elementi1[i + 1]) {
						T1* temp1 = new T1(*_elementi1[i]);
						_elementi1[i] = _elementi1[i + 1];
						_elementi1[i + 1] = temp1;

						T2* temp2 = new T2(*_elementi2[i]);
						_elementi2[i] = _elementi2[i + 1];
						_elementi2[i + 1] = temp2;

						flag = true;
					}
				}
			}
		}

		if (sortBy == DESC) {
			bool flag = true;
			while (flag)
			{
				flag = false;
				for (size_t i = 0; i < _trenutnoElemenata - 1; i++)
				{
					if (*_elementi1[i] < * _elementi1[i + 1]) {
						T1* temp1 = new T1(*_elementi1[i]);
						_elementi1[i] = _elementi1[i + 1];
						_elementi1[i + 1] = temp1;

						T2* temp2 = new T2(*_elementi2[i]);
						_elementi2[i] = _elementi2[i + 1];
						_elementi2[i + 1] = temp2;

						flag = true;
					}
				}
			}
		}

	}
public:
	Kolekcija(Sortiranje sortiranje = ASC, bool dozvoliDupliranje = true) {
		_trenutnoElemenata = 0;
		_sortiranje = sortiranje;
		_dozvoliDupliranje = dozvoliDupliranje;
	}
	~Kolekcija() {
		for (size_t i = 0; i < _trenutnoElemenata; i++) {
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}

	Kolekcija(const Kolekcija& original) {
		_trenutnoElemenata = original._trenutnoElemenata;
		_dozvoliDupliranje = original._dozvoliDupliranje;
		_sortiranje = original._sortiranje;
		for (size_t i = 0; i < _trenutnoElemenata; i++)
		{
			_elementi1[i] = new T1(*original._elementi1[i]);
			_elementi2[i] = new T2(*original._elementi2[i]);
		}
	}

	Kolekcija& operator=(const Kolekcija& original) {
		if (this!=&original)
		{
			for (size_t i = 0; i < _trenutnoElemenata; i++) {
				delete _elementi1[i]; 
				delete _elementi2[i]; 
			}
			_trenutnoElemenata = original._trenutnoElemenata;
			_dozvoliDupliranje = original._dozvoliDupliranje;
			_sortiranje = original._sortiranje;
			for (size_t i = 0; i < _trenutnoElemenata; i++)
			{
				_elementi1[i] = new T1(*original._elementi1[i]);
				_elementi2[i] = new T2(*original._elementi2[i]);
			}

		}
		return *this;
	}


	void AddElement(const T1& t1, const T2& t2) {
		if (_trenutnoElemenata >= max)
			throw exception("Prekoracenje opsega\n");
		if (!_dozvoliDupliranje)
			for (int i = 0; i < _trenutnoElemenata; i++)
				if (*_elementi1[i] == t1 && *_elementi2[i] == t2)
					throw exception("Pokusaj dupliranja.\n");
		_elementi1[_trenutnoElemenata] = new T1(t1);
		_elementi2[_trenutnoElemenata] = new T2(t2);
		_trenutnoElemenata++;

		Sortiraj(_sortiranje);

	}

	int GetTrenutno() const { return _trenutnoElemenata; }
	int GetMax() const { return max; }

	T1& GetElement1(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi1[lokacija];
	}
	T2& GetElement2(int lokacija) const {
		if (lokacija < 0 || lokacija >= _trenutnoElemenata)
			throw exception(nedozvoljena_operacija);
		return *_elementi2[lokacija];
	}
	void SetElement2(int lokacija, T2 ele2) {
		_elementi2[lokacija] = ele2;
	}
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
		for (size_t i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
};

class Aktivnost {
	shared_ptr<Razred> _razred;
	string _opis;
	int _ocjena;//1-5
public:
	Aktivnost(Razred razred = I, int ocjena = 0, string opis = not_set) {
		_ocjena = ocjena;
		_opis = opis;
		_razred = make_shared<Razred>(razred);
	}
	int  GetOcjenu() const { return _ocjena; }
	string GetOpis() const { return _opis; }
	Razred GetRazred() const { return *_razred; }

	friend bool operator==(const Aktivnost& a1, const Aktivnost& a2) {
		if (a1._razred == a2._razred && a1._ocjena == a2._ocjena && a1._opis == a2._opis)
			return true;
		return false;
	}

	friend ostream& operator<<(ostream& COUT, const Aktivnost& obj) {
		COUT << "Razred: "<<*obj._razred << "  | Ocjena-> " << obj._ocjena << " |Opis-> " << obj._opis << endl;
		return COUT;
	}
};

class Polaznik {
protected:
	char* _imePrezime;
	string _brojTelefona;
public:
	Polaznik(string imePrezime, string brojTelefona) : _imePrezime(AlocirajNizKaraktera(imePrezime.c_str())) {
		if (IspravanFormat(brojTelefona)) {
			_brojTelefona = brojTelefona;
			cout << brojTelefona << " zadovoljava format.\n";
		}
		else
		{
			cout << brojTelefona << " NE zadovoljava format.\n";
			_brojTelefona = not_set;
		}

	}

	Polaznik(const Polaznik& original) {
		_imePrezime = AlocirajNizKaraktera(original._imePrezime);
		_brojTelefona = original._brojTelefona;
	}

	Polaznik& operator=(const Polaznik& original) {
		if (this!=&original)
		{
			delete[] _imePrezime;
			_imePrezime = AlocirajNizKaraktera(original._imePrezime);
			_brojTelefona = original._brojTelefona;
		}
		return *this;
	}

	~Polaznik() { delete[] _imePrezime; }
	char* GetImePrezime() { return _imePrezime; }
	string GetTelefon() { return _brojTelefona; }
	virtual void PredstaviSe() = 0;
};

class Ucenik: public Polaznik {
	Kolekcija<Predmet, Aktivnost, 16>* _aktivnosti;
public:

	Ucenik(string imePrezime, string brojTelefona) :Polaznik(imePrezime, brojTelefona) {
		_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>();
	}

	Ucenik(const Ucenik& original):Polaznik(original) {
		_aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>(*original._aktivnosti);

	}

	Ucenik& operator=(const Ucenik& original) {
		if (this!=&original)
		{
			Polaznik::operator=(original);
		    delete _aktivnosti;
		   _aktivnosti = new Kolekcija<Predmet, Aktivnost, 16>(*original._aktivnosti);


		}
		return *this;
	}

	~Ucenik() { delete _aktivnosti; _aktivnosti = nullptr; }
	Kolekcija<Predmet, Aktivnost, 16>& GetAktivnosti() { return *_aktivnosti; };
	friend ostream& operator<<(ostream& COUT, Ucenik& n)
	{
		/*COUT << "PODACI O UCENIKU I NJEGOVIM AKTIVNOSTIMA....IMPLEMENTIRATI" << endl;*/
		COUT <<"Ime i prezime ucenika: " <<n._imePrezime << endl;
		COUT << "Broj telefona: " << n._imePrezime << endl;
		cout << "Aktivnosti: " << endl;
		for (size_t i = 0; i < n.GetAktivnosti().GetTrenutno(); i++)
		{
			cout <<i+1 <<". predmet "<<predmetIspis[n.GetAktivnosti().GetElement1(i)] << endl;
			cout << i + 1 << ". aktivnost " << n._aktivnosti->GetElement2(i) << endl;
		}
		return COUT;
	}
	void PredstaviSe() {
		cout << *this << endl;
	}
};


mutex cuvar;


class Skola {
	char* _naziv;
	vector<Ucenik> _ucenici;
public:
	Skola(const char* naziv = nullptr) {
		_naziv = AlocirajNizKaraktera(naziv);
	}

	Skola(const Skola& original) {
		_naziv = AlocirajNizKaraktera(original._naziv);
		_ucenici = original._ucenici;
	}

	Skola& operator=(const Skola& original) {
		if (this!=&original)
		{
			delete[] _naziv;
			_naziv = AlocirajNizKaraktera(original._naziv);
			_ucenici = original._ucenici;
		}
		return *this;
	}

	~Skola() {
		delete[] _naziv; _naziv = nullptr;
	}

	void operator()(string imePrezime, string brojTelefona) {
		for (size_t i = 0; i < _ucenici.size(); i++)
		{
			if (strcmp(_ucenici[i].GetImePrezime(), imePrezime.c_str()) == 0 || brojTelefona == _ucenici[i].GetTelefon())
				throw exception("Pokusaj dupliranja.\n");
		}
		_ucenici.push_back(Ucenik(imePrezime, brojTelefona));
	}

	char* GetNaziv()const { return _naziv; }
	vector<Ucenik>& GetUcenici() { return _ucenici; };


	
	

	bool DodajAktivnost(const char* imePrezime, Predmet predmet, Aktivnost aktivnost) {
	
	
		for (size_t i = 0; i < _ucenici.size(); i++)
		{
			if (strcmp(_ucenici[i].GetImePrezime(), imePrezime) == 0)
			{
				float prosjek = 0;
				for (size_t j = 0; j < _ucenici[i].GetAktivnosti().GetTrenutno(); j++)
				{
					if (_ucenici[i].GetAktivnosti().GetElement2(j).GetRazred() == aktivnost.GetRazred()) {
						if (_ucenici[i].GetAktivnosti().GetTrenutno() >= 4)
							return false;
						if (_ucenici[i].GetAktivnosti().GetElement1(j) == predmet)
							return false;
					
						if (_ucenici[i].GetAktivnosti().GetElement2(j).GetOcjenu() > 1 && _ucenici[i].GetAktivnosti().GetElement2(j).GetOcjenu() < 6) {
							prosjek += _ucenici[i].GetAktivnosti().GetElement2(j).GetOcjenu();
							
						}
					}
				}
						_ucenici[i].GetAktivnosti().AddElement(predmet, aktivnost);
						if (_ucenici[i].GetTelefon() != not_set && _ucenici[i].GetAktivnosti().GetTrenutno()>=4) {
							prosjek += aktivnost.GetOcjenu();
							prosjek /= _ucenici[i].GetAktivnosti().GetTrenutno();
							thread t1([&]() {
								cuvar.lock();
								cout << "Postovani " << _ucenici[i].GetImePrezime() << " cestitamo na uspjehu koji iznosi " << prosjek << endl;
								cuvar.unlock();
								});
							t1.join();
						}
						return true;
			}
		}
	}


	pair<Polaznik*, float> GetNajboljegUcenika() {
		float najProsjek = 0;
		int indeksNajboljeg = 0;

		for (size_t i = 0; i < _ucenici.size(); i++)
		{
			float prosjek = 0;
		int brojac = 0;
			for (size_t j = 0; j < _ucenici[i].GetAktivnosti().GetTrenutno(); j++)
			{
				prosjek += _ucenici[i].GetAktivnosti().GetElement2(j).GetOcjenu();
				brojac++;
			}
			prosjek /= brojac;

			if (prosjek > najProsjek) {
				najProsjek = prosjek;
				indeksNajboljeg = i;
			}
			
		}

		

		return (make_pair(&_ucenici[indeksNajboljeg], najProsjek));
	}


	friend ostream& operator<<(ostream& COUT, Skola& obj) {
		COUT << "Skola: " << obj._naziv << endl;
		COUT << "Ucenici: " << endl;
		for (size_t i = 0; i < obj._ucenici.size(); i++)
			COUT << obj._ucenici[i] << endl;
		return COUT;
	}
};

int main() {
	Kolekcija<int, int, 10> kolekcija1(DESC, false);
	try {
		kolekcija1.AddElement(1, 2);
		//dupliranje elemenata nije dozvoljeno
		kolekcija1.AddElement(1, 2);
	}
	catch (exception & ex) {
		cout << ex.what();
	}

	/*nakon svakog dodavanja, elemente sortirati prema T1 i vrijednosti atributa _sortiranje*/
	for (size_t i = 1; i < kolekcija1.GetMax() - 1; i++)
		kolekcija1.AddElement(rand(), rand());

	cout << kolekcija1 << endl;

	try {
		//prekoracen maksimalan broj elemenata
		kolekcija1.AddElement(rand(), rand());
	}
	catch (exception & ex) {
		cout << ex.what();
	}
	cout << kolekcija1 << endl;

	Kolekcija<int, int, 10> kolekcija2(kolekcija1);
	cout << kolekcija2 << endl;

	Skola gimnazijaMostar("GIMNAZIJA MOSTAR");
	//dodaje novog ucenika u skolu
	gimnazijaMostar("Jasmin Azemovic", "+387(61)111-222");
	gimnazijaMostar("Adel Handzic", "+387(61)333-444");

	/*
	koristeci regex, osigurati sljedeci format za broj telefona: +387(6X)XXX-XXX ili +387 6X XXX-XXX
	onemoguciti pojavljivanje samo jedne zagrade, a ukoliko format nije adekvatna koristiti vrijednost not_set
	*/
	gimnazijaMostar("Telefon NotValidFormat", "387 61)333-444");

	try
	{
		/*onemoguciti dodavanje ucenika sa istim imenom i prezimenom ili brojem telefona*/
		gimnazijaMostar("Adel Handzic", "+387(61)333-444");
	}
	catch (exception & ex)
	{
		cout << ex.what() << endl;
	}

	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Priprema za takmicenje iz Matematije koje se odrzava u Konjicu 07.02.2019")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*na nivou svakog razreda se mogu evidentirati maksimalno 4 aktivnosti, a takodjer, na nivou razreda se ne smiju ponavljati aktivnosti iz istog predmeta*/
	if (!gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", MATEMATIKA, Aktivnost(I, 4, "Aktivnosti iz matematike")))
		cout << "Aktivnost nije uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", HEMIJA, Aktivnost(I, 5, "Priprema otopina za vjezbe iz predmeta Hemija")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", FIZIKA, Aktivnost(I, 4, "Analiza stepena apsorpcije materijala ")))
		cout << "Aktivnost uspjesno dodana" << endl;
	/*u slucaju da je ucenik uspjesno (ocjenom vecom od 1) realizovao aktivnosti na nivou odredjenog razreda, te posjeduje validan broj telefona,
	u okviru zasebnog thread-a se salje SMS sa sadrzajem "Uspjesno ste okoncali aktivnosti u okviru X razreda sa prosjecnom ocjenom X.X"*/
	if (gimnazijaMostar.DodajAktivnost("Jasmin Azemovic", GEOGRAFIJA, Aktivnost(I, 4, "Izrada 5 reljefa Mostara")))
		cout << "Aktivnost uspjesno dodana" << endl;
	if (gimnazijaMostar.DodajAktivnost("Adel Handzic", MATEMATIKA, Aktivnost(I, 5, "Izrada skripte na temu integralni racun")))
		cout << "Aktivnost uspjesno dodana" << endl;

	//ispisuje sve ucenike i njihove aktivnosti
	cout << gimnazijaMostar << endl;


	pair<Polaznik*, float> par = gimnazijaMostar.GetNajboljegUcenika();
	cout << "Najbolji ucenik je " << par.first->GetImePrezime() << " sa prosjekom " << par.second << endl;

	///*U fajl (npr. Gimnazija.txt) upisati podatke (podatke upisati kao obicni tekst) o skoli i svim ucenicima.
	//Nakon upisa, potrebno je ispisati sadrzaj fajla. Parametar tipa bool oznacava da li ce ranije dodani sadrzaj fajla prethodno biti pobrisan*/

	//if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt"))
	//	cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;
	//if (gimnazijaMostar.SpasiUFajl("Gimnazija.txt", true))
	//	cout << "Podaci o ucenicima uspjesno pohranjeni u fajl" << endl;

	cin.get();
	system("pause>0");
	return 0;
}