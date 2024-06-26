#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct data {
	char name[80];		 // Имя
	char surname[80];	 // Фамилия
	char patronymic[80]; // Отчество
	int groupNumber;	 // Номер группы
	float avrMark;		 // Средний балл успеваемости
	int physics;		 // Запись на физику
	int programing;		 // Запись на программирование
	int english;		 // Запись на английский язык
	int dataBase;		 // Запись на базы данных
};

struct accounts {
	char username[80];	 // Имя пользователя
	char password[80];	 // Пароль
	char role[6];		 // Роль
};

struct data* data;
struct accounts* account;
int isAdmin = 0, isLoged = 0;	// Роль пользователя и состояние авторизации

void enterToContinue(void);
FILE* openFile(char* filename, char* mode);
void safeScan(char *text, char *format, void *var);
void makeDefaultAccount(void);
void isThereAccounts(void);
const char* isEnrolled(int l);
int login(void);
int youSure(void);
int isUsernameExists(char* name);
void enterUsername(char* username);
int isSurnameExists(char* name);
void enterRole(char* role);
void enterGroupNumber(int* number);
void enterAvrMark(float* mark);
void addAccount(void);
void viewAccount(void);
void editAccount(void);
void deleteAccount(void);
void printData(void);
void viewData(void);
void addData(void);
void deleteData(void);
void getEnrollment(char* subject, int* enrollment);
const char* getCourseName(int i);
void editData(void);
void printCourseStudents(int course);
void printCoursePopularity(void);
void searchByField(char* searchStr);
void searchByAvrMark(void);
void searchByGroupNumber(void);
void sortBySurname(void);
void sortByGroupNumber(void);
void sortByAvrMark(void);
void userMenu(void);
void adminMenu(void);
void accountManagementMenu(void);
void workingWithDataMenu(void);
void editModeMenu(void);
void dataProcessingMenu(void);
void individualTaskMenu(void);
void choiceOfCourseMenu(void);
void searchDataMenu(void);
void sortingDataMenu(void);
int main(void);

int main(void) {	// Основная ф-я
	system("chcp 1251");	// Установка кодовой страницы для русского языка
	system("cls");
	setlocale(LC_ALL, "Rus");	// Установка русской локали
	isThereAccounts();
	while(!isLoged)
		if(login())
	if(isAdmin)
		adminMenu();
	else
		userMenu();
	return 0;
}

void enterToContinue(void) {	// Ф-я, прерывающая выполнение программы до нажатия любой клавиши
	system("pause");
	system("cls");
}

FILE* openFile(char* filename, char* mode) {	// Ф-я, открывающая файлы
	FILE* file = fopen(filename, mode);
	if (file == NULL) {
		system("cls");
		printf("Не удалось открыть файл %s\n", filename);
		enterToContinue();
		return NULL;
	}
	return file;
}

void safeScan(char *text, char *format, void *var) {	// Ф-я для вывода текста и защиты от ввода символов неверного формата
	int result;
	do {
		printf("%s", text);
		while ((result = scanf(format, var)) != 1) {
			while (getchar() != '\n');	// очистка буфера ввода
			printf("Неверный формат!\n\n");
			printf("%s", text);
		}
	} while(result != 1);
}

void makeDefaultAccount(void) {	// Ф-я, создающая файл с аккаунтом по умолчанию
	FILE* file = openFile("accounts.dat", "ab");
	if(!file) {
		printf("Ошибка создания файла!");
		return;
	}
	account = malloc(sizeof(struct accounts));
	strcpy(account->username, "admin");	// Ввод в файл имени по умолчанию
	strcpy(account->password, "admin");	// Ввод в файл пароля по умолчанию
	strcpy(account->role, "admin");		// Ввод в файл роли по умолчанию
	fwrite(account, sizeof(struct accounts), 1, file);
	fclose(file);
	free(account);
	printf("Создан файл accounts.dat с учётной записью по умолчанию.\n");
	enterToContinue();
}

void isThereAccounts(void) {	// Ф-я, проверяющая наличие аккаунтов в файле
	FILE* file;
	if(!(file = openFile("accounts.dat", "rb")))
		makeDefaultAccount();
	fclose(file);
}

const char* isEnrolled(int l) {	// Ф-я, выводящая строки в соответствии с записью студента
	const char* yes = "записан";
	const char* no = "не записан";
	const char* err = "ошибка";
	if(l == 1)
		return yes;
	else if(l == 0)
		return no;
	else
		return err;
}

int login(void) {	// Ф-я входа в учётную запись
	char inputUsername[80];
	char inputPassword[80];
	system("cls");
	safeScan("Введите имя пользователя: ", "%s", inputUsername);
	safeScan("Введите пароль: ", "%s", inputPassword);
	FILE* file = openFile("accounts.dat", "rb");
	if (!file)
		return 0;
	account = (struct accounts*)malloc(sizeof(struct accounts));
	while (fread(account, sizeof(struct accounts), 1, file)) {
		if (strcmp(account->username, inputUsername) == 0 && strcmp(account->password, inputPassword) == 0) {
			isLoged = 1;
			if (strcmp(account->role, "admin") == 0) {
				isAdmin = 1;
				printf("Вы вошли как администратор.\n\n");
				enterToContinue();
			} else {
				printf("Вы вошли как пользователь.\n\n");
				enterToContinue();
			}
			free(account);
			fclose(file);
			return 1;
		}
	}
	if(!isLoged) {
		printf("Неверный логин или пароль!\n\n");
		enterToContinue();
		free(account);
		fclose(file);
		return 0;
	}
}

int youSure(void) {	// Ф-я, запрашивающая подтверждение у пользователя
	int option;
	system("cls");
	while(1) {
		printf("Вы действительно хотите удалить данные?\n");
		printf("1. Да;\n");
		printf("2. Нет.\n");
		safeScan("Выберите вариант ответа: ", "%d", &option);
		if(option == 1)
			return 1;
		else if(option == 2) {
			system("cls");
    		printf("Операция удаления отменена.\n");
    		enterToContinue();
			return 0;
		}
		else {
			system("cls");
			printf("Введите 1 или 2!\n");
			enterToContinue();
		}
	}
	system("cls");
}

int isUsernameExists(char* name) {	// Ф-я, проверяющая наличие имени пользователя
    FILE* file = openFile("accounts.dat", "rb");
    if (!file)
        return 0;
    struct accounts* account = (struct accounts*)malloc(sizeof(struct accounts));
    while(fread(account, sizeof(struct accounts), 1, file)) {
        if (strcmp(account->username, name) == 0) {
            free(account);
            fclose(file);
            return 1;
        }
    }
    free(account);
    fclose(file);
    return 0;
}

void enterUsername(char* username) {	// Ф-я ввода имени пользователя
	while(1) {
		safeScan("Введите имя пользователя: ", "%s", username);
		if (isUsernameExists(username)) {
			system("cls");
			printf("Пользователь с таким именем уже существует.\n\n");
			enterToContinue();
		} else
			break;
	}
}

int isSurnameExists(char* name) {	// Ф-я, проверяющая наличие фамилии
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return 0;
    struct data* data = (struct data*)malloc(sizeof(struct data));
    while(fread(data, sizeof(struct data), 1, file)) {
        if (strcmp(data->surname, name) == 0) {
            free(data);
            fclose(file);
            return 1;
        }
    }
    free(data);
    fclose(file);
    return 0;
}

void enterRole(char* role) {	// Ф-я ввода роли
	while (1) {
		safeScan("Введите роль: ", "%s", role);
		if (strcmp(role, "admin") == 0 || strcmp(role, "user") == 0)
			break;
		else {
			system("cls");
			printf("Введите admin или user!\n\n");
			enterToContinue();
		}
	}
}

void addAccount(void) {	// Ф-я добавления учётной записи
	FILE* file = openFile("accounts.dat", "ab");
	if (!file)
		return;
	account = (struct accounts*)malloc(sizeof(struct accounts));
	system("cls");
	enterUsername(account->username);
	safeScan("Введите пароль: ", "%s", account->password);
	enterRole(account->role);
	fwrite(account, sizeof(struct accounts), 1, file);
	fclose(file);
	free(account);
	printf("\nАккаунт успешно создан.\n\n");
	enterToContinue();
}

void viewAccount(void) {	// Ф-я вывода учётных записей
	FILE* file = openFile("accounts.dat", "rb");
	if (!file)
		return;
	account = (struct accounts*)malloc(sizeof(struct accounts));
	system("cls");
	printf("Список учётных записей:\n\n");
	while(fread(account, sizeof(struct accounts), 1, file))
		printf("Имя пользователя: %s\nПароль: %s\nРоль: %s\n\n", account->username, account->password, account->role);
	fclose(file);
	free(account);
	enterToContinue();
}

void editAccount(void) {	// Ф-я редактирования учётной записи
	char username[80];
	FILE* file = openFile("accounts.dat", "rb+");	// rb+ т.к. нужно и считывать и записывать данные
	if (!file)
		return;
	struct accounts* tempAccount = (struct accounts*)malloc(sizeof(struct accounts));
	system("cls");
	safeScan("Введите имя пользователя для редактирования: ", "%s", username);
	if (!isUsernameExists(username)) {
		system("cls");
		printf("Пользователь с таким именем не найден.\n\n");
		enterToContinue();
		fclose(file);
		free(tempAccount);
		return;
	}
	while(fread(tempAccount, sizeof(struct accounts), 1, file)) {
		if (strcmp(tempAccount->username, username) == 0) {
			printf("\nВведите новые данные для %s:\n", username);
			enterUsername(tempAccount->username);
			safeScan("Введите пароль: ", "%s", tempAccount->password);
			enterRole(tempAccount->role);
			fseek(file, -sizeof(struct accounts), SEEK_CUR);	// перемещение указателя назад на размер структуры от текущей позиции
			fwrite(tempAccount, sizeof(struct accounts), 1, file);	// запись одного объекта в файл
			break;
		}
	}
	fclose(file);
	free(tempAccount);
	printf("\nДанные аккаунта успешно обновлены.\n\n");
	enterToContinue();
}

void deleteAccount(void) {	// Ф-я удаления учётной записи
    char username[80];
    FILE* file = openFile("accounts.dat", "rb");
    if (!file)
        return;
    struct accounts* tempAccount = (struct accounts*)malloc(sizeof(struct accounts));
    system("cls");
    safeScan("Введите имя пользователя для удаления: ", "%s", username);
    int userExists = 0;
    while(fread(tempAccount, sizeof(struct accounts), 1, file))
        if (strcmp(tempAccount->username, username) == 0)
            userExists = 1;
    fclose(file);
    if (!userExists) {
    	system("cls");
        printf("Ошибка: Пользователь с введенным именем не найден.\n\n");
        free(tempAccount);
        enterToContinue();
        return;
    }
    if(!youSure()) {
    	free(tempAccount);
		return;
	}
    FILE* tempFile = openFile("temp.dat", "wb");
    if (!tempFile) {
    	free(tempAccount);
        return;
	}
    file = openFile("accounts.dat", "rb");
    while(fread(tempAccount, sizeof(struct accounts), 1, file))
		if (strcmp(tempAccount->username, username) != 0)
            fwrite(tempAccount, sizeof(struct accounts), 1, tempFile);
    fclose(file);
    fclose(tempFile);
    remove("accounts.dat");
    rename("temp.dat", "accounts.dat");
    free(tempAccount);
    printf("\nАккаунт успешно удален.\n\n");
    enterToContinue();
}

void enterSurname(char* surname) {	// Ф-я ввода фамилии студента
	while(1) {
		safeScan("Введите фамилию: ", "%s", surname);
		if (isSurnameExists(surname)) {
			system("cls");
			printf("Студент с такой фамилией уже существует.\n\n");
			enterToContinue();
		} else
			break;
	}
}

void enterGroupNumber(int* number) {	// Ф-я ввода номера группы
    while (1) {
        safeScan("Введите номер группы: ", "%d", number);
        if (*number >= 1)
            break;
        else {
            printf("\nВведите корректные данные!\n\n");
        }
    }
}

void enterAvrMark(float* mark) {	// Ф-я ввода среднего балла успеваемости
    while (1) {
        safeScan("Введите средний балл: ", "%f", mark);
        if (*mark >= 1 && *mark <= 10)
            break;
        else {
            printf("\nВведите корректные данные!\n\n");
        }
    }
}

void printData(void) {	// Ф-я вывода данных о студенте
	printf("ФИО: %s %s %s\n", data->surname, data->name, data->patronymic);
	printf("Номер группы: %d\n", data->groupNumber);
	printf("Средний балл: %0.2f\n", data->avrMark);
	printf("Физика: %s\n", isEnrolled(data->physics));
	printf("Программирование: %s\n", isEnrolled(data->programing));
	printf("Английский язык: %s\n", isEnrolled(data->english));
	printf("Базы данных: %s\n\n", isEnrolled(data->dataBase));
}

void viewData(void) {	// Ф-я вывода данных о студенте
	FILE* file = openFile("data.dat", "rb");
	if (!file)
		return;
	data = (struct data*)malloc(sizeof(struct data));
	system("cls");
	while(fread(data, sizeof(struct data), 1, file)) {
		printData();
	}
	fclose(file);
	free(data);
	enterToContinue();
}

void addData(void) {	// Ф-я добавления данных о студенте
	FILE* file = openFile("data.dat", "ab");
	if (!file)
		return;
	data = (struct data*)malloc(sizeof(struct data));
	system("cls");
	enterSurname(data->surname);
	safeScan("Введите имя: ", "%s", data->name);
	safeScan("Введите отчество: ", "%s", data->patronymic);
	enterGroupNumber(&(data->groupNumber));
	enterAvrMark(&(data->avrMark));
	getEnrollment("физику", &(data->physics));
	getEnrollment("программирование", &(data->programing));
	getEnrollment("английский язык", &(data->english));					// Указаель на указатель?
	getEnrollment("базы данных", &(data->dataBase));
	fwrite(data, sizeof(struct data), 1, file);
	fclose(file);
	free(data);
	printf("\nДанные о студенте успешно введены.\n\n");
	enterToContinue();
}

void deleteData(void) {	// Ф-я удаления данных о студенте
    char surname[80];
    FILE* tempFile = openFile("temp.dat", "wb");
    if (!tempFile)
        return;
    FILE* file = openFile("data.dat", "rb");
    if (!file) {
    	fclose(tempFile);
        return;
	}
    struct data* tempData = (struct data*)malloc(sizeof(struct data));
    system("cls");
    safeScan("Введите фамилию студента для удаления: ", "%s", surname);
    int dataExists = 0;
    while (fread(tempData, sizeof(struct data), 1, file))
        if (strcmp(tempData->surname, surname) == 0) {
            dataExists = 1;
            break; // Найден студент, выходим из цикла
        }
    fclose(file);
    if (!dataExists) {
        system("cls");
        printf("Ошибка: Студент с введенной фамилией не найден.\n\n");
        free(tempData);
        fclose(tempFile);
        fclose(file);
        enterToContinue();
        return;
    }
    if (!youSure()) {
        fclose(tempFile);
        fclose(file);
        free(tempData);
        return;
    }
    file = openFile("data.dat", "rb"); // Открываем файл заново
    while (fread(tempData, sizeof(struct data), 1, file))
        if (strcmp(tempData->surname, surname) != 0)
            fwrite(tempData, sizeof(struct data), 1, tempFile);
    fclose(file);
    fclose(tempFile);
    remove("data.dat");
    rename("temp.dat", "data.dat");
    free(tempData);
    printf("\nСтудент успешно удален.\n\n");
    enterToContinue();
}

void getEnrollment(char* subject, int* enrollment) {	// Ф-я ввода записи на факультатив
    while (1) {
        printf("Запись на %s (1 - да, 0 - нет): ", subject);
        safeScan("", "%d", enrollment);
        if (*enrollment == 0 || *enrollment == 1)
            break;
        else
            printf("Введите 0 или 1!\n");
    }
}

const char* getCourseName(int i) {	// Функция, возвращающая название факультатива
	const char* physics = "физика";
	const char* programming = "программирование";
	const char* english = "английский язык";
	const char* dataBase = "базы данных";
	switch(i) {
		case 1:
			return physics;
			break;
		case 2:
			return programming;
			break;
		case 3:
			return english;
			break;
		case 4:
			return dataBase;
			break;
	}
}

void editData(void) {	// Ф-я редактирования данных
    FILE* file = openFile("data.dat", "rb+");
    if (!file)
        return;
    char searchSurname[80];
    system("cls");
    safeScan("Введите фамилию студента, данные о котором нужно заменить: ", "%s", searchSurname);
	if (!isSurnameExists(searchSurname)) {
		system("cls");
		printf("Пользователь с такой фамилей не найден.\n\n");
		enterToContinue();
		fclose(file);
		return;
	}
    data = (struct data*)malloc(sizeof(struct data));
    while (fread(data, sizeof(struct data), 1, file)) {
        if (strcmp(data->surname, searchSurname) == 0) {
            printf("Введите новые данные для студента %s:\n", data->surname);
            enterSurname(data->surname);
            safeScan("Введите имя: ", "%s", data->name);
            safeScan("Введите отчество: ", "%s", data->patronymic);
            enterGroupNumber(&(data->groupNumber));
            enterAvrMark(&(data->avrMark));
            getEnrollment("физику", &(data->physics));
            getEnrollment("программирование", &(data->programing));
            getEnrollment("английский язык", &(data->english));
            getEnrollment("базы данных", &(data->dataBase));
            fseek(file, -sizeof(struct data), SEEK_CUR);	// перемещение указателя назад на размер структуры от текущей позиции
            fwrite(data, sizeof(struct data), 1, file);	// запись одного объекта в файл
            printf("\nДанные о студенте успешно обновлены.\n\n");
            break;
        }
    }
    fclose(file);
    free(data);
    enterToContinue();
}

void printCourseStudents(int course) {	// Ф-я вывода студентов, записанных на факультативное занятие
    struct data* data = (struct data*)malloc(sizeof(struct data));
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
    struct data students[100], temp;
    int numStudents = 0;
    while (fread(&students[numStudents], sizeof(struct data), 1, file)) {	// Подсчет кол-ва студентов на факультативе
        numStudents++;
    }
    fclose(file);
    for (int i = 0; i < numStudents - 1; i++) {	// Сортировка студентов по среднему баллу успеваемости
        for (int j = i + 1; j < numStudents; j++) {
            if (students[i].avrMark < students[j].avrMark) {
                temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
    system("cls");
    printf("Студенты, записанные только на факультативное занятие \"%s\":\n", getCourseName(course));
    for (int i = 0; i < 15; i++) {
        if ((course == 1 && students[i].physics == 1) ||
            (course == 2 && students[i].programing == 1) ||
            (course == 3 && students[i].english == 1) ||
            (course == 4 && students[i].dataBase == 1)) {
            printf("%s %s %s\n", students[i].surname, students[i].name, students[i].patronymic);
        }
    }
    printf("\n");
    enterToContinue();
}

void printCoursePopularity(void) {	// Вывод общего кол-ва студентов, записанных на факультативы
    int physicsCount = 0, programingCount = 0, englishCount = 0, dataBaseCount = 0;
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
	struct data* data = (struct data*)malloc(sizeof(struct data));
    while(fread(data, sizeof(struct data), 1, file)) {
        physicsCount += data->physics;
        programingCount += data->programing;
        englishCount += data->english;
        dataBaseCount += data->dataBase;
    }
    fclose(file);
	system("cls");
    printf("Популярность факультативных занятий:\n");
    printf("Физика: %d\n", physicsCount);
    printf("Программирование: %d\n", programingCount);			
    printf("Английский язык: %d\n", englishCount);
    printf("Базы данных: %d\n\n", dataBaseCount);
    fclose(file);
    free(data);
    enterToContinue();
}

void searchByField(char* searchStr) {	// Ф-я  поиска данных по ФИО
    char searchValue[80];
    int found = 0;
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
	data = (struct data*)malloc(sizeof(struct data));
	system("cls");
    safeScan("Введите строку для поиска: ", "%s", searchValue);
    while(fread(data, sizeof(struct data), 1, file)) {
        if(strcmp(searchStr, "name") == 0 && strcmp(data->name, searchValue) == 0) {
            printData();
            found = 1;
        }
        else if(strcmp(searchStr, "surname") == 0 && strcmp(data->surname, searchValue) == 0) {
            printData();
            found = 1;
        }
        else if(strcmp(searchStr, "patronymic") == 0 && strcmp(data->patronymic, searchValue) == 0) {
            printData();
            found = 1;
        }
    }
    if(!found) {
    	system("cls");
        printf("Студент не найден.\n\n");
    }
    fclose(file);
    free(data);
    enterToContinue();
}

void searchByAvrMark(void) {	// Ф-я поиска по среднему баллу успеваемости
    float avrMark;
    int found = 0;
    system("cls");
    safeScan("Введите средний балл для поиска: ", "%f", &avrMark);
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
	data = (struct data*)malloc(sizeof(struct data));
    while(fread(data, sizeof(struct data), 1, file))
        if(data->avrMark >= avrMark) {
            printData();
            found = 1;
        }
    fclose(file);
    free(data);
    if(!found) {
    	system("cls");
        printf("Студент не найден.\n\n");
    }
    enterToContinue();
}

void searchByGroupNumber(void) {	// Ф-я поиска по номеру группы
    int groupNumber;
    int found = 0;
	system("cls");
    safeScan("Введите номер группы для поиска: ", "%d", &groupNumber);
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
	data = (struct data*)malloc(sizeof(struct data));
    while(fread(data, sizeof(struct data), 1, file))
        if(data->groupNumber == groupNumber) {
            printData();
            found = 1;
        }
    fclose(file);
    free(data);
    if(!found) {
    	system("cls");
        printf("Студент не найден.\n\n");
    }
    enterToContinue();
}

void sortBySurname(void) {	// Ф-я сортировки по фамилии
    struct data* dataArray = NULL;
    int n = 0;
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
    while (fread(&data, sizeof(struct data), 1, file))	// Подсчет кол-ва записей
        n++;
    dataArray = (struct data*)malloc(n * sizeof(struct data));
    rewind(file);	// Переход в начало файла
    for (int i = 0; i < n; i++)
        fread(&dataArray[i], sizeof(struct data), 1, file);
    fclose(file);
    for (int i = 0; i < n - 1; i++)	// Сортировка
		for (int j = 0; j < n - i - 1; j++)
			if (strcmp(dataArray[j].surname, dataArray[j + 1].surname) > 0) {
				struct data temp = dataArray[j];
				dataArray[j] = dataArray[j + 1];
				dataArray[j + 1] = temp;
            }
    file = openFile("data.dat", "wb");
    if (!file) {
        free(dataArray);
        return;
    }
    for (int i = 0; i < n; i++)
        fwrite(&dataArray[i], sizeof(struct data), 1, file);	// Запись отсортированных данных в файл
    fclose(file);
    free(dataArray);
	system("cls");
    printf("Данные успешно отсортированы по фамилии.\n\n");
    enterToContinue();
}

void sortByGroupNumber(void) {	// Ф-я сортировки по номеру группы
    struct data* dataArray = NULL;
    int n = 0;
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
    while (fread(&data, sizeof(struct data), 1, file))	// Подсчет кол-ва записей
        n++;
    dataArray = (struct data*)malloc(n * sizeof(struct data));
    rewind(file);	// Переход в начало файла
    for (int i = 0; i < n; i++)
        fread(&dataArray[i], sizeof(struct data), 1, file);
    fclose(file);
    for (int i = 0; i < n - 1; i++)	// Сортировка
        for (int j = 0; j < n - i - 1; j++)
            if (dataArray[j].groupNumber > dataArray[j + 1].groupNumber) {
                struct data temp = dataArray[j];
                dataArray[j] = dataArray[j + 1];
                dataArray[j + 1] = temp;
            }
    file = openFile("data.dat", "wb");
    if (!file) {
        free(dataArray);
        return;
    }
    for (int i = 0; i < n; i++)
        fwrite(&dataArray[i], sizeof(struct data), 1, file);	// Запись отсортированных данных в файл
    fclose(file);
    free(dataArray);
	system("cls");
    printf("Данные успешно отсортированы по номеру группы.\n\n");
    enterToContinue();
}

void sortByAvrMark(void) {	// Ф-я сортировки по среднему баллу успеваемости
    struct data* dataArray = NULL;
    int n = 0;
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
    while (fread(&data, sizeof(struct data), 1, file))	// Подсчет кол-ва записей
        n++;
    dataArray = (struct data*)malloc(n * sizeof(struct data));
    rewind(file);	// Переход в начало файла
    for (int i = 0; i < n; i++)
        fread(&dataArray[i], sizeof(struct data), 1, file);
    fclose(file);
    for (int i = 0; i < n - 1; i++)	// Сортировка
        for (int j = 0; j < n - i - 1; j++)
            if (dataArray[j].avrMark > dataArray[j + 1].avrMark) {
                struct data temp = dataArray[j];
                dataArray[j] = dataArray[j + 1];
                dataArray[j + 1] = temp;
            }
    file = openFile("data.dat", "wb");
    if (!file) {
        free(dataArray);
        return;
    }
    for (int i = 0; i < n; i++)
        fwrite(&dataArray[i], sizeof(struct data), 1, file);	// Запись отсортированных данных в файл
    fclose(file);
    free(dataArray);
	system("cls");
    printf("Данные успешно отсортированы по успеваемости.\n\n");
    enterToContinue();
}

void userMenu(void) {	// Ф-я меню пользователя
	system("cls");
	int option = 0;
	while(1) {
		printf("Главное меню:\n");
		printf("1. Просмотр всех данных;\n");
		printf("2. Выполнение индивидуального задания;\n");
		printf("3. Поиск данных;\n");
		printf("4. сортировка;\n");
		printf("5. Выход.\n\n");
		safeScan("Введите номер пункта: ", "%d", &option);
		switch(option) {
			case 1:
				viewData();
				break;
			case 2:
				individualTaskMenu();
				break;
			case 3:
				searchDataMenu();
				break;
			case 4:
				sortingDataMenu();
				break;
			case 5:
				system("cls");
				return;
			default:
				system("cls");
				printf("\nВведите верный номер пункта!\n\n");
				enterToContinue();
		}
	}
}

void adminMenu(void) {	// Ф-я меню администратора
	system("cls");
	int option = 0;
	while(1) {
		printf("Главное меню:\n");
		printf("1. Управление учётными записями пользователей;\n");
		printf("2. Работа с данными;\n");
		printf("3. Выход.\n\n");
		safeScan("Введите номер пункта: ", "%d", &option);
		switch (option) {
			case 1:
				accountManagementMenu();
				break;
			case 2:
				workingWithDataMenu();
				break;
			case 3:
				system("cls");
				return;
			default:
				system("cls");
				printf("\nВведите верный номер пункта!\n\n");
				enterToContinue();
		}
	}
}

void accountManagementMenu(void) {	// Ф-я меню управления учётными записями пользователей
	system("cls");
	int option = 0;
	while(1) {
		printf("Управление учётными записями пользователей:\n");
		printf("1. Просмотр всех учётных записей;\n");
		printf("2. Добавление новой учётной записи;\n");
		printf("3. Редактирование учётной записи;\n");
		printf("4. Удаление учётной записи;\n");
		printf("5. Выход.\n\n");
		safeScan("Введите номер пункта: ", "%d", &option);
		getchar();											//зачем?
		switch (option) {
			case 1:
				viewAccount();
				break;
			case 2:
				addAccount();
				break;
			case 3:
				editAccount();
				break;
			case 4:
				deleteAccount();
				break;
			case 5:
				system("cls");
				return;
			default:
				system("cls");
				printf("\nВведите верный номер пункта!\n\n");
				enterToContinue();
		}
	}
}

void workingWithDataMenu(void) {	// Ф-я меню работы с данными
	system("cls");
	int option = 0;
	while(1) {
		printf("Работа с данными:\n");
		printf("1. Режим редактирования;\n");
		printf("2. Режим обработки данных;\n");
		printf("3. Выход.\n\n");
		safeScan("Введите номер пункта: ", "%d", &option);
		switch(option) {
			case 1:
				editModeMenu();
				break;
			case 2:
				dataProcessingMenu();
				break;
			case 3:
				system("cls");
				return;
			default:
				system("cls");
				printf("\nВведите верный номер пункта!\n\n");
				enterToContinue();
		}
	}
}

void editModeMenu(void) {	// Ф-я меню режима редактирования
	system("cls");
	int option = 0;
	while(1) {
		printf("Режим редактирования:\n");
		printf("1. Просмотр всех данных;\n");
		printf("2. Добавление новой записи;\n");
		printf("3. Удаление записи;\n");
		printf("4. Редактирование записи;\n");
		printf("5. Выход.\n\n");
		safeScan("Введите номер пункта: ", "%d", &option);
		switch(option) {
			case 1:
				viewData();
				break;
			case 2:
				addData();
				break;
			case 3:
				deleteData();
				break;
			case 4:
				editData();
				break;
			case 5:
				system("cls");
				return;
			default:
				system("cls");
				printf("\nВведите верный номер пункта!\n\n");
				enterToContinue();
		}
	}
}

void dataProcessingMenu(void) {	// Ф-я меню обработки данных
	system("cls");
	int option = 0;
	while(1) {
		printf("Режим обработки данных:\n");
		printf("1. Выполнение индивидуального задания;\n");
		printf("2. Поиск данных;\n");
		printf("3. сортировка;\n");
		printf("4. Выход.\n\n");
		safeScan("Введите номер пункта: ", "%d", &option);
		switch(option) {
			case 1:
				individualTaskMenu();
				break;
			case 2:
				searchDataMenu();
				break;
			case 3:
				sortingDataMenu();
				break;
			case 4:
				system("cls");
				return;
			default:
				system("cls");
				printf("\nВведите верный номер пункта!\n\n");
				enterToContinue();
		}
	}
}

void individualTaskMenu(void) {	// Ф-я меню индивидуального задания
	system("cls");
	int option = 0;
	while(1) {
		printf("Выполнение индивидульного задания:\n");
		printf("1. Вывод студентов записанных на факультативное занятие;\n");
		printf("2. Общее количество студентов записанных на факультативные занятия;\n");
		printf("3. Выход.\n\n");
		safeScan("Введите номер пункта: ", "%d", &option);
		switch(option) {
			case 1:
				choiceOfCourseMenu();
				break;
			case 2:
				printCoursePopularity();
				break;
			case 3:
				system("cls");
				return;
			default:
				system("cls");
				printf("\nВведите верный номер пункта!\n\n");
				enterToContinue();
		}
	}
}

void choiceOfCourseMenu(void) {	// Ф-я меню выбора дисциплины
	system("cls");
	int option = 0;
	while(1) {
		printf("Выберите Факультативное занятие:\n");
		printf("1. Физика;\n");
		printf("2. Программирование;\n");
		printf("3. Английский язык;\n");
		printf("4. Базы данных;\n");
		printf("5. Выход.\n\n");
		safeScan("Введите номер пункта: ", "%d", &option);
		switch(option) {
			case 1:
				printCourseStudents(1);
				break;
			case 2:
				printCourseStudents(2);
				break;
			case 3:
				printCourseStudents(3);
				break;
			case 4:
				printCourseStudents(4);
				break;
			case 5:
				system("cls");
				return;
			default:
				system("cls");
				printf("\nВведите верный номер пункта!\n\n");
				enterToContinue();
		}
	}
}

void searchDataMenu(void) {	// Ф-я меню выбора параметра для поиска
	system("cls");
	int option = 0;
	while(1) {
		printf("Выберите параметр для поиска:\n");
		printf("1. Имя;\n");
		printf("2. Фамилия;\n");
		printf("3. Отчество;\n");
		printf("4. Номер группы;\n");
		printf("5. Срединий балл;\n");
		printf("6. Выход.\n\n");
		safeScan("Введите номер пункта: ", "%d", &option);
		switch(option) {
			case 1:
				searchByField("name");
				break;
			case 2:
				searchByField("surname");
				break;
			case 3:
				searchByField("patronymic");
				break;
			case 4:
				searchByGroupNumber();
				break;
			case 5:
				searchByAvrMark();
				break;
			case 6:
				system("cls");
				return;
			default:
				system("cls");
				printf("\nВведите верный номер пункта!\n\n");
				enterToContinue();
		}
	}
}

void sortingDataMenu(void) {	// Ф-я меню выбора параметра сортировки
	system("cls");
	int option = 0;
	while(1) {
		printf("Выберите параметр для сортировки:\n");
		printf("1. Фамилия;\n");
		printf("2. Номер группы;\n");
		printf("3. Успеваемость;\n");
		printf("4. Выход.\n\n");
		safeScan("Введите номер пункта: ", "%d", &option);
		switch(option) {
			case 1:
				sortBySurname();
				break;
			case 2:
				sortByGroupNumber();
				break;
			case 3:
				sortByAvrMark();
				break;
			case 4:
				system("cls");
				return;
			default:
				system("cls");
				printf("\nВведите верный номер пункта!\n\n");
				enterToContinue();
		}
	}
}
