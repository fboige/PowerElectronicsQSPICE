// Automatically generated C++ file on Sat Mar 16 21:14:30 2024
//
// To build with Digital Mars C++ Compiler:
//
//    dmc -mn -WD pmsm.cpp kernel32.lib
#include <cmath>

union uData
{
   bool b;
   char c;
   unsigned char uc;
   short s;
   unsigned short us;
   int i;
   unsigned int ui;
   float f;
   double d;
   long long int i64;
   unsigned long long int ui64;
   char *str;
   unsigned char *bytes;
};

static double timecheck = 0.0;

void abcToDq(double A, double B, double C, double theta_e, double& D, double& Q);
void dqToAbc(double D, double Q, double theta_e, double& A, double& B, double& C);
void modelPMSM(double Vd, double Vq, double Ld, double Lq, double flux_linkage, double Rs, double omega_e, double dt, double& Id, double& Iq);

// int DllMain() must exist and return 1 for a process to load the .DLL
// See https://docs.microsoft.com/en-us/windows/win32/dlls/dllmain for more information.
int __stdcall DllMain(void *module, unsigned int reason, void *reserved) { return 1; }

// #undef pin names lest they collide with names in any header file(s) you might include.
#undef Ia
#undef Ib
#undef Ic
#undef VaSense
#undef VbSense
#undef VcSense
#undef theta_e
#undef Torque
#undef omega_e

extern "C" __declspec(dllexport) void pmsm(void **opaque, double t, union uData *data)
{
   double  VaSense     = data[ 0].d; // input
   double  VbSense     = data[ 1].d; // input
   double  VcSense     = data[ 2].d; // input
   double  theta_e     = data[ 3].d; // input
   double  omega_e     = data[ 4].d; // input
   double  Ld          = data[ 5].d; // input parameter
   double  Lq          = data[ 6].d; // input parameter
   double  fluxLinkage = data[ 7].d; // input parameter
   double  Rs          = data[ 8].d; // input parameter
   double &Ia          = data[9].d; // output
   double &Ib          = data[10].d; // output
   double &Ic          = data[11].d; // output
   double &Torque      = data[12].d; // output

// Implement module evaluation code here:
   double dt = t-timecheck;
   double Vd, Vq;
   double Id, Iq;

   if ( dt >= 20.e-6){
      timecheck = t;
      abcToDq(VaSense, VbSense, VcSense, theta_e, Vd, Vq);
      modelPMSM(Vd, Vq, Ld, Lq, fluxLinkage, Rs, omega_e, dt, Id, Iq);
      Torque = 3/2*(fluxLinkage*Iq+(Ld-Lq)*Iq*Id);
      dqToAbc(Id, Iq, theta_e, Ia, Ib, Ic);
   };
}
//FUNCTIONS
void abcToDq(double A, double B, double C, double theta_e, double& D, double& Q)
{
   D = sqrt(2.0/3.0)*( cos(theta_e)*A + cos(theta_e - 2.*M_PI/3.)*B + cos(theta_e + 2.*M_PI/3.)*C);
   Q = sqrt(2.0/3.0)*(-sin(theta_e)*A - sin(theta_e - 2.*M_PI/3.)*B - sin(theta_e + 2.*M_PI/3.)*C);
}

void dqToAbc(double D, double Q, double theta_e, double& A, double& B, double& C)
{
   A = cos(theta_e)*D - sin(theta_e)*Q;
   B = cos(theta_e-2.*M_PI/3.)*D - sin(theta_e-2.*M_PI/3.)*Q;
   C = cos(theta_e-4.*M_PI/3.)*D - sin(theta_e-4.*M_PI/3.)*Q;
};

void modelPMSM(double Vd, double Vq, double Ld, double Lq, double flux_linkage, double Rs, double omega_e, double dt, double& Id, double& Iq)
{
    // This code snippet demonstrates a basic Euler integration approach to update Id and Iq over time.
    //A true Newton-Raphson method would involve setting up and solving the Jacobian matrix of the system,
    //which is more complex and beyond this simplified example.

    double dId_dt = (Vd + omega_e * Lq * Iq - Rs * Id) / Ld;
    double dIq_dt = (Vq - omega_e * Ld * Id - omega_e * flux_linkage - Rs * Iq) / Lq;

    // Update currents using Euler method
    Id += dId_dt * dt;
    Iq += dIq_dt * dt;
};
