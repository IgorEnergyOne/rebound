#include "rebound.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAXSHELLS 100

extern unsigned long rebd_drift[MAXSHELLS];
extern unsigned long rebd_viol1[MAXSHELLS];
extern unsigned long rebd_viol2[MAXSHELLS];
double E0;

void heartbeat(struct reb_simulation* r){
    for(int s=0;s<r->ri_mercurana.Nmaxshells;s++){
        printf("%2d ", s);
        for (int ptype=0; ptype<5; ptype++){
            if (r->ri_mercurana.pisd[ptype].shellN){
                printf("pt(%d)=%04d ", ptype, r->ri_mercurana.pisd[ptype].shellN[s]);
            }
        }
        //printf("%12lu ", rebd_drift[i]);
        //printf("%12lu ", rebd_viol1[i]);
        //printf("%12lu ", rebd_viol2[i]);
        printf("\n");
    }
    printf("-------------\n");
    printf("maxshells %d\n", r->ri_mercurana.Nmaxshellsused);
    double E1 = reb_tools_energy(r);
    printf("dE/E= %e (offset=%e)  N= %d  N_active= %d  moved= %d\n",fabs((E0-E1)/E0), r->energy_offset, r->N, r->N_active, r->ri_mercurana.Nmoved);
}

int main(int argc, char* argv[]) {
    struct reb_simulation* r = reb_create_simulation();
    srand(4);
    r->exact_finish_time = 0;
    r->dt = 0.40;
    r->heartbeat = heartbeat;
    r->testparticle_type = 1;
    r->integrator = REB_INTEGRATOR_MERCURANA;
    r->ri_mercurana.kappa = 1e-4;
    r->ri_mercurana.N_dominant = 1;
    r->ri_mercurana.Nmaxshells = 30;
    r->collision = REB_COLLISION_DIRECT;
    r->collision_resolve = reb_collision_resolve_merge;
    r->track_energy_offset = 1;

    if (1){
        struct reb_particle p1 = {0}; 
        p1.m = 1.;
        p1.r = 0.0046524726;
        reb_add(r, p1);  
        
        struct reb_particle p2 = {0};
        p2.x = 1;
        p2.vy = 1;
        p2.m = 1e-3;
        p2.r = 0.0046732617;
        reb_add(r, p2); 
        
        struct reb_particle p3 = {0};
        p3.x = 2;
        p3.vy = 0.74;
        p3.m = 1e-3;
        p3.r = 0.0046732617;
        reb_add(r, p3); 

        for (int i=0; i<200;i++){
            double a = reb_random_uniform(0.8,1.2);
            double omega = reb_random_uniform(0.,M_PI*2);
            double f = reb_random_uniform(0.,M_PI*2.);
            struct reb_particle p = reb_tools_orbit2d_to_particle(1.,p1,1e-6,a,0.1,omega,f);
            p.r = 0.000046;
            reb_add(r,p);
        }
        
        r->N_active = r->N;

        for (int i=0; i<0;i++){
            double a = reb_random_uniform(0.8,1.2);
            //double a = reb_random_uniform(2.4,3.2);
            double omega = reb_random_uniform(0.,M_PI*2);
            double f = reb_random_uniform(0.,M_PI*2.);
            double m = r->testparticle_type==0?0:1e-6;
            struct reb_particle p = reb_tools_orbit2d_to_particle(1.,p1,m,a,0.1,omega,f);
            p.r = 0.000046;
            reb_add(r,p);
        }

        reb_move_to_com(r);
    }

    E0 = reb_tools_energy(r);
    reb_integrate(r,10000.);
    double E1 = reb_tools_energy(r);
    printf("dE/E = %e\n",fabs((E0-E1)/E0));
}

