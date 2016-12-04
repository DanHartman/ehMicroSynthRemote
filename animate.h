byte noWay(byte temp) {
	static byte r = 20;
  static byte g = 35;
  static byte b = 187;
  static bool r_up = TRUE;
  static bool g_up = FALSE;
  static bool b_up = FALSE;

  if(temp == 0) {
    if(r_up) {
	  	if(r < 255) r += 1;
	  	else r_up = FALSE;
	  }
	  else {
	  	if(r) r -= 1;
	  	else r_up = TRUE;
	  }
  	return r;
  }
	  
  if(temp == 1) {
    if(g_up) {
	  	if(g < 255) g += 1;
	  	else g_up = FALSE;
	  }
	  else {
	  	if(g) g -= 1;
	  	else g_up = TRUE;
	  }
  	return g;
  }
    
	if(temp == 2) {
		if(b_up) {
	  	if(b < 255) b += 1;
	  	else b_up = FALSE;
	  }
	  else {
	  	if(b) b -= 1;
	  	else b_up = TRUE;
	  }
	  return b;
	}
}