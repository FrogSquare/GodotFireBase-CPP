/**
 * Copyright 2017 RameshRavone. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 **/

#include "utils.h"
#include "io/json.h"
#include "os/file_access.h"

NS_GODOT_BEGINE

int Utils::_script_id = -1;
Error Utils::parse_file_dict(const String p_filepath, Dictionary &p_dest) {

	Error err;
	String l_str_dest;
	Dictionary l_dest;

	err = open_file(p_filepath, l_str_dest);

	if (err) {
		ERR_FAIL_COND_V(err != OK, err);
	}

	String errs;
	int errl;

	err = JSON::parse(l_str_dest, l_dest, errs, errl);
	if (err != OK) {
		print_line(String(itos(errl) + ":" + errs));
		ERR_FAIL_COND_V(err != OK, err);
	}

	p_dest = l_dest;

	return OK;
}

Error Utils::open_file(const String p_filepath, String &p_dest) {
	Error err;
	FileAccess *file = FileAccess::open(p_filepath, FileAccess::READ, &err);

	if (err) {
		ERR_FAIL_COND_V(err != OK, err);
	}

	int length = file->get_len();
	ERR_FAIL_COND_V(length < 1, ERR_FILE_CORRUPT);

	char *data = memnew_arr(char, length + 1);
	file->get_buffer((uint8_t *)data, length);
	data[length] = 0;

	p_dest = data;
	memdelete(file);

	return OK;
}

NS_GODOT_END
