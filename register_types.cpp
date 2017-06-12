/** register_types.cpp **/

#include "register_types.h"
#include "object_type_db.h"
#include "globals.h"
#include "gd_firebase.h"

void register_gdfirebase_types() {
	//ObjectTypeDB::register_type<godot::GDFireBase>();
	Globals::get_singleton()->add_singleton(
	Globals::Singleton("GDFireBase", memnew(godot::GDFireBase)));
}

void unregister_gdfirebase_types() {
	//nothing to do here
}

