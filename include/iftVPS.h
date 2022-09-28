#ifndef _iftVPS_H_
#define _iftVPS_H_

#include "iftCommon.h"
#include "iftSort.h"

/* Fitness function used for optimization problems */
typedef float (*iftVPSFitnessFunc)   (void *problem, uchar *theta); 

/* Data structures for the varsity player selection algorithm --- A
   new feature selection algorithm */

typedef struct ift_team {
  uchar  eliminated;/* indicates when there is anoother team with the
			same players in order to avoid reevaluation (0/1) */ 
  uchar *player;     /* binary vector which indicates the presence
		       (player[i]=1) or absence (player[i]=0) of a player
		       (feature) i in the team (feature vector) */
  float  fitness;   /* the fitness value of the team used for ranking it */
} iftTeam;

typedef struct ift_VPS {
  iftVPSFitnessFunc iftFitness;   /* fitness function for optimization. */
  void    *problem;            /* the context of the problem */
  iftTeam *team;               /* list of teams considered for tryouts */
  int      nteams;             /* number of teams  */
  float   *Ps;                 /* probability of each player to be
				  selected for a team */
  int      nplayers;           /* number of players (features) */
  int      nrounds;            /* maximum number of tryout rounds (iterations)*/
  iftTeam  best_team;          /* the best team */
} iftVPS;     

iftVPS         *iftCreateVPS(int nplayers, int nteams, iftVPSFitnessFunc f, void *prob);
void            iftDestroyVPS(iftVPS **vps);
float           iftVPSMax(iftVPS *vps);
float           iftVPSMin(iftVPS *vps);

#endif
