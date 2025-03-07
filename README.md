# 🎮 Tank Battle Arena

Un joc shooter multiplayer unde jucătorii controlează tancuri într-o arenă generată procedural, cu ziduri distructibile, bombe și portale.

## 📋 Descriere

Tank Battle Arena este un joc de tip shooter cu perspectivă top-down în care până la 4 jucători se înfruntă într-o arenă generată procedural. Jucătorii controlează tancuri, pot trage proiectile și trebuie să elimine ceilalți jucători pentru a câștiga runda.

## 🚀 Caracteristici principale

- **Arhitectură client-server**: Jocul funcționează pe un model client-server pentru multiplayer.
- **Multiplayer**: Suport pentru până la 4 jucători.
- **Hartă generată procedural**: Fiecare partidă are o hartă unică.
- **Obiecte interactive**:
  - Ziduri distructibile
  - Ziduri indestructibile
  - Ziduri cu bombe (explodează și distrug zidurile din jur)
  - Portale pentru teleportare
- **Sistem de progresie**: Scor, puncte și clasament pentru fiecare jucător.
- **Upgrade-uri**: Jucătorii pot debloca îmbunătățiri pentru rata de foc și viteza proiectilelor.
- **Sistem de conturi**: Datele jucătorilor sunt salvate într-o bază de date SQLite.

## 🧩 Structura proiectului

Proiectul este împărțit în două componente principale: **server** și **client**.

### Componenta Client

Clientul este scris în C++ folosind framework-ul **Qt** și gestionează interfața grafică, intrările utilizatorului și comunicarea cu serverul.

#### Funcționalități principale ale clientului:
- **Autentificare și înregistrare**: Jucătorii se pot înregistrare sau autentifica folosind un sistem de conturi.
- **Interfață grafică**: Interfața este construită cu ajutorul **QWidget** și **QGridLayout**, oferind o experiență intuitivă.
- **Comunicare cu serverul**: Clientul trimite cereri HTTP către server pentru a gestiona jocul (de exemplu, mișcarea, tragerea, upgrade-urile).
- **Afișarea hărții**: Harta este afișată folosind **QLabel** și **QPixmap**, cu imagini pentru ziduri, proiectile și alte elemente ale jocului.
- **Upgrade-uri**: Jucătorii pot îmbunătăți viteza proiectilelor și rata de tragere prin cereri HTTP către server.

#### Detalii tehnice:
- **Qt**: Folosit pentru interfața grafică și gestionarea evenimentelor.
- **QNetworkAccessManager**: Folosit pentru a trimite și primi cereri HTTP de la server.
- **QJsonDocument**: Folosit pentru a procesa datele JSON primite de la server.
- **QPixmap**: Folosit pentru a încărca și afișa imagini în interfață.

### Componenta Server

Serverul gestionează logica jocului, simulările fizice, generarea hărții și stocarea datelor utilizatorilor.

## 📊 Mecanica jocului

### Generarea hărții

Harta este generată aleatoriu la începutul fiecărei partide, cu dimensiuni variabile (10-11 unități). Aceasta conține:
- Ziduri distructibile și indistructibile distribuite aleatoriu.
- 1-3 ziduri cu bombe plasate aleatoriu.
- 2-5 perechi de portale plasate pe marginile hărții.

### Sistemul de portale

- Portalele permit teleportarea între două puncte de pe hartă.
- Atât jucătorii cât și proiectilele pot traversa portalele.
- Direcția de mișcare este ajustată în funcție de poziția portalului de ieșire.

### Sistemul de sănătate și respawn

- Jucătorii încep cu 3 puncte de sănătate.
- Când un jucător pierde toate punctele de sănătate, este eliminat din joc.
- Jucătorii care primesc daune dar nu sunt eliminați revin la poziția lor inițială.

### Sistemul de scor și recompense

- Jucătorii primesc 100 de puncte pentru fiecare inamic eliminat.
- Ultimul jucător rămas primește un bonus de 200 de puncte.
- Clasamentul final oferă puncte bonus (2 pentru locul 1, 1 pentru locul 2).
- Toate datele sunt salvate în contul jucătorului.

### Sistemul de upgrade-uri

- Jucătorii pot îmbunătăți rata de tragere (reducând timpul de cooldown la jumătate).
- Jucătorii pot îmbunătăți viteza proiectilelor (dublând viteza).


### Proiect realizat de catre :
### - Dragomir Cezar-Andrei
### - Hermeneanu Silviu Ionut
### - Iftimie Razvan
