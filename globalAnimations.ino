// functions defined in this file rely on multiple sections,
// so they are not included as Section class functions.

void distribute(){
  int i = 0;
  int delta = 255/NUM_SECTIONS;
  int sum = 0;
  while (i < NUM_SECTIONS){
     sections[i].h = delta;
     sum += delta;
     delta *= 2;
  }
  sections[i-2].h = sum/(NUM_SECTIONS - 2);
}

// todo
