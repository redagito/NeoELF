#pragma once

// Logging and global error management

void elfStartLog();

void elfLogWrite(const char* fmt, ...);

void elfSetError(int code, const char* fmt, ...);

void elfWriteLogLine(const char* str);