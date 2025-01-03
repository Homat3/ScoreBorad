#include "decoder.h"

int ReadIntMsg(String &from){
  if (from[0] != '-') return -1;
  else from.remove(0, 1);
  int res = 0;
  while(from[0] != '-'){
    res = from[0] - '0' + res * 10;
    from.remove(0, 1);
  }
  return res;
}
String ReadStringMsg(String &from){
  if (from[0] != '-') return String("Null");
  else from.remove(0, 1);
  String res = "";
  while(from[0] != '-'){
    res += from[0];
    from.remove(0, 1);
  }
  return res;
}