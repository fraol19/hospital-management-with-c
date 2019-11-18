#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>
#include <stdbool.h>

FILE *fd,*fp,*fdp,*fpass, *ftmp, *fcounts;

/**************** report for a patient ***************/
struct Count{
  int pcount,dcount,dpcount,ucount;
}counter;
struct Report{
  int docid;
  char text[500];
}rep;
/***************** address for both patient and doctor **************/
struct Address{
  char city[20];
  char phone[20];
  char houseNo[20];
}ad;
/*************** department struct ***********/
struct Department{
  int id;
  char name[60];
}dep;
/************* Tests for a patient ********/
struct Test{
  char name[50];
  int fee;
}test;
/******************* Doctor struct ****/
struct Doctor{
  int id;
  struct Department dep;
  char name[60];
  char sex;
  char specialize[40];
  struct Address ad;
  char since[30];
}doc;
/******************** patient struct ******/
struct Patient{
  int id;
  int testIndex;
  int pstate;
  char name[60];
  int age;
  char sex,roomNo[4];
  char disease[60];
  char date[30];
  int totalfee;
  struct Test test[15];
  struct Address ad;
  struct Report rep;
  struct Doctor doc;
}patient;
/*************users accessing the system*/
struct User{
  int id;
  int docid;
  int type;
  char name[60];
  char password[40];
  struct Address ad;
}user;
typedef struct Patient Patient;
typedef struct Doctor Doctor;
typedef struct Department Department;
typedef struct User User;

/*****function declarations *****/
void menuMenu();
void docsMenu();
void mainLogin();
void addPatient();
void addDoctor();
void addDepartment();
void addUser(int call);
void showPatients();
void showDoctors();
void showDepartments();
void showUsers();
void showReport();
void searchPatient();
void searchDoctor();
void searchDepartment();
void searchUser();
void editPatient();
void editDoctor(int callType);
void editDepartment();
void editUser();
void editDocAuth();
void deletePatient();
void deleteDoctor();
void deleteDepartment();
void deleteUser();
void format();
void showMyPatient();
void addTest(int id);
void showTest(int id);
void generateReport(int id);
void myReports();
void printPatient(Patient p);
void printDoctor(Doctor d);
void printDepartment(Department dp);
void printUser(User u);
void printReport(Patient p);
void printBill();
bool isAuth(char name[40], char pass[40], int type);
void passInput(int type);
void init();
bool get_Pdata(bool addOp);
bool get_Ddata(int call,bool addOp);
void get_Dpdata(bool addOp);
void get_Udata(bool addOp);
void copy_docs();
void copy_patients();
void copy_deprtment();
void copy_users();
void display_list();
bool assign_doctor(int id);
bool assign_department(int id);
void updateCounts();
void sort(int type);
void sortedPatient();
void sortedDoctor();
void sortedDepartment();
void sortedUser();

/********* each patient,doctor,user,and department count ******/
static int pcount = 0;
static int dcount = 0;
static int ucount = 0;
static int dpcount = 0;
static int cur_user_id;
static int cur_doc_id;
//int yAxis=4;
char input[30]; //input for searching
char snum[5];  //convert ids to string to compare with input
bool found = false;
/************ file names for each data-sets ****/
char docfile[] = "doctors.dat";
char patientfile[] = "patients.dat";
char depfile[] = "departments.dat";
char logfile[] = "users.dat";
char tempfile[] = "tmp.dat";
/************* change cursor position*******************/
COORD coord = {0, 0};
void gotoxy (int x, int y){
  coord.X = x; coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void holdOn(){
  printf("\n\npress anykey to continue...........");
  fflush(stdin);
  getch();
}
/****************** string size counter***************/
int lengthOf(char x[]){
  int i;
  for(i=0 ; x[i] != '\0' ; i++ ){
  }
  return i;
}
int toInt(char a[]){
 int c,offset=0,n=0;
 if(isdigit(a[0]) || isdigit(a[1]) || isdigit(a[2])){
   return 0;
 }else{
   for(c = offset; a[c] != '\0'; c++){
     n = n*10 + a[c] - '0';
   }
   return n;
 }

}
/************** pritty line *********/
void prittyLine(char a[]){
 printf("\n");
 int i;
 for(i=0 ; i<15 ; i++)
   printf("\xdb");
 printf(" %s ", a);
 for(i=0; i<15 ; i++)
    printf("\xdb");
 printf("\n");
}
void liner(int len){
 int i;
 for(i=0 ; i<len ; i++)
   printf("\xdb");
 printf("\n");
}

/************** custom Linked List Data-structure for storing datas ****************/
struct NodeP{
  Patient Pdata;
  struct NodeP *prev;
  struct NodeP *next;
};
struct NodeD{
  Doctor Ddata;
  struct NodeD *prev;
  struct NodeD *next;
};
struct NodeDp{
  Department Dpdata;
  struct NodeDp *prev;
  struct NodeDp *next;
};
struct NodeU{
  User Udata;
  struct NodeU *prev;
  struct NodeU *next;
};
struct NodeP *head1 = NULL;  //head for patient data-sets
struct NodeD *head2 = NULL;  //head for doctor data-sets
struct NodeDp *head3 = NULL;  //head for user data-sets
struct NodeU *head4 = NULL;  //head for department data-sets

/******************** insert to a linked list **********************/
void insertP(Patient data){
  struct NodeP* newnode = malloc(sizeof(struct NodeP));
  newnode->Pdata = data;
  if(head1 == NULL){
    head1 = newnode;
    head1->prev = NULL;
    head1->next = NULL;
  }else{
     struct NodeP *tmp = head1;
     while(tmp->next != NULL){
        tmp = tmp->next;
     }
     tmp->next = newnode;
     newnode->prev = tmp;
     newnode->next = NULL;
  }
}
void insertD(Doctor data){
  struct NodeD* newnode = malloc(sizeof(struct NodeD));
  newnode->Ddata = data;
  if(head2 == NULL){
    head2= newnode;
    head2->prev = NULL;
    head2->next = NULL;
  }else{
     struct NodeD *tmp = head2;
     while(tmp->next != NULL){
        tmp = tmp->next;
     }
     tmp->next = newnode;
     newnode->prev = tmp;
     newnode->next = NULL;
  }
}
void insertU(User data){
  struct NodeU* newnode = malloc(sizeof(struct NodeU));
  newnode->Udata = data;
  if(head4 == NULL){
    head4= newnode;
    head4->prev = NULL;
    head4->next = NULL;
  }else{
     struct NodeU *tmp = head4;
     while(tmp->next != NULL){
        tmp = tmp->next;
     }
     tmp->next = newnode;
     newnode->prev = tmp;
     newnode->next = NULL;
  }
}
void insertDp(Department data){
  struct NodeDp* newnode = malloc(sizeof(struct NodeDp));
  newnode->Dpdata = data;
  if(head3 == NULL){
    head3= newnode;
    head3->prev = NULL;
    head3->next = NULL;
  }else{
     struct NodeDp *tmp = head3;
     while(tmp->next != NULL){
        tmp = tmp->next;
     }
     tmp->next = newnode;
     newnode->prev = tmp;
     newnode->next = NULL;
  }
}
void erasePlist(){
   struct NodeP *tmp = head1;
   struct NodeP *next;
   while(tmp != NULL){
     next = tmp->next;
     free(tmp);
     tmp = next;
   }
   head1 = NULL;
}
void eraseDlist(){
   struct NodeD *tmp = head2;
   struct NodeD *next;
   while(tmp != NULL){
     next = tmp->next;
     free(tmp);
     tmp = next;
   }
   head2 = NULL;
}
void eraseDplist(){
   struct NodeDp *tmp = head3;
   struct NodeDp *next;
   while(tmp != NULL){
     next = tmp->next;
     free(tmp);
     tmp = next;
   }
   head3 = NULL;
}
void eraseUlist(){
   struct NodeU *tmp = head4;
   struct NodeU *next;
   while(tmp != NULL){
     next = tmp->next;
     free(tmp);
     tmp = next;
   }
   head4 = NULL;
}

/********** Receiving input from user ******/
bool get_Pdata(bool addOp){
  patient.testIndex = 0;
  patient.totalfee = 0;
  if(addOp){
      time_t t = time(NULL);
      char* date = strtok(ctime(&t),"\n");
      int i;
      for(i=0; date[i] != '\0'; i++){
        patient.date[i] = date[i];
      }
      printf("Date Of Registration : %s\n", patient.date);

  }
  printf("Enter name : ");
  scanf(" %[^\n]s",patient.name);
  sexlable:
  printf("Enter sex :");
  fflush(stdin);
  patient.sex = getch();
  putch(patient.sex);
  if(patient.sex != 'm' && patient.sex != 'f' && patient.sex != 'M' && patient.sex != 'F'){
    printf("\tIncorrect Sex (use M/m or F/f) \n");
    goto sexlable;
  }
  printf("\nEnter Room No.  :");
  scanf(" %s", patient.roomNo);
  printf("Enter Age :  ");
  scanf("%d", &patient.age);
  printf("Enter Disease :   ");
  scanf(" %[^\n]s ", patient.disease);
  PTYPE:
  printf("1 - P.D.O \n2 - Emergency :  ");
  scanf("%d",&patient.pstate);
  if(patient.pstate != 2 && patient.pstate != 1){
    printf("Select Proper patient type \n (1/2) \n");
    goto PTYPE;
  }else{
    if(patient.pstate == 1)
      patient.totalfee = 150;
    else
      patient.totalfee = 250;
  }
  printf("Address  : : : : \n");
  printf("City :");
  scanf(" %[^\n]s" , patient.ad.city);
  printf("Phone :");
  scanf(" %[^\n]s" , patient.ad.phone);
  printf("House No :");
  scanf(" %[^\n]s" , patient.ad.houseNo);


  //assign Doctor
  if(head2 != NULL){
      tryD:
      prittyLine(" Select Doctor ");
      display_list();
      int tmp;
      printf("\nSelect Id : ");
      scanf(" %d" ,&tmp);
      if(!assign_doctor(tmp)){
       printf("\t Error Doctor Assignment(check the id of the doctor) \n\n");
       printf("Do want to assign again(y/n) : ");
       fflush(stdin);
       if(getch()=='y')
        goto tryD;
       return;
      }
  }else{
    printf("Doctor :  Not Assigned (no doctors available) \n");
  }
  if(addOp){
    ++pcount;
      patient.id = pcount;
      printf("Patient Id : %d\n" , patient.id);
  }
  return true;
}
bool get_Ddata(int call,bool addOp){
  if(addOp){
      time_t t = time(NULL);
      char* date = strtok(ctime(&t),"\n");
      int i;
      for(i=0; date[i] != '\0'; i++){
        doc.since[i] = date[i];
      }
  }
  printf("Date Of Registration : %s\n", doc.since);
  printf("Enter full name : ");
  scanf(" %[^\n]s",doc.name);
  dsexlable:
  printf("Enter sex :");
  fflush(stdin);
  doc.sex = getch();
  putch(doc.sex);
  if(doc.sex != 'm' && doc.sex != 'f' && doc.sex != 'M' && doc.sex != 'F'){
    printf("Incorrect Sex \n");
    goto dsexlable;
  }
  printf("\nEnter Specialization :   ");
  scanf(" %[^\n]s ", doc.specialize);
  printf("Address  : : : : \n");
  printf("City :");
  scanf(" %[^\n]s" , doc.ad.city);
  printf("Phone :");
  scanf(" %[^\n]s" , doc.ad.phone);
  printf("House No :");
  scanf(" %[^\n]s" , doc.ad.houseNo);
  int tmp;
  tryDp:
  showDepartments();
  printf("Give Department ID : ");
  scanf(" %d" , &tmp);
  if(head3 != NULL){
    if(!assign_department(tmp)){
       printf("\t Error Department Assignment (\xDb !!check the existence of the department) \n\n");
       printf("Do you want to assign again(y/n) : ");
       fflush(stdin);
       if(getch() == 'y')
        goto tryDp;
       return;
    }
  }else{
    printf("Department :  Not Assigned (no department available) \n");
  }
  if(addOp){
    ++dcount;
    doc.id = dcount;
    printf("Doctor ID : %d\n",doc.id);
  }
  if(call == 1){
    ++ucount;
    user.id = ucount;
    user.type = 1;
    user.ad = doc.ad;
    user.docid = doc.id;
    prittyLine(" Login Credential Info ");
    printf("Enter user name :");
    scanf(" %[^\n]s" , user.name);
    printf("\nEnter password for login credentials : ");
    scanf(" %[^\n]s" , user.password);
    fpass=fopen(logfile,"ab+");
    fseek(fpass,0,SEEK_END);
    fwrite(&user,sizeof(user),1,fpass);
    fclose(fpass);
  }
  return true;
}
void get_Dpdata(bool addOp){
  printf("Enter name : ");
  scanf(" %[^\n]s",dep.name);
  if(addOp){
   ++dpcount;
   dep.id = dpcount;
   printf("Department Id : %d\n",dep.id);
  }
}
void get_Udata(bool addOp){
  fflush(stdin);
  system("cls");
  user.type = 0;
  printf("Enter name :");
  scanf(" %[^\n]s" , user.name);
  printf("\nEnter password :");
  scanf(" %[^\n]s" , user.password);
  printf("\nAddress  : : : : \n");
  printf("City :");
  scanf(" %[^\n]s" , user.ad.city);
  printf("Phone :");
  scanf(" %[^\n]s" , user.ad.phone);
  printf("House No :");
  scanf(" %[^\n]s" , user.ad.houseNo);
  if(addOp){
   ++ucount;
   user.id = ucount;
   printf("User ID : %d\n",user.id);
  }
}
/*****
Record Adding.................
*******/
void addPatient(){
  system("cls");
  prittyLine("Add Patient");
  addMore:
  fp=fopen(patientfile,"ab+");
  printf("\n");
  fseek(fp,0,SEEK_END);
  if(!get_Pdata(true)){
   prittyLine(" Failed to record data \n");
   fclose(fp);
  }else{

      fwrite(&patient,sizeof(patient),1,fp);
      fclose(fp);
      updateCounts();
      erasePlist();
      copy_patients();
      prittyLine(" Patient Recorded Successfully ");
      printf("\nDo you want to add More(y/n) : ");
      fflush(stdin);
      if(getch() == 'y')
      goto addMore;
  }
  holdOn();
}
void addDoctor(){
  system("cls");
  prittyLine("Add Doctor");
  addMore:
  fd=fopen(docfile,"ab+");
  printf("\n");
  fseek(fd,0,SEEK_END);
  if(!get_Ddata(1,true)){
   prittyLine(" Failed to record Doctor data \n");
   fclose(fp);
  }else{
    fwrite(&doc,sizeof(doc),1,fd);
    fclose(fd);
    updateCounts();
    eraseDlist();
    copy_docs();
    prittyLine(" Doctor Recorded Successfully ");
    printf("\nDo you want to add More(y/n) : ");
    fflush(stdin);
    if(getch() == 'y')
     goto addMore;
  }
  holdOn();
}
void addDepartment(){
  system("cls");
  prittyLine("Add Department");
  addMore:
  fdp=fopen(depfile,"ab+");
  printf("\n");
  fseek(fdp,0,SEEK_END);
  get_Dpdata(true);
  fwrite(&dep,sizeof(dep),1,fdp);
  fclose(fdp);
  updateCounts();
  eraseDplist();
  copy_deprtment();
  prittyLine(" Department Registerd Successfully ");
  printf("\nDo you want to add More(y/n) : ");
  fflush(stdin);
  if(getch() == 'y')
    goto addMore;
  holdOn();
}
void addUser(int call){
  system("cls");
  prittyLine("Add User");
  addMore:
  fpass=fopen(logfile,"ab+");
  printf("\n");
  fseek(fpass,0,SEEK_END);
  get_Udata(true);
  fwrite(&user,sizeof(user),1,fpass);
  fclose(fpass);
  updateCounts();
  eraseUlist();
  copy_users();
  prittyLine(" User Registerd Successfully ");
  printf("\nDo you want to add More(y/n) : ");
  fflush(stdin);
  if(getch() == 'y')
    goto addMore;
  if(call == 1){
    mainLogin();
  }
  holdOn();
}
/***end of record adding methods***/

/****
Display Records........
****/
void showPatients(){
 system("cls");
 fp=fopen(patientfile,"rb");
 printf("\n");
 liner(134);
 printf("%-4s %-20s %-3s %-5s %-8s %-15s %-9s %-15s %-25s %-10s %-20s","ID","NAME","SEX","AGE","ROOM NO","CITY","HOUSE_NO","PHONE","DISEASE","STATE","DOA");
 printf("\n");
 liner(134);
 printf("\n");
 while(fread(&patient,sizeof(patient),1,fp)==1){
   printPatient(patient);
 }
 fclose(fp);
}
void showDoctors(){
 system("cls");
 fd=fopen(docfile,"rb");
 printf("\n");
 liner(145);
 printf("%-4s %-20s %-3s %-15s %-25s %-25s %-8s %-15s %-30s","ID","NAME","SEX","DEPARTMENT","SPECIALIZATION","CITY","HOUSE_NO","PHONE","WORKING SINCE");
 printf("\n");
 liner(145);
 printf("\n");
 while(fread(&doc,sizeof(doc),1,fd)==1){
   printDoctor(doc);
 }
 fclose(fd);
}
void showDepartments(){
 system("cls");
 fdp=fopen(depfile,"rb");
 printf("\n");
 liner(31);
 printf("%-6s %-25s","ID","NAME");
 printf("\n");
 liner(31);
 printf("\n");
 while(fread(&dep,sizeof(dep),1,fdp)==1){
   printDepartment(dep);
 }
 fclose(fdp);
}
void showUsers(){
 system("cls");
 fpass=fopen(logfile,"rb");
 printf("\n");
 liner(84);
 printf("%-4s %-20s %-25s %-10s %-15s %-10s","ID","USERNAME","CITY","HOUSE_NO","PHONE","TYPE");
 printf("\n");
 liner(84);
 printf("\n");
 while(fread(&user,sizeof(user),1,fpass)==1){
   printUser(user);
 }
 fclose(fpass);
}
void showReport(){
     system("cls");
     printf("\n");
     liner(61);
     printf("%-25s %-6s %-30s","PATIENT","DOC_ID","REPORT");
     printf("\n");
     liner(61);
     printf("\n");
     fp = fopen(patientfile,"rb");
     while(fread(&patient,sizeof(patient),1,fp) == 1){
       printReport(patient);
     }
     fclose(fp);
}
void printPatient(Patient p){
 printf("%-4d %-20s %-3c %-5d %-8s %-15s %-9s %-15s %-25s",p.id,p.name,p.sex,p.age,p.roomNo,p.ad.city,p.ad.houseNo,p.ad.phone,p.disease);
 if(p.pstate == 1)
   printf(" %-10s","P.D.O");
 else
   printf(" %-10s","Emergency");
 printf(" %-20s",p.date);
 printf("\n");
}
void printDoctor(Doctor d){
 printf("%-4d %-20s %-3c %-15s %-25s %-25s %-8s %-15s %-30s",d.id,d.name,d.sex,d.dep.name,d.specialize,d.ad.city,d.ad.houseNo,d.ad.phone,d.since);
 printf("\n\n");
}
void printDepartment(Department dp){
 printf("%-6d %-25s",dp.id,dp.name);
 printf("\n");
}
void printUser(User u){
  printf("%-4d %-20s %-25s %-10s %-15s",u.id,u.name,u.ad.city,u.ad.houseNo,u.ad.phone);
  if(u.type == 0){
   printf(" %-10s","Admin");
  }else{
    printf(" %-10s","Doctor");
  }
  printf("\n");
}
void printReport(Patient p){
  printf("%-25s %-6d",p.name,p.rep.docid);
  if(strlen(p.rep.text) == 0){
    printf("%-30s" ,"NO report");
  }else{
    printf("%-30s",p.rep.text);
  }
  printf("\n");
}

/*****search functions******/
void searchPatient(){
 system("cls");
 printf("Give ID or Name  : ");
 scanf(" %[^\n]s",input);
 fp = fopen(patientfile,"rb");
  printf("\n");
  liner(134);
  printf("%-4s %-20s %-3s %-5d %-8s %-15s %-9s %-15s %-25s %-10s %-20s","ID","NAME","SEX","AGE","ROOM NO","CITY","HOUSE_NO","PHONE","DISEASE","STATE","DOA");
  printf("\n");
  liner(134);
  while(fread(&patient,sizeof(patient),1,fp) == 1){
   itoa(patient.id,snum,10);
    if(strcasecmp(patient.name,input) == 0 || strcmp(snum,input) == 0){
       found = true;
       printPatient(patient);
     }
   }
   if(!found){
    prittyLine(" Record Not Found ");
    found = false;
   }
   fclose(fp);
 }
void searchDoctor(){
  system("cls");
  printf("Give ID or Name  : ");
  scanf(" %[^\n]s",input);
  fd = fopen(docfile,"rb");
  printf("\n");
  liner(145);
  printf("%-4s %-20s %-3s %-15s %-25s %-25s %-8s %-15s %-30s","ID","NAME","SEX","DEPARTMENT","SPECIALIZATION","CITY","HOUSE_NO","PHONE","WORKING SINCE");
  printf("\n");
  liner(145);
  printf("\n");
  while(fread(&doc,sizeof(doc),1,fd) == 1){
   itoa(doc.id,snum,10);
    if(strcasecmp(doc.name,input) == 0 || strcmp(snum,input) == 0){
       printDoctor(doc);
       found = true;
     }
   }
   if(!found){
    prittyLine(" Record Not Found ");
    found = false;
   }
   fclose(fd);
}
void searchDepartment(){
  system("cls");
  printf("Give ID or Name  : ");
  scanf(" %[^\n]s",input);
  fdp = fopen(depfile,"rb");
  printf("\n");
  liner(31);
  printf("%-6s %-25s","ID","NAME");
  printf("\n");
  liner(31);
  printf("\n");
  while(fread(&dep,sizeof(dep),1,fdp) == 1){
   itoa(dep.id,snum,10);
    if(strcasecmp(dep.name,input) == 0 || strcmp(snum,input) == 0){
       printDepartment(dep);
       found = true;
     }
   }
   if(!found){
    prittyLine(" Record Not Found ");
    found = false;
   }
   fclose(fdp);
}
void searchUser(){
  system("cls");
  printf("Give ID or Name  : ");
  scanf(" %[^\n]s",input);
  fpass = fopen(logfile,"rb");
  printf("\n");
  liner(84);
  printf("%-4s %-20s %-25s %-10s %-15s %-10s","ID","USERNAME","CITY","HOUSE_NO","PHONE","TYPE");
  printf("\n");
  liner(84);
  printf("\n");
  while(fread(&user,sizeof(user),1,fpass) == 1){
   itoa(user.id,snum,10);
    if(strcasecmp(user.name,input) == 0 || strcmp(snum,input) == 0){
       printUser(user);
       found = true;
    }
   }
   if(!found){
    prittyLine(" Record Not Found ");
    found = false;
   }
   fclose(fpass);
}

/***Data Modification*****/
void editPatient(){
  prittyLine(" Edit Patient ");
  char other;
  int id;
  do{
    printf("Enter Id of the patient : ");
    scanf(" %d",&id);
    if(id > pcount || id <= 0){
      printf("\tRecord Not Found (Check your ID)\n");
      return;
    }

    fp = fopen(patientfile, "rb+");
     while (fread(&patient, sizeof (patient), 1, fp) == 1){
       if(patient.id == id){
            printf("\n");
            liner(134);
            printf("%-4s %-20s %-3s %-5s %-8s %-15s %-9s %-15s %-25s %-10s %-18s","ID","NAME","SEX","AGE","ROOM NO","CITY","HOUSE_NO","PHONE","DISEASE","STATE","DOA");
            printf("\n");
            liner(134);
            printf("\n");
            printPatient(patient);
            printf("\n Record Of a Patient Edit data \n");
            if(get_Pdata(false)){
                fseek(fp, ftell(fp) - sizeof (patient), 0);
                fwrite(&patient, sizeof (patient), 1, fp);
                fclose(fp);
                printf("\tRecord Modified Successfully !\n");
                erasePlist();
                copy_patients();
                found = true;
                break;
            }else{
              printf("\tUnable to Modify Record (some error occurred getting input data)\n");
              found = true;
              fclose(fp);
              break;
            }
       }
     }
     if(!found){
       printf("\tRecord Not Found (Check your ID)\n");
     }
     found = false;
     printf("Modify Another Patient? (y/n) : ");
     fflush(stdin);
     other = getch();
     printf("%c",other);
     printf("\n");
  }while(other == 'y');
}
void editDoctor(int callType){
  int id = callType;
  prittyLine(" Edit Doctor ");
    if(callType == 0){
      printf("Enter Id of the Doctor : ");
      scanf(" %d",&id);
      if(id > dcount || id <= 0){
        printf("\tRecord Not Found (Check your ID)\n");
        return;
      }
    }
    fd = fopen(docfile, "rb+");
     while (fread(&doc, sizeof(doc), 1, fd) == 1){
       if(doc.id == id){
            printf("\n");
            liner(145);
            printf("%-4s %-20s %-3s %-15s %-25s %-25s %-8s %-15s %-30s","ID","NAME","SEX","DEPARTMENT","SPECIALIZATION","CITY","HOUSE_NO","PHONE","WORKING SINCE");
            printf("\n");
            liner(145);
            printf("\n");
            printDoctor(doc);
            printf("\n Record Of a Doctor Edit data \n");
            if(get_Ddata(0,false)){
                fseek(fd, ftell(fd)-sizeof(doc), 0);
                fwrite(&doc, sizeof (doc), 1, fd);
                fclose(fd);
                printf("Record Modified Successfully !\n");
                eraseDlist();
                copy_docs();
                found = true;
                break;
            }else{
              printf("\tUnable to Modify Record (some error occurred getting input data)\n");
              found = true;
              fclose(fd);
              break;
            }
       }
     }
     if(!found){
       printf("\tRecord Not Found(check your ID)\n");
     }
     found = false;
}
void editDepartment(){
  prittyLine(" Edit Department ");
  char other;
  int id;
  bool found = false;
  do{
        printf("Enter Id of the Department : ");
        scanf(" %d",&id);
        if(id > dpcount || id <= 0){
          printf("\tRecord Not Found (Check your ID)\n");
          return;
        }
        fdp = fopen(depfile, "rb+");
         while (fread(&dep, sizeof (dep), 1, fdp) == 1){
           if(dep.id == id){
                printf("\n");
                liner(31);
                printf("%-6s %-25s","ID","NAME");
                printf("\n");
                liner(31);
                printf("\n");
                printDepartment(dep);
                printf("\n Record Of a department Give new Data \n");
                get_Dpdata(false);
                fseek(fdp, ftell(fdp) - sizeof (dep), 0);
                fwrite(&dep, sizeof (dep), 1, fdp);
                fclose(fdp);
                found = true;
                break;
           }
         }
         if(found){
             eraseDplist();
             copy_deprtment();
             printf("\tRecord Modified Successfully !\n");
             found = false;
         }
         else{
           printf("\tRecord Not Found \n");
         }
         printf("Modify Another Department? (y/n) : ");
         fflush(stdin);
         other = getch();
         printf("%c",other);
         printf("\n");
  }while(other == 'y');
}
void editUser(){
  prittyLine(" Edit User ");
  char other;
  int id;
  bool found = false;
  do{
          printf("Enter Id of the User : ");
          scanf(" %d",&id);
          if(id > pcount || id <= 0){
            printf("\tRecord Not Found (Check your ID)\n");
            return;
          }
        fpass = fopen(logfile, "rb+");
         while (fread(&user, sizeof (user), 1, fpass) == 1){
           if(user.id == id){
                printf("\n");
                liner(84);
                printf("%-4s %-20s %-25s %-10s %-15s %-10s","ID","USERNAME","CITY","HOUSE_NO","PHONE","TYPE");
                printf("\n");
                liner(84);
                printf("\n");
                printf("\n Record Of a User Give new Data\n");
                get_Udata(false);
                fseek(fpass, ftell(fpass) - sizeof (user), 0);
                fwrite(&user, sizeof (user), 1, fpass);
                fclose(fpass);
                found = true;
                break;
           }
         }
         if(found){
             eraseUlist();
             copy_users();
             printf("\t Record Modified Successfully !\n");
             found = false;
         }
         else {
           printf("\tRecord Not Found \n");
         }
         printf("Modify Another Department? (y/n) : ");
         fflush(stdin);
         other = getch();
         printf("%c",other);
         printf("\n");
  }while(other == 'y' || other == 'Y');
}

/********** Deleting Resource **********/
void deletePatient(){
  system("cls");
  int id;
  printf("Enter ID of a Patient : ");
  scanf(" %d",&id);
  if(id > pcount || id <= 0){
    prittyLine(" Record Not Found ");
    return;
  }
  struct NodeP *tmp = head1;
  while(tmp != NULL){
   if(tmp->Pdata.id == id){
    found = true;
    printf("\n");
    liner(134);
    printf("%-4s %-20s %-3s %-5d %-8s %-15s %-9s %-15s %-25s %-10s %-20s","ID","NAME","SEX","AGE","ROOM NO","CITY","HOUSE_NO","PHONE","DISEASE","STATE","DOA");
    printf("\n");
    liner(134);
    printPatient(tmp->Pdata);
    break;
   }
   tmp = tmp->next;
  }
  if(!found){
    prittyLine(" Record not found ");
    found = false;
    return;
  }
  printf("Are you sure to delete the record (y/n)? : ");
  fflush(stdin);
  if(getch() == 'y'){
      fp = fopen(patientfile,"rb+");
      ftmp = fopen(tempfile,"wb+");
      while(fread(&patient,sizeof(patient),1,fp) == 1){
        if(patient.id != id){
          fseek(ftmp,0,SEEK_END);
          fwrite(&patient,sizeof(patient),1,ftmp);
        }
      }
      fclose(fp);
      fclose(ftmp);
      remove(patientfile);
      rename(tempfile,patientfile);
      prittyLine(" Record Deleted Successfully! ");
      erasePlist();
      copy_patients();
  }
}
void deleteDoctor(){
  system("cls");
  int id;
  printf("Enter ID of a Doctor : ");
  scanf(" %d",&id);
  if(id > dcount || id <= 0){
    prittyLine(" Record Not Found ");
    return;
  }
  struct NodeD *tmp = head2;
  while(tmp != NULL){
    if(tmp->Ddata.id == id){
      found = true;
      printf("\n");
      liner(145);
      printf("%-4s %-20s %-3s %-15s %-25s %-25s %-8s %-15s %-30s","ID","NAME","SEX","DEPARTMENT","SPECIALIZATION","CITY","HOUSE_NO","PHONE","WORKING SINCE");
      printf("\n");
      liner(145);
      printDoctor(tmp->Ddata);
      break;
    }
    tmp = tmp->next;
  }
  if(!found){
    prittyLine(" Record not found ");
    found = false;
    return;
  }
  printf("Are you sure to delete the record (y/n)? : ");
  fflush(stdin);
  if(getch() == 'y'){
      fd = fopen(docfile,"rb+");
      ftmp = fopen(tempfile,"wb+");
      while(fread(&doc,sizeof(doc),1,fd) == 1){
        if(doc.id != id){
          fseek(ftmp,0,SEEK_CUR);
          fwrite(&doc,sizeof(doc),1,ftmp);
        }
      }
      fclose(fd);
      fclose(ftmp);
      remove(docfile);
      rename(tempfile,docfile);
      prittyLine(" Record Deleted Successfully! ");
      eraseDlist();
      copy_docs();
  }
}
void deleteDepartment(){
  system("cls");
  int id;
  printf("Enter ID of a Department : ");
  scanf(" %d",&id);
  if(id > dpcount || id <= 0){
    prittyLine(" Record Not Found ");
    return;
  }
  struct NodeDp *tmp = head3;
  while(tmp != NULL){
    if(tmp->Dpdata.id == id){
      found = true;
      printf("\n");
      liner(31);
      printf("%-6s %-25s","ID","NAME");
      printf("\n");
      liner(31);
      printDepartment(tmp->Dpdata);
      break;
    }
    tmp = tmp->next;
  }
  if(!found){
    prittyLine(" Record not found ");
    found = false;
    return;
  }
  printf("Are you sure to delete the record (y/n)? : ");
  fflush(stdin);
  if(getch() == 'y'){
      fdp = fopen(depfile,"rb+");
      ftmp = fopen(tempfile,"wb+");
      while(fread(&dep,sizeof(dep),1,fdp) == 1){
        if(dep.id != id){
          fseek(ftmp,0,SEEK_CUR);
          fwrite(&dep,sizeof(dep),1,ftmp);
        }
      }
      fclose(fdp);
      fclose(ftmp);
      remove(depfile);
      rename(tempfile,depfile);
      prittyLine(" Record Deleted Successfully! ");
      eraseDplist();
      copy_deprtment();
  }
}
void deleteUser(){
  system("cls");
  int id;
  printf("Enter ID of a User : ");
  scanf(" %d",&id);
  if(id > ucount || id <= 0){
    prittyLine(" Record Not Found ");
    return;
  }
  struct NodeU *tmp = head4;
  while(tmp != NULL){
    if(tmp->Udata.id == id){
      found = true;
      printf("\n");
      liner(84);
      printf("%-4s %-20s %-25s %-10s %-15s %-10s","ID","USERNAME","CITY","HOUSE_NO","PHONE","TYPE");
      printf("\n");
      liner(84);
      printUser(tmp->Udata);
      break;
    }
    tmp = tmp->next;
  }
  if(!found){
    prittyLine(" Record not found ");
    found = false;
    return;
  }
  printf("Are you sure to delete the record (y/n)? : ");
  fflush(stdin);
  if(getch() == 'y'){
      fpass = fopen(logfile, "rb+");
      ftmp = fopen(tempfile, "wb+");
      rewind(fpass);
      while(fread(&user,sizeof(user),1,fpass) == 1){
        if(user.id != id){
          fseek(ftmp,0,SEEK_CUR);
          fwrite(&user,sizeof(user),1,ftmp);
        }
      }
      fclose(fpass);
      fclose(ftmp);
      remove(logfile);
      rename(tempfile,logfile);
      prittyLine(" Record Deleted Successfully! ");
      eraseUlist();
      copy_users();
  }
}
void format(){
 system("cls");
 system("color 04");
 printf("\t\t Are you sure to format the database(y/n) :  ");
 if(getch() == 'y'){
     erasePlist();
     eraseDlist();
     eraseDplist();
     eraseUlist();
     pcount = 0;
     dcount = 0;
     dpcount = 0;
     ucount = 0;
  fp=fopen(patientfile,"wb");
  fputs("",fp);
  fclose(fp);
  fd=fopen(docfile,"wb");
  fputs("",fd);
  fclose(fd);
  fdp = fopen(depfile,"wb");
  fputs("",fdp);
  fclose(fdp);
  fpass = fopen(logfile,"wb");
  fputs("",fpass);
  fclose(fpass);
  updateCounts();
  prittyLine(" Database formatted successfully! ");
 }
}
/****** print bill ****/
void prittyBill(Patient p){
  int i,totalfee=0;
  if(p.testIndex == 0){
   printf("\t\t\t \xdb\xdb NO TEST YET \xdb\xdb\n");
   printf("\t\t\t Total FEE : %d\n\n",p.totalfee);
  }else{
    for(i=0 ; i<p.testIndex ; i++){
      printf("\n\t");
      liner(31);
      printf(" \t%-25 %-6s","TEST NAME","FEE");
      printf("\n\t");
      liner(31);
      printf(" \t%-25s %-6d",p.test[i].name,p.test[i].fee);
      printf("\n");
      totalfee += p.test[i].fee;
    }
    if(p.pstate == 1)
       printf(" \t%-25s %-6d","P.D.O",150);
    else
       printf(" \t%-25s %-6d","Emergency",250);
    printf("\n");
    printf(" \t%-25s %-6d","TOTAL COST",p.totalfee);
    printf("\n");
  }
}
void printBill(){
  system("cls");
  system("color 01");
  int id;
  TRYBILL:
  printf("Enter Id of the patient : ");
  scanf(" %d",&id);
  if(id > pcount || id <= 0){
    prittyLine(" Record Not Found ");
    printf("Do you want to try again(y/n) : ");
    fflush(stdin);
    if(getch() == 'y')
      goto TRYBILL;
    return;
  }
  fp = fopen(patientfile,"rb");
  while(fread(&patient,sizeof(patient),1,fp) == 1){
    if(patient.id == id){
     found = true;
     printf("\n");
     liner(134);
     printf("%-4s %-20s %-3s %-5s %-8s %-15s %-9s %-15s %-25s %-10s %-20s","ID","NAME","SEX","AGE","ROOM NO","CITY","HOUSE_NO","PHONE","DISEASE","STATE","DOA");
     printf("\n");
     liner(134);
     printPatient(patient);
     printf("\n\n");
     prittyBill(patient);
     break;
    }
  }
  if(!found){
    prittyLine(" Record Not Found ");
  }
  found = false;
  holdOn();
}
/******* Doctors operations *****/
void showMyPatient(){
     bool hasP = false;
     int id;
     char choice;
     fp = fopen(patientfile,"rb");
     printf("\n");
     liner(134);
     printf("%-4s %-20s %-3s %-5s %-8s %-15s %-9s %-15s %-25s %-10s %-20s","ID","NAME","SEX","AGE","ROOM NO","CITY","HOUSE_NO","PHONE","DISEASE","STATE","DOA");
     printf("\n");
     liner(134);
     while(fread(&patient,sizeof(patient),1,fp) == 1){
       if(patient.doc.id == cur_doc_id){
         hasP = true;
         printPatient(patient);
       }
     }
     fclose(fp);
     if(!hasP){
      prittyLine( "You have no patients yet !" );
     }
     LABLE:
     printf("\n\n\t\t");
     liner(31);
     printf("\t\t\t\tOPERATIONS");
     printf("\n\t\t");
     liner(31);
     printf("\t\t*******************************\n");
     printf("\t\t* [1] Add test for a patient  *\n");
     printf("\t\t* [2] show tests of a patient *\n");
     printf("\t\t* [3] Generate report         *\n");
     printf("\t\t* [4] back <-                 *\n");
     printf("\t\t*******************************\n");
     printf("Enter your choice:");
     fflush(stdin);
     choice = getch();
     printf("%c",choice);
     printf("\n");
     switch(choice){
       case '1':
         printf("Enter patient ID : ");
         scanf(" %d",&id);
         addTest(id);
        break;
       case '2':
         printf("Enter patient ID : ");
         scanf(" %d",&id);
         showTest(id);
        break;
       case '3':
         printf("Enter patient ID : ");
         scanf(" %d",&id);
         generateReport(id);
        break;
       case '4':
         break;
       default:
        goto LABLE;
     }

}

void myReports(){
  system("cls");
  fp = fopen(patientfile,"rb");
  printf("\n");
  liner(55);
  printf("%-25s %-30s","PATIENT","REPORT");
  printf("\n");
  liner(55);
  printf("\n");
  while(fread(&patient,sizeof(patient),1,fp) == 1){
    if(patient.doc.id = cur_doc_id){
       found = true;
       printf("%-25s",patient.name);
       if(strlen(patient.rep.text) == 0)
         printf("%-30s" ,"Not reported");
       else
         printf("%-30s",patient.rep.text);
       printf("\n");
    }
  }
  if(!found)
    prittyLine(" You have no patients ");
  fclose(fp);
}
void getTestInput(){
 printf("Give Test Name : ");
 scanf(" %[^\n]s",test.name);
 printf("Enter Cost : ");
 scanf(" %d",&test.fee);
}
void addTest(int id){
  system("cls");
  if(id > pcount || id <= 0){
    prittyLine(" No Patient with the given ID ");
    return;
  }else{
    fp = fopen(patientfile,"rb+");
    while(fread(&patient,sizeof(patient),1,fp) == 1){
      if(patient.id == id){
        found = true;
        if(patient.doc.id == cur_doc_id){
         printf("\n");
         liner(27);
         printf("%-7s %-20s","ID","PATIENT");
         printf("\n");
         liner(27);
         printf("%-7d %-20s",patient.id,patient.name);
         printf("\n");
         if(patient.testIndex < 15){
            getTestInput();
            patient.test[patient.testIndex] = test;
            patient.testIndex++;
            patient.totalfee += test.fee;
            fseek(fp, ftell(fp) - sizeof (patient), 0);
            fwrite(&patient, sizeof (patient), 1, fp);
            prittyLine(" Laboratory Test Added Successfully! ");
         }else{
           prittyLine(" Maximum of test for a patient is reached (you can't add more) ");
         }
         fclose(fp);
         break;
        }else{
          prittyLine(" This Patient is not yours (Wrong Authentication) ");
          fclose(fp);
          break;
        }
      }
    }
    if(!found)
      prittyLine(" No Patient with the given ID ");
  }
  found = false;
  erasePlist();
  copy_patients();
}
void showTest(int id){
  system("cls");
  if(id > pcount || id <= 0){
    prittyLine(" No Patient with the given ID ");
    return;
  }else{
    fp = fopen(patientfile,"rb");
    while(fread(&patient,sizeof(patient),1,fp) == 1){
      if(patient.id == id){
        found = true;
        if(patient.doc.id == cur_doc_id){
         printf("\n\n\n");
         liner(27);
         printf("%-7s %-20s","ID","PATIENT");
         printf("\n");
         liner(27);
         printf("%-7d %-20s",patient.id,patient.name);
         printf("\n");
         if(patient.testIndex > 0){
             int i;
             printf("%-7s %-10s %-25s","#No","FEE","Test Name");
             printf("\n");
             for(i = 0 ; i<patient.testIndex ; i++){
              printf("%-7d %-10d %-25s",(i+1),patient.test[i].fee,patient.test[i].name);
              printf("\n");
             }
         }else{
           prittyLine(" No Tests are recorded for this patient ");
         }
         fclose(fp);
         break;
        }else{
          prittyLine(" This Patient is not yours (Wrong Authentication) ");
          fclose(fp);
          break;
        }
      }
    }
    if(!found)
      prittyLine(" No Patient with the given ID ");
  }
  found = false;
}
void generateReport(int id){
  system("cls");
  if(id > pcount || id <= 0){
    prittyLine(" No Patient with the given ID ");
    return;
  }else{
    fp = fopen(patientfile,"rb+");
    while(fread(&patient,sizeof(patient),1,fp) == 1){
      if(patient.id == id){
        found = true;
        if(patient.doc.id == cur_doc_id){
         printf("\n");
         liner(37);
         printf("%-7s %-30s","ID","PATIENT");
         printf("\n");
         liner(37);
         printf("%-7d %-30s",patient.id,patient.name);
         printf("\n");
         printf("REPORT  :-> %s\n\n",patient.rep.text);
         printf("Write a report :  ");
         scanf(" %[^\n]s",patient.rep.text);
         fseek(fp, ftell(fp) - sizeof (patient), 0);
         fwrite(&patient, sizeof (patient), 1, fp);
         prittyLine(" Report Generated Successfully! ");
         fclose(fp);
         break;
        }else{
          prittyLine(" This Patient is not yours (Wrong Authentication) ");
          fclose(fp);
          break;
        }
      }
    }
    if(!found)
      prittyLine(" No Patient with the given ID ");
  }
  found = false;
  erasePlist();
  copy_patients();
}
void editDocAuth(){
  system("cls");
  int id;
  fpass = fopen(logfile,"rb+");
  while(fread(&user,sizeof(user),1,fpass) == 1){
    if(user.id == cur_user_id){
        printf("\n");
        liner(50);
        printf("%-30s %-20s","NAME","PASSWORD");
        printf("\n");
        liner(50);
        printf("\n");
        printf("%-30s %-20s",user.name,user.password);
        prittyLine(" Edit Login Credential Info ");
        printf("Enter user name :");
        scanf(" %[^\n]s" , user.name);
        printf("\nEnter password for login credentials : ");
        scanf(" %[^\n]s" , user.password);
        fseek(fpass, ftell(fpass) - sizeof (user), 0);
        fwrite(&user, sizeof (user), 1, fpass);
        prittyLine(" Login Credential Updated Successfully! ");
        break;
    }
  }
  eraseUlist();
  copy_users();
}
/***
 Main Function.............
***/

void main(){
   init();
   copy_docs();
   copy_patients();
   copy_deprtment();
   copy_users();
   mainLogin();
}
/*********************** Main ADMIN menu********************************/
void mainMenu(){
   char choice,Mchoice;
   lable1:
   do{
       system("color F0");
       system("cls");
       printf("\n\n\t\t");
       liner(48);
       printf(" \n\t\t\t\t  MAIN MENU \n ");
       printf("\n\t\t");
       liner(48);
       printf("\n\n");
       printf("\t\t***********************************************\n");
       printf("\t\t*         [1] Add Record                      *\n");
       printf("\t\t*         [2] Show Record                     *\n");
       printf("\t\t*         [3] Search Record                   *\n");
       printf("\t\t*         [4] Edit Record                     *\n");
       printf("\t\t*         [5] Delete Record                   *\n");
       printf("\t\t*         [6] Patient Bill Information        *\n");
       printf("\t\t*         [7] Back <-                         *\n");
       printf("\t\t***********************************************\n");

       printf("Enter your choice:");
       Mchoice = getch();
       printf("%c",Mchoice);
       printf("\n");
       switch(Mchoice){
         case '1':
            {
            system("cls");
            system("color 02");
            printf("\t\t******************************\n");
            printf("\t\t*   [1] Add Patient          *\n");
            printf("\t\t*   [2] Add Doctor           *\n");
            printf("\t\t*   [3] Add Department       *\n");
            printf("\t\t*   [4] Add User             *\n");
            printf("\t\t******************************\n");
            printf("Enter your choice : ");
            choice = getch();
            printf("%c",choice);
            printf("\n");
             switch(choice){
               case '1':
                addPatient();
                break;
               case '2':
                addDoctor();
                break;
               case '3':
                addDepartment();
                break;
               case '4':
                addUser(0);
                break;
               default:
                goto lable1;
             }
            }
            break;
         case '2':
            {
            system("cls");
            system("color 0F");
            printf("\t\t******************************\n");
            printf("\t\t*   [1] Show Patients        *\n");
            printf("\t\t*   [2] Show Doctors         *\n");
            printf("\t\t*   [3] Show Departments     *\n");
            printf("\t\t*   [4] Show Users           *\n");
            printf("\t\t*   [5] Show Report          *\n");
            printf("\t\t******************************\n");
            printf("Enter your choice : ");
            choice = getch();
            printf("%c",choice);
            printf("\n");
             switch(choice){
               case '1':
                   {
                    if(head1 != NULL){
                      showPatients();
                      printf("\n\npress 's' for sorted View/and other key to return back .... ");
                      fflush(stdin);
                      if(getch() == 's')
                       sort(1);
                    }
                    else
                     prittyLine(" No Patients Available ");
                   }
                 break;
               case '2':
                   {
                    if(head2 != NULL){
                      showDoctors();
                      printf("\n\npress 's' for sorted View/and other key to return back .... ");
                      fflush(stdin);
                      if(getch() == 's')
                       sort(2);
                    }
                    else
                     prittyLine(" No Doctors Available ");
                   }
                  break;
               case '3':
                   {
                     if(head3  != NULL){
                       showDepartments();
                       printf("\n\npress 's' for sorted View/and other key to return back .... ");
                      fflush(stdin);
                      if(getch() == 's')
                       sort(3);
                     }
                     else
                      prittyLine(" No Departments Available ");
                   }
                  break;
               case '4':
                  {
                    if(head4 != NULL){
                      showUsers();
                      printf("\n\npress 's' for sorted View/and other key to return back .... ");
                      fflush(stdin);
                      if(getch() == 's')
                       sort(4);
                    }
                    else
                     prittyLine(" No Users Available ");

                  }
                 break;
               case '5':
                {
                    if(head1 != NULL)
                     showReport();
                    else
                     prittyLine(" No Patient Report Available ");
                     holdOn();

                }
                break;
               default:
                goto lable1;
             }
            }
            break;
         case '3':
            {
            system("cls");
            system("color 0D");
            printf("\t\t******************************\n");
            printf("\t\t*   [1] Search Patient       *\n");
            printf("\t\t*   [2] Search Doctor        *\n");
            printf("\t\t*   [3] Search Department    *\n");
            printf("\t\t*   [4] Search User          *\n");
            printf("\t\t******************************\n");
            printf("Enter your choice : ");
            choice = getch();
            printf("%c",choice);
            printf("\n");
             switch(choice){
               case '1':
                   {
                    if(head1 != NULL)
                     searchPatient();
                    else
                     prittyLine(" No Patients Available ");
                     holdOn();
                   }
                break;
               case '2':
                   {
                    if(head2 != NULL)
                     searchDoctor();
                    else
                     prittyLine(" No Doctors Available ");
                      holdOn();
                   }
                break;
               case '3':
                   {
                    if(head3 != NULL)
                     searchDepartment();
                    else
                     prittyLine(" No Departments Available ");
                      holdOn();
                   }
                break;
               case '4':
                   {
                    if(head4 != NULL)
                     searchUser();
                    else
                     prittyLine(" No Users Available ");
                      holdOn();
                   }
                break;
               default:
                goto lable1;
                break;
             }
            }
            break;
         case '4':
            {
            system("cls");
            system("color 0E");
            printf("\t\t******************************\n");
            printf("\t\t*    [1] Edit Patient        *\n");
            printf("\t\t*    [2] Edit Doctor         *\n");
            printf("\t\t*    [3] Edit Department     *\n");
            printf("\t\t*    [4] Edit User           *\n");
            printf("\t\t******************************\n");
            printf("Enter your choice : ");
            choice = getch();
            printf("%c",choice);
            printf("\n");
             switch(choice){
               case '1':
                   {
                    if(head1 != NULL)
                     editPatient();
                    else
                     prittyLine(" No Patients Available ");
                      holdOn();
                   }
                break;
               case '2':
                   {
                    if(head2 != NULL){
                      char yes;
                      do{
                        editDoctor(0);
                        printf("\nDo you want to edit more(y/n)\n");
                        fflush(stdin);
                        yes = getch();
                        printf("%c\n",yes);
                      }while(yes == 'y'|| yes == 'Y');
                    }
                    else
                     prittyLine(" No Doctors Available ");
                    holdOn();
                   }
                break;
               case '3':
                   {
                    if(head3 != NULL)
                     editDepartment();
                    else
                     prittyLine(" No Department Available ");
                      holdOn();
                   }
                break;
               case '4':
                   {
                    if(head4 != NULL)
                       editUser();
                    else
                     prittyLine(" No User Available ");
                      holdOn();
                   }
                break;
               default:
                goto lable1;
                break;
             }
            }
            break;
         case '5':
            {
            system("cls");
            system("color 0C");
            printf("\t\t******************************\n");
            printf("\t\t*    [1] Delete Patient      *\n");
            printf("\t\t*    [2] Delete Doctor       *\n");
            printf("\t\t*    [3] Delete Department   *\n");
            printf("\t\t*    [4] Delete User         *\n");
            printf("\t\t*    [5] Format DataBase     *\n");
            printf("\t\t******************************\n");
            printf("Enter your choice : ");
            choice = getch();
            printf("%c",choice);
            printf("\n");
             switch(choice){
               case '1':
                  if(head1 != NULL)
                    deletePatient();
                  else
                    prittyLine(" No Patient record to delete ");
                     holdOn();
                break;
               case '2':
                  if(head2 != NULL)
                    deleteDoctor();
                  else
                    prittyLine(" No Doctor record to delete ");
                     holdOn();
                break;
               case '3':
                  if(head3 != NULL)
                    deleteDepartment();
                  else
                    prittyLine(" No Department record to delete ");
                     holdOn();
                break;
               case '4':
                   if(head4 != NULL)
                     deleteUser();
                   else
                     prittyLine(" No User record to delete ");
                      holdOn();
                break;
               case '5':
                   if(head1 == NULL && head2 == NULL && head3 == NULL && head4 == NULL){
                      prittyLine(" Database is already empty ");
                   }
                   else
                      format();
                       holdOn();
                break;
               default:
                goto lable1;
                break;
             }
            }
            break;
         case '6':
            printBill();
            break;
         case '7':
            mainLogin();
         default:
            system("cls");
            goto lable1;

       }
   }while(Mchoice != '7');

}
/********************** Main DOctors Menu ***********************/
void docsMenu(){
   char choice;
   lable2:
   do{
   system("color F0");
   system("cls");
   printf("\n\n\t\t");
   liner(35);
   printf("\n\t\t\t  DOCTOR MENU \n ");
   printf("\n\t\t");
   liner(35);
   printf("\n\n");
   printf("\t\t***********************************\n");
   printf("\t\t*   [1] Show My Patients          *\n");
   printf("\t\t*   [2] Edit My Data              *\n");
   printf("\t\t*   [3] My Reports                *\n");
   printf("\t\t*   [4] Edit My Login Credentials *\n");
   printf("\t\t*   [5] Back <-                   *\n");
   printf("\t\t***********************************\n");
   printf("Enter your choice : ");
   choice = getch();
   printf("%c",choice);
   printf("\n");
   switch(choice){
      case '1':
         if(head1 == NULL)
           prittyLine(" NO Patient Records ");
         else{
           system("color 0A");
           showMyPatient();
         }
         holdOn();
         break;
      case '2':
         system("color 0E");
         editDoctor(cur_doc_id);
         holdOn();
         break;
      case '3':
          system("color 0B");
          myReports();
          holdOn();
          break;
      case '4':
          system("color 0E");
          editDocAuth();
          holdOn();
         break;
      case '5':
         mainLogin();
         break;
      default:
        goto lable2;
        break;
   }
  }while( choice != '5' );
}

/*********** Main Login Page ********************/
void mainLogin(){
   system("cls");
   lable3:
   printf("\n\n\t\t");
   liner(31);
   printf("\t\t    WELCOME TO OUR HOSPITAL\n");
   printf("\n \t\t\t    Login Page \n ");
   printf("\n\t\t");
   liner(31);
   char choice;
   printf("\n");
   printf("\t\t******************************\n");
   printf("\t\t* [1] Admin Login            *\n");
   printf("\t\t* [2] Doctor Login           *\n");
   printf("\t\t* [3] Register New User      *\n");
   printf("\t\t* [4] Exit                   *\n");
   printf("\t\t******************************\n");
   fflush(stdin);
   printf("\t\tEnter Your Choice : ");
   choice = getch();
   printf("%c",choice);
   printf("\n\n\n");
   switch(choice){
      case '1':
         if(head4 == NULL){
           printf("\t No Users In the DataBase \n");
           goto lable3;
         }
         else
           passInput(0);
         break;
      case '2':
         if(head4 == NULL){
            printf("\t No Users In the DataBase \n");
            goto lable3;
         }
         else
           passInput(1);
         break;
      case '3':
         addUser(1);
         break;
      case '4':
         updateCounts();
         printf("\n\n");
         liner(64);
         printf("\n\t\tTHANK YOU FOR USING OUR HOSPITAL\n\n");
         liner(64);
         printf("\n\n\n\n\n");
         exit(0);
         break;
      default:
        system("cls");
        goto lable3;
        break;
   }
}
/*********** Password Hider *******************/
void passInput(int type){
  char username[40];
  int verify=0;
  passlable:
      printf("\t\tEnter Username : ");
      scanf(" %[^\n]s",username);
      printf("\t\tEnter Password :");
      char pass[40],a;
      int i;
      fflush(stdin);
      for(i=0;;){
            a=getch();
            if((a>='a'&&a<='z')||(a>='A'&&a<='Z')||(a>='0'&&a<='9')){
               pass[i]=a;
               ++i;
               putch('*');
            }
            if(a=='\b'&&i>=1){
              printf("\b \b");
              --i;
            }
            if(a=='\r'){
              pass[i]='\0';
              system("cls");
              break;

            }
       }
       if(isAuth(username,pass,type)){
         system("color 09");
         gotoxy(50,20);
         printf("  ....<Acces Granted>....");
         gotoxy(50,25);
         printf("press anykey to continue............");
         getch();
         if(type == 0){
           mainMenu();
         }
         if(type == 1){
           docsMenu();
         }

       }else{
            ++verify;
            if(verify==3){
                gotoxy(50,20);
                printf("... YOU FAILED 3-TIMES ......");
                gotoxy(50,24);
                printf(".... PROGRAM STOPPED  .....");
                exit(0);
            }
            system("color 0C");
            printf("\n\t..<Access Denied>..\n");
            goto passlable;

       }
}
/*********** Password Authenticator ******************/
bool isAuth(char name[],char pass[],int type){
  fpass = fopen(logfile,"rb");
  while(fread(&user,sizeof(user),1,fpass)==1){
    if(user.type == type && (strcasecmp(user.name,name) == 0) && (strcmp(user.password,pass) == 0)){
      if(type == 1){
       cur_doc_id = user.docid;
      }
      cur_user_id = user.id;
      fclose(fpass);
      return true;
    }
  }
  fclose(fpass);
  return false;
}

/***** Initialize counts ******/
void init(){
  fcounts = fopen("counts.dat","rb");
  if(fread(&counter,sizeof(counter),1,fcounts) == 1){
    pcount = counter.pcount;
    dcount = counter.dcount;
    dpcount = counter.dpcount;
    ucount = counter.ucount;
  }else{
    updateCounts();
  }
  fclose(fcounts);
}
/*****reading doctors to the linked list****/
void copy_docs(){
  fd = fopen(docfile, "rb");
  while(fread(&doc,sizeof(doc),1,fd)==1){
    insertD(doc);
  }
  fclose(fd);
}
/******readings patients to the linked list*****/
void copy_patients(){
  fp = fopen(patientfile,"rb");
  while(fread(&patient,sizeof(patient),1,fp)==1){
    insertP(patient);
  }
  fclose(fp);
}
void copy_deprtment(){
  fdp = fopen(depfile,"rb");
  while(fread(&dep,sizeof(dep),1,fdp)==1){
   insertDp(dep);
  }
  fclose(fdp);
}
void copy_users(){
  fpass = fopen(logfile,"rb");
  while(fread(&user,sizeof(user),1,fpass) == 1){
    insertU(user);
  }
  fclose(fpass);
}
/*****display doctors to be assigned to the patient*****/
void display_list(){
      struct NodeD *tmp = head2;
      printf("\n");
      liner(86);
      printf("%-6s %-25s %-25s %-30s","ID","NAME","PECIALIZATION","WORKING SINCE");
      printf("\n");
      liner(86);
      printf("\n");
      while(tmp != NULL){
         printf("%-6d %-25s %-25s %-30s",tmp->Ddata.id,tmp->Ddata.name,tmp->Ddata.specialize,tmp->Ddata.since);
         printf("\n");
         tmp = tmp->next;
      }

}
/****assign doctor to a patient*****/
bool assign_doctor(int id){
 fd=fopen(docfile,"rb");
 while(fread(&doc,sizeof(doc),1,fd)==1){
   if(doc.id == id){
     patient.doc = doc;
     fclose(fd);
     return true;
   }
 }
 fclose(fd);
 return false;
}
/****assign department to a doctor***/
bool assign_department(int id){
 fdp=fopen(depfile,"rb");
 while(fread(&dep,sizeof(dep),1,fdp)==1){
   if(dep.id == id){
     doc.dep = dep;
     fclose(fdp);
     return true;
   }
 }
 fclose(fdp);
 return false;
}

void updateCounts(){
  fcounts = fopen("counts.dat","wb");
  counter.dcount = dcount;
  counter.pcount = pcount;
  counter.dpcount = dpcount;
  counter.ucount = ucount;
  fwrite(&counter,sizeof(counter),1,fcounts);
  fclose(fcounts);
}

void sort(int type){
  int swapped;
  if(type == 1){
    struct NodeP *tmp;
    struct NodeP *tmp2=NULL;
    do{
      swapped = 0;
      tmp = head1;
      while(tmp->next != tmp2){
        if(strcasecmp(tmp->Pdata.name,tmp->next->Pdata.name) > 0){
            Patient ptmp=tmp->next->Pdata;
            tmp->next->Pdata = tmp->Pdata;
            tmp->Pdata = ptmp;
            swapped = 1;
        }
        tmp = tmp->next;
      }
      tmp2 = tmp;
    }while(swapped);
    sortedPatient();
  }else if(type == 2){
    struct NodeD *tmp;
    struct NodeD *tmp2=NULL;
    do{
      swapped = 0;
      tmp = head2;
      while(tmp->next != tmp2){
        if(strcasecmp(tmp->Ddata.name ,tmp->next->Ddata.name) > 0){
            Doctor dtmp=tmp->next->Ddata;
            tmp->next->Ddata = tmp->Ddata;
            tmp->Ddata = dtmp;
            swapped = 1;
        }
        tmp = tmp->next;
      }
      tmp2 = tmp;
    }while(swapped);
    sortedDoctor();
  }else if(type == 3 ){
     struct NodeDp *tmp;
     struct NodeDp *tmp2=NULL;
    do{
      swapped = 0;
      tmp = head3;
      while(tmp->next != tmp2){
        if(strcasecmp(tmp->Dpdata.name ,tmp->next->Dpdata.name) > 0){
            Department dtmp=tmp->next->Dpdata;
            tmp->next->Dpdata = tmp->Dpdata;
            tmp->Dpdata = dtmp;
            swapped = 1;
        }
        tmp = tmp->next;
      }
      tmp2 = tmp;
    }while(swapped);
    sortedDepartment();
  }else{
    struct NodeU *tmp;
     struct NodeU *tmp2=NULL;
    do{
      swapped = 0;
      tmp = head4;
      while(tmp->next != tmp2){
        if(strcasecmp(tmp->Udata.name ,tmp->next->Udata.name) > 0){
            User utmp=tmp->next->Udata;
            tmp->next->Udata = tmp->Udata;
            tmp->Udata = utmp;
            swapped = 1;
        }
        tmp = tmp->next;
      }
      tmp2 = tmp;
    }while(swapped);
    sortedUser();
  }

}

void sortedPatient(){
 system("cls");
 struct NodeP *p = head1;
 printf("\n");
 liner(134);
 printf("%-4s %-20s %-3s %-5s %-8s %-15s %-9s %-15s %-25s %-10s %-20s","ID","NAME","SEX","AGE","ROOM NO","CITY","HOUSE_NO","PHONE","DISEASE","STATE","DOA");
 printf("\n");
 liner(134);
 printf("\n");
 while(p != NULL){
 printf("%-4d %-20s %-3c %-5d %-8s %-15s %-9s %-15s %-25s",p->Pdata.id,p->Pdata.name,p->Pdata.sex,p->Pdata.age,p->Pdata.roomNo,p->Pdata.ad.city,p->Pdata.ad.houseNo,p->Pdata.ad.phone,p->Pdata.disease);
 if(p->Pdata.pstate == 1)
   printf(" %-10s","P.D.O");
 else
   printf(" %-10s","Emergency");
 printf(" %-20s",p->Pdata.date);
 printf("\n");
 p = p->next;
 }
 holdOn();
}
void sortedDoctor(){
 system("cls");
 struct NodeD *d = head2;
 printf("\n");
 liner(145);
 printf("%-4s %-20s %-3s %-15s %-25s %-25s %-8s %-15s %-30s","ID","NAME","SEX","DEPARTMENT","SPECIALIZATION","CITY","HOUSE_NO","PHONE","WORKING SINCE");
 printf("\n");
 liner(145);
 printf("\n");
 while(d != NULL){
   printf("%-4d %-20s %-3c %-15s %-25s %-25s %-8s %-15s %-30s",d->Ddata.id,d->Ddata.name,d->Ddata.sex,d->Ddata.dep.name,d->Ddata.specialize,d->Ddata.ad.city,d->Ddata.ad.houseNo,d->Ddata.ad.phone,d->Ddata.since);
   printf("\n\n");
   d = d->next;
 }
 holdOn();
}
void sortedDepartment(){
 system("cls");
 struct NodeDp *dp = head3;
 printf("\n");
 liner(31);
 printf("%-6s %-25s","ID","NAME");
 printf("\n");
 liner(31);
 printf("\n");
 while(dp != NULL){
   printf("%-6d %-25s",dp->Dpdata.id,dp->Dpdata.name);
   printf("\n");
  dp = dp->next;
 }
 holdOn();
}
void sortedUser(){
	 system("cls");
	 struct NodeU *u = head4;
	 printf("\n");
	 liner(84);
	 printf("%-4s %-20s %-25s %-10s %-15s %-10s","ID","USERNAME","CITY","HOUSE_NO","PHONE","TYPE");
	 printf("\n");
	 liner(84);
	while(u != NULL){
      printf("%-4d %-20s %-25s %-10s %-15s",u->Udata.id,u->Udata.name,u->Udata.ad.city,u->Udata.ad.houseNo,u->Udata.ad.phone);
	  if(u->Udata.type == 0){
	   printf(" %-10s","Admin");
	  }else{
	    printf(" %-10s","Doctor");
	  }
	  printf("\n");
	  u = u->next;
	}
	holdOn();
}
