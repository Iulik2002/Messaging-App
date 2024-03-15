Cozmac Iulian 325CD
Detalii despre structura temei:  	
Am folosit scheletul de cod din laboratorul 9 cu mici schimbari in unele funcții.
Codul se execută într-o buclă while infinită, în care așteaptă comenzi de la utilizator de la tastatură. Comenzile suportate sunt:

"register" - pentru înregistrarea unui cont nou:
	- citesc datele cerute de la tastatura si le scriu in json;
  	- compun un mesaj de post si trimit mesajul la server;
  	- primesc si afisez raspunsul.
"login" - pentru autentificare:
	- citesc datele de la tastatura si le scriu json
  	- salvez cookieul;
  	- trimit mesajul la server, primesc si afisez raspunsul
"enter_library" - pentru solicitarea de acces în bibliotecă:
	- trimit un mesaj de get cu cookieul salvat
  	- primesc un raspuns si salvez jwtul;
  	- afisez raspunsul de la server.
"get_books" - pentru afișarea informațiilor sumare despre cărți:
	- trimit un mesaj de get cu cookieul salvat,
  	sa demonstrez ca am acces
  	- primesc un raspuns de la server si il afisez.
"get_book" - pentru afișarea detaliilor despre o carte:
	- citesc de la tastatura un id;
  	- trimit mesajul de get cu idul citit la server;
  	- primesc raspuns si afisez.
"add_book" - pentru adăugarea unei cărți noi în bibliotecă:
	- citesc toate datele de la tastatura;
  	- le scriu in json;
  	- compun mesajul si il trimit catre server;
  	- primesc raspuns si afisez.
"delete_book" - pentru ștergerea unei cărți din bibliotecă:
	- citesc idul de la tastatura;
  	- compun mesajul de delete cu id-ul citit si trimit catre server;
  	- primesc raspuns si afisez.
"logout" - pentru deconectare:
	- inchid conexiunea.
"exit" - pentru a ieși din aplicație:
	- termin executia programlui.

În codul din fisierul client.c am folosit in mare parte `fgets` pentru a citi de la tastura. Problema rândului nou am combatut-o cu copierea variabilei intr-una nouă și scaderea cu 1 din ea.
Pentru fiecare comandă, se efectuează cereri HTTP către serverul specificat în codul sursă și se primește un răspuns de la server. Răspunsurile sunt afișate în consolă.
Pentru a parsa răspunsurile primite de la server am folosit biblioteca cJSON. (Fșierele cJSON.c și cJSON.h)