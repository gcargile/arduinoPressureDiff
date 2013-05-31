#define PSI_PER_FOOT_FRESH .43  
#define PSI_PER_FOOT_SEA   .44

/*
SEE: http://www.geotechenv.com/Reference_Pages/water_measure_tables.pdf

Volume of water contained per foot of well depth
Well casing diameter (inches)	Water volume per foot of water depth (Gallons)*
                       4	.65
		       6	1.47
		       8	2.61
		       10	4.08
		       12	5.88
		       18	13.22
		       24	23.50
		       30	36.72
		       36	52.87
*Volume of water calculated as the volume of a cylinder multiplied by 7.48 gallons/cubic foot.

*/

typedef struct {
  int depth;
  int diameter;
  int level;
} Well;

long getWellVolume(Well well) {
  return volume(well.level, well.diameter);
}

int getWellGallons(Well well) {
  return gallons(getWellVolume(well));
}

/* 
   Alan's Well
   Total Depth = 
   Pump  Depth =
   Full  Depth = 
   Diameter    =
*/
