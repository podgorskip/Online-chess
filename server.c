#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>

#define PRZEGRANA -1000
#define WYGRANA 1000
#define PUSTE_POLE 12

// 0-król, 1-hetman, 2-wieża, 3-goniec, 4-skoczek, 5-pionek,
// 6-król_k, 7-hetman_k, 8-wieża_k, 9-goniec_k, 10-skoczek_k, 11-pionek_k, 12-pole puste

char buff[1000];
int client_sock;

char plansza[8][8] = {8, 11, 12, 12, 12, 12, 5, 2,
                      10, 11, 12, 12, 12, 12, 5, 4,
                      9, 11, 12, 12, 12, 12, 5, 3,
                      6, 11, 12, 12, 12, 12, 5, 0,
                      7, 11, 12, 12, 12, 12, 5, 1,
                      9, 11, 12, 12, 12, 12, 5, 3,
                      10, 11, 12, 12, 12, 12, 5, 4,
                      8, 11, 12, 12, 12, 12, 5, 2};

int MAX_KIER[] = {8, 8, 4, 4, 8, 3, 8, 8, 4, 4, 8, 3, 0};
int MAX_ODL[] = {2, 8, 8, 8, 2, 2, 2, 8, 8, 8, 2, 2, 0};

int WX[12][8] = {{0, 1, 1, 1, 0, -1, -1, -1},
                 {0, 1, 1, 1, 0, -1, -1, -1},
                 {0, 1, 0, -1},
                 {1, 1, -1, -1},
                 {1, 2, 2, 1, -1, -2, -2, -1},
                 {-1, 0, 1},
                 {0, 1, 1, 1, 0, -1, -1, -1},
                 {0, 1, 1, 1, 0, -1, -1, -1},
                 {0, 1, 0, -1},
                 {1, 1, -1, -1},
                 {1, 2, 2, 1, -1, -2, -2, -1},
                 {-1, 0, 1}};

int WY[12][8] = {{-1, -1, 0, 1, 1, 1, 0, -1},
                 {-1, -1, 0, 1, 1, 1, 0, -1},
                 {-1, 0, 1, 0},
                 {-1, 1, 1, -1},
                 {-2, -1, 1, 2, 2, 1, -1, -2},
                 {-1, -1, -1},
                 {-1, -1, 0, 1, 1, 1, 0, -1},
                 {-1, -1, 0, 1, 1, 1, 0, -1},
                 {-1, 0, 1, 0},
                 {-1, 1, 1, -1},
                 {-2, -1, 1, 2, 2, 1, -1, -2},
                 {1, 1, 1}};

void print(char plansza[8][8])
{
    int status;
    bzero(buff, sizeof(buff));
    char fig[] = "khwgspKHWGSP ";
    printf("+-+-+-+-+-+-+-+-+-+\n| |0|1|2|3|4|5|6|7|\n"
           "+-+-+-+-+-+-+-+-+-+\n|0|%c|%c|%c|%c|%c|%c|%c|%c|\n"
           "+-+-+-+-+-+-+-+-+-+\n|1|%c|%c|%c|%c|%c|%c|%c|%c|\n"
           "+-+-+-+-+-+-+-+-+-+\n|2|%c|%c|%c|%c|%c|%c|%c|%c|\n"
           "+-+-+-+-+-+-+-+-+-+\n|3|%c|%c|%c|%c|%c|%c|%c|%c|\n"
           "+-+-+-+-+-+-+-+-+-+\n|4|%c|%c|%c|%c|%c|%c|%c|%c|\n"
           "+-+-+-+-+-+-+-+-+-+\n|5|%c|%c|%c|%c|%c|%c|%c|%c|\n"
           "+-+-+-+-+-+-+-+-+-+\n|6|%c|%c|%c|%c|%c|%c|%c|%c|\n"
           "+-+-+-+-+-+-+-+-+-+\n|7|%c|%c|%c|%c|%c|%c|%c|%c|\n",
           fig[plansza[0][0]], fig[plansza[0][1]], fig[plansza[0][2]], fig[plansza[0][3]], fig[plansza[0][4]], fig[plansza[0][5]], fig[plansza[0][6]], fig[plansza[0][7]],
           fig[plansza[1][0]], fig[plansza[1][1]], fig[plansza[1][2]], fig[plansza[1][3]], fig[plansza[1][4]], fig[plansza[1][5]], fig[plansza[1][6]], fig[plansza[1][7]],
           fig[plansza[2][0]], fig[plansza[2][1]], fig[plansza[2][2]], fig[plansza[2][3]], fig[plansza[2][4]], fig[plansza[2][5]], fig[plansza[2][6]], fig[plansza[2][7]],
           fig[plansza[3][0]], fig[plansza[3][1]], fig[plansza[3][2]], fig[plansza[3][3]], fig[plansza[3][4]], fig[plansza[3][5]], fig[plansza[3][6]], fig[plansza[3][7]],
           fig[plansza[4][0]], fig[plansza[4][1]], fig[plansza[4][2]], fig[plansza[4][3]], fig[plansza[4][4]], fig[plansza[4][5]], fig[plansza[4][6]], fig[plansza[4][7]],
           fig[plansza[5][0]], fig[plansza[5][1]], fig[plansza[5][2]], fig[plansza[5][3]], fig[plansza[5][4]], fig[plansza[5][5]], fig[plansza[5][6]], fig[plansza[5][7]],
           fig[plansza[6][0]], fig[plansza[6][1]], fig[plansza[6][2]], fig[plansza[6][3]], fig[plansza[6][4]], fig[plansza[6][5]], fig[plansza[6][6]], fig[plansza[6][7]],
           fig[plansza[7][0]], fig[plansza[7][1]], fig[plansza[7][2]], fig[plansza[7][3]], fig[plansza[7][4]], fig[plansza[7][5]], fig[plansza[7][6]], fig[plansza[7][7]]);

    sprintf(buff, "+-+-+-+-+-+-+-+-+-+\n| |0|1|2|3|4|5|6|7|\n"
                  "+-+-+-+-+-+-+-+-+-+\n|0|%c|%c|%c|%c|%c|%c|%c|%c|\n"
                  "+-+-+-+-+-+-+-+-+-+\n|1|%c|%c|%c|%c|%c|%c|%c|%c|\n"
                  "+-+-+-+-+-+-+-+-+-+\n|2|%c|%c|%c|%c|%c|%c|%c|%c|\n"
                  "+-+-+-+-+-+-+-+-+-+\n|3|%c|%c|%c|%c|%c|%c|%c|%c|\n"
                  "+-+-+-+-+-+-+-+-+-+\n|4|%c|%c|%c|%c|%c|%c|%c|%c|\n"
                  "+-+-+-+-+-+-+-+-+-+\n|5|%c|%c|%c|%c|%c|%c|%c|%c|\n"
                  "+-+-+-+-+-+-+-+-+-+\n|6|%c|%c|%c|%c|%c|%c|%c|%c|\n"
                  "+-+-+-+-+-+-+-+-+-+\n|7|%c|%c|%c|%c|%c|%c|%c|%c|\n",
            fig[plansza[0][0]], fig[plansza[0][1]], fig[plansza[0][2]], fig[plansza[0][3]], fig[plansza[0][4]], fig[plansza[0][5]], fig[plansza[0][6]], fig[plansza[0][7]],
            fig[plansza[1][0]], fig[plansza[1][1]], fig[plansza[1][2]], fig[plansza[1][3]], fig[plansza[1][4]], fig[plansza[1][5]], fig[plansza[1][6]], fig[plansza[1][7]],
            fig[plansza[2][0]], fig[plansza[2][1]], fig[plansza[2][2]], fig[plansza[2][3]], fig[plansza[2][4]], fig[plansza[2][5]], fig[plansza[2][6]], fig[plansza[2][7]],
            fig[plansza[3][0]], fig[plansza[3][1]], fig[plansza[3][2]], fig[plansza[3][3]], fig[plansza[3][4]], fig[plansza[3][5]], fig[plansza[3][6]], fig[plansza[3][7]],
            fig[plansza[4][0]], fig[plansza[4][1]], fig[plansza[4][2]], fig[plansza[4][3]], fig[plansza[4][4]], fig[plansza[4][5]], fig[plansza[4][6]], fig[plansza[4][7]],
            fig[plansza[5][0]], fig[plansza[5][1]], fig[plansza[5][2]], fig[plansza[5][3]], fig[plansza[5][4]], fig[plansza[5][5]], fig[plansza[5][6]], fig[plansza[5][7]],
            fig[plansza[6][0]], fig[plansza[6][1]], fig[plansza[6][2]], fig[plansza[6][3]], fig[plansza[6][4]], fig[plansza[6][5]], fig[plansza[6][6]], fig[plansza[6][7]],
            fig[plansza[7][0]], fig[plansza[7][1]], fig[plansza[7][2]], fig[plansza[7][3]], fig[plansza[7][4]], fig[plansza[7][5]], fig[plansza[7][6]], fig[plansza[7][7]]);
    status = send(client_sock, buff, strlen(buff), 0);
    printf("\n");
}

int ocena(char plansza[8][8])
{
    int wynik = 0;
    int oc[] = {PRZEGRANA, -17, -8, -5, -3, -1, WYGRANA, 17, 8, 5, 3, 1, 0};
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            wynik += oc[plansza[i][j]];
        }
    }
    return wynik;
}

int najlepszy(char plansza[8][8], int tryb, int *x, int *y, int *k, int *o)
{
    int px_pom, py_pom, k_pom, o_pom;
    int px, py, dx, dy, kierunek, odleglosc;
    int wynik, wmax, wmin, ruch_figury, zbijana_figura;

    wynik = ocena(plansza);

    if (tryb == 0 || 2 * wynik > WYGRANA || 2 * wynik < PRZEGRANA)
    {
        return wynik;
    }

    if (tryb % 2 == 0)
    {
        for (px = 0, wmax = 100 * PRZEGRANA; px < 8; px++)
        {
            for (py = 0; py < 8; py++)
            {
                if (plansza[px][py] >= 6 && plansza[px][py] < 12)
                {
                    for (kierunek = 0; kierunek < MAX_KIER[plansza[px][py]]; kierunek++)
                    {
                        for (odleglosc = 1; odleglosc < MAX_ODL[plansza[px][py]]; odleglosc++)
                        {

                            dx = (odleglosc - 1) * WX[plansza[px][py]][kierunek];
                            dy = (odleglosc - 1) * WY[plansza[px][py]][kierunek];

                            if (odleglosc >= 2 && plansza[px + dx][py + dy] != PUSTE_POLE)
                            {
                                break;
                            }

                            dx = odleglosc * WX[plansza[px][py]][kierunek];
                            dy = odleglosc * WY[plansza[px][py]][kierunek];

                            if (px + dx >= 0 && px + dx < 8 && py + dy >= 0 && py + dy < 8)
                            {
                                if (plansza[px + dx][py + dy] == PUSTE_POLE || plansza[px + dx][py + dy] <= 5)
                                {
                                    if (plansza[px][py] != 11 || (plansza[px + dx][py + dy] == PUSTE_POLE && dx == 0) || (plansza[px + dx][py + dy] != PUSTE_POLE && dx != 0))
                                    {
                                        ruch_figury = plansza[px][py];
                                        zbijana_figura = plansza[px + dx][py + dy];
                                        plansza[px + dx][py + dy] = plansza[px][py];
                                        plansza[px][py] = PUSTE_POLE;

                                        if (plansza[px + dx][py + dy] == 11 && py + dy == 7)
                                        { // zamiana na hetmana
                                            plansza[px + dx][py + dy] = 7;
                                        }

                                        wynik = najlepszy(plansza, tryb - 1, &px_pom, &py_pom, &k_pom, &o_pom);

                                        plansza[px][py] = ruch_figury;
                                        plansza[px + dx][py + dy] = zbijana_figura;

                                        if (wynik >= wmax)
                                        {
                                            wmax = wynik;
                                            *x = px;
                                            *y = py;
                                            *k = kierunek;
                                            *o = odleglosc;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return wmax;
    }
    else
    {
        for (px = 0, wmin = 100 * WYGRANA; px < 8; px++)
        {
            for (py = 0; py < 8; py++)
            {
                if (plansza[px][py] <= 5)
                {
                    for (kierunek = 0; kierunek < MAX_KIER[plansza[px][py]]; kierunek++)
                    {
                        for (odleglosc = 1; odleglosc < MAX_ODL[plansza[px][py]]; odleglosc++)
                        {

                            dx = (odleglosc - 1) * WX[plansza[px][py]][kierunek];
                            dy = (odleglosc - 1) * WY[plansza[px][py]][kierunek];

                            if (odleglosc >= 2 && plansza[px + dx][py + dy] != PUSTE_POLE)
                            {
                                break;
                            }

                            dx = odleglosc * WX[plansza[px][py]][kierunek];
                            dy = odleglosc * WY[plansza[px][py]][kierunek];

                            if (px + dx >= 0 && px + dx < 8 && py + dy >= 0 && py + dy < 8)
                                if (plansza[px + dx][py + dy] >= 6)
                                {
                                    if (plansza[px][py] != 5 || (plansza[px + dx][py + dy] == PUSTE_POLE && dx == 0) || (plansza[px + dx][py + dy] != PUSTE_POLE && dx != 0))
                                    {
                                        ruch_figury = plansza[px][py];
                                        zbijana_figura = plansza[px + dx][py + dy];
                                        plansza[px + dx][py + dy] = plansza[px][py];
                                        plansza[px][py] = PUSTE_POLE;

                                        if (plansza[px + dx][py + dy] == 5 && py + dy == 0)
                                        { // zamiana na hetmana
                                            plansza[px + dx][py + dy] = 1;
                                        }

                                        wynik = najlepszy(plansza, tryb - 1, &px_pom, &py_pom, &k_pom, &o_pom);

                                        plansza[px][py] = ruch_figury;
                                        plansza[px + dx][py + dy] = zbijana_figura;

                                        if (wynik <= wmin)
                                        {
                                            wmin = wynik;
                                            *x = px;
                                            *y = py;
                                            *k = kierunek;
                                            *o = odleglosc;
                                        }
                                    }
                                }
                        }
                    }
                }
            }
        }

        return wmin;
    }
}

int main()
{
    int server_sock, status;
    struct sockaddr_in server;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        printf("[-]Socket error.");
        return 0;
    }
    else
    {
        printf("[+]TCP server socket created.\n");
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(8000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    status = bind(server_sock, (struct sockaddr *)&server, sizeof server);
    if (status < 0)
    {
        printf("[-]Bind error.\n");
        return 0;
    }
    else
    {
        printf("[+]Bind to the port: %d.\n", 8000);
    }

    status = listen(server_sock, 10);
    if (status < 0)
    {
        printf("[-]Listen error.\n");
        return 0;
    }
    else
    {
        printf("Listening: \n");
    } 
    
    while (1)
    {
        client_sock = accept(server_sock, 0, 0);
        if (client_sock < 0)
        {
            printf("[-]Accept error.\n");
            return 0;
        }
        else
        {
            printf("[+]Client connected.\n");
        }

        int x, y, k, o, dx, dy, x2, y2;
        sprintf(buff, "1");

        while (1)
        {

            najlepszy(plansza, 4, &x, &y, &k, &o);

            dx = o * WX[plansza[x][y]][k];
            dy = o * WY[plansza[x][y]][k];

            plansza[x + dx][y + dy] = plansza[x][y];
            plansza[x][y] = 12;

            if (plansza[x + dx][y + dy] == 11 && y + dy == 7)
            {
                plansza[x + dx][y + dy] = 7;
            }

            print(plansza); // sends board after computer's move

           if(ocena(plansza) >= WYGRANA) {
                printf("Computer wins.\n");
                close(client_sock);
                close(server_sock);
            } else if(ocena(plansza) <= PRZEGRANA) {
                printf("You win.\n");
                close(client_sock);
                close(server_sock);
            }

            strcpy(buff, "Please enter coordinates - from (<, ^): ");
            send(client_sock, buff, strlen(buff), 0);
            bzero(buff, sizeof(buff));

            status = recv(client_sock, buff, sizeof(buff), 0); // gets coordinates which chessman moves
            buff[status] = '\0';
            x = (int)buff[0] - '0';
            y = (int)buff[1] - '0';
            bzero(buff, sizeof(buff));

            strcpy(buff, "Please enter coordinates - to (<, ^): ");
            send(client_sock, buff, strlen(buff), 0);
            bzero(buff, sizeof(buff));

            status = recv(client_sock, buff, sizeof(buff), 0); // gets coordinates where chessman moves
            buff[status] = '\0';
            x2 = (int)buff[0] - '0';
            y2 = (int)buff[1] - '0';
            printf("\nFrom (%d,%d) to (%d, %d).\n", x, y, x2, y2);
            bzero(buff, sizeof(buff));
            
            plansza[x2][y2] = plansza[x][y];
            plansza[x][y] = PUSTE_POLE;

            if (plansza[x2][y2] == 5 && y2 == 0)
            {
                plansza[x2][y2] = 1;
            }

            print(plansza);

            if(ocena(plansza) >= WYGRANA) {
                printf("Computer wins.\n");
                close(client_sock);
                close(server_sock);
            } else if(ocena(plansza) <= PRZEGRANA) {
                printf("You win.\n");
                close(client_sock);
                close(server_sock);
            }
        }
    }
}
