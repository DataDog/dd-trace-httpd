/* 
**  mod_ddtrace.c -- Apache sample ddtrace module
**  [Autogenerated via ``apxs -n ddtrace -g'']
**
**  To play with this sample module first compile it into a
**  DSO file and install it into Apache's modules directory 
**  by running:
**
**    $ apxs -c -i mod_ddtrace.c
**
**  Then activate it in Apache's apache2.conf file for instance
**  for the URL /ddtrace in as follows:
**
**    #   apache2.conf
**    LoadModule ddtrace_module modules/mod_ddtrace.so
**    <Location /ddtrace>
**    SetHandler ddtrace
**    </Location>
**
**  Then after restarting Apache via
**
**    $ apachectl restart
**
**  you immediately can request the URL /ddtrace and watch for the
**  output of this module. This can be achieved for instance via:
**
**    $ lynx -mime_header http://localhost/ddtrace 
**
**  The output should be similar to the following one:
**
**    HTTP/1.1 200 OK
**    Date: Tue, 31 Mar 1998 14:42:22 GMT
**    Server: Apache/1.3.4 (Unix)
**    Connection: close
**    Content-Type: text/html
**  
**    The sample page from mod_ddtrace.c
*/ 

#include "httpd.h"
#include "http_config.h"
#include "http_log.h"
#include "http_protocol.h"
#include "http_request.h"
#include "ap_config.h"
#include "apr_hooks.h"

#include "ddtrace.h"

APLOG_USE_MODULE(ddtrace);

static void ddtrace_mod_child_init(apr_pool_t *pool, server_rec *s) {
  ddtrace_create_tracer();
}

static int ddtrace_mod_begin_request(request_rec *r) {
  ap_log_error(APLOG_MARK, APLOG_ERR, 0, NULL, "ddtrace: request started: %pp %s %s %s", r, r->method, r->uri, r->protocol);
  ddtrace_start_span(r);
  return DECLINED;
}

static int ddtrace_mod_end_request(request_rec *r) {
  ap_log_error(APLOG_MARK, APLOG_ERR, 0, NULL, "ddtrace: request completed: %pp", r);
  ddtrace_finish_span(r);
  return DECLINED;
}

static void ddtrace_mod_register_hooks(apr_pool_t *p) {
  ap_log_error(APLOG_MARK, APLOG_ERR, 0, NULL, "ddtrace: module loaded");
  ap_hook_child_init(ddtrace_mod_child_init, NULL, NULL, APR_HOOK_REALLY_FIRST);
  ap_hook_post_read_request(ddtrace_mod_begin_request, NULL, NULL, APR_HOOK_REALLY_FIRST);
  ap_hook_log_transaction(ddtrace_mod_end_request,NULL,NULL,APR_HOOK_REALLY_FIRST);
}

/* Dispatch list for API hooks */
module AP_MODULE_DECLARE_DATA ddtrace_module = {
    STANDARD20_MODULE_STUFF, 
    NULL,                      /* create per-dir    config structures */
    NULL,                      /* merge  per-dir    config structures */
    NULL,                      /* create per-server config structures */
    NULL,                      /* merge  per-server config structures */
    NULL,                      /* table of config file commands       */
    ddtrace_mod_register_hooks  /* register hooks                      */
};
