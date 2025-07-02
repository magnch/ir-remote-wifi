#define ADDRESS 0x707

enum Code {
  power = 0,
  source,
  pad_up,
  pad_down,
  ok,
  vol_up,
  vol_down
};

int code_to_hex(Code code) {
  switch(code) {
    case power:
      return 0xFD02;
    case source:
      return 0xFE01;
    case pad_up:
      return 0x60;
    case pad_down:
      return 0x61;
    case ok:
      return 0x68;
    case vol_up:
      return 0x7;
    case vol_down:
      return 0xB;
    default:
      return 0xFD02;  
  }
}

String code_to_string(Code code) {
  switch(code) {
    case power:
      return "Power";
    case source:
      return "Source";
    case pad_up:
      return "D-Pad up";
    case pad_down:
      return "D-Pad down";
    case ok:
      return "OK";
    case vol_up:
      return "Volume up";
    case vol_down:
      return "Volume down";
    default:
      return "Power";  
  }
}
