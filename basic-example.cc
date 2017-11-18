#include "Base.h"
#include <math.h>
// On va stocker les imagettes-prototypes au sein d'une grille.
#define WIDTH  5
#define HEIGHT 4
typedef uci::Map<WIDTH,HEIGHT,
		 uci::Database::imagette::width,
		 uci::Database::imagette::height> Prototypes;

// Pour une imagette, les indices sont definis comme suit
//
//         j
//         |
//   ......|.....
//   ......|.....
//   ......|.....
//   ......|.....
//   ......|.....
//   ......#------ i   img(i,j)
//   ............
//   ............
//   ............
//   ............
//   ............  


// Cette fonction permet d'affecter un prototype (dont les pixels sont
// des double dans [0,255]) a une imagette tiree de la base (dont les
// pixels sont des unsigned char). Le & evite les copies inutiles.
void initProto(Prototypes::imagette& w,
	       const uci::Database::imagette& xi) {
  for(int i = 0 ; i < uci::Database::imagette::height ; ++i)
    for(int j = 0 ; j < uci::Database::imagette::width ; ++j)
      w(i,j) = (double)(xi(i,j));
}

 void learnProto(double alpha, Prototypes::imagette& w, const uci::Database::imagette& xi)     {
	 for(int i=0;i<uci::Database::imagette::height;i++){
		for(int j=0; j<uci::Database::imagette::width;j++){
			w(i,j)=w(i,j)+alpha*((double)xi(i,j)-w(i,j));
		}
	}
	}
	
double distanceProto(const Prototypes::imagette& w, const uci::Database::imagette& xi)        {
	double distance(0.0);
	for(int i=0;i<uci::Database::imagette::height;i++){
		for(int j=0; j<uci::Database::imagette::width;j++){
			distance += (w(i,j)-(double)xi(i,j))*(w(i,j)-(double)xi(i,j));
		}
	}
	return distance=sqrt(distance);
	}
	
void winnerProto(const Prototypes& protos, const uci::Database::imagette& xi, int& i, int& j) {
	 //change the position of the winner 
	 double distance(0);
	 double distanceWinner(0);
	 for(int m=0;m<HEIGHT;m++){
	  for(int n=0;n<WIDTH;n++){
		  distance=distanceProto(protos(m,n),xi);
		  distanceWinner=distanceProto(protos(i,j),xi);
		  if(distance<distanceWinner){i=m;j=n;}		  
	  }
  }
}

/*double winningRate(int i_winner, int j_winner, int i, int j) {
	
	}*/

int main(int argc, char* argv[]) {
  Prototypes prototypes;
  
  // Utilisons la base de donnees.
  uci::Database database;
  
  
 //first fill the prototype with 20 images (two groups of number from 0-9)
  for(int m=0;m<HEIGHT;m++){
	  for(int n=0;n<WIDTH;n++){
		  database.Next();
		  uci::Database::imagette& xi = database.input;
		  initProto(prototypes(m,n),xi);
	  }
  }
  
 // On met une valeur de alpha
 double alpha(0.6);
 
 // On peut afficher la grille de prototypes.
  prototypes.PPM("proto_initial",1);
  
 //optimize the prototype
 for(int k=2;k<200;k++){
	 database.Next();
	 uci::Database::imagette& x = database.input;
	 int indexWinnerx(0);
	 int indexWinnery(0);
	 winnerProto(prototypes,x,indexWinnerx,indexWinnery);
	 learnProto(alpha,prototypes(indexWinnerx,indexWinnery),x);
	 // prototypes.PPM("proto_initial",k);
	 }
	 prototypes.PPM("protofinal",2);
  
  //double winningRate(int i_winner, int j_winner, int i, int j) {...}

  


  return 0;
}
