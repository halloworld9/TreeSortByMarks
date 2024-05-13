#pragma once

enum request_method {
	get_req, post_req
};

request_method get_request_method();
void get_form_data(char*& result);
void get_param_value(const char* data, const char* param_name, char*& value);