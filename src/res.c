#include <include/util.h>
#include <include/res.h>
#include <stdio.h>

hmap_t *status_codes;

void
status_codes_init()
{
	status_codes = hmap_new_cap(15);

	// 1xx - Informational
	hmap_push(status_codes, "100", (void*) "Continue");
	hmap_push(status_codes, "101", (void*) "Switching Protocols");

	// 2xx - Success
	hmap_push(status_codes, "200", (void*) "OK");
	hmap_push(status_codes, "201", (void*) "Created");
	hmap_push(status_codes, "202", (void*) "Accepted");
	hmap_push(status_codes, "203", (void*) "Non-Authoritative Information");
	hmap_push(status_codes, "204", (void*) "No Content");
	hmap_push(status_codes, "205", (void*) "Reset Content");
	hmap_push(status_codes, "206", (void*) "Partial Content");

	// 3xx - Redirection
	hmap_push(status_codes, "300", (void*) "Multiple Choices");
	hmap_push(status_codes, "301", (void*) "Moved Permanently");
	hmap_push(status_codes, "302", (void*) "Found");
	hmap_push(status_codes, "303", (void*) "See Other");
	hmap_push(status_codes, "304", (void*) "Not Modified");
	hmap_push(status_codes, "305", (void*) "Use Proxy");
	hmap_push(status_codes, "307", (void*) "Temporary Redirect");

	// 4xx - Client Error
	hmap_push(status_codes, "400", (void*) "Bad Request");
	hmap_push(status_codes, "401", (void*) "Unauthorized");
	hmap_push(status_codes, "402", (void*) "Payment Required");
	hmap_push(status_codes, "403", (void*) "Forbidden");
	hmap_push(status_codes, "404", (void*) "Not Found");
	hmap_push(status_codes, "405", (void*) "Method Not Allowed");
	hmap_push(status_codes, "406", (void*) "Not Acceptable");
	hmap_push(status_codes, "407", (void*) "Proxy Authentication Required");
	hmap_push(status_codes, "408", (void*) "Request Time-out");
	hmap_push(status_codes, "409", (void*) "Conflict");
	hmap_push(status_codes, "410", (void*) "Gone");
	hmap_push(status_codes, "411", (void*) "Length Required");
	hmap_push(status_codes, "412", (void*) "Precondition Failed");
	hmap_push(status_codes, "413", (void*) "Request Entity Too Large");
	hmap_push(status_codes, "414", (void*) "Request-URI Too Large");
	hmap_push(status_codes, "415", (void*) "Unsupported Media Type");
	hmap_push(status_codes, "416", (void*) "Requested range not satisfiable");
	hmap_push(status_codes, "417", (void*) "Expectation Failed");

	// 5xx - Server Error
	hmap_push(status_codes, "500", (void*) "Internal Server Error");
	hmap_push(status_codes, "501", (void*) "Not Implemented");
	hmap_push(status_codes, "502", (void*) "Bad Gateway");
	hmap_push(status_codes, "503", (void*) "Service Unavailable");
	hmap_push(status_codes, "504", (void*) "Gateway Time-out");
	hmap_push(status_codes, "505", (void*) "HTTP Version not supported");
}


void
res_send(res_t *res, char* body)
{
	if(res->sent) {
		print_error("Can not send response more than once for the same request\n");
		return;
	}

	res->sent = true;

	char status[4] = {0};
	sprintf(status, "%d", res->status);

	char response[1024] = {0};

	int j = 0;

	// Status Line
	j += sprintf(response + j, "HTTP/1.1 ");
	j += sprintf(response + j, "%d %s\r\n", res->status, hmap_get(status_codes, status));

	// Headers

	if(!hmap_get(res->headers, "Content-Type")) {
		hmap_push(res->headers, "Content-Type", "text/plain");
	}

	j += sprintf(response + j, "%s: %d\r\n", "Content-Length", strlen(body));
	hmap_itr_t *itr = hmap_itr_new(res->headers);
	hmap_node_t *node;
	while((node = hmap_itr_adv(itr)) != NULL) {
		j += sprintf(response + j, "%s: %s\r\n", node->key, (char*) node->value);
	}

	// Lone CRLF
	j += sprintf(response + j, "\r\n");

	// Body
	j += sprintf(response + j, "%s", body);

	// Sending
	write(res->client, response, sizeof(char) * (strlen(response) + 1));
}