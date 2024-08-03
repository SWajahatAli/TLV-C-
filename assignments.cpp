// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

// #define MAX_TAG_LENGTH 3
// #define MAX_LENGTH_LENGTH 3
// #define MAX_VALUE_LENGTH 25

// typedef struct {
//     char account_no[25];
//     long long avail_bal;
//     long long ledger_bal;
//     char update_date[9];
//     char update_time[7];
// } AccountInfo;

// void print_account_info(const AccountInfo *info);
// void process_record(AccountInfo *info);
// void print_sql_statement(AccountInfo *info);
// void parse_input(char *input, int commit_limit);

// int main(int argc, char *argv[]) {
//     int commit_limit = 0;

//     if (argc > 1 && strcmp(argv[1], "-c") == 0) {
//         if (argc > 2) {
//             commit_limit = atoi(argv[2]);
//         }
//     }

//     char input[1024];
//     while (fgets(input, sizeof(input), stdin)) {
//         parse_input(input, commit_limit);
//     }

//     return 0;
// }

// void parse_input(char *input, int commit_limit) {
//     char *pos = input;
//     char tag[MAX_TAG_LENGTH + 1];
//     int length;
//     char value[MAX_VALUE_LENGTH + 1];
//     AccountInfo info = {"", 0, 0, "", ""};
//     int commit_counter = 0;
//     print_account_info(&info);
    
//     while (*pos) {
//         // Read tag
//         strncpy(tag, pos, MAX_TAG_LENGTH);
//         tag[MAX_TAG_LENGTH] = '\0';
//         pos += MAX_TAG_LENGTH;

//         // Read length
//         char length_str[MAX_LENGTH_LENGTH + 1];
//         strncpy(length_str, pos, MAX_LENGTH_LENGTH);
//         length_str[MAX_LENGTH_LENGTH] = '\0';
//         length = atoi(length_str);
//         pos += MAX_LENGTH_LENGTH;

//         // Read value
//         strncpy(value, pos, length);
//         value[length] = '\0';
//         pos += length;

//         // Process the tag
//         if (strcmp(tag, "NA") == 0) {
//             strncpy(info.account_no, value, length);
//             info.account_no[length] = '\0';
//         } else if (strcmp(tag, "AB") == 0) {
//             info.avail_bal = atoll(value + 1) * (value[0] == 'C' ? 1 : -1);
//         } else if (strcmp(tag, "LB") == 0) {
//             info.ledger_bal = atoll(value + 1) * (value[0] == 'C' ? 1 : -1);
//         } else if (strcmp(tag, "UT") == 0) {
//             strncpy(info.update_date, value, 10);
//             info.update_date[8] = '\0';
//             strncpy(info.update_time, value + 11, 8);
//             info.update_time[6] = '\0';
//         }
//     }

//     print_sql_statement(&info);

//     if (commit_limit > 0) {
//         commit_counter++;
//         if (commit_counter >= commit_limit) {
//             printf("COMMIT;\n");
//             commit_counter = 0;
//         }
//     }
// }

// void print_sql_statement(AccountInfo *info) {
//     printf("INSERT INTO T_ACCOUNT (ACCOUNT_NO, AVAIL_BAL, LEDGER_BAL, UPD_DATE, UPD_TIME) VALUES ('%s', %lld, %lld, %s, %s);\n",
//            info->account_no, info->avail_bal, info->ledger_bal, info->update_date, info->update_time);
// }

// // Function to print AccountInfo
// void print_account_info(const AccountInfo *info) {
//     printf("Account Number: %s\n", info->account_no);
//     printf("Available Balance: %lld\n", info->avail_bal);
//     printf("Ledger Balance: %lld\n", info->ledger_bal);
//     printf("Update Date: %s\n", info->update_date);
//     printf("Update Time: %s\n", info->update_time);
// }



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TAG_LENGTH 2
#define MAX_LENGTH_LENGTH 2
#define MAX_VALUE_LENGTH 25

typedef struct {
    char account_no[25];
    long long avail_bal;
    long long ledger_bal;
    char update_date[9];
    char update_time[7];
} AccountInfo;

void print_sql_statement(const AccountInfo *info);

void parse_input(char *input, int commit_limit) {
    char *pos = input;
    char tag[MAX_TAG_LENGTH + 1];
    int length;
    char value[MAX_VALUE_LENGTH + 1];
    AccountInfo info = {"", 0, 0, "", ""};
    int commit_counter = 0;

    while (*pos) {
        // Skip any whitespace
        while (isspace(*pos)) pos++;

        // Check for end of input
        if (*pos == '\0') break;

        // Read tag
        strncpy(tag, pos, MAX_TAG_LENGTH);
        tag[MAX_TAG_LENGTH] = '\0';
        pos += MAX_TAG_LENGTH;

        // Read length
        char length_str[MAX_LENGTH_LENGTH + 1];
        strncpy(length_str, pos, MAX_LENGTH_LENGTH);
        length_str[MAX_LENGTH_LENGTH] = '\0';
        length = atoi(length_str);
        pos += MAX_LENGTH_LENGTH;

        // Read value
        if (length > MAX_VALUE_LENGTH) length = MAX_VALUE_LENGTH; // Ensure no buffer overrun
        strncpy(value, pos, length);
        value[length] = '\0';
        pos += length;

        // Debug prints
        printf("Tag: [%s]\n", tag);
        printf("Length: %d\n", length);
        printf("Value: [%s]\n", value);

        // Process the tag
        if (strcmp(tag, "NA") == 0) {
            strncpy(info.account_no, value, sizeof(info.account_no) - 1);
            info.account_no[sizeof(info.account_no) - 1] = '\0';
        } else if (strcmp(tag, "AB") == 0) {
            info.avail_bal = atoll(value + 1) * (value[0] == 'C' ? 1 : -1);
        } else if (strcmp(tag, "LB") == 0) {
            info.ledger_bal = atoll(value + 1) * (value[0] == 'C' ? 1 : -1);
        } else if (strcmp(tag, "UT") == 0) {
            strncpy(info.update_date, value, 10);
            info.update_date[10] = '\0';
            strncpy(info.update_time, value + 11, 8);
            info.update_time[8] = '\0';
        } else {
            // Handle unknown tags
            printf("Unknown tag: %s\n", tag);
        }
    }

    print_sql_statement(&info);

    if (commit_limit > 0) {
        commit_counter++;
        if (commit_counter >= commit_limit) {
            printf("COMMIT;\n");
            commit_counter = 0;
        }
    }
}

void print_sql_statement(const AccountInfo *info) {
    printf("INSERT INTO T_ACCOUNT (ACCOUNT_NO, AVAIL_BAL, LEDGER_BAL, UPD_DATE, UPD_TIME) VALUES ('%s', %lld, %lld, '%s', '%s');\n",
           info->account_no, info->avail_bal, info->ledger_bal, info->update_date, info->update_time);
}

int main(int argc, char *argv[]) {
    int commit_limit = 0;

    if (argc > 1 && strcmp(argv[1], "-c") == 0) {
        if (argc > 2) {
            commit_limit = atoi(argv[2]);
        }
    }

    char input[1024];
    while (fgets(input, sizeof(input), stdin)) {
        parse_input(input, commit_limit);
    }

    return 0;
}