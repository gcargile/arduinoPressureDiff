String newLine() {
  return "\n\r";
}

String fillLine(char fill) {
  String retval;
  int i;
  for (i = 0; i < 80; i++)
    retval.concat(fill);
  return retval;
}
