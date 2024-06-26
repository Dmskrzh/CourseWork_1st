#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

struct data {
	char name[80];		 // ���
	char surname[80];	 // �������
	char patronymic[80]; // ��������
	int groupNumber;	 // ����� ������
	float avrMark;		 // ������� ���� ������������
	int physics;		 // ������ �� ������
	int programing;		 // ������ �� ����������������
	int english;		 // ������ �� ���������� ����
	int dataBase;		 // ������ �� ���� ������
};

struct accounts {
	char username[80];	 // ��� ������������
	char password[80];	 // ������
	char role[6];		 // ����
};

struct data* data;
struct accounts* account;
int isAdmin = 0, isLoged = 0;	// ���� ������������ � ��������� �����������

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

int main(void) {	// �������� �-�
	system("chcp 1251");	// ��������� ������� �������� ��� �������� �����
	system("cls");
	setlocale(LC_ALL, "Rus");	// ��������� ������� ������
	isThereAccounts();
	while(!isLoged)
		if(login())
	if(isAdmin)
		adminMenu();
	else
		userMenu();
	return 0;
}

void enterToContinue(void) {	// �-�, ����������� ���������� ��������� �� ������� ����� �������
	system("pause");
	system("cls");
}

FILE* openFile(char* filename, char* mode) {	// �-�, ����������� �����
	FILE* file = fopen(filename, mode);
	if (file == NULL) {
		system("cls");
		printf("�� ������� ������� ���� %s\n", filename);
		enterToContinue();
		return NULL;
	}
	return file;
}

void safeScan(char *text, char *format, void *var) {	// �-� ��� ������ ������ � ������ �� ����� �������� ��������� �������
	int result;
	do {
		printf("%s", text);
		while ((result = scanf(format, var)) != 1) {
			while (getchar() != '\n');	// ������� ������ �����
			printf("�������� ������!\n\n");
			printf("%s", text);
		}
	} while(result != 1);
}

void makeDefaultAccount(void) {	// �-�, ��������� ���� � ��������� �� ���������
	FILE* file = openFile("accounts.dat", "ab");
	if(!file) {
		printf("������ �������� �����!");
		return;
	}
	account = malloc(sizeof(struct accounts));
	strcpy(account->username, "admin");	// ���� � ���� ����� �� ���������
	strcpy(account->password, "admin");	// ���� � ���� ������ �� ���������
	strcpy(account->role, "admin");		// ���� � ���� ���� �� ���������
	fwrite(account, sizeof(struct accounts), 1, file);
	fclose(file);
	free(account);
	printf("������ ���� accounts.dat � ������� ������� �� ���������.\n");
	enterToContinue();
}

void isThereAccounts(void) {	// �-�, ����������� ������� ��������� � �����
	FILE* file;
	if(!(file = openFile("accounts.dat", "rb")))
		makeDefaultAccount();
	fclose(file);
}

const char* isEnrolled(int l) {	// �-�, ��������� ������ � ������������ � ������� ��������
	const char* yes = "�������";
	const char* no = "�� �������";
	const char* err = "������";
	if(l == 1)
		return yes;
	else if(l == 0)
		return no;
	else
		return err;
}

int login(void) {	// �-� ����� � ������� ������
	char inputUsername[80];
	char inputPassword[80];
	system("cls");
	safeScan("������� ��� ������������: ", "%s", inputUsername);
	safeScan("������� ������: ", "%s", inputPassword);
	FILE* file = openFile("accounts.dat", "rb");
	if (!file)
		return 0;
	account = (struct accounts*)malloc(sizeof(struct accounts));
	while (fread(account, sizeof(struct accounts), 1, file)) {
		if (strcmp(account->username, inputUsername) == 0 && strcmp(account->password, inputPassword) == 0) {
			isLoged = 1;
			if (strcmp(account->role, "admin") == 0) {
				isAdmin = 1;
				printf("�� ����� ��� �������������.\n\n");
				enterToContinue();
			} else {
				printf("�� ����� ��� ������������.\n\n");
				enterToContinue();
			}
			free(account);
			fclose(file);
			return 1;
		}
	}
	if(!isLoged) {
		printf("�������� ����� ��� ������!\n\n");
		enterToContinue();
		free(account);
		fclose(file);
		return 0;
	}
}

int youSure(void) {	// �-�, ������������� ������������� � ������������
	int option;
	system("cls");
	while(1) {
		printf("�� ������������� ������ ������� ������?\n");
		printf("1. ��;\n");
		printf("2. ���.\n");
		safeScan("�������� ������� ������: ", "%d", &option);
		if(option == 1)
			return 1;
		else if(option == 2) {
			system("cls");
    		printf("�������� �������� ��������.\n");
    		enterToContinue();
			return 0;
		}
		else {
			system("cls");
			printf("������� 1 ��� 2!\n");
			enterToContinue();
		}
	}
	system("cls");
}

int isUsernameExists(char* name) {	// �-�, ����������� ������� ����� ������������
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

void enterUsername(char* username) {	// �-� ����� ����� ������������
	while(1) {
		safeScan("������� ��� ������������: ", "%s", username);
		if (isUsernameExists(username)) {
			system("cls");
			printf("������������ � ����� ������ ��� ����������.\n\n");
			enterToContinue();
		} else
			break;
	}
}

int isSurnameExists(char* name) {	// �-�, ����������� ������� �������
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

void enterRole(char* role) {	// �-� ����� ����
	while (1) {
		safeScan("������� ����: ", "%s", role);
		if (strcmp(role, "admin") == 0 || strcmp(role, "user") == 0)
			break;
		else {
			system("cls");
			printf("������� admin ��� user!\n\n");
			enterToContinue();
		}
	}
}

void addAccount(void) {	// �-� ���������� ������� ������
	FILE* file = openFile("accounts.dat", "ab");
	if (!file)
		return;
	account = (struct accounts*)malloc(sizeof(struct accounts));
	system("cls");
	enterUsername(account->username);
	safeScan("������� ������: ", "%s", account->password);
	enterRole(account->role);
	fwrite(account, sizeof(struct accounts), 1, file);
	fclose(file);
	free(account);
	printf("\n������� ������� ������.\n\n");
	enterToContinue();
}

void viewAccount(void) {	// �-� ������ ������� �������
	FILE* file = openFile("accounts.dat", "rb");
	if (!file)
		return;
	account = (struct accounts*)malloc(sizeof(struct accounts));
	system("cls");
	printf("������ ������� �������:\n\n");
	while(fread(account, sizeof(struct accounts), 1, file))
		printf("��� ������������: %s\n������: %s\n����: %s\n\n", account->username, account->password, account->role);
	fclose(file);
	free(account);
	enterToContinue();
}

void editAccount(void) {	// �-� �������������� ������� ������
	char username[80];
	FILE* file = openFile("accounts.dat", "rb+");	// rb+ �.�. ����� � ��������� � ���������� ������
	if (!file)
		return;
	struct accounts* tempAccount = (struct accounts*)malloc(sizeof(struct accounts));
	system("cls");
	safeScan("������� ��� ������������ ��� ��������������: ", "%s", username);
	if (!isUsernameExists(username)) {
		system("cls");
		printf("������������ � ����� ������ �� ������.\n\n");
		enterToContinue();
		fclose(file);
		free(tempAccount);
		return;
	}
	while(fread(tempAccount, sizeof(struct accounts), 1, file)) {
		if (strcmp(tempAccount->username, username) == 0) {
			printf("\n������� ����� ������ ��� %s:\n", username);
			enterUsername(tempAccount->username);
			safeScan("������� ������: ", "%s", tempAccount->password);
			enterRole(tempAccount->role);
			fseek(file, -sizeof(struct accounts), SEEK_CUR);	// ����������� ��������� ����� �� ������ ��������� �� ������� �������
			fwrite(tempAccount, sizeof(struct accounts), 1, file);	// ������ ������ ������� � ����
			break;
		}
	}
	fclose(file);
	free(tempAccount);
	printf("\n������ �������� ������� ���������.\n\n");
	enterToContinue();
}

void deleteAccount(void) {	// �-� �������� ������� ������
    char username[80];
    FILE* file = openFile("accounts.dat", "rb");
    if (!file)
        return;
    struct accounts* tempAccount = (struct accounts*)malloc(sizeof(struct accounts));
    system("cls");
    safeScan("������� ��� ������������ ��� ��������: ", "%s", username);
    int userExists = 0;
    while(fread(tempAccount, sizeof(struct accounts), 1, file))
        if (strcmp(tempAccount->username, username) == 0)
            userExists = 1;
    fclose(file);
    if (!userExists) {
    	system("cls");
        printf("������: ������������ � ��������� ������ �� ������.\n\n");
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
    printf("\n������� ������� ������.\n\n");
    enterToContinue();
}

void enterSurname(char* surname) {	// �-� ����� ������� ��������
	while(1) {
		safeScan("������� �������: ", "%s", surname);
		if (isSurnameExists(surname)) {
			system("cls");
			printf("������� � ����� �������� ��� ����������.\n\n");
			enterToContinue();
		} else
			break;
	}
}

void enterGroupNumber(int* number) {	// �-� ����� ������ ������
    while (1) {
        safeScan("������� ����� ������: ", "%d", number);
        if (*number >= 1)
            break;
        else {
            printf("\n������� ���������� ������!\n\n");
        }
    }
}

void enterAvrMark(float* mark) {	// �-� ����� �������� ����� ������������
    while (1) {
        safeScan("������� ������� ����: ", "%f", mark);
        if (*mark >= 1 && *mark <= 10)
            break;
        else {
            printf("\n������� ���������� ������!\n\n");
        }
    }
}

void printData(void) {	// �-� ������ ������ � ��������
	printf("���: %s %s %s\n", data->surname, data->name, data->patronymic);
	printf("����� ������: %d\n", data->groupNumber);
	printf("������� ����: %0.2f\n", data->avrMark);
	printf("������: %s\n", isEnrolled(data->physics));
	printf("����������������: %s\n", isEnrolled(data->programing));
	printf("���������� ����: %s\n", isEnrolled(data->english));
	printf("���� ������: %s\n\n", isEnrolled(data->dataBase));
}

void viewData(void) {	// �-� ������ ������ � ��������
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

void addData(void) {	// �-� ���������� ������ � ��������
	FILE* file = openFile("data.dat", "ab");
	if (!file)
		return;
	data = (struct data*)malloc(sizeof(struct data));
	system("cls");
	enterSurname(data->surname);
	safeScan("������� ���: ", "%s", data->name);
	safeScan("������� ��������: ", "%s", data->patronymic);
	enterGroupNumber(&(data->groupNumber));
	enterAvrMark(&(data->avrMark));
	getEnrollment("������", &(data->physics));
	getEnrollment("����������������", &(data->programing));
	getEnrollment("���������� ����", &(data->english));					// �������� �� ���������?
	getEnrollment("���� ������", &(data->dataBase));
	fwrite(data, sizeof(struct data), 1, file);
	fclose(file);
	free(data);
	printf("\n������ � �������� ������� �������.\n\n");
	enterToContinue();
}

void deleteData(void) {	// �-� �������� ������ � ��������
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
    safeScan("������� ������� �������� ��� ��������: ", "%s", surname);
    int dataExists = 0;
    while (fread(tempData, sizeof(struct data), 1, file))
        if (strcmp(tempData->surname, surname) == 0) {
            dataExists = 1;
            break; // ������ �������, ������� �� �����
        }
    fclose(file);
    if (!dataExists) {
        system("cls");
        printf("������: ������� � ��������� �������� �� ������.\n\n");
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
    file = openFile("data.dat", "rb"); // ��������� ���� ������
    while (fread(tempData, sizeof(struct data), 1, file))
        if (strcmp(tempData->surname, surname) != 0)
            fwrite(tempData, sizeof(struct data), 1, tempFile);
    fclose(file);
    fclose(tempFile);
    remove("data.dat");
    rename("temp.dat", "data.dat");
    free(tempData);
    printf("\n������� ������� ������.\n\n");
    enterToContinue();
}

void getEnrollment(char* subject, int* enrollment) {	// �-� ����� ������ �� �����������
    while (1) {
        printf("������ �� %s (1 - ��, 0 - ���): ", subject);
        safeScan("", "%d", enrollment);
        if (*enrollment == 0 || *enrollment == 1)
            break;
        else
            printf("������� 0 ��� 1!\n");
    }
}

const char* getCourseName(int i) {	// �������, ������������ �������� ������������
	const char* physics = "������";
	const char* programming = "����������������";
	const char* english = "���������� ����";
	const char* dataBase = "���� ������";
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

void editData(void) {	// �-� �������������� ������
    FILE* file = openFile("data.dat", "rb+");
    if (!file)
        return;
    char searchSurname[80];
    system("cls");
    safeScan("������� ������� ��������, ������ � ������� ����� ��������: ", "%s", searchSurname);
	if (!isSurnameExists(searchSurname)) {
		system("cls");
		printf("������������ � ����� ������� �� ������.\n\n");
		enterToContinue();
		fclose(file);
		return;
	}
    data = (struct data*)malloc(sizeof(struct data));
    while (fread(data, sizeof(struct data), 1, file)) {
        if (strcmp(data->surname, searchSurname) == 0) {
            printf("������� ����� ������ ��� �������� %s:\n", data->surname);
            enterSurname(data->surname);
            safeScan("������� ���: ", "%s", data->name);
            safeScan("������� ��������: ", "%s", data->patronymic);
            enterGroupNumber(&(data->groupNumber));
            enterAvrMark(&(data->avrMark));
            getEnrollment("������", &(data->physics));
            getEnrollment("����������������", &(data->programing));
            getEnrollment("���������� ����", &(data->english));
            getEnrollment("���� ������", &(data->dataBase));
            fseek(file, -sizeof(struct data), SEEK_CUR);	// ����������� ��������� ����� �� ������ ��������� �� ������� �������
            fwrite(data, sizeof(struct data), 1, file);	// ������ ������ ������� � ����
            printf("\n������ � �������� ������� ���������.\n\n");
            break;
        }
    }
    fclose(file);
    free(data);
    enterToContinue();
}

void printCourseStudents(int course) {	// �-� ������ ���������, ���������� �� �������������� �������
    struct data* data = (struct data*)malloc(sizeof(struct data));
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
    struct data students[100], temp;
    int numStudents = 0;
    while (fread(&students[numStudents], sizeof(struct data), 1, file)) {	// ������� ���-�� ��������� �� ������������
        numStudents++;
    }
    fclose(file);
    for (int i = 0; i < numStudents - 1; i++) {	// ���������� ��������� �� �������� ����� ������������
        for (int j = i + 1; j < numStudents; j++) {
            if (students[i].avrMark < students[j].avrMark) {
                temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
    system("cls");
    printf("��������, ���������� ������ �� �������������� ������� \"%s\":\n", getCourseName(course));
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

void printCoursePopularity(void) {	// ����� ������ ���-�� ���������, ���������� �� ������������
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
    printf("������������ �������������� �������:\n");
    printf("������: %d\n", physicsCount);
    printf("����������������: %d\n", programingCount);			
    printf("���������� ����: %d\n", englishCount);
    printf("���� ������: %d\n\n", dataBaseCount);
    fclose(file);
    free(data);
    enterToContinue();
}

void searchByField(char* searchStr) {	// �-�  ������ ������ �� ���
    char searchValue[80];
    int found = 0;
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
	data = (struct data*)malloc(sizeof(struct data));
	system("cls");
    safeScan("������� ������ ��� ������: ", "%s", searchValue);
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
        printf("������� �� ������.\n\n");
    }
    fclose(file);
    free(data);
    enterToContinue();
}

void searchByAvrMark(void) {	// �-� ������ �� �������� ����� ������������
    float avrMark;
    int found = 0;
    system("cls");
    safeScan("������� ������� ���� ��� ������: ", "%f", &avrMark);
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
        printf("������� �� ������.\n\n");
    }
    enterToContinue();
}

void searchByGroupNumber(void) {	// �-� ������ �� ������ ������
    int groupNumber;
    int found = 0;
	system("cls");
    safeScan("������� ����� ������ ��� ������: ", "%d", &groupNumber);
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
        printf("������� �� ������.\n\n");
    }
    enterToContinue();
}

void sortBySurname(void) {	// �-� ���������� �� �������
    struct data* dataArray = NULL;
    int n = 0;
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
    while (fread(&data, sizeof(struct data), 1, file))	// ������� ���-�� �������
        n++;
    dataArray = (struct data*)malloc(n * sizeof(struct data));
    rewind(file);	// ������� � ������ �����
    for (int i = 0; i < n; i++)
        fread(&dataArray[i], sizeof(struct data), 1, file);
    fclose(file);
    for (int i = 0; i < n - 1; i++)	// ����������
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
        fwrite(&dataArray[i], sizeof(struct data), 1, file);	// ������ ��������������� ������ � ����
    fclose(file);
    free(dataArray);
	system("cls");
    printf("������ ������� ������������� �� �������.\n\n");
    enterToContinue();
}

void sortByGroupNumber(void) {	// �-� ���������� �� ������ ������
    struct data* dataArray = NULL;
    int n = 0;
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
    while (fread(&data, sizeof(struct data), 1, file))	// ������� ���-�� �������
        n++;
    dataArray = (struct data*)malloc(n * sizeof(struct data));
    rewind(file);	// ������� � ������ �����
    for (int i = 0; i < n; i++)
        fread(&dataArray[i], sizeof(struct data), 1, file);
    fclose(file);
    for (int i = 0; i < n - 1; i++)	// ����������
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
        fwrite(&dataArray[i], sizeof(struct data), 1, file);	// ������ ��������������� ������ � ����
    fclose(file);
    free(dataArray);
	system("cls");
    printf("������ ������� ������������� �� ������ ������.\n\n");
    enterToContinue();
}

void sortByAvrMark(void) {	// �-� ���������� �� �������� ����� ������������
    struct data* dataArray = NULL;
    int n = 0;
    FILE* file = openFile("data.dat", "rb");
    if (!file)
        return;
    while (fread(&data, sizeof(struct data), 1, file))	// ������� ���-�� �������
        n++;
    dataArray = (struct data*)malloc(n * sizeof(struct data));
    rewind(file);	// ������� � ������ �����
    for (int i = 0; i < n; i++)
        fread(&dataArray[i], sizeof(struct data), 1, file);
    fclose(file);
    for (int i = 0; i < n - 1; i++)	// ����������
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
        fwrite(&dataArray[i], sizeof(struct data), 1, file);	// ������ ��������������� ������ � ����
    fclose(file);
    free(dataArray);
	system("cls");
    printf("������ ������� ������������� �� ������������.\n\n");
    enterToContinue();
}

void userMenu(void) {	// �-� ���� ������������
	system("cls");
	int option = 0;
	while(1) {
		printf("������� ����:\n");
		printf("1. �������� ���� ������;\n");
		printf("2. ���������� ��������������� �������;\n");
		printf("3. ����� ������;\n");
		printf("4. ����������;\n");
		printf("5. �����.\n\n");
		safeScan("������� ����� ������: ", "%d", &option);
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
				printf("\n������� ������ ����� ������!\n\n");
				enterToContinue();
		}
	}
}

void adminMenu(void) {	// �-� ���� ��������������
	system("cls");
	int option = 0;
	while(1) {
		printf("������� ����:\n");
		printf("1. ���������� �������� �������� �������������;\n");
		printf("2. ������ � �������;\n");
		printf("3. �����.\n\n");
		safeScan("������� ����� ������: ", "%d", &option);
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
				printf("\n������� ������ ����� ������!\n\n");
				enterToContinue();
		}
	}
}

void accountManagementMenu(void) {	// �-� ���� ���������� �������� �������� �������������
	system("cls");
	int option = 0;
	while(1) {
		printf("���������� �������� �������� �������������:\n");
		printf("1. �������� ���� ������� �������;\n");
		printf("2. ���������� ����� ������� ������;\n");
		printf("3. �������������� ������� ������;\n");
		printf("4. �������� ������� ������;\n");
		printf("5. �����.\n\n");
		safeScan("������� ����� ������: ", "%d", &option);
		getchar();											//�����?
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
				printf("\n������� ������ ����� ������!\n\n");
				enterToContinue();
		}
	}
}

void workingWithDataMenu(void) {	// �-� ���� ������ � �������
	system("cls");
	int option = 0;
	while(1) {
		printf("������ � �������:\n");
		printf("1. ����� ��������������;\n");
		printf("2. ����� ��������� ������;\n");
		printf("3. �����.\n\n");
		safeScan("������� ����� ������: ", "%d", &option);
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
				printf("\n������� ������ ����� ������!\n\n");
				enterToContinue();
		}
	}
}

void editModeMenu(void) {	// �-� ���� ������ ��������������
	system("cls");
	int option = 0;
	while(1) {
		printf("����� ��������������:\n");
		printf("1. �������� ���� ������;\n");
		printf("2. ���������� ����� ������;\n");
		printf("3. �������� ������;\n");
		printf("4. �������������� ������;\n");
		printf("5. �����.\n\n");
		safeScan("������� ����� ������: ", "%d", &option);
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
				printf("\n������� ������ ����� ������!\n\n");
				enterToContinue();
		}
	}
}

void dataProcessingMenu(void) {	// �-� ���� ��������� ������
	system("cls");
	int option = 0;
	while(1) {
		printf("����� ��������� ������:\n");
		printf("1. ���������� ��������������� �������;\n");
		printf("2. ����� ������;\n");
		printf("3. ����������;\n");
		printf("4. �����.\n\n");
		safeScan("������� ����� ������: ", "%d", &option);
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
				printf("\n������� ������ ����� ������!\n\n");
				enterToContinue();
		}
	}
}

void individualTaskMenu(void) {	// �-� ���� ��������������� �������
	system("cls");
	int option = 0;
	while(1) {
		printf("���������� �������������� �������:\n");
		printf("1. ����� ��������� ���������� �� �������������� �������;\n");
		printf("2. ����� ���������� ��������� ���������� �� �������������� �������;\n");
		printf("3. �����.\n\n");
		safeScan("������� ����� ������: ", "%d", &option);
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
				printf("\n������� ������ ����� ������!\n\n");
				enterToContinue();
		}
	}
}

void choiceOfCourseMenu(void) {	// �-� ���� ������ ����������
	system("cls");
	int option = 0;
	while(1) {
		printf("�������� �������������� �������:\n");
		printf("1. ������;\n");
		printf("2. ����������������;\n");
		printf("3. ���������� ����;\n");
		printf("4. ���� ������;\n");
		printf("5. �����.\n\n");
		safeScan("������� ����� ������: ", "%d", &option);
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
				printf("\n������� ������ ����� ������!\n\n");
				enterToContinue();
		}
	}
}

void searchDataMenu(void) {	// �-� ���� ������ ��������� ��� ������
	system("cls");
	int option = 0;
	while(1) {
		printf("�������� �������� ��� ������:\n");
		printf("1. ���;\n");
		printf("2. �������;\n");
		printf("3. ��������;\n");
		printf("4. ����� ������;\n");
		printf("5. �������� ����;\n");
		printf("6. �����.\n\n");
		safeScan("������� ����� ������: ", "%d", &option);
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
				printf("\n������� ������ ����� ������!\n\n");
				enterToContinue();
		}
	}
}

void sortingDataMenu(void) {	// �-� ���� ������ ��������� ����������
	system("cls");
	int option = 0;
	while(1) {
		printf("�������� �������� ��� ����������:\n");
		printf("1. �������;\n");
		printf("2. ����� ������;\n");
		printf("3. ������������;\n");
		printf("4. �����.\n\n");
		safeScan("������� ����� ������: ", "%d", &option);
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
				printf("\n������� ������ ����� ������!\n\n");
				enterToContinue();
		}
	}
}
