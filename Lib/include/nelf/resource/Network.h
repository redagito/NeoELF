#pragma once

#include "nelf/resource/resourceHeader.h"

struct elfRequest
{
    ELF_RESOURCE_HEADER;
    char *url;
    char *method;
};

// TODO Return bool
void elfInitNetworking();
void elfDeinitNetworking();

elfRequest *elfCreateRequest(const char *name);
void elfDestroyRequest(void *data);

const char *elfGetRequestUrl(elfRequest *request);
void elfSetRequestUrl(elfRequest *request, const char *url);

const char *elfGetRequestMethod(elfRequest *request);
void elfSetRequestMethod(elfRequest *request, const char *method);

int elfGetRequestState();

void elfSendRequest(elfRequest *req);