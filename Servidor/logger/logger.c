// logger.c
#include "logger.h"
#include <stdio.h>

int CURRENT_LOG_LEVEL = LOG_INFO; // Declarar y establecer el nivel de severidad predeterminado

void log_message(int severity, const char *message)
{
    if (severity <= CURRENT_LOG_LEVEL)
    {
        FILE *log_file = fopen("log.txt", "a"); // Abre el archivo en modo "a" (append)
        if (log_file)
        {
            switch (severity)
            {
            case LOG_INFO:
                fprintf(log_file, "INFO: %s\n", message);
                break;
            case LOG_WARNING:
                fprintf(log_file, "WARNING: %s\n", message);
                break;
            case LOG_ERROR:
                fprintf(log_file, "ERROR: %s\n", message);
                break;
            case LOG_DEBUG:
                fprintf(log_file, "DEBUG: %s\n", message);
                break;
            }
            fclose(log_file);
        }
    }
}
