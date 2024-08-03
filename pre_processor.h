#ifndef PRE_PROCESSOR_H
#define PRE_PROCESSOR_H

typedef struct {
    char * name;
    char * body;
} Macr;

void initialize_macrs();
int add_macr(const char *name, const char *body);
int is_macr_name_valid(const char *name);
void process_macros_from_string(const char *assembly_code);

#endif 
