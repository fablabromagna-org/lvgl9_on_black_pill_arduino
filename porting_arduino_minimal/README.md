# Arduino Port for Black Pill STM32F411CEU6

Port Arduino del progetto `stm32_nucleo_lvgl_minimal` per Black Pill `STM32F411CEU6`.
I file originali STM32 CubeMX non vengono toccati.

## Stato attuale

- display ILI9341 funzionante
- touch XPT2046 funzionante
- UI EEZ-Studio/LVGL funzionante
- supporto mouse dal PC via seriale USB funzionante
- cursore visibile sul display

Il progetto e' tenuto volutamente con i file nella root dello sketch, perche' con l'IDE Arduino e' piu' pratico vedere tutto senza sottocartelle aggiuntive.

## Demo Video

[![Guarda il video](https://img.youtube.com/vi/Lx8yKO_QNRM/hqdefault.jpg)](https://www.youtube.com/watch?v=Lx8yKO_QNRM)

Clicca sull'immagine per aprire la demo su YouTube.

## Struttura del progetto

File che ha senso modificare a mano:

- [porting_arduino_minimal.ino](./porting_arduino_minimal.ino)
- [app_main.cpp](./app_main.cpp)
- [ui_logic.cpp](./ui_logic.cpp)
- [board_config.h](./board_config.h)
- [ILI9341.cpp](./ILI9341.cpp)
- [XPT2046.cpp](./XPT2046.cpp)
- [TouchController.cpp](./TouchController.cpp)
- [platform_app.cpp](./platform_app.cpp)

File generati o derivati da EEZ-Studio:

- [ui.cpp](./ui.cpp)
- [screens.cpp](./screens.cpp)
- [images.cpp](./images.cpp)
- [styles.cpp](./styles.cpp)
- [eez-flow.cpp](./eez-flow.cpp)
- relativi `.h`

## Librerie richieste

- `LVGL` per Arduino
- `SPI` inclusa nel core STM32duino

Versione consigliata e testata:

- `LVGL v9.2.2`
- release ufficiale: <https://github.com/lvgl/lvgl/releases/tag/v9.2.2>
- download ZIP diretto: <https://github.com/lvgl/lvgl/archive/refs/tags/v9.2.2.zip>

Per il mouse dal PC serve anche Python sul PC con:

```bash
pip install pyserial pynput
```

## Board Arduino

Board testata:

- core ufficiale `STM32 MCU based boards`
- board `BlackPill F411CE`

Note pratiche:

- upload via ST-Link clone: ok
- seriale USB CDC: ok, se abilitata nelle opzioni board

## Impostazioni consigliate nell'IDE Arduino

- `Board`: `BlackPill F411CE`
- `Upload method`: `STLink` oppure quello che usi normalmente
- `USB support`: `CDC`

Se non abiliti `CDC`, il backend mouse via seriale dal PC non puo' funzionare.

## Pinout di default

Bus SPI condiviso su `SPI1`:

- `PA5` -> `SCK`
- `PA6` -> `MISO`
- `PA7` -> `MOSI`

Display ILI9341:

- `PB0` -> `TFT_CS`
- `PB1` -> `TFT_DC`
- `PB10` -> `TFT_RST`

Touch XPT2046:

- `PB12` -> `T_CS`
- `PB14` -> `T_IRQ`

App:

- `PC13` -> LED onboard, attivo basso
- `PA0` -> pulsante utente esterno opzionale verso GND, con `INPUT_PULLUP`

Se vuoi cambiare i pin, modifica [board_config.h](./board_config.h).

## Note display

- flush LVGL via SPI bloccante
- niente DMA
- display e touch condividono lo stesso bus SPI
- orientamento landscape

La resa video e' risultata migliore mantenendo il font del bottone `Press Me` un po' piu' grande del valore originale.

## Mouse dal PC via seriale

Il progetto puo' ricevere un puntatore dal PC via seriale USB e usarlo come input LVGL.
Se arrivano pacchetti seriali validi, hanno priorita' sul touch XPT2046.

Configurazione:

- abilita/disabilita in [board_config.h](./board_config.h) con `BOARD_ENABLE_SERIAL_MOUSE`
- baudrate in `BOARD_SERIAL_BAUDRATE`

Protocollo:

- una riga per aggiornamento
- formato: `M x y b`
- `x`: coordinata X in pixel
- `y`: coordinata Y in pixel
- `b`: `0` rilasciato, `1` premuto

Esempi:

```text
M 120 80 0
M 140 90 1
M 140 90 0
```

Dopo circa 1 secondo senza pacchetti, il backend seriale si disattiva e torna attivo il touch normale.

## Cursore visibile

Il cursore e' gestito in [TouchController.cpp](./TouchController.cpp):

- giallo con bordo nero quando rilasciato
- rosso con bordo bianco quando premuto

Segue sia il touch reale sia il mouse seriale dal PC.

## Script Python per mouse PC

Script pronto:

- [pc_mouse_to_bp.py](./pc_mouse_to_bp.py)

Uso:

```bash
python pc_mouse_to_bp.py
```

Se la porta non e' `COM33`, modifica la costante `PORT` nello script.

## Entry point

- sketch: [porting_arduino_minimal.ino](./porting_arduino_minimal.ino)
- bootstrap: [app_main.cpp](./app_main.cpp)
