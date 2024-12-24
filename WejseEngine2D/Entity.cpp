#include "Entity.h"


int Entity::nextid = 0;

Entity::Entity() : id(nextid++) {
	name =  "Entity " + std::to_string(id);
}
