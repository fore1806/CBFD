String inputString = "";
bool stringComplete = false;

void setup() {
  Serial.begin(9600);
  // Configurar los pines del 2 al 13 como salidas
  for (int pin = 2; pin <= 13; pin++) {
    pinMode(pin, OUTPUT);
  }
  inputString.reserve(50); // Reservar memoria para la cadena de entrada
}

void loop() {
  if (stringComplete) {
    processCommand(inputString);
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void processCommand(String command) {
  command.trim();  // Eliminar espacios y saltos de línea
  command.toUpperCase();  // Convertir a mayúsculas

  if (command.startsWith("SET")) {
    int firstSpace = command.indexOf(' ');
    int secondSpace = command.indexOf(' ', firstSpace + 1);
    if (firstSpace > 0 && secondSpace > 0) {
      int pin = command.substring(firstSpace + 1, secondSpace).toInt();
      String state = command.substring(secondSpace + 1);

      if (pin >= 2 && pin <= 13) {
        if (state == "HIGH") {
          digitalWrite(pin, HIGH);
          Serial.println("Pin " + String(pin) + " set to HIGH");
        } else if (state == "LOW") {
          digitalWrite(pin, LOW);
          Serial.println("Pin " + String(pin) + " set to LOW");
        } else {
          Serial.println("Estado inválido. Usa HIGH o LOW.");
        }
      } else {
        Serial.println("Pin fuera de rango (2-13).");
      }
    } else {
      Serial.println("Comando mal formado. Usa: SET <pin> <HIGH/LOW>");
    }

  } else if (command.startsWith("ALL")) {
    int spaceIndex = command.indexOf(' ');
    if (spaceIndex > 0) {
      String state = command.substring(spaceIndex + 1);
      if (state == "HIGH" || state == "LOW") {
        int value = (state == "HIGH") ? HIGH : LOW;
        for (int pin = 2; pin <= 13; pin++) {
          digitalWrite(pin, value);
        }
        Serial.println("Todos los pines puestos en " + state);
      } else {
        Serial.println("Estado inválido para ALL. Usa HIGH o LOW.");
      }
    } else {
      Serial.println("Comando mal formado. Usa: ALL <HIGH/LOW>");
    }

  } else {
    Serial.println("Comando desconocido.");
  }
}
