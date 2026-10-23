#include "./../../myformat.cpp"

#include "./../Onegin/strfuncs.h"
#include "./../Onegin/io.h"

#include "./../Stack/stack.h"

#include "enum.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

//----------------------------------------------------------------------------------------------------------------

#define MAX_LABEL_QTY 1000
//----------------------------------------------------------------------------------------------------------------

enum asm_err_n
{
    asm_err_NO_ERR          = 0,
    asm_err_WRONG_LABEL     = 1,
    asm_err_WRONG_CMD       = 2,
    asm_err_WRONG_ARG       = 3,
    asm_err_ARG_QTY_EXC     = 4,
    asm_err_LABEL_REDFNTN   = 5,
    asm_err_MISS_LABEL      = 6,
    asm_err_LABEL_NOT_EXST  = 7,
};

str_t ASM_ERR_MSG_L[] = {
    "",
    "Wrong label given",
    "Wrong command given",
    "Wrong argument given",
    "Too many agruments given",
    "Attempt to redefine the label",
    "Miss a label",
    "Such a label doesn't exist",
};

enum arg_def_ret_t
{
    arg_def_ret_OK          = 0,
    arg_def_ret_WRONG_ARG   = 1,
    arg_def_ret_TAG         = -1
};

//----------------------------------------------------------------------------------------------------------------

struct asm_data_t
{
    int         number_of_cmnds;
    int         pc;
    int*        label_buf;
    bool        on_write;
    filedata    inp_file_data;

    FILE*       inp_fl;
    FILE*       asm_lst_fl;
    FILE*       bin_fl;
    str_t       inpfl_nm, asm_lst_fl_nm, bin_fl_nm;
};

struct asm_err_t {

    asm_err_n errnum;
    int nln;
    str_t str, flnm;
};

//----------------------------------------------------------------------------------------------------------------

void errlog(asm_err_t error_t);

bool is_onlyspace(string str);

/// Skips no more then n first spaces, or until '\0' is found
/// If all n symbols are spaces, return 0, else return 1 and put in "str_ptr" the pointer on first non-space symbol.
int skipn_spaces(const char* str, const char** str_ptr, int n);

asm_err_t compile(asm_data_t* asm_data_ptr);

int asm_data_ctor(asm_data_t* asm_data_ptr);

int asm_data_dtor(asm_data_t* asm_data_ptr);

int write_header(asm_data_t asm_data);

//----------------------------------------------------------------------------------------------------------------

#define FORMAT_LINE "=========================================================================================================\n"

#define WRITE_LABEL_LIST    false
#define WRITE_DATA_IN_FILE  true

int main(int argc, char** argv) {

    asm_data_t asm_data = {.inpfl_nm = argv[1], .asm_lst_fl_nm = "asmlist.txt", .bin_fl_nm = "code.bin"};
    asm_data_ctor(&asm_data);

    if (asm_data.inp_fl == NULL) {

        fprintf(stderr, "No such file.\n");
        asm_data_dtor(&asm_data);
        return 1;
    }

    asm_data.on_write = WRITE_LABEL_LIST;
    asm_err_t compile_ok = compile(&asm_data);

    $ad(asm_data.label_buf, 10);
    $d(asm_data.pc);
    $d(asm_data.number_of_cmnds);

    if (!compile_ok.errnum) {

        write_header(asm_data);

        // asm_data.code_buf_sz = asm_data.pc;
        asm_data.pc = 0;
        asm_data.on_write = WRITE_DATA_IN_FILE;
        compile(&asm_data);
    }
    else {

        compile_ok.flnm = asm_data.inpfl_nm;
        compile_ok.str = asm_data.inp_file_data.prsdbffr[compile_ok.nln].str;
        $err(FAT RED "Error\n" DEF);
        errlog(compile_ok);
    }

    $ad(asm_data.label_buf, 10);

    asm_data_dtor(&asm_data);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

void errlog(asm_err_t err_t) {

    fprintf(stderr, FAT"%s:%d: " RED"Syntax error: " DEF"%s.\n", err_t.flnm, err_t.nln + 1, ASM_ERR_MSG_L[err_t.errnum]);
    fprintf(stderr, "%5d | %s\n%5s |\n", err_t.nln, err_t.str, "");
}

//----------------------------------------------------------------------------------------------------------------

bool is_onlyspace(string str) {

    for (int i = 0; i < str.len - 1; i++) {

        if (!isspace(str.str[i])) {

            return false;
        }
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------


//MENTORSKAYA MUDROST
//  счтал токен - скипнул нахуй (передвинул указатель дальше)
//  всё
//
//

/// Skips no more then n first spaces, or until '\0' is found
/// If all symbols are spaces, return 0, else return 1 and put in "str_ptr" the pointer on first non-space symbol.
int skipn_spaces(const char* str, const char** str_ptr, int n) {

    // fprintf(stderr, "str = |%s|\n", str);
    int i = 0;
    // $d(n);
    for (i = 0; i < n && str[i] == ' ' && str[i] != '\0' && str[i] != EOF; i++) {

    }

    if (i < n && str[i] != '\0' && str[i] != EOF) {

        if (str_ptr != NULL) {

            *str_ptr = str + i;
        }

        return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int read_one_command(char** current_str_ptr);
cmd_n command_handle(asm_data_t* asm_data_ptr, char** current_str_ptr);
int string_pointer_shift(char** current_str_ptr, int n_chr_shft);
asm_err_n command_as_label_handle(asm_data_t* asm_data_ptr, char** current_str_ptr);
asm_err_n argument_as_label_handle(asm_data_t* asm_data_ptr, char** current_str_ptr);
asm_err_n default_argument_handle(asm_data_t* asm_data_ptr, char** current_str_ptr);
int write_command(asm_data_t* asm_data_ptr, int argument, bool is_command);

#define ERROR   1
#define NOERROR 0

#define RETURN_ERROR(ERR) return (asm_err_t) {.errnum = ERR, .nln = nln}

#define RETURN_IF_END_EMPTY if (!is_onlyspace({.str = current_str, .len = (int) strlen(current_str) + 1})) RETURN_ERROR(asm_err_ARG_QTY_EXC); else continue;

asm_err_t compile(asm_data_t* asm_data_ptr) {


    int nln = 0;
    // $d(asm_data_ptr->inp_file_data.nlns);
    for (nln = 0; nln < asm_data_ptr->inp_file_data.nlns; nln++) {

        // $std("\n");
        char* current_str = asm_data_ptr->inp_file_data.prsdbffr[nln].str;

        if (is_onlyspace({.str = current_str, .len = (int) strlen(current_str) + 1})) continue;

        // $s(current_str);
        int command_number = command_handle(asm_data_ptr, &current_str); // передавать это в хандлеры
        // $d(command_number);

        if (command_number == cmd_n_ERROR) {

            RETURN_ERROR(asm_err_WRONG_CMD);
        }

        if (command_number == cmd_n_LABEL) {


            asm_err_n errnum = command_as_label_handle(asm_data_ptr, &current_str);
            if (errnum) RETURN_ERROR(errnum);

            RETURN_IF_END_EMPTY;
        }

        if (cmd_l[command_number].label) {

            asm_err_n errnum = argument_as_label_handle(asm_data_ptr, &current_str);
            if (errnum) RETURN_ERROR(errnum);
        }
        // $;
        if (cmd_l[command_number].argqty > 0) {
            // $;

            asm_err_n errnum = default_argument_handle(asm_data_ptr, &current_str);
            if (errnum) RETURN_ERROR(errnum);
        }
//
//         $s(current_str);
//
//         $d( is_onlyspace({.str = current_str, .len = (int) strlen(current_str) + 1}) );


        RETURN_IF_END_EMPTY;
    }

    RETURN_ERROR(asm_err_NO_ERR);
}

//----------------------------------------------------------------------------------------------------------------

#define CMD     true
#define NONCMD  false
#define PC_INCR asm_data_ptr->pc++;

//----------------------------------------------------------------------------------------------------------------

cmd_n command_handle(asm_data_t* asm_data_ptr, char** current_str_ptr) {

    int number_of_characters_read = read_one_command(current_str_ptr);
    // $d(number_of_characters_read);
    if (*current_str_ptr[0] == ':') {

        string_pointer_shift(current_str_ptr, 1);
        return cmd_n_LABEL;
    }

    int command_number_read = 0;

    for (command_number_read = 0;                                                                   \
        command_number_read < CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT - 2      \
        && strncmp(*current_str_ptr, cmd_l[command_number_read].name, number_of_characters_read);   \
        command_number_read++) {}

    string_pointer_shift(current_str_ptr, number_of_characters_read);

    if (command_number_read == CMD_N__TOUCH_ONLY_ON_ADDING_NEW_COMMANDS_AND_NO_EXCEPT_BLYAT - 2) {

        return cmd_n_ERROR;
    }

    PC_INCR;
    asm_data_ptr->number_of_cmnds ++;
    write_command(asm_data_ptr, cmd_l[command_number_read].num, CMD);
    return cmd_l[command_number_read].num;
}

//----------------------------------------------------------------------------------------------------------------

asm_err_n command_as_label_handle(asm_data_t* asm_data_ptr, char** current_str_ptr) {

    int number_of_characters_read = read_one_command(current_str_ptr);

    int label_number = -1;
    sscanf(*current_str_ptr, "%d", &label_number);
    string_pointer_shift(current_str_ptr, number_of_characters_read);

    if (asm_data_ptr->on_write) {

        return asm_err_NO_ERR;
    }

    if (label_number == -1) {

        return asm_err_WRONG_LABEL; //WRONG LABEL NAME
    }

    if (asm_data_ptr->label_buf[label_number] != -1) {

        return asm_err_LABEL_REDFNTN; //REDEFINITION OF LABEL
    }

    asm_data_ptr->label_buf[label_number] = asm_data_ptr->pc;

    return asm_err_NO_ERR;
}

//----------------------------------------------------------------------------------------------------------------

asm_err_n argument_as_label_handle(asm_data_t* asm_data_ptr, char** current_str_ptr) {

    int number_of_characters_read = read_one_command(current_str_ptr);

    if (*current_str_ptr[0] != ':') {

        return asm_err_MISS_LABEL; //MISS A LABEL
    }

    string_pointer_shift(current_str_ptr, 1);
    number_of_characters_read--;

    if (number_of_characters_read == 0) {

        number_of_characters_read = read_one_command(current_str_ptr);
    }

    int label_number = -1;
    sscanf(*current_str_ptr, "%d", &label_number);
    string_pointer_shift(current_str_ptr, number_of_characters_read);

    if (label_number == -1) {

        return asm_err_WRONG_LABEL; //WRONG LABEL NAME
    }

    if (!asm_data_ptr->on_write) { // NO CHECK LABEL IF LABEL ARR NOT WRITTEN (FIRST CODE ASM)

        asm_data_ptr->pc += ARGSZ;
        return asm_err_NO_ERR;
    }

    if (asm_data_ptr->label_buf[label_number] == -1) {

        return asm_err_LABEL_NOT_EXST; //NO SUCH LABEL
    }

    asm_data_ptr->pc += ARGSZ;
    write_command(asm_data_ptr, asm_data_ptr->label_buf[label_number], NONCMD);

    return asm_err_NO_ERR;
}

//----------------------------------------------------------------------------------------------------------------

asm_err_n default_argument_handle(asm_data_t* asm_data_ptr, char** current_str_ptr) {

    int number_of_characters_read = read_one_command(current_str_ptr);

    double argument = NAN;
    sscanf(*current_str_ptr, "%lg", &argument);
    string_pointer_shift(current_str_ptr, number_of_characters_read);

    if (isnan(argument)) {

        return asm_err_WRONG_ARG; //WRONG ARGUMENT VALUE
    }

    int led_to_int_argument = (int) (SPU_EXPANENT_COEFF * argument);

    asm_data_ptr->pc += ARGSZ;
    write_command(asm_data_ptr, led_to_int_argument, NONCMD);

    return asm_err_NO_ERR;

}

//----------------------------------------------------------------------------------------------------------------

int read_one_command(char** current_str_ptr) {

    skipn_spaces(*current_str_ptr, (const char**) current_str_ptr, (int) strlen(*current_str_ptr));

    int nread = 0;
    char* useless_str = (char*) calloc(10, sizeof(char));

    sscanf(*current_str_ptr, "%s%n", useless_str, &nread);

    free(useless_str);

    return nread;
}

//----------------------------------------------------------------------------------------------------------------

int write_command(asm_data_t* asm_data_ptr, int arg, bool is_command) {

    if (!asm_data_ptr->on_write) {

        return 0;
    }

    if (is_command) {

        char arr_to_write[1] = {(char) arg};
        fwrite(arr_to_write, sizeof(char), 1, asm_data_ptr->bin_fl);

        fprintf(asm_data_ptr->asm_lst_fl, "%16s| %8d | %#16.8x | %8s |\n", "", arg, (unsigned) asm_data_ptr->pc, cmd_l[arg].name);


        return 0;
    }

    int arr_to_write = arg;

    fwrite(&arr_to_write, sizeof(int), 1, asm_data_ptr->bin_fl);



    fprintf(asm_data_ptr->asm_lst_fl, "%16s| %8d | %#16.8x | %8s |\n", "", arg, (unsigned) asm_data_ptr->pc, "");

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

int string_pointer_shift(char** current_str_ptr, int n_chr_shft) {

    *current_str_ptr += n_chr_shft;
    return 0;
}

//----------------------------------------------------------------------------------------------------------------
//TODO СДЕЛАТЬ ОБРАБТКУ ОШИБОК
int asm_data_ctor(asm_data_t* asm_data_ptr) {

    asm_data_ptr->label_buf = (int*) calloc(MAX_LABEL_QTY, sizeof(int));

    for (int i = 0; i < MAX_LABEL_QTY; i++){

        asm_data_ptr->label_buf[i] = -1;
    }

    asm_data_ptr->inp_fl      = fopen(asm_data_ptr->inpfl_nm, "r");
    asm_data_ptr->asm_lst_fl  = fopen(asm_data_ptr->asm_lst_fl_nm, "w");
    asm_data_ptr->bin_fl      = fopen(asm_data_ptr->bin_fl_nm, "wb");

    asm_data_ptr->inp_file_data = {.fl = asm_data_ptr->inp_fl};
    rdfrmfl(&asm_data_ptr->inp_file_data);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------

//TODO СДЕЛАТЬ ОБРАБТКУ ОШИБОК
int asm_data_dtor(asm_data_t* asm_data_ptr) {

    free(asm_data_ptr->label_buf);

    filedatastrdestr(&asm_data_ptr->inp_file_data); //YASHA YEBANIY PIDOR DOLBOEB, NE UMEET CHITAT` SLOVA NA ANGLIYSKOM

    fprintf(asm_data_ptr->asm_lst_fl, FORMAT_LINE FORMAT_LINE);

    fclose(asm_data_ptr->inp_fl);
    fclose(asm_data_ptr->asm_lst_fl);
    fclose(asm_data_ptr->bin_fl);

    return 0;
}

//----------------------------------------------------------------------------------------------------------------
// TODO ебани 50 байт на 32 можно положить начало кода (сука какой же он ебень)
int write_header(asm_data_t asm_data) {

    fwrite(SIGNATURE___TOUCH_ONLY_IF_YOU_DONT_AFRAID_OF_THE_CONSEQUENSES, 8, 1, asm_data.bin_fl);

    int write_arr[] = {asm_data.number_of_cmnds, asm_data.pc};

    fwrite(write_arr, sizeof(int), sizeof(write_arr)/sizeof(int), asm_data.bin_fl);

    fprintf(asm_data.asm_lst_fl, FORMAT_LINE);
    fprintf(asm_data.asm_lst_fl, FORMAT_LINE);

    return 0;
}
