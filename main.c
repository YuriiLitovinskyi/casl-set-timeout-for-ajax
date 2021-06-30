#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include <stdbool.h>

// Current program will set new timeout for all devices with protocol Ajax, connected to CASL Cloud

// function for insertion into subject[] at position pos
void append(char subject[], const char insert[], int pos) {
    char buf[256] = {};

    strncpy(buf, subject, pos); // copy at most first pos characters
    int len = strlen(buf);
    strcpy(buf+len, insert); // copy all of insert[] at the end
    len += strlen(insert);  // increase the length by length of insert[]
    strcpy(buf+len, subject+pos); // copy the rest

    strcpy(subject, buf);   // copy it back to subject
    // deallocate buf[] here, if used malloc()
}

// function for checking if entered value is a number
bool isNumber(const char value[]){
	int length, i;
	
	length = strlen(value);
	for(i = 0; i < length; i++){
		if(!isdigit(value[i])){		
			return false;
		}		
	}
	return true;
}

int main(int argc, char *argv[]) {
	
    sqlite3 *db;
    char *err_msg = 0;
    char newTimeout[256];
      
    // Get value from users input
    printf("Enter new timeout for all Ajax devices in seconds: ");
    gets(newTimeout);
    
    // Check if value is a number
    if(isNumber(newTimeout) == false){
    	printf("\nEntered value is not a number! \n");
    	
		system("pause"); 
		       
        return 1;
	}
    
    // Open CASL DB
    int rc = sqlite3_open("data.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open CASL Cloud database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        system("pause");
        
        return 1;
    }
      
    // Change timeout
    char sqlSetAjaxTimeout[256] = "UPDATE device SET timeout =  WHERE dict_device_type = 25";
	append(sqlSetAjaxTimeout, newTimeout, 28);  
    //printf(sqlSetAjaxTimeout);  

    rc = sqlite3_exec(db, sqlSetAjaxTimeout, 0, 0, &err_msg);
     
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        system("pause");
        
        return 1;
    } 
    
    sqlite3_close(db);  
    
    printf("\nTimeout was changed successfully to %s seconds! \n", newTimeout);
    
    system("pause");   
       
	return 0;
}
