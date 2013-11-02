#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


struct zh2py_s_table_row {
    int char_code;
    int py_index;
};
typedef struct zh2py_s_table_row zh2py_table_row;
struct zh2py_s_table_root {
    zh2py_table_row * row;
    int length;
};
typedef struct zh2py_s_table_root zh2py_table_root;
zh2py_table_root * zh2py_read_transform_table_file(char * table_path);
int zh2py_get_ascii(char ch);
int zh2py_free_table(zh2py_table_root * table_root);
char zh2py_pinyin_table[396][8]={"a","ai","an","ang","ao","ba","bai","ban","bang","bao","bei","ben","beng","bi","bian","biao","bie","bin","bing","bo","bu","ca","cai","can","cang","cao","ce","ceng","cha","chai","chan","chang","chao","che","chen","cheng","chi","chong","chou","chu","chuai","chuan","chuang","chui","chun","chuo","ci","cong","cou","cu","cuan","cui","cun","cuo","da","dai","dan","dang","dao","de","deng","di","dian","diao","die","ding","diu","dong","dou","du","duan","dui","dun","duo","e","en","er","fa","fan","fang","fei","fen","feng","fo","fou","fu","ga","gai","gan","gang","gao","ge","gei","gen","geng","gong","gou","gu","gua","guai","guan","guang","gui","gun","guo","ha","hai","han","hang","hao","he","hei","hen","heng","hong","hou","hu","hua","huai","huan","huang","hui","hun","huo","ji","jia","jian","jiang","jiao","jie","jin","jing","jiong","jiu","ju","juan","jue","jun","ka","kai","kan","kang","kao","ke","ken","keng","kong","kou","ku","kua","kuai","kuan","kuang","kui","kun","kuo","la","lai","lan","lang","lao","le","lei","leng","li","lia","lian","liang","liao","lie","lin","ling","liu","long","lou","lu","lv","luan","lue","lun","luo","ma","mai","man","mang","mao","me","mei","men","meng","mi","mian","miao","mie","min","ming","miu","mo","mou","mu","na","nai","nan","nang","nao","ne","nei","nen","neng","ni","nian","niang","niao","nie","nin","ning","niu","nong","nu","nv","nuan","nue","nuo","o","ou","pa","pai","pan","pang","pao","pei","pen","peng","pi","pian","piao","pie","pin","ping","po","pu","qi","qia","qian","qiang","qiao","qie","qin","qing","qiong","qiu","qu","quan","que","qun","ran","rang","rao","re","ren","reng","ri","rong","rou","ru","ruan","rui","run","ruo","sa","sai","san","sang","sao","se","sen","seng","sha","shai","shan","shang","shao","she","shen","sheng","shi","shou","shu","shua","shuai","shuan","shuang","shui","shun","shuo","si","song","sou","su","suan","sui","sun","suo","ta","tai","tan","tang","tao","te","teng","ti","tian","tiao","tie","ting","tong","tou","tu","tuan","tui","tun","tuo","wa","wai","wan","wang","wei","wen","weng","wo","wu","xi","xia","xian","xiang","xiao","xie","xin","xing","xiong","xiu","xu","xuan","xue","xun","ya","yan","yang","yao","ye","yi","yin","ying","yo","yong","you","yu","yuan","yue","yun","za","zai","zan","zang","zao","ze","zei","zen","zeng","zha","zhai","zhan","zhang","zhao","zhe","zhen","zheng","zhi","zhong","zhou","zhu","zhua","zhuai","zhuan","zhuang","zhui","zhun","zhuo","zi","zong","zou","zu","zuan","zui","zun","zuo"};
zh2py_table_root * zh2py_res_table_root=NULL;
char * zh2py_last_result=NULL;
char * zh2py_transform(zh2py_table_root * table_root, char * buf,int * result_len){
    int i=0;
    int l=0;
    int pos=0;
    int index,code,code_more;
    l = strlen(buf);
    zh2py_table_row * target;
    char * result = (char * )malloc(sizeof(char)*l*8);
    if(result == NULL){
        return NULL;
    }
    bzero(result,sizeof(char)*l*8);
    while(i<l){
        index = code = zh2py_get_ascii(*(buf+i));
        if(code>160){
            i++;
            code_more = zh2py_get_ascii(*(buf+i));
            index = code * 256 + code_more - 45217;
            if(index>table_root->length){
            }else{
                target = table_root->row+index;
                strcpy(result+pos,zh2py_pinyin_table[target->py_index]);
                pos += strlen(zh2py_pinyin_table[target->py_index]);
                *(result+pos)=' ';
                pos ++;
            }
        }else{
            if(code>0 && code<160){
                *(result+pos)  =  buf[i];
                pos++;
                *(result+pos) = ' ';
                pos ++;
            }
        }
        i++;
    }
    *result_len = pos;
    return result;
}
void zh2py_free_result(char ** result){
    if(*result!=NULL){
        free(*result);
        *result = NULL;
    }
}
int zh2py_get_ascii(char ch){
    int i = (int) ch;
    if(i>=0)
        return i;
    else
        return 256+i;
}
int zh2py_free_table(zh2py_table_root * table_root){
    if(munmap(table_root->row,table_root->length)==-1){
        return -1;
    }
    free(table_root); 
    return 0;
}
int zh2py_table_loaded(){
    if(zh2py_res_table_root ==NULL){
        return 0;
    }else{
        return 1;
    }
}
zh2py_table_root * zh2py_read_transform_table_file(char * table_path){
    zh2py_table_row * row;
    zh2py_table_root * root;
    struct stat buf;
    int fd = open(table_path, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    if(fd==-1){
        return NULL;
    }
    fstat(fd,&buf);
    int length = buf.st_size/sizeof(zh2py_table_row);
    row = (zh2py_table_row *)mmap(0,buf.st_size,PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if(row==MAP_FAILED){
        return NULL;
    }
    if(close(fd)==-1){
        return NULL;
    }

    root = malloc(sizeof(zh2py_table_root));
    root->length = length;
    root->row = row;
    return root;
}
