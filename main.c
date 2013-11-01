#include "zh2py.h"
int main(int argc,char ** argv){
    int result = 0;
    if ( argc < 2 ){
        printf("usage:%s {text_dict_file}",argv[0]);
        exit(0);
    }
    char * dict_path = argv[1];

    printf("read file:%s",dict_path);
    zh2py_res_table_root= zh2py_read_transform_table_file(dict_path);
    if(zh2py_res_table_root == NULL){
        printf("Cant load table file\n");
    }

    int l;
    int i=0;
    int code=0;
    int code_more=0;
    int index=0;

    char * final_result;
    FILE * fp = fopen("./gbkall","r");
    char buf[1024];
    while(fgets(buf,1024,fp)){
        l=strlen(buf);
        final_result = NULL;
        final_result = zh2py_transform(zh2py_res_table_root,(char *)buf);
        if(final_result==NULL){
            printf("Got an error");
        }
        printf("%s\n",final_result);
    }
    fclose(fp);
    zh2py_free_result(&final_result);
    zh2py_free_result(&final_result);
    int ret = zh2py_free_table(zh2py_res_table_root);
    printf("free_table return:%d\n",ret);
    return 0;
}

