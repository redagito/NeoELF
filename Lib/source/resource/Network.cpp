#include "nelf/resource/Network.h"

#ifdef USE_SERF
#include <apr.h>
#include <apr_atomic.h>
#include <apr_base64.h>
#include <apr_general.h>
#include <apr_getopt.h>
#include <apr_strings.h>
#include <apr_thread_proc.h>
#include <apr_uri.h>
#include <apr_version.h>

#include "serf.h"
#endif

#ifdef USE_SERF
apr_pool_t *pool;
#endif

void elfInitNetworking()
{
#ifdef USE_SERF
    apr_initialize();
    atexit(apr_terminate);

    apr_pool_create(&pool, NULL);
#endif
}

void elfDeinitNetworking()
{
#ifdef USE_SERF
    apr_pool_destroy(pool);
#endif
}

#ifdef USE_SERF
typedef struct
{
    int using_ssl;
    serf_ssl_context_t *ssl_ctx;
    serf_bucket_alloc_t *bkt_alloc;
} app_baton_t;

static void closed_connection(serf_connection_t *conn, void *closed_baton, apr_status_t why, apr_pool_t *pool)
{
    if (why)
    {
        abort();
    }
}

static apr_status_t ignore_all_cert_errors(void *data, int failures, const serf_ssl_certificate_t *cert)
{
    /* In a real application, you would normally would not want to do this */
    return APR_SUCCESS;
}

static apr_status_t conn_setup(apr_socket_t *skt, serf_bucket_t **input_bkt, serf_bucket_t **output_bkt,
                               void *setup_baton, apr_pool_t *pool)
{
    serf_bucket_t *c;
    app_baton_t *ctx = setup_baton;

    c = serf_bucket_socket_create(skt, ctx->bkt_alloc);
    if (ctx->using_ssl)
    {
        c = serf_bucket_ssl_decrypt_create(c, ctx->ssl_ctx, ctx->bkt_alloc);
        if (!ctx->ssl_ctx)
        {
            ctx->ssl_ctx = serf_bucket_ssl_decrypt_context_get(c);
        }
        serf_ssl_server_cert_callback_set(ctx->ssl_ctx, ignore_all_cert_errors, NULL);

        *output_bkt = serf_bucket_ssl_encrypt_create(*output_bkt, ctx->ssl_ctx, ctx->bkt_alloc);
    }

    *input_bkt = c;

    return APR_SUCCESS;
}

static serf_bucket_t *accept_response(serf_request_t *request, serf_bucket_t *stream, void *acceptor_baton,
                                      apr_pool_t *pool)
{
    serf_bucket_t *c;
    serf_bucket_alloc_t *bkt_alloc;

    /* get the per-request bucket allocator */
    bkt_alloc = serf_request_get_alloc(request);

    /* Create a barrier so the response doesn't eat us! */
    c = serf_bucket_barrier_create(stream, bkt_alloc);

    return serf_bucket_response_create(c, bkt_alloc);
}

typedef struct
{
#if APR_MAJOR_VERSION > 0
    apr_uint32_t completed_requests;
#else
    apr_atomic_t completed_requests;
#endif
    int print_headers;

    serf_response_acceptor_t acceptor;
    app_baton_t *acceptor_baton;

    serf_response_handler_t handler;

    const char *host;
    const char *method;
    const char *path;
    const char *req_body_path;
    const char *authn;
} handler_baton_t;

/* Kludges for APR 0.9 support. */
#if APR_MAJOR_VERSION == 0
#define apr_atomic_inc32 apr_atomic_inc
#define apr_atomic_dec32 apr_atomic_dec
#define apr_atomic_read32 apr_atomic_read
#endif

char url_copy[256];
char method_copy[256];
int thread_state = 0;

static apr_status_t handle_response(serf_request_t *request, serf_bucket_t *response, void *handler_baton,
                                    apr_pool_t *pool)
{
    const char *data;
    apr_size_t len;
    serf_status_line sl;
    apr_status_t status;
    handler_baton_t *ctx = handler_baton;
    FILE *file;

    if (!response)
    {
        /* A NULL response can come back if the request failed completely */
        return APR_EGENERAL;
    }
    status = serf_bucket_response_status(response, &sl);
    if (status)
    {
        return status;
    }

    file = fopen("tmp-response.txt", "w");
    if (file)
        fclose(file);

    file = fopen("tmp-headers.txt", "w");
    if (file)
        fclose(file);

    while (1)
    {
        status = serf_bucket_read(response, 2048, &data, &len);
        if (SERF_BUCKET_READ_ERROR(status))
            return status;

        file = fopen("tmp-response.txt", "ab");
        if (file)
        {
            fwrite(data, 1, len, file);
            fclose(file);
        }
        /* are we done yet? */
        if (APR_STATUS_IS_EOF(status))
        {
            if (ctx->print_headers)
            {
                serf_bucket_t *hdrs;
                hdrs = serf_bucket_response_get_headers(response);
                while (1)
                {
                    status = serf_bucket_read(hdrs, 2048, &data, &len);
                    if (SERF_BUCKET_READ_ERROR(status))
                        return status;

                    file = fopen("tmp-headers.txt", "ab");
                    if (file)
                    {
                        fwrite(data, 1, len, file);
                        fclose(file);
                    }
                    if (APR_STATUS_IS_EOF(status))
                    {
                        break;
                    }
                }
            }

            apr_atomic_inc32(&ctx->completed_requests);
            return APR_EOF;
        }

        /* have we drained the response so far? */
        if (APR_STATUS_IS_EAGAIN(status))
            return status;

        /* loop to read some more. */
    }
    /* NOTREACHED */
}

static apr_status_t setup_request(serf_request_t *request, void *setup_baton, serf_bucket_t **req_bkt,
                                  serf_response_acceptor_t *acceptor, void **acceptor_baton,
                                  serf_response_handler_t *handler, void **handler_baton, apr_pool_t *pool)
{
    handler_baton_t *ctx = setup_baton;
    serf_bucket_t *hdrs_bkt;
    serf_bucket_t *body_bkt;

    if (ctx->req_body_path)
    {
        apr_file_t *file;
        apr_status_t status;

        status = apr_file_open(&file, ctx->req_body_path, APR_READ, APR_OS_DEFAULT, pool);

        if (status)
        {
            printf("Error opening file (%s)\n", ctx->req_body_path);
            return status;
        }

        body_bkt = serf_bucket_file_create(file, serf_request_get_alloc(request));
    }
    else
    {
        body_bkt = nullptr;
    }

    *req_bkt =
        serf_request_bucket_request_create(request, ctx->method, ctx->path, body_bkt, serf_request_get_alloc(request));

    hdrs_bkt = serf_bucket_request_get_headers(*req_bkt);

    serf_bucket_headers_setn(hdrs_bkt, "User-Agent", "cblendlef 0.1 Serf/" SERF_VERSION_STRING);
    /* Shouldn't serf do this for us? */
    serf_bucket_headers_setn(hdrs_bkt, "Accept-Encoding", "gzip");

    if (ctx->authn != NULL)
    {
        serf_bucket_headers_setn(hdrs_bkt, "Authorization", ctx->authn);
    }

    *acceptor = ctx->acceptor;
    *acceptor_baton = ctx->acceptor_baton;
    *handler = ctx->handler;
    *handler_baton = ctx;

    return APR_SUCCESS;
}

char url_copy[256];
char method_copy[256];

/**
 * Thread entry point
 */
static void *APR_THREAD_FUNC making_the_request(apr_thread_t *thd, void *data)
{
    elfRequest *req = (elfRequest *)data;
    apr_status_t status;
    serf_context_t *context;
    serf_connection_t *connection;
    serf_request_t *request;
    app_baton_t app_ctx;
    handler_baton_t handler_ctx;
    apr_uri_t url;
    const char *proxy = nullptr;
    const char *raw_url, *method, *req_body_path = nullptr;
    int count;
    int i;
    int print_headers;
    char *authn = nullptr;
    FILE *file;

    /* Default to one round of fetching. */
    count = 1;
    /* Default to GET. */
    method = method_copy;
    /* Do not print headers by default. */
    print_headers = 1;

    raw_url = req->url;
    raw_url = url_copy;

    file = fopen("tmp-post-data.txt", "r");
    if (file)
    {
        req_body_path = "tmp-post-data.txt";
        fclose(file);
    }

    apr_uri_parse(pool, raw_url, &url);
    if (!url.port)
    {
        url.port = apr_uri_port_of_scheme(url.scheme);
    }
    if (!url.path)
    {
        url.path = "/";
    }

    if (strcasecmp(url.scheme, "https") == 0)
    {
        app_ctx.using_ssl = 1;
    }
    else
    {
        app_ctx.using_ssl = 0;
    }

    context = serf_context_create(pool);

    if (proxy)
    {
        apr_sockaddr_t *proxy_address = nullptr;
        apr_port_t proxy_port;
        char *proxy_host;
        char *proxy_scope;

        status = apr_parse_addr_port(&proxy_host, &proxy_scope, &proxy_port, proxy, pool);
        if (status)
        {
            printf("Cannot parse proxy hostname/port: %d\n", status);
            return NULL;
        }

        if (!proxy_host)
        {
            printf("Proxy hostname must be specified\n");
            return NULL;
        }

        if (!proxy_port)
        {
            printf("Proxy port must be specified\n");
            return NULL;
        }

        status = apr_sockaddr_info_get(&proxy_address, proxy_host, APR_UNSPEC, proxy_port, 0, pool);

        if (status)
        {
            printf("Cannot resolve proxy address '%s': %d\n", proxy_host, status);
            return NULL;
        }

        serf_config_proxy(context, proxy_address);
    }

    /* ### Connection or Context should have an allocator? */
    app_ctx.bkt_alloc = serf_bucket_allocator_create(pool, NULL, NULL);
    app_ctx.ssl_ctx = nullptr;

    status =
        serf_connection_create2(&connection, context, url, conn_setup, &app_ctx, closed_connection, &app_ctx, pool);
    if (status)
    {
        printf("Error creating connection: %d\n", status);
        return NULL;
    }

    handler_ctx.completed_requests = 0;
    handler_ctx.print_headers = print_headers;

    handler_ctx.host = url.hostinfo;
    handler_ctx.method = method;
    handler_ctx.path = url.path;
    handler_ctx.authn = authn;

    handler_ctx.req_body_path = req_body_path;

    handler_ctx.acceptor = accept_response;
    handler_ctx.acceptor_baton = &app_ctx;
    handler_ctx.handler = handle_response;

    for (i = 0; i < count; i++)
    {
        request = serf_connection_request_create(connection, setup_request, &handler_ctx);
    }

    while (1)
    {
        status = serf_context_run(context, SERF_DURATION_FOREVER, pool);
        if (APR_STATUS_IS_TIMEUP(status))
            continue;
        if (status)
        {
            char buf[200];
            printf("Error running context: (%d) %s\n", status, apr_strerror(status, buf, sizeof(buf)));
            return NULL;
        }
        if (apr_atomic_read32(&handler_ctx.completed_requests) >= count)
        {
            break;
        }
        /* Debugging purposes only! */
        serf_debug__closed_conn(app_ctx.bkt_alloc);
    }

    serf_connection_close(connection);
    thread_state = 0;
    apr_thread_exit(thd, APR_SUCCESS);
    return NULL;
}
#endif

elfRequest *elfCreateRequest(const char *name)
{
#ifdef USE_SERF
    elfRequest *request;

    request = (elfRequest *)malloc(sizeof(elfRequest));
    memset(request, 0x0, sizeof(elfRequest));

    request->objType = ELF_REQUEST;
    request->objDestr = elfDestroyRequest;

    if (name)
        request->name = elfCreateString(name);

    request->url = nullptr;
    request->method = nullptr;

    elfIncObj(ELF_REQUEST);

    return request;
#else
    return nullptr;
#endif
}

void elfDestroyRequest(void *data)
{
#ifdef USE_SERF
    elfRequest *request = (elfRequest *)data;

    if (request->name)
        elfDestroyString(request->name);
    if (request->url)
        elfDestroyString(request->url);
    if (request->method)
        elfDestroyString(request->method);

    free(request);

    elfDecObj(ELF_REQUEST);
#endif
}

const char *elfGetRequestUrl(elfRequest *request)
{
#ifdef USE_SERF
    return request->url;
#else
    return nullptr;
#endif
}

void elfSetRequestUrl(elfRequest *request, const char *url)
{
#ifdef USE_SERF
    if (request->url)
        elfDestroyString(request->url);
    request->url = elfCreateString(url);
#endif
}

const char *elfGetRequestMethod(elfRequest *request)
{
#ifdef USE_SERF
    return request->method;
#else
    return nullptr;
#endif
}

void elfSetRequestMethod(elfRequest *request, const char *method)
{
#ifdef USE_SERF
    if (request->method)
        elfDestroyString(request->method);
    request->method = elfCreateString(method);
#endif
}

int elfGetRequestState()
{
#ifdef USE_SERF
    return thread_state;
#else
    return -1;
#endif
}

void elfSendRequest(elfRequest *req)
{
#ifdef USE_SERF
    apr_status_t rv;
    apr_thread_t *thd;
    apr_threadattr_t *thd_attr;

    elfRequest *copy;

    if (thread_state == 0)
    {
        thread_state = 1;
        copy = (elfRequest *)malloc(sizeof(elfRequest));
        memset(copy, 0x0, sizeof(elfRequest));
        memset(url_copy, 0x0, sizeof(url_copy));
        memset(method_copy, 0x0, sizeof(method_copy));
        copy->url = strncpy(url_copy, req->url, sizeof(url_copy) - 1);
        copy->method = strncpy(method_copy, req->method, sizeof(method_copy) - 1);

        /* The default thread attribute: detachable */
        apr_threadattr_create(&thd_attr, pool);

        /* If the thread attribute is a default value, you can pass NULL to the second argument */
        rv = apr_thread_create(&thd, thd_attr, making_the_request, (void *)copy, pool);
    }
#endif
}
