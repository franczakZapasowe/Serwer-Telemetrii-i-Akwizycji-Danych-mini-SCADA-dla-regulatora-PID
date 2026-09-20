# Serwer Telemetrii PID i Monitor Czasu Rzeczywistego

## Opis Projektu

Ten projekt to wysokowydajny, wielowątkowy serwer telemetrii napisany w C++, zaprojektowany do odbierania, trwałego zapisywania i wizualizacji w czasie rzeczywistym danych z symulowanego regulatora PID. Architektura kładzie nacisk na współbieżność i optymalizację operacji wejścia/wyjścia. System obsługuje strumienie pakietów UDP, bezpiecznie kolejkuje je pomiędzy wątkami, grupuje zapisy do bazy danych wykorzystując transakcje SQLite i przesyła aktualizacje na żywo do interfejsu graficznego opartego na frameworku Qt6.

## Kluczowe Funkcjonalności

* **Komunikacja sieciowa UDP:** Asynchroniczny odbiór pakietów telemetrycznych z zewnętrznych urządzeń IoT lub symulatorów za pomocą natywnych gniazd POSIX.
* **Bezpieczna wielowątkowość (Thread-Safety):** Autorska kolejka `ThradeSafeQ` wykorzystująca `std::mutex` oraz `std::condition_variable` do bezpiecznej dystrybucji danych między wątkiem nasłuchującym a wątkami roboczymi (worker threads).
* **Zoptymalizowana persystencja:** Dedykowana pula wątków (`ThreadPool`) buforuje przychodzące pakiety w pamięci RAM i wykonuje masowe zrzuty danych (100 pakietów na transakcję) do bazy SQLite3 przy użyciu spreparowanych zapytań (prepared statements), co drastycznie redukuje wąskie gardła operacji dyskowych I/O.
* **Reaktywne GUI (Qt6):** Odizolowany most `TelemetryBridge` (wykorzystujący mechanizm Sygnałów i Slotów Qt) bezpiecznie transferuje wartości procesu (PV), wartości zadane (SP) i uchyb (Error) z wątku roboczego do głównego wątku interfejsu graficznego, zapobiegając błędom ochrony pamięci (Segmentation Fault) i blokowaniu pętli zdarzeń.
* **System budowania CMake:** Czysta separacja plików wykonywalnych (symulator Klienta vs aplikacja okienkowa Serwera) zarządzana z poziomu konfiguracji CMake.

## Stos Technologiczny

* **Język:** C++17
* **Framework:** Qt6 (Core, Gui, Widgets, Charts)
* **Baza Danych:** SQLite3 (C API)
* **Narzędzie budowania:** CMake
* **Sieć:** Natywne gniazda POSIX (UDP Sockets)

## Architektura Systemu

1. **Klient (`client.cpp`):** Działa jako fizyczny punkt końcowy (urządzenie IoT). Uruchamia lokalną pętlę symulacji PID, pakuje dane w binarną strukturę `PIDTelemetryPayload` i wysyła je w świat protokołem UDP.
2. **Serwer UDP (`UDPServer.cpp`):** Nasłuchuje na wyznaczonym porcie. Waliduje strukturę przychodzących pakietów i przepycha je do bezpiecznej kolejki wątkowej.
3. **Pula Wątków i DB (`ThreadPool.cpp`):** Pobiera ładunki z kolejki, emituje je do interfejsu Qt przez `TelemetryBridge` i wrzuca do lokalnego bufora. Po osiągnięciu zadanego progu, otwiera transakcję SQLite, wykonuje sparametryzowane operacje `INSERT` i twardo zrzuca dane na dysk.
4. **Interfejs Graficzny (`MainWindow`):** Subskrybuje sygnały z mostu `TelemetryBridge`, aby dynamicznie i płynnie odświeżać wykresy (`QChart`) oraz wskaźniki numeryczne w czasie rzeczywistym.

## Budowanie Projektu

### Wymagania wstępne

* CMake (wersja 3.16+)
* Biblioteki Qt6 (komponenty: Core, Gui, Widgets, Charts)
* Biblioteki deweloperskie SQLite3
* Kompilator wspierający standard C++17 (GCC / Clang / MSVC)

### Instrukcja kompilacji

```bash
mkdir build && cd build
cmake ..
cmake --build .

```

## Użytkowanie

Proces budowania generuje dwa niezależne pliki wykonywalne: `pid` (Serwer z interfejsem Qt) oraz `client` (Symulator urządzenia nadawczego).

1. **Uruchomienie Serwera:**
Odpal aplikację Qt. System automatycznie podniesie w tle wątek nasłuchujący UDP, podłączy się do bazy danych i wyświetli interfejs graficzny.
```bash
./pid

```


2. **Uruchomienie Klienta Telemetrycznego:**
W osobnej karcie terminala uruchom klienta. Symulator natychmiast zacznie generować ruch i strzelać pakietami UDP w stronę serwera.
```bash
./client

```



## Autor

**Mirosław Franczak**

