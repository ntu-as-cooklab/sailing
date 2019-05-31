#ifndef MODEL_HPP
#define MODEL_HPP

typedef enum DATASET {DATASET_ALL, DATASET_SIN, DATASET_AVG} DATASET;
typedef enum MODE {MODE_DRFT, MODE_WIND, MODE_DEST, MODE_DIRN, MODE_RAND} MODE;

typedef struct path_t path_t;

int sail_step(path_t* path);

#endif