#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <time.h>


struct Edge {
	char* Name;			// Индекс вершины (расположение в массиве)
	struct Edge* next;		// Указатель на следующее ребро в списке смежности
	int flagLoop;
};

struct Vertex {
	char* Name;			// Уникальное имя вершины
	int x;				// Координата x
	int y;				// Координата y 
	//int size_adj;			// Текущий размер списка смежности (нужно для записи в файл)
	struct Edge* root;			// Указатель на начало списка смежности
};

/* Cтруктура графа */
struct Graph {
	int size;			// Текущее кол-во вершин
	struct Vertex* vertex;			// Массив вершин
	//FILE* fd;			// Дескриптор файла
};


char* msgs[] = { "0. Quit", "1. Add Vertex", "2. Add Edge", "3. Delete Vertex and Edge", "4. Find", "5.Show ", "6.Time", "7.Random graph", "8.File use" };//  массив строк, диалоговых функций 
const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);// количество строк в массиве строк, орое автоматические считаетс



const char* errmsgs[] = { "Ok", "Duplicate key", "Table overflow", "rim" };// массив строк, ошибок

char* getStr() {
	char* Str = (char*)(calloc(256, 1));
	scanf("%s", Str);
	return Str;
}

int getInt(int* rc) {
	int n;
	n = scanf("%d[^\n]", rc);
	return n;
}

int dialog(const char* msgs[], int N)// функция для выбора действия
{
	char* errmsg = "";
	int rc;
	int i, n;
	do {
		puts(errmsg);
		errmsg = "You are wrong. Repeate, please!";
		for (i = 0; i < N; ++i)//откуда N берется?// цикл для вывода строк
			puts(msgs[i]);
		puts("Make your choice: --> ");
		n = getInt(&rc);// что тут происходит или должно происходить
		if (n == 0) rc = 0;
		//printf("rc=%d\n", rc);// в чем разница между rc и &rc?
		printf("rc=%d\n", rc);
	} while (rc < 0 || rc >= N);
	return rc;
}

int AddV(struct Tree* pgraph, char* nameF_dot) {

	int rc = 0, x, y;
	char* name = NULL;
	printf("Input unique vertex name: --> ");
	name = getStr();
	if (!name)
		return 0;
	printf("Input x dimension: --> ");
	int n = getInt(&x);
	if (!n)
		return 0;
	printf("Input y dimension: --> ");
	n = getInt(&y);
	if (!n)
		return 0;
	rc = insert_graph(pgraph, name, x, y, -1);
	printf("%s\n", errmsgs[rc]);
	return 1;

}

/* Функция вставки новой вершины в граф. */
int insert_graph(struct Graph* pgraph, char* name, int x, int y, int flag) {
	//if (flag == -1) {
	for (int i = 0; i < pgraph->size; ++i) {
		if ((!strcmp(name, pgraph->vertex[i].Name)) || ((x == pgraph->vertex[i].x) && (y == pgraph->vertex[i].y)))
			return 2;		// Найдена вершина с таким же именем или такими же координатами
	}
	++(pgraph->size);
	pgraph->vertex = (struct Vertex*)realloc(pgraph->vertex, sizeof(struct Vertex) * pgraph->size);
	pgraph->vertex[pgraph->size - 1].Name = name;
	pgraph->vertex[pgraph->size - 1].x = x;
	pgraph->vertex[pgraph->size - 1].y = y;

	pgraph->vertex[pgraph->size - 1].root = NULL;// вопрос
//\}

	return 0;
}

int AddE(struct Graph* pgraph, char* nameF_dot) {
	int ind_out, ind_in, rc;
	char* name_out = NULL;
	if (!pgraph->size) {
		printf("%s\n", errmsgs[4]);
		return 1;
	}
	printf("Enter the name of the vertex that the edge comes from: --> ");
	name_out = getStr();
	if (!name_out)
		return 0;
	for (int i = 0; i < pgraph->size; ++i) {
		if (!strcmp(name_out, pgraph->vertex[i].Name)) {
			ind_out = i;	// Нашли нужную вершину
			break;
		}
		if (i == pgraph->size - 1) {
			free(name_out);
			printf("%s", errmsgs[1]);
			return 1;	// Вершина не найдена
		}
	}
	char* name_in = NULL;
	printf("Enter the name of the vertex where the edge goes: --> ");
	name_in = getStr();
	if (!name_in)
		return 0;
	char* Edgename = NULL;
	printf("Enter the name of the edge: --> ");
	Edgename = getStr();
	//////////////////////////////////////////////
	rc = insert_edge(pgraph, ind_out, name_in, Edgename, 1);


	free(name_out);
	printf("%s\n", errmsgs[rc]);
	return 1;
}

/* Функция вставки нового ребра в граф. */
int insert_edge(struct Graph* pgraph, int ind_out, char* name_in, char* Edgename, int flag) {
	int ind_in;

	struct Edge* dupEdge;

	if (!pgraph->size)
		return 1;

	for (int k = 0; k < pgraph->size; k++) {
		dupEdge = pgraph->vertex[k].root;
		while (dupEdge) {
			if (!strcmp(Edgename, dupEdge->Name)) {
				printf("warning: edge exists in vertex \"%s\"\n", pgraph->vertex[k].Name);
				return 1;
			}
			dupEdge = dupEdge->next;
		}
	}


	for (int i = 0; i < pgraph->size; ++i) {
		if (!strcmp(name_in, pgraph->vertex[i].Name)) {
			ind_in = i;		// Нашли нужную вершину
			break;
		}
		if (i == pgraph->size - 1)
			return 1;		// Вершина не найдена
	}

	struct Edge* tmpE1 = pgraph->vertex[ind_out].root;
	struct Edge* tmpE2 = pgraph->vertex[ind_in].root;
	while (tmpE1) {
		tmpE2 = pgraph->vertex[ind_in].root;
		while (tmpE2) {
			//for (int i = 0; i < pgraph->size; ++i) {
			//	struct Edge* tmp = pgraph->vertex[i].root;
			if (!strcmp(tmpE1->Name, tmpE2->Name)) {
				printf("warning for vertexes: %s and %s\n", pgraph->vertex[ind_out].Name, pgraph->vertex[ind_in].Name);
				return(1);
				//	--(pgraph->vertex[i].size);
			}

			tmpE2 = tmpE2->next;
		}
		tmpE1 = tmpE1->next;
	}
	struct Edge* pF = pgraph->vertex[ind_out].root;
	while (pF) {
		if (!strcmp(Edgename, pF->Name))
			return 1;		// Такое ребро уже есть
		if (!pF->next)
			break;
		pF = pF->next;
	}
	struct Edge* tmp = (struct Edge*)malloc(sizeof(struct Edge));
	tmp->Name = Edgename;
	tmp->flagLoop = 0;
	if (ind_out == ind_in) {
		tmp->flagLoop = 1;
	}
	//tmp->wd = count_wd(pgraph->vertex[ind_in].x, pgraph->vertex[ind_in].y, pgraph->vertex[ind_out].x, pgraph->vertex[ind_out].y);

	tmp->next = NULL;
	if (!pF)
		pgraph->vertex[ind_out].root = tmp;
	else
		pF->next = tmp;

	pF = pgraph->vertex[ind_in].root;

	while (pF) {
		if (!strcmp(Edgename, pF->Name))
			return 1;		// Такое ребро уже есть
		if (!pF->next)
			break;
		pF = pF->next;
	}
	struct Edge* tmp1 = (struct Edge*)malloc(sizeof(struct Edge));
	tmp1->Name = Edgename;
	tmp1->flagLoop = 0;
	if (ind_out == ind_in) {
		tmp1->flagLoop = 1;
	}
	//tmp->wd = count_wd(pgraph->vertex[ind_in].x, pgraph->vertex[ind_in].y, pgraph->vertex[ind_out].x, pgraph->vertex[ind_out].y);

	tmp1->next = NULL;
	if (!pF)
		pgraph->vertex[ind_in].root = tmp1;
	else
		pF->next = tmp1;
	return 0;
}

/*int Show(struct Graph* pgraph) {
	int ind;
	struct Edge* p;
	struct Edge* p1;
	struct Edge* pLoop;//
	if (!pgraph->size) {
		printf("%s\n", errmsgs[4]);
		return 1;
	}
	int k, z = 0;
	for (int i = 0; i < pgraph->size; ++i) {
		printf("\n \"%d - %d %s\"  ", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
		k = 1;
		p = pgraph->vertex[i].root;
		pLoop = p;
		ind = 0;
		while (p) { //для списка смежности вершины i
			for (int j = 0; j < pgraph->size; ++j) {

				if (pLoop->flagLoop == 1) {
					p1 = pgraph->vertex[j].root; //для списка смежности вершины j
					while (p1) {
						if (!strcmp(p1->Name, p->Name)) {
							ind = j;

							if (k == 1)
							{
								printf("\"%d - %d %s\"\n", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
								k = 0;
							}
							else
							{
								printf(" \"%d - %d %s\" ", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
								printf(" --\"%d - %d %s\"\n", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
							}
								break;
						}
						p1 = p1->next;
					}
				}
				else {
				 if (j == i) continue;
				p1 = pgraph->vertex[j].root; //для списка смежности вершины j
				while (p1) {
					if (!strcmp(p1->Name, p->Name)) {
						ind = j;
						if (k == 1)
						{
							printf("\"%d - %d %s\"\n", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
							k = 0;
						}
						else
						{
							printf(" \"%d - %d %s\"  ", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
							printf("--\"%d - %d %s\"\n", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
						}
						break;
					}
					p1 = p1->next;
				}
				}
			}
			p = p->next;
		}
		//printf("\"%d - %d %s\"  ", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
		//printf("\n");
	}
	return 1;
}*/

int Show(struct Graph* pgraph, char* nameF_dot) {
	int ind;
	struct Edge* p;
	struct Edge* p1;
	struct Edge* pLoop;
	int j;
	int z;
	if (!pgraph->size) {
		printf("%s\n", errmsgs[4]);
		return 1;
	}
	for (int i = 0; i < pgraph->size; ++i) {
		//проверить, есть ли вершина i в массиве уже рассмотренных. если да - continue
		//printf("From vertex \"%d - %d %s\" edges comes to: ", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
		p = pgraph->vertex[i].root;
		//pLoop = p;
		//ind = 0;
		if (!p) {
			printf("From vertex \"%d - %d %s\" edges comes to:\n", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
			continue;
		}

		/////////////
		j = 0;
		for (j = i; j < pgraph->size; j++) {
			p = pgraph->vertex[i].root;
			if (j == i) {
				if (p->flagLoop == 0) {
					continue;
				}
			}
			while (p) {
				p1 = pgraph->vertex[j].root;
				while (p1) {
					/*if (j == i) {
						if (p->flagLoop == 0) {
							continue;
						}
					}*/
					if (!strcmp(p->Name, p1->Name)) {
						printf("From vertex \"%d - %d %s\" edges comes to: ", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
						printf("\"%d - %d %s\"\n", pgraph->vertex[j].x, pgraph->vertex[j].y, pgraph->vertex[j].Name);
						break;
					}
					p1 = p1->next;
				}
				if (j == i) break;
				p = p->next;
			}
		}

		////////////

		/*while (p) { //для списка смежности вершины i
			for (int j = 0; j < pgraph->size; ++j) {
				if (pLoop->flagLoop != 1 && j == i) continue; //не выводит лишний раз
				p1 = pgraph->vertex[j].root; //для списка смежности вершины j
				while (p1) {
					if (!strcmp(p1->Name, p->Name)) {
						ind = j;
						//добавить j в массив, чтобы в основном цикле не рассматривать ее уже
						printf("From vertex \"%d - %d %s\" edges comes to: ", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
						printf("\"%d - %d %s\"\n", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
						//p1 = p1->next;

						break;
					}
					p1 = p1->next;
				}
			}
			p = p->next;
		}*/
	}
	return 1;
}

int Showdot(struct Graph* pgraph, FILE* ddot) {
	int ind;
	struct Edge* p;
	struct Edge* p1;
	struct Edge* pLoop;
	int j;
	int z;
	if (!pgraph->size) {
		printf("%s\n", errmsgs[4]);
		return 1;
	}
	for (int i = 0; i < pgraph->size; ++i) {
		//проверить, есть ли вершина i в массиве уже рассмотренных. если да - continue
		//printf("From vertex \"%d - %d %s\" edges comes to: ", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
		p = pgraph->vertex[i].root;
		//pLoop = p;
		//ind = 0;
		if (!p) {
			fprintf(ddot, "\" %d - %d %s \";\n", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
			continue;
		}

		/////////////
		j = 0;
		for (j = i; j < pgraph->size; j++) {
			p = pgraph->vertex[i].root;
			if (j == i) {
				if (p->flagLoop == 0) {
					continue;
				}
			}
			while (p) {
				p1 = pgraph->vertex[j].root;
				while (p1) {
					/*if (j == i) {
						if (p->flagLoop == 0) {
							continue;
						}
					}*/
					if (!strcmp(p->Name, p1->Name)) {
						fprintf(ddot, "\" %d - %d %s \" -- ", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
						fprintf(ddot, "\" %d - %d %s \";\n", pgraph->vertex[j].x, pgraph->vertex[j].y, pgraph->vertex[j].Name);
						break;
					}
					p1 = p1->next;
				}
				if (j == i) break;
				p = p->next;
			}
		}

	}
	return 1;
}

/*int Show(struct Graph* pgraph) {
	int ind;
	struct Edge* p;
	struct Edge* p1;
	struct Edge* pLoop;//
	if (!pgraph->size) {
		printf("%s\n", errmsgs[4]);
		return 1;
	}
	for (int i = 0; i < pgraph->size; ++i) {
		printf("  From vertex \"%d - %d %s\" edges comes to: ", pgraph->vertex[i].x, pgraph->vertex[i].y, pgraph->vertex[i].Name);
		p = pgraph->vertex[i].root;
		pLoop = p;
		ind = 0;
		while (p) { // i
			for (int j = 0; j < pgraph->size; ++j) {

				if (pLoop->flagLoop == 1) {
					p1 = pgraph->vertex[j].root; // j
					while (p1) {
						if (!strcmp(p1->Name, p->Name)) {
							ind = j;
							printf("\"%d - %d %s\"  ", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
							break;
						}
						p1 = p1->next;
					}
				}
				else {
					if (j == i) continue;
					p1 = pgraph->vertex[j].root; //
					while (p1) {
						if (!strcmp(p1->Name, p->Name)) {
							ind = j;
							printf("\"%d - %d %s\"  ", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
							break;
						}
						p1 = p1->next;
					}
				}
			}
			p = p->next;
		}
		//printf("\"%d - %d %s\"  ", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
		printf("\n");
	}
	return 1;
}*/

int FileF(struct Graph* pgraph, char* nameF_dot) {
	int x = 0;
	printf("1. Read file\n");
	printf("2. Save file\n");
	int n = getInt(&x);
	if (!n)
		return 0;
	if (x == 1) {
		printf("read\n");
	}
	else if (x == 2) {
		printf("zapis\n");
		FILE* ddot = fopen(nameF_dot, "w+");
		fprintf(ddot, "graph G {\n");
		Showdot(pgraph, ddot);
		fprintf(ddot, "}");
		fclose(ddot);
	}
	return 1;
}

struct Edge* del_edge(struct Edge** root, struct Edge* target) {
	if (!(*root)->next) {
		free(*root);
		(*root) = NULL;
		return (*root);
	}
	if ((*root) == target) {
		(*root) = (*root)->next;
		free(target);
		return (*root);
	}
	struct Edge* tmp = (*root);
	while (tmp->next != target)
		tmp = tmp->next;
	tmp->next = target->next;
	free(target);
	return tmp;
}

int delete(struct Graph* pgraph, char* name) {
	int ind_del;
	if (!pgraph->size)
		return 4;
	for (int i = 0; i < pgraph->size; ++i) {
		if (!strcmp(name, pgraph->vertex[i].Name)) {
			ind_del = i;		// Нашли нужную вершину
			break;
		}
		if (i == pgraph->size - 1)
			return 1;		// Вершина не найдена
	}
	struct Edge* tmp_help = pgraph->vertex[ind_del].root;
	while (tmp_help) {
		for (int i = 0; i < pgraph->size; ++i) {
			if (i == ind_del) continue;
			struct Edge* tmp = pgraph->vertex[i].root;
			while (tmp) {
				if (!strcmp(tmp->Name, tmp_help->Name)) {
					printf("DELETE EDGE\n");
					//struct Edge *delEdge = tmp;
					//tmp = tmp->next;
					tmp = del_edge(&pgraph->vertex[i].root, tmp);
					//	--(pgraph->vertex[i].size);
				}
				else
					tmp = tmp->next;
			}
		}
		tmp_help = tmp_help->next;
	}
	struct Edge* tmp = pgraph->vertex[ind_del].root;
	while (tmp) {
		del_edge(&pgraph->vertex[ind_del].root, tmp);
		tmp = pgraph->vertex[ind_del].root;
	}
	for (int i = ind_del; i < pgraph->size - 1; ++i)
		pgraph->vertex[i] = pgraph->vertex[i + 1];
	--(pgraph->size);
	pgraph->vertex = (struct Vertex*)realloc(pgraph->vertex, sizeof(struct Vertex) * pgraph->size);
	return 0;
}

int DeleteAll(struct Graph* pgraph) {
	int ind_del = 0;
	while (ind_del < pgraph->size)
	{
		struct Edge* tmp = pgraph->vertex[ind_del].root;
		while (tmp) {
			tmp = del_edge(&pgraph->vertex[ind_del].root, tmp);
		}

		ind_del++;
	}
	free(pgraph->vertex);
	pgraph->size = 0;
	return 0;
}



int DeleteEV(struct Graph* pgraph, char* nameF_dot) {
	if (!pgraph->size) {
		printf("%s\n", errmsgs[4]);
		return 1;
	}
	int rc;
	printf("Input vertex name you want to delete: --> ");
	char* name = getStr();
	if (!name)
		return 0;
	rc = delete(pgraph, name);
	printf("%s\n", errmsgs[rc]);
	free(name);
	return 1;
}

int find(struct Graph* pgraph, char* name) {
	int ind_find;
	int ind;
	if (!pgraph->size)
		return 4;
	for (int i = 0; i < pgraph->size; ++i) {
		if (!strcmp(name, pgraph->vertex[i].Name)) {
			ind_find = i;		// Нашли нужную вершину
			break;
		}
		if (i == pgraph->size - 1)
			return 1;		// Вершина не найдена
	}
	struct Edge* tmp = pgraph->vertex[ind_find].root;
	struct Edge* tmp1;
	ind = 0;
	int clon = 0;
	//return 0;
	if (!tmp) {
		printf("  From vertex \"%d - %d %s\" edges comes to: ", pgraph->vertex[ind_find].x, pgraph->vertex[ind_find].y, pgraph->vertex[ind_find].Name);
		return 0;
	}
	while (tmp) { //для списка смежности вершины i
		for (int j = 0; j < pgraph->size; ++j) {
			//if (pLoop->flagLoop == 1) {
			tmp1 = pgraph->vertex[j].root; //для списка смежности вершины j
			while (tmp1) {
				if ((!strcmp(tmp->Name, tmp1->Name) && (clon == 0) && (tmp == tmp1))) {


					ind = j;
					printf("\"%d - %d %s\"  ", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
					clon = 1;
					break;
					//printf("petux\n");
				}
				if ((!strcmp(tmp->Name, tmp1->Name) && tmp != tmp1)) {

					ind = j;
					printf("\"%d - %d %s\"  ", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
					break;
					//printf("petux\n");
				}
				tmp1 = tmp1->next;
				//printf("cock\n");
			}
			//tmp1 = tmp1->next;
			//printf("chiken\n");

		}
		//printf("puppy\n");
		tmp = tmp->next;
		//return 0;
	}
	//printf("dog\n");
	return 0;
}

int find2(struct Graph* pgraph, char* name) {
	int ind_find;
	int ind;
	if (!pgraph->size)
		return 4;
	for (int i = 0; i < pgraph->size; ++i) {
		if (!strcmp(name, pgraph->vertex[i].Name)) {
			ind_find = i;		// Нашли нужную вершину
			break;
		}
		if (i == pgraph->size - 1)
			return 1;		// Вершина не найдена
	}
	struct Edge* tmp = pgraph->vertex[ind_find].root;
	struct Edge* tmp1;
	ind = 0;
	int clon = 0;
	//return 0;
	if (!tmp) {
		//printf("  From vertex \"%d - %d %s\" edges comes to: ", pgraph->vertex[ind_find].x, pgraph->vertex[ind_find].y, pgraph->vertex[ind_find].Name);
		return 0;
	}
	while (tmp) { //для списка смежности вершины i
		for (int j = 0; j < pgraph->size; ++j) {
			//if (pLoop->flagLoop == 1) {
			tmp1 = pgraph->vertex[j].root; //для списка смежности вершины j
			while (tmp1) {
				if ((!strcmp(tmp->Name, tmp1->Name) && (clon == 0) && (tmp == tmp1))) {


					ind = j;
					//printf("\"%d - %d %s\"  ", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
					clon = 1;
					break;
					//printf("petux\n");
				}
				if ((!strcmp(tmp->Name, tmp1->Name) && tmp != tmp1)) {

					ind = j;
					//printf("\"%d - %d %s\"  ", pgraph->vertex[ind].x, pgraph->vertex[ind].y, pgraph->vertex[ind].Name);
					break;
					//printf("petux\n");
				}
				tmp1 = tmp1->next;
				//printf("cock\n");
			}
			//tmp1 = tmp1->next;
			//printf("chiken\n");

		}
		//printf("puppy\n");
		tmp = tmp->next;
		//return 0;
	}
	//printf("dog\n");
	return 0;
}

int Random(struct Graph* pgraph, char* nameF_dot)
{


	int n = 10, cnt = 1, k = 0, m = 0;
	//	clock_t first, last;
		//srand(time(NULL));
	while (n-- > 0) {
		/* Райндомайз элементов */
		for (int i = 0; i < cnt; ++i) {
			int x = rand() * rand();
			int y = rand() * rand();
			char* name = (char*)(calloc(11, 1));
			for (int j = 0; j < 10; ++j)
				name[j] = (char)(rand() % (26) + 97);
			name[10] = '\0';
			insert_graph(pgraph, name, x, y, -1);
		}
		int ind_in = 0, ind_out = 0;
		for (int i = 0; i < cnt * (11 - n); ++i) {
			ind_in = rand() % (cnt * (10 - n));
			ind_out = rand() % (cnt * (10 - n));
			char* Edgename = (char*)(calloc(11, 1));
			for (int j = 0; j < 10; ++j)
				Edgename[j] = (char)(rand() % (26) + 97);
			Edgename[10] = '\0';
			insert_edge(pgraph, ind_out, pgraph->vertex[ind_in].Name, Edgename, 1);
		}
		/*first = clock();
		for (int i = 0; i < ((10 - n) * cnt); ++i) {
			int x = rand() * rand();
			int y = rand() * rand();
			char* name = (char*)(malloc(11));
			for (int j = 0; j < 10; ++j)
				name[j] = rand() % (26) + 97;
			name[10] = '\0';
			insert_graph(pgraph, name, x, y, -1);
		}
		last = clock();
	//	printf("test #%d, number of vertices added = %d, time = %lu\n", 10 - n, (10 - n) * cnt, (last - first));
	*/
	}
	//DeleteAll(pgraph);
	return 1;
}
/* Удаление графа из оперативной памяти. */


int FindV(struct Graph* pgraph, char* nameF_dot)
{

	printf("find\n");
	if (!pgraph->size) {
		printf("%s\n", errmsgs[2]);
		return 1;
	}
	int rc;
	printf("Input vertex name you want to find: --> ");
	char* name = getStr();
	if (!name)
		return 0;
	rc = find(pgraph, name);
	printf("%s\n", errmsgs[rc]);
	free(name);
	return 1;
}

int Time(struct Graph* pgraph, char* nameF_dot)
{
	struct Graph  graph = { 0, NULL };
	//graph = (struct Graph*)malloc(sizeof(struct Graph));
	//graph->fd = NULL;
	//graph->size = 0;
	//graph->vertex = NULL;
	int n = 10, cnt = 1000, k = 0, m = 0;
	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0) {
		/* Райндомайз элементов */
		first = clock();
		for (int i = 0; i < ((10 - n) * cnt); ++i) {
			int x = rand() * rand();
			int y = rand() * rand();
			char* name = (char*)(malloc(11));
			for (int j = 0; j < 10; ++j)
				name[j] = rand() % (26) + 97;
			name[10] = '\0';
			insert_graph(&graph, name, x, y, -1);
		}

		last = clock();
		printf("test #%d, number of vertices added = %d, time = %lu\n", 10 - n, (10 - n) * cnt, (last - first));
	}
	DeleteAll(&graph);
	return 1;
}

char* genName(int n) {
	char* res = (char*)calloc(n + 1, 1);
	for (int j = 0; j < n; ++j) {
		//printf("TIME: %d\n", time(NULL));
		res[j] = (char)(rand() % 26 + 97);
		//printf("%c\n", res[j]);
	}
	res[n] = '\0';
	return res;
}



int (*fptr[])(struct Graph*, char* nameF_dot) = { NULL, AddV, AddE, DeleteEV, FindV, Show, Time, Random, FileF/*AddE,DelV, FindV, ShowV  Add, Find, Delete, Show */ };// массив указателей на функции


int main()
{
	struct	Graph graph = { 0, NULL };
	srand(time(NULL));
	//table.n = 0;
//	ptable->n = 0;// зануление счетчика ключей

	char* nameF = (char*)(calloc(256, 1));
	printf(" Enter file \n");
	scanf("%s[^\n]", nameF);
	char* nameF_dot = (char*)(calloc(256, 1));
	//char* nameF_txt = (char*)(calloc(256, 1));

	//strcat(nameF_txt, nameF);
	strcat(nameF_dot, nameF);

	//strcat(nameF_txt, ".txt");
	strcat(nameF_dot, ".dot");

	FILE* fd = fopen(nameF_dot, "a+");
	char* buff = (char*)calloc(256, 1);
	if (fd == NULL) {
		printf("Error");
		free(nameF);
		return 0;
	}
	fseek(fd, 0, SEEK_END);
	long pos = ftell(fd);
	if (pos > 0) {
		fseek(fd, 0, SEEK_SET);
		fscanf(fd, "%[^\n]\n", buff);
		memset(buff, 0, 256);
		while (!feof(fd)) {
			int x, y, x2, y2, ind = 0, rc = 0;
			//int ind;
			fgets(buff, 256, fd);
			if (strstr(buff, "}"))
			{
				printf("end\n");
				break;
			}
			else if (strstr(buff, "--")) {
				//int ind = 0;

				//printf("HH\n");
				char* Edgename = genName(5);
				//*Edgename =(char)( rand() % (26) + 97);
				printf("%s \n", Edgename);
				//Edgename = rand() * rand();
				char* name = (char*)calloc(256, 1);
				char* name2 = (char*)calloc(256, 1);
				sscanf(buff, "\" %d - %d %s \" -- \" %d - %d %s \";[^\n]", &x, &y, name, &x2, &y2, name2);
				printf("%d %d %s -- %d %d %s\n", x, y, name, x2, y2, name2);

				insert_graph(&graph, name, x, y, -1);
				insert_graph(&graph, name2, x2, y2, -1);
				for (int i = 0; i < graph.size; ++i) {
					if (!strcmp(name, graph.vertex[i].Name)) {
						ind = i;		// Нашли нужную вершину

						printf("%d \n", i);
						break;
					}
					//	if (i == graph.size - 1)
					//		return 1;		// Вершина не найдена
				}
				insert_edge(&graph, ind, name2, Edgename, 1);
				//ind = 0;
				x = 0;
				y = 0;
				x2 = 0;
				y2 = 0;
				memset(buff, 0, 256);
			}
			else
			{
				char* name = (char*)calloc(256, 1);
				sscanf(buff, "\"%d - %d %s \";[^\n]", &x, &y, name);
				if (strstr(buff, "--")) printf("-- exists\n");
				printf("%d %d %s\n", x, y, name);
				insert_graph(&graph, name, x, y, -1);
				x = 0;
				y = 0;
				memset(buff, 0, 256);
			}
		}
	}
	fclose(fd);


	int rc = 0;// объявление переменной для вызова функций
	while (rc = dialog(msgs, NMsgs)) {// цикл пока rc не будет равно 0
		if (!fptr[rc](&graph, nameF_dot)) {// пока не выбрали NULL
			break;//
		}
	}
	printf("That's all. Bye!\n");
	free(nameF);
	DeleteAll(&graph);
	//delTable(ptable);// очистка
	return 0;
}