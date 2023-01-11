#include <include/util.h>
#include <include/res.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>

hmap_t *status_codes;
hmap_t *mime_types;

void
status_codes_init(void)
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
mime_init(void)
{
	// Extension - Mime Type
	mime_types = hmap_new_cap(15);

	// Text
	hmap_push(mime_types, "txt", "text/plain");
	hmap_push(mime_types, "css", "text/css");
	hmap_push(mime_types, "csv", "text/csv");
	hmap_push(mime_types, "html", "text/html");
	hmap_push(mime_types, "js", "text/javascript");
	hmap_push(mime_types, "mjs", "text/javascript");

	// Font
	hmap_push(mime_types, "ttf", "font/ttf");
	hmap_push(mime_types, "woff", "font/woff");
	hmap_push(mime_types, "woff2", "font/woff2");

	// Audio
	hmap_push(mime_types, "aac", "audio/aac");
	hmap_push(mime_types, "mp3", "audio/mpeg");
	hmap_push(mime_types, "ogg", "audio/ogg");
	hmap_push(mime_types, "wav", "audio/wav");
	hmap_push(mime_types, "weba", "audio/webm");
	hmap_push(mime_types, "webm", "audio/webm");

	// Image
	hmap_push(mime_types, "gif", "image/gif");
	hmap_push(mime_types, "jpeg", "image/jpeg");
	hmap_push(mime_types, "jpg", "image/jpeg");
	hmap_push(mime_types, "png", "image/png");
	hmap_push(mime_types, "svg", "image/svg+xml");
	hmap_push(mime_types, "webp", "image/webp");

	// Application
	hmap_push(mime_types, "bz", "application/x-bzip");
	hmap_push(mime_types, "doc", "application/msword");
	hmap_push(mime_types, "docx", "application/vnd.openxmlformats-officedocument.wordprocessingml.document");
	hmap_push(mime_types, "epub", "application/epub+zip");
	hmap_push(mime_types, "gz", "application/gzip");
	hmap_push(mime_types, "json", "application/json");
	hmap_push(mime_types, "jsonld", "application/ld+json");
	hmap_push(mime_types, "pdf", "application/pdf");
	hmap_push(mime_types, "php", "application/x-httpd-php");
	hmap_push(mime_types, "ppt", "application/vnd.ms-powerpoint");
	hmap_push(mime_types, "pptx", "application/vnd.openxmlformats-officedocument.presentationml.presentation");
	hmap_push(mime_types, "sh", "application/x-sh");
	hmap_push(mime_types, "tar", "application/x-tar");
	hmap_push(mime_types, "xhtml", "application/xhtml+xml");
	hmap_push(mime_types, "xml", "application/xml");
	hmap_push(mime_types, "zip", "application/zip");

	// Video
	hmap_push(mime_types, "avi", "video/x-msvideo");
	hmap_push(mime_types, "mp4", "video/mpeg");
}


void
res_send(res_t *res)
{
	if(res->sent) {
		error("Can not send response more than once for the same request\n");
		return;
	}

	char status[4] = {0}; // 3 digit status code
	sprintf(status, "%d", res->status);

	char response[1024] = {0};

	int j = 0;

	// Status Line
	j += sprintf(response + j, "HTTP/1.1 "); // Supports only HTTP/1.1 as of now

	char *status_str = hmap_get(status_codes, status);
	if(!status_str) {
		error("Please use a proper status code.");
	}
	j += sprintf(response + j, "%d %s\r\n", res->status, status_str);

	// Headers

	if(!hmap_get(res->headers, "Content-Type")) {
		hmap_push(res->headers, "Content-Type", "text/plain"); // Default value
	}

	// TODO: A node deletion helper function in hmap
	j += sprintf(response + j, "%s: %d\r\n", "Content-Length", strlen(res->body));

	hmap_itr_t *itr = hmap_itr_new(res->headers);
	hmap_node_t *node;
	while((node = hmap_itr_adv(itr)) != NULL) {
		j += sprintf(response + j, "%s: %s\r\n", node->key, (char*) node->value);
	}

	// Lone CRLF
	j += sprintf(response + j, "\r\n");

	// Body
	j += sprintf(response + j, "%s", res->body);

	// Sending
	write(res->client, response, sizeof(char) * (strlen(response) + 1));

	res->sent = true;
}

void
res_send_file(res_t *res, const char *file_path)
{
	char *path = strdup(file_path);

	// Please provide the actual path using __BASE__ like civilized humans.
	// I don't want pain with this commented out code for casees
	// when it doesn't exist.
	// char *file_path = realpath(path, NULL);
	// if(!file_path) {
	// 	error("Couldn't get file");
	// }

	int fd = open(path, O_RDONLY);
	struct stat sb;
	if(!fd || fstat(fd, &sb) == -1) {
		res->status = 404;
		set_res_body(res, "File not found.");
		res_send(res);
		close(fd);
		return;
	}

	char *file = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	// Guessing mime types from file extensions. 'file' command does a very good job
	// at guessing them especially as it guesses css as 'text/plain' and not 'text/css' :')

	char* path_copy = path; //! Malloc gives error on modifying the pointer itself
	char* ext;
	while(1) {
		char *token = strtok_r(path_copy, ".", &path_copy);
		if(token) {
			ext = token;
		} else {
			break;
		}
	}

	char *mime = hmap_get(mime_types, ext);

	set_res_header(res, "Content-Type", mime);
	set_res_body(res, file);
	res_send(res);

	munmap(file, sb.st_size); // TODO: Implement status code 304
	free(path);

	close(fd);
}

/* Utility */

void
set_res_body(res_t* res, char* body)
{
	if(!body) {
		return;
	}

	res->body = body;
}

void
set_res_status(res_t* res, u_int16_t status)
{
	if(status < 100 || status >= 600) {
		error("Incorrect Status Code");
	}

	printf("%d\n", status);
	char status_str[4] = {0};
	sprintf(status_str, "%d", status);

	char *exists = hmap_get(status_codes, status_str);
	if(!exists) {
		error("Incorrect Status Code");
	}

	res->status = status;
}

void
set_res_header(res_t* res, const char* header, char* value)
{
	hmap_push(res->headers, header, value);
}

char*
get_res_header(res_t* res, const char* header)
{
	return hmap_get(res->headers, header);
}