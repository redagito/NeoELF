

//#define GFX_NONE					0x0000
#define GFX_TRANSPARENT 0x0001
#define GFX_ADD 0x0002
#define GFX_MULTIPLY 0x0003
#define GFX_SUBTRACT 0x0004

//#define GFX_NONE					0x0000
//#define GFX_LINEAR					0x0001

#define GFX_MAX_CIRCLE_VERTICES 255

#define GFX_GBUFFER_DEPTH 0x0001
#define GFX_GBUFFER_FILL 0x0002
#define GFX_GBUFFER_LIGHTING 0x0003

//////////////////////////////// GENERAL ////////////////////////////////

void gfxInitGeneral();
void gfxDeinitGeneral();

void gfxIncRef(gfxObject* obj);
void gfxDecRef(gfxObject* obj);
void gfxIncObj(int type);
void gfxDecObj(int type);

int gfxGetObjectType(gfxObject* obj);
int gfxGetObjectRefCount(gfxObject* obj);

int gfxGetGlobalRefCount();
int gfxGetGlobalObjCount();

//////////////////////////////// MATH ////////////////////////////////

void gfxVecToEuler(float* vec, float* euler);
void gfxVecNormalize(float* vec);
void gfxVecDotVec(float* vec1, float* vec2, float* dot);
void gfxVecCrossProduct(float* vec1, float* vec2, float* vec3);
float gfxVecLength(float* vec);
float gfxVecSquaredLength(float* vec);

void gfxQuaSetIdentity(float* qua);
void gfxQuaNormalize(float* qua, float* result);
void gfxQuaGetInverse(float* qua, float* invqua);
void gfxQuaFromAngleAxis(float angle, float* axis, float* qua);
void gfxQuaFromEuler(float x, float y, float z, float* qua);
void gfxQuaToMatrix4(float* qua, float* mat);
void gfxQuaToEuler(float* qua, float* euler);
void gfxRotateQua(float x, float y, float z, float* qua);
void gfxRotateQuaLocal(float x, float y, float z, float* qua);
void gfxMulQuaVec(float* qua, float* vec1, float* vec2);
void gfxMulQuaQua(float* qua1, float* qua2, float* qua3);
void gfxQuaSlerp(float* qa, float* qb, double t, float* result);

void gfxMatrix4SetIdentity(float* mat);
void gfxMatrix3SetIdentity(float* mat);
unsigned char gfxMatrix4GetInverse(float* mat1, float* mat2);
unsigned char gfxMatrix3GetInverse(float* mat1, float* mat2);
void gfxMatrix4GetTranspose(float* mat1, float* mat2);
void gfxMatrix3GetTranspose(float* mat1, float* mat2);
void gfxMatrix3ToQua(float* mat, float* qua);
void gfxMatrix4ToEuler(float* mat, float* eul);
void gfxMulMatrix4Vec3(float* m1, float* vec1, float* vec2);
void gfxMulMatrix4Vec4(float* m1, float* vec1, float* vec2);
void gfxMulMatrix4Matrix4(float* m1, float* m2, float* m3);
void gfxMulMatrix3Matrix4(float* m1, float* m2, float* m3);

unsigned char gfxBoxSphereIntersect(float* bmin, float* bmax, float* spos, float srad);

unsigned char gfxAabbInsideFrustum(float frustum[6][4], float* min, float* max);
unsigned char gfxSphereInsideFrustum(float frustum[6][4], float* pos, float radius);

//////////////////////////////// TRANSFORM ////////////////////////////////

void gfxSetViewport(int x, int y, int width, int height);

void gfxGetPerspectiveProjectionMatrix(float fov, float aspect, float near, float far, float* mat);
void gfxGetOrthographicProjectionMatrix(float left, float right, float bottom, float top, float near, float far,
                                        float* matrix);
void gfxGetFrustum(float proj[16], float modl[16], float frustum[6][4]);
void gfxSetPerspective(float fov, float aspect, float near, float far);
void gfxSetOrthographic(float left, float right, float bottom, float top, float near, float far);

void gfxProject(float x, float y, float z, float modl[16], float proj[16], int viewport[4], float winCoord[3]);
void gfxUnProject(float x, float y, float z, float modl[16], float proj[16], int viewport[4], float objCoord[3]);

void gfxRecalcTransformMatrix(gfxTransform* transform);
float* gfxGetTransformMatrix(gfxTransform* transform);
float* gfxGetTransformNormalMatrix(gfxTransform* transform);
gfxTransform* gfxCreateCameraTransform();
gfxTransform* gfxCreateObjectTransform();
void gfxDestroyTransform(gfxTransform* transform);

void gfxSetTransformPosition(gfxTransform* transform, float x, float y, float z);
void gfxSetTransformRotation(gfxTransform* transform, float x, float y, float z);
void gfxSetTransformScale(gfxTransform* transform, float x, float y, float z);
void gfxSetTransformOrientation(gfxTransform* transform, float x, float y, float z, float w);
void gfxRotateTransform(gfxTransform* transform, float x, float y, float z);
void gfxRotateTransformLocal(gfxTransform* transform, float x, float y, float z);
void gfxMoveTransform(gfxTransform* transform, float x, float y, float z);
void gfxMoveTransformLocal(gfxTransform* transform, float x, float y, float z);
unsigned char gfxGetTransformCameraMode(gfxTransform* transform);
void gfxGetTransformPosition(gfxTransform* transform, float* params);
void gfxGetTransformRotation(gfxTransform* transform, float* params);
void gfxGetTransformScale(gfxTransform* transofrm, float* paramt);
void gfxGetTransformOrientation(gfxTransform* transform, float* params);

//////////////////////////////// DRIVER ////////////////////////////////

unsigned char gfxInit();
void gfxDeinit();

int gfxGetVersion();

void gfxClearBuffers(float r, float g, float b, float a, float d);
void gfxClearColorBuffer(float r, float g, float b, float a);
void gfxClearDepthBuffer(float d);

void gfxReadPixels(int x, int y, int width, int height, int format, int dataFormat, void* data);
void gfxCopyFrameBuffer(gfxTexture* texture, int ox, int oy, int x, int y, int width, int height);

void gfxResetVerticesDrawn();
int gfxGetVerticesDrawn(unsigned int drawMode);

void gfxPrintGLError();

//////////////////////////////// VERTEX ARRAY/INDEX ////////////////////////////////

gfxVertexData* gfxCreateVertexData(int count, int format, int dataType);
void gfxDestroyVertexData(void* data);
int gfxGetVertexDataCount(gfxVertexData* data);
int gfxGetVertexDataFormat(gfxVertexData* data);
int gfxGetVertexDataSizeBytes(gfxVertexData* data);

void* gfxGetVertexDataBuffer(gfxVertexData* data);
void gfxUpdateVertexData(gfxVertexData* data);
void gfxUpdateVertexDataSubData(gfxVertexData* data, int start, int length);

gfxVertexArray* gfxCreateVertexArray(unsigned char gpuData);
void gfxDestroyVertexArray(void* data);

int gfxGetVertexArrayVertexCount(gfxVertexArray* vertexArray);
void gfxSetVertexArrayData(gfxVertexArray* vertexArray, int target, gfxVertexData* data);
void gfxResetVertexArray(gfxVertexArray* vertexArray);
void gfxSetVertexArray(gfxVertexArray* vertexArray);
void gfxDrawVertexArray(gfxVertexArray* vertexArray, int count, int drawMode);

gfxVertexIndex* gfxCreateVertexIndex(unsigned char gpuData, gfxVertexData* data);
void gfxDestroyVertexIndex(void* data);

int gfxGetVertexIndexIndiceCount(gfxVertexIndex* vertexIndex);
void gfxDrawVertexIndex(gfxVertexIndex* vertexIndex, unsigned int drawMode);

//////////////////////////////// TEXTURE ////////////////////////////////

int gfxGetMaxTextureSize();

gfxTexture* gfxCreateTexture();
gfxTexture* gfxCreate2dTexture(unsigned int width, unsigned int height, float anisotropy, int mode, int filter,
                               int format, int internalFormat, int dataFormat, void* data);
gfxTexture* gfxCreateCubeMap(unsigned int width, unsigned int height, float anisotropy, int mode, int filter,
                             int format, int internalFormat, int dataFormat, void* xpos, void* xneg, void* ypos,
                             void* yneg, void* zpos, void* zneg);
void gfxDestroyTexture(void* data);

int gfxGetTextureType(gfxTexture* texture);
int gfxGetTextureWidth(gfxTexture* texture);
int gfxGetTextureHeight(gfxTexture* texture);
int gfxGetTextureFormat(gfxTexture* texture);
int gfxGetTextureDataFormat(gfxTexture* texture);

void gfxSetTexture(gfxTexture* texture, int slot);
void elfDisableTexture(int slot);

//////////////////////////////// SHADER PROGRAM ////////////////////////////////

gfxShaderProgram* gfxCreateShaderProgram(const char* vertex, const char* fragment);
void gfxDestroyShaderProgram(gfxShaderProgram* shaderProgram);
void gfxDestroyShaderPrograms(gfxShaderProgram* shaderProgram);

void gfxSetShaderProgram(gfxShaderProgram* shaderProgram);

void gfxSetShaderProgramUniform1i(const char* name, int i);
void gfxSetShaderProgramUniform1f(const char* name, float f);
void gfxSetShaderProgramUniformVec2(const char* name, float x, float y);
void gfxSetShaderProgramUniformVec3(const char* name, float x, float y, float z);
void gfxSetShaderProgramUniformVec4(const char* name, float x, float y, float z, float w);
void gfxSetShaderProgramUniformMat4(const char* name, float* matrix);

//////////////////////////////// RENDER TARGET ////////////////////////////////

gfxRenderTarget* gfxCreateRenderTarget(unsigned int width, unsigned int height);
void gfxDestroyRenderTarget(void* data);

gfxRenderTarget* gfxGetCurRenderTarget();
void gfxSetRenderTargetColorTexture(gfxRenderTarget* renderTarget, unsigned int n, gfxTexture* color);
void gfxSetRenderTargetDepthTexture(gfxRenderTarget* renderTarget, gfxTexture* depth);
unsigned char gfxSetRenderTarget(gfxRenderTarget* renderTarget);
void gfxDisableRenderTarget();

//////////////////////////////// SHADER PARAMS ////////////////////////////////

void gfxSetColor(gfxColor* color, float r, float g, float b, float a);
void gfxSetShaderParamsDefault(gfxShaderParams* shaderParams);
void gfxSetMaterialParamsDefault(gfxShaderParams* shaderParams);
void gfxSetTextureParamsDefault(gfxShaderParams* shaderParams);
void gfxSetShaderParams(gfxShaderParams* shaderParams);

//////////////////////////////// SHADER GEN ////////////////////////////////

void gfxGetShaderProgramConfig(gfxShaderParams* shaderParams, gfxShaderConfig* shaderConfig);
gfxShaderProgram* gfxGetShaderProgram(gfxShaderConfig* config);

//////////////////////////////// QUERY ////////////////////////////////

gfxQuery* gfxCreateQuery();
void gfxDestroyQuery(gfxQuery* query);

void gfxBeginQuery(gfxQuery* query);
void gfxEndQuery(gfxQuery* query);
unsigned char gfxIsQueryResult(gfxQuery* query);
int gfxGetQueryResult(gfxQuery* query);

//////////////////////////////// G-BUFFER ////////////////////////////////

gfxGbuffer* gfxCreateGbuffer(int width, int height);
void gfxDestroyGbuffer(void* data);

void gfxBindGbuffer(gfxGbuffer* gbuffer, gfxShaderParams* shaderParams);
void gfxBindGbufferLight(gfxGbuffer* gbuffer, gfxShaderParams* shaderParams);
void gfxBindGbufferMain(gfxGbuffer* gbuffer, gfxShaderParams* shaderParams);

gfxTexture* gfxGetGbufferDepth(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferDiffuse(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferSpecular(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferMain(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferBuf1(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferBuf2(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferBuf3(gfxGbuffer* gbuffer);
gfxTexture* gfxGetGbufferBuf4(gfxGbuffer* gbuffer);
