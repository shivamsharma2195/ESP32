/*
  ===========================================================================================
  Example to demonstrate multi-variable plotting against time
  -------------------------------------------------------------------------------------------
  Plotter
  by Shivam Sharma
  ===========================================================================================
*/

#include "Plotter.h"

// Plotted variables must be declared as globals 
double v;
double w;
double x;
double y;
double z;

// Also declare plotter as global
Plotter p;

void setup()
{
    // Start plotter
    p.Begin();
  
    // Add 5 variable time graph
    p.AddTimeGraph( "5 variable time graph", 1000, "v label", v, "w label", w, "x label", x,
		    "y label", y, "z label", z );
}

void loop()
{
    // Update variables with arbitrary sine/cosine data
//    v = 3*cos( 2.0*PI*( millis()/2500.0 ) );
//    w = 4.0;
//    x = 10*sin( 2.0*PI*( millis() / 5000.0 ) );
//    y = 7*cos( 2.0*PI*( millis() / 5000.0 ) );
//    z = 5*sin( 2.0*PI*( millis() / 5000.0 ) );


    v = 25;
    w = 20;
    x = 15;
    y = 10;
    z = 5;
    // Plot
    p.Plot();
}
