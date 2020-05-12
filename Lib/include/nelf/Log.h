#pragma once

// Logging and global error management

// Initializes with default log path
void elfInitLog();
void elfDeinitLog();

// Sets custom log path
void elfSetLogFilePath(const char* filePath);

// Creates log file, starts logging to file
void elfStartLog();

void elfLogWrite(const char* fmt, ...);

// Sets last error message and code
void elfSetError(int code, const char* fmt, ...);

void elfWriteLogLine(const char* str);

// Retrieve last set error info
const char* elfGetErrorString();
int elfGetError();