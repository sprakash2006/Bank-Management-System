#include <stdio.h>
#include <string.h>

void create_account();
void deposit_money();
void withdraw_money();
void check_balance();
void transfer_money();
void view_account_details();
void update_account_info();
void close_account();
void list_all_accounts();
int account_exists(int acc_no);

const char *ACCOUNT_FILE = "account.txt";

typedef struct
{
  char name[50];
  int acc_no;
  float balance;
} Account;

int main()
{
  printf("\n");
  printf("\n                   The banks                                       ");
  while (1)
  {
    int choice;
    printf("\n\n                  bank of india       \n ");
    printf("\n   *** Bank Management System ***");
    printf("\n 1. Create Account");
    printf("\n 2. Deposit Money");
    printf("\n 3. Withdraw Money");
    printf("\n 4. Check Balance");
    printf("\n 5. Transfer Money");
    printf("\n 6. View Account Details");
    printf("\n 7. Update Account Information");
    printf("\n 8. Close Account");
    printf("\n 9. List All Accounts");
    printf("\n 10. Exit");
    printf("\nEnter your choice: \n ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
      create_account();
      break;
    case 2:
      deposit_money();
      break;
    case 3:
      withdraw_money();
      break;
    case 4:
      check_balance();
      break;
    case 5:
      transfer_money();
      break;
    case 6:
      view_account_details();
      break;
    case 7:
      update_account_info();
      break;
    case 8:
      close_account();
      break;
    case 9:
      list_all_accounts();
      break;
    case 10:
      printf("\nClosing the Bank, Thanks for your visit.\n");
      return 0;
    default:
      printf("\nInvalid choice!\n");
    }
  }
  printf("\033[0");
}

int account_exists(int acc_no)
{
  FILE *file = fopen(ACCOUNT_FILE, "r");
  if (file == NULL)
    return 0;

  Account acc;
  char line[256];
  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, "%d,%[^,],%f", &acc.acc_no, acc.name, &acc.balance) == 3)
    {
      if (acc.acc_no == acc_no)
      {
        fclose(file);
        return 1;
      }
    }
  }
  fclose(file);
  return 0;
}

void create_account()
{
  Account acc;
  FILE *file = fopen(ACCOUNT_FILE, "a");
  if (file == NULL)
  {
    printf("\nUnable to open file!!");
    return;
  }

  while ((getchar()) != '\n')
    ;

  printf("\nEnter your name: ");
  if (fgets(acc.name, sizeof(acc.name), stdin) != NULL)
  {
    acc.name[strcspn(acc.name, "\n")] = '\0';
  }

  do
  {
    printf("Enter your account number (positive number): ");
    if (scanf("%d", &acc.acc_no) != 1 || acc.acc_no <= 0)
    {
      printf("Invalid input! Please enter a positive number.\n");
      while (getchar() != '\n')
        ;
      continue;
    }
    if (account_exists(acc.acc_no))
    {
      printf("Account number already exists! Please choose another.\n");
      continue;
    }
    break;
  } while (1);

  acc.balance = 0;

  fprintf(file, "%d,%s,%.2f\n", acc.acc_no, acc.name, acc.balance);
  fclose(file);
  printf("\nAccount created successfully!");
}

void deposit_money()
{
  FILE *file = fopen(ACCOUNT_FILE, "r");
  if (file == NULL)
  {
    printf("Unable to open account file!!");
    return;
  }

  FILE *temp = fopen("temp.txt", "w");
  if (temp == NULL)
  {
    printf("Unable to create temporary file!");
    fclose(file);
    return;
  }

  int acc_no;
  float money;
  Account acc;
  int found = 0;
  char line[256];

  printf("Enter your account number: ");
  scanf("%d", &acc_no);
  printf("Enter amount to deposit: ");
  scanf("%f", &money);

  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, "%d,%[^,],%f", &acc.acc_no, acc.name, &acc.balance) == 3)
    {
      if (acc.acc_no == acc_no)
      {
        acc.balance += money;
        found = 1;
      }
      fprintf(temp, "%d,%s,%.2f\n", acc.acc_no, acc.name, acc.balance);
    }
  }

  fclose(file);
  fclose(temp);
  remove(ACCOUNT_FILE);
  rename("temp.txt", ACCOUNT_FILE);

  if (found)
  {
    printf("Successfully deposited Rs.%.2f \n New balance is Rs.%.2f", money, acc.balance);
  }
  else
  {
    printf("Account no %d was not found in records.", acc_no);
  }
}

void withdraw_money()
{
  FILE *file = fopen(ACCOUNT_FILE, "r");
  if (file == NULL)
  {
    printf("\nUnable to open the account file!!!.\n");
    return;
  }

  FILE *temp = fopen("temp.txt", "w");
  if (temp == NULL)
  {
    printf("Unable to create temporary file!");
    fclose(file);
    return;
  }

  int acc_no;
  float money;
  Account acc;
  int found = 0;
  char line[256];

  printf("Enter your account number: ");
  scanf("%d", &acc_no);
  printf("Enter the amount you wish to withdraw: ");
  scanf("%f", &money);

  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, "%d,%[^,],%f", &acc.acc_no, acc.name, &acc.balance) == 3) // sscanf is used to read the data from the file and store it in the acc variable
    {
      if (acc.acc_no == acc_no)
      {
        found = 1;
        if (acc.balance >= money)
        {
          acc.balance -= money; // acc.balance = acc.balance - money
          fprintf(temp, "%d,%s,%.2f\n", acc.acc_no, acc.name, acc.balance);
          printf("Successfully withdrawn Rs.%.2f. Remaining balance is Rs.%.2f", money, acc.balance);
        }
        else
        {
          fprintf(temp, "%d,%s,%.2f\n", acc.acc_no, acc.name, acc.balance);
          printf("Insufficient balance!");
        }
      }
      else
      {
        fprintf(temp, "%d,%s,%.2f\n", acc.acc_no, acc.name, acc.balance);
      }
    }
  }

  fclose(file);
  fclose(temp);
  remove(ACCOUNT_FILE);
  rename("temp.txt", ACCOUNT_FILE);

  if (!found)
  {
    printf("Money could not be withdrawn as the Account no %d was not found in records.", acc_no);
  }
}

void check_balance()
{
  FILE *file = fopen(ACCOUNT_FILE, "r");
  if (file == NULL)
  {
    printf("\nUnable to open file!!");
    return;
  }

  int acc_no;
  Account acc;
  int found = 0;
  char line[256];

  printf("Enter your account number: ");
  scanf("%d", &acc_no);

  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, "%d,%[^,],%f", &acc.acc_no, acc.name, &acc.balance) == 3)
    {
      if (acc.acc_no == acc_no)
      {
        printf("\nYour current balance is Rs.%.2f", acc.balance);
        found = 1;
        break;
      }
    }
  }
  fclose(file);

  if (!found)
  {
    printf("\nAccount No:%d was not found.\n", acc_no);
  }
}

void transfer_money()
{
  FILE *file = fopen(ACCOUNT_FILE, "r");
  if (file == NULL)
  {
    printf("\nUnable to open the account file!!!.\n");
    return;
  }

  FILE *temp = fopen("temp.txt", "w");
  if (temp == NULL)
  {
    printf("Unable to create temporary file!");
    fclose(file);
    return;
  }

  int acc_no1, acc_no2;
  float money;
  Account acc;
  int found1 = 0, found2 = 0;
  float balance1, balance2;
  char line[256];

  printf("Enter your account number to transfer from: ");
  scanf("%d", &acc_no1);
  printf("Enter the account number to transfer to: ");
  scanf("%d", &acc_no2);

  // Add check for same account numbers
  if (acc_no1 == acc_no2)
  {
    printf("Cannot transfer money to the same account!\n");
    fclose(file);
    fclose(temp);
    return;
  }

  printf("Enter the amount you wish to transfer: ");
  scanf("%f", &money);

  // Add validation for transfer amount
  if (money <= 0)
  {
    printf("Invalid amount! Please enter a positive value.\n");
    fclose(file);
    fclose(temp);
    return;
  }

  // First pass: Check if both accounts exist and verify balance
  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, "%d,%[^,],%f", &acc.acc_no, acc.name, &acc.balance) == 3)
    {
      if (acc.acc_no == acc_no1)
      {
        found1 = 1;
        balance1 = acc.balance;
      }
      if (acc.acc_no == acc_no2)
      {
        found2 = 1;
        balance2 = acc.balance;
      }
    }
  }

  if (!found1)
  {
    printf("Account number %d not found.\n", acc_no1);
    fclose(file);
    fclose(temp);
    return;
  }
  if (!found2)
  {
    printf("Account number %d not found.\n", acc_no2);
    fclose(file);
    fclose(temp);
    return;
  }
  if (balance1 < money)
  {
    printf("Insufficient balance in account number %d.\n", acc_no1);
    fclose(file);
    fclose(temp);
    return;
  }

  // Second pass: Perform the transfer
  rewind(file);
  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, "%d,%[^,],%f", &acc.acc_no, acc.name, &acc.balance) == 3)
    {
      if (acc.acc_no == acc_no1)
      {
        acc.balance -= money;
      }
      else if (acc.acc_no == acc_no2)
      {
        acc.balance += money;
      }
      fprintf(temp, "%d,%s,%.2f\n", acc.acc_no, acc.name, acc.balance);
    }
  }

  fclose(file);
  fclose(temp);
  remove(ACCOUNT_FILE);
  rename("temp.txt", ACCOUNT_FILE);

  printf("Successfully transferred Rs.%.2f from account number %d to account number %d.\n",
         money, acc_no1, acc_no2);
}

void view_account_details()
{
  FILE *file = fopen(ACCOUNT_FILE, "r");
  if (file == NULL)
  {
    printf("\nUnable to open file!!");
    return;
  }

  int acc_no;
  Account acc;
  int found = 0;
  char line[256];

  printf("Enter your account number: ");
  scanf("%d", &acc_no);

  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, "%d,%[^,],%f", &acc.acc_no, acc.name, &acc.balance) == 3)
    {
      if (acc.acc_no == acc_no)
      {
        printf("\nAccount Details:");
        printf("\nName: %s", acc.name);
        printf("\nAccount Number: %d", acc.acc_no);
        printf("\nBalance: Rs.%.2f", acc.balance);
        found = 1;
        break;
      }
    }
  }
  fclose(file);

  if (!found)
  {
    printf("\nAccount No:%d was not found.\n", acc_no);
  }
}




void update_account_info()
{
  FILE *file = fopen(ACCOUNT_FILE, "r");
  if (file == NULL)
  {
    printf("\nUnable to open file!!");
    return;
  }

  FILE *temp = fopen("temp.txt", "w");
  if (temp == NULL)
  {
    printf("\nUnable to create temporary file!!");
    fclose(file);
    return;
  }

  int acc_no;
  Account acc;
  int found = 0;
  char line[256];

  printf("Enter account number to update: ");
  scanf("%d", &acc_no);
  while (getchar() != '\n')
    ; // Clear input buffer

  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, "%d,%[^,],%f", &acc.acc_no, acc.name, &acc.balance) == 3)
    {
      if (acc.acc_no == acc_no)
      {
        found = 1;
        printf("Enter new name: ");
        if (fgets(acc.name, sizeof(acc.name), stdin) != NULL)
        {
          acc.name[strcspn(acc.name, "\n")] = '\0';
        }
        fprintf(temp, "%d,%s,%.2f\n", acc.acc_no, acc.name, acc.balance);
      }
      else
      {
        fprintf(temp, "%d,%s,%.2f\n", acc.acc_no, acc.name, acc.balance);
      }
    }
  }

  fclose(file);
  fclose(temp);
  remove(ACCOUNT_FILE);
  rename("temp.txt", ACCOUNT_FILE);

  if (found)
  {
    printf("\nAccount information updated successfully!\n");
  }
  else
  {
    printf("\nAccount number %d not found.\n", acc_no);
  }
}

void close_account()
{
  FILE *file = fopen(ACCOUNT_FILE, "r");
  if (file == NULL)
  {
    printf("\nUnable to open file!!");
    return;
  }
  FILE *temp = fopen("temp.txt", "w");
  if (temp == NULL)
  {
    printf("\nUnable to create temporary file!!");
    fclose(file);
    return;
  }

  int acc_no, found = 0;
  Account acc;
  char line[256];
  printf("Enter account number to close: ");
  scanf("%d", &acc_no);

  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, "%d,%[^,],%f", &acc.acc_no, acc.name, &acc.balance) == 3)
    {
      if (acc.acc_no == acc_no)
      {
        found = 1;
        continue;
      }
      fprintf(temp, "%d,%s,%.2f\n", acc.acc_no, acc.name, acc.balance);
    }
  }

  fclose(file);
  fclose(temp);
  remove(ACCOUNT_FILE);
  rename("temp.txt", ACCOUNT_FILE);

  if (found)
    printf("\nAccount closed successfully!\n");
  else
    printf("\nAccount number %d not found.\n", acc_no);
}

void list_all_accounts()
{
  FILE *file = fopen(ACCOUNT_FILE, "r");
  if (file == NULL)
  {
    printf("\nUnable to open file!!");
    return;
  }

  Account acc;
  char line[256];

  printf("\nList of All Accounts:\n");
  printf("------------------------------------\n");
  printf("%-10s %-20s %-10s\n", "Acc No", "Name", "Balance");
  printf("------------------------------------\n");

  while (fgets(line, sizeof(line), file))
  {
    if (sscanf(line, "%d,%[^,],%f", &acc.acc_no, acc.name, &acc.balance) == 3)
    {
      printf("%-10d %-20s Rs.%.2f\n", acc.acc_no, acc.name, acc.balance);
    }
  }
  fclose(file);
}

  fclose(file);
  printf("\nAccount No:%d was not found.\n", acc_no);

}
