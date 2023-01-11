#include <stdio.h>
#include <complex>
#include <vector>
#include <iostream>   // Provide cout, endl stuff;
#include <cstdlib>    // Provides NULL, rand, size_t;
#include <set>        // provides <set> (axiomatic);
#include <algorithm>  // provides <iterator>;
#include <ctime>      // to get a good seed for rand();
#include <cctype>     // Provides toupper;
#include <cmath>      // Provides pow;
#include <climits>    // Provides UINT_MAX;
#include <cstring>    // String class;
#include <string>     // to_string and surprisingly memcpy and more surprisingly array, vector etc.;
#include <fstream>    // Open read and write files;
#include <exception>  // Try catch exceptions to catch
#include <new>        // new[]
// #include <tgmath.h>  // Maybe atanl


// static const long double PI_LD = (long double format) 0x4000921fb54442d18469898cc51701b8;
const static long double PI_LD = 3.141592653589793238462643383279503L;
static const double ERROR_FLOAT = 0.000000001;

//    This is how a function as an argument call looks
//    function( printItem<int>, tree0);
//    This is how a function as an argument definition looks
//    template <class Process, class BTNode>
//    void inorder(Process f, BTNode* node_ptr);
template <class T>
void printItem(T item)
{
    std::cout<< "rule 0: " << item << std::endl;
}

void limits( void) {
   static unsigned int min_bits = (unsigned int) logl(std::numeric_limits<long double>::min())/logl(2.0L);
   static unsigned int max_bits = (unsigned int) logl(std::numeric_limits<long double>::max())/logl(2.0L);

   std::cout << "MIN:                 : " << logl(std::numeric_limits<long double>::min()) << std::endl;
   std::cout << "MAX                  : " << logl(std::numeric_limits<long double>::max()) << std::endl;
   std::cout << "binary 0's -:        : " << logl(std::numeric_limits<long double>::min())/logl(2.0) << std::endl;
   std::cout << "binary 0's +:        : " << logl(std::numeric_limits<long double>::max())/logl(2.0) << std::endl;
   std::cout << "RAND_MAX:            : " << RAND_MAX << std::endl;
   std::cout << "sizeof(char)         : " << sizeof(char) << std::endl;
   std::cout << "sizeof(short int)    : " << sizeof(short int) << std::endl;
   std::cout << "sizeof(int)          : " << sizeof(int) << std::endl;
   std::cout << "sizeof(long int)     : " << sizeof(long int) << std::endl;
   std::cout << "sizeof(long long int): " << sizeof(long long int) << std::endl;
   std::cout << "sizeof(float)        : " << sizeof(float) << std::endl;
   std::cout << "sizeof(double)       : " << sizeof(double) << std::endl;
   std::cout << "sizeof(long double)  : " << sizeof(long double) << std::endl;
   std::cout << std::endl;
   return;
}

typedef struct spoint {
   float i;
   float j;
   float k;
} point;

typedef struct sfacet {
   point normal;
   point v0;
   point v1;
   point v2;
   unsigned short int attribute_byte_count;
   unsigned short int blank;
} facet;

typedef struct sstl_bin {
   char ascii[80];
   unsigned int facet_count;

   facet *t_array;
} stl_bin;

long double get_angle_rad( long double x, long double y);
void invert_endian( unsigned int *value);
void print_facet( facet &f0);
void normalize_point( point &p);
void set_facet_normal( facet &f0, bool t_uv);

int main (int argc, char *argv[])
{
   time_t t0;
   srand( time( &t0));
   int roll0 = rand();
   facet *tetra_array;

   size_t read_count;

   stl_bin stl0;

   if( argc < 2) {
      printf("USAGE: %s filename\n", argv[0]);
      exit(-1);
   }
   FILE *fd;
   fd = fopen( argv[1], "rb+");

   limits();

   read_count = fread( &stl0, 4, 20, fd);
   printf("READ: %lu\n", read_count);
   if( read_count != 20) {
      printf("ERROR reading header: %lu\n", read_count);
   }
   read_count = fread( &stl0.facet_count, 4, 1, fd);
   printf("READ: %lu\n", read_count);
   if( read_count != 1) {
      printf("ERROR reading facet_count: %lu\n", read_count);
   }
   printf("ASCII[80]  : %s\n", stl0.ascii);
   printf("FACET COUNT: %u\n", stl0.facet_count);
   
   stl0.t_array = new(std::nothrow) facet[stl0.facet_count];
   for( int i = 0; i < stl0.facet_count; ++i) {
      read_count = fread( &stl0.t_array[i], 4, 12, fd);
      read_count = fread( &stl0.t_array[i], 2, 1, fd);
      printf("READ: %lu\n", read_count);
   }

printf( "sizeof( spoint)      : %lu\n", sizeof(spoint));
printf( "sizeof( sfacet): %lu\n", sizeof(sfacet));
printf( "sizeof( sstl_bin)    : %lu\n", sizeof( sstl_bin));

   for( int i = 0; i < stl0.facet_count; ++i) {
      printf("TETRAHEDRON: %d\n", i);
      print_facet( stl0.t_array[i]);
   }

   fclose( fd);

   if( argc < 3) {
      return(0);
   }
   FILE *fdw;
   fdw = fopen( argv[2], "wb");
   char buffer[81] = "STL File generation test.                                                       ";
   stl_bin stl1;

   memcpy( &stl1.ascii, buffer, 80);
   stl1.facet_count = 8;
   stl1.t_array = new(std::nothrow) facet[stl1.facet_count];

   point p_array[6] = { { 0.0f, 0.0f, 0.0f}, // A
 {1.0f, 1.0f, 0.0f},                         // B
 {3.0f, 1.0f, 0.0f},                         // C
 {0.0f, 0.0f, 1.0f},                         // D
 {1.0f, 1.0f, 1.0f},                         // E
 {3.0f, 1.0f, 1.0f} };                       // F
   facet f_array[8] = { { p_array[0], p_array[0], p_array[1], p_array[2], 0, 0},
  { p_array[0], p_array[0], p_array[2], p_array[5], 0, 0},
  { p_array[0], p_array[0], p_array[5], p_array[3], 0, 0},
  { p_array[0], p_array[3], p_array[5], p_array[4], 0, 0},
  { p_array[0], p_array[0], p_array[4], p_array[1], 0, 0},
  { p_array[0], p_array[0], p_array[3], p_array[4], 0, 0},
  { p_array[0], p_array[1], p_array[5], p_array[2], 0, 0},
  { p_array[0], p_array[1], p_array[4], p_array[5], 0, 0} };
// ABC, ACF, AFD, DFE, AEB, ADE, BFC, BEF

/*******
   f0.normal.i = 0.0f;
   f0.normal.j = 0.0f;
   f0.normal.k = 0.0f;

   f0.v0.i = p_array[0];
   f0.v0.j = p_array[0];
   f0.v0.k = p_array[0];

   f0.v1.i = p_array[1];
   f0.v1.j = p_array[1];
   f0.v1.k = p_array[1];

   f0.v2.i = p_array[2];
   f0.v2.j = p_array[2];
   f0.v2.k = p_array[2];
   f0.attribute_byte_count = 0;
************/

   set_facet_normal( f_array[0], true);
   set_facet_normal( f_array[1], true);
   set_facet_normal( f_array[2], true);
   set_facet_normal( f_array[3], true);
   set_facet_normal( f_array[4], true);
   set_facet_normal( f_array[5], true);
   set_facet_normal( f_array[6], true);
   set_facet_normal( f_array[7], true);

   fwrite( buffer, 4, 20, fdw);
   fwrite( &stl1.facet_count, 4, 1, fdw);

for( int i = 0; i < stl1.facet_count; ++i) {
   fwrite( &f_array[i].normal.i, 4, 1, fdw);
   fwrite( &f_array[i].normal.j, 4, 1, fdw);
   fwrite( &f_array[i].normal.k, 4, 1, fdw);

   fwrite( &f_array[i].v0.i, 4, 1, fdw);
   fwrite( &f_array[i].v0.j, 4, 1, fdw);
   fwrite( &f_array[i].v0.k, 4, 1, fdw);

   fwrite( &f_array[i].v1.i, 4, 1, fdw);
   fwrite( &f_array[i].v1.j, 4, 1, fdw);
   fwrite( &f_array[i].v1.k, 4, 1, fdw);

   fwrite( &f_array[i].v2.i, 4, 1, fdw);
   fwrite( &f_array[i].v2.j, 4, 1, fdw);
   fwrite( &f_array[i].v2.k, 4, 1, fdw);

   fwrite( &f_array[i].attribute_byte_count, 2, 1, fdw);
}

   fclose( fdw);
   return 0;
}
// --------------------------------- END MAIN ---------------------------------------

long double get_angle_rad( long double x, long double y)
{
   bool sign_x = std::signbit( x);
   bool sign_y = std::signbit( x);
   long double angle = 0.0;
   x = fabsl( x);
   y = fabsl( y);

   if( x < 0.000000001) {
      if( y < 0.000000001) {  // 0.0 / 0.0
         angle = 0.0;
      } else {                // +/-Y/ 0.0
         if( sign_y) {        // -Y / 0.0
            angle = 1.5*PI_LD;
         } else {             // Y / 0.0
            angle = PI_LD/2.0;
         }
      }
      return(angle);
   }
   angle = atanl( fabs(y/x));
   if ( sign_x) {            // +/-Y / -X
      if( sign_y) {          // -Y / -X
         angle += PI_LD;
      } else {               //  Y / -X
         angle = PI_LD - angle;
      }
   } else {                  // +/-Y / X
      if( sign_y) {          // -Y / X
         angle = 2.0*PI_LD - angle;
      }                      //  Y / X
   }
   return( angle);
}

void invert_endian( unsigned int *value)
{
   unsigned int byte0;
   unsigned int byte1;
   unsigned int byte2;
   unsigned int byte3;

   byte0 = (*value & 0x000000FF) << 24;
   byte1 = (*value & 0x0000FF00) << 8;
   byte2 = (*value & 0x00FF0000) >> 8;
   byte3 = (*value & 0xFF000000) >> 24;

   *value = byte0 + byte1 + byte2 + byte3;
   
   return;
}

void print_facet( facet &f0)
{
   printf("NORMAL : ( %f, %f, %f)\n", f0.normal.i, f0.normal.j, f0.normal.k);
   printf("Vertex0: ( %f, %f, %f)\n", f0.v0.i, f0.v0.j, f0.v0.k);
   printf("vertex1: ( %f, %f, %f)\n", f0.v1.i, f0.v1.j, f0.v1.k);
   printf("Vertex2: ( %f, %f, %f)\n", f0.v2.i, f0.v2.j, f0.v2.k);
   printf("ATTRIBUTE: %hu\n", f0.attribute_byte_count);

   printf("NORMAL : ( %x, %x, %x)\n",  (unsigned int) f0.normal.i, (unsigned int) f0.normal.j, (unsigned int) f0.normal.k);
   printf("Vertex0: ( %x, %x, %x)\n",  (unsigned int) f0.v0.i,  (unsigned int) f0.v0.j,  (unsigned int) f0.v0.k);
   printf("vertex1: ( %x, %x, %x)\n",  (unsigned int) f0.v1.i,  (unsigned int) f0.v1.j,  (unsigned int) f0.v1.k);
   printf("Vertex2: ( %x, %x, %x)\n",  (unsigned int) f0.v2.i,  (unsigned int) f0.v2.j,  (unsigned int) f0.v2.k);
   printf("ATTRIBUTE: %hu\n", f0.attribute_byte_count);
}

void normalize_point( point &p)
{
   float magnitude = sqrt( pow(p.i, 2.0) + pow(p.j, 2.0) + pow(p.k, 2.0));

   if( magnitude < ERROR_FLOAT) {
      return;
   }

   p.i = p.i/magnitude;
   p.j = p.j/magnitude;
   p.k = p.k/magnitude;

   return;
}

// given 3 points in R^3 find the plane associated with them and return unit normal vector
// if t_uv is true p0 is basis for vectors defined to get cross product  p1-p0 X p2-p0
// note y-term in cross product is inverted I seem to have forgotten why (and derivation which I once knew)
// ah turns out that i x k = -j causing inversion of y term because this is really quaternion space
// This must make 4D and higher more quaternially confusing to cross also nope
// next place with cross products is octonions and 7D space
void set_facet_normal( facet &f0, bool t_uv)
{
   f0.normal.i = (f0.v1.j-f0.v0.j) * (f0.v2.k-f0.v0.k) - (f0.v2.j-f0.v0.j) * (f0.v1.k-f0.v0.k);
   f0.normal.j = (f0.v2.i-f0.v0.i) * (f0.v1.k-f0.v0.k) - (f0.v1.i-f0.v0.i) * (f0.v2.k-f0.v0.k);
   f0.normal.k = (f0.v1.i-f0.v0.i) * (f0.v2.j-f0.v0.j) - (f0.v2.i-f0.v0.i) * (f0.v1.j-f0.v0.j);

   if( t_uv) {
      normalize_point( f0.normal);
   }
   return;
}
