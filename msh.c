#include <stdio.h> // printf(), fgets(), stdin, etc.
#include <dlfcn.h> // dlerror(), etc.
#include <unistd.h> // fork(), execvp(), etc.
#include <stdlib.h> // exit(), etc.
#include <sys/wait.h> // wait(), waitpid(), etc.
#include <string.h> // strtok(), etc.

// Macros defined for per program instrucions for implementation simplicity
#define COMMAND_LENGTH 201
#define ARGS_LENGTH 21
#define MAX_ARGS 21
#define MAX_PLUGINS 11

// Plugin struct
typedef struct {
    char name[COMMAND_LENGTH];
    void * handle;
} Plugin;

// Function declarations
Plugin * find_plugin(char * name, Plugin * plugins_list, int num_plugins);



int main() {
    char command[COMMAND_LENGTH]; // Holds length of user command line
    char * args[MAX_ARGS]; // Holds the arguments inputted
    Plugin plugins_list[MAX_PLUGINS]; // Holds the instances of plugins inputted
    int num_plugins = 0; // Number of plugins inputted

    while(1) { // runs until exited

        // Prompt user
        printf("> ");
        fflush(stdout);

        /* Read the command inputted by the user from the standard input line,
           and cleanup the new line character */
        fgets(command, COMMAND_LENGTH, stdin);
        command[strcspn(command, "\n")] = '\0';

        // Split command into tokens
        char * token = strtok(command, " "); // whitespace delineates new token
        int i = 0;
        while(token != NULL && i < MAX_ARGS) {
            args[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        args[i] = NULL; // terminate the end of the argument list

        // No command was entered; - ensuring no error on empty input
        if(i == 0) {
            continue;
        }

        if(strcmp(args[0], "exit") == 0) {
            // Exit command - terminate
            exit(0);
        } else if(strcmp(args[0], "load") == 0 && i >= 2 && num_plugins < MAX_PLUGINS) {
            // Load command


            Plugin plugin;

            char plugin_path[30];
            snprintf(plugin_path, sizeof(plugin_path), "./%s.so", args[1]);


            void * handle = dlopen(plugin_path, RTLD_LAZY);
            if(!handle){
                printf("Error: Plugin %s initialization failed!\n", args[1]);
                continue;
            }

            // Check if plugin already exists in plugin array
            Plugin * plugin_ptr = find_plugin(args[1], plugins_list, num_plugins);
            if(!plugin_ptr) {
                // Plugin does not already exist

                int(* initialize)();

                initialize = dlsym(handle, "initialize");
                char * error = dlerror();
                if(dlerror()) {
                    fprintf(stderr, "%s\n", error);
                    return 1;
                }

                if(initialize() != 0) {
                    printf("Error: Plugin %s initialization failed!\n", args[1]);
                    dlclose(handle);
                    continue;
                }

                strcpy(plugin.name, args[1]);
                plugin.handle = handle;
                // Add plugin to list
                plugins_list[num_plugins] = plugin;
                num_plugins++;



            } else {
                // Plugin exists
                printf("Error: Plugin %s initialization failed!\n", args[1]);
                strcpy(plugin_ptr->name, args[1]);
                plugin_ptr->handle = handle;
            }

        } else if(find_plugin(args[0], plugins_list, num_plugins)) {
            // Run plugin
            Plugin * plugin_ptr = find_plugin(args[0], plugins_list, num_plugins);

            int (* run)();
            run = dlsym(plugin_ptr->handle, "run");

            char * error = dlerror();
            if(error != NULL) {
                fprintf(stderr, "%s\n", error);
                return 1;
            }

            if(run != NULL) {
                run(args);
            } else {
                fprintf(stderr, "Error: Plugin %s does not contain a 'run' function\n", args[0]);
            }

        } else {
            // Execute non built-in command
            pid_t pid = fork();
            if(pid == -1) {
                // error on fork
                perror("fork");
                exit(1);
            } else if(pid == 0) {
                // child process
                execvp(args[0], args);
                printf("Error: Plugin %s initialization failed!\n", args[1]); // return of execvp indicates failure
                exit(1);
            } else {
                // parent process
                int status;
                if(waitpid(pid, &status, 0) == -1) {
                    perror("waitpid");
                    exit(1);
                }
            }


        }
    }
    return 0;
}

// Find a plugin
Plugin * find_plugin(char * name, Plugin * plugins_list, int num_plugins) {
    for(int i = 0; i < num_plugins; i++) {
        if(strcmp(name, plugins_list[i].name) == 0) {
            return &plugins_list[i];
        }
    }
    // No plugin within list
    return NULL;

}
