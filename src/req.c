#include <include/req.h>
#include <include/util.h>
#include <stdio.h>

req_t*
parse_req(char *req_str, int size)
{
	if(!req_str) {
		return NULL;
	}

	int n = size, k;

	req_t *req = malloc(sizeof(req_t));
	req->body = NULL;
	req->headers = NULL;
	req->method = NULL;
	req->path = NULL;
	req->queries = NULL;
	req->params = NULL;
	req->c_queue = queue_new();

	char temp[MAX_SIZE] = {0};

	int index = 0, start;

	// Get method
	start = index;
	for(;index < n && req_str[index] != ' '; index++) {
		temp[index - start] = req_str[index];
	}
	temp[index - start] = '\0';
	k = strlen(temp);
	req->method = strdup(temp);
	memset(temp, 0, k + 1);

	index++;

	// Get path
	start = index;
	for(;index < n && !(req_str[index] == ' ' || req_str[index] == '?'); index++) {
		temp[index - start] = req_str[index];
	}
	temp[index - start] = '\0';
	k = strlen(temp);
	req->path = strdup(temp);
	memset(temp, 0, k + 1);

	// Get Queries
	char *key, *value;
	if(req_str[index] == '?') {
		req->queries = hmap_new_cap(5);
		index++;

		for(;index < n && req_str[index] != ' '; index++) {

			// Key
			start = index;
			for(;index < n && req_str[index] != '='; index++) {
				temp[index - start] = req_str[index];
			}
			temp[index] = '\0';
			k = strlen(temp);
			key = strdup(temp);
			memset(temp, 0, k + 1);

			index++;

			// Value
			start = index;
			for(;index < n && !(req_str[index] == '&' || req_str[index] == ' '); index++) {
				temp[index - start] = req_str[index];
			}
			temp[index] = '\0';
			k = strlen(temp);
			value = strdup(temp);
			memset(temp, 0, k + 1);

			hmap_push(req->queries, key, (void*) value);

			index--;
		}
	}

	// Get Headers
	req->headers = hmap_new_cap(5);
	for(;index < n && req_str[index] != '\n'; index++);
	index++; // Reaching Header Line

	for(;index + 2 < n && !(req_str[index] == '\n' && req_str[index + 1] == '\r' && req_str[index + 2] == '\n'); index++) {
		if(req_str[index] == '\n') {
			index++;
		}

		// Key
		start = index;
		for(;index < n && req_str[index] != ':'; index++) {
			temp[index - start] = req_str[index];
		}
		temp[index - start] = '\0';
		k = strlen(temp);
		key = strdup(temp);
		memset(temp, 0, k + 1);

		index+=2; // For ": "

		// Value
		start = index;
		for(;index < n && req_str[index] != '\r'; index++) {
			temp[index - start] = req_str[index];
		}
		temp[index - start] = '\0';
		k = strlen(temp);
		value = strdup(value);
		memset(temp, 0, k + 1);

		hmap_push(req->headers, key, (void*) value);
	}

	// Escaping lone CRLF
	index+=2;

	// Body
	index++;

	start = index;
	for(;index < n; index++) {
		temp[index - start] = req_str[index];
	}
	temp[index - start] = '\0';
	req->body = strdup(temp);

	return req;
}

/* Utility */

char*
get_req_body(req_t* req)
{
	return req->body;
}

char*
get_req_header(req_t* req, const char* header)
{
	return hmap_get(req->headers, header);
}

char*
get_req_method(req_t* req)
{
	return req->method;
}

char*
get_req_param(req_t* req, const char* param)
{
	return hmap_get(req->params, param);
}

char*
get_req_path(req_t* req)
{
	return req->path;
}

char*
get_req_query(req_t* req, const char* query)
{
	return hmap_get(req->queries, query);
}