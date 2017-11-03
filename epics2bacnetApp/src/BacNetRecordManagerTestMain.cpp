/*
 * BacNetRecordManagerTestMain.cpp
 *
 *  Created on: Nov 7, 2011
 *      Author: 8w4
 */

/** System*/
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

/** Local*/
#include "BacNetRecordManager.h"
#include "BacNetVariable.h"
#include "BacNetVariableDouble.h"

int main(int argc, char *argv[]) {


	/** Instantiate a record-set...*/
	BacNetRecordManager *rec = new BacNetRecordManager();
	BacNetVariable *var[255];
	uint8_t id = 0;

	for(int i=0; i<255; i++){
		var[i] = new BacNetVariableDouble(0, i, 85, -1, 0, NULL);
		id = rec->register_variable(var[i], BACNET_RECORD::READ);
		//if(id > 0){printf("\nRegistered var_%d(%p) to id(%d)\n", i, var[i], id);}
		//else {printf("\nCould not register var_%d(%p) to id(%d)\n", i, var[i], id);}
	}

	for(int i=0; i<255; i++){
		assert(rec->get_id_byIndex(i) == (i+1));
	}

	/** Whatever the invoke-id is, should be its position in the table minus 1...*/
	assert(var[4]->get_variable_que()->invoke_id == rec->get_id_byIndex(var[4]->get_variable_que()->invoke_id - 1));

	/** List is full; try to register one more; should return 0...*/
	BacNetVariable *var_255 = new BacNetVariableDouble(0, 255, 85, -1, 0, NULL);
	id = rec->register_variable(var_255, BACNET_RECORD::READ);
	assert(id == 0);

	/** Now release one of the variables in the list*/
	rec->release_variable(var[2]);
	assert(var[2]->get_variable_que() == NULL);

	/** Try to register var_255 again...*/
	id = rec->register_variable(var_255, BACNET_RECORD::READ);

	assert(id == 3);

	//BACNET_RECORD *ptr = var[0]->get_variable_que();

	printf("\nAll is well (i.e It works!)...\n");

	rec->report();
	return 0;
}
