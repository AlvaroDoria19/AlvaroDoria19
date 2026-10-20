#include <ArduinoRobot.h>
#include <avr/pgmspace.h>

// ============================================================
// MENU DEMOSTRATIVO PARA EL ARDUINO ROBOT ORIGINAL
//
// Melodia extraida de la pista "Voice" del archivo MIDI
// bolivia_national_voice.mid (100 BPM, 4/4).
// Incluye los cinco modos del proyecto y un modo opcional para HC-SR04.
// ============================================================

int opcion = 0;
const byte TOTAL_DEMOS = 6;
const byte OPCIONES_POR_PANTALLA = 6;
byte pasoBaile = 0;
unsigned long ultimaOrdenBaile = 0;

// La pista vocal del MIDI tiene 176 notas.
const int CANTIDAD_NOTAS = 176;

// Numeros de nota MIDI. Se guardan en la memoria de programa
// para no ocupar la poca memoria RAM del robot.
const byte notas[CANTIDAD_NOTAS] PROGMEM = {
  67, 76, 76, 74, 72, 72, 71, 69, 69, 67, 65, 64, 62, 64, 65, 67,
  69, 71, 72, 74, 76, 72, 67, 76, 76, 74, 72, 72, 71, 69, 69, 67,
  67, 66, 64, 62, 67, 71, 76, 74, 72, 69, 67, 67, 67, 67, 67, 67,
  67, 65, 67, 68, 70, 67, 68, 67, 65, 63, 65, 67, 68, 70, 74, 72,
  70, 68, 67, 67, 67, 67, 69, 67, 69, 67, 69, 67, 69, 67, 65, 64,
  67, 67, 69, 67, 72, 71, 69, 69, 67, 65, 64, 67, 67, 67, 69, 67,
  69, 67, 69, 67, 69, 67, 69, 71, 72, 69, 65, 69, 69, 67, 72, 71,
  69, 69, 67, 65, 64, 75, 73, 72, 68, 75, 75, 73, 72, 68, 70, 68,
  67, 65, 67, 68, 70, 72, 73, 74, 75, 72, 75, 73, 72, 70, 68, 72,
  72, 73, 74, 75, 72, 72, 74, 76, 72, 72, 74, 71, 71, 72, 72, 74,
  76, 72, 72, 74, 71, 71, 72, 72, 74, 76, 72, 72, 74, 71, 71, 72
};

// Tiempo durante el cual suena cada nota, en milisegundos.
const unsigned int tiempoSonando[CANTIDAD_NOTAS] PROGMEM = {
  450, 150, 600, 450, 150, 600, 450, 150, 1200, 284, 450, 150, 450, 141, 450, 150,
  450, 141, 450, 150, 1200, 284, 450, 150, 600, 450, 150, 600, 450, 150, 1200, 284,
  199, 188, 201, 600, 450, 150, 450, 141, 450, 150, 600, 450, 150, 600, 450, 150,
  450, 141, 450, 150, 1200, 284, 450, 150, 450, 141, 450, 150, 450, 141, 450, 150,
  900, 284, 300, 450, 150, 450, 141, 450, 150, 450, 141, 450, 150, 900, 284, 300,
  450, 150, 600, 450, 150, 450, 141, 199, 188, 201, 600, 450, 150, 450, 141, 450,
  150, 450, 141, 450, 150, 900, 150, 141, 300, 300, 450, 150, 600, 300, 150, 450,
  150, 199, 188, 201, 600, 450, 150, 1200, 284, 150, 450, 150, 1200, 284, 450, 150,
  450, 141, 450, 150, 450, 141, 450, 150, 1200, 284, 450, 150, 900, 284, 300, 150,
  199, 199, 201, 1200, 284, 450, 150, 600, 450, 150, 600, 450, 150, 600, 450, 150,
  600, 450, 150, 600, 450, 150, 600, 450, 150, 600, 450, 150, 600, 450, 150, 600
};

// Silencio que viene despues de cada nota, tambien obtenido del MIDI.
const unsigned int tiempoSilencio[CANTIDAD_NOTAS] PROGMEM = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 316, 0, 0, 0, 9, 0, 0,
  0, 9, 0, 0, 0, 316, 0, 0, 0, 0, 0, 0, 0, 0, 0, 316,
  0, 12, 0, 0, 0, 0, 0, 9, 0, 0, 1200, 0, 0, 0, 0, 0,
  0, 9, 0, 0, 0, 316, 0, 0, 0, 9, 0, 0, 0, 9, 0, 0,
  0, 16, 300, 0, 0, 0, 9, 0, 0, 0, 9, 0, 0, 0, 16, 300,
  0, 0, 0, 0, 0, 0, 9, 0, 12, 0, 1200, 0, 0, 0, 9, 0,
  0, 0, 9, 0, 0, 0, 0, 9, 0, 0, 0, 0, 0, 150, 0, 0,
  0, 0, 12, 0, 1200, 0, 0, 0, 166, 0, 0, 0, 0, 316, 0, 0,
  0, 9, 0, 0, 0, 9, 0, 0, 0, 316, 0, 0, 0, 16, 150, 0,
  0, 0, 0, 0, 316, 0, 0, 0, 0, 0, 0, 0, 0, 1200, 0, 0,
  0, 0, 0, 0, 0, 0, 1200, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Incrementos de fase de Squawk para 44.100 muestras/s,
// correspondientes a las notas MIDI 62 (Re4) a 76 (Mi5).
const unsigned int frecuenciasSquawk[] PROGMEM = {
  436, 462, 490, 519, 550, 583, 617, 654,
  693, 734, 778, 824, 873, 925, 980
};

// Imprime textos directamente desde flash para ahorrar la RAM del robot.
void textoFlash(const __FlashStringHelper* texto, int x, int y) {
  Robot.setTextWrap(false);
  Robot.setCursor(x, y);
  Robot.print(texto);
}

void setup() {
  Robot.begin();
  Robot.beginTFT();
  Robot.beginSpeaker();
  Robot.setMode(MODE_SIMPLE);
  Robot.motorsStop();

  mostrarMenu();
}

void loop() {
  int tecla = leerPulsacion();

  if (tecla == BUTTON_NONE) {
    delay(40);
    return;
  }

  if (tecla == BUTTON_UP || tecla == BUTTON_LEFT) {
    opcion--;
    if (opcion < 0) opcion = TOTAL_DEMOS - 1;
    mostrarMenu();
  } else if (tecla == BUTTON_DOWN || tecla == BUTTON_RIGHT) {
    opcion++;
    if (opcion >= TOTAL_DEMOS) opcion = 0;
    mostrarMenu();
  } else if (tecla == BUTTON_MIDDLE) {
    abrirDemo();
    Robot.setMode(MODE_SIMPLE);
    Robot.motorsStop();
    mostrarMenu();
  }
}

void mostrarMenu() {
  Robot.fillScreen(BLACK);
  Robot.stroke(255, 255, 255);
  Robot.setTextSize(1);

  textoFlash(F("ARDUINO SMART ROBOT"), 3, 4);

  for (int i = 0; i < TOTAL_DEMOS; i++) {
    mostrarNombreDemo(i, 23 + i * 16);
  }

  textoFlash(F("ARR/ABAJO: ELEGIR"), 3, 127);
  textoFlash(F("CENTRO: INICIAR"), 3, 145);
}

void modoBaileHimno() {
  Robot.fillScreen(BLACK);
  Robot.stroke(255, 255, 255);
  textoFlash(F("HIMNO + BAILE"), 5, 10);
  textoFlash(F("Deja espacio libre"), 5, 35);
  textoFlash(F("y coloca pilas."), 5, 52);
  textoFlash(F("CENTRO: EMPEZAR"), 5, 92);
  textoFlash(F("Cualquier boton"), 5, 112);
  textoFlash(F("para ambos"), 5, 129);

  if (esperarPulsacion() != BUTTON_MIDDLE) return;

  Robot.fillScreen(BLACK);
  Robot.stroke(255, 255, 255);
  textoFlash(F("HIMNO DE BOLIVIA"), 5, 10);
  textoFlash(F("BAILANDO..."), 5, 35);
  textoFlash(F("PERILLA: VOLUMEN"), 5, 65);
  textoFlash(F("BOTON: CANCELAR"), 5, 95);

  // Reactiva el sintetizador por si se habia detenido antes.
  Robot.play();
  Robot.setMode(MODE_SIMPLE);
  Robot.motorsStop();
  pasoBaile = 0;
  ultimaOrdenBaile = millis() - 700;

  for (int i = 0; i < CANTIDAD_NOTAS; i++) {
    byte notaMidi = pgm_read_byte(&notas[i]);
    unsigned int suena = pgm_read_word(&tiempoSonando[i]);
    unsigned int silencio = pgm_read_word(&tiempoSilencio[i]);

    // Comenzar la nota.
    osc[2].freq = frecuenciaInterna(notaMidi);

    // Durante la nota se siguen leyendo la perilla y los botones.
    if (!esperarConControles(suena)) {
      apagarSonido();
      Robot.motorsStop();
      esperarQueSuelteElBoton();
      return;
    }

    // Hacer el silencio indicado por el MIDI.
    osc[2].freq = 0;
    if (!esperarConControles(silencio)) {
      apagarSonido();
      Robot.motorsStop();
      esperarQueSuelteElBoton();
      return;
    }
  }

  apagarSonido();
  Robot.motorsStop();
}

// Espera sin bloquear los controles.
// Devuelve false si se presiono cualquier boton.
bool esperarConControles(unsigned int tiempo) {
  unsigned long inicio = millis();

  while (millis() - inicio < tiempo) {
    // La perilla entrega 0 a 1023. Squawk usa volumen 0 a 127.
    int volumen = map(Robot.knobRead(), 0, 1023, 0, 127);
    osc[2].vol = volumen;

    if (millis() - ultimaOrdenBaile >= 700) {
      switch (pasoBaile) {
        case 0: Robot.motorsWrite(100, 100); break;
        case 1: Robot.motorsWrite(80, -80); break;
        case 2: Robot.motorsWrite(100, 100); break;
        default: Robot.motorsWrite(-80, 80); break;
      }
      pasoBaile = (pasoBaile + 1) % 4;
      ultimaOrdenBaile = millis();
    }

    // BUTTON_NONE significa que no hay ningun boton pulsado.
    if (Robot.keyboardRead() != BUTTON_NONE) {
      return false;
    }

    delay(5);
  }

  return true;
}

void esperarQueSuelteElBoton() {
  while (Robot.keyboardRead() != BUTTON_NONE) {
    delay(20);
  }
}

void apagarSonido() {
  osc[2].freq = 0;
  osc[2].vol = 0;
}

// Convierte una nota MIDI en el valor interno que usa Squawk.
// La melodia solo necesita las notas MIDI 62 (Re4) a 76 (Mi5).
unsigned int frecuenciaInterna(byte notaMidi) {
  if (notaMidi < 62 || notaMidi > 76) {
    return 0;
  }

  return pgm_read_word(&frecuenciasSquawk[notaMidi - 62]);
}

void abrirDemo() {
  switch (opcion) {
    case 0: modoControlManual(); break;
    case 1: demoSensoresSuelo(); break;
    case 2: demoBrujula(); break;
    case 3: modoBaileHimno(); break;
    case 4: demoSeguirLinea(); break;
    case 5: demoHCSR04(); break;
  }
}

void mostrarNombreDemo(int numero, int y) {
  Robot.stroke(255, 255, 255);
  if (numero == opcion) textoFlash(F(">"), 2, y);

  switch (numero) {
    case 0: textoFlash(F("CONTROL MANUAL"), 14, y); break;
    case 1: textoFlash(F("SENSORES IR"), 14, y); break;
    case 2: textoFlash(F("BRUJULA"), 14, y); break;
    case 3: textoFlash(F("HIMNO + BAILE"), 14, y); break;
    case 4: textoFlash(F("SEGUIR LINEA"), 14, y); break;
    case 5: textoFlash(F("HC-SR04 OPCIONAL"), 14, y); break;
  }
}

int leerPulsacion() {
  int tecla = Robot.keyboardRead();
  if (tecla != BUTTON_NONE) {
    esperarQueSuelteElBoton();
    delay(120);
  }
  return tecla;
}

int esperarPulsacion() {
  int tecla = BUTTON_NONE;
  while (tecla == BUTTON_NONE) {
    tecla = leerPulsacion();
  }
  return tecla;
}

void modoControlManual() {
  Robot.setMode(MODE_SIMPLE);
  Robot.motorsStop();
  Robot.fillScreen(BLACK);
  Robot.stroke(255, 255, 255);
  textoFlash(F("CONTROL MANUAL"), 4, 8);
  textoFlash(F("ARRIBA: ADELANTE"), 4, 27);
  textoFlash(F("ABAJO: ATRAS"), 4, 43);
  textoFlash(F("IZQ/DER: GIRAR"), 4, 59);
  textoFlash(F("PERILLA: VELOCIDAD"), 4, 79);
  textoFlash(F("DURACION: 3 SEGUNDOS"), 4, 95);
  textoFlash(F("PULSA FLECHA UNA VEZ"), 4, 112);
  textoFlash(F("CENTRO: PARAR/SALIR"), 3, 143);

  int teclaAnterior = BUTTON_NONE;
  bool movimientoActivo = false;
  unsigned long inicioMovimiento = 0;
  const unsigned long duracionMovimiento = 3000;

  while (true) {
    int tecla = Robot.keyboardRead();
    if (tecla == BUTTON_MIDDLE) {
      Robot.motorsStop();
      esperarQueSuelteElBoton();
      return;
    }

    if (tecla == BUTTON_NONE) {
      teclaAnterior = BUTTON_NONE;
    } else if (tecla != teclaAnterior) {
      teclaAnterior = tecla;

      int velocidad = map(Robot.knobRead(), 0, 1023, 0, 180);
      int motorIzq = 0;
      int motorDer = 0;

      if (tecla == BUTTON_UP) {
        motorIzq = velocidad;
        motorDer = velocidad;
      } else if (tecla == BUTTON_DOWN) {
        motorIzq = -velocidad;
        motorDer = -velocidad;
      } else if (tecla == BUTTON_LEFT) {
        motorIzq = -velocidad;
        motorDer = velocidad;
      } else if (tecla == BUTTON_RIGHT) {
        motorIzq = velocidad;
        motorDer = -velocidad;
      }

      if (motorIzq != 0 || motorDer != 0) {
        Robot.motorsWrite(motorIzq, motorDer);
        inicioMovimiento = millis();
        movimientoActivo = true;

        Robot.fillRect(4, 112, 120, 10, BLACK);
        Robot.stroke(255, 255, 255);
        if (tecla == BUTTON_UP) textoFlash(F("ADELANTE"), 4, 112);
        else if (tecla == BUTTON_DOWN) textoFlash(F("ATRAS"), 4, 112);
        else if (tecla == BUTTON_LEFT) textoFlash(F("GIRANDO IZQUIERDA"), 4, 112);
        else textoFlash(F("GIRANDO DERECHA"), 4, 112);

        Robot.fillRect(4, 127, 120, 10, BLACK);
        Robot.stroke(255, 255, 255);
        textoFlash(F("V:"), 4, 127);
        Robot.text(velocidad, 16, 127);
        textoFlash(F("T:"), 57, 127);
        Robot.text((int)duracionMovimiento, 69, 127);
        textoFlash(F("ms"), 105, 127);
      }
    }

    if (movimientoActivo && millis() - inicioMovimiento >= duracionMovimiento) {
      Robot.motorsStop();
      movimientoActivo = false;
      Robot.fillRect(4, 112, 120, 10, BLACK);
      Robot.stroke(255, 255, 255);
      textoFlash(F("TIEMPO CUMPLIDO"), 4, 112);
    }
  }
}

void demoHCSR04() {
  Robot.fillScreen(BLACK);
  Robot.stroke(255, 255, 255);
  textoFlash(F("HC-SR04 OPCIONAL"), 3, 8);
  textoFlash(F("TRIG -> TKD0"), 3, 31);
  textoFlash(F("ECHO -> TKD1"), 3, 49);
  textoFlash(F("VCC: 5V  GND: GND"), 3, 67);
  textoFlash(F("CENTRO: INICIAR"), 3, 105);
  textoFlash(F("Cualquier boton"), 3, 125);
  textoFlash(F("detiene la lectura"), 3, 143);

  if (esperarPulsacion() != BUTTON_MIDDLE) return;

  pinMode(TKD0, OUTPUT);
  pinMode(TKD1, INPUT);
  digitalWrite(TKD0, LOW);

  while (true) {
    if (Robot.keyboardRead() != BUTTON_NONE) {
      esperarQueSuelteElBoton();
      return;
    }

    digitalWrite(TKD0, LOW);
    delayMicroseconds(2);
    digitalWrite(TKD0, HIGH);
    delayMicroseconds(10);
    digitalWrite(TKD0, LOW);

    unsigned long eco = pulseIn(TKD1, HIGH, 25000UL);
    Robot.fillRect(3, 35, 120, 22, BLACK);
    Robot.stroke(255, 255, 255);
    if (eco == 0) {
      textoFlash(F("SIN ECO"), 3, 39);
    } else {
      int centimetros = eco / 58;
      Robot.text(centimetros, 3, 39);
      textoFlash(F("cm"), 34, 39);
    }
    delay(60);
  }
}

void demoBrujula() {
  Robot.fillScreen(WHITE);
  Robot.stroke(BLACK);
  textoFlash(F("BRUJULA: GRADOS"), 2, 5);
  textoFlash(F("GIRA EL ROBOT"), 2, 145);

  unsigned long ultimaLectura = 0;
  while (true) {
    if (leerPulsacion() != BUTTON_NONE) return;

    if (millis() - ultimaLectura >= 250) {
      uint16_t grados = Robot.compassRead();
      Robot.drawCompass(grados);
      ultimaLectura = millis();
    }
  }
}

void demoSensoresSuelo() {
  Robot.fillScreen(BLACK);
  Robot.stroke(255, 255, 255);
  textoFlash(F("SENSORES INFRARROJOS"), 2, 5);
  for (int i = 0; i < 5; i++) {
    int y = 28 + i * 18;
    textoFlash(F("IR"), 5, y);
    Robot.text(i, 22, y);
  }
  textoFlash(F("CAMBIA SUPERFICIE"), 2, 125);
  textoFlash(F("BOTON: VOLVER"), 2, 145);

  while (true) {
    Robot.updateIR();
    Robot.stroke(255, 255, 255);
    for (int i = 0; i < 5; i++) {
      int y = 28 + i * 18;
      Robot.fillRect(55, y, 55, 9, BLACK);
      Robot.text((int)Robot.IRarray[i], 55, y);
    }

    if (leerPulsacion() != BUTTON_NONE) return;
    delay(100);
  }
}

void demoSeguirLinea() {
  Robot.fillScreen(BLACK);
  Robot.stroke(255, 255, 255);
  textoFlash(F("SEGUIR LINEA"), 3, 8);
  textoFlash(F("Necesita pista con"), 3, 32);
  textoFlash(F("cinta negra y pilas."), 3, 49);
  textoFlash(F("CENTRO: INICIAR"), 3, 92);
  textoFlash(F("Cualquier boton"), 3, 112);
  textoFlash(F("detiene el robot."), 3, 129);

  if (esperarPulsacion() != BUTTON_MIDDLE) return;

  Robot.lineFollowConfig(11, 7, 50, 5);
  Robot.setMode(MODE_LINE_FOLLOW);

  unsigned long inicio = millis();
  bool termino = false;
  while (millis() - inicio < 15000) {
    if (Robot.keyboardRead() != BUTTON_NONE) {
      esperarQueSuelteElBoton();
      break;
    }
    if (Robot.isActionDone()) {
      termino = true;
      break;
    }
    delay(20);
  }

  Robot.pauseMode(true);
  Robot.setMode(MODE_SIMPLE);
  Robot.motorsStop();

  Robot.fillScreen(BLACK);
  Robot.stroke(255, 255, 255);
  if (termino) {
    textoFlash(F("FIN DE PISTA"), 3, 35);
  } else {
    textoFlash(F("SEGUIDOR DETENIDO"), 3, 35);
  }
  textoFlash(F("BOTON: VOLVER"), 3, 100);
  esperarPulsacion();
}
