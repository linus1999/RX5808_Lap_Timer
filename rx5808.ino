const uint16_t channelTable[] PROGMEM = {
#define _CHANNEL_REG_FLO(f) ((f - 479) / 2)
#define _CHANNEL_REG_N(f) (_CHANNEL_REG_FLO(f) / 32)
#define _CHANNEL_REG_A(f) (_CHANNEL_REG_FLO(f) % 32)
#define CHANNEL_REG(f) (_CHANNEL_REG_N(f) << 7) | _CHANNEL_REG_A(f)

  // A
  CHANNEL_REG(5865),
  CHANNEL_REG(5845),
  CHANNEL_REG(5825),
  CHANNEL_REG(5805),
  CHANNEL_REG(5785),
  CHANNEL_REG(5765),
  CHANNEL_REG(5745),
  CHANNEL_REG(5725),

  // B
  CHANNEL_REG(5733),
  CHANNEL_REG(5752),
  CHANNEL_REG(5771),
  CHANNEL_REG(5790),
  CHANNEL_REG(5809),
  CHANNEL_REG(5828),
  CHANNEL_REG(5847),
  CHANNEL_REG(5866),

  // E
  CHANNEL_REG(5705),
  CHANNEL_REG(5685),
  CHANNEL_REG(5665),
  CHANNEL_REG(5645),
  CHANNEL_REG(5885),
  CHANNEL_REG(5905),
  CHANNEL_REG(5925),
  CHANNEL_REG(5945),

  // F
  CHANNEL_REG(5740),
  CHANNEL_REG(5760),
  CHANNEL_REG(5780),
  CHANNEL_REG(5800),
  CHANNEL_REG(5820),
  CHANNEL_REG(5840),
  CHANNEL_REG(5860),
  CHANNEL_REG(5880),

  // C / R
  CHANNEL_REG(5658),
  CHANNEL_REG(5695),
  CHANNEL_REG(5732),
  CHANNEL_REG(5769),
  CHANNEL_REG(5806),
  CHANNEL_REG(5843),
  CHANNEL_REG(5880),
  CHANNEL_REG(5917)

#undef _CHANNEL_REG_FLO
#undef _CHANNEL_REG_A
#undef _CHANNEL_REG_N
#undef CHANNEL_REG
};

int channelOrder[] = {31,
        28,
        24,
        20,
        17,
        13,
        10,
         6,
         8,
        11,
        15,
        18,
        22,
        25,
        29,
        32,
         5,
         3,
         2,
         0,
        35,
        36,
        38,
        39,
         9,
        12,
        16,
        19,
        23,
        26,
        30,
        33,
         1,
         4,
         7,
        14,
        21,
        27,
        34,
        37
};





int RSSI_PIN = A0;
int SPI_CLK = 10;
int SPI_DATA = 8;
int SPI_ENABLE = 9;

void setup() {
  pinMode(RSSI_PIN, INPUT);
  pinMode(SPI_CLK, OUTPUT);
  pinMode(SPI_DATA, OUTPUT);
  pinMode(SPI_ENABLE, OUTPUT);


  Serial.begin(115200);
  while (!Serial);

  /*for (int k = 0; k < 40; k++)
  {
    uint16_t displayInt = pgm_read_word_near(channelTable + k);
    pgm_read_word_near(channelIndexToOrderedIndex + k);

  }*/
  Serial.println();



  delay(1000);
  Serial.println(channelOrder[0]);
  Serial.println(pgm_read_word_near(&channelTable[channelOrder[19]]), HEX);
  //sendRegister(pgm_read_word_near(channelTable + 0));

}

void loop() {
  static int channel = 0;
  static int timer = 0;
  delay(10);
  Serial.print(analogRead(RSSI_PIN));
  Serial.print(",");
  Serial.println(channel);
  timer = (timer + 1) % 5;
  if (timer == 0) {
    channel = (channel + 1) % 40;
      //sendRegister(pgm_read_word_near(&channelTable[channel]));
      sendRegister(pgm_read_word_near(&channelTable[channelOrder[channel]]));
      //Serial.println(channelTable + pgm_read_word_near(channelIndexToOrderedIndex + channel));
  }
}

static void sendRegister(uint32_t data) {
  digitalWrite(SPI_ENABLE, LOW);
  delayMicroseconds(1);
  sendBits(0x01, 4);
  sendBit(HIGH);
  sendBits(data, 20);
  digitalWrite(SPI_ENABLE, HIGH);
  delayMicroseconds(1);
  digitalWrite(SPI_CLK, LOW);
  digitalWrite(SPI_DATA, LOW);
}




static void sendBits (uint32_t bits, uint8_t count) {
  for (uint8_t i = 0; i < count; i++) {
    sendBit(bits & 0x1);
    bits >>= 1;
  }
}

static void sendBit(uint8_t value) {
  digitalWrite(SPI_CLK, LOW);
  delayMicroseconds(1);

  digitalWrite(SPI_DATA, value);
  delayMicroseconds(1);
  digitalWrite(SPI_CLK, HIGH);
  delayMicroseconds(1);

  digitalWrite(SPI_CLK, LOW);
  delayMicroseconds(1);
}
