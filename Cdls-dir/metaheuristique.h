

/**
   La métaheuristique est rnovlety
 */
int rnoveltyHyb(void (* fixe)());

/**
   Version hybride avec novelty   
 */
int noveltyHyb(void (* fixe)());

/**
   Base d'une approche hybride basé sur l'heuristique best
 */
int bestHyb(void (* fixe)());

/**
   Update statistics end flip
 */
void update_statistics_end_flip(void);

/**
   Approche hybride basé sur noveltyPlus
 */
int noveltyPlusHyb(void (* fixe)());


/**
   Approche hybride basé sur rnovelty+
 */
int rnoveltyPlusHyb(void (* fixe)());
