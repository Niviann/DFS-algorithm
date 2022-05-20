#include <iostream>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <iomanip>
using namespace std;

const bool map_segments[6][5][5] = { //Wgranie segmentów mapy
    {
        {0, 0, 1, 0, 0},
        {0, 1, 1, 0, 0},
        {1, 1, 0, 1, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 0, 0},
    },
    {
        {0, 0, 1, 1, 0},
        {1, 0, 0, 1, 0},
        {1, 1, 1, 1, 1},
        {0, 1, 1, 1, 0},
        {0, 0, 1, 1, 0},
    },
    {
        {1, 1, 1, 1, 1},
        {1, 1, 0, 1, 1},
        {1, 1, 0, 0, 1},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 1},
    },
    {
        {0, 0, 1, 1, 1},
        {0, 1, 1, 0, 1},
        {1, 1, 0, 1, 1},
        {1, 0, 1, 1, 0},
        {0, 1, 1, 0, 0},
    },
    {
        {1, 1, 1, 0, 0},
        {0, 1, 1, 1, 0},
        {1, 1, 0, 1, 1},
        {1, 1, 0, 1, 1},
        {1, 1, 1, 1, 0},
    },
    {
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
        {1, 1, 1, 1, 1},
        {0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0},
    }};

int map_generator(int map[20][40])
{
    int whitecount = 2;
    srand((unsigned)time(0)); //generacja mapy bez pozycji
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 8; j++)
        {
            int map_segment = (rand() % 6);
            for (int k = 0; k < 5; k++)
                for (int l = 0; l < 5; l++)
                    map[k + 5 * i][l + 5 * j] = map_segments[map_segment][k][l];
        }

    for (int i = 0; i < 20; i++)
        for (int j = 0; j < 40; j++)
            if (map[i][j] == 0)
                map[i][j] = -1;
            else
                map[i][j] = -2;

    srand((unsigned)time(0)); //początek generacji pozycji startowej
    int pos_y = (rand() % 20);
    int pos_x = (rand() % 40);

    while (map[pos_y][pos_x] != -2)
    {
        srand((unsigned)time(0));
        pos_y = (rand() % 20);
        pos_x = (rand() % 40);
    }

    map[pos_y][pos_x] = 0; //ustawienie pozycji startowej

    srand((unsigned)time(0)); //początek generacji pozycji końcowej
    pos_y = (rand() % 20);
    pos_x = (rand() % 40);

    while (map[pos_y][pos_x] != -2)
    {
        srand((unsigned)time(0));
        pos_y = (rand() % 20);
        pos_x = (rand() % 40);
    }

    map[pos_y][pos_x] = 1; //ustawienie pozycji końcowej

    for (int i = 0; i < 20; i++) //ustawienie id każdemu z pól
        for (int j = 0; j < 40; j++)
            if (map[i][j] == -2)
            {
                map[i][j] = whitecount;
                whitecount++;
            }
    return whitecount;
}

void map_output(int map[20][40], bool end = false)
{
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 40; j++)
            if (map[i][j] == -1)
                cout << "|###|";
            else if (map[i][j] == -2)
                cout << "  +  ";
            else if (end == true && map[i][j] >= 2)
            {
                cout << "     ";
            }
            else if (end == true && map[i][j] == 0)
                cout << "start";
            else if (end == true && map[i][j] == 1)
                cout << " end ";
            else
                cout << " " << setw(3) << map[i][j] << " ";
        cout << endl;
    }
}

struct node
{
    int des;
    int src = 0;
    bool visited = false;
    node *next;
};

int DFS(node **list, int map[20][40], int pos = 0)
{
    node *p;
    p = list[pos];
    p->visited = true;
    if (pos == 1)
        return (1);

    while (p)
    {
        if (p->des != p->src /*Czy nie wraca do poprzedniego*/ && list[p->des]->visited != true /*Czy następny nie był już odwiedzony*/)
        {
            list[p->des]->src = pos; //ustawiam w następnym który był poprzedni
            pos = p->des;            //ustawiam pozycję na następnego
            if (DFS(list, map, pos) == 1)
            {
                cout << endl
                     << pos;

                if (pos != 1) //tworzę ścieżkę na mapie
                    for (int i = 0; i < 20; i++)
                        for (int j = 0; j < 40; j++)
                            if (map[i][j] == pos)
                                map[i][j] = -2;

                return (1);
            }
        }
        p = p->next; //jeżeli nie znaleziono sprawdzam kolejnego sąsiada
    }
    return (0);
}

void list_output(node **list, node *p, int whitecount)
{
    cout << endl
         << "Mapa w postaci listy sasiedztw:" << endl;
    for (int i = 0; i < whitecount; i++) //drukowanie listy
    {
        p = list[i];
        cout << i << " ---->";

        while (p)
        {
            cout << "  " << p->des << " ->";
            p = p->next;
        }
        cout << " XX" << endl;
    }
}

void clearLists(node **list, node *&p, int whitecount)
{
    node *temp;
    for (int i = 0; i < whitecount; i++)
    {
        p = list[i];
        while (p != NULL)
        {
            temp = p;
            p = p->next;
            delete temp;
        }
    }
    delete[] list;
}

int main()
{
    int map[20][40] = {0};
    int whitecount = map_generator(map);
    cout << "Wygenerowana mapa:" << endl;
    map_output(map);

    node **list;
    node *p;

    list = new node *[whitecount]; //Tablica list sąsiedztwa

    for (int i = 0; i < whitecount; i++) //wypełniam pustymi listami
        list[i] = NULL;

    for (int j = 0; j < 20; j++)
        for (int k = 0; k < 40; k++)
            if (map[j][k] >= 0)
            {
                if (map[j][k + 1] >= 0 && (k) < 39) // przesunięty w prawo
                {
                    p = new node;
                    p->des = map[j][k + 1];
                    p->next = list[map[j][k]];
                    list[map[j][k]] = p;
                }
                if (map[j][k - 1] >= 0 && (k) > 0) // przesunięty w lewo
                {
                    p = new node;
                    p->des = map[j][k - 1];
                    p->next = list[map[j][k]];
                    list[map[j][k]] = p;
                }
                if (map[j - 1][k] >= 0 && (j) > 0) // przesunięty w górę
                {
                    p = new node;
                    p->des = map[j - 1][k];
                    p->next = list[map[j][k]];
                    list[map[j][k]] = p;
                }
                if (map[j + 1][k] >= 0 && (j) < 19) // przesunięty w dół
                {
                    p = new node;
                    p->des = map[j + 1][k];
                    p->next = list[map[j][k]];
                    list[map[j][k]] = p;
                }
            }
    p = NULL;
    list_output(list, p, whitecount);

    cout << endl
         << "Numery ID drogi:";

    if (DFS(list, map) == 1)
        cout << endl
             << 0 << endl
             << endl
             << "Sciezka na mapie:" << endl;

    map_output(map, true);

    clearLists(list, p, whitecount);

    system("PAUSE");
    return (0);
}