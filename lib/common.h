#ifndef COMMON_H
#define COMMON_H

#include <cstring>
#include <iostream>
#include <map>

namespace life {
   // Coordenadas do pixel (x,y)
   struct Coordinates
   {
       typedef long coord_type; // Tipo da coordenada
       coord_type x; // Ponto x
       coord_type y; // Ponto y
       // Cria o ponto da coordenada
       Coordinates( coord_type x=0, coord_type y=0 ) : x{x}, y{y} {/*empty*/}
       // Construtor cópia
       Coordinates( const Coordinates& clone ) : x{ clone.x }, y{ clone.y } {/*empty*/}
       // Operador de comparação
       bool operator==( const Coordinates& rhs ) const
       { return x==rhs.x and y==rhs.y ; }
       // Operador de comparação de diferença
       bool operator!=( const Coordinates& rhs ) const
       { return not operator==( rhs ) ; }
       // Decrementador
       Coordinates operator--()
       {
           --x; --y;
           return *this;
       }
       // Decrementa apos a operação
       Coordinates operator--(int)
       {
           Coordinates temp(*this);
           x--; y--;
           return temp;
       }
       /// Operador de streams
       friend std::ostream& operator<<( std::ostream& os, const Coordinates& p )
       {
           os << "(x="<< p.x << ", y=" << p.y << ")";
           return os;
       }
   };

   /// Representa a coordenada na matrix
   struct Matrix
   {
       typedef long coord_type;
       coord_type r; // linha
       coord_type c; // coluna
       // Cria a localização na matriz
       Matrix( coord_type r=0, coord_type c=0 ) : r{r}, c{c} {/*empty*/}
       // Construtor cópia
       Matrix( const Matrix& clone ) : r{ clone.r }, c{ clone.c } {/*empty*/}
       // Comparador de igualdade
       bool operator==( const Matrix& rhs ) const
       { return r==rhs.r and c==rhs.c ; }
       // Comparador de diferença
       bool operator!=( const Matrix& rhs ) const
       { return not operator==( rhs ) ; }
       bool operator<( const Matrix& rhs ) const
       { return (r < rhs.r or ( r == rhs.r and c < rhs.c ) ); }
       // Decrementador anterior
       Matrix operator--()
       {
           --r; --c;
           return *this;
       }
       // Decrementador posterior
       Matrix operator--(int)
       {
           Matrix temp(*this);
           r--; c--;
           return temp;
       }
       // Operador de stream
       friend std::ostream& operator<<( std::ostream& os, const Matrix& l )
       {
           os << "(row="<< l.r << ", col=" << l.c << ")";

           return os;
       }
   };

   // Converte localização na matrix para um ponto de coordenada
   inline Coordinates CoordinateToMatrix( const Matrix &l )
   { return Coordinates{ l.c, l.r }; }

   // Converte coordenada para localização na matriz
   inline Matrix MatrixToCoordinate( const Coordinates &p )
   { return Matrix{ p.y, p.x }; }

   // Representa a cor no RGB
   struct Color
   {

       typedef unsigned char color_t; 

       enum channel_e : color_t { R=0, 
                                  G=1, 
                                  B=2, 
                                  A=3  
       };  

       static constexpr short N_CHANNELS = 4;

    
       color_t channels[N_CHANNELS]; 
 
       // Construtor da cor
       Color(color_t r=0, color_t g=0, color_t b=0, color_t a=0) : channels{r,g,b,a} {/*empty*/}

       // Construtor cópia
       Color( const Color& clone )
       {
           std::memcpy( channels, clone.channels, sizeof(color_t)*N_CHANNELS );
       }

       // Operador de atribuição
       Color& operator = ( const Color& rhs )
       {
           if ( &rhs != this )
               std::memcpy( channels, rhs.channels, sizeof(color_t)*N_CHANNELS );
           return *this;
       }

       // Operador de comparação
       bool operator == ( const Color& rhs )
       {
           return not std::memcmp( channels, rhs.channels, sizeof(color_t)*N_CHANNELS );
       }
       
       friend std::ostream& operator<<( std::ostream& os, const Color& c )
       {
           os << "(" << (int)c.channels[R] << ","
                     << (int)c.channels[G] << ","
                     << (int)c.channels[B] << ","
                     << (int)c.channels[A] << ")";
           return os;
       }
   };

 // Paleta de cores
   static const Color BLACK         = Color{0,0,0, 255}      ; 
   static const Color WHITE         = Color{255,255,255, 255}; 
   static const Color DARK_GREEN    = Color{0,100,0, 255}    ; 
   static const Color GREEN         = Color{0,250,0, 255}    ; 
   static const Color RED           = Color{255,0,0, 255}    ; 
   static const Color BLUE          = Color{0,0,255, 255}    ; 
   static const Color CRIMSON       = Color{220,20,60, 255}  ; 
   static const Color LIGHT_BLUE    = Color{135,206,250, 255}; 
   static const Color LIGHT_GREY    = Color{210,210,210, 255}; 
   static const Color DEEP_SKY_BLUE = Color{0,191,255, 255}  ; 
   static const Color DODGER_BLUE   = Color{30,144,255, 255} ; 
   static const Color STEEL_BLUE    = Color{70,130,180, 255} ; 
   static const Color YELLOW        = Color{255,255,0, 255}  ; 
   static const Color LIGHT_YELLOW  = Color{255,255,153, 255}; 

   /// Paleta de cores
   static std::map< std::string, Color > color_pallet{
       {  "black",         BLACK },
       {  "white",         WHITE },
       {  "dark green",    DARK_GREEN },
       {  "red",           RED },
       {  "green",         GREEN },
       {  "blue",          BLUE },
       {  "crimson",       CRIMSON },
       {  "light blue",    LIGHT_BLUE },
       {  "light grey",    LIGHT_GREY },
       {  "deep sky blue", DEEP_SKY_BLUE },
       {  "dodger blue",   DODGER_BLUE },
       {  "steel blue",    STEEL_BLUE },
       {  "yellow",        YELLOW },
       {  "light yellow",  LIGHT_YELLOW }
   };

} // namespace

#endif // COMMON_H
